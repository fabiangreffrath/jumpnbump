/*
 * network.c
 * Copyright (C) 1998 Brainchild Design - http://brainchilddesign.com/
 *
 * Copyright (C) 2001 Chuck Mason <cemason@users.sourceforge.net>
 *
 * Copyright (C) 2002 Florian Schulze <crow@icculus.org>
 *
 * Copyright (C) 2015 Côme Chilliet <come@chilliet.eu>
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "network.h"

static buggered_off = 0;
int is_server = 1;
int is_net = 0;
int server_said_bye = 0;

void processMovePacket(NetPacket *pkt)
{
	int playerid = pkt->arg;
	int movetype = ((pkt->arg2 >> 16) & 0xFF);
	int newval   = ((pkt->arg2 >>  0) & 0xFF);

	if (movetype == MOVEMENT_LEFT) {
		player[playerid].action_left = newval;
	} else if (movetype == MOVEMENT_RIGHT) {
		player[playerid].action_right = newval;
	} else if (movetype == MOVEMENT_UP) {
		player[playerid].action_up = newval;
	} else {
		printf("bogus MOVE packet!\n");
	}

	player[playerid].x = pkt->arg3;
	player[playerid].y = pkt->arg4;
}

void tellServerPlayerMoved(int playerid, int movement_type, int newval)
{
	NetPacket pkt;

	pkt.cmd = NETCMD_MOVE;
	pkt.arg = playerid;
	pkt.arg2 = ( ((movement_type & 0xFF) << 16) | ((newval & 0xFF) << 0) );
	pkt.arg3 = player[playerid].x;
	pkt.arg4 = player[playerid].y;

	if (is_server) {
		processMovePacket(&pkt);
#ifdef USE_NET
		if (is_net)
			sendPacketToAll(&pkt);
	} else {
		sendPacketToSock(sock, &pkt);
#endif /* USE_NET */
	}
}

void serverSendKillPacket(int killer, int victim)
{
	NetPacket pkt;

	assert(is_server);
	pkt.cmd = NETCMD_KILL;
	pkt.arg = killer;
	pkt.arg2 = victim;
	pkt.arg3 = player[victim].x;
	pkt.arg4 = player[victim].y;
	processKillPacket(&pkt);
#ifdef USE_NET
	if (is_net)
		sendPacketToAll(&pkt);
#endif
}

void processKillPacket(NetPacket *pkt)
{
	int c1 = pkt->arg;
	int c2 = pkt->arg2;
	int x = pkt->arg3;
	int y = pkt->arg4;
	int c4 = 0;
	int s1 = 0;

	player[c1].y_add = -player[c1].y_add;
	if (player[c1].y_add > -262144L)
		player[c1].y_add = -262144L;
	player[c1].jump_abort = 1;
	player[c2].dead_flag = 1;
	if (player[c2].anim != 6) {
		player[c2].anim = 6;
		player[c2].frame = 0;
		player[c2].frame_tick = 0;
		player[c2].image = player_anims[player[c2].anim].frame[player[c2].frame].image + player[c2].direction * 9;
		if (main_info.no_gore == 0) {
			for (c4 = 0; c4 < 6; c4++)
				add_object(OBJ_FUR, (x >> 16) + 6 + rnd(5), (y >> 16) + 6 + rnd(5), (rnd(65535) - 32768) * 3, (rnd(65535) - 32768) * 3, 0, 44 + c2 * 8);
			for (c4 = 0; c4 < 6; c4++)
				add_object(OBJ_FLESH, (x >> 16) + 6 + rnd(5), (y >> 16) + 6 + rnd(5), (rnd(65535) - 32768) * 3, (rnd(65535) - 32768) * 3, 0, 76);
			for (c4 = 0; c4 < 6; c4++)
				add_object(OBJ_FLESH, (x >> 16) + 6 + rnd(5), (y >> 16) + 6 + rnd(5), (rnd(65535) - 32768) * 3, (rnd(65535) - 32768) * 3, 0, 77);
			for (c4 = 0; c4 < 8; c4++)
				add_object(OBJ_FLESH, (x >> 16) + 6 + rnd(5), (y >> 16) + 6 + rnd(5), (rnd(65535) - 32768) * 3, (rnd(65535) - 32768) * 3, 0, 78);
			for (c4 = 0; c4 < 10; c4++)
				add_object(OBJ_FLESH, (x >> 16) + 6 + rnd(5), (y >> 16) + 6 + rnd(5), (rnd(65535) - 32768) * 3, (rnd(65535) - 32768) * 3, 0, 79);
		}
		dj_play_sfx(SFX_DEATH, (unsigned short)(SFX_DEATH_FREQ + rnd(2000) - 1000), 64, 0, 0, -1);
		player[c1].bumps++;
		player[c1].bumped[c2]++;
		s1 = player[c1].bumps % 100;
		add_leftovers(0, 360, 34 + c1 * 64, s1 / 10, &number_gobs);
		add_leftovers(1, 360, 34 + c1 * 64, s1 / 10, &number_gobs);
		add_leftovers(0, 376, 34 + c1 * 64, s1 - (s1 / 10) * 10, &number_gobs);
		add_leftovers(1, 376, 34 + c1 * 64, s1 - (s1 / 10) * 10, &number_gobs);
	}
}

