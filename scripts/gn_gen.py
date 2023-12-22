#!/usr/bin/env python3

import argparse
import os
import subprocess
import sys

from bootstrap import ROOT_DIR, SRC_DIR

def main():
  parser = argparse.ArgumentParser(description='Generate GN configs')
  args, unknown_args = parser.parse_known_args()

  gn_args = [
    # On Linux allocator shim is not working fine for shared libaries, the
    # dynamic libraries somehow prefers libc's allocator instead of Chromium's
    # one and results in crashes.
    # We probably want to fix it eventually, but for now let's just disable
    # the allocator shim for all.
    'use_allocator_shim=false',
    'use_partition_alloc_as_malloc=false',
    'enable_backup_ref_ptr_support=false',
    # The C APIs allow passing function pointers.
    'is_cfi=false',
  ]

  gn_gen_args = [ os.path.join(ROOT_DIR, 'scripts/build_chromium/gn_gen.py'),
                  '--src-dir', SRC_DIR ]
  for arg in gn_args:
    gn_gen_args += [ '--arg', arg ]
  gn_gen_args += unknown_args

  try:
    subprocess.check_call([ sys.executable ] + gn_gen_args, cwd=ROOT_DIR)
  except KeyboardInterrupt:
    sys.exit(1)
  except subprocess.CalledProcessError as e:
    sys.exit(e.returncode)

if __name__ == '__main__':
  main()
