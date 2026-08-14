typedef unsigned char   undefined;

typedef unsigned char    bool;
typedef unsigned char    byte;
typedef unsigned int    dword;
typedef unsigned long long    GUID;
typedef pointer32 ImageBaseOffset32;

typedef long long    longlong;
typedef unsigned long long    qword;
typedef unsigned char    uchar;
typedef unsigned int    uint;
typedef unsigned long    ulong;
typedef unsigned long long    ulonglong;
typedef unsigned char    undefined1;
typedef unsigned int    undefined4;
typedef unsigned long long    undefined8;
typedef unsigned short    ushort;
typedef unsigned short    wchar16;
typedef short    wchar_t;
typedef unsigned short    word;
typedef struct _s__RTTIBaseClassDescriptor _s__RTTIBaseClassDescriptor, *P_s__RTTIBaseClassDescriptor;

typedef struct _s__RTTIBaseClassDescriptor RTTIBaseClassDescriptor;

typedef RTTIBaseClassDescriptor *RTTIBaseClassDescriptor *32 __((image-base-relative));

typedef RTTIBaseClassDescriptor *32 __((image-base-relative)) *RTTIBaseClassDescriptor *32 __((image-base-relative)) *32 __((image-base-relative));

typedef struct PMD PMD, *PPMD;

struct PMD {
    int mdisp;
    int pdisp;
    int vdisp;
};

struct _s__RTTIBaseClassDescriptor {
    ImageBaseOffset32 pTypeDescriptor; // ref to TypeDescriptor (RTTI 0) for class
    dword numContainedBases; // count of extended classes in BaseClassArray (RTTI 2)
    struct PMD where; // member displacement structure
    dword attributes; // bit flags
    ImageBaseOffset32 pClassHierarchyDescriptor; // ref to ClassHierarchyDescriptor (RTTI 3) for class
};

typedef struct CLIENT_ID CLIENT_ID, *PCLIENT_ID;

struct CLIENT_ID {
    void *UniqueProcess;
    void *UniqueThread;
};

typedef struct _s__RTTIClassHierarchyDescriptor _s__RTTIClassHierarchyDescriptor, *P_s__RTTIClassHierarchyDescriptor;

typedef struct _s__RTTIClassHierarchyDescriptor RTTIClassHierarchyDescriptor;

struct _s__RTTIClassHierarchyDescriptor {
    dword signature;
    dword attributes; // bit flags
    dword numBaseClasses; // number of base classes (i.e. rtti1Count)
    RTTIBaseClassDescriptor *32 __((image-base-relative)) *32 __((image-base-relative)) pBaseClassArray; // ref to BaseClassArray (RTTI 2)
};

typedef union IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryUnion IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryUnion, *PIMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryUnion;

typedef struct IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryStruct IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryStruct, *PIMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryStruct;

struct IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryStruct {
    dword OffsetToDirectory:31;
    dword DataIsDirectory:1;
};

union IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryUnion {
    dword OffsetToData;
    struct IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryStruct IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryStruct;
};

typedef struct _s__RTTICompleteObjectLocator _s__RTTICompleteObjectLocator, *P_s__RTTICompleteObjectLocator;

struct _s__RTTICompleteObjectLocator {
    dword signature;
    dword offset; // offset of vbtable within class
    dword cdOffset; // constructor displacement offset
    ImageBaseOffset32 pTypeDescriptor; // ref to TypeDescriptor (RTTI 0) for class
    ImageBaseOffset32 pClassDescriptor; // ref to ClassHierarchyDescriptor (RTTI 3)
};

typedef struct _s__RTTICompleteObjectLocator RTTICompleteObjectLocator;


// WARNING! conflicting data type names: /guiddef.h/GUID - /GUID

typedef GUID IID;

typedef struct _GUID _GUID, *P_GUID;

struct _GUID {
    ulong Data1;
    ushort Data2;
    ushort Data3;
    uchar Data4[8];
};

typedef struct _SECURITY_ATTRIBUTES _SECURITY_ATTRIBUTES, *P_SECURITY_ATTRIBUTES;

typedef ulong DWORD;

typedef void *LPVOID;

typedef int BOOL;

struct _SECURITY_ATTRIBUTES {
    DWORD nLength;
    LPVOID lpSecurityDescriptor;
    BOOL bInheritHandle;
};

typedef struct _SYSTEMTIME _SYSTEMTIME, *P_SYSTEMTIME;

typedef ushort WORD;

struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
};

typedef DWORD (*PTHREAD_START_ROUTINE)(LPVOID);

typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;

typedef struct _SECURITY_ATTRIBUTES *LPSECURITY_ATTRIBUTES;

typedef struct _SYSTEMTIME *LPSYSTEMTIME;

typedef wchar_t WCHAR;

typedef long HRESULT;

typedef char CHAR;

typedef CHAR *LPCSTR;

typedef union _LARGE_INTEGER _LARGE_INTEGER, *P_LARGE_INTEGER;

typedef struct _struct_19 _struct_19, *P_struct_19;

typedef struct _struct_20 _struct_20, *P_struct_20;

typedef longlong LONGLONG;

typedef long LONG;

struct _struct_20 {
    DWORD LowPart;
    LONG HighPart;
};

struct _struct_19 {
    DWORD LowPart;
    LONG HighPart;
};

union _LARGE_INTEGER {
    struct _struct_19 s;
    struct _struct_20 u;
    LONGLONG QuadPart;
};

typedef union _LARGE_INTEGER LARGE_INTEGER;

typedef WCHAR *LPCWSTR;

typedef CHAR *LPSTR;

typedef void *HANDLE;

typedef WCHAR *LPWSTR;

typedef struct IMAGE_DOS_HEADER IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;

struct IMAGE_DOS_HEADER {
    char e_magic[2]; // Magic number
    word e_cblp; // Bytes of last page
    word e_cp; // Pages in file
    word e_crlc; // Relocations
    word e_cparhdr; // Size of header in paragraphs
    word e_minalloc; // Minimum extra paragraphs needed
    word e_maxalloc; // Maximum extra paragraphs needed
    word e_ss; // Initial (relative) SS value
    word e_sp; // Initial SP value
    word e_csum; // Checksum
    word e_ip; // Initial IP value
    word e_cs; // Initial (relative) CS value
    word e_lfarlc; // File address of relocation table
    word e_ovno; // Overlay number
    word e_res[4][4]; // Reserved words
    word e_oemid; // OEM identifier (for e_oeminfo)
    word e_oeminfo; // OEM information; e_oemid specific
    word e_res2[10][10]; // Reserved words
    dword e_lfanew; // File address of new exe header
    byte e_program[64]; // Actual DOS program
};

typedef longlong INT_PTR;

typedef ulonglong ULONG_PTR;

typedef ULONG_PTR SIZE_T;

typedef struct DotNetPdbInfo DotNetPdbInfo, *PDotNetPdbInfo;

struct DotNetPdbInfo {
    char signature[4];
    GUID guid;
    dword age;
    char pdbpath[84];
};

typedef struct _FILETIME _FILETIME, *P_FILETIME;

typedef struct _FILETIME *LPFILETIME;

struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
};

typedef INT_PTR (*FARPROC)(void);

typedef struct HINSTANCE__ HINSTANCE__, *PHINSTANCE__;

typedef struct HINSTANCE__ *HINSTANCE;

struct HINSTANCE__ {
    int unused;
};

typedef struct HWND__ HWND__, *PHWND__;

typedef struct HWND__ *HWND;

struct HWND__ {
    int unused;
};

typedef HINSTANCE HMODULE;

typedef DWORD *LPDWORD;

typedef uint UINT;

typedef struct _IMAGE_RUNTIME_FUNCTION_ENTRY _IMAGE_RUNTIME_FUNCTION_ENTRY, *P_IMAGE_RUNTIME_FUNCTION_ENTRY;

struct _IMAGE_RUNTIME_FUNCTION_ENTRY {
    ImageBaseOffset32 BeginAddress;
    dword EndAddress; // Apply ImageBaseOffset32 to see reference
    ImageBaseOffset32 UnwindInfoAddressOrData;
};

typedef struct IMAGE_RESOURCE_DIRECTORY_ENTRY_NameStruct IMAGE_RESOURCE_DIRECTORY_ENTRY_NameStruct, *PIMAGE_RESOURCE_DIRECTORY_ENTRY_NameStruct;

struct IMAGE_RESOURCE_DIRECTORY_ENTRY_NameStruct {
    dword NameOffset:31;
    dword NameIsString:1;
};

typedef struct IMAGE_LOAD_CONFIG_CODE_INTEGRITY IMAGE_LOAD_CONFIG_CODE_INTEGRITY, *PIMAGE_LOAD_CONFIG_CODE_INTEGRITY;

struct IMAGE_LOAD_CONFIG_CODE_INTEGRITY {
    word Flags;
    word Catalog;
    dword CatalogOffset;
    dword Reserved;
};

typedef struct IMAGE_DEBUG_DIRECTORY IMAGE_DEBUG_DIRECTORY, *PIMAGE_DEBUG_DIRECTORY;

struct IMAGE_DEBUG_DIRECTORY {
    dword Characteristics;
    dword TimeDateStamp;
    word MajorVersion;
    word MinorVersion;
    dword Type;
    dword SizeOfData;
    dword AddressOfRawData;
    dword PointerToRawData;
};

typedef struct IMAGE_FILE_HEADER IMAGE_FILE_HEADER, *PIMAGE_FILE_HEADER;

struct IMAGE_FILE_HEADER {
    word Machine; // 34404
    word NumberOfSections;
    dword TimeDateStamp;
    dword PointerToSymbolTable;
    dword NumberOfSymbols;
    word SizeOfOptionalHeader;
    word Characteristics;
};

typedef struct IMAGE_LOAD_CONFIG_DIRECTORY64 IMAGE_LOAD_CONFIG_DIRECTORY64, *PIMAGE_LOAD_CONFIG_DIRECTORY64;

typedef enum IMAGE_GUARD_FLAGS {
    IMAGE_GUARD_CF_INSTRUMENTED=256,
    IMAGE_GUARD_CFW_INSTRUMENTED=512,
    IMAGE_GUARD_CF_FUNCTION_TABLE_PRESENT=1024,
    IMAGE_GUARD_SECURITY_COOKIE_UNUSED=2048,
    IMAGE_GUARD_PROTECT_DELAYLOAD_IAT=4096,
    IMAGE_GUARD_DELAYLOAD_IAT_IN_ITS_OWN_SECTION=8192,
    IMAGE_GUARD_CF_EXPORT_SUPPRESSION_INFO_PRESENT=16384,
    IMAGE_GUARD_CF_ENABLE_EXPORT_SUPPRESSION=32768,
    IMAGE_GUARD_CF_LONGJUMP_TABLE_PRESENT=65536,
    IMAGE_GUARD_RF_INSTRUMENTED=131072,
    IMAGE_GUARD_RF_ENABLE=262144,
    IMAGE_GUARD_RF_STRICT=524288,
    IMAGE_GUARD_CF_FUNCTION_TABLE_SIZE_MASK_1=268435456,
    IMAGE_GUARD_CF_FUNCTION_TABLE_SIZE_MASK_2=536870912,
    IMAGE_GUARD_CF_FUNCTION_TABLE_SIZE_MASK_4=1073741824,
    IMAGE_GUARD_CF_FUNCTION_TABLE_SIZE_MASK_8=2147483648
} IMAGE_GUARD_FLAGS;

struct IMAGE_LOAD_CONFIG_DIRECTORY64 {
    dword Size;
    dword TimeDateStamp;
    word MajorVersion;
    word MinorVersion;
    dword GlobalFlagsClear;
    dword GlobalFlagsSet;
    dword CriticalSectionDefaultTimeout;
    qword DeCommitFreeBlockThreshold;
    qword DeCommitTotalFreeThreshold;
    pointer64 LockPrefixTable;
    qword MaximumAllocationSize;
    qword VirtualMemoryThreshold;
    qword ProcessAffinityMask;
    dword ProcessHeapFlags;
    word CsdVersion;
    word DependentLoadFlags;
    pointer64 EditList;
    pointer64 SecurityCookie;
    pointer64 SEHandlerTable;
    qword SEHandlerCount;
    pointer64 GuardCFCCheckFunctionPointer;
    pointer64 GuardCFDispatchFunctionPointer;
    pointer64 GuardCFFunctionTable;
    qword GuardCFFunctionCount;
    enum IMAGE_GUARD_FLAGS GuardFlags;
    struct IMAGE_LOAD_CONFIG_CODE_INTEGRITY CodeIntegrity;
    pointer64 GuardAddressTakenIatEntryTable;
    qword GuardAddressTakenIatEntryCount;
    pointer64 GuardLongJumpTargetTable;
    qword GuardLongJumpTargetCount;
    pointer64 DynamicValueRelocTable;
    pointer64 CHPEMetadataPointer;
    pointer64 GuardRFFailureRoutine;
    pointer64 GuardRFFailureRoutineFunctionPointer;
    dword DynamicValueRelocTableOffset;
    word DynamicValueRelocTableSection;
    word Reserved1;
    pointer64 GuardRFVerifyStackPointerFunctionPointer;
    dword HotPatchTableOffset;
    dword Reserved2;
    qword Reserved3;
};

typedef struct IMAGE_RESOURCE_DIRECTORY_ENTRY IMAGE_RESOURCE_DIRECTORY_ENTRY, *PIMAGE_RESOURCE_DIRECTORY_ENTRY;

typedef union IMAGE_RESOURCE_DIRECTORY_ENTRY_NameUnion IMAGE_RESOURCE_DIRECTORY_ENTRY_NameUnion, *PIMAGE_RESOURCE_DIRECTORY_ENTRY_NameUnion;

union IMAGE_RESOURCE_DIRECTORY_ENTRY_NameUnion {
    struct IMAGE_RESOURCE_DIRECTORY_ENTRY_NameStruct IMAGE_RESOURCE_DIRECTORY_ENTRY_NameStruct;
    dword Name;
    word Id;
};

struct IMAGE_RESOURCE_DIRECTORY_ENTRY {
    union IMAGE_RESOURCE_DIRECTORY_ENTRY_NameUnion NameUnion;
    union IMAGE_RESOURCE_DIRECTORY_ENTRY_DirectoryUnion DirectoryUnion;
};

typedef struct IMAGE_OPTIONAL_HEADER64 IMAGE_OPTIONAL_HEADER64, *PIMAGE_OPTIONAL_HEADER64;

typedef struct IMAGE_DATA_DIRECTORY IMAGE_DATA_DIRECTORY, *PIMAGE_DATA_DIRECTORY;

struct IMAGE_DATA_DIRECTORY {
    ImageBaseOffset32 VirtualAddress;
    dword Size;
};

struct IMAGE_OPTIONAL_HEADER64 {
    word Magic;
    byte MajorLinkerVersion;
    byte MinorLinkerVersion;
    dword SizeOfCode;
    dword SizeOfInitializedData;
    dword SizeOfUninitializedData;
    ImageBaseOffset32 AddressOfEntryPoint;
    ImageBaseOffset32 BaseOfCode;
    pointer64 ImageBase;
    dword SectionAlignment;
    dword FileAlignment;
    word MajorOperatingSystemVersion;
    word MinorOperatingSystemVersion;
    word MajorImageVersion;
    word MinorImageVersion;
    word MajorSubsystemVersion;
    word MinorSubsystemVersion;
    dword Win32VersionValue;
    dword SizeOfImage;
    dword SizeOfHeaders;
    dword CheckSum;
    word Subsystem;
    word DllCharacteristics;
    qword SizeOfStackReserve;
    qword SizeOfStackCommit;
    qword SizeOfHeapReserve;
    qword SizeOfHeapCommit;
    dword LoaderFlags;
    dword NumberOfRvaAndSizes;
    struct IMAGE_DATA_DIRECTORY DataDirectory[16];
};

typedef struct IMAGE_SECTION_HEADER IMAGE_SECTION_HEADER, *PIMAGE_SECTION_HEADER;

typedef union Misc Misc, *PMisc;

