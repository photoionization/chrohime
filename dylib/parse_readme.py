#!/usr/bin/env python3

import argparse
import os
import platform
import sys
import yaml

def current_cpu():
  arch = platform.machine()
  if arch == 'AMD64' or arch == 'x86_64' or arch == 'x64':
    return 'x64'
  elif arch == 'ARM64':
    return 'arm64'
  elif arch.startswith('ARM'):
    return 'arm'
  else:
    raise ValueError(f'Unrecognized CPU architecture: {arch}')

def current_os():
  if sys.platform.startswith('linux'):
    return 'linux'
  elif sys.platform.startswith('win'):
    return 'win'
  elif sys.platform == 'darwin':
    return 'mac'
  else:
    raise ValueError(f'Unsupported platform: {sys.platform}')

def get_file_item_text(args, item):
  if args.with_content:
    name = 'chrohime_with_content'
  else:
    name = 'chrohime'
  if args.target_os == 'win':
    libprefix = ''
  else:
    libprefix = 'lib'
  if args.target_os == 'mac':
    shlib_ext = 'dylib'
  elif args.target_os == 'win':
    shlib_ext = 'dll'
  else:
    shlib_ext = 'so'

  filename = item['filename'].format(name=name,
                                     libprefix=libprefix,
                                     shlib_ext=shlib_ext)
  text = f'`{filename}` - '
  if 'description' in item:
    text += item['description']
  return text

def write_markdown(file, args, data, indent_level=0):
  spaces = '  ' * indent_level
  for index, item in enumerate(data):
    if isinstance(item, str):
      file.write(f'{spaces}{item}\n')
    elif isinstance(item, dict):
      if 'platform' in item and args.target_os not in item['platform']:
        continue
      if 'arch' in item and args.target_cpu not in item['arch']:
        continue
      if 'with_content' in item:
        if item['with_content'] != args.with_content:
          continue
      if 'list' in item:
        if 'description' in item:
          file.write(f'{spaces}* {item["description"]}\n')
        write_markdown(file, args, item['list'], indent_level + 1)
      elif 'filename' in item:
        file.write(f'{spaces}* {get_file_item_text(args, item)}\n')
      elif 'description' in item:
        file.write(f'{spaces}{item["description"]}\n')
    else:
      write_markdown(file, args, item, indent_level + 1)
    if indent_level == 0 and index != len(data) - 1:
      file.write('\n')

def main():
  parser = argparse.ArgumentParser()
  parser.add_argument('input', help='Path to the input YAML file')
  parser.add_argument('output', help='Path to the output Markdown file')
  parser.add_argument('--with-content', action='store_true',
                      help='The name of chrohime library')
  parser.add_argument('--target-cpu', default=current_cpu(),
                      help='Target CPU architecture')
  parser.add_argument('--target-os', default=current_os(),
                      help='Target operating system (win, mac, or linux)')
  args = parser.parse_args()

  with open(args.input, 'r') as f:
    data = yaml.safe_load(f)

  if args.output == '-':
    write_markdown(sys.stdout, args, data)
  else:
    with open(args.output, 'w') as f:
      write_markdown(f, args, data)

if __name__ == '__main__':
  main()
