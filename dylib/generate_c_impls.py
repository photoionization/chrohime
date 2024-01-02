#!/usr/bin/env python3

import argparse
import copy
import json
import os
import re

def write_c_header_file(file, apis, public_header=False):
  if not public_header:
    file.write('// This file is automatically generated by write_c_impls.py\n\n'
               '#if defined(CHROHIME_C_IMPLEMENTATION)\n')
    # Write the typedefs for actual C++ implementations.
    for api in apis:
      if api['type']['type'] in [ 'refcounted', 'class' ]:
        if is_content_api(api):
          file.write('#if defined(CHROHIME_WITH_CONTENT)\n')
        file.write(f'typedef {api["type"]["cpp"]}* {api["type"]["c"]};\n'
                   f'typedef const {api["type"]["cpp"]}* {api["type"]["c"][:-1]}const_t;\n')
        if is_content_api(api):
          file.write('#endif\n')
    file.write('#else\n')
  # Write opque pointer typedefs for library users.
  file.write('// Declarations for opaque pointers.\n')
  for api in apis:
    if api['type']['type'] in [ 'refcounted', 'class' ]:
      file.write(f'typedef struct priv_{api["type"]["c"]}* {api["type"]["c"]};\n'
                 f'typedef const struct priv_{api["type"]["c"]}* {api["type"]["c"][:-1]}const_t;\n')
  if not public_header:
    file.write('#endif\n')
  file.write('\n')
  for api in apis:
    if api['type']['type'] in [ 'enum', 'enum class' ]:
      file.write('typedef enum {\n')
      for enum in api['enums']:
        if public_header and 'description' in enum:
          file.write(prefix_each_line(get_comment(enum), '  '))
        file.write(f'  {enum["c"]}')
        if 'value' in enum:
          file.write(f' = {enum["value"]}')
        file.write(',\n')
      file.write(f'}} {api["type"]["c"]};\n\n')
  file.write('#if defined(__cplusplus)\n'
             'extern "C" {\n'
             '#endif\n\n')
  for api in apis:
    write_c_impls(file, api, write_impl=False, public_header=public_header)
  file.write('#if defined(__cplusplus)\n'
             '}\n'
             '#endif\n')

def write_c_impl_file(file, apis):
  file.write('// This file is automatically generated by write_c_impls.py\n\n')
  # Write converters between structures.
  file.write('namespace {\n\n')
  for api in apis:
    if api['type']['type'] in [ 'struct', 'geometry', 'enum', 'enum class' ]:
      write_converters(file, api)
  # Write a helper for converting c struct array to vectors.
  file.write('template<typename T, typename F>\n'
             'std::vector<std::remove_const_t<T>> ToHimeVector(F* from, size_t from_size) {\n'
             '  std::vector<std::remove_const_t<T>> result;\n'
             '  result.reserve(from_size);\n'
             '  for (size_t i = 0; i < from_size; ++i)\n'
             '    result.push_back(ToHime(from + i));\n'
             '  return result;\n'
             '}\n\n')
  file.write('}  // namespace\n\n')
  # Write the implementations of APIs.
  for api in apis:
    write_c_impls(file, api, write_impl=True)

