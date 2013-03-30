/*
 * log.c - 日志记录 - StanZhai 2013/3/29
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

FILE *fpLog = NULL;

void err(int eval, const char *fmt, ...)
{
	va_list ap;
	
	va_start(ap, fmt);
	if (fmt != NULL) {
		if (!fpLog) fpLog = stderr;
		(void)vfprintf(fpLog, fmt, ap);
		(void)fprintf(fpLog, ": ");
	}
	va_end(ap);
	(void)fprintf(fpLog, "%s\n", strerror(errno));
	exit(eval);
}

void warn(const char *fmt, ...)
{
	va_list ap;
	
	va_start(ap, fmt);
	if (fmt != NULL) {
		if (!fpLog) fpLog = stderr;
		(void)vfprintf(fpLog, fmt, ap);
		(void)fprintf(fpLog, ": ");
	}
	va_end(ap);
	(void)fprintf(fpLog, "%s\n", strerror(errno));
}

void errx(int eval, const char *fmt, ...)
{
	va_list ap;
	
	va_start(ap, fmt);
	if (fmt != NULL)
		if (!fpLog) fpLog = stderr;
		(void)vfprintf(fpLog, fmt, ap);
	(void)fprintf(fpLog, "\n");
	va_end(ap);
	exit(eval);
}

void warnx(const char *fmt, ...)
{
	va_list ap;
	
	va_start(ap, fmt);
	if (fmt != NULL) {
		if (!fpLog) fpLog = stderr;
		(void)vfprintf(fpLog, fmt, ap);
        (void)fprintf(fpLog, "\n");
	}
	va_end(ap);
}

