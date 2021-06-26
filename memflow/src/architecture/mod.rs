/*!
Module for handling different architectures in memflow.

Each architecture implements the `Architecture` trait
and all function calls are dispatched into their own
architecture specific sub-modules.

Virtual address translations are done using `VirtualTranslate3`
trait, which is linked to a particular architecture.

Each architecture also has a `ByteOrder` assigned to it.
When reading/writing data from/to the target it is necessary
that memflow know the proper byte order of the target system.
*/

pub mod arm;
pub mod x86;
#[macro_use]
mod mmu;

pub(crate) use mmu::ArchMmuDef;

use crate::error::{Error, Result};
use crate::iter::{FnExtend, SplitAtIndex};
use crate::mem::PhysicalMemory;
use crate::types::size;

use crate::types::{Address, PhysicalAddress};

/// Identifies the byte order of a architecture
///
/// This enum is used when reading/writing to/from the memory of a target system.
/// The memory will be automatically converted to the endianess memflow is currently running on.
///
/// See the [wikipedia article](https://en.wikipedia.org/wiki/Endianness) for more information on the subject.
#[derive(Debug, Clone, Copy, Eq, PartialEq)]
#[cfg_attr(feature = "serde", derive(::serde::Serialize))]
#[repr(u8)]
pub enum Endianess {
    /// Little Endianess
    LittleEndian,
    /// Big Endianess
    BigEndian,
}

/// Translates virtual memory to physical using internal translation base (usually a process' dtb)
///
/// This trait abstracts virtual address translation for a single virtual memory scope.
/// On x86 architectures, it is a single `Address` - a CR3 register. But other architectures may
/// use multiple translation bases, or use a completely different translation mechanism (MIPS).
pub trait VirtualTranslate3: Clone + Copy + Send {
    /// Translate a single virtual address
    ///
    /// # Examples
    /// ```
    /// # use memflow::error::Result;
    /// # use memflow::types::{PhysicalAddress, Address};
    /// # use memflow::dummy::{DummyMemory, DummyOs};
    /// use memflow::architecture::VirtualTranslate3;
    /// use memflow::architecture::x86::x64;
    /// use memflow::types::size;
    ///
    /// # const VIRT_MEM_SIZE: usize = size::mb(8);
    /// # const CHUNK_SIZE: usize = 2;
    /// #
    /// # let mem = DummyMemory::new(size::mb(16));
    /// # let mut os = DummyOs::new(mem);
    /// # let (dtb, virtual_base) = os.alloc_dtb(VIRT_MEM_SIZE, &[]);
    /// # let mut mem = os.into_inner();
    /// # let translator = x64::new_translator(dtb);
    /// let arch = x64::ARCH;
    ///
    /// // Translate a mapped address
    /// let res = translator.virt_to_phys(
    ///     &mut mem,
    ///     virtual_base,
    /// );
    ///
    /// assert!(res.is_ok());
    ///
    /// // Translate unmapped address
    /// let res = translator.virt_to_phys(
    ///     &mut mem,
    ///     virtual_base - 1,
    /// );
    ///
    /// assert!(res.is_err());
    ///
    /// ```
    fn virt_to_phys<T: PhysicalMemory>(
        &self,
        mem: &mut T,
        addr: Address,
    ) -> Result<PhysicalAddress> {
        let mut buf: [std::mem::MaybeUninit<u8>; 512] =
            unsafe { std::mem::MaybeUninit::uninit().assume_init() };
        let mut output = None;
        let mut success = FnExtend::new(|elem: (PhysicalAddress, _)| {
            if output.is_none() {
                output = Some(elem.0);
            }
        });
        let mut output_err = None;
        let mut fail = FnExtend::new(|elem: (Error, _, _)| output_err = Some(elem.0));
        self.virt_to_phys_iter(
            mem,
            Some((addr, 1)).into_iter(),
            &mut success,
            &mut fail,
            &mut buf,
        );
        output.map(Ok).unwrap_or_else(|| Err(output_err.unwrap()))
    }

    fn virt_to_phys_iter<
        T: PhysicalMemory + ?Sized,
        B: SplitAtIndex,
        VI: Iterator<Item = (Address, B)>,
        VO: Extend<(PhysicalAddress, B)>,
        FO: Extend<(Error, Address, B)>,
    >(
        &self,
        mem: &mut T,
        addrs: VI,
        out: &mut VO,
        out_fail: &mut FO,
        tmp_buf: &mut [std::mem::MaybeUninit<u8>],
    );

    fn translation_table_id(&self, address: Address) -> usize;

    fn arch(&self) -> ArchitectureObj;
}

