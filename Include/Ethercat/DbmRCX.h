/*************************************************************************************/
/*!
 \file DbmRCX.h 
  This file will contain all definitions which are used also by the RCX DBM
  functions.<br>
  All structures with the prefix <span class="hint">DBMRCS_</span> are defintions
  for old RCS like databases.<br>
  All structures with the prefix <span class="hint">DBMRCX_</span> are defintions
  for new RCX like databases.<br>
  \li \c any OS         : yes
  \li \c Unicode        : yes but not tested

  ======================== Copyright =====================================
  \par License: Copyright &copy; 2009  Hilscher GmbH

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3.0 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
   
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA

  ========================================================================

  \manonly
  START of the SourceSafer tags
   Filename:
   $Workfile: DbmRCX.h $
   Last Modification:
    $Author: Harald $
    $Modtime: 7.07.09 13:54 $
    $Revision: 4 $
  END of the SourceSafer tags
  \endmanonly

  \section DbmRCX_h_history History:
  \version see <a href=".\OwnHtml\version.html" title="Versions">Versions</a>
*/
#ifndef _DBMRCX_H_
#define _DBMRCX_H_

/*---------------------------------------------------------------------------*/
/* Compiler settings */
/* pragma pack */
#ifdef PRAGMA_PACK_ENABLE
#pragma PRAGMA_PACK_1(MID_DBM)
#endif
/*---------------------------------------------------------------------------*/

/* ************************************************************************ */
/* START of global defines                                                  */
/* ************************************************************************ */
#define RCSMAXNAMELENGTH        18
#define RCSMAXTOGGLENAMELENGTH  32
#define MAXOLDDBMTABLES         100
#define MAXRCSTABLES            32
#define HEADERTEXTV1_1991       "DBM V1.0 1991" //must be a char string not a UNICODE string
#define DWVERSION1000           1000
#define HEADERTEXTV2_1992       "DBM V2.0 1992" //must be a char string not a UNICODE string
#define DWVERSION2000           2000
#define HEADERTEXTV203_1993     "DBM V2.03 1993" //must be a char string not a UNICODE string
#define DWVERSION2030           2030
#define HEADERTEXTV21_1993      "DBM V2.10 1993" //must be a char string not a UNICODE string
#define DWVERSION2100           2100
#define DBMOLDSNAP              "DBMFILE  " //must be a char string not a UNICODE string
#define DBMCOMPANYSNAP          "HILSCHER" //must be a char string not a UNICODE string

#ifndef SUCCEEDED
#define SUCCEEDED(hr) (hr==RX_OK)
#endif
#ifndef _countof
#define _countof(a) (sizeof(a)/sizeof(a[0]))
#endif //_countof
#ifndef ExSizeof
#define ExSizeof(struct,member)   (size_t)( sizeof(((struct*)0)->member) )
#endif //ExSizeof
#ifndef offsetof
//The following is a copy from stddef.h
#ifdef  _WIN64
#define offsetof(s,m)   (size_t)( (ptrdiff_t)&(((s *)0)->m) )
#else
#define offsetof(s,m)   (size_t)&(((s *)0)->m)
#endif
#endif //offsetof
/* ************************************************************************ */
/* END of global defines                                                    */
/* ************************************************************************ */

/* ************************************************************************ */
/* START of structure definitions                                           */
/* ************************************************************************ */
/* ******************************************************************** */
/* START RCS definitions                                                      */
/* ******************************************************************** */
/**
  union to access an 8 Bit value
*/
typedef union DBM_8BIT_SUtag {
  char            m_ch;
  unsigned char   m_b;
}DBM_8BIT_SU, *PDBM_8BIT_SU;
/**
  union to access an 16 Bit value
*/
typedef union DBM_16BIT_SUtag {
  short           m_s;
  unsigned short  m_us;
}DBM_16BIT_SU, *PDBM_16BIT_SU;
/**
  union to access an 32 Bit value
*/
typedef union DBM_32BIT_SUtag {
  long          m_l;
  unsigned long m_dw;
}DBM_32BIT_SU, *PDBM_32BIT_SU;

/**
\todo describe this structure
*/
typedef struct DBM_OStag {
  unsigned short  m_wLength;  ///< length prefix 
  unsigned char   m_abData[1]; ///< data (byte stream) with length m_wLength
} DBM_OS, *PDBM_OS;
/**
\todo describe this structure
*/
typedef struct DBM_BLOBtag {
  unsigned long   m_dwLength;  ///< length prefix 
  union {
    unsigned char   m_abData[1]; ///< data (byte stream) with length m_wLength
    char            m_acData[1]; ///< data (byte stream) with length m_wLength
  } u;
} DBM_BLOB, *PDBM_BLOB;
#define DBM_IP_DATA_LENGTH 4
/** 
\todo describe this structure
*/
typedef union DBM_IPtag {
  unsigned char     m_abData[DBM_IP_DATA_LENGTH]; ///< bytewise access
  unsigned short    m_wAdr[DBM_IP_DATA_LENGTH/2];///< wordwise access
  unsigned long     m_dwAdr;  ///< 32bit wise access
} DBM_IP, *PDBM_IP;
#define DBM_UUID_DATA1_LENGTH 6
#define DBM_UUID_DATA2_LENGTH 2
#define DBM_UUID_DATA3_LENGTH 2
#define DBM_UUID_DATA4_LENGTH 2
#define DBM_UUID_DATA5_LENGTH 4
/** 
\todo describe this structure
*/
typedef struct DBM_UUIDtag {
  unsigned char    m_abData5[DBM_UUID_DATA5_LENGTH];   ///< UUID5 this is the leftmost value in the string
  unsigned char    m_abData4[DBM_UUID_DATA4_LENGTH];   ///< UUID4
  unsigned char    m_abData3[DBM_UUID_DATA3_LENGTH];   ///< UUID3
  unsigned char    m_abData2[DBM_UUID_DATA2_LENGTH];   ///< UUID2
  unsigned char    m_abData1[DBM_UUID_DATA1_LENGTH];   ///< UUID1 this is the rightmost value in the string
} DBM_UUID, *PDBM_UUID;
#define DBM_MAC_DATA_LENGTH 6
/** 
\todo describe this structure
*/
typedef struct DBM_MACtag {
  unsigned char    m_abData[DBM_MAC_DATA_LENGTH]; ///< MAC Element1
} DBM_MAC, *PDBM_MAC;

