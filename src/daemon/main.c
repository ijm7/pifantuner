#include "pifantuner.h"

#include <signal.h>
#include <stdbool.h>
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

int main(int argc, char **argv) {
        signal(SIGINT, signal_handler);
        struct pifantuner_ctx *ctx = pifantuner_create(NULL);
        if (!ctx) {
                return 1;
        }
        run_daemon(ctx);

        pifantuner_destroy(ctx);
        return 0;
}
