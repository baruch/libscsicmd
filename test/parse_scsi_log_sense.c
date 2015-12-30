#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#include "parse_log_sense.h"

static unsigned char char2val(unsigned char ch)
{
	if (ch >= '0' && ch <= '9')
		return ch - '0';
	else if (ch >= 'a' && ch <= 'f')
		return ch - 'a' + 10;
	else if (ch >= 'A' && ch <= 'F')
		return ch - 'A' + 10;
	else
		return 0;
}

static void print_hex(uint8_t *buf, unsigned buf_len)
{
	unsigned i;
	for (i = 0; i < buf_len; i++) {
		printf("%02x ", buf[i]);
	}
	printf("\n");
}

static int parse_hex(unsigned char *buf, unsigned buf_size, char *str)
{
	unsigned char ch;
	unsigned len = 0;
	bool top_char = true;

	for (; *str && len < buf_size; str++) {
		if (isspace(*str)) {
			if (!top_char) {
				printf("Leftover character\n");
				return -1;
			}
		} else if (isxdigit(*str)) {
			if (top_char) {
				ch = char2val(*str);
				top_char = false;
			} else {
				buf[len++] = (ch<<4) | char2val(*str);
				top_char = true;
			}
		} else {
			printf("Unknown character '%c'\n", *str);
			return -1;
		}
	}
	return len;
}

static inline const char *yes_no(bool val)
{
	return val ? "yes" : "no";
}

static void unparsed_data(uint8_t *buf, unsigned buf_len)
{
	printf("Unparsed data: ");
	print_hex(buf, buf_len);
}

static void parse_log_sense_param_informational_exceptions(uint16_t param_code, uint8_t *param, uint8_t param_len)
{
	switch (param_code) {
		case 0:
			printf("Information Exceptions ASC: %02X\n", param[0]);
			printf("Information Exceptions ASCQ: %02X\n", param[1]);
			printf("Temperature: %u\n", param[2]);
			if (param_len > 3)
				unparsed_data(param+3, param_len-3);
			break;
		default:
			unparsed_data(param, param_len);
	}
}

static void parse_log_sense_param(uint8_t page, uint8_t subpage, uint16_t param_code, uint8_t *param, uint8_t param_len)
{
	switch (page) {
		case 0x2F: parse_log_sense_param_informational_exceptions(param_code, param, param_len); break;
		default: unparsed_data(param, param_len); break;
	}
}

static int parse_log_sense(unsigned char *data, unsigned data_len)
{
	printf("Parsing\n");
	if (data_len < LOG_SENSE_MIN_LEN) {
		printf("Insufficient data in log sense to begin parsing\n");
		return 1;
	}
	printf("Log Sense Page Code: 0x02%x\n", log_sense_page_code(data));
	printf("Log Sense Subpage: 0x%02x\n", log_sense_subpage_code(data));
	printf("Log Sense Subpage format: %s\n", yes_no(log_sense_subpage_format(data)));
	printf("Log Sense Data Saved: %s\n", yes_no(log_sense_data_saved(data)));
	printf("Log Sense Data Length: %u\n", log_sense_data_len(data));

	uint8_t *param;
	for_all_log_sense_params(data, data_len, param) {
		putchar('\n');
		printf("Log Sense Param Code: 0x%04x\n", log_sense_param_code(param));
		printf("Log Sense Param Len: %u\n", log_sense_param_len(param));
		parse_log_sense_param(log_sense_page_code(data), log_sense_subpage_code(data), log_sense_param_code(param), log_sense_param_data(param), log_sense_param_len(param));
	}

	return 0;
}

int main(int argc, char **argv)
{
	unsigned char cdb[32];
	unsigned char sense[256];
	unsigned char data[64*1024];
	int cdb_len, sense_len, data_len;

	if (argc != 4) {
		printf("Usage: %s \"cdb\" \"sense\" \"data\"\n", argv[0]);
		return 1;
	}

	cdb_len = parse_hex(cdb, sizeof(cdb), argv[1]);
	sense_len = parse_hex(sense, sizeof(sense), argv[2]);
	data_len = parse_hex(data, sizeof(data), argv[3]);

	if (cdb_len < 0) {
		printf("Failed to parse CDB\n");
		return 1;
	}
	if (sense_len < 0) {
		printf("Failed to parse SENSE\n");
		return 1;
	}
	if (data_len < 0) {
		printf("Failed to parse DATA\n");
		return 1;
	}

	if (cdb[0] != 0x4D) {
		printf("Mismatch in CDB opcode\n");
		return 1;
	}

	if (sense_len > 0) {
		printf("Sense data indicates an error\n");
		return 1;
	}

	return parse_log_sense(data, data_len);
}
