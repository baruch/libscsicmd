#include "ata_smart.h"
#include <stdlib.h>

int ata_smart_get_temperature(const ata_smart_attr_t *attrs, int num_attrs, const smart_table_t *table, int *min_temp, int *max_temp)
{
	const smart_attr_t *attr_info;
	int i;

	attr_info = smart_attr_for_type(table, SMART_ATTR_TYPE_TEMP);
	if (attr_info == NULL)
		return -1;

	for (i = 0; i < num_attrs; i++) {
		if (attrs[i].id == attr_info->id) {
			// Temperature is 150 minus the current value
			int temp = 150 - attrs[i].value;
			if (temp == (int)(attrs[i].raw & 0xFFFF)) {
				*min_temp = (attrs[i].raw >> 16) & 0xFFFF;
				*max_temp = (attrs[i].raw >> 32) & 0xFFFF;
			} else {
				*min_temp = *max_temp = -1;
			}
			return temp;
		}
	}

	return -1;

}
