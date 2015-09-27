#include "ata_smart.h"
#include <stdlib.h>


static const smart_attr_t *ata_smart_get(const ata_smart_attr_t *attrs, int num_attrs, const smart_table_t *table, const ata_smart_attr_t **pattr, smart_attr_type_e attr_type)
{
	const smart_attr_t *attr_info;
	int i;

	attr_info = smart_attr_for_type(table, attr_type);
	if (attr_info == NULL)
		return NULL;

	for (i = 0; i < num_attrs; i++) {
		if (attrs[i].id == attr_info->id) {
			*pattr = &attrs[i];
			return attr_info;
		}
	}

	// Attribute not found in disk smart data
	return NULL;
}

int ata_smart_get_temperature(const ata_smart_attr_t *attrs, int num_attrs, const smart_table_t *table, int *pmin_temp, int *pmax_temp)
{
	const smart_attr_t *attr_info;
	const ata_smart_attr_t *smart_attr;

	attr_info = ata_smart_get(attrs, num_attrs, table, &smart_attr, SMART_ATTR_TYPE_TEMP);
	if (attr_info == NULL)
		return -1;

	// Temperature is some offset minus the current value, usually
	int temp = attr_info->offset - smart_attr->value;
	*pmin_temp = *pmax_temp = -1;

	if (smart_attr->raw) {
		int min_temp = (smart_attr->raw >> 16) & 0xFFFF;
		int max_temp = (smart_attr->raw >> 32) & 0xFFFF;

		temp = smart_attr->raw & 0xFFFF;

		if (max_temp >= temp && min_temp <= temp) {
			*pmin_temp = min_temp;
			*pmax_temp = max_temp;
		}
	}
	return temp;
}

int ata_smart_get_power_on_hours(const ata_smart_attr_t *attrs, int num_attrs, const smart_table_t *table, int *pminutes)
{
	const smart_attr_t *attr_info;
	const ata_smart_attr_t *smart_attr;

	attr_info = ata_smart_get(attrs, num_attrs, table, &smart_attr, SMART_ATTR_TYPE_POH);
	if (attr_info == NULL)
		return -1;

	*pminutes = -1;
	return attrs[i].raw;
}
