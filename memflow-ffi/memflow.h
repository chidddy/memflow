#ifndef MEMFLOW_H
#define MEMFLOW_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
typedef void *Library;

/**
 * Identifies the byte order of a architecture
 *
 * This enum is used when reading/writing to/from the memory of a target system.
 * The memory will be automatically converted to the endianess memflow is currently running on.
 *
 * See the [wikipedia article](https://en.wikipedia.org/wiki/Endianness) for more information on the subject.
 */
enum Endianess
#ifdef __cplusplus
  : uint8_t
#endif // __cplusplus
 {
    /**
     * Little Endianess
     */
    Endianess_LittleEndian,
    /**
     * Big Endianess
     */
    Endianess_BigEndian,
};
#ifndef __cplusplus
typedef uint8_t Endianess;
#endif // __cplusplus

typedef struct ArchitectureObj ArchitectureObj;

/**
 * Type definition for temporary return value wrapping storage.
 *
 * The trait does not use return wrapping, thus is a typedef to `PhantomData`.
 *
 * Note that `cbindgen` will generate wrong structures for this type. It is important
 * to go inside the generated headers and fix it - all RetTmp structures without a
 * body should be completely deleted, both as types, and as fields in the
 * groups/objects. If C++11 templates are generated, it is important to define a
 * custom type for CGlueTraitObj that does not have `ret_tmp` defined, and change all
 * type aliases of this trait to use that particular structure.
 */
typedef void CloneRetTmp;

/**
 * Type definition for temporary return value wrapping storage.
 *
 * The trait does not use return wrapping, thus is a typedef to `PhantomData`.
 *
 * Note that `cbindgen` will generate wrong structures for this type. It is important
 * to go inside the generated headers and fix it - all RetTmp structures without a
 * body should be completely deleted, both as types, and as fields in the
 * groups/objects. If C++11 templates are generated, it is important to define a
 * custom type for CGlueTraitObj that does not have `ret_tmp` defined, and change all
 * type aliases of this trait to use that particular structure.
 */
typedef void ConnectorCpuStateInnerRetTmp;

/**
 * Type definition for temporary return value wrapping storage.
 *
 * The trait does not use return wrapping, thus is a typedef to `PhantomData`.
 *
 * Note that `cbindgen` will generate wrong structures for this type. It is important
 * to go inside the generated headers and fix it - all RetTmp structures without a
 * body should be completely deleted, both as types, and as fields in the
 * groups/objects. If C++11 templates are generated, it is important to define a
 * custom type for CGlueTraitObj that does not have `ret_tmp` defined, and change all
 * type aliases of this trait to use that particular structure.
 */
typedef void CpuStateRetTmp;

/**
 * The core of the plugin system
 *
 * It scans system directories and collects valid memflow plugins. They can then be instantiated
 * easily. The reason the libraries are collected is to allow for reuse, and save performance
 *
 * # Examples
 *
 * Creating a OS instance, the recommended way:
 *
 * ```no_run
 * use memflow::plugins::Inventory;
 * # use memflow::error::Result;
 * # use memflow::os::OsInstanceArcBox;
 * # fn test() -> Result<OsInstanceArcBox<'static>> {
 * let inventory = Inventory::scan();
 * inventory
 *   .builder()
 *   .connector("qemu_procfs")
 *   .os("win32")
 *   .build()
 * # }
 * # test().ok();
 * ```
 *
 * Nesting connectors and os plugins:
 * ```no_run
 * use memflow::plugins::{Inventory, Args};
 * # use memflow::error::Result;
 * # fn test() -> Result<()> {
 * let inventory = Inventory::scan();
 * let os = inventory
 *   .builder()
 *   .connector("qemu_procfs")
 *   .os("linux")
 *   .connector("qemu_procfs")
 *   .os("win32")
 *   .build();
 * # Ok(())
 * # }
 * # test().ok();
 * ```
 */
typedef struct Inventory Inventory;

/**
 * Type definition for temporary return value wrapping storage.
 *
 * The trait does not use return wrapping, thus is a typedef to `PhantomData`.
 *
 * Note that `cbindgen` will generate wrong structures for this type. It is important
 * to go inside the generated headers and fix it - all RetTmp structures without a
 * body should be completely deleted, both as types, and as fields in the
 * groups/objects. If C++11 templates are generated, it is important to define a
 * custom type for CGlueTraitObj that does not have `ret_tmp` defined, and change all
 * type aliases of this trait to use that particular structure.
 */
typedef void KeyboardRetTmp;

/**
 * Type definition for temporary return value wrapping storage.
 *
 * The trait does not use return wrapping, thus is a typedef to `PhantomData`.
 *
 * Note that `cbindgen` will generate wrong structures for this type. It is important
 * to go inside the generated headers and fix it - all RetTmp structures without a
 * body should be completely deleted, both as types, and as fields in the
 * groups/objects. If C++11 templates are generated, it is important to define a
 * custom type for CGlueTraitObj that does not have `ret_tmp` defined, and change all
 * type aliases of this trait to use that particular structure.
 */
typedef void KeyboardStateRetTmp;

/**
 * Type definition for temporary return value wrapping storage.
 *
 * The trait does not use return wrapping, thus is a typedef to `PhantomData`.
 *
 * Note that `cbindgen` will generate wrong structures for this type. It is important
 * to go inside the generated headers and fix it - all RetTmp structures without a
 * body should be completely deleted, both as types, and as fields in the
 * groups/objects. If C++11 templates are generated, it is important to define a
 * custom type for CGlueTraitObj that does not have `ret_tmp` defined, and change all
 * type aliases of this trait to use that particular structure.
 */
typedef void OsInnerRetTmp;

/**
 * Type definition for temporary return value wrapping storage.
 *
 * The trait does not use return wrapping, thus is a typedef to `PhantomData`.
 *
 * Note that `cbindgen` will generate wrong structures for this type. It is important
 * to go inside the generated headers and fix it - all RetTmp structures without a
 * body should be completely deleted, both as types, and as fields in the
 * groups/objects. If C++11 templates are generated, it is important to define a
 * custom type for CGlueTraitObj that does not have `ret_tmp` defined, and change all
 * type aliases of this trait to use that particular structure.
 */
typedef void OsKeyboardInnerRetTmp;

