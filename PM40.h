#pragma once
#include <SetupAPI.h>
#include <winioctl.h>

#define MY_ULONGLONG_MAX    0xFFFFFFFFFFFFFFFF
#include "..\DMADriver\Include\DmaDriverIoctl.h"
#include "..\DMADriver\TestCli\DmaDriverDLL\DmaDriverDll.h"

#define MY_ULONG_MAX    2147483647

// Defines
#define NUMBER_PACKET_RECVS             32

#define DEFAULT_CARD_NUMBER             0
#define DEFAULT_DMA_ENGINE_OFFSET       0
#define DEFAULT_PACKET_MODE             TRUE
#define DEFAULT_BUFFER_SIZE             16777216
#define DEFAULT_MAX_PACKET_SIZE         32768
#define DEFAULT_ITERATIONS              -1
#define DEFAULT_SECONDS                 -1
#define DEFAULT_NUMBER_THREAD           2
#define DEFAULT_VERBOSE                 FALSE
#define DEFAULT_DEBUG                   FALSE
#define DEFAULT_QUIET                   FALSE
#define DEFAULT_SEED_PATTERN            0
#define DEFAULT_MEM_LENGTH              4
#define DEFAULT_DMA_LENGTH              8192
#define DEFAULT_ADDR_PACKET_DMA_MAX     131072
#define DEFAULT_STRESS_DMA_LENGTH       131072 // 524288
#define DEFAULT_MAX_CARD_OFFSET         131072 // 0x100000
#define DEFAULT_STOP_ON_ERROR           FALSE;
#define DEFAULT_RANDOMIZE               FALSE;

#define PACKET_GENERATOR_MAX_PACKET_SIZE    1048575

#define MAX_THREADS                     10
#define MAX_CMD_LENGTH                  20

#define PATTERN_INCREMENTING            0
#define PATTERN_SPECIFIED               1
#define PATTERN_RANDOM                  2

#define ERROR_DLL_CALL_FAILED           -1
#define ERROR_TRANSFER_COUNT_MISMATCH   -2
#define ERROR_TRANSFER_DATA_MISMATCH    -3
#define ERROR_NO_DATA_TRANSFER          -4
#define ERROR_BAD_PARAMETER             -10
#define ERROR_BAD_TEST_COMMAND          -11
#define ERROR_NO_BOARD_FOUND            -20
#define ERROR_MEMORY_ALLOC_FAILED       -30
#define ERROR_NO_DMA_ENGINE_FOUND       -40

enum CLI_DMA_DIRECTION {
	CLI_NO_DIRECTION,
	CLI_SYSTEM_TO_CARD,
	CLI_CARD_TO_SYSTEM,
};

// Defines for Stress tests
// Define params structs for Read and Writes.
#define READ_STRESS                     0
#define WRITE_STRESS                    1
#define NUM_STRESS_PARAMS               2

enum CLI_TEST_TYPE {
	PACKET_SEND,
	PACKET_RECV,
	PACKET_RECVS,
	PACKET_LOOP,
	PACKET_WRITE,
	PACKET_READ,
	PACKET_WR,
};

#define _NELEM(arr)                 ((int) (sizeof(arr) / sizeof(arr[0])))
#ifdef CONFIG_X86_64
#define _OFFSETOF(t,m)              ((UINT64) &((t *)0)->m)
#else
#define _OFFSETOF(t,m)              ((UINT32) &((t *)0)->m)
#endif

#ifdef WIN32    // Windows Environment
#define BEGIN_CRITICAL(x)       EnterCriticalSection(x);
#define END_CRITICAL(x)         LeaveCriticalSection(x);
#else           // Linux Environment
#define BEGIN_CRITICAL(x)       pthread_mutex_lock(x);
#define END_CRITICAL(x)         pthread_mutex_unlock(x);
#define ERROR_SEM_TIMEOUT       ETIMEDOUT
#endif          // WIndows | Linux

// Byte Ordering
static inline UINT32 swap_uint32(UINT32 val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return (val << 16) | (val >> 16);
}

