#include "argon.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include "pifantuner.h"

static int32_t argon_fan_iface_update(const struct pifantuner_ctx *ctx, uint8_t value) {
		assert(ctx);
		assert(ctx->handle);
		const struct pifantuner_argon *handle = ctx->handle;
		return i2c_smbus_write_byte(handle->file, value);
}

static void argon_fan_iface_destroy(struct pifantuner_ctx *ctx) {
		assert(ctx);
		struct pifantuner_argon *handle = ctx->handle;
		if (handle) {
				// reset fan
				i2c_smbus_write_byte(handle->file, 0x00);
				close(handle->file);
				free(handle);
				handle = NULL;
		}
}

static int argon_fan_iface_create(struct pifantuner_ctx *ctx) {
		assert(ctx);
		assert(!ctx->handle);
		struct pifantuner_argon *handle = malloc(sizeof(struct pifantuner_argon));
		if (!handle) {
				goto error;
		}
		handle->file = open("/dev/i2c-1", O_RDWR);
		if (handle->file < 0) {
				// TODO
		}
		static const int addr = 0x1a;
		if (ioctl(handle->file, I2C_SLAVE, addr) < 0) {
				// TODO
		}

		ctx->handle = handle;
		return 0;
error:
		argon_fan_iface_destroy(ctx);
		return 1;
}

struct pifantuner_iface argon_fan_iface = {
		.create = argon_fan_iface_create,
		.update = argon_fan_iface_update,
		.destroy = argon_fan_iface_destroy
};
