/*
 * network.h
 * Copyright (C) 1998 Brainchild Design - http://brainchilddesign.com/
 *
 * Copyright (C) 2001 Chuck Mason <cemason@users.sourceforge.net>
 *
 * Copyright (C) 2002 Florian Schulze <crow@icculus.org>
 *
 * This file is part of Jump'n'Bump.
 *
 * Jump'n'Bump is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Jump'n'Bump is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "globals.h"
#ifdef USE_NET
#include "SDL_net.h"
#endif /* USE_NET */

/* networking shite. */

#define NETPKTBUFSIZE (4 + 4 + 4 + 4 + 4)

#define NETCMD_NACK         (0xF00DF00D + 0)
#define NETCMD_ACK          (0xF00DF00D + 1)
#define NETCMD_HELLO        (0xF00DF00D + 2)
#define NETCMD_GREENLIGHT   (0xF00DF00D + 3)
#define NETCMD_MOVE         (0xF00DF00D + 4)
#define NETCMD_BYE          (0xF00DF00D + 5)
#define NETCMD_POSITION     (0xF00DF00D + 6)
#define NETCMD_ALIVE        (0xF00DF00D + 7)
#define NETCMD_KILL         (0xF00DF00D + 8)

extern int is_server;
extern int is_net;
extern int server_said_bye;

typedef struct
{
	Uint32 cmd;
	Sint32 arg;
	Sint32 arg2;
	Sint32 arg3;
	Sint32 arg4;
} NetPacket;

void processMovePacket(NetPacket *pkt);

void tellServerPlayerMoved(int playerid, int movement_type, int newval);

#ifdef USE_NET

extern TCPsocket sock;
extern SDLNet_SocketSet socketset;

typedef struct
{
	TCPsocket sock;
	IPaddress addr;
	SDLNet_SocketSet socketset;
} NetInfo;

NetInfo net_info[JNB_MAX_PLAYERS];

void bufToPacket(const char *buf, NetPacket *pkt);

void packetToBuf(const NetPacket *pkt, char *buf);

void sendPacketToSock(TCPsocket s, NetPacket *pkt);

void sendPacket(int playerid, NetPacket *pkt);

void sendPacketToAll(NetPacket *pkt);

/** read a packet from the given TCPsocket
Returns -1 if some error occured, 0 if there was no data available and 1 if a
packet was successfully read.
Note: the socket has to be in the supplied socketset.
TODO: this function will bomb if a packet arrives in pieces, there is no
inherent guarantee that the next call will be made on the same socket. */
int grabPacket(TCPsocket s, SDLNet_SocketSet ss, NetPacket *pkt);

int serverRecvPacket(NetPacket *pkt);

void wait_for_greenlight(void);

void tellServerGoodbye(void);

void tellServerNewPosition(void);

void processKillPacket(NetPacket *pkt);

void processPositionPacket(NetPacket *pkt);

void processAlivePacket(NetPacket *pkt);

void serverTellEveryoneGoodbye(void);

int update_players_from_server(void);

void serverSendAlive(int playerid);

void serverSendKillPacket(int killer, int victim);

void update_players_from_clients(void);

void init_server(const char *netarg);

void connect_to_server(char *netarg);

#endif /* USE_NET */

#endif /* __NETWORK_H__ */
