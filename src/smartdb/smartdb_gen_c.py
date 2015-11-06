#!/usr/bin/env python

import sys
import xml.etree.ElementTree as ET

tree = ET.parse(sys.argv[1])
root = tree.getroot()

assert root.tag == 'smartdb'

names = {}
defaults = {}

raw_types = {
        'hex48': 'SMART_ATTR_RAW_HEX48',
        'dec48': 'SMART_ATTR_RAW_DEC48',
}
raw_type_default = 'dec48'
def raw_type_to_enum(raw):
    return raw_types.get(raw)

attr_code = {
    'none': 'SMART_ATTR_TYPE_NONE',
    'poh': 'SMART_ATTR_TYPE_POH',
    'temperature': 'SMART_ATTR_TYPE_TEMP',
    'reallocations': 'SMART_ATTR_TYPE_REALLOC',
    'pending_reallocations': 'SMART_ATTR_TYPE_REALLOC_PENDING',
    'crc_count': 'SMART_ATTR_TYPE_CRC_ERRORS',
}
attr_code_default = 'none'
def attr_code_to_enum(code):
    try:
        return attr_code[code]
    except KeyError:
        sys.stderr.write('Cannot find key "%s" in the known attribute code list: %s\n' % (code, attr_code.keys()))
        raise

def validate_name(c, root):
    assert root.tag == 'name'
    name = root.text.strip()
    assert len(name) > 0
    for child in root:
        assert False, 'node cant have a child'
    assert name not in names
    names[name] = root.get('type', attr_code_default)

def validate_attr(d, root):
    assert root.tag == 'attr'
    aid = None
    name = None
    tempoffset = -1
    raw = raw_type_default
    code = attr_code_default
    for child in root:
        assert child.tag in ('id', 'name', 'raw', 'tempoffset')
        if child.tag == 'id':
            val = int(child.text)
            assert val > 0
            assert val <= 255
            aid = val
        elif child.tag == 'name':
            val = child.text.strip()
            assert val in names
            name = val
            code = names.get(name)
        elif child.tag == 'raw':
            val = child.text.strip()
            assert val in list(raw_types.keys())
            raw = val
        elif child.tag == 'tempoffset':
            val = int(child.text)
            assert val > 0 and val < 255
            tempoffset = val
        d[aid] = (aid, name, raw, code, tempoffset)

nodes = {
        'names': validate_name,
        'default': validate_attr,
}

for child in root:
    if child.tag not in list(nodes.keys()):
        raise 'tag %s is unknown at smartdb level' % child.tag
    vfunc = nodes.get(child.tag)
    for subchild in child:
        vfunc(defaults, subchild)

print('#include "smartdb.h"')

print('static const smart_table_t defaults = {')
print('.num_attrs = %d,' % len(defaults))
print('.attrs = {')
keys = list(defaults.keys())
keys.sort()
for aid in keys:
    attr = defaults[aid]
    name = attr[1]
    raw = raw_type_to_enum(attr[2])
    atype = attr_code_to_enum(attr[3])
    tempoffset = attr[4]
    print('{.id=%d, .type=%s, .name="%s", .raw=%s, .offset=%d},' % (aid, atype, name, raw, tempoffset))
print('}')
print('};')

print('const smart_table_t * smart_table_for_disk(const char *vendor, const char *model, const char *firmware)')
print('{')
print('(void)vendor;')
print('(void)model;')
print('(void)firmware;')
print('return &defaults;')
print('}')