/**
 * Type definition for temporary return value wrapping storage.
 *
 * The trait does not use return wrapping, thus is a typedef to `PhantomData`.
 *
 * Note that `cbindgen` will generate wrong structures for this type. It is important
 * to go inside the generated headers and fix it - all RetTmp structures without a
 * body should be completely deleted, both as types, and as fields in the
 * groups/objects. If C++11 templates are generated, it is important to define a
 * custom type for CGlueTraitObj that does not have `ret_tmp` defined, and change all
 * type aliases of this trait to use that particular structure.
 */
typedef void PhysicalMemoryRetTmp;

/**
 * Type definition for temporary return value wrapping storage.
 *
 * The trait does not use return wrapping, thus is a typedef to `PhantomData`.
 *
 * Note that `cbindgen` will generate wrong structures for this type. It is important
 * to go inside the generated headers and fix it - all RetTmp structures without a
 * body should be completely deleted, both as types, and as fields in the
 * groups/objects. If C++11 templates are generated, it is important to define a
 * custom type for CGlueTraitObj that does not have `ret_tmp` defined, and change all
 * type aliases of this trait to use that particular structure.
 */
typedef void ProcessRetTmp;

/**
 * Type definition for temporary return value wrapping storage.
 *
 * The trait does not use return wrapping, thus is a typedef to `PhantomData`.
 *
 * Note that `cbindgen` will generate wrong structures for this type. It is important
 * to go inside the generated headers and fix it - all RetTmp structures without a
 * body should be completely deleted, both as types, and as fields in the
 * groups/objects. If C++11 templates are generated, it is important to define a
 * custom type for CGlueTraitObj that does not have `ret_tmp` defined, and change all
 * type aliases of this trait to use that particular structure.
 */
typedef void VirtualMemoryRetTmp;

/**
 * Type definition for temporary return value wrapping storage.
 *
 * The trait does not use return wrapping, thus is a typedef to `PhantomData`.
 *
 * Note that `cbindgen` will generate wrong structures for this type. It is important
 * to go inside the generated headers and fix it - all RetTmp structures without a
 * body should be completely deleted, both as types, and as fields in the
 * groups/objects. If C++11 templates are generated, it is important to define a
 * custom type for CGlueTraitObj that does not have `ret_tmp` defined, and change all
 * type aliases of this trait to use that particular structure.
 */
typedef void VirtualTranslateRetTmp;

/**
 * This type represents a address on the target system.
 * It internally holds a `u64` value but can also be used
 * when working in 32-bit environments.
 *
 * This type will not handle overflow for 32-bit or 64-bit addresses / lengths.
 */
typedef uint64_t Address;
/**
 * A address with the value of zero.
 *
 * # Examples
 *
 * ```
 * use memflow::types::Address;
 *
 * println!("address: {}", Address::NULL);
 * ```
 */
#define Address_NULL 0

/**
 * Describes the type of a page using a bitflag.
 */
typedef uint8_t PageType;
/**
 * The page explicitly has no flags.
 */
#define PageType_NONE 0
/**
 * The page type is not known.
 */
#define PageType_UNKNOWN 1
/**
 * The page contains page table entries.
 */
#define PageType_PAGE_TABLE 2
/**
 * The page is a writeable page.
 */
#define PageType_WRITEABLE 4
/**
 * The page is read only.
 */
#define PageType_READ_ONLY 8
/**
 * The page is not executable.
 */
#define PageType_NOEXEC 16

/**
 * This type represents a wrapper over a [address](address/index.html)
 * with additional information about the containing page in the physical memory domain.
 *
 * This type will mostly be used by the [virtual to physical address translation](todo.html).
 * When a physical address is translated from a virtual address the additional information
 * about the allocated page the virtual address points to can be obtained from this structure.
 *
 * Most architectures have support multiple page sizes (see [huge pages](todo.html))
 * which will be represented by the containing `page` of the `PhysicalAddress` struct.
 */
typedef struct PhysicalAddress {
    Address address;
    PageType page_type;
    uint8_t page_size_log2;
} PhysicalAddress;

/**
 * FFI-safe box
 *
 * This box has a static self reference, alongside a custom drop function.
 *
 * The drop function can be called from anywhere, it will free on correct allocator internally.
 */
typedef struct CBox_c_void {
    void *instance;
    void (*drop)(void*);
} CBox_c_void;

typedef struct COptArc_c_void {
    const void *inner;
    const void *(*clone_fn)(const void*);
    void (*drop_fn)(const void**);
} COptArc_c_void;

typedef struct CtxBox_c_void__COptArc_c_void {
    struct CBox_c_void inner;
    struct COptArc_c_void ctx;
} CtxBox_c_void__COptArc_c_void;

/**
 * CGlue vtable for trait Clone.
 *
 * This virtual function table contains ABI-safe interface for the given trait.
 */
typedef struct CloneVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void {
    struct CtxBox_c_void__COptArc_c_void (*clone)(const void *thisptr, const struct COptArc_c_void *cglue_ctx);
} CloneVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void;

/**
 * Wrapper around mutable slices.
 *
 * This is meant as a safe type to pass across the FFI boundary with similar semantics as regular
 * slice. However, not all functionality is present, use the slice conversion functions.
 */
typedef struct CSliceMut_u8 {
    uint8_t *data;
    uintptr_t len;
} CSliceMut_u8;

typedef struct PhysicalReadData {
    struct PhysicalAddress _0;
    struct CSliceMut_u8 _1;
} PhysicalReadData;

/**
 * Wrapper around mutable slices.
 *
 * This is meant as a safe type to pass across the FFI boundary with similar semantics as regular
 * slice. However, not all functionality is present, use the slice conversion functions.
 */
typedef struct CSliceMut_PhysicalReadData {
    struct PhysicalReadData *data;
    uintptr_t len;
} CSliceMut_PhysicalReadData;

/**
 * Wrapper around const slices.
 *
 * This is meant as a safe type to pass across the FFI boundary with similar semantics as regular
 * slice. However, not all functionality is present, use the slice conversion functions.
 */
typedef struct CSliceRef_u8 {
    const uint8_t *data;
    uintptr_t len;
} CSliceRef_u8;

typedef struct PhysicalWriteData {
    struct PhysicalAddress _0;
    struct CSliceRef_u8 _1;
} PhysicalWriteData;

/**
 * Wrapper around const slices.
 *
 * This is meant as a safe type to pass across the FFI boundary with similar semantics as regular
 * slice. However, not all functionality is present, use the slice conversion functions.
 */
