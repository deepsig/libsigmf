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

#ifndef LIBSIGMF_CAPTURE_H
#define LIBSIGMF_CAPTURE_H

#include "variadic_data_class.h"

/*
 * Helper macros-- mixed opinions abound!
 */
#define cc0(x0) Capture<x0::DescrT>

#define cc1(x0, x1) Capture<x0::DescrT, x1::DescrT>

#define cc2(x0, x1, x2) Capture<x0::DescrT, x1::DescrT, x2::DescrT>

#define cc3(x0, x1, x2, x3) Capture<x0::DescrT, x1::DescrT, x2::DescrT, x3::DescrT>

#define cc4(x0, x1, x2, x3, x4) Capture<x0::DescrT, x1::DescrT, x2::DescrT, \
x3::DescrT, x4::DescrT>

#define cc5(x0, x1, x2, x3, x4, x5) Capture<x0::DescrT, x1::DescrT, x2::DescrT, \
x3::DescrT, x4::DescrT, x5::DescrT>

#define cc6(x0, x1, x2, x3, x4, x5, x6) Capture<x0::DescrT, x1::DescrT, x2::DescrT, \
x3::DescrT, x4::DescrT, x5::DescrT, x6::DescrT>

#define cc7(x0, x1, x2, x3, x4, x5, x6, x7) Capture<x0::DescrT, x1::DescrT, x2::DescrT, \
x3::DescrT, x4::DescrT, x5::DescrT, x6::DescrT, x7::DescrT>

#define cc8(x0, x1, x2, x3, x4, x5, x6, x7, x8) Capture<x0::DescrT, x1::DescrT, x2::DescrT, \
x3::DescrT, x4::DescrT, x5::DescrT, x6::DescrT, x7::DescrT, x8::DescrT>

#define cc9(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9) Capture<x0::DescrT, x1::DescrT, x2::DescrT, \
x3::DescrT, x4::DescrT, x5::DescrT, x6::DescrT, x7::DescrT, x8::DescrT, x9::DescrT>


#define GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME, ...) NAME
#define create_capture(...) GET_MACRO(__VA_ARGS__, cc9, cc8, cc7, cc6, cc5, cc4, cc3, cc2, cc1, cc0)(__VA_ARGS__)


namespace sigmf {

    // This template is mostly here so we can specialize it. If we can get an expansion/loop of T... so that for every
    // T we can create a VariadicDataClass<T::annotation, then we'd be able to eliminate all of the specializations
    template<typename...T>
    class Capture : public VariadicDataClass<T...> {
    };

    template<typename T>
    class Capture<T>
            : public VariadicDataClass<typename std::pointer_traits<decltype(T::capture)>::element_type> {
    public:
        template<typename U>
        auto &get() {// -> typename std::pointer_traits<decltype(U::annotation)>::element_type& {
            return this->template access<typename std::pointer_traits<decltype(U::capture)>::element_type>();
        }
    };

    template<typename T, typename T2>
    class Capture<T, T2>
            : public VariadicDataClass<typename std::pointer_traits<decltype(T::capture)>::element_type,
                    typename std::pointer_traits<decltype(T2::capture)>::element_type> {
    public:
        template<typename U>
        auto &get() {
            return this->template access<typename std::pointer_traits<decltype(U::capture)>::element_type>();
        }
    };

    template<typename T, typename T2, typename T3>
    class Capture<T, T2, T3>
            : public VariadicDataClass<typename std::pointer_traits<decltype(T::capture)>::element_type,
                    typename std::pointer_traits<decltype(T2::capture)>::element_type,
                    typename std::pointer_traits<decltype(T3::capture)>::element_type> {
    public:
        template<typename U>
        auto &get() {
            return this->template access<typename std::pointer_traits<decltype(U::capture)>::element_type>();
        }
    };


    template<typename T, typename T2, typename T3, typename T4>
    class Capture<T, T2, T3, T4>
            : public VariadicDataClass<typename std::pointer_traits<decltype(T::capture)>::element_type,
                    typename std::pointer_traits<decltype(T2::capture)>::element_type,
                    typename std::pointer_traits<decltype(T3::capture)>::element_type,
                    typename std::pointer_traits<decltype(T4::capture)>::element_type> {
    public:
        template<typename U>
        auto &get() {
            return this->template access<typename std::pointer_traits<decltype(U::capture)>::element_type>();
        }
    };

    template<typename T, typename T2, typename T3, typename T4, typename T5>
    class Capture<T, T2, T3, T4, T5>
            : public VariadicDataClass<typename std::pointer_traits<decltype(T::capture)>::element_type,
                    typename std::pointer_traits<decltype(T2::capture)>::element_type,
                    typename std::pointer_traits<decltype(T3::capture)>::element_type,
                    typename std::pointer_traits<decltype(T4::capture)>::element_type,
                    typename std::pointer_traits<decltype(T5::capture)>::element_type> {
    public:
        template<typename U>
        auto &get() {
            return this->template access<typename std::pointer_traits<decltype(U::capture)>::element_type>();
        }
    };