/** 
 This Structure is used as a Header of all Data in the DBM file.
 This means that there is no Table or Record or any other data except the
 Segment Header (DBMRCS_SEGMENT_HEADERtag) which is not preceded
 by this structure.
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCS_STD_STRUCTtag {
  unsigned char   m_bProperty;      ///< see eDbmStdStructureProperty
  unsigned char   m_abReserved[3];  ///< Reserved initialized with 0
  unsigned short  m_wSegmentOffset; ///< For a Record this member is the offset to the beginning of the 
  ///< <span class="red">segment</span> where the offset for the table which belongs to this record
  ///< is stored. Calculated as <span class="green">
  ///< (sizeof(DBMRCS_SEGMENT_HEADERtag)/2 + TableNo*sizeof(DBMRCS_TABLE_INDEXtag)/2)</span>.
  ///< For a Table this member is set to 0
  unsigned short  m_wTableOffset; ///< For a Record this member is the offset in the table
  ///< where the offset for this record is stored. Calculated as <span class="green">
  ///< (sizeof(DBMRCS_STD_STRUCTtag)/2 + RecNo)</span>.
  ///< For a Table this member is set to 0
  unsigned char   m_bOdd; ///< != 0 when the last byte of the following data is a dummy byte.
  ///< This is necessary when the data length of the data which belongs to this Header 
  ///< is not an even number.
  unsigned char   m_bRes; ///< reserved do not interpret nor use
  unsigned short  m_wByteLength;    ///< The length of this structure (<span class="green">12 Bytes</span>) + 
  ///< the following data length in Bytes
}DBMRCS_STD_STRUCT, *PDBMRCS_STD_STRUCT;

/** 
 Structure to store the Offsets of the Records of one Table
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCS_TABLEtag {
  DBMRCS_STD_STRUCT    m_tStd;            ///< see DBMRCS_STD_STRUCTtag
  unsigned short    m_awRecordOffset[1];  ///< Offset of the Record in the file or RAM area.
  ///< The Seek Method of the IStream Interface can be used to get the beginning
  ///< of the DBMRCS_STD_STRUCTtag Header of the related Record.
}DBMRCS_TABLE, *PDBMRCS_TABLE;

/** 
 Structure to store the Records sorted.
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCS_INDEXtag {
  DBMRCS_STD_STRUCT    m_tStd;          ///< see DBMRCS_STD_STRUCTtag
  unsigned short    m_wRecordNumber[1]; ///< Number of the Record in the sorted order.
}DBMRCS_INDEX, *PDBMRCS_INDEX;

/**
 Offset Pair for each Table in this Database to address the 
 beginning of the Record and the Index directory of each Table.
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCS_TABLE_INDEXtag {
  unsigned short m_wTable;  ///< Offset to the beginning of the Table description (DBMRCS_TABLEtag) structure.
  unsigned short m_wIndex;  ///< Offset to the beginning of the Index description (DBMRCS_INDEXtag) structure.
  ///< Can be 0 when the Records are not sorted.
}DBMRCS_TABLE_INDEX, *PDBMRCS_TABLE_INDEX;

/**
 This structure is used to interpret old DBM files.
 Therefore the structure must be compatible with this old
 versions of files.
 Immediately after this structure the structure DBMRCS_TABLE_INDEXtag
 is following m_wCountOfTable times.
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCS_SEGMENT_HEADERtag {
  unsigned short  m_wChecksum;        ///< Checksum as wordwise addition of all data in the file.
  unsigned short  m_wStartTable;       ///< Number of the first Table in this Database.
  unsigned short  m_wCountOfTable;     ///< Maximum Number of Tables in this Database.
  ///< This structure is followed by the DBMRCS_TABLE_INDEXtag[m_wCountOfTable] structures
  ///< to address the beginning of each Table.
  unsigned short  m_wOpenTable;        ///< Count of actual stored Tables in this Database.
  unsigned short  m_wMaxIndex;         ///< Maximum Word Index in this WordArray. This is equal
  ///< to the maximum length of the Database.
  unsigned short  m_wFirstGap;         ///< First Index which points to deleted informations
  ///< in this Database. UNSUPPORTED in the IDbmDatabase Interface.
  unsigned short  m_wFirstFreeIndex;   ///< First unused Word Index in the WordArray
  unsigned char   m_bTyp;              ///< Type of Data in this file. Must be 1 for a DBM file see also enum eDbmFileType
  unsigned char   m_fNeedCompression;  ///< != 0 when the Database needs compression.
  ///< UNSUPPORTED in the IDbmDatabase Interface.
}DBMRCS_SEGMENT_HEADER, *PDBMRCS_SEGMENT_HEADER;

/** 
 Structure to be used in DBMRCS_FILE_HEADERtag to address each Segment
 in this file.
 The IDbmDatabase Interface can only read files where only one Database
 Segment is stored.
 Immediately after the last of this structures the structure DBMRCS_SEGMENT_HEADERtag
 is following to start description of the first segment
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCS_SEG_LISTtag {
  unsigned short  m_wSegNo;
  unsigned short  m_wSegLength;
  unsigned long   m_dwOffset;
}DBMRCS_SEG_LIST, *PDBMRCS_SEG_LIST;
/** 
 Structure to be used in DBMRCS_FILE_HEADER_V1tag to address each Segment
 in this file.
 The IDbmDatabase Interface can only read files where only one Database
 Segment is stored.
 Immediately after the last of this structures the structure DBMRCS_SEGMENT_HEADER_V1tag
 is following to start description of the first segment
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCS_SEG_LIST_V1tag {
  unsigned short  m_wSegNo;
  unsigned short  m_wSegLength;
  unsigned short  m_wOffset;
}DBMRCS_SEG_LIST_V1, *PDBMRCS_SEG_LIST_V1;

/**
  \enum eDbmTableProperty
 Enumerator for 
*/
enum eDbmTableProperty
    {	eDbmTablePropertyNothing	= 0,
	eDbmTablePropertyStrict	= 1,
	eDbmTablePropertyReserved2	= 2,
	eDbmTablePropertyReserved4	= 4
    } ;

/**
  \enum eDbmTablesMask
 Enumerator for 
*/
enum eDbmTablesMask
    {	eDbmTablesMaskNothing	= 0,
	eDbmTablesMaskAll	= eDbmTablesMaskNothing + 1,
	eDbmTablesMaskDiagTables	= eDbmTablesMaskAll + 1
    } ;

/**
  \enum eDbmDataType
 Enumerator for 
*/
enum eDbmDataType
    {	eDbmDataTypeToggle	= 0,
	eDbmDataType8Bit	= 0x10,
	eDbmDataType16Bit	= 0x20,
	eDbmDataTypeAscii	= 0x30,
	eDbmDataType32Bit	= 0x40,
	eDbmDataTypeFloat	= 0x50,
	eDbmDataTypeUUID	= 0x60,
	eDbmDataTypeMACAddress	= 0x70,
	eDbmDataTypeCompressed8Bit	= 0x80,
	eDbmDataTypeUnicode	= 0x90,
	eDbmDataTypeIpAddress	= 0xa0,
	eDbmDataTypeAssignedToggle	= 0xb0,
	eDbmDataTypeOctetString	= 0xc0,
	eDbmDataTypeCompressed16Bit	= 0xd0,
	eDbmDataTypeCompressed32Bit	= 0xe0,
	eDbmDataTypeHelpText	= 0xf0,
	eDbmLastRcsCompatibleType	= 0xf0,
	eDbmDataTypeMD5	= 0x100,
	eDbmDataTypeFOREIGNKEY	= 0x400,
	eDbmDataTypeBlob	= 0x800,
	eDbmDataTypeRECURSIVEKEY	= 0x1000,
	eDbmDataTypeUnstructured	= 0x4000,
	eDbmDataTypeArray	= 0x8000
    } ;

/**
  \enum eDbmAccess
 Enumerator for 
*/
enum eDbmAccess
    {	eDbmAccessNothing	= 0,
	eDbmAccessRW	= 1,
	eDbmAccessRO	= 2,
	eDbmAccessHidden	= 3
    } ;

/**
  \enum eDbmSave
 Enumerator for 
*/
enum eDbmSave
    {	eDbmSaveNothing	= 0,
	eDbmSaveIncludeRCXHeader	= eDbmSaveNothing + 1,
	eDbmSaveIncludeRCSHeader	= eDbmSaveIncludeRCXHeader + 1,
	eDbmSaveExcludeRCSHeader	= eDbmSaveIncludeRCSHeader + 1,
	eDbmSaveIncludeRCXV3Header	= eDbmSaveExcludeRCSHeader + 1,
	eWithoutStructure	= 0x100,
	eDbmSaveIncludeRCXHeaderWithoutStructure	= eWithoutStructure | eDbmSaveIncludeRCXHeader,
	eDbmSaveIncludeRCSHeaderWithoutStructure	= eWithoutStructure | eDbmSaveIncludeRCSHeader,
	eDbmSaveExcludeRCSHeaderWithoutStructure	= eWithoutStructure | eDbmSaveExcludeRCSHeader,
	eDbmSaveIncludeRCXV3HeaderWithoutStructure	= eWithoutStructure | eDbmSaveIncludeRCXV3Header
    } ;

/**
  \enum eDbmRcxViewMode
 Enumerator for 
*/
enum eDbmRcxViewMode
    {	eDbmViewModeNothing	= 0,
	eDbmViewModeUnsigned	= 0x1,
	eDbmViewModeHex	= 0x2,
	eDbmViewModeSigned	= 0x4,
	eDbmViewModeAscii	= 0x8,
	eDbmViewModeHexDump	= 0x10,
	eDbmViewModeBinary	= 0x20
    } ;

/**
  \enum eDbmSetElementPropertyValidMask
 Enumerator for 
*/
enum eDbmSetElementPropertyValidMask
    {	eDbmSetPropertyNone	= 0,
	eDbmConstDescription	= 0x1,
	eDbmConstViewMode	= 0x2,
	eDbmConstAccess	= 0x4,
	eDbmConstDataType	= 0x8,
	eDbmConstAdditionalStrings	= 0x10,
	eDbmConstAdditionalNumber	= 0x20,
	eDbmConstMinValue	= 0x40,
	eDbmConstMaxValue	= 0x80,
	eDbmConstDefaultValue	= 0x100,
	eSetElementPropertyAll	= 0xffff
    } ;

/**
  \enum eDbmSetTablePropertyValidMask
 Enumerator for 
*/
enum eDbmSetTablePropertyValidMask
    {	eConstName	= 0x1,
	eConstProperty	= 0x2
    } ;

/**
  \enum eDbmNewRecordFlag
 Enumerator for 
*/
enum eDbmNewRecordFlag
    {	eAppend	= 0x1,
	eInsertAt	= 0x2,
	eSearchFreeNumber	= 0x3
    } ;


/**
  \enum eDbmStdStructureProperty 
 Enumerator for use in DBMRCS_STD_STRUCTtag m_dwProperty
*/
enum eDbmStdStructureProperty {
  eDBMRCS_STD_PROPERTY_REC      = 0x00
  ,eDBMRCS_STD_PROPERTY_REC_NDIR = 0x01
  ,eDBMRCS_STD_PROPERTY_REC_IDIR = 0x02
  ,eDBMRCS_STD_PROPERTY_REC_0    = 0x04
  ,eDBMRCS_STD_PROPERTY_DELETED  = 0x80
};

