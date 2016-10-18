/********************************************************************
 *
 *      File:   pkt_gen.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       UDP socket based packet transmit and rx library. 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

/*
 * Copyright (c) 2006 Nuova Systems, Inc.  All rights reserved.
 * $Id$
 *
 * Routines to interface with the network.
 */

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>

#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netpacket/packet.h>

#include <arpa/inet.h>

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define ETH_CRC_SIZE 		4
#define ERR	     		printf

// Sockets to communicate with cli, supervisors, and outside world.
static int s_inband = -1;


/********************************/
/* Static function declarations */
/********************************/

void network_append_crc_32(u_int8_t *data, size_t datalen);
void network_send(int s, struct sockaddr_in *dst_addr, void *data, size_t datalen);
int network_get_mtu(const char *int_name, int *mtu);
int network_set_mtu(const char *int_name, int mtu);

/*
 * Append a CRC-32 blob onto the end of the buffer.  We assume that there 
 * are 4 bytes of space free at the end of the buffer for this.
 */
#define CRCPOLY_LE 0xEDB88320
#define CRCPOLY	0x2083B8ED 

void network_append_crc_32(u_int8_t *data, size_t datalen)
{
    int i;
    u_int32_t *crc = (u_int32_t *)(data + datalen);

    *crc = ~0;
    while (datalen--) {
	*crc ^= *data++;
	for (i = 0; i < 8; i++)
	    *crc = (*crc >> 1) ^ ((*crc & 1) ? CRCPOLY : 0);
    }
    *crc = ~(*crc);
}

/*
 * Send data out of this socket.
 */
void
network_send(int s, struct sockaddr_in *dst_addr, void *data, size_t datalen)
{
    ssize_t sent_len;

    sent_len = sendto(s, data, datalen, 0, (struct sockaddr *)dst_addr, 
		      sizeof(*dst_addr));
    if (sent_len < 0) {
	ERR("could not send traffic: %s\n", strerror(errno));
	return;
    } else if ((size_t)sent_len != datalen) {
	printf("only %u out of %u bytes sent", sent_len, datalen);
	return;
    }
}

/*
 * Receive data on a socket.
 */
int
network_recv_data(int s, u_int8_t *data, size_t *datasize)
{
    ssize_t pktlen;

    /* Read data from fd */
    pktlen = recv(s, data, *datasize, 0);

    if (pktlen < 0) {
	if (errno == EAGAIN) {
	    /* Evidently can happen sometimes for non-blocking sockets ... */
	    return 0;
	} else {
	    ERR("could not receive traffic from supervisor: %s, pktlen=%d\n",
		strerror(errno), pktlen);
	    return -1;
	}
    }

    *datasize = pktlen;
    return 0;
}

/*
 * Grab an interface's MTU.
 */
int
network_get_mtu(const char *int_name, int *mtu)
{
    struct ifreq ifr;

    assert(mtu);

    /* Grab the ifindex corresponding to the interface name. */
    memset(&ifr, '\0', sizeof(ifr));
    (void)strncpy(ifr.ifr_name, int_name, sizeof(ifr.ifr_name));
    ifr.ifr_addr.sa_family = AF_INET;
    if (ioctl(s_inband, SIOCGIFMTU, &ifr) < 0) {
	ERR("could not get MTU from interface %s: %s",
	    int_name, strerror(errno));
	return -1;
    }

    *mtu = ifr.ifr_mtu;

    return 0;
}

/*
 * Set an interface's MTU.
 */
int
network_set_mtu(const char *int_name, int mtu)
{
    struct ifreq ifr;

    /* Grab the ifindex corresponding to the interface name. */
    memset(&ifr, '\0', sizeof(ifr));
    (void)strncpy(ifr.ifr_name, int_name, sizeof(ifr.ifr_name));
    ifr.ifr_addr.sa_family = AF_INET;
    ifr.ifr_mtu = mtu;
    if (ioctl(s_inband, SIOCSIFMTU, &ifr) < 0) {
	ERR("could not set MTU on interface %s: %s",
	    int_name, strerror(errno));
	return -1;
    }

    return 0;
}

