extern crate memflow_bench;
use memflow_bench::{phys, vat, virt};

use criterion::*;

use memflow::error::{Error, Result};
use memflow::prelude::v1::*;
use memflow_win32::prelude::v1::*;
use memflow_win32::win32::Win32Kernel;

use rand::prelude::*;
use rand::{Rng, SeedableRng};
use rand_xorshift::XorShiftRng as CurRng;

fn create_connector(args: &Args) -> Result<impl PhysicalMemory + Clone> {
    // this workaround is to prevent loaded libraries
    // from spitting out to much log information and skewing benchmarks
    let filter = log::max_level();
    log::set_max_level(log::Level::Error.to_level_filter());

    let result = Inventory::scan().create_connector("qemu_procfs", None, args)?;

    log::set_max_level(filter);
    Ok(result)
}

fn initialize_virt_ctx() -> Result<(
    impl PhysicalMemory,
    DirectTranslate,
    ProcessInfo,
    impl VirtualTranslate3,
    ModuleInfo,
)> {
    let mut phys_mem = create_connector(&Args::new())?;

    let kernel_info = Win32KernelInfo::scanner(phys_mem.forward_mut())
        .scan()
        .map_err(|_| {
            Error(ErrorOrigin::Other, ErrorKind::NotFound).log_error("unable to find kernel")
        })?;
    let vat = DirectTranslate::new();
    let offsets = Win32Offsets::builder()
        .kernel_info(&kernel_info)
        .build()
        .map_err(|_| {
            Error(ErrorOrigin::Other, ErrorKind::NotFound)
                .log_error("unable to initialize win32 offsets with guid")
        })?;

    let mut kernel = Win32Kernel::new(phys_mem.clone(), vat.clone(), offsets, kernel_info);

    let mut rng = CurRng::from_rng(thread_rng()).unwrap();

    let proc_list = kernel.process_info_list().map_err(|_| {
        Error(ErrorOrigin::Other, ErrorKind::NotFound).log_error("unable to read process list")
    })?;
    for i in -100..(proc_list.len() as isize) {
        let idx = if i >= 0 {
            i as usize
        } else {
            rng.gen_range(0..proc_list.len())
        };

        let mod_list: Vec<ModuleInfo> = {
            let mut prc = kernel.process_by_info(proc_list[idx].clone())?;
            prc.module_list()
                .unwrap_or_default()
                .into_iter()
                .filter(|module| module.size > 0x1000)
                .collect()
        };

        if !mod_list.is_empty() {
            let tmod = &mod_list[rng.gen_range(0..mod_list.len())];
            let proc = proc_list[idx].clone();
            let translator = kernel
                .process_info_from_base_info(proc.clone())?
                .translator();
            return Ok((phys_mem, vat, proc, translator, tmod.clone())); // TODO: remove clone of mem + vat
        }
    }

    Err(Error(ErrorOrigin::Other, ErrorKind::ModuleNotFound))
}

fn win32_read_group(c: &mut Criterion) {
    virt::seq_read(c, "win32", &initialize_virt_ctx);
    virt::chunk_read(c, "win32", &initialize_virt_ctx);
    phys::seq_read(c, "win32", &|| create_connector(&Args::new()));
    phys::chunk_read(c, "win32", &|| create_connector(&Args::new()));
    vat::chunk_vat(c, "win32", &initialize_virt_ctx);
}

criterion_group! {
    name = win32_read;
    config = Criterion::default()
        .warm_up_time(std::time::Duration::from_millis(500))
        .measurement_time(std::time::Duration::from_millis(5000));
    targets = win32_read_group
}

criterion_main!(win32_read);
