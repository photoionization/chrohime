#!/usr/bin/env python3

import argparse
import json
import os
import yaml

def fill_default_values(data):
  if data['type'] in [ 'class', 'refcounted' ]:
    for key in ['class_methods', 'constructors', 'methods', 'events']:
      data.setdefault(key, [])
      for item in data[key]:
        item.setdefault('args', [])
        if key == 'constructors':
          item.setdefault('returnType', data['name'])
        else:
          item.setdefault('returnType', 'void')
  return data

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

def parse_files(files):
  data = {'apis': []}
  for file in files:
    with open(file) as f:
      data['apis'].append(fill_default_values(yaml.safe_load(f)))
  return data

def write_json(data, output_file):
  is_file_changed = True
  if os.path.isfile(output_file):
    with open(output_file, 'r') as f:
      try:
        old_data = json.load(f)
        if old_data == data:
          is_file_changed = False
      except json.JSONDecodeError:
        pass
  if is_file_changed:
    with open(output_file, 'w') as f:
      json.dump(data, f, indent=2)

def main():
  parser = argparse.ArgumentParser(
      description='Parse API descriptions into JSON files.')
  parser.add_argument('files', nargs='+', help='Input YAML files')
  parser.add_argument('--output-dir', required=True,
                      help='Target dir to write JSON files')
  args = parser.parse_args()

  data = parse_files(args.files)
  write_json(data,
             os.path.join(args.output_dir, 'chrohime_apis.json'))
  # Strip descriptions so editing an API's description does not trigger a
  # re-compilation of C wrappers.
  write_json(strip_descriptions(data),
             os.path.join(args.output_dir, 'chrohime_apis_barebone.json'))

if __name__ == '__main__':
  main()
