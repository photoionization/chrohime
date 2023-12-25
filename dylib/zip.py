#!/usr/bin/env python3
#
# Copyright 2014 The Chromium Authors
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

# This file is modified from build/android/gyp/zip.py.

"""Archives a set of files."""

import argparse
import os
import pathlib
import posixpath
import sys
import zipfile

# Adds //build to sys.path.
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__),
                                             os.pardir, os.pardir, os.pardir,
                                             'chromium', 'build')))
import action_helpers
import zip_helpers


def main(args):
  parser = argparse.ArgumentParser(args)
  parser.add_argument('--input-files', help='GN-list of files to zip.')
  parser.add_argument(
      '--ignore-stripped-suffix',
      action='store_true',
      help='Ignore the .stripped suffix when adding files to zip.')
  parser.add_argument(
      '--input-files-base-dir',
      help='Paths in the archive will be relative to this directory')
  parser.add_argument('--output', required=True, help='Path to output archive.')
  options = parser.parse_args(args)

  with action_helpers.atomic_output(options.output) as f:
    with zipfile.ZipFile(f.name, 'w',
                         # zipfile.ZIP_BZIP2 can generate smaller files but will
                         # trigger error in old versions of unzip utility.
                         compression=zipfile.ZIP_DEFLATED,
                         compresslevel=6) as out_zip:
      if options.input_files:
        base_dir = options.input_files_base_dir
        files = action_helpers.parse_gn_list(options.input_files)
        input_tuples = []
        for path in files:
          src_path = path
          zip_path = os.path.relpath(src_path, base_dir)
          # Zip files always use / as path separator.
          if os.path.sep != posixpath.sep:
            zip_path = str(pathlib.Path(zip_path).as_posix())
          if options.ignore_stripped_suffix and zip_path.endswith('.stripped'):
            zip_path = zip_path[:-9]
          input_tuples.append((zip_path, src_path))
        zip_helpers.add_files_to_zip(input_tuples,
                                     out_zip,
                                     base_dir=base_dir)


if __name__ == '__main__':
  main(sys.argv[1:])