typedef enum SectionFlags {
    IMAGE_SCN_TYPE_NO_PAD=8,
    IMAGE_SCN_RESERVED_0001=16,
    IMAGE_SCN_CNT_CODE=32,
    IMAGE_SCN_CNT_INITIALIZED_DATA=64,
    IMAGE_SCN_CNT_UNINITIALIZED_DATA=128,
    IMAGE_SCN_LNK_OTHER=256,
    IMAGE_SCN_LNK_INFO=512,
    IMAGE_SCN_RESERVED_0040=1024,
    IMAGE_SCN_LNK_REMOVE=2048,
    IMAGE_SCN_LNK_COMDAT=4096,
    IMAGE_SCN_GPREL=32768,
    IMAGE_SCN_MEM_16BIT=131072,
    IMAGE_SCN_MEM_PURGEABLE=131072,
    IMAGE_SCN_MEM_LOCKED=262144,
    IMAGE_SCN_MEM_PRELOAD=524288,
    IMAGE_SCN_ALIGN_1BYTES=1048576,
    IMAGE_SCN_ALIGN_2BYTES=2097152,
    IMAGE_SCN_ALIGN_4BYTES=3145728,
    IMAGE_SCN_ALIGN_8BYTES=4194304,
    IMAGE_SCN_ALIGN_16BYTES=5242880,
    IMAGE_SCN_ALIGN_32BYTES=6291456,
    IMAGE_SCN_ALIGN_64BYTES=7340032,
    IMAGE_SCN_ALIGN_128BYTES=8388608,
    IMAGE_SCN_ALIGN_256BYTES=9437184,
    IMAGE_SCN_ALIGN_512BYTES=10485760,
    IMAGE_SCN_ALIGN_1024BYTES=11534336,
    IMAGE_SCN_ALIGN_2048BYTES=12582912,
    IMAGE_SCN_ALIGN_4096BYTES=13631488,
    IMAGE_SCN_ALIGN_8192BYTES=14680064,
    IMAGE_SCN_LNK_NRELOC_OVFL=16777216,
    IMAGE_SCN_MEM_DISCARDABLE=33554432,
    IMAGE_SCN_MEM_NOT_CACHED=67108864,
    IMAGE_SCN_MEM_NOT_PAGED=134217728,
    IMAGE_SCN_MEM_SHARED=268435456,
    IMAGE_SCN_MEM_EXECUTE=536870912,
    IMAGE_SCN_MEM_READ=1073741824,
    IMAGE_SCN_MEM_WRITE=2147483648
} SectionFlags;

union Misc {
    dword PhysicalAddress;
    dword VirtualSize;
};

struct IMAGE_SECTION_HEADER {
    char Name[8];
    union Misc Misc;
    ImageBaseOffset32 VirtualAddress;
    dword SizeOfRawData;
    dword PointerToRawData;
    dword PointerToRelocations;
    dword PointerToLinenumbers;
    word NumberOfRelocations;
    word NumberOfLinenumbers;
    enum SectionFlags Characteristics;
};

typedef struct IMAGE_NT_HEADERS64 IMAGE_NT_HEADERS64, *PIMAGE_NT_HEADERS64;

struct IMAGE_NT_HEADERS64 {
    char Signature[4];
    struct IMAGE_FILE_HEADER FileHeader;
    struct IMAGE_OPTIONAL_HEADER64 OptionalHeader;
};

typedef struct IMAGE_BASE_RELOCATION IMAGE_BASE_RELOCATION, *PIMAGE_BASE_RELOCATION;

struct IMAGE_BASE_RELOCATION {
    dword VirtualAddress;
    dword SizeOfBlock;
};

typedef struct IMAGE_RESOURCE_DATA_ENTRY IMAGE_RESOURCE_DATA_ENTRY, *PIMAGE_RESOURCE_DATA_ENTRY;

struct IMAGE_RESOURCE_DATA_ENTRY {
    dword OffsetToData;
    dword Size;
    dword CodePage;
    dword Reserved;
};

typedef struct IMAGE_RESOURCE_DIRECTORY IMAGE_RESOURCE_DIRECTORY, *PIMAGE_RESOURCE_DIRECTORY;

struct IMAGE_RESOURCE_DIRECTORY {
    dword Characteristics;
    dword TimeDateStamp;
    word MajorVersion;
    word MinorVersion;
    word NumberOfNamedEntries;
    word NumberOfIdEntries;
};

typedef struct IMAGE_DIRECTORY_ENTRY_EXPORT IMAGE_DIRECTORY_ENTRY_EXPORT, *PIMAGE_DIRECTORY_ENTRY_EXPORT;

struct IMAGE_DIRECTORY_ENTRY_EXPORT {
    dword Characteristics;
    dword TimeDateStamp;
    word MajorVersion;
    word MinorVersion;
    ImageBaseOffset32 Name;
    dword Base;
    dword NumberOfFunctions;
    dword NumberOfNames;
    ImageBaseOffset32 AddressOfFunctions;
    ImageBaseOffset32 AddressOfNames;
    ImageBaseOffset32 AddressOfNameOrdinals;
};

typedef struct _iobuf _iobuf, *P_iobuf;

struct _iobuf {
    char *_ptr;
    int _cnt;
    char *_base;
    int _flag;
    int _file;
    int _charbuf;
    int _bufsiz;
    char *_tmpfname;
};

typedef struct _iobuf FILE;

typedef int PMFN;

typedef struct _s_ThrowInfo _s_ThrowInfo, *P_s_ThrowInfo;

struct _s_ThrowInfo {
    uint attributes;
    PMFN pmfnUnwind;
    int pForwardCompat;
    int pCatchableTypeArray;
};

typedef struct TypeDescriptor TypeDescriptor, *PTypeDescriptor;

struct TypeDescriptor {
    void *pVFTable;
    void *spare;
    char name[0];
};

typedef struct _s_ThrowInfo ThrowInfo;

typedef ulonglong uintptr_t;

typedef ulonglong size_t;

typedef int errno_t;

typedef size_t rsize_t;



undefined DAT_1800081f8;
pointer[2] vftable;
undefined DAT_180007748;
ulonglong DAT_180008040;
FILE *DAT_1800081c0;
HMODULE DAT_1800081c8;
FARPROC DAT_1800081e0;
FARPROC DAT_1800081d0;
FARPROC DAT_1800081f0;
FARPROC DAT_1800081b8;
FARPROC DAT_1800081e8;
HMODULE DAT_1800081d8;
undefined1 DAT_180006a74;
undefined DAT_180006a76;
undefined *DAT_1800081e0;
undefined *DAT_1800081d0;
undefined *DAT_1800081f0;
undefined *DAT_1800081b8;
undefined *DAT_1800081e8;
undefined DAT_1800068b4;
undefined FUN_1800016d0;
undefined8 DAT_1800081c0;
undefined UNK_180002148;
uint DAT_180008004;
undefined DAT_180006760;
uintptr_t DAT_180008040;
pointer[1] vftable;
int DAT_180008168;
int DAT_180008140;
undefined DAT_180004220;
undefined DAT_180004228;
undefined DAT_180004230;
undefined _guard_dispatch_icall;
undefined DAT_180004238;
undefined DAT_180008008;
undefined DAT_180008010;
uint DAT_180008144;
ulonglong DAT_180008018;
undefined DAT_180008000;
undefined DAT_180008148;
undefined DAT_180007678;
undefined DAT_180008080;
undefined DAT_180008150;
undefined DAT_180008160;
void *DAT_180008170;
void *StackBase;
undefined DAT_180008180;
undefined1 DAT_180008178;
char DAT_180008179;
undefined DAT_180008190;
undefined DAT_180008198;
undefined DAT_1800081a8;
undefined8 UNK_180008188;
undefined8 UNK_1800081a0;
IMAGE_SECTION_HEADER IMAGE_SECTION_HEADER_180000210;
undefined DAT_180000300;
undefined8 DAT_180008170;
char DAT_180008178;
undefined DAT_180008200;
undefined8 DAT_180007300;
undefined8 DAT_180007310;
int DAT_180008090;
undefined FUN_180002a00;

undefined * FUN_180001000(void)

{
  return &DAT_1800081f8;
}



// Library Function - Single Match
//  snprintf
// 
// Library: Visual Studio 2019 Release

int snprintf(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  int iVar1;
  ulonglong *puVar2;
  undefined8 local_res20;
  
  local_res20 = param_4;
  puVar2 = (ulonglong *)FUN_180001000();
  iVar1 = __stdio_common_vsprintf(*puVar2 | 2,param_1,param_2,param_3,0,&local_res20);
  if (iVar1 < 0) {
    iVar1 = -1;
  }
  return iVar1;
}



undefined8 * FUN_180001070(undefined8 *param_1,longlong param_2)

{
  *param_1 = std::exception::vftable;
  param_1[1] = 0;
  param_1[2] = 0;
  __std_exception_copy(param_2 + 8);
  return param_1;
}



char * FUN_1800010b0(longlong param_1)

{
  char *pcVar1;
  
  pcVar1 = "Unknown exception";
  if (*(char **)(param_1 + 8) != (char *)0x0) {
    pcVar1 = *(char **)(param_1 + 8);
  }
  return pcVar1;
}



undefined8 * FUN_1800010d0(undefined8 *param_1,ulonglong param_2)

{
  *param_1 = std::exception::vftable;
  __std_exception_destroy(param_1 + 1);
  if ((param_2 & 1) != 0) {
    free(param_1);
  }
  return param_1;
}



undefined8 * FUN_180001140(undefined8 *param_1)

{
  param_1[2] = 0;
  param_1[1] = "bad array new length";
  *param_1 = std::bad_array_new_length::vftable;
  return param_1;
}



void FUN_180001170(void)

{
  undefined8 local_28 [5];
  
  FUN_180001140(local_28);
                    // WARNING: Subroutine does not return
  _CxxThrowException(local_28,(ThrowInfo *)&DAT_180007748);
}



undefined8 * FUN_180001190(undefined8 *param_1,longlong param_2)

{
  *param_1 = std::exception::vftable;
  param_1[1] = 0;
  param_1[2] = 0;
  __std_exception_copy(param_2 + 8);
  *param_1 = std::bad_array_new_length::vftable;
  return param_1;
}



undefined8 * FUN_1800011d0(undefined8 *param_1,longlong param_2)

{
  *param_1 = std::exception::vftable;
  param_1[1] = 0;
  param_1[2] = 0;
  __std_exception_copy(param_2 + 8);
  *param_1 = std::bad_alloc::vftable;
  return param_1;
}



void FUN_180001210(void)

{
  code *pcVar1;
  
  std::_Xlength_error("string too long");
  pcVar1 = (code *)swi(3);
  (*pcVar1)();
  return;
}



// WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie

void FUN_180001230(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  int iVar1;
  ulonglong *puVar2;
  FILE *pFVar3;
  int iVar4;
  undefined8 local_res10;
  undefined8 local_res18;
  undefined8 local_res20;
  undefined1 auStack_478 [32];
  undefined8 local_458;
  undefined8 *local_450;
  uint local_448;
  _SYSTEMTIME local_438;
  char local_428 [1024];
  ulonglong local_28;
  
  local_28 = DAT_180008040 ^ (ulonglong)auStack_478;
  local_438.wYear = 0;
  local_438.wMonth = 0;
  local_438.wDayOfWeek = 0;
  local_438.wDay = 0;
  local_438.wHour = 0;
  local_438.wMinute = 0;
  local_438.wSecond = 0;
  local_438.wMilliseconds = 0;
  local_res10 = param_2;
  local_res18 = param_3;
  local_res20 = param_4;
  GetLocalTime(&local_438);
  local_448 = (uint)local_438.wMilliseconds;
  local_450 = (undefined8 *)CONCAT44(local_450._4_4_,(uint)local_438.wSecond);
  local_458 = CONCAT44(local_458._4_4_,(uint)local_438.wMinute);
  iVar1 = snprintf(local_428,0x400,"[%02d:%02d:%02d.%03d] ",local_438._8_8_ & 0xffff);
  iVar4 = 0;
  if (-1 < iVar1) {
    iVar4 = iVar1;
  }
  puVar2 = (ulonglong *)FUN_180001000();
  local_458 = 0;
  local_450 = &local_res10;
  __stdio_common_vsprintf(*puVar2 | 2,local_428 + iVar4,0x400 - (longlong)iVar4,param_1);
  if (DAT_1800081c0 != (FILE *)0x0) {
    fputs(local_428,DAT_1800081c0);
    fflush(DAT_1800081c0);
  }
  pFVar3 = (FILE *)__acrt_iob_func(1);
  fputs(local_428,pFVar3);
  pFVar3 = (FILE *)__acrt_iob_func(1);
  fflush(pFVar3);
  return;
}



// WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie

void FUN_180001370(void)

{
  longlong lVar1;
  code *pcVar2;
  size_t sVar3;
  ulonglong uVar4;
  undefined8 ***pppuVar5;
  undefined8 ****ppppuVar6;
  LPCWSTR ***ppppWVar7;
  size_t sVar8;
  undefined1 *puVar9;
  size_t sVar10;
  undefined8 in_R9;
  undefined1 auStackY_2c8 [8];
  undefined1 auStackY_2c0 [24];
  undefined8 ***local_290;
  undefined8 uStack_288;
  size_t local_280;
  ulonglong uStack_278;
  LPCWSTR **local_270;
  undefined8 **ppuStack_268;
  undefined8 **local_260;
  undefined8 **ppuStack_258;
  WCHAR local_248 [264];
  ulonglong local_38;
  
  puVar9 = auStackY_2c8;
  local_38 = DAT_180008040 ^ (ulonglong)auStackY_2c8;
  ppppuVar6 = (undefined8 ****)0x0;
  if (DAT_1800081c8 != (HMODULE)0x0) goto LAB_18000169e;
  memset(local_248,0,0x208);
  GetSystemDirectoryW(local_248,0x104);
  local_290 = (undefined8 ****)0x0;
  uStack_288 = 0;
  local_280 = 0;
  uStack_278 = 0;
  sVar3 = wcslen(local_248);
  if (0x7ffffffffffffffe < sVar3) {
                    // WARNING: Subroutine does not return
    FUN_180001210();
  }
  if (sVar3 < 8) {
    uStack_278 = 7;
    sVar8 = sVar3 * 2;
    sVar10 = sVar8;
    local_280 = sVar3;
    memcpy(&local_290,local_248,sVar8);
    *(undefined2 *)((longlong)&local_290 + sVar8) = 0;
LAB_1800014ce:
    if (uStack_278 - local_280 < 0xc) {
      ppppuVar6 = (undefined8 ****)FUN_180002150(&local_290,0xc,sVar10,in_R9,0xc);
    }
    else {
      sVar3 = local_280 + 0xc;
      ppppuVar6 = &local_290;
      if (7 < uStack_278) {
        ppppuVar6 = (undefined8 ****)local_290;
      }
      lVar1 = local_280 * 2;
      local_280 = sVar3;
      memmove((void *)((longlong)ppppuVar6 + lVar1),L"\\dinput8.dll",0x18);
      *(undefined2 *)((longlong)ppppuVar6 + sVar3 * 2) = 0;
      ppppuVar6 = &local_290;
    }
    local_270 = (LPCWSTR **)0x0;
    ppuStack_268 = (undefined8 **)0x0;
    local_260 = (undefined8 **)0x0;
    ppuStack_258 = (undefined8 **)0x0;
    local_270 = (LPCWSTR **)*ppppuVar6;
    ppuStack_268 = ppppuVar6[1];
    local_260 = ppppuVar6[2];
    ppuStack_258 = ppppuVar6[3];
    ppppuVar6[2] = (undefined8 ***)0x0;
    ppppuVar6[3] = (undefined8 ***)0x7;
    *(undefined2 *)ppppuVar6 = 0;
    if (7 < uStack_278) {
      ppppuVar6 = (undefined8 ****)local_290;
      if ((0xfff < uStack_278 * 2 + 2) &&
         (ppppuVar6 = (undefined8 ****)local_290[-1],
         0x1f < (ulonglong)((longlong)local_290 + (-8 - (longlong)ppppuVar6)))) goto LAB_18000168f;
      free(ppppuVar6);
    }
    puVar9 = auStackY_2c8;
    ppppWVar7 = &local_270;
    if ((undefined8 ***)0x7 < ppuStack_258) {
      ppppWVar7 = (LPCWSTR ***)local_270;
    }
    DAT_1800081c8 = LoadLibraryW((LPCWSTR)ppppWVar7);
    if (DAT_1800081c8 != (HMODULE)0x0) {
      DAT_1800081e0 = GetProcAddress(DAT_1800081c8,"DirectInput8Create");
      DAT_1800081d0 = GetProcAddress(DAT_1800081c8,"DllCanUnloadNow");
      DAT_1800081f0 = GetProcAddress(DAT_1800081c8,"DllGetClassObject");
      DAT_1800081b8 = GetProcAddress(DAT_1800081c8,"DllRegisterServer");
      DAT_1800081e8 = GetProcAddress(DAT_1800081c8,"DllUnregisterServer");
    }
    if (ppuStack_258 < (undefined8 ***)0x8) goto LAB_18000169e;
    ppppWVar7 = (LPCWSTR ***)local_270;
    puVar9 = auStackY_2c8;
    if ((0xfff < (longlong)ppuStack_258 * 2 + 2U) &&
       (ppppWVar7 = (LPCWSTR ***)local_270[-1], puVar9 = auStackY_2c8,
       0x1f < (ulonglong)((longlong)local_270 + (-8 - (longlong)ppppWVar7)))) goto LAB_18000168f;
  }
  else {
    uVar4 = sVar3 | 7;
    if (uVar4 < 0x7fffffffffffffff) {
      if (uVar4 < 10) {
        uVar4 = 10;
      }
      if (0x7fffffffffffffff < uVar4 + 1) goto LAB_1800016c1;
      sVar8 = (uVar4 + 1) * 2;
      if (sVar8 != 0) goto LAB_18000143b;
LAB_1800014a8:
      sVar8 = sVar3 * 2;
      sVar10 = sVar8;
      local_290 = ppppuVar6;
      local_280 = sVar3;
      uStack_278 = uVar4;
      memcpy(ppppuVar6,local_248,sVar8);
      *(undefined2 *)(sVar8 + (longlong)ppppuVar6) = 0;
      goto LAB_1800014ce;
    }
    sVar8 = 0xfffffffffffffffe;
    uVar4 = 0x7ffffffffffffffe;
LAB_18000143b:
    if (sVar8 < 0x1000) {
      ppppuVar6 = (undefined8 ****)FUN_180002990(sVar8);
      goto LAB_1800014a8;
    }
    if (sVar8 + 0x27 <= sVar8) {
LAB_1800016c1:
                    // WARNING: Subroutine does not return
      FUN_180001170();
    }
    pppuVar5 = (undefined8 ***)FUN_180002990(sVar8 + 0x27);
    if (pppuVar5 != (undefined8 ***)0x0) {
      ppppuVar6 = (undefined8 ****)((longlong)pppuVar5 + 0x27U & 0xffffffffffffffe0);
      ppppuVar6[-1] = pppuVar5;
      goto LAB_1800014a8;
    }
LAB_18000168f:
    pcVar2 = (code *)swi(0x29);
    ppppWVar7 = (LPCWSTR ***)(*pcVar2)(5);
    puVar9 = auStackY_2c0;
  }
  *(undefined8 *)(puVar9 + -8) = 0x18000169e;
  free(ppppWVar7);
LAB_18000169e:
  *(undefined8 *)(puVar9 + -8) = 0x1800016ad;
  return;
}



// WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie

undefined4 FUN_1800016d0(HMODULE param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  code *pcVar1;
  DWORD DVar2;
  undefined4 uVar3;
  size_t sVar4;
  ulonglong uVar5;
  longlong lVar6;
  undefined1 (*pauVar7) [16];
  char cVar8;
  undefined1 (*pauVar9) [16];
  ulonglong uVar10;
  undefined1 *puVar11;
  ulonglong uVar12;
  undefined1 *puVar13;
  undefined1 *puVar14;
  undefined1 *_Size;
  undefined1 (*pauVar15) [16];
  byte *pbVar16;
  undefined1 *puVar17;
  char *pcVar18;
  char **ppcVar19;
  undefined8 uStack_2f0;
  undefined1 auStack_2e8 [8];
  undefined1 auStack_2e0 [24];
  undefined1 (*local_2c8) [16];
  char *local_2c0 [3];
  char *local_2a8 [32];
  undefined1 (*local_1a8) [16];
  undefined8 uStack_1a0;
  undefined1 *local_198;
  ulonglong uStack_190;
  undefined1 (*local_188) [16];
  undefined8 uStack_180;
  undefined1 *local_178;
  ulonglong uStack_170;
  CHAR local_168 [272];
  ulonglong local_58;
  
  puVar13 = auStack_2e8;
  puVar14 = auStack_2e8;
  local_58 = DAT_180008040 ^ (ulonglong)auStack_2e8;
  memset(local_168,0,0x104);
  DVar2 = GetModuleFileNameA(param_1,local_168,0x104);
  if (DVar2 == 0) {
    uVar3 = 1;
    puVar14 = auStack_2e8;
    goto LAB_180001735;
  }
  local_188 = (undefined1 (*) [16])0x0;
  uStack_180 = 0;
  local_178 = (undefined1 *)0x0;
  uStack_170 = 0;
  sVar4 = strlen(local_168);
  FUN_180002050(&local_188,local_168,sVar4);
  uVar12 = uStack_170;
  _Size = local_178;
  pauVar7 = local_188;
  pauVar15 = (undefined1 (*) [16])&local_188;
  if (0xf < uStack_170) {
    pauVar15 = local_188;
  }
  puVar11 = (undefined1 *)0xffffffffffffffff;
  if (local_178 != (undefined1 *)0x0) {
    puVar17 = (undefined1 *)0xffffffffffffffff;
    if (local_178 + -1 != (undefined1 *)0xffffffffffffffff) {
      puVar17 = local_178 + -1;
    }
    if (puVar17 + 3 < (undefined1 *)0x10) {
      memset(local_2a8,0,0x100);
      pbVar16 = &DAT_180006a74;
      do {
        *(undefined1 *)((longlong)local_2a8 + (ulonglong)*pbVar16) = 1;
        pbVar16 = pbVar16 + 1;
      } while (pbVar16 != &DAT_180006a76);
      pauVar9 = (undefined1 (*) [16])(puVar17 + (longlong)pauVar15);
      cVar8 = *(char *)((longlong)local_2a8 + (ulonglong)(byte)(*pauVar9)[0]);
      while (cVar8 == '\0') {
        if (pauVar9 == pauVar15) goto LAB_180001858;
        pauVar9 = (undefined1 (*) [16])(pauVar9[-1] + 0xf);
        cVar8 = *(char *)((longlong)local_2a8 + (ulonglong)(byte)(*pauVar9)[0]);
      }
      puVar11 = (undefined1 *)((longlong)pauVar9 - (longlong)pauVar15);
    }
    else {
      param_4 = 2;
      puVar11 = thunk_FUN_180002300(pauVar15,puVar17 + 1,(undefined1 (*) [16])&DAT_180006a74,2);
      _Size = local_178;
      uVar12 = uStack_170;
      pauVar7 = local_188;
    }
  }
LAB_180001858:
  if (puVar11 == (undefined1 *)0xffffffffffffffff) {
LAB_1800018f8:
    local_2c0[0] = "scripthook.dll";
    local_2c0[1] = "CTOSOVERLOAD.dll";
    local_2c0[2] = "MyLoader.dll";
    ppcVar19 = local_2c0;
    do {
      pauVar15 = (undefined1 (*) [16])0x0;
      sVar4 = strlen(*ppcVar19);
      if (0x7fffffffffffffffU - (longlong)_Size < sVar4) {
                    // WARNING: Subroutine does not return
        FUN_180001210();
      }
      local_2c8 = (undefined1 (*) [16])&local_188;
      if (0xf < uVar12) {
        local_2c8 = pauVar7;
      }
      local_1a8 = (undefined1 (*) [16])0x0;
      uStack_1a0 = 0;
      local_198 = (undefined1 *)0x0;
      uStack_190 = 0;
      puVar11 = _Size + sVar4;
      if (puVar11 < (undefined1 *)0x10) {
        uVar12 = 0xf;
        pauVar15 = (undefined1 (*) [16])&local_1a8;
        pauVar7 = local_1a8;
      }
      else {
        uVar12 = (ulonglong)puVar11 | 0xf;
        if (uVar12 < 0x8000000000000000) {
          if (uVar12 < 0x16) {
            uVar12 = 0x16;
          }
          uVar10 = uVar12 + 1;
          pauVar7 = pauVar15;
          if (uVar10 != 0) {
            if (0xfff < uVar10) {
              uVar5 = uVar12 + 0x28;
              if (uVar5 <= uVar10) {
                    // WARNING: Subroutine does not return
                FUN_180001170();
              }
              goto LAB_1800019bb;
            }
            pauVar15 = (undefined1 (*) [16])FUN_180002990(uVar10);
            pauVar7 = pauVar15;
          }
        }
        else {
          uVar12 = 0x7fffffffffffffff;
          uVar5 = 0x8000000000000027;
LAB_1800019bb:
          lVar6 = FUN_180002990(uVar5);
          if (lVar6 == 0) goto LAB_180001b8c;
          pauVar15 = (undefined1 (*) [16])(lVar6 + 0x27U & 0xffffffffffffffe0);
          *(longlong *)(pauVar15[-1] + 8) = lVar6;
          pauVar7 = pauVar15;
        }
      }
      local_1a8 = pauVar7;
      local_198 = puVar11;
      uStack_190 = uVar12;
      memcpy(pauVar15,local_2c8,(size_t)_Size);
      memcpy(_Size + (longlong)*pauVar15,*ppcVar19,sVar4);
      puVar11[(longlong)pauVar15] = 0;
      pauVar7 = (undefined1 (*) [16])&local_1a8;
      if (0xf < uStack_190) {
        pauVar7 = local_1a8;
      }
      FUN_180001230("[dinput8] Loading payload: %s\n",pauVar7,sVar4,param_4);
      pauVar7 = (undefined1 (*) [16])&local_1a8;
      if (0xf < uStack_190) {
        pauVar7 = local_1a8;
      }
      DAT_1800081d8 = LoadLibraryA((LPCSTR)pauVar7);
      if (DAT_1800081d8 != (HMODULE)0x0) {
        FUN_180001230("[dinput8] Payload loaded @ %p\n",DAT_1800081d8,sVar4,param_4);
        puVar14 = auStack_2e8;
        if (uStack_190 < 0x10) goto LAB_180001b9b;
        pauVar7 = local_1a8;
        if ((uStack_190 + 1 < 0x1000) ||
           (pauVar7 = *(undefined1 (**) [16])(local_1a8[-1] + 8), puVar13 = auStack_2e8,
           (LPCSTR)((longlong)local_1a8 +
                   (-8 - (longlong)*(undefined1 (**) [16])(local_1a8[-1] + 8))) < (LPCSTR)0x20))
        goto LAB_180001b96;
        goto LAB_180001b8c;
      }
      DVar2 = GetLastError();
      if (DVar2 == 5) {
        pcVar18 = "5 = access denied";
        cVar8 = '5';
      }
      else if (DVar2 == 0x7e) {
        pcVar18 = "126 = module or dependency missing (file absent or VC++ runtime?)";
        cVar8 = '1';
      }
      else if (DVar2 == 0xe1) {
        pcVar18 = 
        "225 = blocked by Windows Defender / AV (add bin folder exclusion, restore quarantined DLL)"
        ;
        cVar8 = '2';
      }
      else {
        pcVar18 = "";
        cVar8 = '\0';
      }
      if (cVar8 == '\0') {
        FUN_180001230("[dinput8] Load failed (GetLastError=%lu)\n",(ulonglong)DVar2,pcVar18,param_4)
        ;
      }
      else {
        FUN_180001230("[dinput8] Load failed (GetLastError=%lu) %s\n",(ulonglong)DVar2,pcVar18,
                      param_4);
      }
      uVar10 = uStack_190;
      if (0xf < uStack_190) {
        uVar10 = uStack_190 + 1;
        pauVar7 = local_1a8;
        if (0xfff < uVar10) {
          pauVar7 = *(undefined1 (**) [16])(local_1a8[-1] + 8);
          if ((LPCSTR)0x1f < (LPCSTR)((longlong)local_1a8 + (-8 - (longlong)pauVar7)))
          goto LAB_180001b8c;
          uVar10 = uStack_190 + 0x28;
        }
        free(pauVar7);
      }
      ppcVar19 = ppcVar19 + 1;
      _Size = local_178;
      uVar12 = uStack_170;
      pauVar7 = local_188;
    } while (ppcVar19 != local_2a8);
    FUN_180001230("[dinput8] ERROR: no payload DLL found beside dinput8.dll\n",uVar10,pcVar18,
                  param_4);
    uVar3 = 2;
  }
  else {
    local_1a8 = (undefined1 (*) [16])0x0;
    uStack_1a0 = 0;
    local_198 = (undefined1 *)0x0;
    uStack_190 = 0;
    puVar17 = puVar11 + 1;
    if (_Size < puVar11 + 1) {
      puVar17 = _Size;
    }
    pauVar15 = (undefined1 (*) [16])&local_188;
    if (0xf < uVar12) {
      pauVar15 = pauVar7;
    }
    FUN_180002050(&local_1a8,pauVar15,(size_t)puVar17);
    if (uStack_170 < 0x10) {
LAB_1800018d0:
      local_188 = local_1a8;
      uStack_180 = uStack_1a0;
      local_178 = local_198;
      uStack_170 = uStack_190;
      _Size = local_198;
      uVar12 = uStack_190;
      pauVar7 = local_1a8;
      goto LAB_1800018f8;
    }
    pauVar7 = local_188;
    if ((uStack_170 + 1 < 0x1000) ||
       (pauVar7 = *(undefined1 (**) [16])((longlong)local_188[-1] + 8),
       (LPCSTR)((longlong)local_188 + (-8 - (longlong)pauVar7)) < (LPCSTR)0x20)) {
      free(pauVar7);
      goto LAB_1800018d0;
    }
LAB_180001b8c:
    pcVar1 = (code *)swi(0x29);
    pauVar7 = (undefined1 (*) [16])(*pcVar1)(5);
    puVar13 = auStack_2e0;
LAB_180001b96:
    *(undefined8 *)(puVar13 + -8) = 0x180001b9b;
    free(pauVar7);
    puVar14 = puVar13;
LAB_180001b9b:
    uVar3 = 0;
  }
  if (0xf < uStack_170) {
    pauVar7 = local_188;
    if (0xfff < uStack_170 + 1) {
      pauVar7 = *(undefined1 (**) [16])((longlong)local_188[-1] + 8);
      if ((LPCSTR)((longlong)local_188 + (-8 - (longlong)pauVar7)) < (LPCSTR)0x20) {
        *(undefined8 *)(puVar14 + -8) = 0x180001bd3;
        free(pauVar7);
        goto LAB_180001735;
      }
      pcVar1 = (code *)swi(0x29);
      pauVar7 = (undefined1 (*) [16])(*pcVar1)(5);
      puVar14 = puVar14 + 8;
    }
    *(undefined8 *)(puVar14 + -8) = 0x180001be9;
    free(pauVar7);
  }
LAB_180001735:
  *(undefined8 *)(puVar14 + -8) = 0x180001744;
  return uVar3;
}



undefined8
DirectInput8Create(undefined8 param_1,undefined4 param_2,undefined8 param_3,undefined8 param_4)

{
  undefined8 uVar1;
  
                    // 0x1c00  1  DirectInput8Create
  FUN_180001370();
  if (DAT_1800081e0 == (code *)0x0) {
    return 0x80004005;
  }
                    // WARNING: Could not recover jumptable at 0x000180001c46. Too many branches
                    // WARNING: Treating indirect jump as call
  uVar1 = (*DAT_1800081e0)(param_1,param_2,param_3,param_4);
  return uVar1;
}



HRESULT __stdcall DllCanUnloadNow(void)

{
  HRESULT HVar1;
  
                    // 0x1c50  2  DllCanUnloadNow
  FUN_180001370();
  if (DAT_1800081d0 == (code *)0x0) {
    return 1;
  }
                    // WARNING: Could not recover jumptable at 0x000180001c73. Too many branches
                    // WARNING: Treating indirect jump as call
  HVar1 = (*DAT_1800081d0)();
  return HVar1;
}



HRESULT __stdcall DllGetClassObject(IID *rclsid,IID *riid,LPVOID *ppv)

{
  HRESULT HVar1;
  
                    // 0x1c80  3  DllGetClassObject
  FUN_180001370();
  if (DAT_1800081f0 == (code *)0x0) {
    return -0x7ffbfeef;
  }
                    // WARNING: Could not recover jumptable at 0x000180001cd6. Too many branches
                    // WARNING: Treating indirect jump as call
  HVar1 = (*DAT_1800081f0)(rclsid,riid,ppv);
  return HVar1;
}



undefined8 DllRegisterServer(void)

{
  undefined8 uVar1;
  
                    // 0x1ce0  4  DllRegisterServer
  FUN_180001370();
  if (DAT_1800081b8 == (code *)0x0) {
    return 0x80004005;
  }
                    // WARNING: Could not recover jumptable at 0x000180001d03. Too many branches
                    // WARNING: Treating indirect jump as call
  uVar1 = (*DAT_1800081b8)();
  return uVar1;
}



