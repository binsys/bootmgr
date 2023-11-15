//---------------------------------------------------------------------
#define FILE_PATH_VERSION (1)

// https://github.com/selfrender/Windows-Server-2003/blob/5c6fe3db626b63a384230a1aa6b92ac416b0765f/base/efiutil/efinvram/nvrio.h#L85
#define FILE_PATH_TYPE_ARC           (1)
#define FILE_PATH_TYPE_ARC_SIGNATURE (2)
#define FILE_PATH_TYPE_NT            (3) // FILE_PATH_TYPE_WINDOWS ?
#define FILE_PATH_TYPE_EFI           (4)
#define FILE_PATH_TYPE_BOOT_ENV      (5)

#pragma pack(push, 4)
typedef struct _FILE_PATH
{
	ULONG Version;
	ULONG Length;
	ULONG Type;
	UCHAR FilePath[ANYSIZE_ARRAY];
} FILE_PATH, * PFILE_PATH;
C_ASSERT(sizeof(FILE_PATH) == 0x10);
#pragma pack(pop)

typedef UNALIGNED PFILE_PATH PUNALIGNED_FILE_PATH;
typedef UNALIGNED FILE_PATH  UNALIGNED_FILE_PATH;
//---------------------------------------------------------------------
typedef enum _PartitionTYPE
{
	GPT = 0,
	MBR = 1,
	RAW = 2,
} PartitionTYPE;

typedef PartitionTYPE PARTITION_FORMAT;
//---------------------------------------------------------------------
// hard disk driver
typedef struct _HARD_DISK_IDENTIFIER
{
	PartitionTYPE PartitionType;
	union
	{
		struct
		{
			ULONG Signature;
		} Mbr;

		struct
		{
			GUID Signature;
		} Gpt;

		struct
		{
			ULONG DiskNumber;
		} Raw;
	};
} HARD_DISK_IDENTIFIER, * PHARD_DISK_IDENTIFIER;
C_ASSERT(sizeof(HARD_DISK_IDENTIFIER) == 0x14);
//---------------------------------------------------------------------
// floppy
typedef struct _REMOVABLE_DISK_IDENTIFIER
{
	ULONG DriveNumber;
} REMOVABLE_DISK_IDENTIFIER, * PREMOVABLE_DISK_IDENTIFIER;
C_ASSERT(sizeof(REMOVABLE_DISK_IDENTIFIER) == 0x04);
//---------------------------------------------------------------------
// cdrom
typedef struct _CDROM_IDENTIFIER
{
	/* OFFSET 0x14 */ ULONG CdromNumber;
} CDROM_IDENTIFIER, * PCDROM_IDENTIFIER;
C_ASSERT(sizeof(CDROM_IDENTIFIER) == 0x04);
//---------------------------------------------------------------------
// ramdisk
#pragma pack(push, 1)
typedef struct _RAM_DISK_IDENTIFIER
{
	/* OFFSET 0x14 */ PHYSICAL_ADDRESS ImageBase;
	/* OFFSET 0x1c */ LONGLONG ImageSize;
	/* OFFSET 0x24 */ ULONG ImageOffset;
	FILE_IDENTIFIER Source;
} RAM_DISK_IDENTIFIER, * PRAM_DISK_IDENTIFIER;
#pragma pack(pop)
#if _M_IX86
C_ASSERT(sizeof(RAM_DISK_IDENTIFIER) == 0x24);
#elif _M_AMD64
C_ASSERT(sizeof(RAM_DISK_IDENTIFIER) == 0x24);
#else
#pragma PRAGMA_MSG("NotImpl")
#endif
//---------------------------------------------------------------------
// unknown size
typedef struct _BLOCK_ID_IDENTIFIER_PARTITION
{
	ULONG Dummy;
} BLOCK_IO_IDENTIFIER_PARTITION, * PBLOCK_ID_IDENTIFIER_PARTITION;
//---------------------------------------------------------------------
typedef struct _FILE_IDENTIFIER
{
	UNALIGNED_FILE_PATH SourcePath;
} FILE_IDENTIFIER, * PFILE_IDENTIFIER;
C_ASSERT(sizeof(FILE_IDENTIFIER) == 0x10);
//---------------------------------------------------------------------