typedef struct CSliceRef_PhysicalWriteData {
    const struct PhysicalWriteData *data;
    uintptr_t len;
} CSliceRef_PhysicalWriteData;

typedef struct PhysicalMemoryMetadata {
    uintptr_t size;
    bool readonly;
} PhysicalMemoryMetadata;

typedef struct PhysicalMemoryMapping {
    Address base;
    uintptr_t size;
    Address real_base;
} PhysicalMemoryMapping;

/**
 * Wrapper around const slices.
 *
 * This is meant as a safe type to pass across the FFI boundary with similar semantics as regular
 * slice. However, not all functionality is present, use the slice conversion functions.
 */
typedef struct CSliceRef_PhysicalMemoryMapping {
    const struct PhysicalMemoryMapping *data;
    uintptr_t len;
} CSliceRef_PhysicalMemoryMapping;

/**
 * CGlue vtable for trait PhysicalMemory.
 *
 * This virtual function table contains ABI-safe interface for the given trait.
 */
typedef struct PhysicalMemoryVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void {
    int32_t (*phys_read_raw_list)(void *thisptr, struct CSliceMut_PhysicalReadData data);
    int32_t (*phys_write_raw_list)(void *thisptr, struct CSliceRef_PhysicalWriteData data);
    struct PhysicalMemoryMetadata (*metadata)(const void *thisptr);
    void (*set_mem_map)(void *thisptr, struct CSliceRef_PhysicalMemoryMapping mem_map);
    int32_t (*phys_read_raw_into)(void *thisptr, struct PhysicalAddress addr, struct CSliceMut_u8 out);
    int32_t (*phys_write_raw)(void *thisptr, struct PhysicalAddress addr, struct CSliceRef_u8 data);
} PhysicalMemoryVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void;

/**
 * CGlue vtable for trait CpuState.
 *
 * This virtual function table contains ABI-safe interface for the given trait.
 */
typedef struct CpuStateVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void {
    void (*pause)(void *thisptr);
    void (*resume)(void *thisptr);
} CpuStateVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void;

/**
 * Simple CGlue trait object.
 *
 * This is the simplest form of trait object, represented by a this pointer, and a vtable for
 * single trait.
 *
 * `instance` value usually is either a reference, or a mutable reference, or a `CBox`, which
 * contains static reference to the instance, and a dedicated drop function for freeing resources.
 */
typedef struct CGlueTraitObj_CtxBox_c_void__COptArc_c_void________CpuStateVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void________CpuStateRetTmp {
    struct CtxBox_c_void__COptArc_c_void instance;
    const struct CpuStateVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *vtbl;
} CGlueTraitObj_CtxBox_c_void__COptArc_c_void________CpuStateVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void________CpuStateRetTmp;

/**
 * Base CGlue trait object for trait CpuState.
 */
typedef struct CGlueTraitObj_CtxBox_c_void__COptArc_c_void________CpuStateVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void________CpuStateRetTmp CpuStateBase_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void;

/**
 * Trait group potentially implementing `:: cglue :: ext :: core :: clone :: Clone < > + CpuState < >` traits.
 *
 * Optional traits are not implemented here, however. There are numerous conversion
 * functions available for safely retrieving a concrete collection of traits.
 *
 * `check_impl_` functions allow to check if the object implements the wanted traits.
 *
 * `into_impl_` functions consume the object and produce a new final structure that
 * keeps only the required information.
 *
 * `cast_impl_` functions merely check and transform the object into a type that can
 *be transformed back into `IntoCpuState` without losing data.
 *
 * `as_ref_`, and `as_mut_` functions obtain references to safe objects, but do not
 * perform any memory transformations either. They are the safest to use, because
 * there is no risk of accidentally consuming the whole object.
 */
typedef struct IntoCpuState_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void {
    struct CtxBox_c_void__COptArc_c_void instance;
    const struct CloneVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *vtbl_clone;
    const struct CpuStateVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *vtbl_cpustate;
} IntoCpuState_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void;

/**
 * CGlue vtable for trait ConnectorCpuStateInner.
 *
 * This virtual function table contains ABI-safe interface for the given trait.
 */
typedef struct ConnectorCpuStateInnerVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void {
    int32_t (*cpu_state)(void *thisptr, const struct COptArc_c_void *cglue_ctx, CpuStateBase_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *ok_out);
    int32_t (*into_cpu_state)(struct CtxBox_c_void__COptArc_c_void thisobj, struct IntoCpuState_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *ok_out);
} ConnectorCpuStateInnerVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void;

/**
 * Trait group potentially implementing `:: cglue :: ext :: core :: clone :: Clone < > + PhysicalMemory < > + for < 'cglue_c > ConnectorCpuStateInner < 'cglue_c, >` traits.
 *
 * Optional traits are not implemented here, however. There are numerous conversion
 * functions available for safely retrieving a concrete collection of traits.
 *
 * `check_impl_` functions allow to check if the object implements the wanted traits.
 *
 * `into_impl_` functions consume the object and produce a new final structure that
 * keeps only the required information.
 *
 * `cast_impl_` functions merely check and transform the object into a type that can
 *be transformed back into `ConnectorInstance` without losing data.
 *
 * `as_ref_`, and `as_mut_` functions obtain references to safe objects, but do not
 * perform any memory transformations either. They are the safest to use, because
 * there is no risk of accidentally consuming the whole object.
 */
typedef struct ConnectorInstance_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void {
    struct CtxBox_c_void__COptArc_c_void instance;
    const struct CloneVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *vtbl_clone;
    const struct PhysicalMemoryVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *vtbl_physicalmemory;
    const struct ConnectorCpuStateInnerVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *vtbl_connectorcpustateinner;
} ConnectorInstance_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void;

typedef struct ConnectorInstance_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void ConnectorInstanceBase2CtxBox_c_void__COptArc_c_void_____COptArc_c_void;

typedef ConnectorInstanceBase2CtxBox_c_void__COptArc_c_void_____COptArc_c_void ConnectorInstanceBase2ArcBox_c_void__c_void__c_void;

typedef ConnectorInstanceBase2ArcBox_c_void__c_void__c_void ConnectorInstanceArcBox;

typedef ConnectorInstanceArcBox MuConnectorInstanceArcBox;

