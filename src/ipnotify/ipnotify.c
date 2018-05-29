
#include <errno.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#ifdef DEBUG
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug(...) 
#endif

static void print_rtm_newaddr(struct nlmsghdr *h)
{
	struct ifaddrmsg *ifa = (struct ifaddrmsg *)NLMSG_DATA(h);
	struct rtattr *rth = IFA_RTA(ifa);
	int rtl = IFA_PAYLOAD(h);

	while (rtl && RTA_OK(rth, rtl)) {
		debug("rth->rta_type = %d\n", rth->rta_type);
		if (rth->rta_type == IFA_LOCAL) {
			uint32_t ipaddr = htonl(*((uint32_t *) RTA_DATA(rth)));
			char name[IFNAMSIZ];

			if_indextoname(ifa->ifa_index, name);
			printf("%s is %d.%d.%d.%d\n", name,
			       (ipaddr >> 24) & 0xff, (ipaddr >> 16) & 0xff,
			       (ipaddr >> 8) & 0xff, ipaddr & 0xff);
		}

		rth = RTA_NEXT(rth, rtl);
	}
}

static void print_rtm_newdel(struct nlmsghdr *h)
{
	debug("RTM_DELADDR: TODO ...\n");
}

static int process(sock)
{
	int ret = 0;
	char buf[4096];
	struct iovec iov = { buf, sizeof(buf) };
	struct sockaddr_nl snl;
	struct msghdr msg = { (void *)&snl, sizeof(snl), &iov, 1, NULL, 0, 0 };
	struct nlmsghdr *h;

	ret = recvmsg(sock, &msg, 0);
	if (ret < 0) {
		if (errno == EWOULDBLOCK || errno == EAGAIN)
			return 0;
		printf("Error recvmsg: %d\n", ret);
		return ret;
	}

	for (h = (struct nlmsghdr *)buf;
	     NLMSG_OK(h, (unsigned int)ret); h = NLMSG_NEXT(h, ret)) {

		switch (h->nlmsg_type) {
		case RTM_NEWADDR:
			print_rtm_newaddr(h);
			break;
		case RTM_DELADDR:
			print_rtm_newdel(h);
			break;
		}
	}
	return 0;
}

int listen_ip_addr_change(void)
{
	int sock;
	struct sockaddr_nl local;

	sock = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
	if (sock < 0) {
		fprintf(stderr, "Fail to open netlink socket\n");
		return -1;
	}

	memset(&local, 0, sizeof(local));
	local.nl_family = AF_NETLINK;
	local.nl_pid = getpid();
	local.nl_groups = RTMGRP_IPV4_IFADDR;

	if (bind(sock, (struct sockaddr *)&local, sizeof(local)) < 0) {
		fprintf(stderr, "Fail to bind\n");
		return -1;
	}

	while (1) {
		int retval;
		struct timeval tv = {.tv_sec = 60,.tv_usec = 0 };
		fd_set rfds;

		FD_ZERO(&rfds);
		FD_CLR(sock, &rfds);
		FD_SET(sock, &rfds);

		retval = select(FD_SETSIZE, &rfds, NULL, NULL, &tv);
		if (retval == -1) {
			fprintf(stderr, "Fail to select() \n");
			break;
		} else if (retval) {
			process(sock);
		}
	}

	close(sock);

	return 0;
}

int main(int argc, char **argv)
{
	return listen_ip_addr_change();
}
