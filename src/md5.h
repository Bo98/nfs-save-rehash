#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	uint_fast32_t lo, hi;
	uint_fast32_t a, b, c, d;
	uint8_t buffer[64];
	uint_fast32_t block[16];
} md5_context;

void md5_init(md5_context *ctx);
void md5_update(md5_context *ctx, const void *data, size_t size);
void md5_final(uint8_t *result, md5_context *ctx);

#ifdef __cplusplus
}
#endif
