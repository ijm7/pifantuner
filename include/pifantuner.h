#pragma once

#include <stdint.h>

struct pifantuner_iface;

struct pifantuner_ctx {
		void *handle;
		const struct pifantuner_iface *interface;
};

struct pifantuner_map {
		int temperature;
		int speed;
};

struct pifantuner_iface {
		int (*create)(struct pifantuner_ctx *);
		int32_t (*update)(const struct pifantuner_ctx *, uint8_t);
		void (*destroy)(struct pifantuner_ctx *);
};

void pifantuner_poll(const struct pifantuner_ctx *ctx);

struct pifantuner_ctx *pifantuner_create(void);
void pifantuner_destroy(struct pifantuner_ctx *ctx);
