/*
 *    Copyright 2019 DeepSig Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#ifndef LIBSIGMF_ANNOTATIONS_H
#define LIBSIGMF_ANNOTATIONS_H

#include "variadic_data_class.h"

/*
 * Helper macros-- mixed opinions abound!
 */
#define ca0(x0) Annotation<x0::DescrT>

#define ca1(x0, x1) Annotation<x0::DescrT, x1::DescrT>

#define ca2(x0, x1, x2) Annotation<x0::DescrT, x1::DescrT, x2::DescrT>

#define ca3(x0, x1, x2, x3) Annotation<x0::DescrT, x1::DescrT, x2::DescrT, x3::DescrT>

#define ca4(x0, x1, x2, x3, x4) Annotation<x0::DescrT, x1::DescrT, x2::DescrT, \
x3::DescrT, x4::DescrT>

#define ca5(x0, x1, x2, x3, x4, x5) Annotation<x0::DescrT, x1::DescrT, x2::DescrT, \
x3::DescrT, x4::DescrT, x5::DescrT>

#define ca6(x0, x1, x2, x3, x4, x5, x6) Annotation<x0::DescrT, x1::DescrT, x2::DescrT, \
x3::DescrT, x4::DescrT, x5::DescrT, x6::DescrT>

#define ca7(x0, x1, x2, x3, x4, x5, x6, x7) Annotation<x0::DescrT, x1::DescrT, x2::DescrT, \
x3::DescrT, x4::DescrT, x5::DescrT, x6::DescrT, x7::DescrT>

#define ca8(x0, x1, x2, x3, x4, x5, x6, x7, x8) Annotation<x0::DescrT, x1::DescrT, x2::DescrT, \
x3::DescrT, x4::DescrT, x5::DescrT, x6::DescrT, x7::DescrT, x8::DescrT>

#define ca9(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9) Annotation<x0::DescrT, x1::DescrT, x2::DescrT, \
x3::DescrT, x4::DescrT, x5::DescrT, x6::DescrT, x7::DescrT, x8::DescrT, x9::DescrT>


#define GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME, ...) NAME
#define create_annotation(...) GET_MACRO(__VA_ARGS__, ca9, ca8, ca7, ca6, ca5, ca4, ca3, ca2, ca1, ca0)(__VA_ARGS__)

#define sigmfns(x) get<x::DescrT>()


namespace sigmf {

    // This template is mostly here so we can specialize it. If we can get an expansion/loop of T... so that for every
    // T we can create a VariadicDataClass<T::annotation, then we'd be able to eliminate all of the specializations
    template<typename...T>
    class Annotation : public VariadicDataClass<T...> {
    };

    template<typename T>
    class Annotation<T>
            : public VariadicDataClass<typename std::pointer_traits<decltype(T::annotation)>::element_type> {
    public:
        template<typename U>
        auto &get() {// -> typename std::pointer_traits<decltype(U::annotation)>::element_type& {
            return this->template access<typename std::pointer_traits<decltype(U::annotation)>::element_type>();
        }
    };

    template<typename T, typename T2>
    class Annotation<T, T2>
            : public VariadicDataClass<typename std::pointer_traits<decltype(T::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T2::annotation)>::element_type> {
    public:
        template<typename U>
        auto &get() {
            return this->template access<typename std::pointer_traits<decltype(U::annotation)>::element_type>();
        }
    };

    template<typename T, typename T2, typename T3>
    class Annotation<T, T2, T3>
            : public VariadicDataClass<typename std::pointer_traits<decltype(T::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T2::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T3::annotation)>::element_type> {
    public:
        template<typename U>
        auto &get() {
            return this->template access<typename std::pointer_traits<decltype(U::annotation)>::element_type>();
        }
    };


    template<typename T, typename T2, typename T3, typename T4>
    class Annotation<T, T2, T3, T4>
            : public VariadicDataClass<typename std::pointer_traits<decltype(T::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T2::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T3::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T4::annotation)>::element_type> {
    public:
        template<typename U>
        auto &get() {
            return this->template access<typename std::pointer_traits<decltype(U::annotation)>::element_type>();
        }
    };

    template<typename T, typename T2, typename T3, typename T4, typename T5>
    class Annotation<T, T2, T3, T4, T5>
            : public VariadicDataClass<typename std::pointer_traits<decltype(T::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T2::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T3::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T4::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T5::annotation)>::element_type> {
    public:
        template<typename U>
        auto &get() {
            return this->template access<typename std::pointer_traits<decltype(U::annotation)>::element_type>();
        }
    };

    template<typename T, typename T2, typename T3, typename T4, typename T5, typename T6>
    class Annotation<T, T2, T3, T4, T5, T6>
            : public VariadicDataClass<typename std::pointer_traits<decltype(T::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T2::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T3::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T4::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T5::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T6::annotation)>::element_type> {
    public:
        template<typename U>
        auto &get() {
            return this->template access<typename std::pointer_traits<decltype(U::annotation)>::element_type>();
        }
    };

    template<typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    class Annotation<T, T2, T3, T4, T5, T6, T7>
            : public VariadicDataClass<typename std::pointer_traits<decltype(T::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T2::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T3::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T4::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T5::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T6::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T7::annotation)>::element_type> {
    public:
        template<typename U>
        auto &get() {
            return this->template access<typename std::pointer_traits<decltype(U::annotation)>::element_type>();
        }
    };

    template<typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    class Annotation<T, T2, T3, T4, T5, T6, T7, T8>
            : public VariadicDataClass<typename std::pointer_traits<decltype(T::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T2::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T3::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T4::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T5::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T6::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T7::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T8::annotation)>::element_type> {
    public:
        template<typename U>
        auto &get() {
            return this->template access<typename std::pointer_traits<decltype(U::annotation)>::element_type>();
        }
    };

    template<typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    class Annotation<T, T2, T3, T4, T5, T6, T7, T8, T9>
            : public VariadicDataClass<typename std::pointer_traits<decltype(T::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T2::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T3::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T4::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T5::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T6::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T7::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T8::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T9::annotation)>::element_type> {
    public:
        template<typename U>
        auto &get() {
            return this->template access<typename std::pointer_traits<decltype(U::annotation)>::element_type>();
        }
    };

    template<typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
    class Annotation<T, T2, T3, T4, T5, T6, T7, T8, T9, T10>
            : public VariadicDataClass<typename std::pointer_traits<decltype(T::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T2::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T3::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T4::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T5::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T6::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T7::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T8::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T9::annotation)>::element_type,
                    typename std::pointer_traits<decltype(T10::annotation)>::element_type> {
    public:
        template<typename U>
        auto &get() {
            return this->template access<typename std::pointer_traits<decltype(U::annotation)>::element_type>();
        }
    };

}

#endif //LIBSIGMF_ANNOTATIONS_H
