#[cfg(feature = "symstore")]
pub mod pdb_struct;
#[cfg(feature = "symstore")]
pub use pdb_struct::PdbStruct;

#[cfg(feature = "symstore")]
pub mod symstore;
#[cfg(feature = "symstore")]
pub use symstore::*;

use std::prelude::v1::*;

use std::fs::File;
use std::io::Read;
use std::path::Path;

use crate::error::{Error, Result};
use crate::kernel_info::KernelInfo;

#[derive(Debug, Clone)]
pub struct Win32GUID {
    pub file_name: String,
    pub guid: String,
}

#[derive(Debug, Clone)]
pub struct Win32Offsets {
    pub list_blink: usize,
    pub eproc_link: usize,

    pub kproc_dtb: usize,
    pub eproc_pid: usize,
    pub eproc_name: usize,
    pub eproc_peb: usize,
    pub eproc_thread_list: usize,
    pub eproc_wow64: usize,

    pub kthread_teb: usize,
    pub ethread_list_entry: usize,
    pub teb_peb: usize,
    pub teb_peb_x86: usize,

    pub peb_ldr_x86: usize,
    pub peb_ldr_x64: usize,
    pub ldr_list_x86: usize,
    pub ldr_list_x64: usize,

    pub ldr_data_base_x86: usize,
    pub ldr_data_base_x64: usize,
    pub ldr_data_size_x86: usize,
    pub ldr_data_size_x64: usize,
    pub ldr_data_name_x86: usize,
    pub ldr_data_name_x64: usize,
}

impl Win32Offsets {
    #[cfg(feature = "symstore")]
    pub fn try_with_guid(guid: &Win32GUID) -> Result<Self> {
        let symstore = SymbolStore::default();
        let pdb = symstore.load(guid)?;
        Self::try_with_pdb_slice(&pdb[..])
    }

    #[cfg(feature = "symstore")]
    pub fn try_with_kernel_info(info: &KernelInfo) -> Result<Self> {
        if let Some(guid) = info.kernel_guid.as_ref() {
            Self::try_with_guid(guid)
        } else {
            Err(Error::Other(
                "Win32Offsets::try_with_kernel_info: KernelInfo does not have a guid",
            ))
        }
    }

    #[cfg(feature = "symstore")]
    pub fn try_with_pdb<P: AsRef<Path>>(pdb_path: P) -> Result<Self> {
        let mut file = File::open(pdb_path)
            .map_err(|_| Error::PDB("unable to open user-supplied pdb file"))?;
        let mut buffer = Vec::new();
        file.read_to_end(&mut buffer)
            .map_err(|_| Error::PDB("unable to read user-supplied pdb file"))?;
        Self::try_with_pdb_slice(&buffer[..])
    }