undefined8 DllUnregisterServer(void)

{
  undefined8 uVar1;
  
                    // 0x1d10  5  DllUnregisterServer
  FUN_180001370();
  if (DAT_1800081e8 == (code *)0x0) {
    return 0x80004005;
  }
                    // WARNING: Could not recover jumptable at 0x000180001d33. Too many branches
                    // WARNING: Treating indirect jump as call
  uVar1 = (*DAT_1800081e8)();
  return uVar1;
}



// WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie

undefined8 FUN_180001d40(HMODULE param_1,int param_2)

{
  FILE *pFVar1;
  HWND hWnd;
  char *pcVar2;
  undefined *puVar3;
  undefined1 auStackY_168 [32];
  FILE *local_138 [2];
  CHAR local_128 [272];
  ulonglong local_18;
  
  local_18 = DAT_180008040 ^ (ulonglong)auStackY_168;
  if (param_2 == 1) {
    DisableThreadLibraryCalls(param_1);
    AllocConsole();
    SetConsoleTitleA("EncryptedStudios Dev Console - Watch Dogs Legion");
    local_138[0] = (FILE *)0x0;
    pFVar1 = (FILE *)__acrt_iob_func(1);
    freopen_s(local_138,"CONOUT$","w",pFVar1);
    pFVar1 = (FILE *)__acrt_iob_func(2);
    freopen_s(local_138,"CONOUT$","w",pFVar1);
    pFVar1 = (FILE *)__acrt_iob_func(0);
    freopen_s(local_138,"CONIN$","r",pFVar1);
    hWnd = GetConsoleWindow();
    if (hWnd != (HWND)0x0) {
      ShowWindow(hWnd,5);
    }
    memset(local_128,0,0x104);
    GetModuleFileNameA(param_1,local_128,0x104);
    pcVar2 = strrchr(local_128,0x5c);
    if ((pcVar2 == (char *)0x0) && (pcVar2 = strrchr(local_128,0x2f), pcVar2 == (char *)0x0)) {
      strcpy_s(local_128,0x104,"EncryptedStudios_boot.log");
    }
    else {
      pcVar2[1] = '\0';
      strcat_s(local_128,0x104,"EncryptedStudios_boot.log");
    }
    puVar3 = &DAT_1800068b4;
    pcVar2 = local_128;
    fopen_s(&DAT_1800081c0,pcVar2,"at");
    FUN_180001230("========== EXE LAUNCH / dinput8 attached ==========\n",pcVar2,puVar3,pFVar1);
    FUN_180001230("[dinput8] Boot logger + console ready\n",pcVar2,puVar3,pFVar1);
    FUN_180001370();
    CreateThread((LPSECURITY_ATTRIBUTES)0x0,0,FUN_1800016d0,param_1,0,(LPDWORD)0x0);
    return 1;
  }
  if (param_2 != 0) {
    return 1;
  }
  if (DAT_1800081c0 != (FILE *)0x0) {
    fclose(DAT_1800081c0);
    DAT_1800081c0 = (FILE *)0x0;
  }
  if (DAT_1800081d8 != (HMODULE)0x0) {
    FreeLibrary(DAT_1800081d8);
    DAT_1800081d8 = (HMODULE)0x0;
  }
  if (DAT_1800081c8 == (HMODULE)0x0) {
    return 1;
  }
  FreeLibrary(DAT_1800081c8);
  DAT_1800081c8 = (HMODULE)0x0;
  return 1;
}



void FUN_180001f70(longlong *param_1)

{
  void *pvVar1;
  code *pcVar2;
  void *_Memory;
  undefined1 *puVar3;
  undefined1 auStack_28 [8];
  undefined1 auStack_20 [24];
  
  if (7 < (ulonglong)param_1[3]) {
    pvVar1 = (void *)*param_1;
    _Memory = pvVar1;
    puVar3 = auStack_28;
    if ((0xfff < param_1[3] * 2 + 2U) &&
       (_Memory = *(void **)((longlong)pvVar1 + -8), puVar3 = auStack_28,
       0x1f < (ulonglong)((longlong)pvVar1 + (-8 - (longlong)_Memory)))) {
      pcVar2 = (code *)swi(0x29);
      _Memory = (void *)(*pcVar2)(5);
      puVar3 = auStack_20;
    }
    *(undefined8 *)(puVar3 + -8) = 0x180001fbd;
    free(_Memory);
  }
  param_1[2] = 0;
  *(undefined2 *)param_1 = 0;
  param_1[3] = 7;
  return;
}



void FUN_180001fe0(longlong *param_1)

{
  void *pvVar1;
  code *pcVar2;
  void *_Memory;
  undefined1 *puVar3;
  undefined1 auStack_28 [8];
  undefined1 auStack_20 [24];
  
  if (0xf < (ulonglong)param_1[3]) {
    pvVar1 = (void *)*param_1;
    _Memory = pvVar1;
    puVar3 = auStack_28;
    if ((0xfff < param_1[3] + 1U) &&
       (_Memory = *(void **)((longlong)pvVar1 + -8), puVar3 = auStack_28,
       0x1f < (ulonglong)((longlong)pvVar1 + (-8 - (longlong)_Memory)))) {
      pcVar2 = (code *)swi(0x29);
      _Memory = (void *)(*pcVar2)(5);
      puVar3 = auStack_20;
    }
    *(undefined8 *)(puVar3 + -8) = 0x180002028;
    free(_Memory);
  }
  param_1[2] = 0;
  param_1[3] = 0xf;
  *(undefined1 *)param_1 = 0;
  return;
}



void FUN_180002050(undefined8 *param_1,void *param_2,size_t param_3)

{
  ulonglong uVar1;
  code *pcVar2;
  ulonglong uVar3;
  ulonglong uVar4;
  longlong lVar5;
  void *_Dst;
  undefined1 *puVar6;
  undefined8 uStack_50;
  undefined1 auStack_48 [40];
  
  puVar6 = auStack_48;
  if (0x7fffffffffffffff < param_3) {
                    // WARNING: Subroutine does not return
    FUN_180001210();
  }
  if (param_3 < 0x10) {
    param_1[2] = param_3;
    param_1[3] = 0xf;
    memcpy(param_1,param_2,param_3);
    *(undefined1 *)(param_3 + (longlong)param_1) = 0;
    return;
  }
  uVar3 = param_3 | 0xf;
  if (uVar3 < 0x8000000000000000) goto LAB_1800020cb;
  uVar4 = 0x8000000000000027;
  puVar6 = auStack_48;
  uVar3 = 0x7fffffffffffffff;
  do {
    *(undefined8 *)(puVar6 + -8) = 0x1800020bf;
    lVar5 = FUN_180002990(uVar4);
    if (lVar5 != 0) {
      _Dst = (void *)(lVar5 + 0x27U & 0xffffffffffffffe0);
      *(longlong *)((longlong)_Dst - 8) = lVar5;
LAB_180002111:
      *param_1 = _Dst;
      param_1[2] = param_3;
      param_1[3] = uVar3;
      *(undefined8 *)(puVar6 + -8) = 0x18000212a;
      memcpy(_Dst,param_2,param_3);
      *(undefined1 *)((longlong)_Dst + param_3) = 0;
      return;
    }
    pcVar2 = (code *)swi(0x29);
    uVar3 = (*pcVar2)(5);
    puVar6 = puVar6 + 8;
LAB_1800020cb:
    if (uVar3 < 0x16) {
      uVar3 = 0x16;
    }
    uVar1 = uVar3 + 1;
    if (uVar1 == 0) {
      _Dst = (void *)0x0;
      goto LAB_180002111;
    }
    if (uVar1 < 0x1000) {
      *(undefined8 *)(puVar6 + -8) = 0x18000210e;
      _Dst = (void *)FUN_180002990(uVar1);
      goto LAB_180002111;
    }
    uVar4 = uVar3 + 0x28;
    if (uVar4 <= uVar1) {
                    // WARNING: Subroutine does not return
      *(undefined **)(puVar6 + -8) = &UNK_180002148;
      FUN_180001170();
    }
  } while( true );
}



void FUN_180002150(undefined8 *param_1,ulonglong param_2,undefined8 param_3,undefined8 param_4,
                  longlong param_5)

{
  undefined8 *puVar1;
  void *pvVar2;
  undefined2 *puVar3;
  longlong lVar4;
  ulonglong uVar5;
  code *pcVar6;
  longlong lVar7;
  void *pvVar8;
  size_t sVar9;
  void *_Src;
  undefined1 *puVar10;
  void *unaff_RDI;
  undefined1 auStack_38 [8];
  undefined1 auStack_30 [24];
  
  puVar10 = auStack_38;
  lVar4 = param_1[2];
  _Src = (void *)0x7ffffffffffffffe;
  if (0x7ffffffffffffffeU - lVar4 < param_2) {
                    // WARNING: Subroutine does not return
    FUN_180001210();
  }
  uVar5 = param_1[3];
  pvVar8 = (void *)(lVar4 + param_2 | 7);
  if ((pvVar8 < (void *)0x7fffffffffffffff) && (uVar5 <= 0x7ffffffffffffffe - (uVar5 >> 1))) {
    pvVar2 = (void *)(uVar5 + (uVar5 >> 1));
    _Src = pvVar8;
    if (pvVar8 < pvVar2) {
      _Src = pvVar2;
    }
    if (0x7fffffffffffffff < (longlong)_Src + 1U) goto LAB_1800022f7;
    sVar9 = ((longlong)_Src + 1U) * 2;
    if (sVar9 != 0) goto LAB_1800021e7;
    unaff_RDI = (void *)0x0;
LAB_180002224:
    sVar9 = lVar4 * 2;
    param_1[2] = lVar4 + param_2;
    param_1[3] = _Src;
    puVar3 = (undefined2 *)((longlong)unaff_RDI + (param_5 + lVar4) * 2);
    if (uVar5 < 8) {
      memcpy(unaff_RDI,param_1,sVar9);
      memcpy((void *)(sVar9 + (longlong)unaff_RDI),L"\\dinput8.dll",param_5 * 2);
      *puVar3 = 0;
      goto LAB_1800022ce;
    }
    _Src = (void *)*param_1;
    memcpy(unaff_RDI,_Src,sVar9);
    memcpy((void *)(sVar9 + (longlong)unaff_RDI),L"\\dinput8.dll",param_5 * 2);
    *puVar3 = 0;
    if (0xfff < uVar5 * 2 + 2) {
      puVar1 = (undefined8 *)((longlong)_Src + -8);
      _Src = (void *)((longlong)_Src + (-8 - (longlong)*puVar1));
      if (_Src < (void *)0x20) {
        free((void *)*puVar1);
        goto LAB_1800022ce;
      }
      goto LAB_18000229d;
    }
  }
  else {
    sVar9 = 0xfffffffffffffffe;
LAB_1800021e7:
    if (sVar9 < 0x1000) {
      unaff_RDI = (void *)FUN_180002990(sVar9);
      goto LAB_180002224;
    }
    if (sVar9 + 0x27 <= sVar9) {
LAB_1800022f7:
                    // WARNING: Subroutine does not return
      FUN_180001170();
    }
    lVar7 = FUN_180002990(sVar9 + 0x27);
    if (lVar7 != 0) {
      unaff_RDI = (void *)(lVar7 + 0x27U & 0xffffffffffffffe0);
      *(longlong *)((longlong)unaff_RDI - 8) = lVar7;
      goto LAB_180002224;
    }
LAB_18000229d:
    pcVar6 = (code *)swi(0x29);
    (*pcVar6)(5);
    puVar10 = auStack_30;
  }
  *(undefined8 *)(puVar10 + -8) = 0x1800022ac;
  free(_Src);
LAB_1800022ce:
  *param_1 = unaff_RDI;
  return;
}



// WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie
// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined1 *
FUN_180002300(undefined1 (*param_1) [16],undefined1 *param_2,undefined1 (*param_3) [16],
             ulonglong param_4)

{
  byte *pbVar1;
  undefined1 auVar2 [16];
  undefined1 auVar3 [16];
  undefined1 auVar4 [32];
  undefined1 *puVar5;
  char cVar6;
  int iVar7;
  undefined7 extraout_var;
  uint uVar8;
  uint uVar9;
  undefined1 auVar10 [32];
  undefined1 auVar11 [64];
  undefined1 auStack_298 [56];
  undefined1 local_260 [32];
  char local_240 [256];
  char local_140 [256];
  ulonglong local_40;
  ulonglong local_38;
  undefined1 extraout_var_00 [56];
  
  local_38 = DAT_180008040 ^ (ulonglong)auStack_298;
  if ((DAT_180008004 & 4) == 0) {
    memset(local_140,0,0x100);
    puVar5 = *param_3;
    for (; param_3 != (undefined1 (*) [16])(puVar5 + param_4);
        param_3 = (undefined1 (*) [16])(*param_3 + 1)) {
      local_140[(byte)(*param_3)[0]] = '\x01';
    }
    do {
      if (param_2 == (undefined1 *)0x0) {
        return (undefined1 *)0xffffffffffffffff;
      }
      pbVar1 = param_1[-1] + 0xf + (longlong)param_2;
      param_2 = param_2 + -1;
    } while (local_140[*pbVar1] == '\0');
  }
  else {
    cVar6 = FUN_1800028c0((ulonglong)param_2,param_4,(byte)(DAT_180008004 >> 5) & 1);
    iVar7 = (int)CONCAT71(extraout_var,cVar6);
    if (iVar7 == 2) {
      auVar11._0_8_ = FUN_180002760((byte *)param_3,param_4);
      auVar11._8_56_ = extraout_var_00;
      local_260 = auVar11._0_32_;
      do {
        if (param_2 < (undefined1 *)0x8) {
          uVar9 = (uint)param_2 & 7;
          if (((ulonglong)param_2 & 7) != 0) {
            memcpy(&local_40,param_1,(ulonglong)uVar9);
            auVar3._8_8_ = 0;
            auVar3._0_8_ = local_40;
            auVar10 = vpmovzxbd_avx2(auVar3);
            auVar4 = vpandn_avx2(auVar10,_DAT_180006760);
            auVar10 = vpsrld_avx2(auVar10,5);
            auVar10 = vpermd_avx2(auVar10,local_260);
            auVar4 = vpsllvd_avx2(auVar10,auVar4);
            uVar8 = vmovmskps_avx(auVar4);
            uVar8 = uVar8 & (1 << (sbyte)uVar9) - 1U;
            if (uVar8 != 0) {
              return (undefined1 *)(ulonglong)(0x1f - LZCOUNT(uVar8));
            }
          }
          return (undefined1 *)0xffffffffffffffff;
        }
        auVar2._8_8_ = 0;
        auVar2._0_8_ = *(ulonglong *)(param_1[-1] + 8 + (longlong)param_2);
        auVar4 = vpmovzxbd_avx2(auVar2);
        param_2 = param_2 + -8;
        auVar10 = vpsrld_avx2(auVar4,5);
        auVar10 = vpermd_avx2(auVar10,local_260);
        auVar4 = vpandn_avx2(auVar4,_DAT_180006760);
        auVar4 = vpsllvd_avx2(auVar10,auVar4);
        iVar7 = vmovmskps_avx(auVar4);
      } while (iVar7 == 0);
      param_2 = param_2 + (0x1f - (ulonglong)(uint)LZCOUNT(iVar7));
    }
    else if (iVar7 == 1) {
      memset(local_240,0,0x100);
      puVar5 = *param_3;
      for (; param_3 != (undefined1 (*) [16])(puVar5 + param_4);
          param_3 = (undefined1 (*) [16])(*param_3 + 1)) {
        local_240[(byte)(*param_3)[0]] = '\x01';
      }
      do {
        if (param_2 == (undefined1 *)0x0) {
          return (undefined1 *)0xffffffffffffffff;
        }
        pbVar1 = param_1[-1] + 0xf + (longlong)param_2;
        param_2 = param_2 + -1;
      } while (local_240[*pbVar1] == '\0');
    }
    else {
      param_2 = FUN_180002540(param_1,(ulonglong)param_2,param_3,param_4);
    }
  }
                    // WARNING: Read-only address (ram,0x000180006760) is written
  return param_2;
}



// WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie
// WARNING: Removing unreachable block (ram,0x0001800026a7)
// WARNING: Removing unreachable block (ram,0x0001800026aa)
// WARNING: Removing unreachable block (ram,0x000180002723)
// WARNING: Removing unreachable block (ram,0x000180002725)

