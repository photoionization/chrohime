#!/usr/bin/env python3

import argparse
import json
import os
import yaml

from name_converter import NameConverter

def parse_apis(apis):
  converter = NameConverter(apis)
  result = []
  for raw in apis:
    type_name = raw['name']
    api = {
      'name': converter.get_clean_name(type_name),
      'type': get_type_info(converter, type_name,
                            converter.get_c_type_name(type_name)),
    }
    copy_common_keys(converter, api, raw)
    if 'inherit' in raw:
      api['inherit'] = get_type_info(converter, raw['inherit'],
                                     converter.get_c_type_name(raw['inherit']))

    if raw['type'] in [ 'enum', 'enum class' ]:
      enums = []
      for e in raw['enums']:
        enums.append({
          'c': converter.get_c_enum_name(api, e),
          'cpp': converter.get_cpp_enum_name(api, e),
        })
        copy_common_keys(converter, enums[-1], e)
      api['enums'] = enums

    elif raw['type'] in [ 'geometry', 'struct' ]:
      api['properties'] = parse_properties(converter, raw['properties'])

    elif raw['type'] in [ 'class', 'refcounted' ]:
      for func_type in [ 'class_methods', 'constructors', 'methods', 'events' ]:
        funcs = []
        for f in raw[func_type]:
          func = {}
          copy_common_keys(converter, func, f)
          func['c'] = converter.get_c_type_prefix(api) + '_' + converter.get_c_name(f)
          if func_type == 'events':
            func['cpp'] = converter.get_c_name(f['name'])
          else:
            func['cpp'] = f['name']
          func['parameters'] = parse_parameters(converter, f['args'])
          func['returnType'] = get_type_info(converter, f['returnType'],
                                             converter.get_c_return_type(f['returnType']))

          funcs.append(func)
        api[func_type] = funcs

    else:
      raise ValueError(f'Unexpected API type {api["metaType"]}')

    result.append(api)

  return result

def parse_parameters(converter, args):
  parameters = []
  for a in args:
    c_name = converter.get_c_name(a)
    c_types = converter.get_c_parameter_types(a['type'])
    parameters.append({
      'type': get_type_info(converter, a['type'], c_types[0]),
    })
    copy_common_keys(converter, parameters[-1], a)
    if len(c_types) > 1:
      assert(c_types[1] == 'size_t')
      parameters.append({
        'name': a['name'] + 'Size',
        'description': f'Size of {a["name"]}.',
        'type': get_type_info(converter, 'argument size', 'size_t'),
      })
      copy_common_keys(converter, parameters[-1], a)
  return parameters

def parse_properties(converter, props):
  properties = []
  for p in props:
    c_name = converter.get_c_name(p)
    c_types = converter.get_c_parameter_types(p['type'])
    properties.append({
      'type': get_type_info(converter, p['type'], c_types[0]),
      'defaultValue': p['defaultValue'],
      'cDefaultValue': converter.get_c_value(p['type'], p['defaultValue']),
    })
    copy_common_keys(converter, properties[-1], p)
    if len(c_types) > 1:
      assert(c_types[1] == 'size_t')
      properties.append({
        'name': p['name'] + 'Size',
        'description': f'Size of {p["name"]}.',
        'type': get_type_info(converter, 'property size', 'size_t'),
        'defaultValue': 0,
        'cDefaultValue': 0,
      })
      copy_common_keys(converter, properties[-1], p)
  return properties

def get_type_info(converter, type_name, c_type):
  info = {
    'name': type_name,
    'type': converter.get_type_of_type(type_name),
    'c': c_type,
    'cpp': converter.get_cpp_type_name(type_name),
  }
  if info['type'] != 'primitive':
    info['id'] = converter.get_c_name(type_name)
  return info

def copy_common_keys(converter, dest, src):
  for key in [ 'name', 'description', 'platform', 'content' ]:
    if key in src and not key in dest:
      dest[key] = src[key]
  if 'id' not in dest:
    dest['id'] = converter.get_c_name(dest['name'])

def get_c_params(api, method, include_this=False):
  parameters = []
  if include_this:
    parameters.append(f'{api["type"]["c"]} self')
  for arg in method['parameters']:
    parameters.append(f'{arg["type"]["c"]} {arg["id"]}')
  return ', '.join(parameters)

def fill_default_values(path, data):
  if data['type'] in [ 'class', 'refcounted' ]:
    for key in ['class_methods', 'constructors', 'methods', 'events']:
      data.setdefault(key, [])
      for item in data[key]:
        item.setdefault('args', [])
        if key == 'constructors':
          item.setdefault('returnType', data['name'])
        else:
          item.setdefault('returnType', 'void')
        if key == 'methods':
          item['args'].insert(0, {'name': 'self', 'type': data['name']})
  return data

def parse_files(files):
  apis = []
  for file in files:
    with open(file) as f:
      apis.append(fill_default_values(file, yaml.safe_load(f)))
  return apis

def strip_descriptions(data):
  if isinstance(data, dict):
    for key in list(data.keys()):
      if key == 'description':
        del data[key]
      else:
        strip_descriptions(data[key])
  elif isinstance(data, list):
    for i in range(len(data)):
      data[i] = strip_descriptions(data[i])
  return data

def write_json(data, output_file):
  new_json = json.dumps(data, indent=2)
  is_file_changed = True
  if os.path.isfile(output_file):
    with open(output_file, 'r') as f:
      if f.read() == new_json:
        is_file_changed = False
  if is_file_changed:
    with open(output_file, 'w') as f:
      f.write(new_json)

def main():
  parser = argparse.ArgumentParser(
      description='Parse API descriptions into JSON files.')
  parser.add_argument('files', nargs='+', help='Input YAML files')
  parser.add_argument('--output-dir', required=True,
                      help='Target dir to write JSON files')
  args = parser.parse_args()

  apis = parse_apis(parse_files(args.files))
  write_json(apis,
             os.path.join(args.output_dir, 'chrohime_apis.json'))
  # Strip descriptions so editing an API's description does not trigger a
  # re-compilation of C wrappers.
  write_json(strip_descriptions(apis),
             os.path.join(args.output_dir, 'chrohime_apis_barebone.json'))

if __name__ == '__main__':
  main()