typedef struct Callback_c_void__Address {
    void *context;
    bool (*func)(void*, Address);
} Callback_c_void__Address;

typedef struct Callback_c_void__Address OpaqueCallback_Address;

typedef OpaqueCallback_Address AddressCallback;

/**
 * Type meant for process IDs
 *
 * If there is a case where Pid can be over 32-bit limit, or negative, please open an issue, we
 * would love to see that.
 */
typedef uint32_t Pid;

/**
 * Wrapper around null-terminated C-style strings.
 *
 * Analog to Rust's `String`, [`ReprCString`] owns the underlying data.
 */
typedef char *ReprCString;

typedef enum ArchitectureIdent_Tag {
    /**
     * Unknown architecture. Could be third-party implemented. memflow knows how to work on them,
     * but is unable to instantiate them.
     */
    ArchitectureIdent_Unknown,
    /**
     * X86 with specified bitness and address extensions
     *
     * First argument - `bitness` controls whether it's 32, or 64 bit variant.
     * Second argument - `address_extensions` control whether address extensions are
     * enabled (PAE on x32, or LA57 on x64). Warning: LA57 is currently unsupported.
     */
    ArchitectureIdent_X86,
    /**
     * Arm 64-bit architecture with specified page size
     *
     * Valid page sizes are 4kb, 16kb, 64kb. Only 4kb is supported at the moment
     */
    ArchitectureIdent_AArch64,
} ArchitectureIdent_Tag;

typedef struct ArchitectureIdent_X86_Body {
    uint8_t _0;
    bool _1;
} ArchitectureIdent_X86_Body;

typedef struct ArchitectureIdent {
    ArchitectureIdent_Tag tag;
    union {
        ArchitectureIdent_X86_Body x86;
        struct {
            uintptr_t a_arch64;
        };
    };
} ArchitectureIdent;

/**
 * Process information structure
 *
 * This structure implements basic process information. Architectures are provided both of the
 * system, and of the process.
 */
typedef struct ProcessInfo {
    /**
     * The base address of this process.
     *
     * # Remarks
     *
     * On Windows this will be the address of the [`_EPROCESS`](https://www.nirsoft.net/kernel_struct/vista/EPROCESS.html) structure.
     */
    Address address;
    /**
     * ID of this process.
     */
    Pid pid;
    /**
     * Name of the process.
     */
    ReprCString name;
    /**
     * Path of the process binary
     */
    ReprCString path;
    /**
     * Command line the process was started with.
     */
    ReprCString command_line;
    /**
     * System architecture of the target system.
     */
    struct ArchitectureIdent sys_arch;
    /**
     * Process architecture
     *
     * # Remarks
     *
     * Specifically on 64-bit systems this could be different
     * to the `sys_arch` in case the process is an emulated 32-bit process.
     *
     * On windows this technique is called [`WOW64`](https://docs.microsoft.com/en-us/windows/win32/winprog64/wow64-implementation-details).
     */
    struct ArchitectureIdent proc_arch;
} ProcessInfo;

typedef struct Callback_c_void__ProcessInfo {
    void *context;
    bool (*func)(void*, struct ProcessInfo);
} Callback_c_void__ProcessInfo;

typedef struct Callback_c_void__ProcessInfo OpaqueCallback_ProcessInfo;

typedef OpaqueCallback_ProcessInfo ProcessInfoCallback;

/**
 * Exit code of a process
 */
typedef int32_t ExitCode;

/**
 * The state of a process
 *
 * # Remarks
 *
 * In case the exit code isn't known ProcessState::Unknown is set.
 */
typedef enum ProcessState_Tag {
    ProcessState_Unknown,
    ProcessState_Alive,
    ProcessState_Dead,
} ProcessState_Tag;

typedef struct ProcessState {
    ProcessState_Tag tag;
    union {
        struct {
            ExitCode dead;
        };
    };
} ProcessState;

/**
 * Pair of address and architecture used for callbacks
 */
typedef struct ModuleAddressInfo {
    Address address;
    struct ArchitectureIdent arch;
} ModuleAddressInfo;

typedef struct Callback_c_void__ModuleAddressInfo {
    void *context;
    bool (*func)(void*, struct ModuleAddressInfo);
} Callback_c_void__ModuleAddressInfo;

typedef struct Callback_c_void__ModuleAddressInfo OpaqueCallback_ModuleAddressInfo;

typedef OpaqueCallback_ModuleAddressInfo ModuleAddressCallback;

/**
 * Module information structure
 */
typedef struct ModuleInfo {
    /**
     * Returns the address of the module header.
     *
     * # Remarks
     *
     * On Windows this will be the address where the [`PEB`](https://docs.microsoft.com/en-us/windows/win32/api/winternl/ns-winternl-peb) entry is stored.
     */
    Address address;
    /**
     * The base address of the parent process.
     *
     * # Remarks
     *
     * This field is analog to the `ProcessInfo::address` field.
     */
    Address parent_process;
    /**
     * The actual base address of this module.
     *
     * # Remarks
     *
     * The base address is contained in the virtual address range of the process
     * this module belongs to.
     */
    Address base;
    /**
     * Size of the module
     */
    uintptr_t size;
    /**
     * Name of the module
     */
    ReprCString name;
    /**
     * Path of the module
     */
    ReprCString path;
    /**
     * Architecture of the module
     *
     * # Remarks
     *
     * Emulated processes often have 2 separate lists of modules, one visible to the emulated
     * context (e.g. all 32-bit modules in a WoW64 process), and the other for all native modules
     * needed to support the process emulation. This should be equal to either
     * `ProcessInfo::proc_arch`, or `ProcessInfo::sys_arch` of the parent process.
     */
    struct ArchitectureIdent arch;
} ModuleInfo;

typedef struct Callback_c_void__ModuleInfo {
    void *context;
    bool (*func)(void*, struct ModuleInfo);
} Callback_c_void__ModuleInfo;

typedef struct Callback_c_void__ModuleInfo OpaqueCallback_ModuleInfo;

typedef OpaqueCallback_ModuleInfo ModuleInfoCallback;

/**
 * Import information structure
 */
typedef struct ImportInfo {
    /**
     * Name of the import
     */
    ReprCString name;
    /**
     * Offset of this import from the containing modules base address
     */
    uintptr_t offset;
} ImportInfo;