undefined1 *
FUN_180002540(undefined1 (*param_1) [16],ulonglong param_2,undefined1 (*param_3) [16],
             ulonglong param_4)

{
  undefined1 (*pauVar1) [16];
  undefined1 auVar2 [16];
  int iVar3;
  int iVar4;
  int iVar5;
  undefined1 *puVar6;
  uint uVar7;
  undefined1 (*pauVar8) [16];
  undefined1 (*pauVar9) [16];
  uint uVar10;
  bool bVar12;
  undefined1 uVar13;
  undefined1 auVar14 [16];
  undefined1 auStack_a8 [32];
  ulonglong local_88;
  undefined8 uStack_80;
  undefined1 local_78 [16];
  ulonglong local_68;
  ulonglong uVar11;
  
  local_68 = DAT_180008040 ^ (ulonglong)auStack_a8;
  uVar10 = (uint)param_2 & 0xf;
  uVar11 = (ulonglong)uVar10;
  puVar6 = *param_1 + param_2;
  iVar3 = -1;
  uVar7 = (uint)param_4;
  local_88 = param_2;
  if (param_4 < 0x11) {
    memcpy(&local_88,param_3,param_4);
    auVar2._8_8_ = uStack_80;
    auVar2._0_8_ = local_88;
    do {
      if (puVar6 == *param_1 + uVar11) {
        if (uVar11 != 0) {
          uVar13 = param_2 < 0x10;
          if ((bool)uVar13) {
            memcpy(&local_88,param_1,param_2);
            auVar14._8_8_ = uStack_80;
            auVar14._0_8_ = local_88;
          }
          else {
            auVar14 = *param_1;
          }
          iVar3 = pcmpestri(auVar2,auVar14,0x40,uVar7,uVar10);
          if ((bool)uVar13) {
            return (undefined1 *)(longlong)iVar3;
          }
        }
        return (undefined1 *)0xffffffffffffffff;
      }
      pauVar9 = (undefined1 (*) [16])(puVar6 + -0x10);
      bVar12 = (undefined1 *)0xf < puVar6;
      puVar6 = puVar6 + -0x10;
      iVar3 = pcmpestri(auVar2,*pauVar9,0x40,uVar7,0x10);
    } while (bVar12);
    puVar6 = puVar6 + ((longlong)iVar3 - (longlong)param_1);
  }
  else {
    uVar7 = uVar7 & 0xf;
    pauVar9 = (undefined1 (*) [16])(*param_3 + (param_4 & 0xfffffffffffffff0));
    memcpy(local_78,pauVar9,(ulonglong)uVar7);
    auVar2 = local_78;
    do {
      if (puVar6 == *param_1 + uVar11) {
        if (uVar11 == 0) {
          return (undefined1 *)0xffffffffffffffff;
        }
        uVar13 = local_88 < 0x10;
        if ((bool)uVar13) {
          memcpy(local_78,param_1,local_88);
        }
        else {
          local_78 = *param_1;
        }
        do {
          iVar5 = pcmpestri(*param_3,local_78,0x40,0x10,uVar10);
          if (((bool)uVar13) && (iVar3 < iVar5)) {
            iVar3 = iVar5;
          }
          param_3 = param_3 + 1;
          uVar13 = param_3 < pauVar9;
        } while (param_3 != pauVar9);
        if ((param_4 & 0xf) != 0) {
          pcmpestri(auVar2,local_78,0x40,uVar7,uVar10);
        }
        return (undefined1 *)(longlong)iVar3;
      }
      pauVar1 = (undefined1 (*) [16])(puVar6 + -0x10);
      bVar12 = puVar6 < (undefined1 *)0x10;
      puVar6 = puVar6 + -0x10;
      iVar5 = -1;
      pauVar8 = param_3;
      do {
        iVar4 = pcmpestri(*pauVar8,*pauVar1,0x40,0x10,0x10);
        if ((bVar12) && (iVar5 < iVar4)) {
          iVar5 = iVar4;
        }
        pauVar8 = pauVar8 + 1;
        bVar12 = pauVar8 < pauVar9;
      } while (pauVar8 != pauVar9);
      if ((param_4 & 0xf) != 0) {
        pcmpestri(local_78,*pauVar1,0x40,uVar7,0x10);
      }
    } while (iVar5 == -1);
    puVar6 = puVar6 + ((longlong)iVar5 - (longlong)param_1);
  }
  return puVar6;
}



// WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie

undefined8 FUN_180002760(byte *param_1,ulonglong param_2)

