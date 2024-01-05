class NameConverter:
  def __init__(self, apis):
    self.apis = apis

  def get_final_name(self, data):
    name = strip_type_decorates(get_name(data))
    if name.startswith('gfx::'):
      name = name[5:]
    return name

  def get_c_name(self, data):
    return convert_to_snake_case(self.get_final_name(data).replace('::', '')
                                                          .replace(' ', '_'))

  def get_c_type_prefix(self, data):
    return f'hime_{self.get_c_name(get_type(data))}'

  def get_cpp_class_name(self, api):
    api_name = get_type(api)
    if api_name.startswith('const '):
      return self.get_cpp_class_name(api_name[6:])
    if api_name.endswith(' out'):
      return self.get_cpp_class_name(api_name[:-4])
    if api_name.startswith('gfx::'):
      if api_name.endswith('f'):
        return api_name[:-1] + 'F'
      else:
        return api_name
    elif api_name == 'buffer':
      return 'hime::Buffer'
    elif api_name in [ 'string', 'string ref' ]:
      return 'std::u16string'
    elif api_name in [ 'BlendMode', 'ClipOp', 'Color' ]:
      return f'Sk{api_name}'
    if self.get_type_of_type(api_name) == 'primitive':
      return api_name
    else:
      return f'hime::{api_name}'

  def get_cpp_type_name(self, api):
    api_name = get_type(api)
    if api_name.startswith('const '):
      return 'const ' + self.get_cpp_type_name(api_name[6:])
    if api_name.endswith(' out'):
      return self.get_cpp_type_name(api_name[:-4]) + '*'
    if api_name.startswith('gfx::'):
      if api_name.endswith('f'):
        return api_name[:-1] + 'F'
      else:
        return api_name
    elif api_name == 'string':
      return 'std::u16string'
    elif api_name == 'string ref':
      return 'std::u16string&'
    elif api_name == 'buffer':
      return 'hime::Buffer'
    elif api_name in [ 'BlendMode', 'ClipOp', 'Color' ]:
      return f'Sk{api_name}'
    elif api_name.startswith('vector'):
      return f'std::vector<{self.get_cpp_type_name(api_name[7:-1])}>'
    meta_type = self.get_type_of_type(api_name)
    if meta_type == 'primitive':
      return api_name
    elif meta_type in [ 'class', 'refcounted' ]:
      return f'hime::{api_name}*'
    else:
      return f'hime::{api_name}'

  def get_c_type_name(self, data, const=False):
    type_name = get_type(data)
    if type_name.startswith('const '):
      return self.get_c_type_name(type_name[6:], const=True)
    if type_name.endswith(' out'):
      return self.get_c_type_name(type_name[:-4]) + '*'
    if type_name in [ 'string', 'GURL' ]:
      # For string copy always pass const char*.
      return 'const char16_t*'
    if type_name == 'buffer':
      if const:
        return 'const unsigned char*'
      else:
        return 'unsigned char*'
    if type_name == 'char**':
      # Special case for Lifetime constructor.
      if const:
        return 'const char**'
      else:
        return 'char**'
    if type_name == 'string ref':
      if const:
        return 'const char16_t*'
      else:
        return 'char16_t*'
    elif type_name[0].isupper() or type_name.startswith('gfx::'):
      prefix = self.get_c_type_prefix(type_name)
      if const:
        return f'{prefix}_const_t'
      else:
        return f'{prefix}_t'
    elif type_name.startswith('vector<'):
      return self.get_c_type_name(type_name[7:-1]) + '*'
    elif type_name.startswith('optional<'):
      return self.get_c_type_name(type_name[9:-1]) + '*'
    else:
      return type_name

  def get_cpp_enum_name(self, api, enum):
    api_name = get_type(api)
    enum_name = get_name(enum)
    if api_name == 'ColorId':
      return 'ui::kColor' + enum_name
    return self.get_cpp_type_name(api) + '::k' + enum_name

  def get_c_enum_name(self, api, enum):
    api_name = get_type(api)
    enum_name = get_name(enum)
    return 'kHime' + api_name.replace('::', '') + enum_name

  def get_c_parameter_types(self, type_name):
    param_type = self.get_c_type_name(type_name)
    if type_name.startswith('const '):
      type_name = type_name[6:]
    if type_name == 'buffer' or \
       type_name.startswith('vector<') or \
       type_name.endswith(' out'):
      return [ param_type, 'size_t' ]
    elif self.get_type_of_type(type_name) == 'struct':
      return [ param_type + '*' ]
    else:
      return [ param_type ]

  def get_c_return_type(self, type_name):
    if type_name in [ 'string', 'buffer' ] or type_name.startswith('vector<'):
      return 'size_t'
    else:
      return self.get_c_type_name(type_name)

  def get_c_value(self, type_name, value):
    if type_name == 'bool':
      return 'true' if value else 'false'
    if type_name in [ 'string', 'string ref' ]:
      return f'u"{value}"'
    if self.get_type_of_type(type_name) in [ 'enum', 'enum class' ]:
      return self.get_c_enum_name(type_name, value)
    return str(value)

  def get_type_of_type(self, type_name):
    type_name = strip_type_decorates(type_name)
    if type_name.endswith(' out') or \
       type_name in [ 'argument size', 'property size' ]:
      return 'c-only type'
    api = self.get_api_from_type_name(type_name)
    if api:
      return api['type']
    else:
      return 'primitive'

  def get_api_from_type_name(self, type_name):
    try:
      return next(api for api in self.apis if api['name'] == type_name)
    except StopIteration:
      return None


def get_name(data):
  if isinstance(data, dict):
    return data['name']
  else:
    return data

def get_type(data):
  if isinstance(data, dict):
    return data['type']['name']
  else:
    return data

def strip_type_decorates(name):
  if name.startswith('const '):
    return strip_type_decorates(name[6:])
  elif name.startswith('vector<'):
    return strip_type_decorates(name[7:-1])
  elif name.startswith('optional<'):
    return strip_type_decorates(name[9:-1])
  else:
    return name

def convert_to_snake_case(string):
  snake_case_string = ''
  for index, char in enumerate(string):
    if char.isupper() and index != 0:
      snake_case_string += '_'
    snake_case_string += char.lower()
  return snake_case_string