/** 
 Structure for the special record with no. 0 which is used
 to store table related informations.
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCS_REC0tag {
  DBMRCS_STD_STRUCT    m_tStd;          ///< see DBMRCS_STD_STRUCTtag
  unsigned short    m_wSortIndex;       ///< Used when the records are sorted
  unsigned short    m_wLastUsedRecNo;   ///< This is the last used (greatest) record 
  ///< number in this Table. But this will not mean that there are as many records 
  ///< as this number gives in the Table.
  unsigned short    m_wMaxRecNo;        ///< This Table can store this maximum of records.
  unsigned short    m_wGapIndex;        ///< Index in DBMRCS_TABLEtag m_awRecordOffset where no
  ///< record offset exist
  unsigned char     m_bTableProperties; ///< see also eDbmTableProperty
  unsigned char     m_bRes;             ///< Reserved
  unsigned short    m_wActualEntrys;    ///< Count of valid record entrys in DBMRCS_TABLEtag m_awRecordOffset
  unsigned short    m_wActualSize;      ///< Actual size of DBMRCS_TABLEtag m_awRecordOffset
  char              m_achTableName[RCSMAXNAMELENGTH];  ///< Name of the Table as '\\0' terminated string
}DBMRCS_REC0, *PDBMRCS_REC0;

/** 
 The structure to store one Record.
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCS_RECtag {
  DBMRCS_STD_STRUCT  m_tStd;   ///< see DBMRCS_STD_STRUCTtag
  unsigned char   m_abData[1]; ///< Data for the record as BYTE Stream.
}DBMRCS_REC, *PDBMRCS_REC;

/** 
  An Toggle Element has in his definition after the Description
  Text only an additional \\0 terminated string which contains
  a comma serperated list of all Toggle Strings.
  An empty string between 2 commas is allowed to specify
  an unused Toggle String (Value).
  The first String in the List is assigned to the Value 0 and each Toggle
  String will increase the value by 1 (even an empty Toggle String)
*/
  typedef __PACKED_PRE struct __PACKED_POST DBMRCS_TOGGLE_DEFtag {
    char szToggleStrings[1]; ///< The Toggle Strings are separated by a comma
    ///< \code "Toggle_1,Toggle_2,,,,Toggle_n" will result in the assigned Values
    ///< Toggle_1 = 0; Toggle_2 = 1 and Toggle_n = 5
    ///< \endcode
  }DBMRCS_TOGGLE_DEF, *PDBMRCS_TOGGLE_DEF;
/** 
  stuct to access an 8 Bit value with it's view mode
*/
  typedef __PACKED_PRE struct __PACKED_POST DBMRCS_8BIT_DEFtag {
    unsigned char   m_bViewMode;
    DBM_8BIT_SU     m_tMin;
    DBM_8BIT_SU     m_tMax;
  }DBMRCS_8BIT_DEF, *PDBMRCS_8BIT_DEF;
/** 
  stuct to access an 16 Bit value with it's view mode
*/
  typedef __PACKED_PRE struct __PACKED_POST DBMRCS_16BIT_DEFtag  {
    unsigned char m_bViewMode;
    DBM_16BIT_SU  m_tMin;
    DBM_16BIT_SU  m_tMax;
  }DBMRCS_16BIT_DEF, *PDBMRCS_16BIT_DEF;
/** 
  stuct to access an 32 Bit value
*/
  typedef __PACKED_PRE struct __PACKED_POST DBMRCS_32BIT_DEFtag  {
    DBM_32BIT_SU  m_tMin;
    DBM_32BIT_SU  m_tMax;
  }DBMRCS_32BIT_DEF, *PDBMRCS_32BIT_DEF;
/** 
  Definition of an float element
*/
  typedef __PACKED_PRE struct __PACKED_POST DBMRCS_FLOAT_DEFtag {
    float m_fltMin;
    float m_fltMax;
  }DBMRCS_FLOAT_DEF, *PDBMRCS_FLOAT_DEF;
/** 
  Definition of an compressed byte element
*/
  typedef __PACKED_PRE struct __PACKED_POST DBMRCS_CB_DEFtag {
    char m_szBitText[1]; ///< The Text for each Bit are separated by a comma
  }DBMRCS_CB_DEF, *PDBMRCS_CB_DEF;
/** 
  Definition of an compressed word element (extended format)
*/
  typedef __PACKED_PRE struct __PACKED_POST DBMRCS_CB16EX_DEFtag {
    unsigned char m_bViewMode;
    DBM_16BIT_SU  m_16Min;
    DBM_16BIT_SU  m_16Max;
    DBM_16BIT_SU  m_16Def;
  }DBMRCS_CB16EX_DEF, *PDBMRCS_CB16EX_DEF;
/** 
  Definition of an compressed word element
*/
  typedef __PACKED_PRE struct __PACKED_POST DBMRCS_CB16_DEFtag {
    char        m_szBitText[1]; ///< The Text for each Bit are separated by a comma
    //After the text the structure DBMRCS_CB16EX_DEFtag is following  
  }DBMRCS_CB16_DEF, *PDBMRCS_CB16_DEF;
/** 
  Definition of an compressed unsigned long element (extended format)
*/
  typedef __PACKED_PRE struct __PACKED_POST DBMRCS_CB32EX_DEFtag {
    unsigned char m_bViewMode;
    DBM_32BIT_SU  m_32Min;
    DBM_32BIT_SU  m_32Max;
    DBM_32BIT_SU  m_32Def;
  }DBMRCS_CB32EX_DEF, *PDBMRCS_CB32EX_DEF;
/** 
  Definition of an compressed unsigned long element
*/
  typedef __PACKED_PRE struct __PACKED_POST DBMRCS_CB32_DEFtag     {
    char        m_szBitText[1]; ///< The Text for each Bit are separated by a comma
    //After the text the structure DBMRCS_CB32EX_DEFtag is following  
  }DBMRCS_CB32_DEF, *PDBMRCS_CB32_DEF;
/** 
  Definition of an Octet String element
*/
  typedef __PACKED_PRE struct __PACKED_POST DBMRCS_OCTET_STRING_DEFtag {
    unsigned char  m_bViewMode;
    unsigned short m_wMin;
    unsigned short m_wMax;
  }DBMRCS_OCTET_STRING_DEF, *PDBMRCS_OCTET_STRING_DEF;
/** 
  Definition of an IP Address element
*/
  typedef __PACKED_PRE struct __PACKED_POST DBMRCS_IPADR_DEFtag   {
    unsigned char m_bViewMode;
    DBM_IP        m_tMin;
    DBM_IP        m_tMax;
  }DBMRCS_IPADR_DEF, *PDBMRCS_IPADR_DEF;
/** 
  Definition of an MACADR element
*/
  typedef __PACKED_PRE struct __PACKED_POST DBMRCS_MACADR_DEFtag   {
    unsigned char m_bViewMode;
    DBM_MAC       m_tMin;
    DBM_MAC       m_tMax;
    DBM_MAC       m_tDef;
  }DBMRCS_MACADR_DEF, *PDBMRCS_MACADR_DEF;
/**
  Definition of an UUID element
*/
  typedef struct DBMRCS_UUID_DEFtag   {
    unsigned char   m_bViewMode;
    DBM_UUID        m_tMin;
    DBM_UUID        m_tMax;
    DBM_UUID        m_tDef;
  }DBMRCS_UUID_DEF, *PDBMRCS_UUID_DEF;
/**
  Definition of an ascii element
*/
typedef struct DBMRCS_ASCII_DEFtag   {
  unsigned char  m_bMin;
  unsigned char  m_bMax;
}DBMRCS_ASCII_DEF, *PDBMRCS_ASCII_DEF;

/**
 \union DBMRCS_DATADEFtag
 Union to handle the definition of an structure element
*/
typedef __PACKED_PRE union __PACKED_POST DBMRCS_DATADEFtag {
  DBMRCS_TOGGLE_DEF         m_tToggle; ///< Element is of type DBMRCS_TOGGLE_DEFtag
  DBMRCS_8BIT_DEF           m_t8Bit;   ///< Element is of type DBMRCS_8BIT_DEFtag
  DBMRCS_16BIT_DEF          m_t16Bit;  ///< Element is of type DBMRCS_16BIT_DEFtag
  DBMRCS_32BIT_DEF          m_t32Bit;  ///< Element is of type DBMRCS_32BIT_DEFtag
  DBMRCS_FLOAT_DEF          m_tFloat;  ///< Element is of type DBMRCS_FLOAT_DEFtag
  DBMRCS_CB_DEF             m_tCB;     ///< Element is of type DBMRCS_CB_DEFtag
  DBMRCS_OCTET_STRING_DEF   m_tOS;     ///< Element is of type DBMRCS_OCTET_STRING_DEFtag
  DBMRCS_IPADR_DEF          m_tIP;     ///< Element is of type DBMRCS_IPADR_DEFtag
  DBMRCS_ASCII_DEF          m_tAscii;  ///< Element is of type DBMRCS_ASCII_DEFtag
}DBMRCS_DATADEF, *PDBMRCS_DATADEF;