typedef struct Callback_c_void__ImportInfo {
    void *context;
    bool (*func)(void*, struct ImportInfo);
} Callback_c_void__ImportInfo;

typedef struct Callback_c_void__ImportInfo OpaqueCallback_ImportInfo;

typedef OpaqueCallback_ImportInfo ImportCallback;

/**
 * Export information structure
 */
typedef struct ExportInfo {
    /**
     * Name of the export
     */
    ReprCString name;
    /**
     * Offset of this export from the containing modules base address
     */
    uintptr_t offset;
} ExportInfo;

typedef struct Callback_c_void__ExportInfo {
    void *context;
    bool (*func)(void*, struct ExportInfo);
} Callback_c_void__ExportInfo;

typedef struct Callback_c_void__ExportInfo OpaqueCallback_ExportInfo;

typedef OpaqueCallback_ExportInfo ExportCallback;

/**
 * Section information structure
 */
typedef struct SectionInfo {
    /**
     * Name of the section
     */
    ReprCString name;
    /**
     * Virtual address of this section (essentially module_info.base + virtual_address)
     */
    Address base;
    /**
     * Size of this section
     */
    uintptr_t size;
} SectionInfo;

typedef struct Callback_c_void__SectionInfo {
    void *context;
    bool (*func)(void*, struct SectionInfo);
} Callback_c_void__SectionInfo;

typedef struct Callback_c_void__SectionInfo OpaqueCallback_SectionInfo;

typedef OpaqueCallback_SectionInfo SectionCallback;

/**
 * CGlue vtable for trait Process.
 *
 * This virtual function table contains ABI-safe interface for the given trait.
 */
typedef struct ProcessVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void {
    struct ProcessState (*state)(void *thisptr);
    int32_t (*module_address_list_callback)(void *thisptr, const struct ArchitectureIdent *target_arch, ModuleAddressCallback callback);
    int32_t (*module_list_callback)(void *thisptr, const struct ArchitectureIdent *target_arch, ModuleInfoCallback callback);
    int32_t (*module_by_address)(void *thisptr, Address address, struct ArchitectureIdent architecture, struct ModuleInfo *ok_out);
    int32_t (*module_by_name_arch)(void *thisptr, struct CSliceRef_u8 name, const struct ArchitectureIdent *architecture, struct ModuleInfo *ok_out);
    int32_t (*module_by_name)(void *thisptr, struct CSliceRef_u8 name, struct ModuleInfo *ok_out);
    int32_t (*primary_module_address)(void *thisptr, Address *ok_out);
    int32_t (*primary_module)(void *thisptr, struct ModuleInfo *ok_out);
    int32_t (*module_import_list_callback)(void *thisptr, const struct ModuleInfo *info, ImportCallback callback);
    int32_t (*module_export_list_callback)(void *thisptr, const struct ModuleInfo *info, ExportCallback callback);
    int32_t (*module_section_list_callback)(void *thisptr, const struct ModuleInfo *info, SectionCallback callback);
    int32_t (*module_import_by_name)(void *thisptr, const struct ModuleInfo *info, struct CSliceRef_u8 name, struct ImportInfo *ok_out);
    int32_t (*module_export_by_name)(void *thisptr, const struct ModuleInfo *info, struct CSliceRef_u8 name, struct ExportInfo *ok_out);
    int32_t (*module_section_by_name)(void *thisptr, const struct ModuleInfo *info, struct CSliceRef_u8 name, struct SectionInfo *ok_out);
    const struct ProcessInfo *(*info)(const void *thisptr);
} ProcessVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void;

typedef struct VirtualReadData {
    Address _0;
    struct CSliceMut_u8 _1;
} VirtualReadData;

/**
 * Wrapper around mutable slices.
 *
 * This is meant as a safe type to pass across the FFI boundary with similar semantics as regular
 * slice. However, not all functionality is present, use the slice conversion functions.
 */
typedef struct CSliceMut_VirtualReadData {
    struct VirtualReadData *data;
    uintptr_t len;
} CSliceMut_VirtualReadData;

typedef struct VirtualWriteData {
    Address _0;
    struct CSliceRef_u8 _1;
} VirtualWriteData;

/**
 * Wrapper around const slices.
 *
 * This is meant as a safe type to pass across the FFI boundary with similar semantics as regular
 * slice. However, not all functionality is present, use the slice conversion functions.
 */
typedef struct CSliceRef_VirtualWriteData {
    const struct VirtualWriteData *data;
    uintptr_t len;
} CSliceRef_VirtualWriteData;

/**
 * CGlue vtable for trait VirtualMemory.
 *
 * This virtual function table contains ABI-safe interface for the given trait.
 */
typedef struct VirtualMemoryVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void {
    int32_t (*virt_read_raw_list)(void *thisptr, struct CSliceMut_VirtualReadData data);
    int32_t (*virt_write_raw_list)(void *thisptr, struct CSliceRef_VirtualWriteData data);
    int32_t (*virt_read_raw_into)(void *thisptr, Address addr, struct CSliceMut_u8 out);
    int32_t (*virt_write_raw)(void *thisptr, Address addr, struct CSliceRef_u8 data);
} VirtualMemoryVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void;

/**
 * Virtual page range information used for callbacks
 */
typedef struct MemoryRange {
    Address address;
    uintptr_t size;
} MemoryRange;

/**
 * Wrapper around const slices.
 *
 * This is meant as a safe type to pass across the FFI boundary with similar semantics as regular
 * slice. However, not all functionality is present, use the slice conversion functions.
 */
typedef struct CSliceRef_MemoryRange {
    const struct MemoryRange *data;
    uintptr_t len;
} CSliceRef_MemoryRange;

/**
 * Virtual page range information with physical mappings used for callbacks
 */
typedef struct VirtualTranslation {
    Address in_virtual;
    uintptr_t size;
    struct PhysicalAddress out_physical;
} VirtualTranslation;

typedef struct Callback_c_void__VirtualTranslation {
    void *context;
    bool (*func)(void*, struct VirtualTranslation);
} Callback_c_void__VirtualTranslation;

typedef struct Callback_c_void__VirtualTranslation OpaqueCallback_VirtualTranslation;

typedef OpaqueCallback_VirtualTranslation VirtualTranslationCallback;

typedef struct VirtualTranslationFail {
    Address from;
    uintptr_t size;
} VirtualTranslationFail;