static inline UINT64 swap_uint64(UINT64 val)
{
	val = ((val << 8) & 0xFF00FF00FF00FF00ULL) | ((val >> 8) & 0x00FF00FF00FF00FFULL);
	val = ((val << 16) & 0xFFFF0000FFFF0000ULL) | ((val >> 16) & 0x0000FFFF0000FFFFULL);
	return (val << 32) | (val >> 32);
}

typedef struct _PASS_STATS {
	UINT64  DriverTime;
	UINT64  HardwareTime;
	UINT64  BytesTransfered;
	UINT64  IntsPerSecond;
	UINT64  DPCsPerSecond;
	INT32   CPUUtil;
} PASS_STATS, *PPASS_STATS;

typedef struct _SUMMARY_STATS {
	UINT64  Samples;
	UINT64  DriverTimeTotal;
	UINT64  HardwareTimeTotal;
	UINT64  BytesTransferedTotal;
	UINT64  IntsPerSecondTotal;
	UINT64  DPCsPerSecondTotal;
	UINT32  CPUUtilTotal;
} SUMMARY_STATS, *PSUMMARY_STATS;


// Configuration paramters structure.
typedef struct _CFG_PARAMS {
	UINT64  UserInfo;
	UINT64  CardAddress;
	UINT64  BufferSize;
	UINT64  UserInfoSeedPattern;
	UINT32  DataSeedPattern;
	UINT32  MaxPacketSize;
	UINT32  Iterations;
	UINT32  Seconds;
	UINT32  MaxDMAAddress;
	INT32   Index;
	INT32   CmdArgc;
	const char ** CmdArgv;
	INT32   Type;
	INT32   cardNum;
	INT32   DMAEngineOffset;
	INT32   NumberThreads;
	INT32   DataPatternType;
	INT32   DataSeedSize;
	INT32   UserInfoPatternType;
	INT32   UserInfoSeedSize;
	INT32   iPass;
	INT32   UserIntTimeout;
	UINT8   DMA_BAR;
	BOOLEAN bVerbose;
	BOOLEAN bPacketMode;
	BOOLEAN bDebug;
	BOOLEAN bQuiet;
	BOOLEAN bRandomize;
	BOOLEAN bStopOnError;
	BOOLEAN bPatternSpecified;
	BOOLEAN bUserInfoPatternSpecified;
	BOOLEAN bFIFO;
	BOOLEAN bSecondRefresh;
	BOOLEAN bBlockingAPIs;
	BOOLEAN bAppendData;
	BOOLEAN bRunning;
	enum CLI_TEST_TYPE  TestType;
	INT32               StatsMultiplier;
	SUMMARY_STATS       SummaryStats;
#ifdef WIN32
	CRITICAL_SECTION CriticalSection;
#else
	pthread_mutex_t  CriticalSection;
#endif // Windows only
} CFG_PARAMS, *PCFG_PARAMS;

typedef INT32(*func_t)(PCFG_PARAMS pcfgParams);

#define TYPE_HELP               0
#define TYPE_GEN                1
#define TYPE_BLOCK              2
#define TYPE_PACKET             3
#define TYPE_ENHANCED_PACKET    4

typedef struct
{
	const char *    cmd;
	func_t          func;
	INT32           type;
	const char *    help;
} command_t;

#define READ_BUFFER_SIGNATURE   0xAA550101
#define WRITE_BUFFER_SIGNATURE  0xF7FE7FFF

// Buffer structure contains information for the test buffers
typedef struct _BUFFER_DATA_STRUCT {
	UINT32  Signature;
	UINT32  ThreadID;
	UINT32  SequenceNumber;
	UINT32  UserDataLen;
	UINT64  CardAddress;
	UINT64  UserInfo;
	UINT8   data[1];
} BUFFER_DATA_STRUCT, *PBUFFER_DATA_STRUCT;

