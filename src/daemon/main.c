#include "pifantuner.h"

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const unsigned int poll_time_seconds = 1;

static bool run = true;

static void signal_handler(int) { run = false; }

static void run_daemon(const struct pifantuner_ctx *ctx) {
        while (run) {
                pifantuner_poll(ctx);
                sleep(poll_time_seconds);
        }
}

long parse_speed_setting_argument(const char *arg) {
        char *end;
        const long parsed_arg = strtol(arg, &end, 10);
        if (arg == end || *end != '\0' ||
            ((parsed_arg == LONG_MIN || parsed_arg == LONG_MAX) &&
             errno == ERANGE)) {
                return -1;
        }

        return parsed_arg;
}

static int parse_opts(struct pifantuner_ctx *ctx, int argc, char **argv) {
        int c;
        while ((c = getopt(argc, argv, "hv")) != -1) {
                switch (c) {
                case 'h':
                case 'v':
                default:
                        goto error;
                        break;
                }
        }
        for (int i = optind; i < argc; i++) {
                int speed;
                uint8_t temperature;

                char *colon_pos = strchr(argv[i], ':');
                if (colon_pos) {
                        *colon_pos = '\0';
                        long parsed_temperature =
                                        parse_speed_setting_argument(argv[i]);
                        if (parsed_temperature < 0 ||
                            parsed_temperature > INT_MAX) {
                                goto error;
                        }
                        temperature = (int)parsed_temperature;

                        const char *speed_pos = ++colon_pos;
                        if (speed_pos) {
                                long parsed_speed =
                                                parse_speed_setting_argument(
                                                                speed_pos);
                                if (parsed_speed < 0 ||
                                    parsed_speed > UINT8_MAX) {
                                        goto error;
                                }
                                speed = (uint8_t)parsed_speed;
                        }
                        pifantuner_add_speed_setting(ctx, temperature, speed);
                        printf("Speed setting added: Temperature [%d] Speed "
                               "[%" PRIu8 "] \n",
                               temperature, speed);
                } else {
                        goto error;
                }
        }

        return 0;

error:
        fprintf(stderr, "Usage: %s temperature:speed temperature:speed ..\n",
                argv[0]);
        return -1;
}

int main(int argc, char **argv) {
        signal(SIGINT, signal_handler);
        struct pifantuner_ctx *ctx = pifantuner_create();
        if (!ctx) {
                return 1;
        }
        if (parse_opts(ctx, argc, argv) == 0) {
                run_daemon(ctx);
        }

        pifantuner_destroy(ctx);
        return 0;
}