{
  byte *pbVar1;
  byte bVar2;
  undefined1 auVar3 [16];
  undefined1 auVar4 [16];
  undefined1 auVar5 [32];
  undefined8 uVar6;
  undefined1 auVar7 [64];
  undefined1 auStack_158 [56];
  undefined1 local_120 [32];
  undefined1 local_100 [32];
  undefined1 local_e0 [32];
  undefined1 local_c0 [32];
  undefined1 local_a0 [32];
  undefined1 local_80 [32];
  undefined1 local_60 [32];
  undefined1 local_40 [32];
  ulonglong local_20;
  
  local_20 = DAT_180008040 ^ (ulonglong)auStack_158;
  pbVar1 = param_1 + param_2;
  if (param_2 < 0x15) {
    auVar7 = ZEXT1664((undefined1  [16])0x0);
    uVar6 = 0;
    while (param_1 != pbVar1) {
      bVar2 = *param_1;
      param_1 = param_1 + 1;
      auVar5 = vpmovzxbq_avx2(ZEXT416((uint)(1 << (bVar2 >> 3 & 0x18))));
      auVar5 = vpsllq_avx2(auVar5,ZEXT416(bVar2 & 0x3f));
      auVar5 = vpor_avx2(auVar5,auVar7._0_32_);
      auVar7 = ZEXT3264(auVar5);
      uVar6 = auVar5._0_8_;
    }
  }
  else {
    memset(local_120,0,0x100);
    for (; param_1 != pbVar1; param_1 = param_1 + 1) {
      local_120[*param_1] = 0xff;
    }
    auVar3 = vpinsrd_avx(ZEXT416((uint)(SUB321(local_a0 >> 7,0) & 1) |
                                 (uint)(SUB321(local_a0 >> 0xf,0) & 1) << 1 |
                                 (uint)(SUB321(local_a0 >> 0x17,0) & 1) << 2 |
                                 (uint)(SUB321(local_a0 >> 0x1f,0) & 1) << 3 |
                                 (uint)(SUB321(local_a0 >> 0x27,0) & 1) << 4 |
                                 (uint)(SUB321(local_a0 >> 0x2f,0) & 1) << 5 |
                                 (uint)(SUB321(local_a0 >> 0x37,0) & 1) << 6 |
                                 (uint)(SUB321(local_a0 >> 0x3f,0) & 1) << 7 |
                                 (uint)(SUB321(local_a0 >> 0x47,0) & 1) << 8 |
                                 (uint)(SUB321(local_a0 >> 0x4f,0) & 1) << 9 |
                                 (uint)(SUB321(local_a0 >> 0x57,0) & 1) << 10 |
                                 (uint)(SUB321(local_a0 >> 0x5f,0) & 1) << 0xb |
                                 (uint)(SUB321(local_a0 >> 0x67,0) & 1) << 0xc |
                                 (uint)(SUB321(local_a0 >> 0x6f,0) & 1) << 0xd |
                                 (uint)(SUB321(local_a0 >> 0x77,0) & 1) << 0xe |
                                 (uint)SUB321(local_a0 >> 0x7f,0) << 0xf |
                                 (uint)(SUB321(local_a0 >> 0x87,0) & 1) << 0x10 |
                                 (uint)(SUB321(local_a0 >> 0x8f,0) & 1) << 0x11 |
                                 (uint)(SUB321(local_a0 >> 0x97,0) & 1) << 0x12 |
                                 (uint)(SUB321(local_a0 >> 0x9f,0) & 1) << 0x13 |
                                 (uint)(SUB321(local_a0 >> 0xa7,0) & 1) << 0x14 |
                                 (uint)(SUB321(local_a0 >> 0xaf,0) & 1) << 0x15 |
                                 (uint)(SUB321(local_a0 >> 0xb7,0) & 1) << 0x16 |
                                 (uint)SUB321(local_a0 >> 0xbf,0) << 0x17 |
                                 (uint)(SUB321(local_a0 >> 199,0) & 1) << 0x18 |
                                 (uint)(SUB321(local_a0 >> 0xcf,0) & 1) << 0x19 |
                                 (uint)(SUB321(local_a0 >> 0xd7,0) & 1) << 0x1a |
                                 (uint)(SUB321(local_a0 >> 0xdf,0) & 1) << 0x1b |
                                 (uint)(SUB321(local_a0 >> 0xe7,0) & 1) << 0x1c |
                                 (uint)(SUB321(local_a0 >> 0xef,0) & 1) << 0x1d |
                                 (uint)(SUB321(local_a0 >> 0xf7,0) & 1) << 0x1e |
                                 (uint)(byte)(local_a0[0x1f] >> 7) << 0x1f),
                         (uint)(SUB321(local_80 >> 7,0) & 1) |
                         (uint)(SUB321(local_80 >> 0xf,0) & 1) << 1 |
                         (uint)(SUB321(local_80 >> 0x17,0) & 1) << 2 |
                         (uint)(SUB321(local_80 >> 0x1f,0) & 1) << 3 |
                         (uint)(SUB321(local_80 >> 0x27,0) & 1) << 4 |
                         (uint)(SUB321(local_80 >> 0x2f,0) & 1) << 5 |
                         (uint)(SUB321(local_80 >> 0x37,0) & 1) << 6 |
                         (uint)(SUB321(local_80 >> 0x3f,0) & 1) << 7 |
                         (uint)(SUB321(local_80 >> 0x47,0) & 1) << 8 |
                         (uint)(SUB321(local_80 >> 0x4f,0) & 1) << 9 |
                         (uint)(SUB321(local_80 >> 0x57,0) & 1) << 10 |
                         (uint)(SUB321(local_80 >> 0x5f,0) & 1) << 0xb |
                         (uint)(SUB321(local_80 >> 0x67,0) & 1) << 0xc |
                         (uint)(SUB321(local_80 >> 0x6f,0) & 1) << 0xd |
                         (uint)(SUB321(local_80 >> 0x77,0) & 1) << 0xe |
                         (uint)SUB321(local_80 >> 0x7f,0) << 0xf |
                         (uint)(SUB321(local_80 >> 0x87,0) & 1) << 0x10 |
                         (uint)(SUB321(local_80 >> 0x8f,0) & 1) << 0x11 |
                         (uint)(SUB321(local_80 >> 0x97,0) & 1) << 0x12 |
                         (uint)(SUB321(local_80 >> 0x9f,0) & 1) << 0x13 |
                         (uint)(SUB321(local_80 >> 0xa7,0) & 1) << 0x14 |
                         (uint)(SUB321(local_80 >> 0xaf,0) & 1) << 0x15 |
                         (uint)(SUB321(local_80 >> 0xb7,0) & 1) << 0x16 |
                         (uint)SUB321(local_80 >> 0xbf,0) << 0x17 |
                         (uint)(SUB321(local_80 >> 199,0) & 1) << 0x18 |
                         (uint)(SUB321(local_80 >> 0xcf,0) & 1) << 0x19 |
                         (uint)(SUB321(local_80 >> 0xd7,0) & 1) << 0x1a |
                         (uint)(SUB321(local_80 >> 0xdf,0) & 1) << 0x1b |
                         (uint)(SUB321(local_80 >> 0xe7,0) & 1) << 0x1c |
                         (uint)(SUB321(local_80 >> 0xef,0) & 1) << 0x1d |
                         (uint)(SUB321(local_80 >> 0xf7,0) & 1) << 0x1e |
                         (uint)(byte)(local_80[0x1f] >> 7) << 0x1f,1);
    auVar3 = vpinsrd_avx(auVar3,(uint)(SUB321(local_60 >> 7,0) & 1) |
                                (uint)(SUB321(local_60 >> 0xf,0) & 1) << 1 |
                                (uint)(SUB321(local_60 >> 0x17,0) & 1) << 2 |
                                (uint)(SUB321(local_60 >> 0x1f,0) & 1) << 3 |
                                (uint)(SUB321(local_60 >> 0x27,0) & 1) << 4 |
                                (uint)(SUB321(local_60 >> 0x2f,0) & 1) << 5 |
                                (uint)(SUB321(local_60 >> 0x37,0) & 1) << 6 |
                                (uint)(SUB321(local_60 >> 0x3f,0) & 1) << 7 |
                                (uint)(SUB321(local_60 >> 0x47,0) & 1) << 8 |
                                (uint)(SUB321(local_60 >> 0x4f,0) & 1) << 9 |
                                (uint)(SUB321(local_60 >> 0x57,0) & 1) << 10 |
                                (uint)(SUB321(local_60 >> 0x5f,0) & 1) << 0xb |
                                (uint)(SUB321(local_60 >> 0x67,0) & 1) << 0xc |
                                (uint)(SUB321(local_60 >> 0x6f,0) & 1) << 0xd |
                                (uint)(SUB321(local_60 >> 0x77,0) & 1) << 0xe |
                                (uint)SUB321(local_60 >> 0x7f,0) << 0xf |
                                (uint)(SUB321(local_60 >> 0x87,0) & 1) << 0x10 |
                                (uint)(SUB321(local_60 >> 0x8f,0) & 1) << 0x11 |
                                (uint)(SUB321(local_60 >> 0x97,0) & 1) << 0x12 |
                                (uint)(SUB321(local_60 >> 0x9f,0) & 1) << 0x13 |
                                (uint)(SUB321(local_60 >> 0xa7,0) & 1) << 0x14 |
                                (uint)(SUB321(local_60 >> 0xaf,0) & 1) << 0x15 |
                                (uint)(SUB321(local_60 >> 0xb7,0) & 1) << 0x16 |
                                (uint)SUB321(local_60 >> 0xbf,0) << 0x17 |
                                (uint)(SUB321(local_60 >> 199,0) & 1) << 0x18 |
                                (uint)(SUB321(local_60 >> 0xcf,0) & 1) << 0x19 |
                                (uint)(SUB321(local_60 >> 0xd7,0) & 1) << 0x1a |
                                (uint)(SUB321(local_60 >> 0xdf,0) & 1) << 0x1b |
                                (uint)(SUB321(local_60 >> 0xe7,0) & 1) << 0x1c |
                                (uint)(SUB321(local_60 >> 0xef,0) & 1) << 0x1d |
                                (uint)(SUB321(local_60 >> 0xf7,0) & 1) << 0x1e |
                                (uint)(byte)(local_60[0x1f] >> 7) << 0x1f,2);
    auVar4 = vpinsrd_avx(ZEXT416((uint)(SUB321(local_120 >> 7,0) & 1) |
                                 (uint)(SUB321(local_120 >> 0xf,0) & 1) << 1 |
                                 (uint)(SUB321(local_120 >> 0x17,0) & 1) << 2 |
                                 (uint)(SUB321(local_120 >> 0x1f,0) & 1) << 3 |
                                 (uint)(SUB321(local_120 >> 0x27,0) & 1) << 4 |
                                 (uint)(SUB321(local_120 >> 0x2f,0) & 1) << 5 |
                                 (uint)(SUB321(local_120 >> 0x37,0) & 1) << 6 |
                                 (uint)(SUB321(local_120 >> 0x3f,0) & 1) << 7 |
                                 (uint)(SUB321(local_120 >> 0x47,0) & 1) << 8 |
                                 (uint)(SUB321(local_120 >> 0x4f,0) & 1) << 9 |
                                 (uint)(SUB321(local_120 >> 0x57,0) & 1) << 10 |
                                 (uint)(SUB321(local_120 >> 0x5f,0) & 1) << 0xb |
                                 (uint)(SUB321(local_120 >> 0x67,0) & 1) << 0xc |
                                 (uint)(SUB321(local_120 >> 0x6f,0) & 1) << 0xd |
                                 (uint)(SUB321(local_120 >> 0x77,0) & 1) << 0xe |
                                 (uint)SUB321(local_120 >> 0x7f,0) << 0xf |
                                 (uint)(SUB321(local_120 >> 0x87,0) & 1) << 0x10 |
                                 (uint)(SUB321(local_120 >> 0x8f,0) & 1) << 0x11 |
                                 (uint)(SUB321(local_120 >> 0x97,0) & 1) << 0x12 |
                                 (uint)(SUB321(local_120 >> 0x9f,0) & 1) << 0x13 |
                                 (uint)(SUB321(local_120 >> 0xa7,0) & 1) << 0x14 |
                                 (uint)(SUB321(local_120 >> 0xaf,0) & 1) << 0x15 |
                                 (uint)(SUB321(local_120 >> 0xb7,0) & 1) << 0x16 |
                                 (uint)SUB321(local_120 >> 0xbf,0) << 0x17 |
                                 (uint)(SUB321(local_120 >> 199,0) & 1) << 0x18 |
                                 (uint)(SUB321(local_120 >> 0xcf,0) & 1) << 0x19 |
                                 (uint)(SUB321(local_120 >> 0xd7,0) & 1) << 0x1a |
                                 (uint)(SUB321(local_120 >> 0xdf,0) & 1) << 0x1b |
                                 (uint)(SUB321(local_120 >> 0xe7,0) & 1) << 0x1c |
                                 (uint)(SUB321(local_120 >> 0xef,0) & 1) << 0x1d |
                                 (uint)(SUB321(local_120 >> 0xf7,0) & 1) << 0x1e |
                                 (uint)(byte)(local_120[0x1f] >> 7) << 0x1f),
                         (uint)(SUB321(local_100 >> 7,0) & 1) |
                         (uint)(SUB321(local_100 >> 0xf,0) & 1) << 1 |
                         (uint)(SUB321(local_100 >> 0x17,0) & 1) << 2 |
                         (uint)(SUB321(local_100 >> 0x1f,0) & 1) << 3 |
                         (uint)(SUB321(local_100 >> 0x27,0) & 1) << 4 |
                         (uint)(SUB321(local_100 >> 0x2f,0) & 1) << 5 |
                         (uint)(SUB321(local_100 >> 0x37,0) & 1) << 6 |
                         (uint)(SUB321(local_100 >> 0x3f,0) & 1) << 7 |
                         (uint)(SUB321(local_100 >> 0x47,0) & 1) << 8 |
                         (uint)(SUB321(local_100 >> 0x4f,0) & 1) << 9 |
                         (uint)(SUB321(local_100 >> 0x57,0) & 1) << 10 |
                         (uint)(SUB321(local_100 >> 0x5f,0) & 1) << 0xb |
                         (uint)(SUB321(local_100 >> 0x67,0) & 1) << 0xc |
                         (uint)(SUB321(local_100 >> 0x6f,0) & 1) << 0xd |
                         (uint)(SUB321(local_100 >> 0x77,0) & 1) << 0xe |
                         (uint)SUB321(local_100 >> 0x7f,0) << 0xf |
                         (uint)(SUB321(local_100 >> 0x87,0) & 1) << 0x10 |
                         (uint)(SUB321(local_100 >> 0x8f,0) & 1) << 0x11 |
                         (uint)(SUB321(local_100 >> 0x97,0) & 1) << 0x12 |
                         (uint)(SUB321(local_100 >> 0x9f,0) & 1) << 0x13 |
                         (uint)(SUB321(local_100 >> 0xa7,0) & 1) << 0x14 |
                         (uint)(SUB321(local_100 >> 0xaf,0) & 1) << 0x15 |
                         (uint)(SUB321(local_100 >> 0xb7,0) & 1) << 0x16 |
                         (uint)SUB321(local_100 >> 0xbf,0) << 0x17 |
                         (uint)(SUB321(local_100 >> 199,0) & 1) << 0x18 |
                         (uint)(SUB321(local_100 >> 0xcf,0) & 1) << 0x19 |
                         (uint)(SUB321(local_100 >> 0xd7,0) & 1) << 0x1a |
                         (uint)(SUB321(local_100 >> 0xdf,0) & 1) << 0x1b |
                         (uint)(SUB321(local_100 >> 0xe7,0) & 1) << 0x1c |
                         (uint)(SUB321(local_100 >> 0xef,0) & 1) << 0x1d |
                         (uint)(SUB321(local_100 >> 0xf7,0) & 1) << 0x1e |
                         (uint)(byte)(local_100[0x1f] >> 7) << 0x1f,1);
    auVar4 = vpinsrd_avx(auVar4,(uint)(SUB321(local_e0 >> 7,0) & 1) |
                                (uint)(SUB321(local_e0 >> 0xf,0) & 1) << 1 |
                                (uint)(SUB321(local_e0 >> 0x17,0) & 1) << 2 |
                                (uint)(SUB321(local_e0 >> 0x1f,0) & 1) << 3 |
                                (uint)(SUB321(local_e0 >> 0x27,0) & 1) << 4 |
                                (uint)(SUB321(local_e0 >> 0x2f,0) & 1) << 5 |
                                (uint)(SUB321(local_e0 >> 0x37,0) & 1) << 6 |
                                (uint)(SUB321(local_e0 >> 0x3f,0) & 1) << 7 |
                                (uint)(SUB321(local_e0 >> 0x47,0) & 1) << 8 |
                                (uint)(SUB321(local_e0 >> 0x4f,0) & 1) << 9 |
                                (uint)(SUB321(local_e0 >> 0x57,0) & 1) << 10 |
                                (uint)(SUB321(local_e0 >> 0x5f,0) & 1) << 0xb |
                                (uint)(SUB321(local_e0 >> 0x67,0) & 1) << 0xc |
                                (uint)(SUB321(local_e0 >> 0x6f,0) & 1) << 0xd |
                                (uint)(SUB321(local_e0 >> 0x77,0) & 1) << 0xe |
                                (uint)SUB321(local_e0 >> 0x7f,0) << 0xf |
                                (uint)(SUB321(local_e0 >> 0x87,0) & 1) << 0x10 |
                                (uint)(SUB321(local_e0 >> 0x8f,0) & 1) << 0x11 |
                                (uint)(SUB321(local_e0 >> 0x97,0) & 1) << 0x12 |
                                (uint)(SUB321(local_e0 >> 0x9f,0) & 1) << 0x13 |
                                (uint)(SUB321(local_e0 >> 0xa7,0) & 1) << 0x14 |
                                (uint)(SUB321(local_e0 >> 0xaf,0) & 1) << 0x15 |
                                (uint)(SUB321(local_e0 >> 0xb7,0) & 1) << 0x16 |
                                (uint)SUB321(local_e0 >> 0xbf,0) << 0x17 |
                                (uint)(SUB321(local_e0 >> 199,0) & 1) << 0x18 |
                                (uint)(SUB321(local_e0 >> 0xcf,0) & 1) << 0x19 |
                                (uint)(SUB321(local_e0 >> 0xd7,0) & 1) << 0x1a |
                                (uint)(SUB321(local_e0 >> 0xdf,0) & 1) << 0x1b |
                                (uint)(SUB321(local_e0 >> 0xe7,0) & 1) << 0x1c |
                                (uint)(SUB321(local_e0 >> 0xef,0) & 1) << 0x1d |
                                (uint)(SUB321(local_e0 >> 0xf7,0) & 1) << 0x1e |
                                (uint)(byte)(local_e0[0x1f] >> 7) << 0x1f,2);
    auVar4 = vpinsrd_avx(auVar4,(uint)(SUB321(local_c0 >> 7,0) & 1) |
                                (uint)(SUB321(local_c0 >> 0xf,0) & 1) << 1 |
                                (uint)(SUB321(local_c0 >> 0x17,0) & 1) << 2 |
                                (uint)(SUB321(local_c0 >> 0x1f,0) & 1) << 3 |
                                (uint)(SUB321(local_c0 >> 0x27,0) & 1) << 4 |
                                (uint)(SUB321(local_c0 >> 0x2f,0) & 1) << 5 |
                                (uint)(SUB321(local_c0 >> 0x37,0) & 1) << 6 |
                                (uint)(SUB321(local_c0 >> 0x3f,0) & 1) << 7 |
                                (uint)(SUB321(local_c0 >> 0x47,0) & 1) << 8 |
                                (uint)(SUB321(local_c0 >> 0x4f,0) & 1) << 9 |
                                (uint)(SUB321(local_c0 >> 0x57,0) & 1) << 10 |
                                (uint)(SUB321(local_c0 >> 0x5f,0) & 1) << 0xb |
                                (uint)(SUB321(local_c0 >> 0x67,0) & 1) << 0xc |
                                (uint)(SUB321(local_c0 >> 0x6f,0) & 1) << 0xd |
                                (uint)(SUB321(local_c0 >> 0x77,0) & 1) << 0xe |
                                (uint)SUB321(local_c0 >> 0x7f,0) << 0xf |
                                (uint)(SUB321(local_c0 >> 0x87,0) & 1) << 0x10 |
                                (uint)(SUB321(local_c0 >> 0x8f,0) & 1) << 0x11 |
                                (uint)(SUB321(local_c0 >> 0x97,0) & 1) << 0x12 |
                                (uint)(SUB321(local_c0 >> 0x9f,0) & 1) << 0x13 |
                                (uint)(SUB321(local_c0 >> 0xa7,0) & 1) << 0x14 |
                                (uint)(SUB321(local_c0 >> 0xaf,0) & 1) << 0x15 |
                                (uint)(SUB321(local_c0 >> 0xb7,0) & 1) << 0x16 |
                                (uint)SUB321(local_c0 >> 0xbf,0) << 0x17 |
                                (uint)(SUB321(local_c0 >> 199,0) & 1) << 0x18 |
                                (uint)(SUB321(local_c0 >> 0xcf,0) & 1) << 0x19 |
                                (uint)(SUB321(local_c0 >> 0xd7,0) & 1) << 0x1a |
                                (uint)(SUB321(local_c0 >> 0xdf,0) & 1) << 0x1b |
                                (uint)(SUB321(local_c0 >> 0xe7,0) & 1) << 0x1c |
                                (uint)(SUB321(local_c0 >> 0xef,0) & 1) << 0x1d |
                                (uint)(SUB321(local_c0 >> 0xf7,0) & 1) << 0x1e |
                                (uint)(byte)(local_c0[0x1f] >> 7) << 0x1f,3);
    vpinsrd_avx(auVar3,(uint)(SUB321(local_40 >> 7,0) & 1) |
                       (uint)(SUB321(local_40 >> 0xf,0) & 1) << 1 |
                       (uint)(SUB321(local_40 >> 0x17,0) & 1) << 2 |
                       (uint)(SUB321(local_40 >> 0x1f,0) & 1) << 3 |
                       (uint)(SUB321(local_40 >> 0x27,0) & 1) << 4 |
                       (uint)(SUB321(local_40 >> 0x2f,0) & 1) << 5 |
                       (uint)(SUB321(local_40 >> 0x37,0) & 1) << 6 |
                       (uint)(SUB321(local_40 >> 0x3f,0) & 1) << 7 |
                       (uint)(SUB321(local_40 >> 0x47,0) & 1) << 8 |
                       (uint)(SUB321(local_40 >> 0x4f,0) & 1) << 9 |
                       (uint)(SUB321(local_40 >> 0x57,0) & 1) << 10 |
                       (uint)(SUB321(local_40 >> 0x5f,0) & 1) << 0xb |
                       (uint)(SUB321(local_40 >> 0x67,0) & 1) << 0xc |
                       (uint)(SUB321(local_40 >> 0x6f,0) & 1) << 0xd |
                       (uint)(SUB321(local_40 >> 0x77,0) & 1) << 0xe |
                       (uint)SUB321(local_40 >> 0x7f,0) << 0xf |
                       (uint)(SUB321(local_40 >> 0x87,0) & 1) << 0x10 |
                       (uint)(SUB321(local_40 >> 0x8f,0) & 1) << 0x11 |
                       (uint)(SUB321(local_40 >> 0x97,0) & 1) << 0x12 |
                       (uint)(SUB321(local_40 >> 0x9f,0) & 1) << 0x13 |
                       (uint)(SUB321(local_40 >> 0xa7,0) & 1) << 0x14 |
                       (uint)(SUB321(local_40 >> 0xaf,0) & 1) << 0x15 |
                       (uint)(SUB321(local_40 >> 0xb7,0) & 1) << 0x16 |
                       (uint)SUB321(local_40 >> 0xbf,0) << 0x17 |
                       (uint)(SUB321(local_40 >> 199,0) & 1) << 0x18 |
                       (uint)(SUB321(local_40 >> 0xcf,0) & 1) << 0x19 |
                       (uint)(SUB321(local_40 >> 0xd7,0) & 1) << 0x1a |
                       (uint)(SUB321(local_40 >> 0xdf,0) & 1) << 0x1b |
                       (uint)(SUB321(local_40 >> 0xe7,0) & 1) << 0x1c |
                       (uint)(SUB321(local_40 >> 0xef,0) & 1) << 0x1d |
                       (uint)(SUB321(local_40 >> 0xf7,0) & 1) << 0x1e |
                       (uint)(byte)(local_40[0x1f] >> 7) << 0x1f,3);
    uVar6 = auVar4._0_8_;
  }
  return uVar6;
}



char FUN_1800028c0(ulonglong param_1,ulonglong param_2,char param_3)

{
  bool bVar1;
  
  if ((param_3 != '\0') && (0x30 < param_1)) {
    if (param_2 < 0x11) {
      return (1000 < param_1) * '\x02';
    }
    if (param_2 < 0x31) {
      return (0x50 < param_1) * '\x02';
    }
    return '\x02';
  }
  bVar1 = false;
  if (0x20 < param_2) {
    if (param_2 < 0x31) {
      return 0x1a0 < param_1;
    }
    if (param_2 < 0x41) {
      return 0xe0 < param_1;
    }
    if (param_2 < 0x51) {
      return 0x80 < param_1;
    }
    if (param_2 < 0x21d) {
      return 0x30 < param_1;
    }
    bVar1 = 0x20 < param_1;
  }
  return bVar1;
}



// WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie
// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined1 *
thunk_FUN_180002300(undefined1 (*param_1) [16],undefined1 *param_2,undefined1 (*param_3) [16],
                   ulonglong param_4)

{
  byte *pbVar1;
  undefined1 auVar2 [16];
  undefined1 auVar3 [16];
  undefined1 auVar4 [32];
  undefined1 *puVar5;
  char cVar6;
  int iVar7;
  undefined7 extraout_var;
  uint uVar8;
  uint uVar9;
  undefined1 auVar10 [32];
  undefined1 auVar11 [64];
  undefined1 auStack_298 [56];
  undefined1 auStack_260 [32];
  char acStack_240 [256];
  char acStack_140 [256];
  ulonglong uStack_40;
  ulonglong uStack_38;
  undefined1 extraout_var_00 [56];
  
  uStack_38 = DAT_180008040 ^ (ulonglong)auStack_298;
  if ((DAT_180008004 & 4) == 0) {
    memset(acStack_140,0,0x100);
    puVar5 = *param_3;
    for (; param_3 != (undefined1 (*) [16])(puVar5 + param_4);
        param_3 = (undefined1 (*) [16])(*param_3 + 1)) {
      acStack_140[(byte)(*param_3)[0]] = '\x01';
    }
    do {
      if (param_2 == (undefined1 *)0x0) {
        return (undefined1 *)0xffffffffffffffff;
      }
      pbVar1 = param_1[-1] + 0xf + (longlong)param_2;
      param_2 = param_2 + -1;
    } while (acStack_140[*pbVar1] == '\0');
  }
  else {
    cVar6 = FUN_1800028c0((ulonglong)param_2,param_4,(byte)(DAT_180008004 >> 5) & 1);
    iVar7 = (int)CONCAT71(extraout_var,cVar6);
    if (iVar7 == 2) {
      auVar11._0_8_ = FUN_180002760((byte *)param_3,param_4);
      auVar11._8_56_ = extraout_var_00;
      auStack_260 = auVar11._0_32_;
      do {
        if (param_2 < (undefined1 *)0x8) {
          uVar9 = (uint)param_2 & 7;
          if (((ulonglong)param_2 & 7) != 0) {
            memcpy(&uStack_40,param_1,(ulonglong)uVar9);
            auVar3._8_8_ = 0;
            auVar3._0_8_ = uStack_40;
            auVar10 = vpmovzxbd_avx2(auVar3);
            auVar4 = vpandn_avx2(auVar10,_DAT_180006760);
            auVar10 = vpsrld_avx2(auVar10,5);
            auVar10 = vpermd_avx2(auVar10,auStack_260);
            auVar4 = vpsllvd_avx2(auVar10,auVar4);
            uVar8 = vmovmskps_avx(auVar4);
            uVar8 = uVar8 & (1 << (sbyte)uVar9) - 1U;
            if (uVar8 != 0) {
              return (undefined1 *)(ulonglong)(0x1f - LZCOUNT(uVar8));
            }
          }
          return (undefined1 *)0xffffffffffffffff;
        }
        auVar2._8_8_ = 0;
        auVar2._0_8_ = *(ulonglong *)(param_1[-1] + 8 + (longlong)param_2);
        auVar4 = vpmovzxbd_avx2(auVar2);
        param_2 = param_2 + -8;
        auVar10 = vpsrld_avx2(auVar4,5);
        auVar10 = vpermd_avx2(auVar10,auStack_260);
        auVar4 = vpandn_avx2(auVar4,_DAT_180006760);
        auVar4 = vpsllvd_avx2(auVar10,auVar4);
        iVar7 = vmovmskps_avx(auVar4);
      } while (iVar7 == 0);
      param_2 = param_2 + (0x1f - (ulonglong)(uint)LZCOUNT(iVar7));
    }
    else if (iVar7 == 1) {
      memset(acStack_240,0,0x100);
      puVar5 = *param_3;
      for (; param_3 != (undefined1 (*) [16])(puVar5 + param_4);
          param_3 = (undefined1 (*) [16])(*param_3 + 1)) {
        acStack_240[(byte)(*param_3)[0]] = '\x01';
      }
      do {
        if (param_2 == (undefined1 *)0x0) {
          return (undefined1 *)0xffffffffffffffff;
        }
        pbVar1 = param_1[-1] + 0xf + (longlong)param_2;
        param_2 = param_2 + -1;
      } while (acStack_240[*pbVar1] == '\0');
    }
    else {
      param_2 = FUN_180002540(param_1,(ulonglong)param_2,param_3,param_4);
    }
  }
                    // WARNING: Read-only address (ram,0x000180006760) is written
  return param_2;
}