#ifdef USE_NET

TCPsocket sock = NULL;
SDLNet_SocketSet socketset = NULL;

//~ NetInfo net_info[JNB_MAX_PLAYERS];

void bufToPacket(const char *buf, NetPacket *pkt)
{
	SDLNet_Write32(*((Uint32*) (buf +  0)), &pkt->cmd);
	SDLNet_Write32(*((Uint32*) (buf +  4)), &pkt->arg);
	SDLNet_Write32(*((Uint32*) (buf +  8)), &pkt->arg2);
	SDLNet_Write32(*((Uint32*) (buf + 12)), &pkt->arg3);
	SDLNet_Write32(*((Uint32*) (buf + 16)), &pkt->arg4);
/*
	pkt->cmd               =        ntohl(*((unsigned long *) (buf +  0)));
	pkt->arg               = (long) ntohl(*((unsigned long *) (buf +  4)));
	pkt->arg2              = (long) ntohl(*((unsigned long *) (buf +  8)));
	pkt->arg3              = (long) ntohl(*((unsigned long *) (buf + 12)));
	pkt->arg4              = (long) ntohl(*((unsigned long *) (buf + 16)));
*/
}


void packetToBuf(const NetPacket *pkt, char *buf)
{
	*((Uint32*) (buf +  0)) = SDLNet_Read32(&pkt->cmd);
	*((Uint32*) (buf +  4)) = SDLNet_Read32(&pkt->arg);
	*((Uint32*) (buf +  8)) = SDLNet_Read32(&pkt->arg2);
	*((Uint32*) (buf + 12)) = SDLNet_Read32(&pkt->arg3);
	*((Uint32*) (buf + 16)) = SDLNet_Read32(&pkt->arg4);
/*
	*((unsigned long *) (buf +  0)) = htonl(pkt->cmd);
	*((unsigned long *) (buf +  4)) = htonl((unsigned long) pkt->arg);
	*((unsigned long *) (buf +  8)) = htonl((unsigned long) pkt->arg2);
	*((unsigned long *) (buf + 12)) = htonl((unsigned long) pkt->arg3);
	*((unsigned long *) (buf + 16)) = htonl((unsigned long) pkt->arg4);
*/
}


void sendPacketToSock(TCPsocket s, NetPacket *pkt)
{
	int bytes_left = NETPKTBUFSIZE;
	int bw;
	char buf[NETPKTBUFSIZE];
	char *ptr = buf;

	packetToBuf(pkt, buf);
	while (bytes_left > 0) {
		bw = SDLNet_TCP_Send(s, ptr, bytes_left);
		if (bw < 0) {
			fprintf(stderr, "SERVER: SDLNet_TCP_Send(): %s\n", SDLNet_GetError());
			SDLNet_TCP_Close(s);
			exit(42);
		} else if (bw == 0) {
			SDL_Delay(1);
		} else {
			bytes_left -= bw;
			ptr += bw;
		}
	}
}


