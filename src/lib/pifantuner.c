#include "pifantuner.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "argon.h"

static struct pifantuner_speed_setting default_speed_settings[] = { { 55, 10 },
																																		{ 60, 55 },
																																		{ 65, 100 } };

static float pifantuner_get_cpu_temperature(void) {
        static const char cpu_temperature_file[] =
                        "/sys/class/thermal/thermal_zone0/temp";
        float cpu_temp_degrees = 0.0;

        FILE *fd = fopen(cpu_temperature_file, "r");
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

static struct pifantuner_config *pifantuner_create_config(void) {
	struct pifantuner_config *config = malloc(sizeof(struct pifantuner_config));
	if (!config) {
		goto error;
	}
	config->poll_time_seconds = 1;

	config->speed_settings_list.settings = NULL;
	config->speed_settings_list.count = 0;

error:
	return NULL;
}

static void pifantuner_config_destroy(struct pifantuner_config *config) {
	if (config) {
		free(config);
	}
}

void pifantuner_add_speed_setting(struct pifantuner_ctx *ctx, int temperature, uint8_t speed) {
	assert(ctx);
	struct pifantuner_config *config = ctx->config;
	assert(config);
	struct pifantuner_speed_setting *settings = config->speed_settings_list.settings;
	settings = realloc(settings, ++config->speed_settings_list.count);
	if (settings) {
		settings->temperature = temperature;
		settings->speed = speed;
	}
}

void pifantuner_clear_speed_settings(struct pifantuner_ctx *ctx) {
	struct pifantuner_speed_setting *settings = ctx->config->speed_settings_list.settings;
	if (settings) {
		free(settings);
	}
}

void pifantuner_poll(const struct pifantuner_ctx *ctx) {
        assert(ctx);
				assert(ctx->config);

				struct pifantuner_speed_settings_list *const speed_settings_list = &ctx->config->speed_settings_list;
				if (speed_settings_list->count == 0) {
					speed_settings_list->settings = default_speed_settings;
					speed_settings_list->count = sizeof(default_speed_settings) / sizeof(default_speed_settings[0]);
				}

        const float current_temperature = pifantuner_get_cpu_temperature();

        uint8_t new_setting = 0;
				assert(speed_settings_list);
        for (int i = 0; i < speed_settings_list->count; i++) {
                if (current_temperature >= speed_settings_list->settings[i].temperature) {
									new_setting = speed_settings_list->settings[i].speed;
                }
        }

        assert(ctx->interface);
        assert(ctx->interface->update);
        ctx->interface->update(ctx, new_setting);
}

struct pifantuner_ctx *pifantuner_create(void) {
        struct pifantuner_ctx *ctx = malloc(sizeof(struct pifantuner_ctx));
        if (!ctx) {
                goto error;
        }

        ctx->interface = &argon_fan_iface;

				assert(ctx->interface->create);
				ctx->interface->create(ctx);

				ctx->config = pifantuner_create_config();
				if (!ctx->config) {
					goto error;
				}

        return ctx;
error:
        pifantuner_destroy(ctx);
        return NULL;
}

void pifantuner_destroy(struct pifantuner_ctx *ctx) {
        if (ctx) {
                if (ctx->handle) {
                        ctx->interface->destroy(ctx);
                }
								if (ctx->config) {
									if (ctx->config->speed_settings_list.settings) {
										pifantuner_clear_speed_settings(ctx);
									}
									pifantuner_config_destroy(ctx->config);
								}
                free(ctx);
        }
}
