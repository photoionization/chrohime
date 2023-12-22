#!/usr/bin/env python3

import argparse
import os
import subprocess
import sys

from bootstrap import ROOT_DIR, SRC_DIR, BUILD_CHROMIUM_DIR

def main():
  parser = argparse.ArgumentParser(description='Build Chrohime')
  parser.add_argument('targets', nargs='*', default=[ 'chrohime_gallery' ],
                      help='Target build')
  parser.add_argument('--config', choices=[ 'Component', 'Release', 'Debug' ],
                      default='Component',
                      help='Which config to build')
  args, unknown_args = parser.parse_known_args()

  build_args = [ os.path.join(BUILD_CHROMIUM_DIR, 'build.py'),
                 '--src-dir', SRC_DIR,
                 '-C', os.path.join('out', args.config) ] + unknown_args + args.targets

  try:
    subprocess.check_call([ sys.executable ] + build_args)
  except KeyboardInterrupt:
    sys.exit(1)
  except subprocess.CalledProcessError as e:
    sys.exit(e.returncode)

if __name__ == '__main__':
  main()
