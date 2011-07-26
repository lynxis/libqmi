#!/usr/bin/env python
# -*- Mode: python; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
#
# This program is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free Software
# Foundation, version 2.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along with
# this program; if not, write to the Free Software Foundation, Inc., 51
# Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#
# Copyright (C) 2011 Red Hat, Inc.
#

class Parser:
    def __init__(self):
        pass

    def parseline(self, line):
        pass

    def parse(self, f):
        for line in f:
            self.parseline(line.strip())

class EntityParser(Parser):
    def __init__(self):
        self.byid = {}
        self.byname = {}

    def parseline(self, line):
        parts = line.split('^')
        if len(parts) < 4:
            return None
        entity = {
            'uniqueid': parts[0] + '.' + parts[1].replace('"', '').replace(',', '.'),
            'type': int(parts[0]),
            'key': parts[1],
            'name': parts[2].replace('"', ''),
            'struct': int(parts[3]),
            'format': None,
            'internal': True,
            'extformat': None,
        }
        if len(parts) > 4:
            entity['format'] = int(parts[4])
        if len(parts) > 5:
            entity['internal'] = int(parts[5]) != 0
        if len(parts) > 6:
            entity['extformat'] = int(parts[6])
        self.byid[entity['uniqueid']] = entity
        self.byname[entity['name']] = entity
        return entity

class StructParser(Parser):
    def __init__(self):
        self.byid = {}
        self.byname = {}

    def parseline(self, line):
        parts = line.split('^')
        if len(parts) < 8:
            return None
        struct = {
            'uniqueid': parts[0] + '.' + parts[1],
            'id': int(parts[0]),
            'order': int(parts[1]),
            'type': int(parts[2]),
            'value': int(parts[3]),
            'name': parts[4].replace('"', ''),
            'offset': int(parts[5]),
            'modtype': int(parts[6]),
            'modval': parts[7].replace('"', '')
        }
        self.byid[struct['uniqueid']] = struct
        self.byname[struct['name']] = struct
        return struct

class FieldParser(Parser):
    def __init__(self):
        self.byid = {}
        self.byname = {}

    def parseline(self, line):
        parts = line.split('^')
        if len(parts) < 6:
            return None
        field = {
            'uniqueid': int(parts[0]),
            'id': int(parts[0]),
            'name': parts[1].replace('"', ''),
            'size': int(parts[2]),
            'type': int(parts[3]),
            'typeval': int(parts[4]),
            'hex': int(parts[5]) != 0,
            'descid': None,
            'internal': False,
        }
        if len(parts) > 6:
            field['descid'] = int(parts[6])
        if len(parts) > 7:
            field['internal'] = int(parts[7])
        self.byid[field['uniqueid']] = field
        self.byname[field['name']] = field
        return field

class EnumParser(Parser):
    def __init__(self):
        self.byid = {}
        self.byname = {}

    def parseline(self, line):
        parts = line.split('^')
        if len(parts) < 4:
            return None
        enum = {
            'uniqueid': int(parts[0]),
            'id': int(parts[0]),
            'name': parts[1].replace('"', ''),
            'descid': int(parts[2]),
            'internal': int(parts[3]) != 0,
        }
        self.byid[enum['uniqueid']] = enum
        self.byname[enum['name']] = enum
        return enum

class EnumEntryParser(Parser):
    def __init__(self):
        self.byid = {}
        self.byname = {}

    def parseline(self, line):
        parts = line.split('^')
        if len(parts) < 3:
            return None
        entry = {
            'uniqueid': '%d.%d' % (int(parts[0]), int(parts[1], 0)),
            'id': int(parts[0]),
            'value': int(parts[1], 0),
            'name': parts[2].replace('"', ''),
        }
        if len(parts) > 3:
            entry['descid'] = int(parts[3])
        self.byid[entry['uniqueid']] = entry
        self.byname[entry['name']] = entry
        return entry

