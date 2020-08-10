#[cfg(feature = "symstore")]
pub mod pdb_struct;
#[cfg(feature = "symstore")]
pub mod symstore;

pub mod offset_data;
#[doc(hidden)]
pub use offset_data::{Win32OffsetsData, Win32OffsetsFile};

#[cfg(feature = "symstore")]
pub use {pdb_struct::PdbStruct, symstore::*};

use dataview::Pod;

use std::prelude::v1::*;

use std::convert::TryFrom;
use std::fs::File;
use std::io::Read;
use std::path::Path;

use crate::error::{Error, Result};
use crate::kernel::{Win32GUID, Win32Version};
use crate::win32::KernelInfo;

pub mod x86 {
    pub const PEB_LDR: usize = 0xC; // _PEB::Ldr
    pub const LDR_LIST: usize = 0xC; // _PEB_LDR_DATA::InLoadOrderModuleList
    pub const LDR_DATA_BASE: usize = 0x18; // _LDR_DATA_TABLE_ENTRY::DllBase
    pub const LDR_DATA_SIZE: usize = 0x20; // _LDR_DATA_TABLE_ENTRY::SizeOfImage
    pub const LDR_DATA_NAME: usize = 0x2C; // _LDR_DATA_TABLE_ENTRY::BaseDllName
}

pub mod x64 {
    pub const PEB_LDR: usize = 0x18; // _PEB::Ldr
    pub const LDR_LIST: usize = 0x10; // _PEB_LDR_DATA::InLoadOrderModuleList
    pub const LDR_DATA_BASE: usize = 0x30; // _LDR_DATA_TABLE_ENTRY::DllBase
    pub const LDR_DATA_SIZE: usize = 0x40; // _LDR_DATA_TABLE_ENTRY::SizeOfImage
    pub const LDR_DATA_NAME: usize = 0x58; // _LDR_DATA_TABLE_ENTRY::BaseDllName
}

#[repr(transparent)]
#[derive(Debug, Clone)]
#[cfg_attr(feature = "serde", derive(::serde::Serialize))]
pub struct Win32Offsets(pub Win32OffsetsData);

impl From<Win32OffsetsData> for Win32Offsets {
    fn from(other: Win32OffsetsData) -> Self {
        Self { 0: other }
    }
}

impl From<Win32Offsets> for Win32OffsetsData {
    fn from(other: Win32Offsets) -> Self {
        other.0
    }
}

impl Win32Offsets {
    pub fn from_pdb<P: AsRef<Path>>(pdb_path: P) -> Result<Self> {
        let mut file = File::open(pdb_path)
            .map_err(|_| Error::PDB("unable to open user-supplied pdb file"))?;
        let mut buffer = Vec::new();
        file.read_to_end(&mut buffer)
            .map_err(|_| Error::PDB("unable to read user-supplied pdb file"))?;
        Self::from_pdb_slice(&buffer[..])
    }

    pub fn from_pdb_slice(pdb_slice: &[u8]) -> Result<Self> {
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
            .offset as _;

        let eproc_link = eproc
            .find_field("ActiveProcessLinks")
            .ok_or_else(|| Error::PDB("_EPROCESS::ActiveProcessLinks not found"))?
            .offset as _;

        let kproc_dtb = kproc
            .find_field("DirectoryTableBase")
            .ok_or_else(|| Error::PDB("_KPROCESS::DirectoryTableBase not found"))?
            .offset as _;
        let eproc_pid = eproc
            .find_field("UniqueProcessId")
            .ok_or_else(|| Error::PDB("_EPROCESS::UniqueProcessId not found"))?
            .offset as _;
        let eproc_name = eproc
            .find_field("ImageFileName")
            .ok_or_else(|| Error::PDB("_EPROCESS::ImageFileName not found"))?
            .offset as _;
        let eproc_peb = eproc
            .find_field("Peb")
            .ok_or_else(|| Error::PDB("_EPROCESS::Peb not found"))?
            .offset as _;
        let eproc_thread_list = eproc
            .find_field("ThreadListHead")
            .ok_or_else(|| Error::PDB("_EPROCESS::ThreadListHead not found"))?
            .offset as _;

        // windows 10 uses an uppercase W whereas older windows versions (windows 7) uses a lowercase w
        let eproc_wow64 = match eproc
            .find_field("WoW64Process")
            .or_else(|| eproc.find_field("Wow64Process"))
        {
            Some(f) => f.offset as _,
            None => 0,
        };

        // threads
        let kthread_teb = kthread
            .find_field("Teb")
            .ok_or_else(|| Error::PDB("_KTHREAD::Teb not found"))?
            .offset as _;
        let ethread_list_entry = ethread
            .find_field("ThreadListEntry")
            .ok_or_else(|| Error::PDB("_ETHREAD::ThreadListEntry not found"))?
            .offset as _;
        let teb_peb = teb
            .find_field("ProcessEnvironmentBlock")
            .ok_or_else(|| Error::PDB("_TEB::ProcessEnvironmentBlock not found"))?
            .offset as _;
        let teb_peb_x86 = if let Ok(teb32) =
            PdbStruct::with(pdb_slice, "_TEB32").map_err(|_| Error::PDB("_TEB32 not found"))
        {
            teb32
                .find_field("ProcessEnvironmentBlock")
                .ok_or_else(|| Error::PDB("_TEB32::ProcessEnvironmentBlock not found"))?
                .offset as _
        } else {
            0
        };

        Ok(Self {
            0: Win32OffsetsData {
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
            },
        })
    }

    pub fn builder() -> Win32OffsetBuilder {
        Win32OffsetBuilder::default()
    }
}