/**
 Header Structure which is added to the front of each Element definition
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCS_ELEMENT_HEADERtag {
  unsigned short  m_wThisLengh; ///< Byte length of this Element definition.
  unsigned char   m_bDataTypAndAccess;   ///< DataType and Access rights of this Element see 
  ///< eDbmDataType and eDbmAccess
  ///< The DataType is stored in the high nibble of this byte and the Access rights
  ///< in the low nibble.
  char            szName[1];    ///< \\0 terminated Description of the Element with variable length.
  ///< ATTENTION: This string is always an ascii string not a unicode one.
  ///< Immediatly behind the Name the definition of the Element with Element specific values
  ///< is following (see union DBMRCS_DATADEFtag)
}DBMRCS_ELEMENT_HEADER, *PDBMRCS_ELEMENT_HEADER;
/**
 Structure to be used in DBMRCS_REC0tag to store structure informations of a Table
 This structure is followed immediately by one or more DBMRCS_ELEMENT_HEADERtag structures. 
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCS_STRUDEF_HEADERtag {
  unsigned short  m_wTableType;  ///< Type of the Table (1,2,3) see eDbmTableType
  unsigned short  m_wCountOfElements; ///<Count of structure Elements in the Table.
  unsigned short  m_wByteLength; ///<BYTE Length of all Structures here.
  ///< Immediatly behind the m_wByteLength Element the first definition starts.
  ///< see (DBMRCS_ELEMENT_HEADERtag)
}DBMRCS_STRUDEF_HEADER, *PDBMRCS_STRUDEF_HEADER;
/**
 Structure to be used in DBMRCS_REC0tag of very old DBM files to store structure informations of a Table
 This structure is followed immediately by one or more DBMRCS_ELEMENT_HEADERtag structures. 
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCS_STRUDEF_HEADER_V1tag {
  unsigned char  m_bTableType;  ///< Type of the Table (1,2,3) see eDbmTableType
  unsigned char  m_bCountOfElements; ///<Count of structure Elements in the Table.
  unsigned short  m_wByteLength; ///<BYTE Length of all Structures here.
  ///< Immediatly behind the m_wByteLength Element the first definition starts.
  ///< see (DBMRCS_ELEMENT_HEADERtag)
}DBMRCS_STRUDEF_HEADER_V1, *PDBMRCS_STRUDEF_HEADER_V1;
/**
 This structure is describing the very beginning of a DBM file.
 Immediately after this structure the structure DBMRCS_SEG_LISTtag
 is following m_wSegCount times
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCS_FILE_HEADERtag {
  char            m_szSnap[10];
  char            m_achDate[ 12 ];
  unsigned char   m_abRes[ 30-10-12 ];
  unsigned short  m_wHeaderLength;
  unsigned short  m_wSegCount;
  unsigned short  m_wVersion;
  DBMRCS_SEG_LIST    m_tList[1];
}DBMRCS_FILE_HEADER, *PDBMRCS_FILE_HEADER;
/**
 This structure is describing the very beginning of a very old DBM file.
 Immediately after this structure the structure DBMRCS_SEG_LIST_V1tag
 is following m_wSegCount times
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCS_FILE_HEADER_V1tag {
  char            m_szSnap[30];
  unsigned short  m_wHeaderLength;
  unsigned short  m_wSegCount;
  DBMRCS_SEG_LIST_V1 m_tList[1];
}DBMRCS_FILE_HEADER_V1, *PDBMRCS_FILE_HEADER_V1;
/* ******************************************************************** */
/* END RCS definitions                                                      */
/* ******************************************************************** */

/* ******************************************************************** */
/* START RCX definitions                                                      */
/* ******************************************************************** */
typedef signed char DBM_ASCII_STR;
typedef short DBM_UCODE_STR;
typedef unsigned char DBM_TOGGLE;
typedef unsigned char DBM_COMP_BYTE;
typedef unsigned short DBM_COMP_WORD;
typedef unsigned long DBM_ASSIGNED_TOGGLE;
typedef unsigned long DBM_COMP_DWORD;
typedef unsigned long DBM_LINKED_LIST;

#define INVALID_LENGTH (unsigned long)-1
#define INVALID_OFFSET (unsigned long)-1

#ifndef RCXWCHAR
/* Because Sven gives the Info that the NetX Kernel will only support the one byte char strings
*/
typedef char RCXWCHAR;
#endif
/**
  \enum eHilscherToolType
 Enumerator to identifier the Tool which creates the File
 with the RCX_FILE_HEADERtag structure at the beginning.
*/
enum eHilscherToolType {
  eHilscherIDBM = 1
};
#ifndef __Dbm40_h__
/**
 \enum eDbmTableType
 Enumeration of the known Table types for the Method IDbmDatabase::NewTable in
 the IDbmDatabase Interface.<br>
*/
enum eDbmTableType {
  eDbmTableTypeNothing              = 0  ///< Unknown
  ,eDbmTableTypeSingleRecordPerElement = 1  ///< Each Record in the Table 
  ///< correspond to a single structure element in the structure definition
  ,eDbmTableTypeMultipleRecords = 2  ///< Each Record is structured 
  ///< by the given structure definition
  ,eDbmTableTypeSingleRecord        = 3 ///< Same as 
  ///< eMultipleRecordsWithSameStructure with only one Record
  ,eDbmTableTypeLinkedList        = 4 ///< This Table is referenced from an other Table
  ///< by a 'LinkedList' Type.<br>
  ///< This means that the structure of this Table is not full User dependend because the first
  ///< parameter must be a unsigned long which is used for referencing from other Table/s.<br>
};
#endif //__IDbmDatabase_h__
/**
  \enum eHilscherFileType   
 Enumerator for the known Hilscher File Types.
 Used in the RCX_FILE_HEADERtag structure.
*/
enum eHilscherFileType {
  eHilscherFileTypeNothing   = 0 ///< Not initialized
  ,eHilscherFileTypeDBM	    = 1 ///< This file contains a DBM Database in the old format
  ,eHilscherFileTypeCODE	    = 2 ///< This file contains executable code (e.g. PLC image)
  ,eHilscherFileTypeTASK	    = 3 ///< This file contains a loadable RCS Task. This one needs a wrapper to work with RCX
  ,eHilscherFileTypeRCSCODE	= 6 ///< not usable on RCX
  ,eHilscherFileTypeSAVE	    = 8 ///< This file contains a Save Database in the old format
  ,eHilscherFileTypeDBM2005  = 9 ///< This file contains a DBM Database in the new Format
  ,eHilscherFileTypeSAVE2005 = 10///< This file contains a Save Database in the new format
  ,eHilscherFileTypeRCXCODE	= 11///< Firmware add ons for use with RCX
  ,eHilscherFileTypeRCXTASK	= 12///< This file contains a loadable RCX Task.
};
/** \htmlonly
 <table class="Separator"><tr><td>
 <a href="#top"><img src="./images/up.gif" alt="up"></a>
 <a href="#bottom"><img src="./images/down.gif" alt="down"></a>
 </td></tr></table>
 \endhtmlonly
 */

