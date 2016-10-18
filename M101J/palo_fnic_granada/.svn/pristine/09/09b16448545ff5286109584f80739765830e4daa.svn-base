/********************************************************************
 *
 *      File:   pkt_que.c
 *      Name:   Sudharshan Kadari
 *
 *      Description:
 *       Receive packets from the intf and queue it. 
 *
 *
 * Copyright (c) 1985-2007 by Cisco Systems, Inc.
 * All rights reserved.
 *
 *
 *********************************************************************/

#include <stdio.h>
#include <stdint.h> 
#include <string.h> 
#include <strings.h> 
#include <malloc.h> 
#include <sys/types.h> 
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> 
#include <errno.h> 
#include <fcntl.h> 
#include "pkt_ge.h"
#include "pkt_ops.h"
#include <sched.h>
#include <sys/ioctl.h>

#define SHM_SIZE (100 * DIAG_MAX_PACKET_SIZE)

static int   pkt_eth1[2] = {-1, -1};
static int   pkt_eth0[2] = {-1, -1};

extern void exit(int);
typedef struct _pkt_q_ pkt_q_t;

struct _pkt_q_ {
	uint8_t *buf;
	uint32_t len;
	pkt_q_t  *next;
};

pkt_q_t *pkt_head[2] = {NULL, NULL};
pkt_q_t *pkt_tail[2] = {NULL, NULL};


int pkt_shm_wr(char *fname, int intf_num)
{
    key_t key;
    int shmid;
    int cnt;
    char *pshm;
    int   count = pkt_queue_count(intf_num);
    pkt_q_t *phead = pkt_head[intf_num];
    pkt_q_t *ptemp = NULL;
    char *pbuf = NULL;
 
    if ((key = ftok(fname, 'R')) == -1) {
        perror("ftok");
        return(-1);
    }

    /* connect to (and possibly create) the segment: */
    if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        return(-1);
    }

    // Attach the shared memory.
    pshm = shmat(shmid, NULL, 0);
    if (pshm <= 0) {
	perror("shmat");
        return (-1);
    }

    // dump all packets to the shm.
    ptemp = malloc(count * sizeof(pkt_q_t));    
    if (!ptemp) {
         printf("Error: %s:%d Failed to allocate\n", __func__, __LINE__);
	 return (-1);
    }
    memset(ptemp, 0, count*sizeof(pkt_q_t));
    
    // create the data structure.
    pbuf = pshm + count*sizeof(pkt_q_t);
    for (phead = pkt_head[intf_num], cnt = 0; phead && cnt < count; cnt++) {
	ptemp[cnt].len = phead->len;
	ptemp[cnt].buf = pbuf; 
	ptemp[cnt].next = (cnt+1 == count) ? NULL : &ptemp[cnt+1]; 

        pbuf += phead->len;
        phead = phead->next;
    } 

    memcpy(pshm, ptemp, count*sizeof(pkt_q_t));
    free(ptemp);

    pshm += count*sizeof(pkt_q_t);
    for (phead = pkt_head[intf_num], cnt = 0; phead && cnt < count; cnt++) {
	memcpy(pshm, phead->buf, phead->len);
	pshm += phead->len;
    }
    return (0);
}

int pkt_shm_dump(char *fname)
{
    key_t key;
    int shmid;
    int cnt;
    char *pshm;
    pkt_q_t *ppkt;

    if ((key = ftok(fname, 'R')) == -1) {
        perror("ftok");
        return(-1);
    }

    /* connect to (and possibly create) the segment: */
    if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        return(-1);
    }

    // Attach the shared memory.
    pshm = shmat(shmid, NULL, 0);
    if (pshm <= 0) {
        perror("shmat");
        return (-1);
    }
    ppkt = (pkt_q_t*)pshm;
    while (ppkt) {
          printf("ppkt=%p, len=%d, buf=%p, next=%p\n",
			ppkt, ppkt->len, ppkt->buf, ppkt->next);	
	  if (ppkt->next == NULL)
		break;
	  ppkt++;
    }

    ppkt = (pkt_q_t*)pshm;
    for (cnt = 0; cnt < 5; cnt++) {
	print_pkt(ppkt[cnt].buf, ppkt[cnt].len);
	printf("=============================================\n\n");
    }
    return (0);
}

