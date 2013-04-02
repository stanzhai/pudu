#include <ev.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

EV_P;
ev_io send_w;
int remote_fd;

static void send_cb (EV_P_ ev_io *w, int revents)
{
  if (revents & EV_WRITE)
  {
    char* data = "GET http://10.6.154.251/ HTTP/1.1\r\nAccept: */*\r\nAccept-Language: zh-CN\r\nUser-Agent: pudu\r\nConnection: Keep-Alive\r\nHost: 10.6.154.251\r\n\r\n";
    if (-1 == send(remote_fd, data, strlen(data) + 1, 0)) {
      perror("echo send");
      exit(1);
    }

    ev_io_stop(EV_A_ &send_w);
    ev_io_set(&send_w, remote_fd, EV_READ);
    ev_io_start(EV_A_ &send_w);
  }
  else if (revents & EV_READ)
  {
    int n;
    char str[1024] = ".\0";

    n = recv(remote_fd, str, 1024, 0);
    if (n <= 0) {
      if (0 == n) {
        ev_io_stop(EV_A_ &send_w);
        close(remote_fd);
      }  else if (EAGAIN == errno) {
        puts("should never get in this state with libev");
      } else {
        perror("recv");
      }
      return;
    }
    printf("%s", str);

  }
}

static void connection(char* ip, int port) {
	int len;
	struct sockaddr_in  servaddr;

	if (-1 == (remote_fd = socket(AF_INET, SOCK_STREAM, 0))) {
	    printf("init remote socket error");
		exit(1);
	}

	// 发送回调信息
	ev_io_init(&send_w, send_cb, remote_fd, EV_READ | EV_WRITE);
	ev_io_start(EV_A_ &send_w);

	memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    if( inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0){
	    printf("inet_pton error for\n");
	    exit(0);
    }

    if( connect(remote_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
	    printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
	    exit(0);
    }
}

int main (void)
{
	loop = ev_loop_new (EVBACKEND_EPOLL | EVFLAG_NOENV);
	connection("10.6.154.251", 80);
	ev_run (loop, 0);
	return 0;
}