// unknown member name
#define VHD_PARTITION_EX_FILE_OFFSET (0x38)

typedef struct _VIRTUAL_HARD_DISK_IDENTIFIER
{
	ULONG Offset00;
	ULONG Offset04;
	ULONG Offset08;
	ULONG Offset0c;
	ULONG Offset10;
} VIRTUAL_HARD_DISK_IDENTIFIER, * PVIRTUAL_HARD_DISK_IDENTIFIER;
C_ASSERT(sizeof(VIRTUAL_HARD_DISK_IDENTIFIER) == 0x14);

//---------------------------------------------------------------------

typedef enum _BLOCK_IO_TYPE
{
	BLOCK_IO_HARD_DISK         = 0,
	BLOCK_IO_REMOVABLE_DISK    = 1, // floppy
	BLOCK_IO_CDROM             = 2,
	BLOCK_IO_RAMDISK           = 3,
	BLOCK_IO_PARTITION         = 4,
	BLOCK_IO_FILE              = 5,
	BLOCK_IO_VIRTUAL_HARD_DISK = 6,
	BLOCK_IO_MAX_TYPE,
} BLOCK_IO_TYPE;

#pragma pack(push, 4)
typedef struct _BLOCK_IO_IDENTIFIER
{

	BLOCK_IO_TYPE BlockIoType;

	union
	{
		// BlockIoType = BLOCK_IO_HARD_DISK = 0
		HARD_DISK_IDENTIFIER HardDisk;

		// BlockIoType = BLOCK_IO_REMOVABLE_DISK = 1 
		REMOVABLE_DISK_IDENTIFIER RemovableDisk;

		// BlockIoType = BLOCK_IO_CDROM = 2
		CDROM_IDENTIFIER Cdrom;

		// BlockIoType = BLOCK_IO_RAMDISK = 3
		RAM_DISK_IDENTIFIER RamDisk;

		// BlockIoType = BLOCK_IO_PARTITION = 4
		BLOCK_IO_IDENTIFIER_PARTITION Partition;

		// BlockIoType = BLOCK_IO_FILE = 5
		FILE_IDENTIFIER File;

		// BlockIoType = BLOCK_IO_VIRTUAL_HARD_DISK = 6
		VIRTUAL_HARD_DISK_IDENTIFIER VirtualHardDisk;
	} u;
} BLOCK_IO_IDENTIFIER, * PBLOCK_IO_IDENTIFIER;

#if _M_IX86
C_ASSERT(sizeof(BLOCK_IO_IDENTIFIER) == 0x28);
#elif _M_AMD64
C_ASSERT(sizeof(BLOCK_IO_IDENTIFIER) == 0x28);
#else
#pragma PRAGMA_MSG("NotImpl")
#endif

#pragma pack(pop)

//---------------------------------------------------------------------
typedef struct _PARTITION_IDENTIFIER
{
	union
	{
		struct
		{
			PVOID BootEntry;
		} ElTorito;

		struct
		{
			GUID PartitionSignature;
		} Gpt;

		struct
		{
			ULONG PartitionNumber;
		} Mbr;
	};
	BLOCK_IO_IDENTIFIER ParentIdentifier;
} PARTITION_IDENTIFIER, * PPARTITION_IDENTIFIER;

