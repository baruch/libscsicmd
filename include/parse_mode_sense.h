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

#ifndef LIBSCSICMD_MODE_SENSE_H
#define LIBSCSICMD_MODE_SENSE_H

#include "scsicmd_utils.h"

/* Mode parameter header for the MODE SENSE 6 */
#define MODE_SENSE_6_MIN_LEN 4u

static inline uint8_t mode_sense_6_data_len(uint8_t *data)
{
	return data[0];
}

static inline uint8_t mode_sense_6_medium_type(uint8_t *data)
{
	return data[1];
}

static inline uint8_t mode_sense_6_device_specific_param(uint8_t *data)
{
	return data[2];
}

static inline uint8_t mode_sense_6_block_descriptor_length(uint8_t *data)
{
	return data[3];
}

static inline uint8_t *mode_sense_6_block_descriptor_data(uint8_t *data)
{
	return data + MODE_SENSE_6_MIN_LEN;
}

static inline uint8_t *mode_sense_6_mode_data(uint8_t *data)
{
	return data + MODE_SENSE_6_MIN_LEN + mode_sense_6_block_descriptor_length(data);
}

/* Mode parameter header for the MODE SENSE 10 */
#define MODE_SENSE_10_MIN_LEN 8u

static inline uint16_t mode_sense_10_data_len(uint8_t *data)
{
	return get_uint16(data, 0);
}

static inline uint8_t mode_sense_10_medium_type(uint8_t *data)
{
	return data[2];
}

static inline uint8_t mode_sense_10_device_specific_param(uint8_t *data)
{
	return data[3];
}

static inline bool mode_sense_10_long_lba(uint8_t *data)
{
	return data[4] & 1;
}

/* SPC4 says length is times 8 without long lba and times 16 with it but it seems the value is taken verbatim */
static inline uint16_t mode_sense_10_block_descriptor_length(uint8_t *data)
{
	return get_uint16(data, 6);
}

static inline uint8_t *mode_sense_10_block_descriptor_data(uint8_t *data)
{
	if (mode_sense_10_block_descriptor_length(data) > 0)
		return data + MODE_SENSE_10_MIN_LEN;
	else
		return NULL;
}

static inline uint8_t *mode_sense_10_mode_data(uint8_t *data)
{
	return data + MODE_SENSE_10_MIN_LEN + mode_sense_10_block_descriptor_length(data);
}

/* Regular block descriptor (as opposed to long) */
#define BLOCK_DESCRIPTOR_LENGTH 8
#define BLOCK_DESCRIPTOR_NUM_BLOCKS_OVERFLOW 0xFFFFFF

static inline uint8_t block_descriptor_density_code(uint8_t *data)
{
	return data[0];
}

static inline uint32_t block_descriptor_num_blocks(uint8_t *data)
{
	return get_uint24(data, 1);
}

static inline uint32_t block_descriptor_block_length(uint8_t *data)
{
	return get_uint24(data, 5);
}

#endif