class DataSet:
    FIELD_BOOL = 0
    FIELD_INT8 = 1
    FIELD_UINT8 = 2
    FIELD_INT16 = 3
    FIELD_UINT16 = 4
    FIELD_INT32 = 5
    FIELD_UINT32 = 6
    FIELD_INT64 = 7
    FIELD_UINT64 = 8
    FIELD_STRING_A = 9
    FIELD_STRING_U = 10
    FIELD_STRING_ANT = 11
    FIELD_STRING_UNT = 12
    FIELD_FLOAT32 = 13
    FIELD_FLOAT64 = 14
    FIELD_STRING_U8 = 15
    FIELD_STRING_U8NT = 16
    FIELD_MAX = 16

    def __init__(self):
        self.enums = {}
        self.fields = {}
        self.structs = {}
        self.entities = {}

    def add_enum(self, enum):
        ent = { 'id': enum['uniqueid'], 'name': enum['name'], 'values': [] }
        self.enums[enum['uniqueid']] = ent

    def add_entry(self, entry):
        enum = self.enums[entry['id']]
        enum['values'].append({ 'value': entry['value'], 'name': entry['name'] })
        enum['values'].sort(lambda x, y: cmp(x['value'], y['value']))

    def add_field(self, field):
        if field['type'] > DataSet.FIELD_MAX:
            print 'Strange field %s' % field
        self.fields[field['id']] = field

    def add_struct(self, struct):
        # Each struct entry is really a 'struct fragment'; we can connect them
        # together by the pair of (id, order) (aka 'uniqueid').
        ent = {
            'id': struct['id'],
            'name': '<none>',
            'fields': [],
        }
        if struct['id'] in self.structs:
            ent = self.structs[struct['id']]
        self.structs[struct['id']] = ent

        field = {
            'type': struct['type'],
            'value': struct['value'],
        }
        ent['fields'].append(field)

    def add_entity(self, entity):
        ent = {
            'name': entity['name'],
            'struct': entity['struct'],
        }
        self.entities[entity['uniqueid']] = ent
        if self.structs[entity['struct']]:
            s = self.structs[entity['struct']]
            if s['name'] == '<none>':
                s['name'] = entity['name']
                pass

    def validate(self):
        for s in self.structs:
            struct = self.structs[s]
            for f in struct['fields']:
                if f['type'] == 0:
                    if f['value'] not in self.fields:
                        print 'No field: %d' % f['value']
                elif f['type'] == 1:
                    if f['value'] not in self.structs:
                        print 'No struct: %d' % f['value']
                elif f['type'] == 2:
                    pass
                elif f['type'] == 6:
                    pass
                else:
                    print 'Surprising struct field: %s' % f
        for e in self.entities:
            ent = self.entities[e]
            if ent['struct'] not in self.structs:
                print 'Entity missing struct: %s' % ent

    def nicename(self, name):
        return name.replace(' ', '_').replace('/', '_').lower().split('-')[0].strip('_')

    def constname(self, name):
        return name.replace(' ', '_').replace('/', '_').upper().split('-')[0].strip('_')

    def emitfield(self, f):
        basetypes = {
            DataSet.FIELD_BOOL: 'bool',
            DataSet.FIELD_INT8: 'int8',
            DataSet.FIELD_UINT8: 'uint8',
            DataSet.FIELD_INT16: 'int16',
            DataSet.FIELD_UINT16: 'uint16',
            DataSet.FIELD_INT32: 'int32',
            DataSet.FIELD_UINT32: 'uint32',
        }
        if f['type'] == 0:
            _f = self.fields[f['value']]
            print "\t%s %s; /* %s */" % (basetypes[_f['type']], self.nicename(_f['name']), _f['name'])
        else:
            print "\t???";

    def emit(self):
        print '/* GENERATED CODE. DO NOT EDIT. */'
        for e in self.enums:
            enum = self.enums[e]
            print 'enum gobi_%s { /* %s */ ' % (self.nicename(enum['name']), enum['name'])
            for en in enum['values']:
                print "\tGOBI_%s_%s\t\t= 0x%08x,     /* %s */" % (
                                                    self.constname(enum['name']),
                                                    self.constname(en['name']),
                                                    en['value'], en['name'])
            print "};\n"
        for s in self.structs:
            struct = self.structs[s];
            if struct['name'] != '<none>':
                struct['printname'] = 'gobi_' + self.nicename(struct['name'])
            else:
                struct['printname'] = 'gobi_%d' % struct['id']
            print 'struct %s; /* %s */' % (struct['printname'], struct['name'])
        for s in self.structs:
            struct = self.structs[s]
            print 'struct %s { /* %s */' % (struct['printname'], struct['name'])
            for f in struct['fields']:
                self.emitfield(f)
            print "};\n"

import sys

if len(sys.argv) > 2:
    print "Usage: qmidb.py <path to Entity.txt>"
    sys.exit(1)
path = ""
if len(sys.argv) == 2:
    path = sys.argv[1] + "/"

entities = EntityParser()
entries = EnumEntryParser()
enums = EnumParser()
fields = FieldParser()
structs = StructParser()
entities.parse(file(path + 'Entity.txt'))
entries.parse(file(path + 'EnumEntry.txt'))
enums.parse(file(path + 'Enum.txt'))
fields.parse(file(path + 'Field.txt'))
structs.parse(file(path + 'Struct.txt'))
print 'Loaded: %d entities, %d enums (%d entries), %d fields, %d structs' % (
    len(entities.byid),
    len(enums.byid),
    len(entries.byid),
    len(fields.byid),
    len(structs.byid))

ds = DataSet()
for e in enums.byid:
    ds.add_enum(enums.byid[e])
for e in entries.byid:
    ds.add_entry(entries.byid[e])
for f in fields.byid:
    ds.add_field(fields.byid[f])
for s in structs.byid:
    ds.add_struct(structs.byid[s])
for e in entities.byid:
    ds.add_entity(entities.byid[e])
ds.validate()
ds.emit()