#if _M_IX86
C_ASSERT(sizeof(PARTITION_IDENTIFIER) == (0x10 + 0x28));
#elif _M_AMD64
C_ASSERT(sizeof(PARTITION_IDENTIFIER) == (0x10 + 0x28));
#else
#pragma PRAGMA_MSG("NotImpl")
#endif
//---------------------------------------------------------------------
// serial port for debug
typedef struct _SERIAL_PORT_IDENTIFIER
{
	ULONG Type;
	union
	{
		struct
		{
			ULONG PortNumber;
		} LEGACY_16550;
		struct
		{
			GEN_ADDR BaseAddress;
		} GEN_16550;
	} u;
} SERIAL_PORT_IDENTIFIER,*PSERIAL_PORT_IDENTIFIER;
C_ASSERT(sizeof(SERIAL_PORT_IDENTIFIER) == 0x10);
//---------------------------------------------------------------------
#pragma pack(push, 4)
typedef struct _PARTITION_IDENTIFIER_EX
{
	union
	{
		struct
		{
			ULONG BootEntry;
		} ElTorito;

		struct
		{
			GUID PartitionSignature;
		} Gpt;

		struct
		{
			ULONGLONG PartitionOffset;
		} Mbr;
	};
	BLOCK_IO_IDENTIFIER ParentIdentifier;
} PARTITION_IDENTIFIER_EX, * PPARTITION_IDENTIFIER_EX;
#pragma pack(pop)
#if _M_IX86
C_ASSERT(sizeof(PARTITION_IDENTIFIER_EX) == (0x10 + 0x28));
#elif _M_AMD64
C_ASSERT(sizeof(PARTITION_IDENTIFIER_EX) == (0x10 + 0x28));
#else
#pragma PRAGMA_MSG("NotImpl")
#endif

//---------------------------------------------------------------------

// udp pxe
#define UDP_IDENTIFIER_HW_TYPE_BOOT_DEVICE 0x100

typedef struct _UDP_IDENTIFIER
{
	ULONG HwType;
	UCHAR HwAddress[16];
} UDP_IDENTIFIER, * PUDP_IDENTIFIER;
C_ASSERT(sizeof(UDP_IDENTIFIER) == 0x14);
//---------------------------------------------------------------------
// HyperV VM BUS
typedef struct _BOOT_ENVIRONMENT_DEVICE_VMBUS
{
	GUID InterfaceType;
	GUID InterfaceInstance;
} BOOT_ENVIRONMENT_DEVICE_VMBUS, * PBOOT_ENVIRONMENT_DEVICE_VMBUS;

//---------------------------------------------------------------------

#define LOCATE_TYPE_STRING 1

typedef struct _LOCATE_IDENTIFIER
{
	ULONG Type;
	ULONG ElementType;
	ULONG ParentOffset;
	PWCHAR Path;
} LOCATE_IDENTIFIER, * PLOCATE_IDENTIFIER;

//---------------------------------------------------------------------




typedef enum _DEVICE_TYPE
{
	DEVICE_BLOCK_IO     = 0,

	// deprecated
	// identifiers MBR partitions using partition numbers
	DEVICE_PARTITION    = 2,
	DEVICE_SERIAL_PORT  = 3,
	DEVICE_UDP          = 4,
	DEVICE_BOOT         = 5,

	// identifies MBR partitions using offsets on disk,
	DEVICE_PARTITION_EX = 6,
	DEVICE_VMBUS        = 7,

	// http://www.mistyprojects.co.uk/documents/BCDEdit/files/device_locate.htm
	// NEED PARENT
	DEVICE_LOCATE       = 8,
} DEVICE_TYPE;

//---------------------------------------------------------------------


#define DEVICE_FLAG_IDENTIFIER_UNSPECIFIED   (1)
#define DEVICE_FLAG_PARTITION_FORCE_FVE      (2)
#define DEVICE_FLAG_PARENT_SIGNATURE_INVALID (4)
//---------------------------------------------------------------------

