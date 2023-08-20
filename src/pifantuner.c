#include "pifantuner.h"

#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <sys/ioctl.h>

static float pifantuner_get_cpu_temperature(void) {
		static constexpr const char cpu_temperature_file[] = "/sys/class/thermal/thermal_zone0/temp";
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

static void pifantuner_control(const pifantuner_t *pifantuner) {
		static constexpr const pifantuner_map_t default_settings[] = {
				{55, 10},
				{60, 55},
				{65, 100}
		};
		const float current_temperature = pifantuner_get_cpu_temperature();
		
}

pifantuner_t *pifantuner_create(void) {
		const int file = open("/dev/i2c-1", O_RDWR);
		if (file < 0) {
				// TODO
		}
		static constexpr const int addr = 0x1a;
		if (ioctl(file, I2C_SLAVE, addr) < 0) {
				// TODO
		}
		

}

void pifantuner_destroy(pifantuner_t *pifantuner) {

}
