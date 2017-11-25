/**
 * Copyright 2017 @ mtdcy.chen
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 **/

/**
 * File: 
 * Desc:
 *
 * Changes:
 * 20171201     mtdcy   initial version
 *
 **/


#include <mtp/log.h>

#include <stdio.h>
#include <stdarg.h>

#include <unistd.h>

#define BUF_SIZE    4096

#ifdef __APPLE__
#include <pthread.h>
extern "C" pid_t gettid() {
    uint64_t tid;
    pthread_threadid_np(NULL, &tid);
    return (pid_t)tid;
}
#endif

void log_write_impl0(const char *tag, const char *msg) {
    pid_t tid = gettid();
    fprintf(stdout, "[%d][%s]: %s\n", tid, tag, msg);
}

extern "C"
void __mtp_log_write(const char *tag, const char *fmt, ...) {
    char buf[BUF_SIZE];

    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUF_SIZE, fmt, ap);
    va_end(ap);

    log_write_impl0(tag, buf);
}
