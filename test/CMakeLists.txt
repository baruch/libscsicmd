add_executable(ata_check_power_mode ata_check_power_mode.c main.c sense_dump.c)
target_link_libraries(ata_check_power_mode scsicmd)

add_custom_command(OUTPUT ata_identify_dump.h
                   COMMAND ../structs/ata_struct_2_h_dump.py ../structs/ata_identify.yaml > ata_identify_dump.h
                   DEPENDS ../structs/ata_struct_2_h_dump.py ../structs/ata_identify.yaml)
add_custom_command(OUTPUT ata_identify_dump.c
                   COMMAND ../structs/ata_struct_2_c_dump.py ../structs/ata_identify.yaml > ata_identify_dump.c
                   DEPENDS ../structs/ata_struct_2_c_dump.py ../structs/ata_identify.yaml)
add_executable(ata_identify ata_identify.c main.c sense_dump.c ata_identify_dump.c ata_identify_dump.h)
target_link_libraries(ata_identify scsicmd)

add_executable(ata_smart_read_data ata_smart_read_data.c main.c sense_dump.c)
target_link_libraries(ata_smart_read_data scsicmd)

add_executable(ata_smart_return_status ata_smart_return_status.c main.c sense_dump.c)
target_link_libraries(ata_smart_return_status scsicmd)

add_executable(scsi_inquiry scsi_inquiry.c main.c sense_dump.c)
target_link_libraries(scsi_inquiry scsicmd)

add_executable(scsi_log_sense scsi_log_sense.c main.c sense_dump.c)
target_link_libraries(scsi_log_sense scsicmd)

add_executable(scsi_read_capacity_10 scsi_read_capacity_10.c main.c sense_dump.c)
target_link_libraries(scsi_read_capacity_10 scsicmd)

add_executable(scsi_read_capacity_16 scsi_read_capacity_16.c main.c sense_dump.c)
target_link_libraries(scsi_read_capacity_16 scsicmd)

add_executable(sense_decode sense_decode.c sense_dump.c)
target_link_libraries(sense_decode scsicmd)