/**  \enum eRcxStrucType
 Enumerator for RCX structures. Such an enum is written to
 the front of each structure in an RCX DBM file to make clear
 what structure is following.
*/
enum eRcxStrucType {
  eUnknownStructure = 0
  ,eDBMRCX_ELEMENT_HEADER = 1       ///< 1 This marks a DBMRCX_ELEMENT_HEADERtag structure
  ,eDBMRCX_TOGGLE_DEF               ///< 2 This marks a DBMRCX_TOGGLE_DEFtag structure
  ,eDBMRCX_ASSIGNED_TOGGLE_SUB_DEF  ///< 3 This marks a DBMRCX_ASSIGNED_TOGGLE_SUB_DEFtag structure
  ,eDBMRCX_ASSIGNED_TOGGLE_DEF      ///< 4 This marks a DBMRCX_ASSIGNED_TOGGLE_DEFtag structure
  ,eDBMRCX_8BIT_DEF                 ///< 5 This marks a DBMRCX_8BIT_DEFtag structure
  ,eDBMRCX_16BIT_DEF                ///< 6 This marks a DBMRCX_16BIT_DEFtag structure
  ,eDBMRCX_32BIT_DEF                ///< 7 This marks a DBMRCX_32BIT_DEFtag structure
  ,eDBMRCX_FLOAT_DEF                ///< 8 This marks a DBMRCX_FLOAT_DEFtag structure
  ,eDBMRCX_CB_DEF                   ///< 9 This marks a DBMRCS_CB_DEFtag structure
  ,eDBMRCX_CB8EX_DEF                ///< 10 (0xa) This marks a DBMRCX_CB8EX_DEFtag structure
  ,eDBMRCX_CB16_DEF                 ///< 11 (0xb) This marks a DBMRCS_CB16_DEFtag structure
  ,eDBMRCX_CB32_DEF                 ///< 12 (0xc) This marks a DBMRCS_CB32_DEFtag structure
  ,eDBMRCX_CB16EX_DEF               ///< 13 (0xd) This marks a DBMRCX_CB16EX_DEFtag structure
  ,eDBMRCX_CB32EX_DEF               ///< 14 (0xe) This marks a DBMRCX_CB32EX_DEFtag structure
  ,eDBMRCX_OCTET_STRING_DEF         ///< 15 (0xf) This marks a DBMRCX_OCTET_STRING_DEFtag structure
  ,eDBMRCX_IPADR_DEF                ///< 16 (0x10) This marks a DBMRCX_IPADR_DEFtag structure
  ,eDBMRCX_MACADR_DEF               ///< 17 (0x11) This marks a DBMRCX_MACADR_DEFtag structure
  ,eDBMRCX_UUID_DEF                 ///< 18 (0x12) This marks a DBMRCX_UUID_DEFtag structure
  ,eDBMRCX_ASCII_DEF                ///< 19 (0x13) This marks a DBMRCX_ASCII_DEFtag structure
  ,eDBMRCX_DATADEF                  ///< 20 (0x14) This marks a DBMRCS_DATADEFtag structure
  ,eDBMRCX_STRUDEF_HEADER           ///< 21 (0x15) This marks a DBMRCX_STRUDEF_HEADERtag structure
  ,eDBMRCX_REC0                     ///< 22 (0x16) This marks a DBMRCX_REC0tag structure
  ,eDBMRCX_REC                      ///< 23 (0x17) This marks a DBMRCX_RECtag structure
  ,eDBMRCX_INDEX                    ///< 24 (0x18) This marks a DBMRCX_INDEXtag structure
  ,eDBMRCX_TABLE                    ///< 25 (0x19) This marks a DBMRCX_TABLEtag structure
  ,eDBMRCX_OFFSET_LIST              ///< 26 (0x1a) This marks a DBMRCX_OFFSET_LISTtag structure
  ,eDBMRCX_HEADER                   ///< 27 (0x1b) This marks a DBMRCX_HEADERtag structure
  ,eDBMRCX_UNICODE_DEF              ///< 28 (0x1c) This marks a DBMRCX_UNICODE_DEFtag structure
  ,eRCX_FILE_HEADER                 ///< 29 (0x1d) This marks a RCX_FILE_HEADERtag structure
  ,eDBMRCX_LINKED_LIST_DEF          ///< 30 (0x1e) This marks a DBMRCX_LINKED_LIST_DEFtag structure
  ,eDBMRCX_LLTAB                    ///< 31 (0x1f) This marks a DBMRCX_LLTABtag structure
  ,eDBMRCX_MD5_DEF                  ///< 32 (0x20) This marks a DBMRCX_MD5tag structure
  ,eDBMRCX_BLOB_DEF                 ///< 33 (0x21) This marks a DBMRCX_BLOBtag structure
};
/** \htmlonly
 <table class="Separator"><tr><td>
 <a href="#top"><img src="./images/up.gif" alt="up"></a>
 <a href="#bottom"><img src="./images/down.gif" alt="down"></a>
 </td></tr></table>
 \endhtmlonly
 */