typedef struct _BOOT_ENVIRONMENT_DEVICE
{
	DEVICE_TYPE DeviceType;
	ULONG Flags;
	ULONG Size;
	ULONG Pad;

	union {

		// DeviceType = DEVICE_BLOCK_IO = 0;
		BLOCK_IO_IDENTIFIER BlockIo;

		// DeviceType = DEVICE_PARTITION = 2
		PARTITION_IDENTIFIER Partition;

		// DeviceType = DEVICE_SERIAL_PORT = 3
		SERIAL_PORT_IDENTIFIER SerialPort;

		// DeviceType = DEVICE_UDP = 4;
		UDP_IDENTIFIER Udp;

		// DeviceType = DEVICE_BOOT = 5;
		DEVICE_BOOT //need resovle to "true" device at runtime,may be come from BCD BcdLibraryDevice_ApplicationDevice or BOOT_APPLICATION_PARAMETERS.BootDeviceOffset

		// DeviceType = DEVICE_PARTITION_EX = 6;
		PARTITION_IDENTIFIER_EX PartitionEx;

		// DeviceType = DEVICE_VMBUS = 7;
		BOOT_ENVIRONMENT_DEVICE_VMBUS Vmbus;

		// DeviceType = DEVICE_LOCATE = 8;
		LOCATE_IDENTIFIER Locate;

		struct
		{
			UCHAR Placeholder[0x38];
		} Dummy;
	} u;
} BOOT_ENVIRONMENT_DEVICE, * PBOOT_ENVIRONMENT_DEVICE;
#if _M_IX86
C_ASSERT(sizeof(BOOT_ENVIRONMENT_DEVICE) == 0x48);
#elif _M_AMD64
C_ASSERT(sizeof(BOOT_ENVIRONMENT_DEVICE) == 0x48);
#else
#pragma PRAGMA_MSG("NotImpl")
#endif


//---------------------------------------------------------------------





typedef enum
{
	BCDE_FORMAT_NONE         = 0x0,

	// A boot environment device, represented as a GUID in string format.For example "{500ec897-0b25-449a-8ecd-8d81628baa9c}".
	BCDE_FORMAT_DEVICE       = 0x1,

	//  A NULL - terminated Unicode string.
	BCDE_FORMAT_STRING       = 0x2,

	// A GUID in string format.
	BCDE_FORMAT_OBJECT       = 0x3,

	// An array of GUIDs.
	BCDE_FORMAT_OBJECT_LIST  = 0x4,

	// A 64 - bit integer.
	BCDE_FORMAT_INTEGER      = 0x5,

	// A boolean.
	BCDE_FORMAT_BOOLEAN      = 0x6,

	// An array of 64 - bit integers.
	BCDE_FORMAT_INTEGER_LIST = 0x7,

	// Binary
	BCDE_FORMAT_BINARY       = 0x8,

} BCDE_FORMAT;

//---------------------------------------------------------------------

typedef struct
{
	union
	{
		ULONG PackedValue;
		struct
		{
			ULONG SubType : 24;
			ULONG Format  :  4;
			ULONG Class   :  4;
		};
	};
} BCDE_DATA_TYPE_STRUCT, * PBCDE_DATA_TYPE_STRUCT;
C_ASSERT(sizeof(BCDE_DATA_TYPE_STRUCT) == 4);
//---------------------------------------------------------------------
typedef ULONG BCDE_DATA_TYPE;
C_ASSERT(sizeof(BCDE_DATA_TYPE) == 4); // same as BCDE_DATA_TYPE_STRUCT
//---------------------------------------------------------------------
typedef struct _BOOT_ENTRY_OPTION
{
	BCDE_DATA_TYPE Type;
	ULONG DataOffset;
	ULONG DataSize;
	union
	{
		ULONG AssociatedOptionsOffset;
		ULONG FveHashState;
	};
	ULONG NextOptionOffset;
	ULONG Invalid;
} BOOT_ENTRY_OPTION, * PBOOT_ENTRY_OPTION;
C_ASSERT(sizeof(BOOT_ENTRY_OPTION) == 0x18);
//---------------------------------------------------------------------

#define BOOT_ENTRY_IDENTIFIER_UNSPECIFIED       (0x00000001)
#define BOOT_ENTRY_OPTIONS_ALLOCATED            (0x00000002)
#define BOOT_ENTRY_OS_LOADER                    (0x00000004)
#define BOOT_ENTRY_RESTART_LOADER               (0x00000008)

