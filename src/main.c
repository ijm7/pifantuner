#include "pifantuner.h"

#include <signal.h>
#include <stdbool.h>
#include <unistd.h>

static const unsigned int poll_time_seconds = 1;

static bool run = true;

static void signal_handler(int) {
		run = false;
}

static void run_daemon(const pifantuner_t *pifantuner) {
		while (run) {
				pifantuner_poll(pifantuner);
				sleep(poll_time_seconds);
		}
}

int main(int argc, char **argv) {
		signal(SIGINT, signal_handler);
		pifantuner_t *pifantuner = pifantuner_create();
		if (!pifantuner) {
				return 1;
		}
		run_daemon(pifantuner);

		pifantuner_destroy(pifantuner);
		return 0;
}