void sendPacket(int playerid, NetPacket *pkt)
{
	if ( (playerid < JNB_MAX_PLAYERS) && (playerid >= 0)) {
		if ((player[playerid].enabled) && (playerid != client_player_num)) {
			sendPacketToSock(net_info[playerid].sock, pkt);
		}
	}
}


void sendPacketToAll(NetPacket *pkt)
{
	int i;

	for (i = 0; i < JNB_MAX_PLAYERS; i++) {
		sendPacket(i, pkt);
	}
}

/** read a packet from the given TCPsocket
Returns -1 if some error occured, 0 if there was no data available and 1 if a
packet was successfully read.
Note: the socket has to be in the supplied socketset.
TODO: this function will bomb if a packet arrives in pieces, there is no
inherent guarantee that the next call will be made on the same socket. */
int grabPacket(TCPsocket s, SDLNet_SocketSet ss, NetPacket *pkt)
{
	static char buf[NETPKTBUFSIZE];
	static int buf_count = 0;
	int rc;

	if (SDLNet_CheckSockets(ss, 0) <= 0)
		return 0;

	if(!SDLNet_SocketReady(s))
		return 0;

	rc = SDLNet_TCP_Recv(s, &buf[buf_count], NETPKTBUFSIZE - buf_count);
	if (rc <= 0) {
		/* closed connection? */
		return -1;
	} else if (rc != NETPKTBUFSIZE) {
		/* we got a partial packet. Store what we got in the static buffer and
		return so that the next call can read the rest. Hopefully. */
		buf_count = rc;
		return 0;
	} else {
		buf_count = 0;
		bufToPacket(buf, pkt);
		return 1;
	}
}

int serverRecvPacket(NetPacket *pkt)
{
	int rc;
	int i;

	assert(is_server);

	for (i = 0; i < JNB_MAX_PLAYERS; i++) {
		TCPsocket s = net_info[i].sock;

		if ((i == client_player_num) || (!player[i].enabled))
			continue;

		rc = grabPacket(s, net_info[i].socketset, pkt);
		if (rc < 0) {
			NetPacket pkt;

			player[i].enabled = 0;
			SDLNet_TCP_Close(s);
			pkt.cmd = NETCMD_BYE;
			pkt.arg = i;
			pkt.arg2 = 0;
			pkt.arg3 = 0;
			pkt.arg4 = 0;
			sendPacketToAll(&pkt);
		} else if (rc > 0) {
			return(i);  /* it's all good. */
		}
	}

	return(-1);  /* no packets available currently. */
}

void wait_for_greenlight(void)
{
	NetPacket pkt;
	int i;

	printf("CLIENT: Waiting for greenlight...\n");

	do {
		int rc;
		while ((rc = grabPacket(sock, socketset, &pkt)) == 0) {
			SDL_Delay(100);  /* nap and then try again. */
		}

		if (rc < 0) {
			printf("CLIENT: Lost connection.\n");
			SDLNet_TCP_Close(sock);
			exit(42);
		}
	} while (pkt.cmd != NETCMD_GREENLIGHT);

	printf("CLIENT: Got greenlight.\n");

	for (i = 0; i < JNB_MAX_PLAYERS; i++) {
		if (pkt.arg & (1 << i)) {
			printf("CLIENT: There is a player #%d.\n", i);
			player[i].enabled = 1;
		}
	}
}

void tellServerGoodbye(void)
{
	NetPacket pkt;

	if (!buggered_off) {
		buggered_off = 1;
		pkt.cmd = NETCMD_BYE;
		pkt.arg = client_player_num;
		pkt.arg2 = 0;
		pkt.arg3 = 0;
		pkt.arg4 = 0;
		sendPacketToSock(sock, &pkt);
	}
}

void tellServerNewPosition(void)
{
	NetPacket pkt;
	pkt.cmd = NETCMD_POSITION;
	pkt.arg = client_player_num;
	pkt.arg2 = player[client_player_num].x;
	pkt.arg3 = player[client_player_num].y;

	if (is_server) {
		sendPacketToAll(&pkt);
	} else {
		sendPacketToSock(sock, &pkt);
	}
}

