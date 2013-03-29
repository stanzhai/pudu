
#ifndef _HTTP_H_
#define _HTTP_H_

#define HTTP_MAXRETRY			3
#define HTTP_MAXCONNECTS		20
#define HTTP_CONNECT_TIMEOUT	50
#define HTTP_READTIMEOUT		180

#define HTTP_MAXPARALLEL		3	/* number of connections per host */

#define HTTP_PREFIX				"http://"
#define HTTP_DEFAULTPORT		80

#define HTTP_OK					200
#define HTTP_MOVEPERM			301
#define HTTP_MOVETEMP			302
#define HTTP_NOTFOUND			404

#define HTTP_HEADEREND			"\r\n\r\n"

#define HTTP_REQUEST_GET		0x0001
#define HTTP_REQUEST_HEAD		0x0002

#define HTTP_MAXMEM				128*1024

#define URI_CLEANCONNECT		0x01	/* reduces number of connects */

#define url_host				url.host
#define url_file				url.file
#define url_port				url.port

struct url {
	char *host;
	char *file;
	u_short port;
};

struct uri_small {
	TAILQ_ENTRY(uri_small) next;

	struct url url;

	u_short depth;
	u_short flags;
	u_short retry;
};

struct uri {
	u_short depth;
	u_short flags;
	u_short retry;

	int code;
	int save_fd;
	
	struct url url;
	
	char *format;
	ssize_t length;

	char *header;
	size_t hdlen;

	char *body;
	size_t bdmemlen;
	size_t bdread;
	size_t bdlen;

	struct dns_entry *dns;	/* reference counter for parallel access */

	int fd;
	struct event ev;
};

struct header {
	char *name;
	char **where;
};

struct dispatch {
	TAILQ_ENTRY(dispatch) next;

	char *type;
	int (*cb)(struct uri *);
};

TAILQ_HEAD(uri_list, uri_small);

struct uri *uri_new(void);
void uri_free(struct uri *, int);

int http_isrobotstxt(struct uri *);
int http_newconnection(struct uri *, char *, u_short);

int http_init(char *);
int http_add(u_short, char *, u_short);

int http_setuseragent(char *);

#define http_setcallback(cb, x)	do { \
					extern void (*cb)(); \
					cb = x; \
				} while (0)


int http_postevent(void);

void http_fetch(struct uri *uri);
int http_hostportfile(char *, char **, u_short *, char **);
void http_parseheader(struct uri *);
void http_readheader(int, short, void *);
void http_readbody(int, short, void *);
void http_dispatch(struct uri *, int);
#define HTTPDIS_KEEP	0
#define HTTPDIS_FREE	1

int http_restore_state(char *);
int http_register_dispatch(char *, int (*)(struct uri *));
int http_mark_seen(char *);

char *http_normalize_uri(char *);
char *http_basename(struct uri *);
char *http_make_uri(char *, char *);

char *http_make_url(struct url *);

void http_print_stats(void);

#endif /* !_HTTP_H_ */