    template<typename T, typename T2, typename T3, typename T4, typename T5, typename T6>
    class Capture<T, T2, T3, T4, T5, T6>
            : public VariadicDataClass<typename std::pointer_traits<decltype(T::capture)>::element_type,
                    typename std::pointer_traits<decltype(T2::capture)>::element_type,
                    typename std::pointer_traits<decltype(T3::capture)>::element_type,
                    typename std::pointer_traits<decltype(T4::capture)>::element_type,
                    typename std::pointer_traits<decltype(T5::capture)>::element_type,
                    typename std::pointer_traits<decltype(T6::capture)>::element_type> {
    public:
        template<typename U>
        auto &get() {
            return this->template access<typename std::pointer_traits<decltype(U::capture)>::element_type>();
        }
    };

    template<typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    class Capture<T, T2, T3, T4, T5, T6, T7>
            : public VariadicDataClass<typename std::pointer_traits<decltype(T::capture)>::element_type,
                    typename std::pointer_traits<decltype(T2::capture)>::element_type,
                    typename std::pointer_traits<decltype(T3::capture)>::element_type,
                    typename std::pointer_traits<decltype(T4::capture)>::element_type,
                    typename std::pointer_traits<decltype(T5::capture)>::element_type,
                    typename std::pointer_traits<decltype(T6::capture)>::element_type,
                    typename std::pointer_traits<decltype(T7::capture)>::element_type> {
    public:
        template<typename U>
        auto &get() {
            return this->template access<typename std::pointer_traits<decltype(U::capture)>::element_type>();
        }
    };

    template<typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    class Capture<T, T2, T3, T4, T5, T6, T7, T8>
            : public VariadicDataClass<typename std::pointer_traits<decltype(T::capture)>::element_type,
                    typename std::pointer_traits<decltype(T2::capture)>::element_type,
                    typename std::pointer_traits<decltype(T3::capture)>::element_type,
                    typename std::pointer_traits<decltype(T4::capture)>::element_type,
                    typename std::pointer_traits<decltype(T5::capture)>::element_type,
                    typename std::pointer_traits<decltype(T6::capture)>::element_type,
                    typename std::pointer_traits<decltype(T7::capture)>::element_type,
                    typename std::pointer_traits<decltype(T8::capture)>::element_type> {
    public:
        template<typename U>
        auto &get() {
            return this->template access<typename std::pointer_traits<decltype(U::capture)>::element_type>();
        }
    };

    template<typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    class Capture<T, T2, T3, T4, T5, T6, T7, T8, T9>
            : public VariadicDataClass<typename std::pointer_traits<decltype(T::capture)>::element_type,
                    typename std::pointer_traits<decltype(T2::capture)>::element_type,
                    typename std::pointer_traits<decltype(T3::capture)>::element_type,
                    typename std::pointer_traits<decltype(T4::capture)>::element_type,
                    typename std::pointer_traits<decltype(T5::capture)>::element_type,
                    typename std::pointer_traits<decltype(T6::capture)>::element_type,
                    typename std::pointer_traits<decltype(T7::capture)>::element_type,
                    typename std::pointer_traits<decltype(T8::capture)>::element_type,
                    typename std::pointer_traits<decltype(T9::capture)>::element_type> {
    public:
        template<typename U>
        auto &get() {
            return this->template access<typename std::pointer_traits<decltype(U::capture)>::element_type>();
        }
    };

    template<typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
    class Capture<T, T2, T3, T4, T5, T6, T7, T8, T9, T10>
            : public VariadicDataClass<typename std::pointer_traits<decltype(T::capture)>::element_type,
                    typename std::pointer_traits<decltype(T2::capture)>::element_type,
                    typename std::pointer_traits<decltype(T3::capture)>::element_type,
                    typename std::pointer_traits<decltype(T4::capture)>::element_type,
                    typename std::pointer_traits<decltype(T5::capture)>::element_type,
                    typename std::pointer_traits<decltype(T6::capture)>::element_type,
                    typename std::pointer_traits<decltype(T7::capture)>::element_type,
                    typename std::pointer_traits<decltype(T8::capture)>::element_type,
                    typename std::pointer_traits<decltype(T9::capture)>::element_type,
                    typename std::pointer_traits<decltype(T10::capture)>::element_type> {
    public:
        template<typename U>
        auto &get() {
            return this->template access<typename std::pointer_traits<decltype(U::capture)>::element_type>();
        }
    };

}

#endif //LIBSIGMF_ANNOTATIONS_H