#define BOOT_ENTRY_NEED_RESTART                 (0x00000020)
#define BOOT_ENTRY_NTLDR                        (0x00000040)
#define BOOT_ENTRY_OPTIONS_ALLOCATED_EXTERNALLY (0x00000080)

#define BOOT_ENTRY_OS_RESUME_LOADER             (0x00000100)
#define BOOT_ENTRY_SETUPLDR                     (0x00000200)
#define BOOT_ENTRY_BOOT_SECTOR                  (0x00000400)

#define BOOT_ENTRY_BOOT_MANAGER                 (0x00001000)

#define BOOT_ENTRY_DEBUG_TRANSITION             (0x00004000)

#define BOOT_ENTRY_DISPLAY                      (0x08000000)

#define BOOT_ENTRY_TOOL                         (0x10000000)
//---------------------------------------------------------------------

#define BOOT_ENTRY_SIGNATURE "BTAPENT"

#pragma pack(push, 1)
typedef struct _BOOT_APPLICATION_TRANSITION_ENTRY
{
	CHAR Signature[8];
	ULONG Attributes;
	GUID Identifier;
	GUID unknow;
	BOOT_ENTRY_OPTION InlineOptions; // BOOT_ENTRY_OPTION InlineOptions[ANYSIZE_ARRAY] ?
} BOOT_APPLICATION_TRANSITION_ENTRY, * PBOOT_APPLICATION_TRANSITION_ENTRY;
#pragma pack(pop)
C_ASSERT(sizeof(BOOT_APPLICATION_TRANSITION_ENTRY) == 0x44);
//---------------------------------------------------------------------
typedef struct _BOOT_APPLICATION_ENTRY
{
	ULONG Attributes;
	GUID Identifier;
	PBOOT_ENTRY_OPTION Options;
} BOOT_APPLICATION_ENTRY, * PBOOT_APPLICATION_ENTRY;
typedef BOOT_APPLICATION_ENTRY BOOT_ENTRY;
//---------------------------------------------------------------------


typedef struct _MEMORY_DESCRIPTOR
{
	/* OFFSET 0x00 */
	LIST_ENTRY ListEntry;

	union
	{
		struct {
			ULONG Placeholder;
		} FixedLengthData;
		
		struct {
			/* OFFSET 0x08 */
			ULONGLONG BasePage;

			/* OFFSET 0x10 */
			ULONGLONG MappedBasePage;

			/* OFFSET 0x18 */
			ULONGLONG PageCount;

			/* OFFSET 0x20 */
			ULONG Attributes;

			/* OFFSET 0x24 */
			ULONG MemoryType;
		};

		struct {
			/* OFFSET 0x08 */
			ULONGLONG Base;

			/* OFFSET 0x10 */
			ULONGLONG MappedBase;

			/* OFFSET 0x18 */

			ULONGLONG Size;

			/* OFFSET 0x20 */
			ULONG Attributes;

			/* OFFSET 0x24 */
			ULONG MemoryType;
		} ByteGranular;
	};
} MEMORY_DESCRIPTOR, * PMEMORY_DESCRIPTOR;
//---------------------------------------------------------------------
#define ALLOCATED_MEMORY_VERSION            1


typedef struct _ALLOCATED_MEMORY
{
	//ALLOCATED_MEMORY_VERSION
	ULONG Version;

	// sizeof(ALLOCATED_MEMORY);
	ULONG MdlOffset;

	ULONG NumberOfDescriptors;

	// sizeof(MEMORY_DESCRIPTOR);
	ULONG SizeOfDescriptor;

	// UFIELD_OFFSET(MEMORY_DESCRIPTOR,FixedLengthData);
	ULONG FixedDataOffset;

} ALLOCATED_MEMORY, * PALLOCATED_MEMORY;
C_ASSERT(sizeof(ALLOCATED_MEMORY) == 0x14);

