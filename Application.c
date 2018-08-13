#include <sys/socket.h>
#include <linux/netlink.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/un.h>
#define NETLINK_USER 31
#define SOCK_PATH "/tmp/.python"
#define MAX_PAYLOAD 1024 /* maximum payload size*/
struct sockaddr_nl src_addr, dest_addr;
struct nlmsghdr *nlh = NULL;
struct iovec iov;
int sock_fd;
struct msghdr msg;

int main()
{

    int s, s2, t, len;
    struct sockaddr_un local, remote;
    char str[100];
    //init unix socket
    if ((s = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) 
	{
        perror("socket");
        exit(1);
        }
    local.sun_family = AF_UNIX;
    strcpy(local.sun_path, SOCK_PATH);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(s, (struct sockaddr *)&local, len) == -1) 
	{
        perror("bind");
        exit(1);
        }

    if (listen(s, 5) == -1) {
        perror("listen");
        exit(1);
        }
    printf("Waiting for a connection...\n");
    t = sizeof(remote)
    if ((s2 = accept(s, (struct sockaddr *)&remote, &t)) == -1) {
    	perror("accept");
        exit(1);
        }
    //Socket for Kernel Communication
    sock_fd=socket(PF_NETLINK, SOCK_RAW, NETLINK_USER);
    if(sock_fd<0)
	return -1;
    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid(); /* self pid */
    bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr));
    memset(&dest_addr, 0, sizeof(dest_addr));
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0; /* For Linux Kernel */
    dest_addr.nl_groups = 0; /* unicast */
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;
    strcpy(NLMSG_DATA(nlh), "Register");
    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    printf("Sending message to kernel\n");
    sendmsg(sock_fd,&msg,0);
    printf("Waiting for message from kernel\n");
/* Read message from kernel */
   recvmsg(sock_fd, &msg, 0);
   printf("Received message payload: %s\n", (char *)NLMSG_DATA(nlh));
   str="ON"
   while(1)
   {  
   	recvmsg(sock_fd, &msg, 0);//receive message from kernel
	printf("Received message payload: %s\n", (char *)NLMSG_DATA(nlh));
	if (send(s2, str, n, 0) < 0) {//send messsage to unix socket
                    perror("send");
                    done = 1;
        }

   }



   close(sock_fd);
   }