typedef struct Callback_c_void__VirtualTranslationFail {
    void *context;
    bool (*func)(void*, struct VirtualTranslationFail);
} Callback_c_void__VirtualTranslationFail;

typedef struct Callback_c_void__VirtualTranslationFail OpaqueCallback_VirtualTranslationFail;

typedef OpaqueCallback_VirtualTranslationFail VirtualTranslationFailCallback;

typedef struct Callback_c_void__MemoryRange {
    void *context;
    bool (*func)(void*, struct MemoryRange);
} Callback_c_void__MemoryRange;

typedef struct Callback_c_void__MemoryRange OpaqueCallback_MemoryRange;

typedef OpaqueCallback_MemoryRange MemoryRangeCallback;

/**
 * A `Page` holds information about a memory page.
 *
 * More information about paging can be found [here](https://en.wikipedia.org/wiki/Paging).
 */
typedef struct Page {
    /**
     * Contains the page type (see above).
     */
    PageType page_type;
    /**
     * Contains the base address of this page.
     */
    Address page_base;
    /**
     * Contains the size of this page.
     */
    uintptr_t page_size;
} Page;

/**
 * FFI-safe Option.
 *
 * This type is not really meant for general use, but rather as a last-resort conversion for type
 * wrapping.
 *
 * Typical workflow would include temporarily converting into/from COption.
 */
typedef enum COption_Address_Tag {
    COption_Address_None_Address,
    COption_Address_Some_Address,
} COption_Address_Tag;

typedef struct COption_Address {
    COption_Address_Tag tag;
    union {
        struct {
            Address some;
        };
    };
} COption_Address;

/**
 * CGlue vtable for trait VirtualTranslate.
 *
 * This virtual function table contains ABI-safe interface for the given trait.
 */
typedef struct VirtualTranslateVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void {
    void (*virt_to_phys_list)(void *thisptr, struct CSliceRef_MemoryRange addrs, VirtualTranslationCallback out, VirtualTranslationFailCallback out_fail);
    void (*virt_to_phys_range)(void *thisptr, Address start, Address end, VirtualTranslationCallback out);
    void (*virt_translation_map_range)(void *thisptr, Address start, Address end, VirtualTranslationCallback out);
    void (*virt_page_map_range)(void *thisptr, uintptr_t gap_size, Address start, Address end, MemoryRangeCallback out);
    int32_t (*virt_to_phys)(void *thisptr, Address address, struct PhysicalAddress *ok_out);
    int32_t (*virt_page_info)(void *thisptr, Address addr, struct Page *ok_out);
    void (*virt_translation_map)(void *thisptr, VirtualTranslationCallback out);
    struct COption_Address (*phys_to_virt)(void *thisptr, Address phys);
    void (*virt_page_map)(void *thisptr, uintptr_t gap_size, MemoryRangeCallback out);
} VirtualTranslateVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void;

/**
 * Trait group potentially implementing `Process < > + VirtualMemory < > + VirtualTranslate < >` traits.
 *
 * Optional traits are not implemented here, however. There are numerous conversion
 * functions available for safely retrieving a concrete collection of traits.
 *
 * `check_impl_` functions allow to check if the object implements the wanted traits.
 *
 * `into_impl_` functions consume the object and produce a new final structure that
 * keeps only the required information.
 *
 * `cast_impl_` functions merely check and transform the object into a type that can
 *be transformed back into `ProcessInstance` without losing data.
 *
 * `as_ref_`, and `as_mut_` functions obtain references to safe objects, but do not
 * perform any memory transformations either. They are the safest to use, because
 * there is no risk of accidentally consuming the whole object.
 */
typedef struct ProcessInstance_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void {
    struct CtxBox_c_void__COptArc_c_void instance;
    const struct ProcessVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *vtbl_process;
    const struct VirtualMemoryVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *vtbl_virtualmemory;
    const struct VirtualTranslateVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *vtbl_virtualtranslate;
} ProcessInstance_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void;

/**
 * Trait group potentially implementing `:: cglue :: ext :: core :: clone :: Clone < > + Process < > + VirtualMemory < > + VirtualTranslate < >` traits.
 *
 * Optional traits are not implemented here, however. There are numerous conversion
 * functions available for safely retrieving a concrete collection of traits.
 *
 * `check_impl_` functions allow to check if the object implements the wanted traits.
 *
 * `into_impl_` functions consume the object and produce a new final structure that
 * keeps only the required information.
 *
 * `cast_impl_` functions merely check and transform the object into a type that can
 *be transformed back into `IntoProcessInstance` without losing data.
 *
 * `as_ref_`, and `as_mut_` functions obtain references to safe objects, but do not
 * perform any memory transformations either. They are the safest to use, because
 * there is no risk of accidentally consuming the whole object.
 */
typedef struct IntoProcessInstance_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void {
    struct CtxBox_c_void__COptArc_c_void instance;
    const struct CloneVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *vtbl_clone;
    const struct ProcessVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *vtbl_process;
    const struct VirtualMemoryVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *vtbl_virtualmemory;
    const struct VirtualTranslateVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *vtbl_virtualtranslate;
} IntoProcessInstance_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void;

/**
 * Information block about OS
 *
 * This provides some basic information about the OS in question. `base`, and `size` may be
 * omitted in some circumstances (lack of kernel, or privileges). But architecture should always
 * be correct.
 */
typedef struct OsInfo {
    /**
     * Base address of the OS kernel
     */
    Address base;
    /**
     * Size of the OS kernel
     */
    uintptr_t size;
    /**
     * System architecture
     */
    struct ArchitectureIdent arch;
} OsInfo;

/**
 * CGlue vtable for trait OsInner.
 *
 * This virtual function table contains ABI-safe interface for the given trait.
 */