/**
 A Toggle Element becomes after his description text the following 
 definition extension.
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_TOGGLE_DEFtag
{
  unsigned long       m_eStrucType;///< should be eDBMRCX_TOGGLE_DEF for this structure
  unsigned char       m_bMin; ///< Minimum value for this Element. This is set automatically
  ///< to the first value for which a string with a length > 0 is given.
  unsigned char       m_bMax; ///< Maximum value for this Element. This is set automatically
  ///< to the last value for which a string with a length > 0 is given.
  unsigned char       m_bDef; ///< Default value for this Element. This is set automatically
  ///< to the minimum value.
  RCXWCHAR               m_wchToggleStrings[1]; ///< The Toggle Strings are separated by a comma
  ///< \code "Toggle_1,Toggle_2,,,,Toggle_n" will result in the assigned Values
  ///< Toggle_1 = 0; Toggle_2 = 1 and Toggle_n = 5
  ///< \endcode
}DBMRCX_TOGGLE_DEF, *PDBMRCX_TOGGLE_DEF;

#define RCX_TOGGLEDEF_STRUCT_LENGTH (sizeof(DBMRCX_TOGGLE_DEF) - sizeof (RCXWCHAR))
/**
 Sub structure for defining an assigned toggle with the 
 DBMRCX_ASSIGNED_TOGGLE_DEF structure.
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_ASSIGNED_TOGGLE_SUB_DEFtag 
{
  unsigned long       m_eStrucType;///< should be eDBMRCX_ASSIGNED_TOGGLE_SUB_DEF for this structure
  long                m_lAssignedValue;      ///< User assigned value for this toggle string
  RCXWCHAR            m_wchToggleElement[1];  ///< Toggle Strings
}DBMRCX_ASSIGNED_TOGGLE_SUB_DEF, *PDBMRCX_ASSIGNED_TOGGLE_SUB_DEF;
#define RCX_ASSIGNEDTOGGLESUB_DEF_STRUCT_LENGTH (sizeof (DBMRCX_ASSIGNED_TOGGLE_SUB_DEF) - sizeof(RCXWCHAR))
/**
 Definition of the assigned toggle string
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_ASSIGNED_TOGGLE_DEFtag
{
  unsigned long       m_eStrucType;///< should be eDBMRCX_ASSIGNED_TOGGLE_DEF for this structure
  long                m_lMin;  ///< automatically inserted and set to the minimum of user assigned values
  long                m_lMax;  ///< automatically inserted and set to the maximum of user assigned values
  long                m_lDef;  ///< can be set by the user. If no User input than it's set to the minimum value
  unsigned long       m_lCountOfStringsFollowing; ///< count of following definitions of the strings and their values
  DBMRCX_ASSIGNED_TOGGLE_SUB_DEF  m_atElements[1];
}DBMRCX_ASSIGNED_TOGGLE_DEF, *PDBMRCX_ASSIGNED_TOGGLE_DEF;
#define RCX_ASSIGNEDTOGGLE_DEF_STRUCT_LENGTH (sizeof(DBMRCX_ASSIGNED_TOGGLE_DEF) - sizeof (DBMRCX_ASSIGNED_TOGGLE_SUB_DEF))
/**
 This structure will describe the type specific paremeter for
 an <span class="blue">8 Bit</span> Element in the Record
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_8BIT_DEFtag
{
  unsigned long       m_eStrucType;///< should be eDBMRCX_8BIT_DEF for this structure
  DBM_8BIT_SU         m_tMin;  ///< Minimum value for this Element
  DBM_8BIT_SU         m_tMax;  ///< Maximum value for this Element
  DBM_8BIT_SU         m_tDef;  ///< Default value for this Element
}DBMRCX_8BIT_DEF, *PDBMRCX_8BIT_DEF;
#define RCX_8BIT_DEF_STRUCT_LENGTH (sizeof(DBMRCX_8BIT_DEF))
/**
 This structure will describe the type specific paremeter for
 an <span class="blue">16 Bit</span> Element in the Record
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_16BIT_DEFtag
{
  unsigned long       m_eStrucType;///< should be eDBMRCX_16BIT_DEF for this structure
  DBM_16BIT_SU        m_tMin;  ///< Minimum value for this Element
  DBM_16BIT_SU        m_tMax;  ///< Maximum value for this Element
  DBM_16BIT_SU        m_tDef;  ///< Default value for this Element
}DBMRCX_16BIT_DEF, *PDBMRCX_16BIT_DEF;
#define RCX_16BIT_DEF_STRUCT_LENGTH (sizeof(DBMRCX_16BIT_DEF))
/**
 This structure will describe the type specific paremeter for
 an <span class="blue">32 Bit</span> Element in the Record
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_32BIT_DEFtag
{
  unsigned long       m_eStrucType;///< should be eDBMRCX_32BIT_DEF for this structure
  DBM_32BIT_SU        m_tMin;  ///< Minimum value for this Element
  DBM_32BIT_SU        m_tMax;  ///< Maximum value for this Element
  DBM_32BIT_SU        m_tDef;  ///< Default value for this Element
}DBMRCX_32BIT_DEF, *PDBMRCX_32BIT_DEF;
#define RCX_32BIT_DEF_STRUCT_LENGTH (sizeof(DBMRCX_32BIT_DEF))
/**
 This structure will describe the type specific paremeter for
 an <span class="blue">Float (IEEE)</span> Element in the Record
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_FLOAT_DEFtag
{
  unsigned long       m_eStrucType;///< should be eDBMRCX_FLOAT_DEF for this structure
  float               m_fltMin;  ///< Minimum value for this Element
  float               m_fltMax;  ///< Maximum value for this Element
  float               m_fltDef;  ///< Default value for this Element
  float               m_fltDeadband; ///< This value is used to compare one float with an other.
  ///< This 2 floats are equal, when the first one is within the second ones deadband.
  ///< e.g. <span class="hint">flt1 == flt2 when flt2-deadband <= flt1 <= flt2+deadband</span>
}DBMRCX_FLOAT_DEF, *PDBMRCX_FLOAT_DEF;
#define RCX_FLOAT_DEF_STRUCT_LENGTH (sizeof(DBMRCX_FLOAT_DEF))
/**
 This structure will describe the type specific paremeter for
 an <span class="blue">compressed 8 Bit</span> Element in the Record
 <br>After this structure the structure DBMRCX_CB_DEF follows immediately with
 8 comma separated strings for the description of the Bit0 to Bit7.
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_CB8EX_DEFtag
{
  unsigned long       m_eStrucType;///< should be eDBMRCX_CB8EX_DEF for this structure
  DBM_8BIT_SU         m_tMin;  ///< Minimum value for this Element
  DBM_8BIT_SU         m_tMax;  ///< Maximum value for this Element
  DBM_8BIT_SU         m_tDef;  ///< Default value for this Element
  RCXWCHAR               m_wchBitDescription[1]; ///< Extended parameter for this type
}DBMRCX_CB8EX_DEF, *PDBMRCX_CB8EX_DEF;
#define RCX_CB8_DEF_STRUCT_LENGTH (sizeof(DBMRCX_CB8EX_DEF) - sizeof(RCXWCHAR))
/**
 This structure will describe the type specific paremeter for
 an <span class="blue">compressed 16 Bit</span> Element in the Record
 <br>After this structure the structure DBMRCX_CB_DEF follows immediately with
 16 comma separated strings for the description of the Bit0 to Bit15.
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_CB16EX_DEFtag
{
  unsigned long       m_eStrucType;///< should be eDBMRCX_CB16EX_DEF for this structure
  DBM_16BIT_SU        m_tMin;  ///< Minimum value for this Element
  DBM_16BIT_SU        m_tMax;  ///< Maximum value for this Element
  DBM_16BIT_SU        m_tDef;  ///< Default value for this Element
  RCXWCHAR               m_wchBitDescription[1]; ///< Extended parameter for this type
}DBMRCX_CB16EX_DEF, *PDBMRCX_CB16EX_DEF;
#define RCX_CB16_DEF_STRUCT_LENGTH (sizeof(DBMRCX_CB16EX_DEF) - sizeof(RCXWCHAR))
/**
 This structure will describe the type specific paremeter for
 an <span class="blue">compressed 32 Bit</span> Element in the Record
 <br>After this structure the structure DBMRCX_CB_DEF follows immediately with
 32 comma separated strings for the description of the Bit0 to Bit31.
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_CB32EX_DEFtag
{
  unsigned long       m_eStrucType;///< should be eDBMRCX_CB32EX_DEF for this structure
  DBM_32BIT_SU        m_tMin;  ///< Minimum value for this Element
  DBM_32BIT_SU        m_tMax;  ///< Maximum value for this Element
  DBM_32BIT_SU        m_tDef;  ///< Default value for this Element
  RCXWCHAR               m_wchBitDescription[1]; ///< Extended parameter for this type
}DBMRCX_CB32EX_DEF, *PDBMRCX_CB32EX_DEF;
#define RCX_CB32_DEF_STRUCT_LENGTH (sizeof(DBMRCX_CB32EX_DEF) - sizeof(RCXWCHAR))
/**
 This structure will describe the type specific paremeter for
 an <span class="blue">OctetString</span> Element in the Record
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_OCTET_STRING_DEFtag 
{
  unsigned long       m_eStrucType;///< should be eDBMRCX_OCTET_STRING_DEF for this structure
  unsigned short      m_wMin; ///< Minimum length for the OctetString
  unsigned short      m_wMax; ///< Maximum length for the OctetString
  ///< This Type will always use as much space as given in the maximum length
  ///< plus 2 Bytes because before the OctetString the actual length is
  ///< stored in the Record.
}DBMRCX_OCTET_STRING_DEF, *PDBMRCX_OCTET_STRING_DEF;
#define RCX_OCTETSTRING_DEF_STRUCT_LENGTH (sizeof(DBMRCX_OCTET_STRING_DEF))
/**
 This structure will describe the type specific paremeter for
 an <span class="blue">OctetString</span> Element in the Record
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_BLOB_DEFtag 
{
  unsigned long       m_eStrucType;///< should be eDBMRCX_BLOB_DEF for this structure
  unsigned long       m_dwMin; ///< Minimum length for the Blob
  unsigned long       m_dwMax; ///< Maximum length for the Blob
  ///< This Type will occupie as much space as defined in the actual length 
  ///< plus 4 Bytes for the length itself. The length is stored before the array of data.
}DBMRCX_BLOB_DEF, *PDBMRCX_BLOB_DEF;
#define RCX_BLOB_DEF_STRUCT_LENGTH (sizeof(DBMRCX_BLOB_DEF))
/**
 This structure will describe the type specific paremeter for
 an <span class="blue">IP Address</span> Element in the Record
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_IPADR_DEFtag
{
  unsigned long       m_eStrucType;///< should be eDBMRCX_IPADR_DEF for this structure
  DBM_IP              m_tMin; ///< Minimum value for the IP Address
  DBM_IP              m_tMax; ///< Maximum value for the IP Address
  DBM_IP              m_tDef; ///< Default value for the IP Address
}DBMRCX_IPADR_DEF, *PDBMRCX_IPADR_DEF;
#define RCX_IPADR_DEF_STRUCT_LENGTH (sizeof(DBMRCX_IPADR_DEF))
/**
 This structure will describe the type specific paremeter for
 an <span class="blue">MAC Address</span> Element in the Record
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_MACADR_DEFtag
{
  unsigned long       m_eStrucType;///< should be eDBMRCX_MACADR_DEF for this structure
  DBM_MAC             m_tMin; ///< Minimum value for the MAC Address
  DBM_MAC             m_tMax; ///< Maximum value for the MAC Address
  DBM_MAC             m_tDef; ///< Default value for the MAC Address
}DBMRCX_MACADR_DEF, *PDBMRCX_MACADR_DEF;
#define RCX_MACADR_DEF_STRUCT_LENGTH (sizeof(DBMRCX_MACADR_DEF))
/**
 This structure will describe the type specific paremeter for
 an <span class="blue">UUID</span> Element in the Record
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_UUID_DEFtag
{
  unsigned long       m_eStrucType;///< should be eDBMRCX_UUID_DEF for this structure
  DBM_UUID            m_tMin; ///< Minimum value for the UUID
  DBM_UUID            m_tMax; ///< Maximum value for the UUID
  DBM_UUID            m_tDef; ///< Default value for the UUID
}DBMRCX_UUID_DEF, *PDBMRCX_UUID_DEF;
#define RCX_UUID_DEF_STRUCT_LENGTH (sizeof(DBMRCX_UUID_DEF))
/**
 This structure will describe the type specific paremeter for
 an <span class="blue">UUID</span> Element in the Record
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_LINKED_LIST_DEFtag
{
  unsigned long     m_eStrucType;///< should be eDBMRCX_LINKED_LIST_DEF for this structure
  unsigned long    m_dwMin; ///< Minimum value for the KEY
  unsigned long    m_dwMax; ///< Maximum value for the KEY
  unsigned long    m_dwDef; ///< Default value for the KEY
}DBMRCX_LINKED_LIST_DEF, *PDBMRCX_LINKED_LIST_DEF;
#define RCX_LINKEDLIST_DEF_STRUCT_LENGTH (sizeof(DBMRCX_LINKED_LIST_DEF))
/**
 This structure will describe the type specific paremeter for
 an <span class="blue">Ascii</span> Element in the Record
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_ASCII_DEFtag
{
  unsigned long  m_eStrucType;///< should be eDBMRCX_ASCII_DEF for this structure
  unsigned char  m_bMin;   ///< This means the minimum length of the string
  unsigned char  m_bMax;   ///< This means the maximum length of the string. The Record will
  RCXWCHAR          m_wchDefText[1]; ///< Default Text
  ///< reserve always as much space as the maximum string length + 1 is.
}DBMRCX_ASCII_DEF, *PDBMRCX_ASCII_DEF;
#define RCX_ASCII_DEF_STRUCT_LENGTH (sizeof(DBMRCX_ASCII_DEF) - sizeof(RCXWCHAR))
/**
 This structure will describe the type specific paremeter for
 an <span class="blue">Unicode</span> Element in the Record
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_UNICODE_DEFtag
{
  unsigned long  m_eStrucType;///< should be eDBMRCX_UNICODE_DEF for this structure
  unsigned long  m_ulMin;   ///< This means the minimum length of the string 
  ///< in bytes
  unsigned long  m_ulMax;   ///< This means the maximum length of the string 
  ///< in bytes. The Record will reserve always as much space as the maximum 
  ///< string length + 2 is.
  RCXWCHAR          m_wchDefText[1]; ///< Default Text
}DBMRCX_UNICODE_DEF, *PDBMRCX_UNICODE_DEF;
#define RCX_UNICODE_DEF_STRUCT_LENGTH (sizeof(DBMRCX_UNICODE_DEF) - sizeof(RCXWCHAR))
/**
 \union UMD5tag
 128 Bit security value,
 This value is used to detect if a change was done in 
 the data this Checksum belongs to after the Checksum
 was calculated.
*/
#ifndef UMD5_DEFINED
#define UMD5_DEFINED
typedef __PACKED_PRE union __PACKED_POST UMD5tag {
  unsigned char   m_abVal[16];	///< 128 Bit as Byte Array
  unsigned short  m_ausVal[8];	///< 128 Bit as Word Array
  unsigned long   m_adwVal[4];	///< 128 Bit as DWord Array
} UMD5;
#endif
/**
 This structure will describe the type specific paremeter for
 an <span class="blue">MD5</span> Element in the Record
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_MD5_DEFtag
{
  unsigned long       m_eStrucType;///< should be eDBMRCX_MD5_DEF for this structure
  UMD5                m_tMin; ///< Minimum value for the MD5
  UMD5                m_tMax; ///< Maximum value for the MD5
  UMD5                m_tDef; ///< Default value for the MD5
}DBMRCX_MD5_DEF, *PDBMRCX_MD5_DEF;
#define RCX_MD5_DEF_STRUCT_LENGTH (sizeof(DBMRCX_MD5_DEF))
/**
 This structure is only for making the description strutured.
 To resolve all links.
*/
typedef __PACKED_PRE union __PACKED_POST DBMRCX_TYPE_SPECIFICtag 
{
  unsigned long               m_eStrucType;///< should be eDBMRCX_DATADEF for this structure
  DBMRCX_TOGGLE_DEF           m_tToggle;///< Toggle (selection of strings)
  DBMRCX_ASSIGNED_TOGGLE_DEF  m_tAssignedToggle;///< Toggle with user assigned values
  DBMRCX_8BIT_DEF             m_t8Bit;  ///< 32Bit value
  DBMRCX_16BIT_DEF            m_t16Bit; ///< 32Bit value
  DBMRCX_32BIT_DEF            m_t32Bit; ///< 32Bit value
  DBMRCX_FLOAT_DEF            m_tFloat; ///< Float
  DBMRCX_CB8EX_DEF            m_tCB8;   ///< Compressed 8Bit
  DBMRCX_CB16EX_DEF           m_tCB16;  ///< Compressed 8Bit
  DBMRCX_CB32EX_DEF           m_tCB32;  ///< Compressed 8Bit
  DBMRCX_OCTET_STRING_DEF     m_tOS;    ///< Octet string (byte array)
  DBMRCX_IPADR_DEF            m_tIP;    ///< IP address
  DBMRCX_ASCII_DEF            m_tAscii; ///< Ascii string
  DBMRCX_MACADR_DEF           m_tMAC;   ///< MAC address
  DBMRCX_UUID_DEF             m_tUUID;  ///< UUID
  DBMRCX_MD5_DEF              m_tMD5;   ///< MD5
}DBMRCX_TYPE_SPECIFIC, *PDBMRCX_TYPE_SPECIFIC;
/**
 Header Structure which is added to the front of each Element definition
 (DBMRCX_TOGGLE_DEFtag, ...,DBMRCX_LL_DEFtag )
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_ELEMENT_HEADERtag
{
  unsigned long         m_eStrucType;///< should be eDBMRCX_ELEMENT_HEADER for this structure
  unsigned long         m_ulThisLengh; ///< Byte length of this Element definition (including the 
  ///< dummys if neccessary)
  unsigned long       m_dwDummysAdded;  ///< Count of dummy bytes added to this data to
  ///< start at a 4 byte boundary with the next structure
  unsigned long         m_ulTypeSpecificOffset; ///< Offset in this structure where the type 
  ///< specific parameter starts (see DBMRCX_TYPE_SPECIFICtag). This is an Offset relative to the
  ///< beginning of this structure.
  unsigned long         m_eDataType;   ///< DataType of this Element see eDbmDataType
  unsigned long         m_eAccessType; ///< Access rights of this Element see eDbmAccess
  unsigned long         m_eViewMode; ///< View Mode for this Element
  unsigned long         m_ulArrayElements; ///< Count of Elements if this is an Array
  RCXWCHAR              m_wchDescription[1];    ///< \\0 terminated Description of the Element with variable length.
  ///< ATTENTION: This string is always a unicode string.
  ///< Immediatly behind the Name the definition of the Element with Element specific values
  ///< is following (see union DBMRCX_TYPE_SPECIFICtag)
}DBMRCX_ELEMENT_HEADER, *PDBMRCX_ELEMENT_HEADER;
#define RCX_ELEMENT_HEADER_STRUCT_LENGTH (sizeof(DBMRCX_ELEMENT_HEADER) - sizeof(RCXWCHAR))
#define RCX_STRUCT_ELEMENT_DEF_LENGTH(a) (unsigned long)(a->m_ulThisLengh)
/**
 Structure to be used in DBMRCX_REC0tag to store structure informations of a Table
 This structure is followed immediately by one or more DBMRCX_ELEMENT_HEADERtag structures. 
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_STRUDEF_HEADERtag
{
  unsigned long       m_eStrucType;     ///< should be eDBMRCX_STRUDEF_HEADER for this structure
  unsigned long       m_dwThisLength;   ///< Length of this structure in Bytes 
  unsigned long       m_ulCountOfElements; ///<Count of structure Elements in the Table.
  ///< Immediatly behind the m_ulCountOfElements Element the first definition starts.
  ///< see (DBMRCX_ELEMENT_HEADER)
}DBMRCX_STRUDEF_HEADER, *PDBMRCX_STRUDEF_HEADER;
#define RCX_STRUDEF_HEADER_STRUCT_LENGTH (sizeof(DBMRCX_STRUDEF_HEADER))
#define RCX_STRUCT_DEFINITION_LENGTH(a) (unsigned long)(a->m_dwThisLength-a->m_dwDummysAdded)
/**
 Structure for the special record with no. 0 which is used
 to store table related informations.
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_REC0tag 
{
  unsigned long       m_eStrucType;     ///< should be eDBMRCX_REC0 for this structure
  unsigned long       m_dwThisLength;   ///< Length of this structure in Bytes (including the 
  ///< dummys if neccessary)
  unsigned long       m_dwDummysAdded;  ///< Count of dummy bytes added to this data to
  ///< start at a 4 byte boundary with the next structure
  unsigned long       m_ulIndexType;    ///< Used when the records are sorted
//  unsigned long       m_ulMaxRecords;   ///< This Table can store this maximum of records.
  unsigned long       m_eTableProperty; ///< see also eDbmTableProperty
  RCXWCHAR            m_awchTableName[1];///< Name of the Table as '\\0' terminated string
  ///< when a structure definition is present in this table the structure DBMRCX_STRUDEF_HEADERtag
  ///< will follow immediately after the m_achTableName Element.
}DBMRCX_REC0, *PDBMRCX_REC0;
#define RCX_REC0_STRUCT_LENGTH (unsigned long)(sizeof(DBMRCX_REC0)-sizeof(RCXWCHAR)) //length minus name
/**
 The structure to store one Record.
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_RECtag 
{
  unsigned long       m_eStrucType;     ///< should be eDBMRCX_REC for this structure
  unsigned long       m_dwThisLength;   ///< Length of this structure in Bytes (including the 
  ///< dummys if neccessary)
  unsigned long       m_dwDummysAdded;  ///< Count of dummy bytes added to this data to
  ///< start at a 4 byte boundary with the next structure
  unsigned long       m_dwRecordNumber; ///< Number of this Record
  unsigned char       m_abData[1];      ///< Data for the record as BYTE Stream.
}DBMRCX_REC, *PDBMRCX_REC;
#define RCX_REC_STRUCT_LENGTH (sizeof(DBMRCX_REC)-sizeof(unsigned char)) //length minus data array
#define RCX_REC_DATA_LENGTH(ptRec) (unsigned long)(ptRec->m_dwThisLength-ptRec->m_dwDummysAdded-RCX_REC_STRUCT_LENGTH)
/**
 Structure to store the Records sorted.
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_INDEXtag 
{
  unsigned long       m_eStrucType;      ///< should be eDBMRCX_INDEX for this structure
  unsigned long       m_dwThisLength;      ///< Length of this structure in Bytes
  unsigned long       m_aulRecordNumber[1]; ///< Number of the Records in the sorted order.
}DBMRCX_INDEX, *PDBMRCX_INDEX;
#define RCX_INDEX_STRUCT_LENGTH (sizeof(DBMRCX_INDEX)-sizeof(unsigned long)) //length minus data array
/**
 Structure to store the Offsets of the Records of one KEY
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_LLTABtag 
{
  unsigned long       m_eStrucType;       ///< should be eDBMRCX_LLTAB for this structure
  unsigned long       m_dwThisLength;      ///< Length of this structure in Bytes
  unsigned long       m_ulKEY;            ///< The KEY for all Records referenced in this structure
  unsigned long       m_ulOffsetArraySize; ///< Count of valid record entrys in m_aulRecordOffset
  unsigned long       m_aulRecordOffset[1];  ///< Array of Offsets for the Records in this Table.
  ///< The Seek Method of the IStream Interface can be used to get the beginning
  ///< of the DBMRCX_RECtag structure for the related Record.
}DBMRCX_LLTAB, *PDBMRCX_LLTAB;
#define RCX_LLSTRUCT_LENGTH (sizeof(DBMRCX_LLTAB)-sizeof(unsigned long)) //length minus data array
/**
 Structure to store the Offsets of the Records of one Table
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_TABLEtag 
{
  unsigned long       m_eStrucType;      ///< should be eDBMRCX_TABLE for this structure
  unsigned long       m_dwThisLength;      ///< Length of this structure in Bytes
  unsigned long       m_ulStructureStartOffset; ///< Not equal 0, when this Table contain structure information. 
  unsigned long       m_ulOffsetArraySize; ///< Count of valid record entrys in m_aulRecordOffset
  UMD5                m_uMd5Password;      ///< Password for this Table or 0 if not used.
  unsigned long       m_eTableType;     ///< Type of this Table (1,2,3,4) see eDbmTableType
  unsigned long       m_ulVersion;     ///< Version of this Table
  unsigned long       m_aulRecordOffset[1];  ///< Offset Array for all Records in this Table.
  ///< In case this Table is of type eDbmTableTypeSingleRecordPerElement(1), 
  ///< eDbmTableTypeMultipleRecords(2) or eDbmTableTypeSingleRecord the offsets starting from
  ///< m_aulRecordOffset[1] in this Array can be used to point to the DBMRCX_RECtag 
  ///< structure of the related Record.<br>
  ///< <span class="red">ATTENTION:</span> In case this Table is of type eDbmTableTypeLinkedList
  ///< we will have offsets to the structure DBMRCX_LLTABtag in this Array.<br>
  ///< In all types the offset in m_aulRecordOffset[0] will point to the internal structure
  ///< DBMRCX_REC0tag which is used to hold table related properties and the structure 
  ///< definition if available.
}DBMRCX_TABLE, *PDBMRCX_TABLE;
#define RCX_TABLE_STRUCT_LENGTH (sizeof(DBMRCX_TABLE)-sizeof(unsigned long)) //length minus data array
/**
 Offset Pair for each Table in this Database to address the 
 beginning of the Record and the Index directory of each Table.
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_OFFSET_LISTtag 
{
  unsigned long       m_ulTableOffset;  ///< Offset to the beginning of the Table description (DBMRCX_TABLEtag) structure.
  unsigned long       m_ulIndexOffset;  ///< Offset to the beginning of the Index description (DBMRCX_INDEXtag) structure.
  ///< Can be 0 when the Records are not sorted.
  ///< This structure is immediately followed by the first DBMRCX_TABLEtag structure
}DBMRCX_OFFSET_LIST , *PDBMRCX_OFFSET_LIST ;
#define RCX_OFFSETLIST_STRUCT_LENGTH (sizeof(DBMRCX_OFFSET_LIST)) //length minus Offset List
/**
 When this file is an RCX DBM file this structure will follow the 
 RCX_FILE_HEADERtag structure and contain the first DBM specific values.
*/
typedef __PACKED_PRE struct __PACKED_POST DBMRCX_HEADERtag 
{
  unsigned long       m_eStrucType;      ///< should be eDBMRCX_HEADER for this structure
  unsigned long       m_ulCountOfTables; ///< Maximum Number of Tables in this Database.
  DBMRCX_OFFSET_LIST  m_atOffsetList[1]; ///< with <span class="red">m_ulCountOfTables</span> entries.
}DBMRCX_HEADER, *PDBMRCX_HEADER;
#define RCX_HEADER_STRUCT_LENGTH (sizeof(DBMRCX_HEADER) - sizeof(DBMRCX_OFFSET_LIST)) //length minus Offset List
/**
 This is the Header for each RCX file and it contains file global definitions.
 File Header for the new DBM Format.<br>
 This Header is only used to find out if this is really a DBM file and that this
 file contain the database in the expected format.
 This structure has a length of 48 (0x30) Bytes. 
*/
typedef __PACKED_PRE struct __PACKED_POST RCX_FILE_HEADERtag 
{
  char	                  m_achSnapString[10];	///< Snap String "HILSCHER " used to find out if 
  char	                  m_achFill[2];	///< to bring the next member to a 4 byte boundary
  ///< this file was created by the company <span class="redBig">Hilscher GmbH</span>
  unsigned long           m_eStrucType;         ///< should be eRCX_FILE_HEADER for this structure
  unsigned long           m_eToolType;          ///< identification of the Tool which creates the 
  ///< File see eHilscherToolType
  unsigned long           m_ulVersion;		      ///< Version of the Tools who generate this file
  unsigned long           m_ulFileLength;		    ///< Length of this file in BYTES
  UMD5                    m_umd5Val;             ///< Security Value for change detection. 
  ///< See DbmComVariant::MD5Tag structure.<br>
  ///< <span class="hint">The file Header itself is not a part of the checksum.</span>
  ///< Immediatly behind the m_dwReserved2 Element the Segment definition structure starts
  ///< see (DBMRCX_HEADERtag)
  unsigned long           m_eFileType;		      ///< Type of this file see enum eHilscherFileType
  ///< This is <span class="red">not defined</span> as eHilscherFileType because it's possible that
  ///< some compilers use a short for enums. This will cause errors when the file is written
  ///< with a different Tool which creates an 32bit value for an enum.
}RCX_FILE_HEADER, *PRCX_FILE_HEADER;
#define RCX_FILEHEADER_STRUCT_LENGTH (sizeof(RCX_FILE_HEADER))

