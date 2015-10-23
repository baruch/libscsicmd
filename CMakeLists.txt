cmake_minimum_required(VERSION 2.8)
project(libscsicmd)

export(PACKAGE libscsicmd)

include_directories("include")

add_custom_command(OUTPUT src/smartdb/smartdb_gen.c
                   COMMAND src/smartdb/smartdb_gen_c.py src/smartdb/smartdb.xml > src/smartdb/smartdb_gen.c
                   DEPENDS src/smartdb/smartdb_gen_c.py src/smartdb/smartdb.xml
                   )
add_custom_command(OUTPUT include/ata_parse.h
                   COMMAND structs/ata_struct_2_h.py structs/ata_identify.yaml > include/ata_parse.h
                   DEPENDS structs/ata_struct_2_h.py structs/ata_identify.yaml)

add_library(scsicmd STATIC src/ata.c src/ata_smart.c src/cdb.c src/parse_inquiry.c src/parse_read_cap.c src/parse_sense.c src/str_map.c src/smartdb/smartdb.c src/smartdb/smartdb_gen.c include/ata_parse.h)

add_subdirectory(test)
