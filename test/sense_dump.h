#ifndef _SENSE_DUMP_H
#define _SENSE_DUMP_H

void sense_dump(unsigned char *sense, int sense_len);
void response_dump(unsigned char *buf, int buf_len);
void cdb_dump(unsigned char *cdb, int cdb_len);

#endif