//---------------------------------------------------------------------
#define BOOT_APPLICATION_PARAMETERS_SIGNATURE						((ULONGLONG)'PPA ' <<32 | (ULONGLONG)'TOOB')
#define BOOT_APPLICATION_PARAMETERS_VERSION 2

// note: on amd64 pcat boot,bootmgr is 32bit IMAGE_FILE_MACHINE_I386 exe.
#if defined(_X86_)
#define BOOT_APPLICATION_MACHINE_TYPE IMAGE_FILE_MACHINE_I386
#endif

	/*
	 BOOT_APPLICATION_MACHINE_TYPE list:
	 IMAGE_FILE_MACHINE_I386
	 IMAGE_FILE_MACHINE_AMD64
	 IMAGE_FILE_MACHINE_ARMNT
	 IMAGE_FILE_MACHINE_ARM64
	*/


typedef struct _BOOT_APPLICATION_PARAMETERS_HEADER
{
	/* OFFSET 0x00 */
	// BOOT_APPLICATION_PARAMETERS_SIGNATURE
	ULONGLONG Signature;

	/* OFFSET 0x08 */
	// BOOT_APPLICATION_PARAMETERS_VERSION
	ULONG Version;

	/* OFFSET 0x0c */
	ULONG SizeOfParameters;
} BOOT_APPLICATION_PARAMETERS_HEADER, * PBOOT_APPLICATION_PARAMETERS_HEADER;

C_ASSERT(sizeof(BOOT_APPLICATION_PARAMETERS_HEADER) == 16);


//---------------------------------------------------------------------
#define BOOT_APPLICATION_RETURN_ARGUMENTS_VERSION         (0x01)

#define RETURN_ARGUMENT_LAUNCH_RECOVERY                   (0x01)
#define RETURN_ARGUMENT_LAUNCH_RECOVERY_MANUAL            (0x04)
#define RETURN_ARGUMENT_LAUNCH_RECOVERY_FOR_BAD_BOOT      (0x08)
#define RETURN_ARGUMENT_LAUNCH_RECOVERY_FOR_BAD_SHUTDOWN  (0x10)
#define RETURN_ARGUMENT_LAUNCH_RECOVERY_FOR_UPDATE        (0x20)
#define RETURN_ARGUMENT_NO_PAE_SUPPORT                    (0x40)

#pragma pack(push, 4)
typedef struct _BOOT_APPLICATION_RETURN_ARGUMENTS
{
	ULONG Version;
	ULONG ReturnStatus;
	ULONG Flags;
	struct
	{
		ULONGLONG PageCount;
		ULONGLONG PhysicalBasePage;
	} PersistentData;
} BOOT_APPLICATION_RETURN_ARGUMENTS, * PBOOT_APPLICATION_RETURN_ARGUMENTS;
#pragma pack(pop)

