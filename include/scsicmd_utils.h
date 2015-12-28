/* Copyright 2015 Baruch Even
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef LIBSCSICMD_UTILS_H
#define LIBSCSICMD_UTILS_H

#include <stdint.h>

static inline uint16_t get_uint16(unsigned char *buf, int start)
{
	return (uint16_t)buf[start] << 8 |
		   (uint16_t)buf[start+1];
}

static inline uint32_t get_uint24(unsigned char *buf, int start)
{
	return (uint32_t)buf[start] << 16 |
		   (uint32_t)buf[start+1] << 8 |
		   (uint32_t)buf[start+2];
}

static inline void get_uint32(unsigned char *buf, int start, uint32_t *val)
{
	*val = (uint32_t)buf[start] << 24 |
		   (uint32_t)buf[start+1] << 16 |
		   (uint32_t)buf[start+2] << 8 |
		   (uint32_t)buf[start+3];
}

static inline void get_uint64(unsigned char *buf, int start, uint64_t *val)
{
	*val = (uint64_t)buf[start] << 56 |
		   (uint64_t)buf[start+1] << 48 |
		   (uint64_t)buf[start+2] << 40 |
		   (uint64_t)buf[start+3] << 32 |
		   (uint64_t)buf[start+4] << 24 |
		   (uint64_t)buf[start+5] << 16 |
		   (uint64_t)buf[start+6] << 8 |
		   (uint64_t)buf[start+7];
}

#endif
