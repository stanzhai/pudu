/*
 * log.h - 日志记录 - StanZhai 2013/3/29
 */

#ifndef _LOG_H_
#define _LOG_H_

void err(int eval, const char *fmt, ...);
void warn(const char *fmt, ...);
void errx(int eval, const char *fmt, ...);
void warnx(const char *fmt, ...);

#endif /* !_LOG_H_ */