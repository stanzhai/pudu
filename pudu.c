#include <sys/types.h>
#include <stdio.h>
#include <regex.h>
#include <ev.h>

FILE* fpLog=NULL;
void usage();

void shutdown()
{
	fclose(fpLog);
}

int main(int argc, char* argv[])
{
	fprintf(stderr, "pudu web crawler(built on %s)\n", __DATE__);

	// 解析命令行参数
	int i;
	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'h':
				usage();
				break;
			case 'l':
				if ((++i)<argc) fpLog = freopen(argv[i], "w", stderr);
				break;
			}
		}
	}

	if (fpLog != NULL) fprintf(fpLog, "test");
	shutdown();
	exit(0);
}

void usage()
{
	fprintf(stderr,"Usage: pudu	-h	: 显示此帮助信息\n"
			       "		-l	: 指定日志文件\n");
	fflush(stderr);
    exit(1);
}

