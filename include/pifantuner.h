#pragma once

#include <stddef.h>
#include <stdint.h>

struct pifantuner_iface;

struct pifantuner_speed_setting {
        int temperature;
        uint8_t speed;
};

struct pifantuner_speed_settings_list {
        struct pifantuner_speed_setting *settings;
        size_t count;
};

struct pifantuner_config {
        unsigned poll_time_seconds;
        struct pifantuner_speed_settings_list speed_settings_list;
};

struct pifantuner_ctx {
        void *handle;
        const struct pifantuner_iface *interface;
        struct pifantuner_config *config;
};

struct pifantuner_iface {
        int (*create)(struct pifantuner_ctx *);
        int32_t (*update)(const struct pifantuner_ctx *, uint8_t);
        void (*destroy)(struct pifantuner_ctx *);
};

void pifantuner_poll(const struct pifantuner_ctx *ctx);

void pifantuner_add_speed_setting(struct pifantuner_ctx *ctx, int temperature,
                                  uint8_t speed);
void pifantuner_clear_speed_settings(struct pifantuner_ctx *ctx);

struct pifantuner_ctx *pifantuner_create();
void pifantuner_destroy(struct pifantuner_ctx *ctx);
