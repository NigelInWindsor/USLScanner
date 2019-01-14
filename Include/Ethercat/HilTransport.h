/******************************************************************************

  Copyright (C) 2010 [Hilscher Gesellschaft für Systemautomation mbH]

  This program can be used by everyone according to the netX Open Source
  Software license. The license agreement can be downloaded from
  http://www.industrialNETworX.com

*******************************************************************************

  Last Modification:
    @version $Id: HilTransport.h 358 2010-05-03 07:27:27Z MichaelT $

  Description:
    Definition for the universal Hilscher Transport Header prepended, to all
    data, send via a communication channel like USB/TCP/Serial.

  Changes:
    Date        Author        Description
  ---------------------------------------------------------------------------
    2010-03-23  NC            File created.

******************************************************************************/


#ifndef __HILTRANSPORT__H
#define __HILTRANSPORT__H

#include <stdint.h>

/*****************************************************************************/
/*! \file HilTransport.h                           
*  Definition for the universal Hilscher Transport Header                    */
/*****************************************************************************/

#define HIL_TRANSPORT_IP_PORT           50111           /**< IP Port number used for transport         */

#define HIL_TRANSPORT_COOKIE            0xA55A5AA5UL    /**< Cookie for HIL_TRANSPORT_HEADER                */
#define HIL_TRANSPORT_PACKETID          "netX"          /**< Packet ID                                      */

#define HIL_TRANSPORT_STATE_OK          0x00            /**< Packet received successfully                   */ 

#define HIL_TSTATE_CHECKSUM_ERROR       0x10            /**< Packet checksum error                          */
#define HIL_TSTATE_LENGTH_INCOMPLETE    0x11            /**< Packet length incomplete                       */
#define HIL_TSTATE_DATA_TYPE_UNKNOWN    0x12            /**< Packet data type unknown                       */  
#define HIL_TSTATE_DEVICE_UNKNOWN       0x13            /**< Device unknown                                 */  
#define HIL_TSTATE_CHANNEL_UNKNOWN      0x14            /**< Channel unknown                                */
#define HIL_TSTATE_SEQUENCE_ERROR       0x15            /**< Sequence number out of sync                    */
#define HIL_TSTATE_BUFFEROVERFLOW_ERROR 0x16            /**< Buffer overflow in receiver                    */
#define HIL_TSTATE_RESOURCE_ERROR       0x17            /**< Out of internal resources 
                                                             (e.g. parallel services exceeded)              */
#define HIL_TSTATE_KEEP_ALIVE_ERROR     0x20            /**< Keep Alive ComID was incorrect                 */

#define HIL_TRANSPORT_TYPE_RCX_PACKET   0x0100          /**< Packet Type for transferring a RCX Packet      */
#define HIL_TRANSPORT_TYPE_MARSHALLER   0x0200          /**< Packet Type for function marshalling           */
#define HIL_TRANSPORT_TYPE_ACKNOWLEDGE  0x8000          /**< Packet Type for acknowledging succesful send   */
#define HIL_TRANSPORT_TYPE_KEEP_ALIVE   0xFFFF          /**< Packet Type for heart beat packet              */

#define HIL_TRANSPORT_KEEP_ALIVE_FIRST_COMID 0x00000000 /**< Default value for the Keep Alive ComID         */
#define HIL_TRANSPORT_KEEP_ALIVE_CLIENT_TIMEOUT     500 /**< Timeout(ms) for the Client-side of the Keep Alive  */
#define HIL_TRANSPORT_KEEP_ALIVE_SERVER_TIMEOUT    2000 /**< Timeout(ms) for the Server-side of the Keep Alive  */


/*****************************************************************************/
/*! Structure preceeding every data packet sent to netX based devices.
*   This structure is independent from the transmission channel.             */
/*****************************************************************************/
// This version is replaced by the following one ...changed by RM.....
//typedef struct HIL_TRANSPORT_HEADERtag
//{
//  uint8_t   abPacketId[4]; /**< Start of packet identifier "netX"        */
//  uint32_t  ulCookie;      /**< Magic cookie                             */
//  uint32_t  ulDataType;    /**< Type of data following                   */
//  uint32_t  ulLength;      /**< Length of following data (in Bytes)      */
//  uint32_t  ulState;       /**< Transmission/Target state                */
//  uint16_t  usChecksum;    /**< CRC16 over all data, 0 = no checksum     */
//  uint16_t  usSequenceNr;  /**< Sequence Nr. Increased with every packet */
//  uint32_t  ulReserved;    /**< unused/reserved for future use           */
//  
//} HIL_TRANSPORT_HEADER, *PHIL_TRANSPORT_HEADER;

typedef struct HIL_TRANSPORT_HEADERtag
{
  uint32_t  ulCookie;      /**< Magic cookie                             */
  uint32_t  ulLength;      /**< Length of following data (in Bytes)      */
  uint16_t  usChecksum;    /**< CRC16 over all data, 0 = no checksum     */
  uint16_t  usDataType;    /**< Type of data following                   */
  uint8_t   bDevice;       /**< Device number                            */
  uint8_t   bChannel;      /**< Channel number                           */
  uint8_t   bSequenceNr;   /**< Sequence Nr. Increased with every packet */
  uint8_t   bState;        /**< Transmission/Target state                */
  uint32_t  ulReserved;    /**< unused/reserved for future use           */
} HIL_TRANSPORT_HEADER, *PHIL_TRANSPORT_HEADER;
  

#endif /*  __HILTRANSPORT__H */