def write_converters(file, api):
  if api['type']['type'] == 'struct':
    set_properties = []
    for prop in api['properties']:
      prop_arg = get_c_arg(prop, 'c->')
      set_properties.append(f'cpp.{prop["id"]} = {prop_arg};\n')
    write_function(file, api, True,
                   f'{api["type"]["cpp"]} ToHime',
                   [ f'{api["type"]["c"]}* c' ],
                   # TODO(zcbenz): Make use of struct_size to implement ABI
                   # compatibility with old versions.
                   f'CHECK_EQ(c->struct_size, sizeof({api["type"]["c"]}))\n'
                   f'    << "Struct size does not match.";\n'
                   f'{api["type"]["cpp"]} cpp;\n' + ''.join(set_properties) + \
                   f'return cpp;')
  elif api['type']['type'] == 'geometry':
    # Write a converter from c to cpp.
    args = [ f'c.{prop["id"]}' for prop in api['properties'] ]
    write_function(file, api, True,
                   f'inline {api["type"]["cpp"]} ToHime',
                   [ f'{api["type"]["c"]} c' ],
                   f'return {api["type"]["cpp"]}({params_join(args)});')
    # Write a converter from c to absl::optional.
    write_function(file, api, True,
                   f'inline absl::optional<{api["type"]["cpp"]}> ToHime',
                   [ f'{api["type"]["c"]}* ptr' ],
                   f'if (!ptr)\n'
                   f'  return absl::optional<{api["type"]["cpp"]}>();\n'
                   f'return ToHime(*ptr);')
    # Write a converter from cpp to c.
    set_properties = [ 'c.{0} = cpp.{0}();\n'.format(prop['id']) for prop in api['properties'] ]
    write_function(file, api, True,
                   f'inline {api["type"]["c"]} FromHime',
                   [ f'const {api["type"]["cpp"]}& cpp' ],
                   f'{api["type"]["c"]} c;\n' + ''.join(set_properties) + \
                   f'return c;')
  elif api['type']['type'] in [ 'enum', 'enum class' ]:
    # Write a converter from c to cpp.
    return_enum_classes = [
        f'  case {enum["c"]}:\n'
        f'    return {enum["cpp"]};\n' for enum in api['enums'] ]
    write_function(file, api, True,
                   f'inline {api["type"]["cpp"]} ToHime',
                   [ f'{api["type"]["c"]} c' ],
                   f'switch (c) {{\n' + ''.join(return_enum_classes) + \
                   f'}}\n')
    # Write a converter from cpp to c.
    return_enums = [
        f'  case {enum["cpp"]}:\n'
        f'    return {enum["c"]};\n' for enum in api['enums'] ]
    write_function(file, api, True,
                   f'inline {api["type"]["c"]} FromHime',
                   [ f'{api["type"]["cpp"]} cpp' ],
                   'switch (cpp) {\n' + ''.join(return_enums) + \
                   f'  default: return static_cast<{api["type"]["c"]}>(0);\n'
                   '}\n')

def write_c_impls(file, api, write_impl, public_header=False):
  if api['type']['type'] in [ 'struct', 'geometry' ]:
    write_struct_impl(file, api, write_impl, public_header)
  elif api['type']['type'] in [ 'refcounted', 'class' ]:
    if is_content_api(api):
      file.write('#if defined(CHROHIME_WITH_CONTENT)\n\n')
    write_class_impl(file, api, write_impl, public_header)
    if is_content_api(api):
      file.write('#endif  // defined(CHROHIME_WITH_CONTENT)\n\n')

def write_struct_impl(file, api, write_impl, public_header):
  if write_impl:
    return
  if public_header:
    file.write(get_comment(api))
  # Write the struct declaration.
  file.write(f'typedef struct {{\n')
  if api['type']['type'] == 'struct':
    file.write(f'  uint32_t struct_size;\n')
  for prop in api['properties']:
    file.write(f'  {prop["type"]["c"]} {prop["id"]};\n')
  file.write(f'}} {api["type"]["c"]};\n')
  # A define that sets the size member and init default values.
  set_properties = []
  if api['type']['type'] == 'struct':
    set_properties.append(f'({api["id"]})->struct_size = sizeof({api["type"]["c"]})')
  for prop in api['properties']:
    set_properties.append(f'({api["id"]})->{prop["id"]} = {prop["defaultValue"]["c"]}')
  file.write(f'#define {api["type"]["c"][:-1]}init({api["id"]}) \\\n')
  file.write(prefix_each_line('; \\\n'.join(set_properties), '  '))
  file.write('\n')

