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

/* Log Sense Header decode */

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

static inline uint8_t *log_sense_data(uint8_t *data)
{
	return data + LOG_SENSE_MIN_LEN;
}

/* Log Sense Parameter decode */
#define LOG_SENSE_MIN_PARAM_LEN 4

static inline uint16_t log_sense_param_code(uint8_t *param)
{
	return get_uint16(param, 0);
}

/* We ignore the control byte for now, it's very fine grained intention is to
 * allow reset of values and knowing when to reset and when a value stops
 * growing.
 * It's not important to just get the data out.
 */

static inline uint8_t log_sense_param_len(uint8_t *param)
{
	return param[3];
}

static inline uint8_t *log_sense_param_data(uint8_t *param)
{
	return param + LOG_SENSE_MIN_PARAM_LEN;
}

#define for_all_log_sense_params(data, data_len, param) \
	for (param = log_sense_data(data); \
		 (param - data) < data_len && (param + LOG_SENSE_MIN_PARAM_LEN - data) < data_len && (param + LOG_SENSE_MIN_PARAM_LEN + log_sense_param_len(param) - data) < data_len; \
		 param = param + LOG_SENSE_MIN_PARAM_LEN + log_sense_param_len(param))

#endif
