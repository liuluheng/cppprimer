// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.
//
// Logger implementation that can be shared by all environments
// where enough posix functionality is available.

#ifndef STORAGE_LEVELDB_UTIL_POSIX_LOGGER_H_
#define STORAGE_LEVELDB_UTIL_POSIX_LOGGER_H_

#include <algorithm>
#include <string>
#include <cstring>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>

// An interface for writing log messages.
class Logger {
public:
    Logger() { }
    virtual ~Logger();

    // Write an entry to the log file with the specified format.
    virtual void Logv(const char* format, va_list ap) = 0;

private:
    // No copying allowed
    Logger(const Logger&);
    void operator=(const Logger&);
};

Logger::~Logger() {
}

class PosixLogger : public Logger {
private:
    FILE* file_;
    uint64_t (*gettid_)();  // Return the thread id for the current thread
public:
    PosixLogger(FILE* f, uint64_t (*gettid)()) : file_(f), gettid_(gettid) { }
    virtual ~PosixLogger() {
        fclose(file_);
    }
    virtual void Logv(const char* format, va_list ap) {
        const uint64_t thread_id = (*gettid_)();

        // We try twice: the first time with a fixed-size stack allocated buffer,
        // and the second time with a much larger dynamically allocated buffer.
        char buffer[500];
        for (int iter = 0; iter < 2; iter++) {
            char* base;
            int bufsize;
            if (iter == 0) {
                bufsize = sizeof(buffer);
                base = buffer;
            } else {
                bufsize = 30000;
                base = new char[bufsize];
            }
            char* p = base;
            char* limit = base + bufsize;

            struct timeval now_tv;
            gettimeofday(&now_tv, NULL);
            const time_t seconds = now_tv.tv_sec;
            struct tm t;
            localtime_r(&seconds, &t);
            p += snprintf(p, limit - p,
                    "%04d/%02d/%02d-%02d:%02d:%02d.%06d %llx ",
                    t.tm_year + 1900,
                    t.tm_mon + 1,
                    t.tm_mday,
                    t.tm_hour,
                    t.tm_min,
                    t.tm_sec,
                    static_cast<int>(now_tv.tv_usec),
                    static_cast<long long unsigned int>(thread_id));

            // Print the message
            if (p < limit) {
                va_list backup_ap;
                va_copy(backup_ap, ap);
                p += vsnprintf(p, limit - p, format, backup_ap);
                va_end(backup_ap);
            }

            // Truncate to available space if necessary
            if (p >= limit) {
                if (iter == 0) {
                    continue;       // Try again with larger buffer
                } else {
                    p = limit - 1;
                }
            }

            // Add newline if necessary
            if (p == base || p[-1] != '\n') {
                *p++ = '\n';
            }

            assert(p <= limit);
            fwrite(base, 1, p - base, file_);
            fflush(file_);
            if (base != buffer) {
                delete[] base;
            }
            break;
        }
    }
};

static uint64_t gettid() {
    pthread_t tid = pthread_self();
    uint64_t thread_id = 0;
    memcpy(&thread_id, &tid, std::min(sizeof(thread_id), sizeof(tid)));
    return thread_id;
}

int NewLogger(const std::string& fname, Logger** result) {
    FILE* f = fopen(fname.c_str(), "w");
    if (f == NULL) {
        *result = NULL;
        return -1;
    } else {
        *result = new PosixLogger(f, &gettid);
        return 0;
    }
}

void Log(Logger* info_log, const char* format, ...) {
    if (info_log != NULL) {
        va_list ap;
        va_start(ap, format);
        info_log->Logv(format, ap);
        va_end(ap);
    }
}
#endif  // STORAGE_LEVELDB_UTIL_POSIX_LOGGER_H_