    #[cfg(feature = "symstore")]
    pub fn try_with_pdb_slice(pdb_slice: &[u8]) -> Result<Self> {
        let list = PdbStruct::with(pdb_slice, "_LIST_ENTRY")
            .map_err(|_| Error::PDB("_LIST_ENTRY not found"))?;
        let kproc = PdbStruct::with(pdb_slice, "_KPROCESS")
            .map_err(|_| Error::PDB("_KPROCESS not found"))?;
        let eproc = PdbStruct::with(pdb_slice, "_EPROCESS")
            .map_err(|_| Error::PDB("_EPROCESS not found"))?;
        let ethread =
            PdbStruct::with(pdb_slice, "_ETHREAD").map_err(|_| Error::PDB("_ETHREAD not found"))?;
        let kthread =
            PdbStruct::with(pdb_slice, "_KTHREAD").map_err(|_| Error::PDB("_KTHREAD not found"))?;
        let teb = PdbStruct::with(pdb_slice, "_TEB").map_err(|_| Error::PDB("_TEB not found"))?;

        let list_blink = list
            .find_field("Blink")
            .ok_or_else(|| Error::PDB("_LIST_ENTRY::Blink not found"))?
            .offset;

        let eproc_link = eproc
            .find_field("ActiveProcessLinks")
            .ok_or_else(|| Error::PDB("_EPROCESS::ActiveProcessLinks not found"))?
            .offset;

        let kproc_dtb = kproc
            .find_field("DirectoryTableBase")
            .ok_or_else(|| Error::PDB("_KPROCESS::DirectoryTableBase not found"))?
            .offset;
        let eproc_pid = eproc
            .find_field("UniqueProcessId")
            .ok_or_else(|| Error::PDB("_EPROCESS::UniqueProcessId not found"))?
            .offset;
        let eproc_name = eproc
            .find_field("ImageFileName")
            .ok_or_else(|| Error::PDB("_EPROCESS::ImageFileName not found"))?
            .offset;
        let eproc_peb = eproc
            .find_field("Peb")
            .ok_or_else(|| Error::PDB("_EPROCESS::Peb not found"))?
            .offset;
        let eproc_thread_list = eproc
            .find_field("ThreadListHead")
            .ok_or_else(|| Error::PDB("_EPROCESS::ThreadListHead not found"))?
            .offset;

        // windows 10 uses an uppercase W whereas older windows versions (windows 7) uses a lowercase w
        let eproc_wow64 = match eproc
            .find_field("WoW64Process")
            .or_else(|| eproc.find_field("Wow64Process"))
        {
            Some(f) => f.offset,
            None => 0,
        };

        // threads
        let kthread_teb = kthread
            .find_field("Teb")
            .ok_or_else(|| Error::PDB("_KTHREAD::Teb not found"))?
            .offset;
        let ethread_list_entry = ethread
            .find_field("ThreadListEntry")
            .ok_or_else(|| Error::PDB("_ETHREAD::ThreadListEntry not found"))?
            .offset;
        let teb_peb = teb
            .find_field("ProcessEnvironmentBlock")
            .ok_or_else(|| Error::PDB("_TEB::ProcessEnvironmentBlock not found"))?
            .offset;
        let teb_peb_x86 = if let Ok(teb32) =
            PdbStruct::with(pdb_slice, "_TEB32").map_err(|_| Error::PDB("_TEB32 not found"))
        {
            teb32
                .find_field("ProcessEnvironmentBlock")
                .ok_or_else(|| Error::PDB("_TEB32::ProcessEnvironmentBlock not found"))?
                .offset
        } else {
            0
        };

        Ok(Self {
            list_blink,
            eproc_link,

            kproc_dtb,

            eproc_pid,
            eproc_name,
            eproc_peb,
            eproc_thread_list,
            eproc_wow64,

            kthread_teb,
            ethread_list_entry,
            teb_peb,
            teb_peb_x86,

            peb_ldr_x86: 0xC,   // _PEB::Ldr
            peb_ldr_x64: 0x18,  // _PEB::Ldr
            ldr_list_x86: 0xC,  // _PEB_LDR_DATA::InLoadOrderModuleList
            ldr_list_x64: 0x10, // _PEB_LDR_DATA::InLoadOrderModuleList

            ldr_data_base_x86: 0x18, // _LDR_DATA_TABLE_ENTRY::DllBase
            ldr_data_base_x64: 0x30, // _LDR_DATA_TABLE_ENTRY::DllBase
            ldr_data_size_x86: 0x20, // _LDR_DATA_TABLE_ENTRY::SizeOfImage
            ldr_data_size_x64: 0x40, // _LDR_DATA_TABLE_ENTRY::SizeOfImage
            ldr_data_name_x86: 0x2C, // _LDR_DATA_TABLE_ENTRY::BaseDllName
            ldr_data_name_x64: 0x58, // _LDR_DATA_TABLE_ENTRY::BaseDllName
        })
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn download_pdb() {
        let offsets = Win32Offsets::try_with_guid(&Win32GUID {
            file_name: "ntkrnlmp.pdb".to_string(),
            guid: "3844DBB920174967BE7AA4A2C20430FA2".to_string(),
        })
        .unwrap();

        assert_eq!(offsets.list_blink, 8);
        assert_eq!(offsets.eproc_link, 392);

        assert_eq!(offsets.kproc_dtb, 40);

        assert_eq!(offsets.eproc_pid, 384);
        assert_eq!(offsets.eproc_name, 736);
        assert_eq!(offsets.eproc_peb, 824);
        assert_eq!(offsets.eproc_thread_list, 776);
        assert_eq!(offsets.eproc_wow64, 800);

        assert_eq!(offsets.kthread_teb, 184);
        assert_eq!(offsets.ethread_list_entry, 1056);
        assert_eq!(offsets.teb_peb, 96);
        assert_eq!(offsets.teb_peb_x86, 48);
    }
}
