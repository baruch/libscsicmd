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

#ifndef LIBSCSICMD_LOG_SENSE_H
#define LIBSCSICMD_LOG_SENSE_H

#include "scsicmd_utils.h"

#define LOG_SENSE_MIN_LEN 4

static inline uint8_t log_sense_page_code(uint8_t *data)
{
	return data[0] & 0x3F;
}

static inline bool log_sense_subpage_format(uint8_t *data)
{
	return data[0] & 0x40;
}

static inline bool log_sense_data_saved(uint8_t *data)
{
	return data[0] & 0x80;
}

static inline uint8_t log_sense_subpage_code(uint8_t *data)
{
	return data[1];
}

static inline uint16_t log_sense_data_len(uint8_t *data)
{
	return get_uint16(data, 2);
}

#endif