/*
 * Actually bring interface for an external port up or down.
 */
#if 0
static int network_interface_up(const char *int_name, int extport, int up);
static int
network_interface_up(const char *int_name, int extport, int up) 
{
    struct ifreq ifr;
    int op_up;

    assert(int_name);

    /* Grab the ifindex corresponding to the interface name. */
    memset(&ifr, 0, sizeof(ifr));
    (void)strncpy(ifr.ifr_name, int_name, sizeof(ifr.ifr_name));
    if (ioctl(s_ioctl, SIOCGIFFLAGS, &ifr) < 0) {
	ERR("could not get interface state from interface name %s: %s\n",
	    int_name, strerror(errno));
	return -1;
    }

    op_up = ifr.ifr_flags & IFF_UP;

    if (up && !op_up) {
	DEBUG3("Bringing interface %s up\n", int_name);
	ifr.ifr_flags |= IFF_UP;
    } else if (!up && op_up) {
	DEBUG3("Bringing interface %s down\n", int_name);
	ifr.ifr_flags &= ~IFF_UP;
    } else {
	/* Nothing to do. */
	DEBUG3("Interface %s is already %s; nothing to do\n", 
		int_name, up ? "up" : "down");
	return 0;
    }

    if (ioctl(s_ioctl, SIOCSIFFLAGS, &ifr) < 0) {
	ERR("could not set interface state from interface name %s: %s\n",
	    int_name, strerror(errno));
	return -1;
    }

    if (extport) {
	if (!up) {
	    /*
	     * We brought a link down.  We now need to close the socket 
	     * corresponding to this external port.
	     */
	    if (extport_close_socket(int_name) < 0) {
		ERR("could not close socket on interface %s\n", int_name);
		return -1;
	    }
	} else {
	    /*
	     * We brought a link up.  We now need to create and re-bind a
	     * socket to this external port.
	     */
	    if (extport_open_socket(int_name) < 0) {
		ERR("could not open socket on interface %s\n", int_name);
		return -1;
	    }
	}
    }

    return 0;
}
#endif

/*
 * Create and bind a raw socket to an interface for subsequent packet
 * operations.
 *
 * ARGUMENTS:
 * const char *int_name - interface name to bind to
 *
 * RETURN VALUE:
 * (int) - file descriptor corresponding to socket if success, -1 otherwise
 */
extern int diag_debug;
int
network_rawsocket_init(const char *int_name)
{
    struct ifreq ifr;
    struct packet_mreq mr;
    struct sockaddr_ll saddr;
    int ifindex = 0, intf=0, optlen = sizeof(int);
static    int raw_s[2] = {-1, -1};

    intf =  (!strncasecmp(int_name, "eth0", strlen("eth0"))) ? 0 : 1;
    if (raw_s[intf] != -1) return (raw_s[intf]);

    raw_s[intf] = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL) );
    if (raw_s[intf] < 0) {
	ERR("could not open socket: %s\n", strerror(errno));
	return -1;
    }