int pkt_shm_rd(char *fname, uint8_t *pkt, uint32_t pcnt, uint32_t max_len)
{
    key_t key;
    int shmid;
    uint32_t cnt = 0;
    char *pshm;
    pkt_q_t *ppkt;

    if ((key = ftok(fname, 'R')) == -1) {
        perror("ftok");
        return(-1);
    }

    /* connect to (and possibly create) the segment: */
    if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        return(-1);
    }

    // Attach the shared memory.
    pshm = shmat(shmid, NULL, 0);
    if (pshm <= 0) {
        perror("shmat");
        return (-1);
    }

    ppkt = (pkt_q_t*)pshm;
    cnt = 0;
    while (ppkt) {
	  if (ppkt->next == NULL) 
		break;
        ppkt++;
        if (cnt == pcnt) { 
          memcpy(pkt, ppkt->buf, (max_len > ppkt->len) ? ppkt->len : max_len);
	  return (max_len > ppkt->len ? ppkt->len : max_len); 
        }
        cnt++;
    }

    return (0);
}

int pkt_shm_rm(char *fname)
{
    key_t key;
    int shmid;

    if ((key = ftok(fname, 'R')) == -1) {
        perror("ftok");
        return(-1);
    }

    /* connect to (and possibly create) the segment: */
    if ((shmid = shmget(key, SHM_SIZE, 0644 | IPC_CREAT)) == -1) {
        perror("shmget");
        return(-1);
    }

    shmctl(shmid, IPC_RMID, NULL);
    return (0);
}


int pkt_enqueue(int intf_num, uint8_t *pkt, uint32_t len)
{
	pkt_q_t *pq = NULL;;

	pq = malloc(sizeof(pkt_q_t));
	if (pq == NULL) {
		printf("Error: Failed to allocate Memory\n");
		return (-1);
	}	
	
	pq->buf = pkt;
	pq->len = len;
	pq->next = NULL;	

	if (pkt_head[intf_num] == NULL) {
		pkt_head[intf_num] = pq;
		pkt_tail[intf_num] = pq;
	} else {
		pkt_tail[intf_num]->next = pq;
		pkt_tail[intf_num] = pq;
	}

	printf("Eth%d Hd=%p, Tail=%p\n", intf_num, pkt_head[intf_num], 
			pkt_tail[intf_num]);
	return (0);
}

// Dequeue the head
int pkt_dequeue_head(int intf_num, int all)
{
	pkt_q_t *pq = pkt_head[intf_num];;

	pkt_head[intf_num] = pq->next;

	// Free the packet buffer.
	free(pq->buf);

	// Free the data structure.
	free(pq);

	printf("Eth%d Hd=%p, Tail=%p\n", intf_num, pkt_head[intf_num], 
			pkt_tail[intf_num]);
	return (0);
}

// Number of packets in the queue.
int pkt_queue_count(int intf_num)
{
	pkt_q_t *pq = pkt_head[intf_num];;
	int	cnt = 0;

	while(pq) {
		cnt++;
		pq = pq->next;
	}

	return (cnt);
}

// Display the packets.
void pkt_queue_dump (int intf_num)
{
	pkt_q_t *pq = pkt_head[intf_num];

	while (pq) {
		printf("%p: pkt(%p) len(%d)\n", pq, pq->buf, pq->len);
		print_pkt(pq->buf, pq->len);
		pq = pq->next;	
	}
}

// Flush the packets.
void pkt_queue_flush (int intf_num)
{
	pkt_q_t *pq = pkt_head[intf_num];;
	pkt_q_t *temp;

	pkt_head[intf_num] = NULL;
	pkt_tail[intf_num] = NULL;

	while(pq) {
		temp = pq;	
		pq = pq->next;

		free (temp->buf);
		free (temp);
	}
}

static int pkt_belongs_to_diag (uint8_t *r_mac, uint8_t *s_mac)
{
	return ((0xFFFFFFFFFFFF0000ull & *(uint64_t *)s_mac) == 
	   (0xFFFFFFFFFFFF0000ull & *(uint64_t*)r_mac)) ? 1 : 0;
}

