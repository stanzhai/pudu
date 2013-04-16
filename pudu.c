#include <ev.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <hiredis/hiredis.h>
#include <iconv.h>

#define HEAD_BUFFER_SIZE 1024

EV_P;
ev_io send_w;
int remote_fd;
redisContext *c;
redisReply *reply;

char* url;
char host[HEAD_BUFFER_SIZE];


static void send_cb (EV_P_ ev_io *w, int revents)
{
  if (revents & EV_WRITE)
  {
    char header[HEAD_BUFFER_SIZE] = {0};
    strcat(header, "GET ");
    strcat(header, url);
    strcat(header, " HTTP/1.1\r\n");
    strcat(header, "Accept: */*\r\n");
    strcat(header, "User-Agent: pudu\r\n");
    strcat(header, "Connection: Keep-Alive\r\n");
    strcat(header, "Host: ");
    strcat(header, host);
    strcat(header, "\r\n\r\n");
    if (-1 == send(remote_fd, header, strlen(header) + 1, 0)) {
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
    iconv_t cd;
    cd = iconv_open("utf-8", "gb2312");
    char result[2048] = {0};
    size_t inlen, outlen;
    inlen = strlen(str);
    outlen = sizeof(result);
    iconv(cd, &str, &inlen, &result, &outlen);
    iconv_close(cd);
    printf("%s", result);
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
  struct timeval timeout = { 1, 500000 }; // 1.5 seconds
  c = redisConnectWithTimeout((char*)"127.0.0.1", 6379, timeout);
  if (c == NULL || c->err) {
      if (c) {
          printf("Connection error: %s\n", c->errstr);
          redisFree(c);
      } else {
          printf("Connection error: can't allocate redis context\n");
      }
      exit(1);
  }

  loop = ev_loop_new (EVBACKEND_EPOLL | EVFLAG_NOENV);

  // 从队列中去一个url
  reply = redisCommand(c, "SPOP %s", "crawl_pending");
  url = reply->str;

  memset(host, 0, sizeof(host));
  char *ptr;
  ptr = strstr(reply->str, "://") + 3;
  int i = 0;
  for (; ptr[i]; i++) {
    if (ptr[i] == '/') 
      break;
    host[i] = ptr[i];
  }
  int port = 80;
  ptr  = strchr(host, ':');
  if (ptr != NULL) {
    ptr++;
    port = atoi(ptr);
  }
  struct hostent *pHost = gethostbyname(host);
  struct in_addr addr;
  memcpy(&addr.s_addr, pHost->h_addr, sizeof(addr.s_addr));
  char ip[32] = {0};
  strcpy(ip, inet_ntoa(addr));
  connection(ip, port);
  ev_run (loop, 0);
  return 0;
}
