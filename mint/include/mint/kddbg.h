/**
 * @file mint/include/mint/kddbg.h
 * @brief Kernel debugger header
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system
 * However, it has been released into the public domain and is free to use
 * This file comes with no warranty.
 */

#pragma once
#ifndef _KDDBG_H
#define _KDDBG_H

/** Includes **/
#include <WinDef.h>


/** Types **/

/* Debugger packet */
typedef struct _KD_PACKET
{
    ULONG Signature; /* Signature, see packet signatures */
    USHORT PacketType; /* Type of te packet */
    USHORT DataLength; /* Data length */
    ULONG PacketID; /* 0x80800800 on initial, 0x80800000 on resync */
    ULONG Checksum; /* Sum of all data bytes */
} KD_PACKET, *PKD_PACKET;


/* Packet signatures */
#define KD_PACKET_DATA_SIGNATURE 0x30303030 /* 0000 */
#define KD_PACKET_CTRL_SIGNATURE 0x69696969 /* iiii */


/* Packet types */
#define KD_PACKET_TYPE_UNUSED 0
#define KD_PACKET_TYPE_STATE_CHANGE32 1
#define KD_PACKET_TYPE_STATE_MANIPULATE 2
#define KD_PACKET_TYPE_DEBUG_IO 3
#define KD_PACKET_TYPE_ACKNOWLEDGE 4
#define KD_PACKET_TYPE_RESEND 5
#define KD_PACKET_TYPE_RESET 6
#define KD_PACKET_TYPE_STATE_CHANGE64 7
#define KD_PACKET_TYPE_POLL_BREAKIN 8
#define KD_PACKET_TYPE_TRACE_IO 9
#define KD_PACKET_TYPE_CONTROL_REQUEST 10
#define KD_PACKET_TYPE_FILE_IO 11
#define KD_PACKET_TYPE_MAX 12

/* KDSTATUS */
typedef ULONG KDSTATUS;

/* Return values of KdReceivePacket */
#define KdPacketReceived    0
#define KdPacketTimedOut    1
#define KdPacketResend      2


/* KD_CONTEXT */
/* Maintains global KD packet layer state */
typedef struct _KD_CONTEXT
{
    ULONG RetryCount;
    BOOLEAN BreakInRequested; 
} KD_CONTEXT, *PKD_CONTEXT;



#endif