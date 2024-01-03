---
priority: 20
description: Walkthrough of object lifetime management.
---

# Object types

There are mainly 4 types of objects in Chrohime, the design was made with
API/ABI stability in mind.

## Geometry

Geometry types like `Rect` and `Size`, are simple C structures. For APIs that
take and return them, they are all passed by value.

```c
hime_rect_t rect = {0, 0, 100, 100};
hime_window_set_bounds(win, rect);
hime_size_t size = hime_window_get_size(win);
```

## Struct

Objects that do not have methods belong to struct types, for example window
options and table columns. It is developer's responsibility to allocate memory
for them, and APIs always take pointer as parameters for struct types.

The difference between struct and geometry types is, struct types may have new
properties added in future, but gemotry types will never change.

After allocating memory for struct types (usually on stack), developer must
call the corresponding `xxx_init` functions to initialize them, which set the
hidden `struct_size` property and assign default values for properties.

```c
hime_window_options_t options;
hime_window_options_init(&options);
hime_window_t window = hime_window_create(&options);
```

## RefCounted

Objects that have methods and inherit from the `Object` class are refcounted
types. Their lifetime are managed by ref-counting and for each refcounted object
created you must call `hime_object_unref` to deference them once you are done
with them.

```c
hime_view_t view = hime_view_create();
hime_object_unref((hime_object_t)view);
```

Unlike geometry and struct types, the memory of refcounted types are always
allocated in heap by Chrohime APIs, i.e. types like `hime_view_t` and
`hime_window_t` are opaque pointers.

When passing refcounted types as arguments in function calls, the callee is free
to retain a reference to the passed object.

```c
hime_view_t child = hime_view_create();
hime_view_add_child_view(view, child);
hime_object_unref((hime_object_t)child);
// `child` is still alive due to being retained by `view`.
```

## Class

Class types are very similar to refcounted types, except that class objects do
not inherit from `Object` and are not ref-counted. For each object created with
`xxx_create` API you can call `xxx_destroy` to release their memory.

```c
hime_state_t state = hime_state_create();
hime_state_destroy(state);
```