typedef struct OsInnerVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void {
    int32_t (*process_address_list_callback)(void *thisptr, AddressCallback callback);
    int32_t (*process_info_list_callback)(void *thisptr, ProcessInfoCallback callback);
    int32_t (*process_info_by_address)(void *thisptr, Address address, struct ProcessInfo *ok_out);
    int32_t (*process_info_by_name)(void *thisptr, struct CSliceRef_u8 name, struct ProcessInfo *ok_out);
    int32_t (*process_info_by_pid)(void *thisptr, Pid pid, struct ProcessInfo *ok_out);
    int32_t (*process_by_info)(void *thisptr, struct ProcessInfo info, const struct COptArc_c_void *cglue_ctx, struct ProcessInstance_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *ok_out);
    int32_t (*into_process_by_info)(struct CtxBox_c_void__COptArc_c_void thisobj, struct ProcessInfo info, struct IntoProcessInstance_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *ok_out);
    int32_t (*process_by_address)(void *thisptr, Address addr, const struct COptArc_c_void *cglue_ctx, struct ProcessInstance_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *ok_out);
    int32_t (*process_by_name)(void *thisptr, struct CSliceRef_u8 name, const struct COptArc_c_void *cglue_ctx, struct ProcessInstance_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *ok_out);
    int32_t (*process_by_pid)(void *thisptr, Pid pid, const struct COptArc_c_void *cglue_ctx, struct ProcessInstance_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *ok_out);
    int32_t (*into_process_by_address)(struct CtxBox_c_void__COptArc_c_void thisobj, Address addr, struct IntoProcessInstance_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *ok_out);
    int32_t (*into_process_by_name)(struct CtxBox_c_void__COptArc_c_void thisobj, struct CSliceRef_u8 name, struct IntoProcessInstance_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *ok_out);
    int32_t (*into_process_by_pid)(struct CtxBox_c_void__COptArc_c_void thisobj, Pid pid, struct IntoProcessInstance_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *ok_out);
    int32_t (*module_address_list_callback)(void *thisptr, AddressCallback callback);
    int32_t (*module_list_callback)(void *thisptr, ModuleInfoCallback callback);
    int32_t (*module_by_address)(void *thisptr, Address address, struct ModuleInfo *ok_out);
    int32_t (*module_by_name)(void *thisptr, struct CSliceRef_u8 name, struct ModuleInfo *ok_out);
    const struct OsInfo *(*info)(const void *thisptr);
} OsInnerVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void;

/**
 * CGlue vtable for trait KeyboardState.
 *
 * This virtual function table contains ABI-safe interface for the given trait.
 */
typedef struct KeyboardStateVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void {
    bool (*is_down)(const void *thisptr, int32_t vk);
} KeyboardStateVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void;

/**
 * Simple CGlue trait object.
 *
 * This is the simplest form of trait object, represented by a this pointer, and a vtable for
 * single trait.
 *
 * `instance` value usually is either a reference, or a mutable reference, or a `CBox`, which
 * contains static reference to the instance, and a dedicated drop function for freeing resources.
 */
typedef struct CGlueTraitObj_CtxBox_c_void__COptArc_c_void________KeyboardStateVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void________KeyboardStateRetTmp {
    struct CtxBox_c_void__COptArc_c_void instance;
    const struct KeyboardStateVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *vtbl;
} CGlueTraitObj_CtxBox_c_void__COptArc_c_void________KeyboardStateVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void________KeyboardStateRetTmp;

/**
 * Base CGlue trait object for trait KeyboardState.
 */
typedef struct CGlueTraitObj_CtxBox_c_void__COptArc_c_void________KeyboardStateVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void________KeyboardStateRetTmp KeyboardStateBase_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void;

/**
 * CGlue vtable for trait Keyboard.
 *
 * This virtual function table contains ABI-safe interface for the given trait.
 */
typedef struct KeyboardVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void {
    bool (*is_down)(void *thisptr, int32_t vk);
    void (*set_down)(void *thisptr, int32_t vk, bool down);
    int32_t (*state)(void *thisptr, const struct COptArc_c_void *cglue_ctx, KeyboardStateBase_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *ok_out);
} KeyboardVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void;

/**
 * Simple CGlue trait object.
 *
 * This is the simplest form of trait object, represented by a this pointer, and a vtable for
 * single trait.
 *
 * `instance` value usually is either a reference, or a mutable reference, or a `CBox`, which
 * contains static reference to the instance, and a dedicated drop function for freeing resources.
 */
typedef struct CGlueTraitObj_CtxBox_c_void__COptArc_c_void________KeyboardVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void________KeyboardRetTmp {
    struct CtxBox_c_void__COptArc_c_void instance;
    const struct KeyboardVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *vtbl;
} CGlueTraitObj_CtxBox_c_void__COptArc_c_void________KeyboardVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void________KeyboardRetTmp;

/**
 * Base CGlue trait object for trait Keyboard.
 */
typedef struct CGlueTraitObj_CtxBox_c_void__COptArc_c_void________KeyboardVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void________KeyboardRetTmp KeyboardBase_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void;

/**
 * Trait group potentially implementing `:: cglue :: ext :: core :: clone :: Clone < > + Keyboard < >` traits.
 *
 * Optional traits are not implemented here, however. There are numerous conversion
 * functions available for safely retrieving a concrete collection of traits.
 *
 * `check_impl_` functions allow to check if the object implements the wanted traits.
 *
 * `into_impl_` functions consume the object and produce a new final structure that
 * keeps only the required information.
 *
 * `cast_impl_` functions merely check and transform the object into a type that can
 *be transformed back into `IntoKeyboard` without losing data.
 *
 * `as_ref_`, and `as_mut_` functions obtain references to safe objects, but do not
 * perform any memory transformations either. They are the safest to use, because
 * there is no risk of accidentally consuming the whole object.
 */
typedef struct IntoKeyboard_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void {
    struct CtxBox_c_void__COptArc_c_void instance;
    const struct CloneVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *vtbl_clone;
    const struct KeyboardVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *vtbl_keyboard;
} IntoKeyboard_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void;

/**
 * CGlue vtable for trait OsKeyboardInner.
 *
 * This virtual function table contains ABI-safe interface for the given trait.
 */
typedef struct OsKeyboardInnerVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void {
    int32_t (*keyboard)(void *thisptr, const struct COptArc_c_void *cglue_ctx, KeyboardBase_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *ok_out);
    int32_t (*into_keyboard)(struct CtxBox_c_void__COptArc_c_void thisobj, struct IntoKeyboard_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *ok_out);
} OsKeyboardInnerVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void;