// WARNING: This is an inlined function

void __cdecl __security_check_cookie(uintptr_t _StackCookie)

{
  if ((_StackCookie == DAT_180008040) && ((short)(_StackCookie >> 0x30) == 0)) {
    return;
  }
  FUN_180003074();
  return;
}



void FUN_180002990(size_t param_1)

{
  int iVar1;
  void *pvVar2;
  
  do {
    pvVar2 = malloc(param_1);
    if (pvVar2 != (void *)0x0) {
      return;
    }
    iVar1 = _callnewh(param_1);
  } while (iVar1 != 0);
  if (param_1 == 0xffffffffffffffff) {
                    // WARNING: Subroutine does not return
    FUN_180001170();
  }
                    // WARNING: Subroutine does not return
  FUN_1800030a0();
}



void __cdecl free(void *_Memory)

{
                    // WARNING: Could not recover jumptable at 0x0001800035c2. Too many branches
                    // WARNING: Treating indirect jump as call
  free(_Memory);
  return;
}



undefined8 * FUN_1800029d4(undefined8 *param_1,ulonglong param_2)

{
  *param_1 = type_info::vftable;
  if ((param_2 & 1) != 0) {
    free(param_1);
  }
  return param_1;
}



ulonglong FUN_180002a00(undefined8 param_1,int param_2,longlong param_3)

{
  byte bVar1;
  undefined1 uVar2;
  ulonglong uVar3;
  undefined7 extraout_var;
  
  if (param_2 == 0) {
    uVar2 = FUN_180002b68(CONCAT71((int7)((ulonglong)param_1 >> 8),param_3 != 0));
    return CONCAT71(extraout_var,uVar2);
  }
  if (param_2 != 1) {
    if (param_2 == 2) {
      bVar1 = FUN_180003240();
    }
    else {
      if (param_2 != 3) {
        return 1;
      }
      bVar1 = FUN_180003268();
    }
    return (ulonglong)bVar1;
  }
  uVar3 = FUN_180002a50(param_1,param_3);
  return uVar3;
}



// WARNING: Function: _guard_dispatch_icall replaced with injection: guard_dispatch_icall

undefined8 FUN_180002a50(undefined8 param_1,undefined8 param_2)

{
  bool bVar1;
  bool bVar2;
  int iVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  longlong *plVar6;
  ulonglong uVar7;
  
  uVar4 = FUN_180003324(0);
  if ((char)uVar4 != '\0') {
    uVar4 = __scrt_acquire_startup_lock();
    bVar1 = true;
    if (DAT_180008168 != 0) {
                    // WARNING: Subroutine does not return
      FUN_1800034dc(7);
    }
    DAT_180008168 = 1;
    bVar2 = FUN_180003228();
    if (bVar2) {
      FUN_1800034e4();
      FUN_180003178();
      FUN_18000319c();
      iVar3 = _initterm_e(&DAT_180004230,&DAT_180004238);
      if ((iVar3 == 0) && (uVar5 = __scrt_dllmain_after_initialize_c(), (char)uVar5 != '\0')) {
        _initterm(&DAT_180004220,&DAT_180004228);
        DAT_180008168 = 2;
        bVar1 = false;
      }
    }
    __scrt_release_startup_lock((char)uVar4);
    if (!bVar1) {
      plVar6 = (longlong *)FUN_1800034d4();
      if ((*plVar6 != 0) && (uVar7 = FUN_1800033ec((longlong)plVar6), (char)uVar7 != '\0')) {
        (*(code *)*plVar6)(param_1,2,param_2,_guard_dispatch_icall);
      }
      DAT_180008140 = DAT_180008140 + 1;
      return 1;
    }
  }
  return 0;
}



undefined1 FUN_180002b68(undefined8 param_1)

{
  undefined1 uVar1;
  undefined8 uVar2;
  undefined7 uVar3;
  
  uVar1 = (undefined1)param_1;
  if (DAT_180008140 < 1) {
    uVar1 = 0;
  }
  else {
    DAT_180008140 = DAT_180008140 + -1;
    uVar2 = __scrt_acquire_startup_lock();
    if (DAT_180008168 != 2) {
                    // WARNING: Subroutine does not return
      FUN_1800034dc(7);
    }
    __scrt_dllmain_uninitialize_c();
    FUN_180003188();
    FUN_180003520();
    DAT_180008168 = 0;
    uVar3 = (undefined7)((ulonglong)param_1 >> 8);
    __scrt_release_startup_lock((char)uVar2);
    uVar1 = __scrt_uninitialize_crt(CONCAT71(uVar3,uVar1),'\0');
    FUN_180003310();
  }
  return uVar1;
}



// WARNING: Function: _guard_dispatch_icall replaced with injection: guard_dispatch_icall
// WARNING: Removing unreachable block (ram,0x000180002ca9)
// WARNING: Removing unreachable block (ram,0x000180002c3a)
// WARNING: Removing unreachable block (ram,0x000180002ce8)

int FUN_180002bec(HMODULE param_1,int param_2,longlong param_3)

{
  int iVar1;
  ulonglong uVar2;
  undefined8 uVar3;
  HMODULE pHVar4;
  
  if ((param_2 == 0) && (DAT_180008140 < 1)) {
    iVar1 = 0;
  }
  else {
    if ((param_2 - 1U < 2) && (uVar2 = FUN_180002a00(param_1,param_2,param_3), (int)uVar2 == 0)) {
      return 0;
    }
    uVar3 = FUN_180001d40(param_1,param_2);
    iVar1 = (int)uVar3;
    if ((param_2 == 1) && (iVar1 == 0)) {
      pHVar4 = param_1;
      FUN_180001d40(param_1,0);
      FUN_180002b68(CONCAT71((int7)((ulonglong)pHVar4 >> 8),param_3 != 0));
    }
    if ((param_2 == 0) || (param_2 == 3)) {
      uVar2 = FUN_180002a00(param_1,param_2,param_3);
      iVar1 = 0;
      if ((int)uVar2 != 0) {
        iVar1 = 1;
      }
    }
  }
  return iVar1;
}



void entry(HMODULE param_1,int param_2,longlong param_3)

{
  if (param_2 == 1) {
    FUN_1800030c8();
  }
  FUN_180002bec(param_1,param_2,param_3);
  return;
}



undefined8 FUN_180002d54(undefined8 param_1,longlong param_2,undefined8 param_3,longlong param_4)

{
  FUN_180002d74(param_2,param_4);
  return 1;
}



// WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie

longlong FUN_180002d74(longlong param_1,longlong param_2)

{
  ulonglong uVar1;
  
  uVar1 = (ulonglong)*(uint *)(*(longlong *)(param_2 + 0x10) + 8);
  if ((*(byte *)(uVar1 + 3 + *(longlong *)(param_2 + 8)) & 0xf) != 0) {
    param_1 = ((ulonglong)*(byte *)(uVar1 + 3 + *(longlong *)(param_2 + 8)) & 0xfffffff0) + param_1;
  }
  return param_1;
}



// WARNING: Removing unreachable block (ram,0x000180002ec3)
// WARNING: Removing unreachable block (ram,0x000180002eb3)
// WARNING: Removing unreachable block (ram,0x000180002e8e)
// WARNING: Removing unreachable block (ram,0x000180002e12)
// WARNING: Removing unreachable block (ram,0x000180002df0)
// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined8 FUN_180002dd8(void)

{
  int *piVar1;
  uint *puVar2;
  int *piVar3;
  longlong lVar4;
  uint uVar5;
  byte bVar6;
  ulonglong uVar7;
  uint uVar8;
  ulonglong uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  ulonglong in_XCR0;
  
  piVar1 = (int *)cpuid_basic_info(0);
  puVar2 = (uint *)cpuid_Version_info(1);
  uVar5 = puVar2[3];
  if ((piVar1[2] == 0x49656e69 && piVar1[3] == 0x6c65746e) && piVar1[1] == 0x756e6547) {
    uVar8 = *puVar2 & 0xfff3ff0;
    _DAT_180008008 = 0x8000;
    _DAT_180008010 = 0xffffffffffffffff;
    if ((((uVar8 == 0x106c0) || (uVar8 == 0x20660)) || (uVar8 == 0x20670)) ||
       ((uVar8 - 0x30650 < 0x21 &&
        ((0x100010001U >> ((ulonglong)(uVar8 - 0x30650) & 0x3f) & 1) != 0)))) {
      DAT_180008144 = DAT_180008144 | 1;
    }
  }
  uVar11 = 0;
  uVar8 = 0;
  if (*piVar1 < 7) {
    uVar12 = 0;
    uVar10 = 0;
  }
  else {
    piVar3 = (int *)cpuid_Extended_Feature_Enumeration_info(7);
    uVar12 = piVar3[1];
    uVar10 = piVar3[2];
    if ((uVar12 >> 9 & 1) != 0) {
      DAT_180008144 = DAT_180008144 | 2;
    }
    if (0 < *piVar3) {
      lVar4 = cpuid_Extended_Feature_Enumeration_info(7);
      uVar11 = *(uint *)(lVar4 + 8);
    }
    if (0x23 < *piVar1) {
      lVar4 = cpuid(0x24);
      uVar8 = *(uint *)(lVar4 + 4);
    }
  }
  _DAT_180008000 = 1;
  DAT_180008004 = 2;
  uVar9 = DAT_180008018 & 0xfffffffffffffffe;
  if ((uVar5 >> 0x14 & 1) != 0) {
    _DAT_180008000 = 2;
    DAT_180008004 = 6;
    uVar9 = DAT_180008018 & 0xffffffffffffffee;
  }
  DAT_180008018 = uVar9;
  if ((uVar5 >> 0x1b & 1) != 0) {
    uVar9 = xinuse(0);
    uVar9 = in_XCR0 & uVar9 & 0xffffffff;
    if (((uVar5 >> 0x1c & 1) != 0) && (bVar6 = (byte)uVar9, (bVar6 & 6) == 6)) {
      _DAT_180008000 = 3;
      uVar5 = DAT_180008004 | 8;
      uVar7 = DAT_180008018;
      if ((uVar12 & 0x20) != 0) {
        _DAT_180008000 = 5;
        uVar5 = DAT_180008004 | 0x28;
        uVar7 = DAT_180008018 & 0xfffffffffffffffd;
        if (((uVar12 & 0xd0030000) == 0xd0030000) && ((bVar6 & 0xe0) == 0xe0)) {
          DAT_180008004 = DAT_180008004 | 0x68;
          _DAT_180008000 = 6;
          uVar5 = DAT_180008004;
          uVar7 = DAT_180008018 & 0xffffffffffffffd9;
        }
      }
      DAT_180008018 = uVar7;
      DAT_180008004 = uVar5;
      if ((uVar10 >> 0x17 & 1) != 0) {
        DAT_180008018 = DAT_180008018 & 0xfffffffffeffffff;
      }
      if (((uVar11 >> 0x13 & 1) != 0) && ((bVar6 & 0xe0) == 0xe0)) {
        _DAT_180008148 = uVar8 & 0x400ff;
        DAT_180008018 = DAT_180008018 & ~((ulonglong)(uVar8 >> 0x10 & 6) | 0x1000029);
        if (1 < (byte)_DAT_180008148) {
          DAT_180008018 = DAT_180008018 & 0xffffffffffffffbf;
        }
      }
    }
    if (((uVar11 >> 0x15 & 1) != 0) && ((uVar9 >> 0x13 & 1) != 0)) {
      DAT_180008018 = DAT_180008018 & 0xffffffffffffff7f;
    }
  }
  return 0;
}



void FUN_180003074(void)

{
  code *pcVar1;
  
  pcVar1 = (code *)swi(0x29);
  (*pcVar1)(2);
  return;
}



undefined8 * FUN_18000307c(undefined8 *param_1)

{
  param_1[2] = 0;
  param_1[1] = "bad allocation";
  *param_1 = std::bad_alloc::vftable;
  return param_1;
}



void FUN_1800030a0(void)

{
  undefined8 local_28 [5];
  
  FUN_18000307c(local_28);
                    // WARNING: Subroutine does not return
  _CxxThrowException(local_28,(ThrowInfo *)&DAT_180007678);
}



void __cdecl free(void *_Memory)

