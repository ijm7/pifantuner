#include "pifantuner.h"

#include <assert.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

static float pifantuner_get_cpu_temperature(void) {
		static const char cpu_temperature_file[] = "/sys/class/thermal/thermal_zone0/temp";
		float cpu_temp_degrees = 0.0;

		FILE *fd = fopen(cpu_temperature_file, "r");
		if (!fd) {
				// TODO logging
				goto error;
		}

		int current_temperature;
		if (fscanf(fd, "%d", &current_temperature) != 1) {
				// TODO logging
				goto error;
		}
		else if (errno == ERANGE) {
				// TODO logging
				goto error;
		}

		cpu_temp_degrees = current_temperature / 1000.0;
error:
		fclose(fd);
		return cpu_temp_degrees;
}

void pifantuner_poll(const pifantuner_t *pifantuner) {
		assert(pifantuner);
		static const pifantuner_map_t default_settings[] = {
				{55, 10},
				{60, 55},
				{65, 100}
		};
		const float current_temperature = pifantuner_get_cpu_temperature();
		static const size_t settings_size = sizeof(default_settings) / sizeof(default_settings[0]);
		uint8_t new_setting = 0;
		for (int i; i < settings_size; i++) {
				if (current_temperature >= default_settings[i].temperature) {
						new_setting = default_settings[i].temperature;
				}
		}

		i2c_smbus_write_byte(pifantuner->file, new_setting);
}

pifantuner_t *pifantuner_create(void) {
		pifantuner_t *pifantuner = malloc(sizeof(pifantuner_t));
		if (!pifantuner) {
				goto error;
		}
		pifantuner->file = open("/dev/i2c-1", O_RDWR);
		if (pifantuner->file < 0) {
				// TODO
		}
		static const int addr = 0x1a;
		if (ioctl(pifantuner->file, I2C_SLAVE, addr) < 0) {
				// TODO
		}

		return pifantuner;
error:
		pifantuner_destroy(pifantuner);
		return NULL;
}

void pifantuner_destroy(pifantuner_t *pifantuner) {
		if (pifantuner) {
				// reset fan
				i2c_smbus_write_byte(pifantuner->file, 0x00);
				close(pifantuner->file);
				free(pifantuner);
		}
}