void processPositionPacket(NetPacket *pkt)
{
	int playerid = pkt->arg;

	player[playerid].x = pkt->arg2;
	player[playerid].y = pkt->arg3;
}

void processAlivePacket(NetPacket *pkt)
{
	int playerid = pkt->arg;

	player[playerid].dead_flag = 0;
	player[playerid].x = pkt->arg2;
	player[playerid].y = pkt->arg3;
}

void serverTellEveryoneGoodbye(void)
{
	int i;

	if (!buggered_off) {
		buggered_off = 1;
		for (i = 0; i < JNB_MAX_PLAYERS; i++) {
			if (player[i].enabled) {
				NetPacket pkt;

				pkt.cmd = NETCMD_BYE;
				pkt.arg = i;
				pkt.arg2 = 0;
				pkt.arg3 = 0;
				pkt.arg4 = 0;
				sendPacketToAll(&pkt);
			}
		}
	}
}

int update_players_from_server(void)
{
	NetPacket pkt;
	int rc;

	assert(!is_server);

	while ((rc = grabPacket(sock, socketset, &pkt)) != 0) {
		if (rc < 0) {
			printf("CLIENT: Lost connection.\n");
			pkt.cmd = NETCMD_BYE;
			pkt.arg = client_player_num;
		}

		if (pkt.cmd == NETCMD_BYE) {
			if (pkt.arg == client_player_num) {
				SDLNet_FreeSocketSet(socketset);
				SDLNet_TCP_Close(sock);
				sock = NULL;
				server_said_bye = 1;
				return(0);
			} else {
				player[pkt.arg].enabled = 0;
			}
		} else if (pkt.cmd == NETCMD_MOVE) {
			processMovePacket(&pkt);
		} else if (pkt.cmd == NETCMD_ALIVE) {
			processAlivePacket(&pkt);
		} else if (pkt.cmd == NETCMD_POSITION) {
			processPositionPacket(&pkt);
		} else if (pkt.cmd == NETCMD_KILL) {
			processKillPacket(&pkt);
		} else {
			printf("CLIENT: Got an unknown packet: 0x%lX.\n", pkt.cmd);
		}
	}

	return(1);
}

void serverSendAlive(int playerid)
{
	NetPacket pkt;

	assert(is_server);
	pkt.cmd = NETCMD_ALIVE;
	pkt.arg = playerid;
	pkt.arg2 = player[playerid].x;
	pkt.arg3 = player[playerid].y;
	sendPacketToAll(&pkt);
}

void update_players_from_clients(void)
{
	int i;
	NetPacket pkt;
	int playerid;

	assert(is_server);

	while ((playerid = serverRecvPacket(&pkt)) >= 0) {
		if (pkt.cmd == NETCMD_BYE) {
			pkt.arg = playerid;  /* just in case. */
			sendPacketToAll(&pkt);
			player[playerid].enabled = 0;
			SDLNet_FreeSocketSet(net_info[playerid].socketset);
			SDLNet_TCP_Close(net_info[playerid].sock);
		} else if (pkt.cmd == NETCMD_POSITION) {
			pkt.arg = playerid;  /* just in case. */
			processPositionPacket(&pkt);
			for (i = 0; i < JNB_MAX_PLAYERS; i++) {
				if (i != playerid) {
					sendPacket(i, &pkt);
				}
			}
		} else if (pkt.cmd == NETCMD_MOVE) {
			pkt.arg = playerid;  /* just in case. */
			/*
			pkt.arg3 = player[playerid].x;
			pkt.arg4 = player[playerid].y;
			*/
			processMovePacket(&pkt);
			sendPacketToAll(&pkt);
		} else {
			printf("SERVER: Got unknown packet (0x%lX).\n", pkt.cmd);
		}
	}
}