typedef struct _THREAD_PARAMS {
	PCFG_PARAMS pcfgParams;
	PUINT8      Buffer;
	UINT32      MaxPacketSize;
	UINT64      TransferLength;
	size_t      AllocSize;
	BOOLEAN     bBlockingAPIs;
#ifdef WIN32
	HANDLE      hThreadHandle;
#else    // Linux
	pthread_t   hThreadHandle;
#endif   // Windows | Linux thread handle conditional compile.
	UINT32      PacketCount;
	UINT32      Iterations;
	UINT32      Direction;
	UINT64      CardOffset;
	UINT64      UserInfo;
	UINT32      MaxDMAAddress;
	INT32       iPass;
	BOOLEAN     Running;
	UINT64      TotalBytesTransferred;
	// Items for Packet Receives mode only
	UINT32      DMAOverrunCount;
	UINT32      DMADescErrorCount;
	UINT32      ZeroPacketCount;
	UINT32      FullPacketCount;
	UINT32      PartialPacketCount;

	INT32       cardNum;
	INT32       DMAEngineOffset;

	INT32       DataSeedPattern;
	INT32       DataPatternType;
	INT32       DataSeedSize;

	INT64       UserInfoSeedPattern;
	INT32       UserInfoPatternType;
	INT32       UserInfoSeedSize;

	BOOLEAN     bDebug;
	BOOLEAN     bQuiet;
	BOOLEAN     bRandomize;
	BOOLEAN     bStopOnError;
	BOOLEAN     bPatternSpecified;
	BOOLEAN     bUserInfoPatternSpecified;
	BOOLEAN     bFIFO;
	BOOLEAN     bAppendData;
#ifdef  WIN32
	CRITICAL_SECTION CriticalSection;
#else   // Linux
	pthread_mutex_t  CriticalSection;
#endif  // Windows | Linux Critical Section definition.
} THREAD_PARAMS, *PTHREAD_PARAMS;

typedef struct _STRESS_THREAD_PARAMS {
	THREAD_PARAMS       WriteParams;
	THREAD_PARAMS       ReadParams;
} STRESS_THREAD_PARAMS, *PSTRESS_THREAD_PARAMS;

typedef struct _WAIT_THREAD_PARAMS {
#ifdef WIN32
	HANDLE              hThreadHandle;
#else    // Linux
	pthread_t           hThreadHandle;
#endif   // Windows | Linux thread handle conditional compile.
	PCFG_PARAMS         pcfgParams;
} WAIT_THREAD_PARAMS, *PWAIT_THREAD_PARAMS;

#define COMMON_DMA_CTRL_IRQ_ENABLE              0x00000001
#define COMMON_DMA_CTRL_IRQ_ACTIVE              0x00000002

#define PACKET_DMA_CTRL_DESC_COMPLETE           0x00000004
#define PACKET_DMA_CTRL_DESC_ALIGN_ERROR        0x00000008
#define PACKET_DMA_CTRL_DESC_FETCH_ERROR        0x00000010
#define PACKET_DMA_CTRL_DESC_CHAIN_END          0x00000080
#define PACKET_DMA_CTRL_DMA_ENABLE              0x00000100
#define PACKET_DMA_CTRL_DMA_RUNNING             0x00000400
#define PACKET_DMA_CTRL_DMA_WAITING             0x00000800
#define PACKET_DMA_CTRL_DMA_WAITING_PERSIST     0x00001000
#define PACKET_DMA_CTRL_DMA_RESET_REQUEST       0x00004000
#define PACKET_DMA_CTRL_DMA_RESET               0x00008000

typedef struct _DMA_ENGINE_STRUCT
{
	UINT32      Capabilities;               // Common Capabilities
	UINT32      ControlStatus;              // Common Control and Status
} DMA_ENGINE_STRUCT, *PDMA_ENGINE_STRUCT;