def write_class_impl(file, api, write_impl, public_header):
  # Write class_create constructor methods.
  for constructor in api['constructors']:
    if public_header:
      file.write(get_comment(constructor))
    constructor_args = params_join(get_c_args(api, constructor))
    constructor_call = f'new {api["type"]["cpp"]}({constructor_args})'
    if api['type']['type'] == 'refcounted':
      # The refcounted types require putting into scoped_refptr first.
      constructor_call = f'base::AdoptRef({constructor_call}).release()'
    write_function(file, constructor, write_impl,
                   f'{api["type"]["c"]} {constructor["c"]}',
                   get_c_params(api, constructor),
                   f'return {constructor_call};')
  # Write class_destroy methods.
  if api['type']['type'] == 'class' and len(api['constructors']) > 0:
    write_function(file, {}, write_impl,
                   f'void {api["type"]["c"][:-1]}destroy',
                   [ f'{api["type"]["c"]} {api["id"]}' ],
                   f'delete {api["id"]};')
  # Write class methods.
  for method in api['class_methods']:
    if public_header:
      file.write(get_comment(method))
    write_function(file, method, write_impl,
                   f'{method["returnType"]["c"]} {method["c"]}',
                   get_c_params(api, method),
                   f'return {api["type"]["cpp"]}::{get_function_call(api, method)};')
  # Write normal methods.
  for method in api['methods']:
    if public_header:
      file.write(get_comment(method))
    return_type = method['returnType']['c']
    params = get_c_params(api, method)
    if method['returnType']['name'] == 'string':
      raise ValueError('Returning string copy in APIs has not been implemented')
    write_function(file, method, write_impl,
                   f'{return_type} {method["c"]}',
                   params,
                   get_method_impl(api, method))
  # Write events.
  for event in api['events']:
    if public_header:
      file.write(get_comment(event))
    event_callback_type = f'{event["c"]}_callback'
    if not write_impl:
      # Write definition of callback type.
      event_parameters = get_c_params(api, event) + [ 'void* data' ]
      file.write(f'typedef {event["returnType"]["name"]} (*{event_callback_type})({params_join(event_parameters)});\n\n')
    callback_params = [
        f'{event_callback_type} callback',
        f'scoped_refptr<DataHolder> holder' ] + get_c_params(api, event)
    callback_args = get_c_args(api, event) + [ 'holder->data()' ]
    impl = (f'return {api["id"]}->{event["cpp"]}.Connect(base::BindRepeating(\n'
            f'    []({params_join(callback_params)}) {{\n'
            f'        return callback({params_join(callback_args)});\n'
            f'    }}, callback, base::MakeRefCounted<DataHolder>(data, free)));\n')
    write_function(file, event, write_impl,
                   f'int32_t {event["c"]}_connect_closure',
                   [ f'{api["type"]["c"]} {api["id"]}',
                     f'{event_callback_type} callback',
                     'void* data',
                     'hime_free_callback free' ],
                   impl);
    if not write_impl:
      # Write a event_connect define for simplifying API.
      file.write(f'#define {event["c"]}_connect({api["id"]}, callback, data) \\\n'
                 f'  {event["c"]}_connect_closure({api["id"]}, callback, data, NULL)\n\n')
    write_function(file, event, write_impl,
                   f'void {event["c"]}_disconnect',
                   [ f'{api["type"]["c"]} {api["id"]}',
                     'uint32_t signal_id' ],
                   f'{api["id"]}->{event["cpp"]}.Disconnect(signal_id);');

def write_function(file, data, write_impl, name, params, impl, export=True):
  defines = get_platform_defines(data)
  if len(defines) > 0:
    defined = ' || '.join([f'defined({define})' for define in  defines])
    file.write(f'#if {defined}\n')
  if export and not write_impl:
    file.write('CHROHIME_C_EXPORT\n')
  file.write(f'{name}({params_join(params)})')
  if write_impl:
    file.write(' {\n' + prefix_each_line(impl, '  ') + '}\n')
  else:
    file.write(';\n')
  if len(defines) > 0:
    file.write('#endif\n')
  file.write('\n')

