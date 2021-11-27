
# libsigmf

`libsigmf` is a header-only C++ library for working with [SigMF](https://github.com/gnuradio/sigmf) metadata. It is
provided under the Apache License 2.0 and the copyright notice can be found in NOTICE.

## Updates in libsigmf v1.0

Version 1.0 has several changes from earlier versions, most notably is the requirement to use optional (nullable)
scalar fields which became supported in flatbuffers since the library was originally published (v0.1). This means
scalar values are ALL implemented as std::optional, which is a change for applications using `libsigmf`. This
change was made to avoid the situation where default values are either not populated or always populated. Now all
fields will be included in the output if and only if they are set.

Other v1.0 changes include: updating to the latest SigMF metadata fields in the included schemas (including
redefining all scalar fields as optional via `=null;`), updating to flatbuffers v2.0.0 syntax, cmake updates,
updates to examples to ensure optional fields are working correctly, update to require c++17 for std::optional.

To update existing code from flatbuffers v0.1 and earlier to v1.0, accessing a field like this:

```c++
annotation_sample_start = annotation.sample_start;
```

is no loger a valid as the `annotation.sample_start` field is not a uint64_t, its a std::optional, it must now 
e checked to see if it has been set, then accessed via a legal means such:

```c++
if (annotation.sample_start.has_value()) {
  annotation_sample_start = annotation.sample_start.value();
} else {
  // what to do if the "sample_start" field is missing
}
```

Users can also preserve existing behavior (where all scalar fields had a default value of zero) with:

```c++
annotation_sample_start = annotation.sample_start.value_or(0);
```

or any other default value of choice.

Generation of JSON is unaffected other than it is no longer required to choose between "all fields" or "no fields
with default values".

## Limitations of libsigmf v1.0

- Subtables are not working correctly, thus the `core:extensions` and `core:geolocation` are not supported yet.
- Only the `antenna` extension fbs schema is included for now.
- Collections are not supported yet.
- Formal test code is still a WIP, examples provide some code coverage.

## Building

libsigmf depends on the following packages:

- nlohmann-json3-dev (3.7.3 preferred, other versions may work)
- libflatbuffers-dev (2.0.0 required as of SigMF v1.0)

These dependencies are included as submodules that can be built alongside libsigmf (default), or system installed
libraries can be used by passing `-DUSE_SYSTEM_JSON=ON -DUSE_SYSTEM_FLATBUFFERS=ON` to cmake. It is recommended
that users install flatbuffers v2.0 system wide as most applications using libsigmf will also need this.

Build with the standard cmake process:

```
mkdir build
cd build
cmake ../ # OR: cmake -DUSE_SYSTEM_JSON=ON -DUSE_SYSTEM_FLATBUFFERS=ON ../
make -j
sudo make install
```

## Usage

### Including in your application

To use libsigmf, your application needs to `#include <sigmf.h>` and link to `libflatbuffers.so`.

One option is to include libsigmf (this git repo) as a submodule inside your project sources. A typical cmake
usage would look like the following (see the CMakeLists.txt in this examples directory for what this looks like
in real usage).

```cmake
add_executable(example_record_with_multiple_namespaces example_record_with_multiple_namespaces.cpp)
target_link_libraries(example_record_with_multiple_namespaces libsigmf::libsigmf)
target_include_directories(example_record_with_multiple_namespaces PRIVATE ${CMAKE_BINARY_DIR}/include)
```

Ideally you install `libsigmf` either system-wide or in a prefix. Provided CMake configuration
then enables you to include `libsigmf` in your project by including this

```cmake
find_package(libsigmf REQUIRED)
add_executable(my_awesome_record my_awesome_record.cpp)
target_link_libraries(my_awesome_record libsigmf::libsigmf)
```

### Code usage

libsigmf internally has a class `sigmf::VariadicDataClass` that does the heavy lifting of keeping objects of collected
metadata using different SigMF namespaces. As a convenience, there are 4 classes that directly deal with SigMF
objects that are all you need to use:

1) `sigmf::SigMF`
2) `sigmf::Global`
3) `sigmf::Capture`
4) `sigmf::Annotation`

## Contributing

Please see `CONTRIBUTING.md` for more information!
