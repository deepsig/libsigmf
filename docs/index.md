
# libsigmf

libsigmf is a C++ library for working with [SigMF](https://github.com/gnuradio/sigmf) records. The primary
design goals are to be flexible with mixing SigMF namesapces using static types so that errors in namespace
usage can be caught at compile time, memory usage is low while working with objects, and accessing fields
has low overhead.

To accomplish this we have taken advantage of the C preprocessor and C++ templates. The result allows creating
objects like so