#if 1
    {
    unsigned int bufsize;
    int on =1;
#if 0
    if (setsockopt(s, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0) {
        ERR("could not set IP_HDRINCL: %s\n", strerror(errno));
        //goto err; // may have to tolerate this...
    }
#endif
    /* Make socket buffers very large */
    if (getsockopt(raw_s[intf], SOL_SOCKET, SO_SNDBUF, &bufsize, &optlen) < 0) {
	ERR("could not set send buffer size: %s\n", strerror(errno));
	goto err;
    } 
    if (bufsize < 104448) {
        bufsize = 104448;
        if (setsockopt(raw_s[intf], SOL_SOCKET, SO_SNDBUF, &bufsize, 
                       sizeof(bufsize)) < 0) {
	    ERR("could not set send buffer size: %s\n", strerror(errno));
	    goto err;
        }
    }
    if (getsockopt(raw_s[intf], SOL_SOCKET, SO_RCVBUF, &bufsize, &optlen) < 0) {
	ERR("could not set send buffer size: %s\n", strerror(errno));
	goto err;
    }
    if (bufsize < 104448) {
        bufsize = 104448;
        if (setsockopt(raw_s[intf], SOL_SOCKET, SO_RCVBUF, &bufsize, 
                       sizeof(bufsize)) < 0) {
	    ERR("could not set send buffer size: %s\n", strerror(errno));
	    goto err;
        }
    }

    /* Make socket non-blocking */
    on = 1;
    if (ioctl(raw_s[intf], FIONBIO, &on) < 0) {
	ERR("could not set socket to non-blocking: %s\n", strerror(errno));
	goto err;
    }
    }
#endif /*0*/
    /* Bind the socket */
    memset(&ifr, 0, sizeof (ifr));
    strncpy(ifr.ifr_name, int_name, sizeof (ifr.ifr_name));
    if (ioctl(raw_s[intf], SIOGIFINDEX, &ifr) < 0) {
	ERR("could not get interface index for %s: %s\n", int_name, 
	    strerror(errno));
	goto err;
    }
    ifindex = ifr.ifr_ifindex;

    memset(&saddr, 0, sizeof(saddr));
    saddr.sll_family = AF_PACKET;
    saddr.sll_ifindex = ifindex;
    if (bind(raw_s[intf], (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
	ERR("could not bind socket: %s\n", strerror(errno));
	goto err;
    }

    /* Make the socket promiscuous */
    memset(&mr, 0, sizeof (mr));
    mr.mr_ifindex = ifindex;
    mr.mr_type = PACKET_MR_PROMISC;
    if (setsockopt(raw_s[intf], SOL_PACKET, PACKET_ADD_MEMBERSHIP, 
		   &mr, sizeof (mr)) < 0) {
	ERR("could not make socket promiscuous: %s\n", strerror(errno));
	goto err;
    }

    if (diag_debug) {
        printf("successfully set up raw socket %d on interface %s\n", 
	   raw_s[intf], int_name);
    }

    return (raw_s[intf]);

err:
    close(raw_s[intf]);
    return -1;
}

/*
 * Send a packet out of an external port.
 */
void
network_send_out(int s, void *data, size_t datalen, int fc_sof, int fc_eof)
{
    size_t sent_len;

	/*
	 * This is inband traffic from a Gatos.
	 * We will have to strip off the CRC before passing it to the 
	 * driver, as the Ethernet MAC will automatically add one on.
 	 * There's no way to make the e1000 driver not strip the CRC, so oh 
	 * well.
	 */
    assert(datalen > ETH_CRC_SIZE);
//    datalen -= ETH_CRC_SIZE;

    /* Send packet out of interface. */
    sent_len = send(s, data, datalen, 0);
    if (sent_len < 0) {
	ERR("could not send traffic: %s\n", strerror(errno));
	return;
    } else if (sent_len != datalen) {
	ERR("only %u out of %u bytes sent [errno: %d, %s]\n", sent_len, datalen, errno,strerror(errno));
	return;
    }
}

/*
 * Grab an IP address.
 */
int
network_get_ipaddr(const char *int_name, struct in_addr *saddr)
{
    struct ifreq ifr;

    assert(saddr);

    /* Grab the ifindex corresponding to the interface name. */
    memset(&ifr, '\0', sizeof(ifr));
    (void)strncpy(ifr.ifr_name, int_name, sizeof(ifr.ifr_name));
    ifr.ifr_addr.sa_family = AF_INET;
    if (ioctl(s_inband, SIOCGIFADDR, &ifr) < 0) {
	if (errno == EADDRNOTAVAIL) {
	    /* If there is no address currently assigned ... */
	    saddr->s_addr = 0;
	    return 0;
	}
        ERR("could not get IP address from interface %s: %s",
            int_name, strerror(errno));
        return -1;
    }

    *saddr = ((struct sockaddr_in *)(&ifr.ifr_addr))->sin_addr;
    saddr->s_addr = ntohl(saddr->s_addr);

    return 0;
}

/*
 * Set an IP address.
 */
int
network_set_ipaddr(const char *int_name, struct in_addr *saddr)
{
    struct ifreq ifr;
    struct sockaddr_in *sin_addr;

    assert(saddr);

    /* Grab the ifindex corresponding to the interface name. */
    memset(&ifr, '\0', sizeof(ifr));
    (void)strncpy(ifr.ifr_name, int_name, sizeof(ifr.ifr_name));
    ifr.ifr_addr.sa_family = AF_INET;
    sin_addr = (struct sockaddr_in *)&ifr.ifr_addr;
    sin_addr->sin_addr.s_addr = saddr->s_addr;
    if (ioctl(s_inband, SIOCSIFADDR, &ifr) < 0) {
	ERR("could not set IP address on interface %s: %s",
	    int_name, strerror(errno));
	return -1;
    }

    return 0;
}

/*
 * Get the MAC address of our inband interface.
 */
int
network_get_inband_mac_address(u_int8_t *mac_addr, char *ifname)
{
    struct ifreq ifr;

    /* Grab the ifindex corresponding to the interface name. */
    memset(&ifr, '\0', sizeof(ifr));
    (void)strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));
    if (ioctl(s_inband, SIOCGIFHWADDR, &ifr) < 0) {
	ERR("could not get hardware address from interface name %s: %s",
	    ifname, strerror(errno));
	return -1;
    }

    memcpy(mac_addr, &ifr.ifr_hwaddr.sa_data[0], ETH_ALEN);

    return 0;
}