//typedef __PACKED_PRE struct __PACKED_POST NETX_FILE_HEADER_CRCSAVEDtag 
//{
//#define NETX_MAG_DBM_COOKIE (0x2E4E5844)  //('.''N''X''D')
//#define NETX_FILE_SIGN      (0x4E455458)  //('N''E''T''X')
//#define NETX_FILE_HEADER_VERSION_1    1000
//  unsigned long           m_ulMagCookie;	  ///< Byte No 0-3:  .NXD (0x2E4E5844)
//  unsigned long           m_ulReserved;	    ///< Byte No 4-7:  Reserved initialized with 0
//  unsigned long           m_ulAppEnt;  	    ///< Byte No 8:  not used in DBM
//  unsigned long           m_ulAppChksm;	    ///< Byte No 12: not used in DBM 
//  unsigned long           m_ulFilSize;	    ///< Byte No 16: File length in BYTE
//  unsigned long           m_ulAppSrtAdd;	  ///< Byte No 20: not used in DBM 
//  unsigned long           m_ulSignt;	      ///< Byte No 24:  NETX
//  UMD5                    m_umd5Val;        ///< Byte No 28-43:  Security Value for change detection. 16Bytes 4unsigned longs
//  unsigned long           m_ulRes5;	        ///< Byte No 44: Reserved initialized with 0
//  unsigned long           m_ulRes6;	        ///< Byte No 48: Reserved initialized with 0
//  unsigned long           m_ulRes7;	        ///< Byte No 52: Reserved initialized with 0
//  unsigned short          m_usHeaderLength; ///< Byte No 56: Byte length of this Header ()
//  unsigned short          m_usHeaderVersion;///< Byte No 58: Version of this Header NETX_FILEHEADER_STRUCT_LENGTH
//}NETX_FILE_HEADER_CRCSAVED, *PNETX_FILE_HEADER_CRCSAVED;
//#define NETX_FILEHEADER_CRCSAVED_STRUCT_LENGTH (sizeof(NETX_FILE_HEADER_CRCSAVED))
///**
// This is the new standard (03/27/07) Header for each NETX file.<br>
// This Header is only used to find out if this is really a DBM file and that this
// file contain the database in the expected format.<br>
// This structure has a length of 64 (0x40) unsigned longs. 
//*/
//typedef __PACKED_PRE struct __PACKED_POST NETX_FILE_HEADERtag 
//{
//  NETX_FILE_HEADER_CRCSAVED m_tCrcSaved;
//  unsigned long             m_ulChkSm;        ///< Byte No 60: CRC32 of data in m_tCrcSaved
//  unsigned long             m_aulDBVersion[2];///< Byte No 64..71: Major/Minor Version of the following database
//}NETX_FILE_HEADER, *PNETX_FILE_HEADER;
//#define NETX_FILEHEADER_STRUCT_LENGTH (sizeof(NETX_FILE_HEADER))
#define HIL_FILE_DATABASE_HEADER_STRUCT_LENGTH (sizeof(HIL_FILE_DATABASE_HEADER))
#define HIL_FILE_DATABASE_HEADER_CRCSAVED_STRUCT_LENGTH (sizeof(HIL_FILE_COMMON_HEADER)-offsetof(HIL_FILE_COMMON_HEADER, ulChksum))

/* ******************************************************************** */
/* END RCX definitions                                                      */
/* ******************************************************************** */
/* ************************************************************************ */
/* END of structure definitions                                           */
/* ************************************************************************ */

/*---------------------------------------------------------------------------*/
/* Compiler settings */
/* pragma pack */
#ifdef PRAGMA_PACK_ENABLE
#pragma PRAGMA_UNPACK_1(MID_DBM)
#endif
/*---------------------------------------------------------------------------*/


#endif //_DBMRCX_H_