void init_server(const char *netarg)
{
	NetPacket pkt;
	IPaddress addr;
	int i;
	int wait_for_clients = ((netarg == NULL) ? 0 : atoi(netarg));
	char *ipstr;

	/** assign player number zero as default for the server */
	if(-1 == client_player_num)
		client_player_num = 0;

	if ((wait_for_clients >= JNB_MAX_PLAYERS) || (wait_for_clients < 0)) {
		printf("SERVER: Waiting for bogus client count (%d).\n", wait_for_clients);
		exit(42);
	}

	if (SDLNet_Init() < 0) {
		exit(42);
	}
	atexit(SDLNet_Quit);

	SDLNet_ResolveHost(&addr, NULL, JNB_INETPORT);
	ipstr = SDLNet_ResolveIP(&addr);
	SDLNet_ResolveHost(&addr, ipstr, JNB_INETPORT);
	printf("SERVER: we are %s (%i.%i.%i.%i:%i).\n", ipstr, (addr.host >> 0) & 0xff, (addr.host >> 8) & 0xff, (addr.host >> 16) & 0xff, (addr.host >> 24) & 0xff, addr.port);
	net_info[client_player_num].addr = addr;

	addr.host = INADDR_ANY;
	sock = SDLNet_TCP_Open(&addr);
	if (sock == NULL) {
		fprintf(stderr, "SERVER: SDLNet_TCP_Open(): %s\n", SDLNet_GetError());
		exit(42);
	}

	player[client_player_num].enabled = 1;

	printf("SERVER: waiting for (%d) clients...\n", wait_for_clients);

	socketset = SDLNet_AllocSocketSet(JNB_MAX_PLAYERS + 1);
	SDLNet_TCP_AddSocket(socketset, sock);

	while (wait_for_clients > 0)
	{
		char buf[NETPKTBUFSIZE];
		IPaddress *from;
		int negatory = 1;
		int br;
		TCPsocket s;

		/* Wait for events */
		SDLNet_CheckSockets(socketset, ~0);
		if ( SDLNet_SocketReady(sock) ) {
			s = SDLNet_TCP_Accept(sock);

			if (s == NULL)
			{
				fprintf(stderr, "SERVER: SDLNet_TCP_Accept(): %s", SDLNet_GetError());
				SDLNet_TCP_Close(sock);
				exit(42);
			}
		} else
			continue;

		br = SDLNet_TCP_Recv(s, buf, NETPKTBUFSIZE);
		if (br < 0) {
			fprintf(stderr, "SERVER: SDLNet_TCP_Recv(): %s\n", SDLNet_GetError());
			SDLNet_TCP_Close(s);
			SDLNet_TCP_Close(sock);
			exit(42);
		}

		from = SDLNet_TCP_GetPeerAddress(s);
		ipstr = SDLNet_ResolveIP(from);
		printf("SERVER: Got data from %s (%i.%i.%i.%i:%i).\n", ipstr, (from->host >> 0) & 0xff, (from->host >> 8) & 0xff, (from->host >> 16) & 0xff, (from->host >> 24) & 0xff, from->port);

		if (br != NETPKTBUFSIZE) {
			printf("SERVER: Bogus packet.\n");
			continue;
		}

		bufToPacket(buf, &pkt);
		if (pkt.cmd != NETCMD_HELLO) {
			printf("SERVER: Bogus packet.\n");
			continue;
		}

		printf("SERVER: Client claims to be player #%ld.\n", pkt.arg);

		if (-1 == pkt.arg) {
			int i;
			for(i=0; i!=JNB_MAX_PLAYERS; ++i) {
				if(!player[i].enabled) {
					printf("SERVER: assigning %d as player number\n", i);
					pkt.arg = i;
					break;
				}
			}
		}

		if ((pkt.arg>=JNB_MAX_PLAYERS)||(pkt.arg<0)) {
			printf("SERVER:  (that's an invalid player number.)\n");
		} else if (player[pkt.arg].enabled) {
			printf("SERVER:  (that player number is already taken.)\n");
		} else {
			negatory = 0;
		}

		if (negatory) {
			printf("SERVER: Forbidding connection.\n");
			pkt.cmd = NETCMD_NACK;
			sendPacketToSock(s, &pkt);
			SDLNet_TCP_Close(s);
		} else {
			player[pkt.arg].enabled = 1;
			net_info[pkt.arg].sock = s;
			net_info[pkt.arg].addr = *from;
			net_info[pkt.arg].socketset = SDLNet_AllocSocketSet(1);
			SDLNet_TCP_AddSocket(net_info[pkt.arg].socketset, net_info[pkt.arg].sock);
			wait_for_clients--;
			printf("SERVER: Granting connection. (%d) to go.\n", wait_for_clients);
			pkt.cmd = NETCMD_ACK;
			sendPacket(pkt.arg, &pkt);
		}
	}

	SDLNet_TCP_Close(sock);  /* done with the listen socket. */
	SDLNet_FreeSocketSet(socketset);
	sock = NULL;
	socketset = NULL;

	printf("SERVER: Got all our connections. Greenlighting clients...\n");

	pkt.cmd = NETCMD_GREENLIGHT;
	pkt.arg = 0;
	for (i = 0; i < JNB_MAX_PLAYERS; i++) {
		if (player[i].enabled) {
			pkt.arg |= (1 << i);
		}
	}
	sendPacketToAll(&pkt);
}