pub struct Win32OffsetBuilder {
    #[cfg(feature = "symstore")]
    symbol_store: Option<SymbolStore>,

    guid: Option<Win32GUID>,
    winver: Option<Win32Version>,
}

impl Default for Win32OffsetBuilder {
    fn default() -> Self {
        Self {
            #[cfg(feature = "symstore")]
            symbol_store: Some(SymbolStore::default()),

            guid: None,
            winver: None,
        }
    }
}

impl Win32OffsetBuilder {
    pub fn new() -> Self {
        Self::default()
    }

    pub fn build(self) -> Result<Win32Offsets> {
        if self.guid.is_none() && self.winver.is_none() {
            return Err(Error::Other(
                "building win32 offsets requires either a guid or winver",
            ));
        }
        // try to build via symbol store
        if let Ok(offs) = self.build_with_symbol_store() {
            return Ok(offs);
        }

        // use static offset list
        if let Ok(offs) = self.build_with_offset_list() {
            return Ok(offs);
        }

        Err(Error::Other("not found"))
    }

    fn build_with_offset_list(&self) -> Result<Win32Offsets> {
        let bytes = &include_bytes!(concat!(env!("OUT_DIR"), "/win32_offsets.bin"))[..];

        let aligned_size = std::mem::size_of::<Win32OffsetsFile>();

        // Try matching exact guid
        if let Some(target_guid) = &self.guid {
            for offsets in bytes.chunks_exact(aligned_size) {
                let offsets = offsets.as_data_view().copy::<Win32OffsetsFile>(0);
                if let (Ok(file), Ok(guid)) = (
                    <&str>::try_from(&offsets.pdb_file_name),
                    <&str>::try_from(&offsets.pdb_guid),
                ) {
                    if target_guid.file_name == file && target_guid.guid == guid {
                        return Ok(Win32Offsets {
                            0: offsets.offsets.clone(),
                        });
                    }
                }
            }
        }

        let mut closest_match = None;
        let mut prev_build_number = 0;

        // Try matching the newest build from that version that is not actually newer
        if let Some(winver) = &self.winver {
            for offsets in bytes.chunks_exact(aligned_size) {
                let offsets = offsets.as_data_view().copy::<Win32OffsetsFile>(0);
                if winver.major_version() == offsets.nt_major_version
                    && winver.minor_version() == offsets.nt_minor_version
                    && winver.build_number() >= offsets.nt_build_number
                    && prev_build_number <= offsets.nt_build_number
                {
                    prev_build_number = offsets.nt_build_number;
                    closest_match = Some(Win32Offsets {
                        0: offsets.offsets.clone(),
                    });
                }
            }

            if prev_build_number != winver.build_number() {
                log::warn!(
                    "no exact build number ({}) found! Closest match: {}",
                    winver.build_number(),
                    prev_build_number
                );
            }
        }

        closest_match.ok_or(Error::Other("not found"))
    }

    #[cfg(feature = "symstore")]
    fn build_with_symbol_store(&self) -> Result<Win32Offsets> {
        if let Some(store) = &self.symbol_store {
            if self.guid.is_some() {
                let pdb = store.load(self.guid.as_ref().unwrap())?;
                Win32Offsets::from_pdb_slice(&pdb[..])
            } else {
                Err(Error::Other("symbol store can only be used with a guid"))
            }
        } else {
            Err(Error::Other("symbol store is disabled"))
        }
    }

    #[cfg(not(feature = "symstore"))]
    fn build_with_symbol_store(&self) -> Result<Win32Offsets> {
        Err(Error::Other(
            "symbol store is deactivated via a compilation feature",
        ))
    }

    pub fn symbol_store(mut self, symbol_store: SymbolStore) -> Self {
        self.symbol_store = Some(symbol_store);
        self
    }

    pub fn no_symbol_store(mut self) -> Self {
        self.symbol_store = None;
        self
    }

    pub fn guid(mut self, guid: Win32GUID) -> Self {
        self.guid = Some(guid);
        self
    }

    pub fn winver(mut self, winver: Win32Version) -> Self {
        self.winver = Some(winver);
        self
    }

    pub fn kernel_info(mut self, kernel_info: &KernelInfo) -> Self {
        if self.guid.is_none() {
            self.guid = kernel_info.kernel_guid.clone();
        }
        if self.winver.is_none() {
            self.winver = kernel_info.kernel_winver.clone();
        }
        self
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn download_pdb() {
        // TODO: symbol store with no local cache

        let guid = Win32GUID {
            file_name: "ntkrnlmp.pdb".to_string(),
            guid: "3844DBB920174967BE7AA4A2C20430FA2".to_string(),
        };
        let offsets = Win32Offsets::builder()
            .symbol_store(SymbolStore::new())
            .guid(guid)
            .build()
            .unwrap();

        assert_eq!(offsets.0.list_blink, 8);
        assert_eq!(offsets.0.eproc_link, 392);

        assert_eq!(offsets.0.kproc_dtb, 40);

        assert_eq!(offsets.0.eproc_pid, 384);
        assert_eq!(offsets.0.eproc_name, 736);
        assert_eq!(offsets.0.eproc_peb, 824);
        assert_eq!(offsets.0.eproc_thread_list, 776);
        assert_eq!(offsets.0.eproc_wow64, 800);

        assert_eq!(offsets.0.kthread_teb, 184);
        assert_eq!(offsets.0.ethread_list_entry, 1056);
        assert_eq!(offsets.0.teb_peb, 96);
        assert_eq!(offsets.0.teb_peb_x86, 48);
    }
}