//! Packet Generator Control DWORD Defines
#define PACKET_GEN_ENABLE                       0x00000001
#define PACKET_GEN_LOOPBACK_ENABLE              0x00000002
#define PACKET_GEN_LOOPBACK_USE_RAM             0x00000004
#define PACKET_GEN_TABLE_ENTRIES_MASK           0x00000030
#define PACKET_GEN_DATA_PATTERN_MASK            0x00000700
#define PACKET_GEN_DATA_PATTERN_CONSTANT        0x00000000
#define PACKET_GEN_DATA_PATTERN_INC_BYTE        0x00000100
#define PACKET_GEN_DATA_PATTERN_LFSR            0x00000200
#define PACKET_GEN_DATA_PATTERN_INC_DWORD       0x00000300
#define PACKET_GEN_CONT_DATA_PATTERN            0x00000800
#define PACKET_GEN_USER_PATTERN_MASK            0x00007000
#define PACKET_GEN_USER_PATTERN_CONSTANT        0x00000000
#define PACKET_GEN_USER_PATTERN_INC_BYTE        0x00001000
#define PACKET_GEN_USER_PATTERN_LFSR            0x00002000
#define PACKET_GEN_USER_PATTERN_INC_DWORD       0x00003000
#define PACKET_GEN_CONT_USER_PATTERN            0x00008000
#define PACKET_GEN_ACTIVE_CLOCK_MASK            0x00FF0000
#define PACKET_GEN_INACTIVE_CLOCK_MASK          0xFF000000

/*!
* \struct GENERATOR_CHECKER_CONTROL
* \brief Control for the traffic Generator.
*/
typedef struct _GENERATOR_CHECKER_CONTROL
{
	UINT32  Control;            // Generator/Checker Control DWORD
} PACKED GENERATOR_CHECKER_CONTROL, *PGENERATOR_CHECKER_CONTROL;

/*!
* \struct GENERATOR_CHECKER_PARAMS
* \brief Parameters for the traffic Generator.
*/
typedef struct _GENERATOR_CHECKER_PARAMS
{
	UINT32  NumPackets;         // Count of packet to generate, 0 = infinite
	UINT32  DataSeed;           // Data Seed pattern
	UINT32  UserCtrlStatSeed;   // Seed for the User Control/Status fields
	UINT32  reserved[4];        // reserved
	UINT32  PacketLength[4];    // Packet Length array
} PACKED GENERATOR_CHECKER_PARAMS, *PGENERATOR_CHECKER_PARAMS;

#define PACKET_GENERATOR_ENGINE_OFFSET          0x0000a000
#define PACKET_GENERATOR_BLOCK_SIZE             0x00000100
#define CHECKER_ERROR_OFFSET                    0x00000010
#define CHECKER_CLEAR_ERROR_CMD                 0x00000080

#define CHECKER_ERROR_SOP                       0x00000001
#define CHECKER_ERROR_EOP                       0x00000002
#define CHECKER_ERROR_CPL                       0x00000004
#define CHECKER_ERROR_DATA                      0x00000008
#define CHECKER_ERROR_BAD_DATA                  0x00000010
#define CHECKER_ERROR_USR_CTL                   0x00000020

#define CHECKER_ERROR_CNTR_MASK                 0xFFFFFF00

typedef struct _PACKET_GENCHK_STRUCT
{
	UINT32  Control;            // Packet Generator Control DWORD
	UINT32  NumPackets;         // Count of packet to generate, 0 = infinite
	UINT32  DataSeed;           // Data Seed pattern
	UINT32  UserCtrlStatSeed;   // Seed for the User Control/Status fields
	UINT32  PacketLength[4];    // Packet Length array
} PACKET_GENCHK_STRUCT, *PPACKET_GENCHK_STRUCT;

#define MAX_NUM_DMA_ENGINES 64

typedef struct _PM40_INFO_STRUCT
{
	INT8        PacketSendEngineCount;
	INT8        PacketRecvEngineCount;
	INT8        PacketSendEngine[MAX_NUM_DMA_ENGINES];
	INT8        PacketRecvEngine[MAX_NUM_DMA_ENGINES];
	INT8        DLLMajorVersion;
	INT8        DLLMinorVersion;
	INT8        DLLSubMinorVersion;
	INT8        DLLBuildNumberVersion;
	INT8        AddressablePacketMode;
	INT8        DMARegistersBAR;
} PM40_INFO_STRUCT, *PPM40_INFO_STRUCT;