C_ASSERT(sizeof(BOOT_APPLICATION_RETURN_ARGUMENTS) == 0x1c);
//---------------------------------------------------------------------
#if defined(PCAT)

	#pragma pack(push, 1)
	typedef struct _FAR_POINTER
	{
		UINT16 Offset;
		UINT16 Segment;
	} FAR_POINTER, * PFAR_POINTER;

	C_ASSERT(sizeof(FAR_POINTER) == sizeof(UINT32));

	typedef struct _PCAT_SERVICE_CONTEXT {

		// 0x00
		union {
			ULONG IntX;
			FAR_POINTER FarCall;
		};

		// 0x04
		union {
			struct {
				UINT8 AL;
				UINT8 AH;
			};
			UINT16 AX;
			UINT32 EAX;
		};

		// 0x08
		union {
			struct {
				UINT8 BL;
				UINT8 BH;
			};
			UINT16 BX;
			UINT32 EBX;
		};

		// 0x0c
		union {
			struct {
				UINT8 CL;
				UINT8 CH;
			};
			UINT16 CX;
			UINT32 ECX;
		};

		// 0x10
		union {
			struct {
				UINT8 DL;
				UINT8 DH;
			};
			UINT16 DX;
			UINT32 EDX;
		};

		// 0x14
		UINT32 ESP;

		// 0x18
		UINT32 EBP;

		// 0x1c
		union {
			UINT16 SI;
			UINT32 ESI;
		};

		// 0x20
		union {
			UINT16 DI;
			UINT32 EDI;
		};

		// 0x24
		UINT32 CS;

		// 0x28
		UINT32 DS;

		// 0x2c
		UINT32 SS;

		// 0x30
		UINT32 ES;

		// 0x34
		UINT32 FS;

		// 0x38
		UINT32 GS;

		// 0x3c
		// https://en.wikipedia.org/wiki/FLAGS_register
		union
		{
			// 0x3c
			UINT32 Value;

			// 0x3c
			struct
			{
				UINT32 CF     : 1; // 0x00000001
				UINT32 Resv1  : 1; // 0x00000002
				UINT32 PF     : 1; // 0x00000004
				UINT32 Resv3  : 1; // 0x00000008
				UINT32 AF     : 1; // 0x00000010
				UINT32 Resv5  : 1; // 0x00000020
				UINT32 ZF     : 1; // 0x00000040
				UINT32 SF     : 1; // 0x00000080
				UINT32 TF     : 1; // 0x00000100
				UINT32 IF     : 1; // 0x00000200
				UINT32 DF     : 1; // 0x00000400
				UINT32 OF     : 1; // 0x00000800
				UINT32 IOPL   : 2; // 0x1000 0x2000 0x3000
				UINT32 NT     : 1; // 0x00004000
				UINT32 MD     : 1; // 0x00008000

				UINT32 RF     : 1; // 0x00010000
				UINT32 VM     : 1; // 0x00020000
				UINT32 AC     : 1; // 0x00040000
				UINT32 VIF    : 1; // 0x00080000
				UINT32 VIP    : 1; // 0x00100000
				UINT32 ID     : 1; // 0x00200000
				UINT32 RESV22 : 8;
				UINT32 NONE   : 1; // 0x40000000
				UINT32 AI     : 1; // 0x80000000
			};
		} EFlags;
	} PCAT_SERVICE_CONTEXT, * PPCAT_SERVICE_CONTEXT;

	C_ASSERT(sizeof(PCAT_SERVICE_CONTEXT) == 0x40);

	#pragma pack(pop)

	typedef void(__cdecl* PFN_PcatBiosInterrupt) (PPCAT_SERVICE_CONTEXT Context);
	typedef void(__cdecl* PFN_PcatExecuteRealModeCode) (PPCAT_SERVICE_CONTEXT Context);
	
	typedef struct _PCAT_SERVICES_TABLE
	{
		PFN_PcatBiosInterrupt PcatBiosInterrupt;
		PFN_PcatExecuteRealModeCode PcatExecuteRealModeCode;
	} PCAT_SERVICES_TABLE, * PPCAT_SERVICES_TABLE;

	typedef struct _PCAT_FIRMWARE_PARAMETERS
	{
		PPCAT_SERVICES_TABLE PcatServicesTable;
		ULONG BootDriverNumber;
	} PCAT_FIRMWARE_PARAMETERS, * PPCAT_FIRMWARE_PARAMETERS;

	typedef struct _FIRMWARE_PARAMETERS
	{
		PPCAT_FIRMWARE_PARAMETERS PcatParameters;
		ULONG UnknownParameters;
	} FIRMWARE_PARAMETERS, * PFIRMWARE_PARAMETERS;

#else defined(EFI)


#endif
//---------------------------------------------------------------------
typedef enum _VA_TRANSLATION_TYPE
{
	VA_TRANSLATION_TYPE_NONE,

#ifdef MM_MIN
	VA_TRANSLATION_TYPE_DEFAULT,
#else
	VA_TRANSLATION_TYPE_PAE,
	VA_TRANSLATION_TYPE_DEFAULT = VA_TRANSLATION_TYPE_PAE,
#endif

	VA_TRANSLATION_TYPE_MAX,
} VA_TRANSLATION_TYPE;
//---------------------------------------------------------------------