def is_content_api(api):
  return 'content' in api and api['content']

def get_comment(data):
  if not 'description' in data:
    return ''
  lines = data['description'].splitlines()
  comments = ['// ' + line if len(line) > 0 else '//' for line in lines]
  return '\n'.join(comments) + '\n'

def get_function_call(api, func, first_arg_is_this=False):
  if first_arg_is_this:
    func = copy.copy(func)
    func['parameters'] = func['parameters'][1:]
  call = f'{func["name"]}({params_join(get_c_args(api, func))})'
  if first_arg_is_this:
    call = f'self->{call}'
  if func['returnType']['type'] in [ 'struct', 'geometry', 'enum', 'enum class' ]:
    call = f'FromHime({call})'
  if func['returnType']['name'].endswith('string ref'):
    return f'{call}.c_str()'
  return call

def get_method_impl(api, method):
  function_call = get_function_call(api, method, first_arg_is_this=True)
  if method['returnType']['name'] != 'void':
    return f'return {function_call};'
  else:
    return f'{function_call};'

def get_c_params(api, method):
  parameters = []
  for arg in method['parameters']:
    parameters.append(f'{arg["type"]["c"]} {arg["id"]}')
  return parameters

def get_c_arg(arg, prefix=''):
  arg_name = f'{prefix}{arg["id"]}'
  if arg['type']['name'].startswith('vector<'):
    return f'ToHimeVector<{arg["type"]["cpp"][12:-1]}>({arg_name}, {arg_name}_size)'
  elif arg['type']['type'] in [ 'struct', 'geometry', 'enum', 'enum class' ]:
    return f'ToHime({arg_name})'
  elif arg['type']['name'] in [ 'GURL' ]:
    return f'{arg["type"]["cpp"]}({arg_name})'
  else:
    return arg_name

def get_c_args(api, data):
  return [ get_c_arg(arg) for arg in data['parameters'] if arg['type']['type'] != 'size parameter']

def get_platform_defines(data):
  defines = []
  if 'platform' in data:
    platform = data['platform']
    if 'win' in platform:
      defines += [ '_WIN32' ]
    if 'mac' in platform:
      defines += [ '__APPLE__' ]
    if 'linux' in platform:
      defines += [ '__linux__' ]
  return defines

def prefix_each_line(string, prefix):
  lines = string.splitlines()
  prefixed_lines = [prefix + line for line in lines]
  return '\n'.join(prefixed_lines) + '\n'

def params_join(params):
  return ', '.join(params)

def main():
  parser = argparse.ArgumentParser(
      description='Generate C implementations for the APIs')
  parser.add_argument('--apis-json', required=True,
                      help='JSON file containing APIs')
  parser.add_argument('--output-dir', required=True,
                      help='Directory to save generated C header/impl files')
  parser.add_argument('--public-header', action='store_true', default=False,
                      help='Generate header for publishing')
  args = parser.parse_args()

  with open(args.apis_json, 'r') as json_file:
    apis = json.load(json_file)

  if args.public_header:
    with open(os.path.join(args.output_dir, 'chrohime.h'), 'w') as file:
      file.write('#ifndef CHROHIME_H_\n'
                 '#define CHROHIME_H_\n\n')
      # Merge the headers into one file.
      inc_file = os.path.join(__file__, '..', 'chrohime_c_impl.inc')
      with open(os.path.abspath(inc_file), 'r') as inc:
        file.write(inc.read())
      file.write('\n')
      # Start writing the API declarations.
      write_c_header_file(file, apis, public_header=True)
      file.write('\n#endif  // CHROHIME_H_\n')
  else:
    with open(os.path.join(args.output_dir, 'generated_impls.h'), 'w') as file:
      write_c_header_file(file, apis)

    with open(os.path.join(args.output_dir, 'generated_impls.cc'), 'w') as file:
      write_c_impl_file(file, apis)

if __name__ == '__main__':
  main()