/**
 * Trait group potentially implementing `:: cglue :: ext :: core :: clone :: Clone < > + for < 'cglue_c > OsInner < 'cglue_c, > + for < 'cglue_c > OsKeyboardInner < 'cglue_c, > + PhysicalMemory < > + VirtualMemory < >` traits.
 *
 * Optional traits are not implemented here, however. There are numerous conversion
 * functions available for safely retrieving a concrete collection of traits.
 *
 * `check_impl_` functions allow to check if the object implements the wanted traits.
 *
 * `into_impl_` functions consume the object and produce a new final structure that
 * keeps only the required information.
 *
 * `cast_impl_` functions merely check and transform the object into a type that can
 *be transformed back into `OsInstance` without losing data.
 *
 * `as_ref_`, and `as_mut_` functions obtain references to safe objects, but do not
 * perform any memory transformations either. They are the safest to use, because
 * there is no risk of accidentally consuming the whole object.
 */
typedef struct OsInstance_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void {
    struct CtxBox_c_void__COptArc_c_void instance;
    const struct CloneVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *vtbl_clone;
    const struct OsInnerVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *vtbl_osinner;
    const struct OsKeyboardInnerVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *vtbl_oskeyboardinner;
    const struct PhysicalMemoryVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *vtbl_physicalmemory;
    const struct VirtualMemoryVtbl_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void *vtbl_virtualmemory;
} OsInstance_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void;

typedef struct OsInstance_CtxBox_c_void__COptArc_c_void________c_void__COptArc_c_void_____COptArc_c_void OsInstanceBase2CtxBox_c_void__COptArc_c_void_____COptArc_c_void;

typedef OsInstanceBase2CtxBox_c_void__COptArc_c_void_____COptArc_c_void OsInstanceBase2ArcBox_c_void__c_void__c_void;

typedef OsInstanceBase2ArcBox_c_void__c_void__c_void OsInstanceArcBox;

typedef OsInstanceArcBox MuOsInstanceArcBox;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

extern const struct ArchitectureObj *X86_32;

extern const struct ArchitectureObj *X86_32_PAE;

extern const struct ArchitectureObj *X86_64;

void log_init(int32_t level_num);

/**
 * Helper to convert `Address` to a `PhysicalAddress`
 *
 * This will create a `PhysicalAddress` with `UNKNOWN` PageType.
 */
struct PhysicalAddress addr_to_paddr(Address address);

/**
 * Create a new connector inventory
 *
 * This function will try to find connectors using PATH environment variable
 *
 * Note that all functions go through each directories, and look for a `memflow` directory,
 * and search for libraries in those.
 *
 * # Safety
 *
 * Inventory is inherently unsafe, because it loads shared libraries which can not be
 * guaranteed to be safe.
 */
struct Inventory *inventory_scan(void);

/**
 * Create a new inventory with custom path string
 *
 * # Safety
 *
 * `path` must be a valid null terminated string
 */
struct Inventory *inventory_scan_path(const char *path);

/**
 * Add a directory to an existing inventory
 *
 * # Safety
 *
 * `dir` must be a valid null terminated string
 */
int32_t inventory_add_dir(struct Inventory *inv, const char *dir);

/**
 * Create a connector with given arguments
 *
 * This creates an instance of `ConnectorInstance`.
 *
 * This instance needs to be dropped using `connector_drop`.
 *
 * # Arguments
 *
 * * `name` - name of the connector to use
 * * `args` - arguments to be passed to the connector upon its creation
 *
 * # Safety
 *
 * Both `name`, and `args` must be valid null terminated strings.
 *
 * Any error strings returned by the connector must not be outputed after the connector gets
 * freed, because that operation could cause the underlying shared library to get unloaded.
 */
int32_t inventory_create_connector(struct Inventory *inv,
                                   const char *name,
                                   const char *args,
                                   MuConnectorInstanceArcBox *out);

/**
 * Create a OS instance with given arguments
 *
 * This creates an instance of `KernelInstance`.
 *
 * This instance needs to be freed using `os_drop`.
 *
 * # Arguments
 *
 * * `name` - name of the OS to use
 * * `args` - arguments to be passed to the connector upon its creation
 * * `mem` - a previously initialized connector instance
 * * `out` - a valid memory location that will contain the resulting os-instance
 *
 * # Remarks
 *
 * The `mem` connector instance is being _moved_ into the os layer.
 * This means upon calling `os_drop` it is not unnecessary to call `connector_drop` anymore.
 *
 * # Safety
 *
 * Both `name`, and `args` must be valid null terminated strings.
 *
 * Any error strings returned by the connector must not be outputed after the connector gets
 * freed, because that operation could cause the underlying shared library to get unloaded.
 */
int32_t inventory_create_os(struct Inventory *inv,
                            const char *name,
                            const char *args,
                            ConnectorInstanceArcBox mem,
                            MuOsInstanceArcBox *out);

/**
 * Free a os plugin
 *
 * # Safety
 *
 * `os` must point to a valid `OsInstance` that was created using one of the provided
 * functions.
 */
void os_drop(OsInstanceArcBox *os);

/**
 * Clone a connector
 *
 * This method is useful when needing to perform multithreaded operations, as a connector is not
 * guaranteed to be thread safe. Every single cloned instance also needs to be dropped using
 * `connector_drop`.
 *
 * # Safety
 *
 * `conn` has to point to a a valid `CloneablePhysicalMemory` created by one of the provided
 * functions.
 */
void connector_clone(const ConnectorInstanceArcBox *conn, MuConnectorInstanceArcBox *out);

/**
 * Free a connector instance
 *
 * # Safety
 *
 * `conn` has to point to a valid [`ConnectorInstance`] created by one of the provided
 * functions.
 *
 * There has to be no instance of `PhysicalMemory` created from the input `conn`, because they
 * will become invalid.
 */
void connector_drop(ConnectorInstanceArcBox *conn);

/**
 * Free a connector inventory
 *
 * # Safety
 *
 * `inv` must point to a valid `Inventory` that was created using one of the provided
 * functions.
 */
void inventory_free(struct Inventory *inv);

uint8_t arch_bits(const struct ArchitectureObj *arch);

Endianess arch_endianess(const struct ArchitectureObj *arch);

uintptr_t arch_page_size(const struct ArchitectureObj *arch);

uintptr_t arch_size_addr(const struct ArchitectureObj *arch);

uint8_t arch_address_space_bits(const struct ArchitectureObj *arch);

/**
 * Free an architecture reference
 *
 * # Safety
 *
 * `arch` must be a valid heap allocated reference created by one of the API's functions.
 */
void arch_free(struct ArchitectureObj *arch);

bool is_x86_arch(const struct ArchitectureObj *arch);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif /* MEMFLOW_H */