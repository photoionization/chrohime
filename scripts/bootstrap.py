#!/usr/bin/env python3

import os
import subprocess
import sys

CHROMIUM_VERSION = '122.0.6182.0'

ROOT_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC_DIR = os.path.join(ROOT_DIR, 'build_chromium', 'src')

# Append scripts/src_import.gni file to src/BUILD.gn.
def append_to_build_gn():
  build_gn = os.path.join(SRC_DIR, 'BUILD.gn')
  with open(build_gn, 'r') as file:
    content_gn = file.read()
  src_import = os.path.join(ROOT_DIR, 'scripts/src_import.gni')
  with open(src_import, 'r') as file:
    content_import = file.read()

  if content_gn.endswith(content_import):
    return

  import_prefix = '\ngroup("chrohime") {'
  if import_prefix in content_gn:
    content_gn = content_gn[:content_gn.index(import_prefix)]

  with open(build_gn, 'w') as file:
    file.write(content_gn + '\n' + content_import)

def main():
  subprocess.check_call([
      sys.executable,
      os.path.join(ROOT_DIR, 'build_chromium', 'bootstrap.py'),
      '--revision', CHROMIUM_VERSION,
      '--src-dir', SRC_DIR ] + sys.argv[1:])

  if not os.path.exists(os.path.join(SRC_DIR, 'chrohime')):
    os.symlink(os.getcwd(), os.path.join(SRC_DIR, 'chrohime'),
               target_is_directory=True)

  append_to_build_gn()

if __name__ == '__main__':
  exit(main())