#pragma pack(push, 4)
typedef struct _BOOT_APPLICATION_PARAMETERS
{
	/* OFFSET 0x00 */ BOOT_APPLICATION_PARAMETERS_HEADER Header;
	/* OFFSET 0x10 */ ULONG MachineType;     // BOOT_APPLICATION_MACHINE_TYPE
	/* OFFSET 0x14 */ ULONG TranslationType; // VA_TRANSLATION_TYPE
	/* OFFSET 0x18 */ ULONGLONG ImageBase;
	/* OFFSET 0x20 */ ULONG ImageSize;
	/* OFFSET 0x24 */ ULONG AllocatedMemoryOffset;   // offset based current struct begin, ALLOCATED_MEMORY
	/* OFFSET 0x28 */ ULONG ApplicationEntryOffset;  // offset based current struct begin, BOOT_APPLICATION_TRANSITION_ENTRY
	/* OFFSET 0x2c */ ULONG BootDeviceOffset;        // offset based current struct begin, BOOT_ENVIRONMENT_DEVICE
	/* OFFSET 0x30 */ ULONG FirmwareDataOffset;      // offset based current struct begin, FIRMWARE_PARAMETERS
	/* OFFSET 0x34 */ ULONG ReturnArgumentOffset;    // offset based current struct begin, BOOT_APPLICATION_RETURN_ARGUMENTS
	/* OFFSET 0x28 */ ULONG Unknown;
} BOOT_APPLICATION_PARAMETERS, * PBOOT_APPLICATION_PARAMETERS;
#pragma pack(pop)

C_ASSERT(sizeof(BOOT_APPLICATION_PARAMETERS) == 0x3c);

//---------------------------------------------------------------------

#define PAGE_ALLOCATION_MINIMUM_COUNT    (0x00000400)
#define HEAP_MINIMUM_SIZE                (0x00200000)

#define MEMORY_ATTRIBUTE_NONE            (0)
#define DEFAULT_DYNAMIC_DESCRIPTOR_COUNT (0)
//---------------------------------------------------------------------
#define LF_NO_DISPLAY_INITIALIZATION      ( 0x00000001) // confirmd
#define LF_REINITIALIZE                   ( 0x00000002) // confirmd
#define LF_BOOT_OPTIONS_INITIALIZED       ( 0x00000004) // confirmd
#define LF_8                              ( 0x00000008) // confirmd
#define LF_ZERO_SENSITIVE_DATA            ( 0x00000010) // confirmd
#define LF_INITIALIZED                    ( 0x00000020) // confirmd
#define LF_BD_FORCE_DEBUG                 ( 0x00000040) // value confirmd

#define LF_DEBUG_TRANSPORT                ( 0x00000200) // value confirmd
#define LF_BD_DISABLED_GRAPHICS           ( 0x00000800) // value confirmd

#define LF_SVN_CHAIN_CHECK_SELF           ( 0x00002000) // confirmd
#define LF_SVN_CHAIN_CHECK_CHILDREN       ( 0x00004000) // confirmd
#define LF_NO_TCG_LOG_WRITETHROUGH        ( 0x00008000) //


//---------------------------------------------------------------------

// between bootmgr/bootmgfw and winload,memtest ...
typedef struct _LIBRARY_PARAMETERS
{
	/* offset 0x00 */ ULONG Flags; //LF_*
	/* offset 0x04 */ ULONG TranslationType; //VA_TRANSLATION_TYPE
	/* offset 0x08 */ ULONG MinimumPageAllocation;
	/* offset 0x0c */ ULONG MinimumHeapSize;
	/* offset 0x10 */ ULONG HeapAllocationAttributes;
	/* offset 0x14 */ PWSTR ApplicationBaseDirectory;
	/* offset 0x18 */ ULONG NumberOfDynamicDescriptors;
	/* offset 0x1c */ PWSTR FontDirectory;
} LIBRARY_PARAMETERS, * PLIBRARY_PARAMETERS;


