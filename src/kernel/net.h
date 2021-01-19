#ifndef KERNEL_NET_STACK_H
#define KERNEL_NET_STACK_H

struct netpack 
{
	int type;
	union 
	{
		struct ip_netpack ip;
		struct tcp_netpack ip;
		struct udp_netpack ip;
	}

	int refs;
	char data[];
};

struct netpack * allocate_net_package(int size);
void deallocate_net_package(struct netpack *);

void init_netpack(struct netpack * pack);

#endif