extern int diag_debug;
// Receive and Enqueue Packets.
int pkt_queue_recv (int intf_num) 
{
	uint8_t *rxpkt = NULL;
	uint32_t psize = DIAG_MAX_PACKET_SIZE;
	int	 raw_s, rc, *pkt_eth = intf_num ? pkt_eth1 : pkt_eth0;
        uint8_t  s_mac[6] = {0x00,0x04,0x9F,0x00,0x39,0x8E};

#if 0
	network_get_inband_mac_address(s_mac, intf_num ? "eth1" : "eth0");
        printf("%s:%d intf %s\n", __func__, __LINE__, intf_num ? "eth1" : "eth0");
#endif
	raw_s = network_rawsocket_init(intf_num ? "eth1" : "eth0");
	if (raw_s < 0) {
		printf("Error: Failed to initialize the socket\n");
	}

	// Start Clean.
	pkt_queue_flush(intf_num);

	printf(" Starting Packet Rx process for Eth%d\n", intf_num);	
	while (1) {
		psize = DIAG_MAX_PACKET_SIZE;
		rxpkt = malloc(psize);
		if (rxpkt == NULL) {
			printf("Error: Failed to allocate memory\n");
			return (-1);
		}	

		rc = network_recv_data(raw_s, rxpkt, &psize);
		if (rc) {
			free(rxpkt);
			printf("Error: Failed to receive packet\n");
			return (rc);
		} 

		// Check Mac Address to verify its diag packet.
		if (pkt_belongs_to_diag(rxpkt+6, s_mac)) {
			if ((psize >= 64) && (psize < DIAG_MAX_PACKET_SIZE)) {
				// write it to the socket.
				if (diag_debug)
				printf("Sending packet on stream socket %d\n", psize);
				if (write(pkt_eth[0], rxpkt, psize) < 0) {
					perror("Writing stream  message\n");
				} 
			}
		} else {
//			printf("Unexpected packet Seen from host %02X:%02X:%02X:%02X:%02X:%02X\n",
//				rxpkt[6], rxpkt[7], rxpkt[8], rxpkt[9], rxpkt[10], rxpkt[11]);
			free(rxpkt);
		}
	}	

	return (0);
}

static int pkt_rx_eth(uint8_t intf_num, uint8_t *rx_pkt, uint32_t psize)
{
	int len = 0;

	int *pkt_eth = intf_num ? pkt_eth1 : pkt_eth0;

	len = read(pkt_eth[1], rx_pkt, psize);
	if (len < 0) {
		printf("errno = %d : %s\n", errno, strerror(errno));
		return((errno == EAGAIN) ? 0 : -1);
	} else {
		print_pkt(rx_pkt, len);
	}

	return (len);
}

int pkt_rx_eth1(uint8_t *rx_pkt, uint32_t psize)
{
	return (pkt_rx_eth(1, rx_pkt, psize));
}
int pkt_rx_eth0(uint8_t *rx_pkt, uint32_t psize)
{
	return (pkt_rx_eth(0, rx_pkt, psize));
}

int pkt_rx_proc_init(int intf_num)
{
	pid_t pid;
	int *pkt_eth = intf_num ? pkt_eth1 : pkt_eth0;
	int on = 1;
    unsigned int bufsize;

	if (socketpair(AF_UNIX, SOCK_STREAM, 0, pkt_eth) < 0) {
		perror("opening stream socket pair");
		exit(1);
	}

    // 7 redwoods, 12 ports, 30 packets/port, 1500 bytes/packet
    bufsize = (7 * 12 * 10 * 1500);
    if (setsockopt(pkt_eth[0], SOL_SOCKET, SO_SNDBUF, &bufsize, sizeof(bufsize)) < 0) {
	printf("could not set send buffer size: %s\n", strerror(errno));
	exit(1);
    }
    if (setsockopt(pkt_eth[1], SOL_SOCKET, SO_RCVBUF, &bufsize, sizeof(bufsize)) < 0) {
	printf("could not set send buffer size: %s\n", strerror(errno));
	exit(1);
    }
	// set non blocking the old way.
	ioctl(pkt_eth[0], FIONBIO, &on);

	// set non blocking the old way.
	ioctl(pkt_eth[1], FIONBIO, &on);

	// Child Process.	
	if ((pid = fork()) < 0) {
		perror("fork");
	} else if (pid) {			// This is parent.
		close(pkt_eth[0]);
	} else {
		close(pkt_eth[1]);
		pkt_queue_recv(intf_num);
	}
	return (0);
}