void connect_to_server(char *netarg)
{
	NetPacket pkt;
	char buf[NETPKTBUFSIZE];
	char *ipstr;
	IPaddress hent;
	IPaddress addr;
	int br;

	if (netarg == NULL) {
		printf("CLIENT: Need to specify host to connect to.\n");
		exit(42);
	}

	if (SDLNet_Init() < 0) {
		exit(42);
	}
	atexit(SDLNet_Quit);

	SDLNet_ResolveHost(&addr, NULL, JNB_INETPORT);
	ipstr = SDLNet_ResolveIP(&addr);
	SDLNet_ResolveHost(&addr, ipstr, JNB_INETPORT);
	printf("CLIENT: we are %s (%i.%i.%i.%i:%i).\n", ipstr, (addr.host >> 0) & 0xff, (addr.host >> 8) & 0xff, (addr.host >> 16) & 0xff, (addr.host >> 24) & 0xff, addr.port);

	if (SDLNet_ResolveHost(&hent, netarg, JNB_INETPORT) < 0) {
		fprintf(stderr, "CLIENT: couldn't find host: %s\n", SDLNet_GetError());
		exit(42);
	}

	sock = SDLNet_TCP_Open(&hent);
	if (sock == NULL) {
		fprintf(stderr, "CLIENT: SDLNet_TCP_Open(): %s\n", SDLNet_GetError());
		exit(42);
	}

	socketset = SDLNet_AllocSocketSet(1);
	SDLNet_TCP_AddSocket(socketset, sock);

	printf("CLIENT: connected to %s...\n", SDLNet_ResolveIP(&hent));

	printf("CLIENT: Sending HELLO packet...\n");
	pkt.cmd = NETCMD_HELLO;
	pkt.arg = client_player_num;
	sendPacketToSock(sock, &pkt);

	printf("CLIENT: Waiting for ACK from server...\n");

	br = SDLNet_TCP_Recv(sock, buf, NETPKTBUFSIZE);
	if (br < 0) {
		fprintf(stderr, "CLIENT: recv(): %s\n", SDLNet_GetError());
		SDLNet_FreeSocketSet(socketset);
		SDLNet_TCP_Close(sock);
		exit(42);
	}

	if (br != NETPKTBUFSIZE) {
		printf("CLIENT: Bogus packet size (%d of %d). FIXME.\n", br, NETPKTBUFSIZE);
		SDLNet_FreeSocketSet(socketset);
		SDLNet_TCP_Close(sock);
		exit(42);
	}

	bufToPacket(buf, &pkt);

	if (pkt.cmd == NETCMD_NACK) {
		printf("CLIENT: Server forbid us from playing.\n");
		SDLNet_FreeSocketSet(socketset);
		SDLNet_TCP_Close(sock);
		exit(42);
	}

	if (pkt.cmd != NETCMD_ACK) {
		printf("CLIENT: Unexpected packet (cmd=0x%lX).\n", pkt.cmd);
		SDLNet_FreeSocketSet(socketset);
		SDLNet_TCP_Close(sock);
		exit(42);
	}

	client_player_num = pkt.arg;
	player[client_player_num].enabled = 1;
	net_info[client_player_num].addr = addr;
	printf("CLIENT: Server accepted our connection.\n");

	wait_for_greenlight();
}

#endif /* USE_NET */
