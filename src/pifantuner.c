#include "pifantuner.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>

#include "argon.h"

static float pifantuner_get_cpu_temperature(void)
{
    static const char cpu_temperature_file[] = "/sys/class/thermal/thermal_zone0/temp";
    float cpu_temp_degrees = 0.0;

    FILE* fd = fopen(cpu_temperature_file, "r");
    if (!fd) {
        goto error;
    }

    int current_temperature;
    if (fscanf(fd, "%d", &current_temperature) != 1) {
        goto error;
    } else if (errno == ERANGE) {
        goto error;
    }

    cpu_temp_degrees = current_temperature / 1000.0;
error:
    fclose(fd);
    return cpu_temp_degrees;
}

void pifantuner_poll(const struct pifantuner_ctx* ctx)
{
    assert(ctx);
    static const struct pifantuner_map default_settings[] = {
        { 55, 10 },
        { 60, 55 },
        { 65, 100 }
    };
    const float current_temperature = pifantuner_get_cpu_temperature();
    static const size_t settings_size = sizeof(default_settings) / sizeof(default_settings[0]);
    uint8_t new_setting = 0;
    for (int i = 0; i < settings_size; i++) {
        if (current_temperature >= default_settings[i].temperature) {
            new_setting = default_settings[i].temperature;
        }
    }

    assert(ctx->interface);
    assert(ctx->interface->update);
    ctx->interface->update(ctx, new_setting);
}

struct pifantuner_ctx* pifantuner_create(void)
{
    struct pifantuner_ctx* ctx = malloc(sizeof(struct pifantuner_ctx));
    if (!ctx) {
        goto error;
    }
    ctx->interface = &argon_fan_iface;
    ctx->interface->create(ctx);

    return ctx;
error:
    pifantuner_destroy(ctx);
    return NULL;
}

void pifantuner_destroy(struct pifantuner_ctx* ctx)
{
    if (ctx) {
        if (ctx->handle) {
            ctx->interface->destroy(ctx);
        }
        free(ctx);
    }
}
