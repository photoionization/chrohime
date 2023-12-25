#!/usr/bin/env python3

import argparse
import os
import shutil
import subprocess
import sys
import tempfile
import time
import zipfile

from bootstrap import SRC_DIR

def find_dists():
  release_dir = os.path.join(SRC_DIR, 'out', 'Release')
  files = os.listdir(release_dir)
  return [os.path.join(release_dir, f) for f in files if f.endswith('.zip')]

def extract_zip(zip_path, target_dir):
  with zipfile.ZipFile(zip_path, 'r') as zip_ref:
    zip_ref.extractall(target_dir)

def run_cmake(target_dir):
  subprocess.check_call([ 'cmake', '.' ], cwd=target_dir)
  subprocess.check_call([ 'cmake', '--build', '.' ], cwd=target_dir)

def run_gallery_test(target_dir):
  if sys.platform.startswith('win'):
    gallery = os.path.join(target_dir, 'Debug', 'gallery.exe')
  else:
    gallery = os.path.join(target_dir, 'gallery')
  subprocess.check_call([ gallery, '--test' ], cwd=target_dir)

def main():
  parser = argparse.ArgumentParser(description='Test distribution zip')
  parser.add_argument('dists', nargs='*', default=find_dists(),
                      help='Path to the dist zip')
  args = parser.parse_args()

  for dist in args.dists:
    try:
      print('Testing', os.path.relpath(dist, SRC_DIR))
      temp_dir = tempfile.mkdtemp()
      extract_zip(dist, temp_dir)
      run_cmake(temp_dir)
      run_gallery_test(temp_dir)
    finally:
      try:
        shutil.rmtree(temp_dir)
      except PermissionError:
        # The child processes might take a while to quit.
        time.sleep(2)
        shutil.rmtree(temp_dir)

if __name__ == '__main__':
  main()
