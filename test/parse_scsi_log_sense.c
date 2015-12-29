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

static int parse_log_sense(unsigned char *data, unsigned data_len)
{
	printf("Parsing\n");
	if (data_len < LOG_SENSE_MIN_LEN) {
		printf("Insufficient data in log sense to begin parsing\n");
		return 1;
	}
	printf("Log Sense Page Code: 0x%x (%u)\n", log_sense_page_code(data), log_sense_page_code(data));
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
