#!/usr/bin/env python3

import argparse
import os
import subprocess
import sys

from bootstrap import ROOT_DIR, SRC_DIR

def main():
  parser = argparse.ArgumentParser(description='Build Chrohime')
  parser.add_argument('target', nargs='?', default='chrohime',
                      help='Target build')
  parser.add_argument('--component', action='store_true', default=False,
                      help='Do component build')
  args, unknown_args = parser.parse_known_args()

  build_args = [ os.path.join(ROOT_DIR, 'build_chromium/build.py'),
                 '-C', 'Component' if args.component else 'Release',
                 args.target ] + unknown_args

  try:
    subprocess.check_call([ sys.executable ] + build_args,
                          cwd=os.path.join(SRC_DIR, 'out'))
  except KeyboardInterrupt:
    sys.exit(1)
  except subprocess.CalledProcessError as e:
    sys.exit(e.returncode)

if __name__ == '__main__':
  main()
