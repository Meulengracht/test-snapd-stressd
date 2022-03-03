/**
 * @file main.c
 * @author Philip Meulengracht (philip.meulengracht@canonical.com)
 * @brief Logger, a small tool that can continiously generate systemd journal log messages
 * @version 0.1
 * @date 2022-03-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <systemd/sd-journal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void log_message(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    sd_journal_printv(LOG_NOTICE, format, args);
    va_end(args);
}

int main(int argc, char** argv)
{
    int    messageCount    = 1;
    int    messageInterval = 1;
    size_t totalWritten    = 0;

    // get logging configuration from parameters
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--count")) {
                messageCount = atoi(argv[i + 1]);
            }
            else if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "--interval")) {
                messageInterval = atoi(argv[i + 1]);
            }
        }
    }

    if (messageCount < 1) {
        fprintf(stderr, "logger: invalid number of messages: %i\n", messageCount);
        return -1;
    }

    if (messageInterval < 1) {
        fprintf(stderr, "logger: invalid interval: %i\n", messageInterval);
        return -1;
    }

    while (1) {
        for (int i = 0; i < messageCount; i++) {
            log_message("LogDaemon: message number: %u", totalWritten++);
        }
        sleep(messageInterval);
    }

    return 0;
}
