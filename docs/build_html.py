#!/usr/bin/env python3

import argparse
import os
import subprocess
import sys

DOCS_DIR = os.path.dirname(os.path.abspath(__file__))

def main():
  parser = argparse.ArgumentParser(description='Generate HTML documentations.')
  parser.add_argument('--apis-json', required=True,
                      help='Paths to chrohime_apis.json File')
  parser.add_argument('--version', required=True,
                      help='Version of Chrohime')
  parser.add_argument('--index-template', required=True,
                      help='Path to the template file of index page')
  parser.add_argument('--guide-template', required=True,
                      help='Path to the template file of guide page')
  parser.add_argument('--api-template', required=True,
                      help='Path to the template file of api page')
  parser.add_argument('--output-dir', required=True,
                      help='Target dir to write HTML files')
  parser.add_argument('files', nargs='+', help='Input files')
  args = parser.parse_args()

  try:
    subprocess.check_call([
      'node', os.path.join(DOCS_DIR, 'build_html.js'),
      args.apis_json, args.version, args.index_template, args.guide_template,
      args.api_template, args.output_dir ] + args.files)
  except KeyboardInterrupt:
    sys.exit(1)
  except subprocess.CalledProcessError as e:
    sys.exit(e.returncode)

if __name__ == '__main__':
  main()
