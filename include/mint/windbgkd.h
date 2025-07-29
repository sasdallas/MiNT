/**
 * @file include/mint/windbgkd.h
 * @brief Defines the kernel debugger structures
 * 
 * 
 * @copyright
 * This file is part of the MiNT operating system.
 * Please see the LICENSE file in the main repository for more details.
 * 
 * Copyright (C) 2025 The MiNT development team
 */

#include <HeaderBase.h>

#ifndef __MINT_WINDBGKD_H__
#define __MINT_WINDBGKD_H__ 1

__MINT_BEGIN_DECLS

#include <ntdef.h>

#define PACKET_LEADER           0x30303030
#define CONTROL_PACKET_LEADER   0x69696969

typedef enum {
    KdPacketReceived = 0,
    KdPacketTimedOut,
    KdPacketNeedsResend,
} KDSTATUS;

typedef struct _KD_CONTEXT {
    ULONG   BreakCount;
    BOOL    BreakInRequested;
} KD_CONTEXT, *PKD_CONTEXT;

typedef struct _KD_PACKET_HEADER {
    ULONG Signature;
    USHORT PacketType;
    USHORT TotalDataLength;
    ULONG PacketID;
    ULONG Checksum;
} KD_PACKET_HEADER, *PKD_PACKET_HEADER;

enum
{
    KdPacketType3 = 3,
    KdPacketAcknowledge = 4,
    KdPacketRetryRequest = 5,
    KdPacketResynchronize = 6,
    KdPacketType7 = 7,
    KdCheckForAnyPacket = 8,
    KdPacketType11 = 11,
};


#define PACKET_TYPE_UNUSED 0
#define PACKET_TYPE_KD_STATE_CHANGE32 1
#define PACKET_TYPE_KD_STATE_MANIPULATE 2
#define PACKET_TYPE_KD_DEBUG_IO 3
#define PACKET_TYPE_KD_ACKNOWLEDGE 4
#define PACKET_TYPE_KD_RESEND 5
#define PACKET_TYPE_KD_RESET 6
#define PACKET_TYPE_KD_STATE_CHANGE64 7
#define PACKET_TYPE_KD_POLL_BREAKIN 8
#define PACKET_TYPE_KD_TRACE_IO 9
#define PACKET_TYPE_KD_CONTROL_REQUEST 10
#define PACKET_TYPE_KD_FILE_IO 11
#define PACKET_TYPE_MAX 12

#define DbgKdPrintStringApi         0x00003230
#define DbgKdLoadSymbolsStateChange 0x00003031

__MINT_END_DECLS

#endif