#define S2C_DIRECTION       TRUE
#define C2S_DIRECTION       FALSE

/*! \struct STAT_STRUCT
 *
 * \brief Status Structure
 *  Status Information from an IOCTL transaction
 */
//typedef struct _STAT_STRUCT
//{
//	UINT64   CompletedByteCount;        // Number of bytes transfered
//} STAT_STRUCT, *PSTAT_STRUCT;

class CPM40
{
protected:
	int		m_nTimedOutCount = 0;
	int		m_nConversionCount = 0;
	double m_dArmedAt = 0.0;
	double m_dConversionCompletedAt= 0.0;

public:

	CPM40();
	~CPM40();
	bool FindDevice();
	BOOL IsConnected();
	void CloseDevice();

	CString & getDllVersion();

	CString & getDriverVersion();

	CString & getAllInterruptCounts();

	void StartADCCommsToBoard();
	void StartWaitThreads(PCFG_PARAMS pcfgParams, PWAIT_THREAD_PARAMS pWaitParams);
	void FinishWaitThreads();

	CString & getLastErrorMessage();

	bool IsCardFound();

	int ReadBar0(UINT64 n64CardAddrs);

	bool WriteBar0(UINT64 n64CardAddrs, int nValue);

	int getDMACap();

	bool IsDMAEnginePresent();

	CString * getDMADirection();

	CString * getDMAEngineType();

	CString * getDMAEngineNumber();

	CString * getDMAMemorySize();

	void ReadDMA();

	void EnableMailbox();

	int MailboxTx(int nValue, int * pnCheckSum);
	void ConfigureWidthDelay(TimeSlotData *pTS, int nBoardNumber);
	void ConfigureADC();
	void ArmADC();
	bool IsConversionComplete(int * nTS = NULL);
	bool IsMyIrq(int nMask);
	void ClearMyIrq(int nMask);
	int getConversionCount();
	double getElapsedTime();
	double getElapsedSinceLastArmed();
	int IncrementTimedOutCount();
	int getTimedOutCount();
	VOID ReadTrace(PTHREAD_PARAMS params, int nSlot);
	BOOL RegisterInterruptCallback();
	void DeRegisterInterruptCallback();
	int m_nMaxMailboxWait = 0;
	int m_nMailboxFailed = 0;
	unsigned int m_nTotalCommandsSent = 0;
	double m_dElapsedTime = 0.0;

	int			m_nEventsCount;
	bool		m_bIntCallbackThreadFinished;
	bool		m_bIntCallbackThreadEnabled;
	HANDLE		m_hEvent;
	CWinThread*	m_pThread;



/*
private:
	UINT32			BufferSize;
	UINT32			MaxPacketSize;
	UINT32			BoardNumber;
	BOOLEAN			AttachedToDriver;
	HANDLE			hDevice;
	HDEVINFO		hDevInfo;
	PM40_INFO_STRUCT	PM40Info;
	PSP_DEVICE_INTERFACE_DETAIL_DATA pDeviceInterfaceDetailData;
	UINT64			pRxPacketBufferHandle;
	*/

protected:
	PCFG_PARAMS			m_pcfgParams;
	DMA_INFO_STRUCT     m_DmaInfo;
	DMA_CAP_STRUCT      m_DMACap;
	INT32               m_CLIArgc;
	const char **       m_CLIArgs;
	UINT32              m_ulCmdValue;
	INT32               m_status = 0;
	bool	            m_bFoundPCIBoard = FALSE;
	CString				m_ErrorMsg;
	int					m_nError;
	WAIT_THREAD_PARAMS  m_WaitParams;
	BOARD_CONFIG_STRUCT m_BoardConfig;
};

extern void WaitThreadProc(PWAIT_THREAD_PARAMS pWaitParams);