{
                    // WARNING: Could not recover jumptable at 0x0001800035c2. Too many branches
                    // WARNING: Treating indirect jump as call
  free(_Memory);
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_1800030c8(void)

{
  DWORD DVar1;
  _FILETIME local_res8;
  LARGE_INTEGER local_res10;
  _FILETIME local_18 [2];
  
  if (DAT_180008040 == 0x2b992ddfa232) {
    local_res8.dwLowDateTime = 0;
    local_res8.dwHighDateTime = 0;
    GetSystemTimeAsFileTime(&local_res8);
    local_18[0] = local_res8;
    DVar1 = GetCurrentThreadId();
    local_18[0] = (_FILETIME)((ulonglong)local_18[0] ^ (ulonglong)DVar1);
    DVar1 = GetCurrentProcessId();
    local_18[0] = (_FILETIME)((ulonglong)local_18[0] ^ (ulonglong)DVar1);
    QueryPerformanceCounter(&local_res10);
    DAT_180008040 =
         ((ulonglong)local_res10.s.LowPart << 0x20 ^
          CONCAT44(local_res10.s.HighPart,local_res10.s.LowPart) ^ (ulonglong)local_18[0] ^
         (ulonglong)local_18) & 0xffffffffffff;
    if (DAT_180008040 == 0x2b992ddfa232) {
      DAT_180008040 = 0x2b992ddfa233;
    }
  }
  _DAT_180008080 = ~DAT_180008040;
  return;
}



void FUN_180003178(void)

{
                    // WARNING: Could not recover jumptable at 0x00018000317f. Too many branches
                    // WARNING: Treating indirect jump as call
  InitializeSListHead(&DAT_180008150);
  return;
}



void FUN_180003188(void)

{
  __std_type_info_destroy_list(&DAT_180008150);
  return;
}



undefined * FUN_180003194(void)

{
  return &DAT_180008160;
}



void FUN_18000319c(void)

{
  ulonglong *puVar1;
  
  puVar1 = (ulonglong *)FUN_180001000();
  *puVar1 = *puVar1 | 0x24;
  puVar1 = (ulonglong *)FUN_180003194();
  *puVar1 = *puVar1 | 2;
  return;
}



// Library Function - Single Match
//  __scrt_acquire_startup_lock
// 
// Libraries: Visual Studio 2015 Release, Visual Studio 2017 Release, Visual Studio 2019 Release

ulonglong __scrt_acquire_startup_lock(void)

{
  void *pvVar1;
  bool bVar2;
  undefined7 extraout_var;
  ulonglong uVar4;
  void *pvVar3;
  
  bVar2 = __scrt_is_ucrt_dll_in_use();
  pvVar3 = (void *)CONCAT71(extraout_var,bVar2);
  if ((int)pvVar3 == 0) {
LAB_1800031e6:
    uVar4 = (ulonglong)pvVar3 & 0xffffffffffffff00;
  }
  else {
    do {
      pvVar3 = (void *)0x0;
      LOCK();
      bVar2 = DAT_180008170 == (void *)0x0;
      pvVar1 = StackBase;
      if (!bVar2) {
        pvVar3 = DAT_180008170;
        pvVar1 = DAT_180008170;
      }
      DAT_180008170 = pvVar1;
      UNLOCK();
      if (bVar2) goto LAB_1800031e6;
    } while (StackBase != pvVar3);
    uVar4 = CONCAT71((int7)((ulonglong)pvVar3 >> 8),1);
  }
  return uVar4;
}



// Library Function - Single Match
//  __scrt_dllmain_after_initialize_c
// 
// Libraries: Visual Studio 2015 Release, Visual Studio 2017 Release, Visual Studio 2019 Release

undefined8 __scrt_dllmain_after_initialize_c(void)

{
  bool bVar1;
  undefined7 extraout_var;
  undefined8 uVar2;
  ulonglong uVar3;
  
  bVar1 = __scrt_is_ucrt_dll_in_use();
  if ((int)CONCAT71(extraout_var,bVar1) == 0) {
    uVar3 = FUN_180003560();
    uVar3 = _configure_narrow_argv(uVar3 & 0xffffffff);
    if ((int)uVar3 != 0) {
      return uVar3 & 0xffffffffffffff00;
    }
    uVar2 = _initialize_narrow_environment();
  }
  else {
    uVar2 = FUN_180002dd8();
  }
  return CONCAT71((int7)((ulonglong)uVar2 >> 8),1);
}



bool FUN_180003228(void)

{
  undefined8 uVar1;
  
  uVar1 = FUN_180003360(0);
  return (char)uVar1 != '\0';
}



undefined1 FUN_180003240(void)

{
  char cVar1;
  
  cVar1 = FUN_1800035ec();
  if (cVar1 != '\0') {
    cVar1 = FUN_1800035ec();
    if (cVar1 != '\0') {
      return 1;
    }
    FUN_1800035ec();
  }
  return 0;
}



undefined1 FUN_180003268(void)

{
  FUN_1800035ec();
  FUN_1800035ec();
  return 1;
}



// WARNING: Function: _guard_dispatch_icall replaced with injection: guard_dispatch_icall
// Library Function - Single Match
//  __scrt_dllmain_exception_filter
// 
// Libraries: Visual Studio 2017 Release, Visual Studio 2019 Release

void __scrt_dllmain_exception_filter
               (undefined8 param_1,int param_2,undefined8 param_3,undefined *param_4,
               undefined4 param_5,undefined8 param_6)

{
  bool bVar1;
  undefined7 extraout_var;
  
  bVar1 = __scrt_is_ucrt_dll_in_use();
  if (((int)CONCAT71(extraout_var,bVar1) == 0) && (param_2 == 1)) {
    (*(code *)param_4)(param_1,0,param_3);
  }
  _seh_filter_dll(param_5,param_6);
  return;
}



// Library Function - Single Match
//  __scrt_dllmain_uninitialize_c
// 
// Libraries: Visual Studio 2015 Release, Visual Studio 2017 Release, Visual Studio 2019 Release

void __scrt_dllmain_uninitialize_c(void)

{
  bool bVar1;
  undefined7 extraout_var;
  undefined8 uVar2;
  
  bVar1 = __scrt_is_ucrt_dll_in_use();
  if ((int)CONCAT71(extraout_var,bVar1) != 0) {
    _execute_onexit_table(&DAT_180008180);
    return;
  }
  uVar2 = FUN_1800035f0();
  if ((int)uVar2 == 0) {
    _cexit();
  }
  return;
}



void FUN_180003310(void)

{
  FUN_1800035ec();
  FUN_1800035ec();
  return;
}



longlong FUN_180003324(int param_1)

{
  char cVar1;
  uint7 extraout_var;
  uint7 uVar2;
  undefined7 extraout_var_00;
  uint7 extraout_var_01;
  
  if (param_1 == 0) {
    DAT_180008178 = 1;
  }
  FUN_180002dd8();
  cVar1 = FUN_1800035ec();
  uVar2 = extraout_var;
  if (cVar1 != '\0') {
    cVar1 = FUN_1800035ec();
    if (cVar1 != '\0') {
      return CONCAT71(extraout_var_00,1);
    }
    FUN_1800035ec();
    uVar2 = extraout_var_01;
  }
  return (ulonglong)uVar2 << 8;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined8 FUN_180003360(uint param_1)

{
  bool bVar1;
  ulonglong in_RAX;
  undefined7 extraout_var;
  
  if (DAT_180008179 == '\0') {
    if (1 < param_1) {
                    // WARNING: Subroutine does not return
      FUN_1800034dc(5);
    }
    bVar1 = __scrt_is_ucrt_dll_in_use();
    if (((int)CONCAT71(extraout_var,bVar1) == 0) || (param_1 != 0)) {
      in_RAX = 0xffffffffffffffff;
      _DAT_180008180 = 0xffffffffffffffff;
      uRam0000000180008188 = 0xffffffffffffffff;
      _DAT_180008190 = 0xffffffffffffffff;
      _DAT_180008198 = 0xffffffffffffffff;
      uRam00000001800081a0 = 0xffffffffffffffff;
      _DAT_1800081a8 = 0xffffffffffffffff;
    }
    else {
      in_RAX = _initialize_onexit_table(&DAT_180008180);
      if (((int)in_RAX != 0) ||
         (in_RAX = _initialize_onexit_table(&DAT_180008198), (int)in_RAX != 0)) {
        return in_RAX & 0xffffffffffffff00;
      }
    }
    DAT_180008179 = '\x01';
  }
  return CONCAT71((int7)(in_RAX >> 8),1);
}



// WARNING: Removing unreachable block (ram,0x000180003479)
// WARNING: Enum "SectionFlags": Some values do not have unique names

ulonglong FUN_1800033ec(longlong param_1)

{
  ulonglong uVar1;
  uint7 uVar2;
  IMAGE_SECTION_HEADER *pIVar3;
  
  uVar1 = 0;
  for (pIVar3 = &IMAGE_SECTION_HEADER_180000210; pIVar3 != (IMAGE_SECTION_HEADER *)&DAT_180000300;
      pIVar3 = pIVar3 + 1) {
    if (((ulonglong)(uint)pIVar3->VirtualAddress <= param_1 - 0x180000000U) &&
       (uVar1 = (ulonglong)((pIVar3->Misc).PhysicalAddress + pIVar3->VirtualAddress),
       param_1 - 0x180000000U < uVar1)) goto LAB_180003462;
  }
  pIVar3 = (IMAGE_SECTION_HEADER *)0x0;
LAB_180003462:
  if (pIVar3 == (IMAGE_SECTION_HEADER *)0x0) {
    uVar1 = uVar1 & 0xffffffffffffff00;
  }
  else {
    uVar2 = (uint7)(uVar1 >> 8);
    if ((int)pIVar3->Characteristics < 0) {
      uVar1 = (ulonglong)uVar2 << 8;
    }
    else {
      uVar1 = CONCAT71(uVar2,1);
    }
  }
  return uVar1;
}



// Library Function - Single Match
//  __scrt_release_startup_lock
// 
// Libraries: Visual Studio 2015 Release, Visual Studio 2017 Release, Visual Studio 2019 Release

void __scrt_release_startup_lock(char param_1)

{
  bool bVar1;
  undefined3 extraout_var;
  
  bVar1 = __scrt_is_ucrt_dll_in_use();
  if ((CONCAT31(extraout_var,bVar1) != 0) && (param_1 == '\0')) {
    LOCK();
    DAT_180008170 = 0;
    UNLOCK();
  }
  return;
}



// Library Function - Single Match
//  __scrt_uninitialize_crt
// 
// Library: Visual Studio 2019 Release

undefined1 __scrt_uninitialize_crt(undefined8 param_1,char param_2)

{
  if ((DAT_180008178 == '\0') || (param_2 == '\0')) {
    FUN_1800035ec();
    FUN_1800035ec();
  }
  return 1;
}



undefined * FUN_1800034d4(void)

{
  return &DAT_180008200;
}



void FUN_1800034dc(undefined4 param_1)

{
  code *pcVar1;
  
  pcVar1 = (code *)swi(0x29);
  (*pcVar1)(param_1);
  return;
}



// WARNING: Function: _guard_dispatch_icall replaced with injection: guard_dispatch_icall

void FUN_1800034e4(void)

{
  undefined8 *puVar1;
  
  for (puVar1 = &DAT_180007300; puVar1 < &DAT_180007300; puVar1 = puVar1 + 1) {
    if ((code *)*puVar1 != (code *)0x0) {
      (*(code *)*puVar1)();
    }
  }
  return;
}



// WARNING: Function: _guard_dispatch_icall replaced with injection: guard_dispatch_icall

void FUN_180003520(void)

{
  undefined8 *puVar1;
  
  for (puVar1 = &DAT_180007310; puVar1 < &DAT_180007310; puVar1 = puVar1 + 1) {
    if ((code *)*puVar1 != (code *)0x0) {
      (*(code *)*puVar1)();
    }
  }
  return;
}



void _guard_check_icall(void)

{
  return;
}



undefined8 FUN_180003560(void)

{
  return 1;
}



// Library Function - Single Match
//  __scrt_is_ucrt_dll_in_use
// 
// Libraries: Visual Studio 2017 Release, Visual Studio 2019 Release

bool __scrt_is_ucrt_dll_in_use(void)

{
  return DAT_180008090 != 0;
}



void __CxxFrameHandler4(void)

{
                    // WARNING: Could not recover jumptable at 0x000180003580. Too many branches
                    // WARNING: Treating indirect jump as call
  __CxxFrameHandler4();
  return;
}



void * __cdecl memcpy(void *_Dst,void *_Src,size_t _Size)

{
  void *pvVar1;
  
                    // WARNING: Could not recover jumptable at 0x000180003586. Too many branches
                    // WARNING: Treating indirect jump as call
  pvVar1 = memcpy(_Dst,_Src,_Size);
  return pvVar1;
}



void * __cdecl memmove(void *_Dst,void *_Src,size_t _Size)

{
  void *pvVar1;
  
                    // WARNING: Could not recover jumptable at 0x00018000358c. Too many branches
                    // WARNING: Treating indirect jump as call
  pvVar1 = memmove(_Dst,_Src,_Size);
  return pvVar1;
}



void * __cdecl memset(void *_Dst,int _Val,size_t _Size)

{
  void *pvVar1;
  
                    // WARNING: Could not recover jumptable at 0x000180003592. Too many branches
                    // WARNING: Treating indirect jump as call
  pvVar1 = memset(_Dst,_Val,_Size);
  return pvVar1;
}



void __stdcall _CxxThrowException(void *pExceptionObject,ThrowInfo *pThrowInfo)

{
                    // WARNING: Could not recover jumptable at 0x00018000359e. Too many branches
                    // WARNING: Subroutine does not return
                    // WARNING: Treating indirect jump as call
  _CxxThrowException(pExceptionObject,pThrowInfo);
  return;
}



void __std_type_info_destroy_list(void)

{
                    // WARNING: Could not recover jumptable at 0x0001800035a4. Too many branches
                    // WARNING: Treating indirect jump as call
  __std_type_info_destroy_list();
  return;
}



int __cdecl _callnewh(size_t _Size)

{
  int iVar1;
  
                    // WARNING: Could not recover jumptable at 0x0001800035aa. Too many branches
                    // WARNING: Treating indirect jump as call
  iVar1 = _callnewh(_Size);
  return iVar1;
}



void * __cdecl malloc(size_t _Size)

{
  void *pvVar1;
  
                    // WARNING: Could not recover jumptable at 0x0001800035b0. Too many branches
                    // WARNING: Treating indirect jump as call
  pvVar1 = malloc(_Size);
  return pvVar1;
}



void _initterm(void)

{
                    // WARNING: Could not recover jumptable at 0x0001800035b6. Too many branches
                    // WARNING: Treating indirect jump as call
  _initterm();
  return;
}



void _initterm_e(void)

{
                    // WARNING: Could not recover jumptable at 0x0001800035bc. Too many branches
                    // WARNING: Treating indirect jump as call
  _initterm_e();
  return;
}



void __cdecl free(void *_Memory)

{
                    // WARNING: Could not recover jumptable at 0x0001800035c2. Too many branches
                    // WARNING: Treating indirect jump as call
  free(_Memory);
  return;
}



void _seh_filter_dll(void)

{
                    // WARNING: Could not recover jumptable at 0x0001800035c8. Too many branches
                    // WARNING: Treating indirect jump as call
  _seh_filter_dll();
  return;
}



void _configure_narrow_argv(void)

{
                    // WARNING: Could not recover jumptable at 0x0001800035ce. Too many branches
                    // WARNING: Treating indirect jump as call
  _configure_narrow_argv();
  return;
}



void _initialize_narrow_environment(void)

{
                    // WARNING: Could not recover jumptable at 0x0001800035d4. Too many branches
                    // WARNING: Treating indirect jump as call
  _initialize_narrow_environment();
  return;
}



void _initialize_onexit_table(void)

{
                    // WARNING: Could not recover jumptable at 0x0001800035da. Too many branches
                    // WARNING: Treating indirect jump as call
  _initialize_onexit_table();
  return;
}



void _execute_onexit_table(void)

{
                    // WARNING: Could not recover jumptable at 0x0001800035e0. Too many branches
                    // WARNING: Treating indirect jump as call
  _execute_onexit_table();
  return;
}



void __cdecl _cexit(void)

{
                    // WARNING: Could not recover jumptable at 0x0001800035e6. Too many branches
                    // WARNING: Treating indirect jump as call
  _cexit();
  return;
}



undefined1 FUN_1800035ec(void)

{
  return 1;
}



undefined8 FUN_1800035f0(void)

{
  return 0;
}



void FUN_1800035f4(longlong param_1,longlong param_2,undefined8 param_3,longlong param_4)

{
  longlong lVar1;
  
  lVar1 = *(longlong *)(param_4 + 0x38);
  FUN_180002d74(param_2,param_4);
  if ((*(uint *)(lVar1 + 4) & ((*(uint *)(param_1 + 4) & 0x66) != 0) + 1) != 0) {
    __CxxFrameHandler4(param_1,param_2,param_3,param_4);
  }
  return;
}



size_t __cdecl strlen(char *_Str)

{
  size_t sVar1;
  
                    // WARNING: Could not recover jumptable at 0x000180003673. Too many branches
                    // WARNING: Treating indirect jump as call
  sVar1 = strlen(_Str);
  return sVar1;
}



size_t __cdecl wcslen(wchar_t *_Str)

{
  size_t sVar1;
  
                    // WARNING: Could not recover jumptable at 0x000180003679. Too many branches
                    // WARNING: Treating indirect jump as call
  sVar1 = wcslen(_Str);
  return sVar1;
}



// WARNING: This is an inlined function

void _guard_dispatch_icall(void)

{
  code *UNRECOVERED_JUMPTABLE;
  
                    // WARNING: Could not recover jumptable at 0x000180003690. Too many branches
                    // WARNING: Treating indirect jump as call
  (*UNRECOVERED_JUMPTABLE)();
  return;
}



// WARNING: This is an inlined function

void _guard_dispatch_icall(void)

{
  code *UNRECOVERED_JUMPTABLE;
  
                    // WARNING: Could not recover jumptable at 0x000180003690. Too many branches
                    // WARNING: Treating indirect jump as call
  (*UNRECOVERED_JUMPTABLE)();
  return;
}



void FUN_1800036dc(undefined8 param_1,longlong param_2)

{
  __scrt_release_startup_lock(*(char *)(param_2 + 0x40));
  return;
}



void FUN_1800036f3(undefined8 param_1,longlong param_2)

{
  __scrt_release_startup_lock(*(char *)(param_2 + 0x20));
  return;
}



void FUN_18000370c(void)

{
  FUN_180003310();
  return;
}



void FUN_180003720(undefined8 *param_1,longlong param_2)

{
  __scrt_dllmain_exception_filter
            (*(undefined8 *)(param_2 + 0x60),*(int *)(param_2 + 0x68),
             *(undefined8 *)(param_2 + 0x70),FUN_180002a00,*(undefined4 *)*param_1,param_1);
  return;
}



bool FUN_180003756(undefined8 *param_1)

{
  return *(int *)*param_1 == -0x3ffffffb;
}


