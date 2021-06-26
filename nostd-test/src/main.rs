#![no_std]
#![no_main]
#![feature(abi_efiapi)]
use core::*;
use uefi::prelude::*;

#[allow(unused)]
#[macro_use]
extern crate alloc;

extern crate rlibc;

use log::*;

use uefi::{Handle, Status};

#[entry]
fn efi_main(_handle: Handle, st: SystemTable<Boot>) -> Status {
    uefi_services::init(&st).expect_success("Failed to initialize utilities");

    info!("memflow EFI test");

    let _bt = st.boot_services();

    Status::SUCCESS
}