#if 0
/*
 * Bring interface up/down.  This is a hard up/down (laser on/off), not 
 * just a link-level up/down.  This is what an MDS does for 
 * "no shutdown/shutdown" so we'll do the same.  And for Ethernet, this 
 * is the only way to do this.
 */
int
network_interface_enable(u_int8_t slot_id, u_int8_t port_id, int up)
{
    const char *int_name;

    int_name = extport_get_by_slot_port(slot_id, port_id, NULL);
    if (int_name) {
	DEBUG3("executing %s for interface %s (slot %hhu, port %hhu)\n", 
	       (up ? "up" : "down"), int_name, slot_id, port_id);
	if (network_interface_up(int_name, 1, up) < 0) {
	    return -1;
        }

	extport_set_admin_status(slot_id, port_id, up, NULL);
    } else {
	ERR("could not get interface state for slot %u port %u\n",
	    slot_id, port_id);
	return -1;
    }

    return 0;
}
#endif /*0*/

/* Check if a particular interface has gone operationally up/down */
int
network_interface_op_check(const char *int_name)
{
    struct ifreq ifr;

    assert(int_name);

    /* Grab the ifindex corresponding to the interface name. */
    memset(&ifr, 0, sizeof(ifr));
    (void)strncpy(ifr.ifr_name, int_name, sizeof(ifr.ifr_name));
    if (ioctl(s_inband, SIOCGIFFLAGS, &ifr) < 0) {
	ERR("could not get interface state from interface name %s: %s\n",
	    int_name, strerror(errno));
	return -1;
    }

    if (ifr.ifr_flags & IFF_RUNNING) {
	return 1;
    } else {
	return 0;
    }

    /* NOTREACHED */
}

/* Check if an interface exists on the system. */
int
network_interface_exists(const char *int_name)
{
    struct ifreq ifr;

    /* Grab the ifindex corresponding to the interface name. */
    memset(&ifr, '\0', sizeof(ifr));
    (void)strncpy(ifr.ifr_name, int_name, sizeof(ifr.ifr_name));
    if (ioctl(s_inband, SIOCGIFINDEX, &ifr) < 0) {
        if (errno != ENODEV) {
	    ERR("check if interface %s exists failed unexpectedly: %s",
		int_name, strerror(errno));
	}
        return -1;
    }

    return 0;
}
