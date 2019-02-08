
# libsigmf

libsigmf is a header-only C++ library for working with [SigMF](https://github.com/gnuradio/sigmf) metadata. It is
provided under the Apache License 2.0 and the copyright notice can be found in NOTICE.

## Usage

### Including in your application

To use libsigmf, your application needs to `#include <sigmf.h>`.

One option is to include libsigmf (this git repo) as a submodule inside your project sources. A typical cmake
usage would look like the following (see the CMakeLists.txt in this examples directory for what this looks like
in real usage).

```cmake
add_executable(example_record_with_multiple_namespaces example_record_with_multiple_namespaces.cpp)
target_link_libraries(example_record_with_multiple_namespaces libsigmf)
target_include_directories(example_record_with_multiple_namespaces PRIVATE ${CMAKE_BINARY_DIR}/include)
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