pub trait Architecture: Send + Sync + 'static {
    /// Returns the number of bits of a pointers width on a `Architecture`.
    /// Currently this will either return 64 or 32 depending on the pointer width of the target.
    /// This function is handy in cases where you only want to know the pointer width of the target\
    /// but you don't want to match against all architecture.
    ///
    /// # Examples
    ///
    /// ```
    /// use memflow::architecture::x86::x32_pae;
    ///
    /// let arch = x32_pae::ARCH;
    /// assert_eq!(arch.bits(), 32);
    /// ```
    fn bits(&self) -> u8;

    /// Returns the byte order of an `Architecture`.
    /// This will either be `Endianess::LittleEndian` or `Endianess::BigEndian`.
    ///
    /// In most circumstances this will be `Endianess::LittleEndian` on all x86 and arm architectures.
    ///
    /// # Examples
    ///
    /// ```
    /// use memflow::architecture::{x86::x32, Endianess};
    ///
    /// let arch = x32::ARCH;
    /// assert_eq!(arch.endianess(), Endianess::LittleEndian);
    /// ```
    fn endianess(&self) -> Endianess;

    /// Returns the smallest page size of an `Architecture`.
    ///
    /// In x86/64 and arm this will always return 4kb.
    ///
    /// # Examples
    ///
    /// ```
    /// use memflow::architecture::x86::x64;
    /// use memflow::types::size;
    ///
    /// let arch = x64::ARCH;
    /// assert_eq!(arch.page_size(), size::kb(4));
    /// ```
    fn page_size(&self) -> usize;

    /// Returns the `usize` of a pointers width on a `Architecture`.
    ///
    /// This function will return the pointer width as a `usize` value.
    /// See `Architecture::bits()` for more information.
    ///
    /// # Examples
    ///
    /// ```
    /// use memflow::architecture::x86::x32;
    ///
    /// let arch = x32::ARCH;
    /// assert_eq!(arch.size_addr(), 4);
    /// ```
    fn size_addr(&self) -> usize;

    /// Returns the address space range in bits for the `Architecture`.
    ///
    /// # Examples
    ///
    /// ```
    /// use memflow::architecture::x86::x32_pae;
    ///
    /// let arch = x32_pae::ARCH;
    /// assert_eq!(arch.address_space_bits(), 36);
    ///
    /// ```
    fn address_space_bits(&self) -> u8;

    /// Returns a FFI-safe identifier
    fn ident(&self) -> ArchitectureIdent;
}

impl std::fmt::Debug for ArchitectureObj {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.debug_struct("ArchitectureObj")
            .field("bits", &self.bits())
            .field("endianess", &self.endianess())
            .field("page_size", &self.page_size())
            .field("size_addr", &self.size_addr())
            .field("address_space_bits", &self.address_space_bits())
            .finish()
    }
}

pub type ArchitectureObj = &'static dyn Architecture;

impl std::cmp::PartialEq<ArchitectureObj> for ArchitectureObj {
    // This lint doesn't make any sense in our usecase, since we never leak underlying Architecture
    // definitions, and each ARCH is a static trait object with a consistent address.
    #[allow(clippy::vtable_address_comparisons)]
    fn eq(&self, other: &ArchitectureObj) -> bool {
        std::ptr::eq(*self, *other)
    }
}

#[repr(C)]
#[derive(Debug, Copy, Clone, Eq, PartialEq)]
#[cfg_attr(feature = "serde", derive(::serde::Serialize, ::serde::Deserialize))]
pub enum ArchitectureIdent {
    /// Unknown architecture. Could be third-party implemented. memflow knows how to work on them,
    /// but is unable to instantiate them.
    Unknown,
    /// X86 with specified bitness and address extensions
    ///
    /// First argument - `bitness` controls whether it's 32, or 64 bit variant.
    /// Second argument - `address_extensions` control whether address extensions are
    /// enabled (PAE on x32, or LA57 on x64). Warning: LA57 is currently unsupported.
    X86(u8, bool),
    /// Arm 64-bit architecture with specified page size
    ///
    /// Valid page sizes are 4kb, 16kb, 64kb. Only 4kb is supported at the moment
    AArch64(usize),
}

impl std::fmt::Display for ArchitectureIdent {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            ArchitectureIdent::X86(32, false) => f.pad("x86_32"),
            ArchitectureIdent::X86(32, true) => f.pad("x86_32 PAE"),
            ArchitectureIdent::X86(64, false) => f.pad("x86_64"),
            ArchitectureIdent::X86(64, true) => f.pad("x86_64 LA57"),
            ArchitectureIdent::X86(_, _) => f.pad("x86"),
            ArchitectureIdent::AArch64(_) => f.pad("AArch64"),
            ArchitectureIdent::Unknown => f.pad("Unknown"),
        }
    }
}

impl ArchitectureIdent {
    pub fn into_obj(self) -> ArchitectureObj {
        self.into()
    }
}

impl From<ArchitectureIdent> for ArchitectureObj {
    fn from(arch: ArchitectureIdent) -> ArchitectureObj {
        const KB4: usize = size::kb(4);
        match arch {
            ArchitectureIdent::X86(32, false) => x86::x32::ARCH,
            ArchitectureIdent::X86(32, true) => x86::x32_pae::ARCH,
            ArchitectureIdent::X86(64, false) => x86::x64::ARCH,
            ArchitectureIdent::AArch64(KB4) => arm::aarch64::ARCH,
            _ => panic!("unsupported architecture! {:?}", arch),
        }
    }
}

#[cfg(feature = "serde")]
impl serde::Serialize for ArchitectureObj {
    fn serialize<S>(&self, serializer: S) -> core::result::Result<S::Ok, S::Error>
    where
        S: serde::Serializer,
    {
        use serde::ser::SerializeStruct;

        let mut state = serializer.serialize_struct("ArchitectureObj", 5)?;
        state.serialize_field("bits", &self.bits())?;
        state.serialize_field("endianess", &self.endianess())?;
        state.serialize_field("page_size", &self.page_size())?;
        state.serialize_field("size_addr", &self.size_addr())?;
        state.serialize_field("address_space_bits", &self.address_space_bits())?;
        state.end()
    }
}
