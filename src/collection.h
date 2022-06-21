/*
 *    Copyright 2019, 2022 DeepSig Inc.
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

#ifndef LIBSIGMF_COLLECTION_H
#define LIBSIGMF_COLLECTION_H

#include "variadic_data_class.h"

/*
 * Helper macros-- mixed opinions abound!
 */
#define cl0(x0) Collection<x0::DescrT>

#define cl1(x0, x1) Collection<x0::DescrT, x1::DescrT>

#define cl2(x0, x1, x2) Collection<x0::DescrT, x1::DescrT, x2::DescrT>

#define cl3(x0, x1, x2, x3) Collection<x0::DescrT, x1::DescrT, x2::DescrT, x3::DescrT>

#define cl4(x0, x1, x2, x3, x4) Collection<x0::DescrT, x1::DescrT, x2::DescrT, \
x3::DescrT, x4::DescrT>

#define cl5(x0, x1, x2, x3, x4, x5) Collection<x0::DescrT, x1::DescrT, x2::DescrT, \
x3::DescrT, x4::DescrT, x5::DescrT>

#define cl6(x0, x1, x2, x3, x4, x5, x6) Collection<x0::DescrT, x1::DescrT, x2::DescrT, \
x3::DescrT, x4::DescrT, x5::DescrT, x6::DescrT>

#define cl7(x0, x1, x2, x3, x4, x5, x6, x7) Collection<x0::DescrT, x1::DescrT, x2::DescrT, \
x3::DescrT, x4::DescrT, x5::DescrT, x6::DescrT, x7::DescrT>

#define cl8(x0, x1, x2, x3, x4, x5, x6, x7, x8) Collection<x0::DescrT, x1::DescrT, x2::DescrT, \
x3::DescrT, x4::DescrT, x5::DescrT, x6::DescrT, x7::DescrT, x8::DescrT>

#define cl9(x0, x1, x2, x3, x4, x5, x6, x7, x8, x9) Collection<x0::DescrT, x1::DescrT, x2::DescrT, \
x3::DescrT, x4::DescrT, x5::DescrT, x6::DescrT, x7::DescrT, x8::DescrT, x9::DescrT>


#define GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME, ...) NAME
#define create_collection(...) GET_MACRO(__VA_ARGS__, cl9, cl8, cl7, cl6, cl5, cl4, cl3, cl2, cl1, cl0)(__VA_ARGS__)

#define sigmfns(x) get<x::DescrT>()


namespace sigmf {

// This template is mostly here so we cln specialize it. If we cln get an expansion/loop of T... so that for every
// T we cln create a VariadicDataClass<T::collection, then we'd be able to eliminate all of the specializations
template<typename...T>
class Collection : public VariadicDataClass<T...> {
};

template<typename T>
class Collection<T>
        : public VariadicDataClass<typename std::pointer_traits<decltype(T::collection)>::element_type> {
public:
    template<typename U>
    auto &get() {// -> typename std::pointer_traits<decltype(U::collection)>::element_type& {
        return this->template access<typename std::pointer_traits<decltype(U::collection)>::element_type>();
    }
};

template<typename T, typename T2>
class Collection<T, T2>
        : public VariadicDataClass<typename std::pointer_traits<decltype(T::collection)>::element_type,
                typename std::pointer_traits<decltype(T2::collection)>::element_type> {
public:
    template<typename U>
    auto &get() {
        return this->template access<typename std::pointer_traits<decltype(U::collection)>::element_type>();
    }
};

template<typename T, typename T2, typename T3>
class Collection<T, T2, T3>
        : public VariadicDataClass<typename std::pointer_traits<decltype(T::collection)>::element_type,
                typename std::pointer_traits<decltype(T2::collection)>::element_type,
                typename std::pointer_traits<decltype(T3::collection)>::element_type> {
public:
    template<typename U>
    auto &get() {
        return this->template access<typename std::pointer_traits<decltype(U::collection)>::element_type>();
    }
};


template<typename T, typename T2, typename T3, typename T4>
class Collection<T, T2, T3, T4>
        : public VariadicDataClass<typename std::pointer_traits<decltype(T::collection)>::element_type,
                typename std::pointer_traits<decltype(T2::collection)>::element_type,
                typename std::pointer_traits<decltype(T3::collection)>::element_type,
                typename std::pointer_traits<decltype(T4::collection)>::element_type> {
public:
    template<typename U>
    auto &get() {
        return this->template access<typename std::pointer_traits<decltype(U::collection)>::element_type>();
    }
};

template<typename T, typename T2, typename T3, typename T4, typename T5>
class Collection<T, T2, T3, T4, T5>
        : public VariadicDataClass<typename std::pointer_traits<decltype(T::collection)>::element_type,
                typename std::pointer_traits<decltype(T2::collection)>::element_type,
                typename std::pointer_traits<decltype(T3::collection)>::element_type,
                typename std::pointer_traits<decltype(T4::collection)>::element_type,
                typename std::pointer_traits<decltype(T5::collection)>::element_type> {
public:
    template<typename U>
    auto &get() {
        return this->template access<typename std::pointer_traits<decltype(U::collection)>::element_type>();
    }
};

template<typename T, typename T2, typename T3, typename T4, typename T5, typename T6>
class Collection<T, T2, T3, T4, T5, T6>
        : public VariadicDataClass<typename std::pointer_traits<decltype(T::collection)>::element_type,
                typename std::pointer_traits<decltype(T2::collection)>::element_type,
                typename std::pointer_traits<decltype(T3::collection)>::element_type,
                typename std::pointer_traits<decltype(T4::collection)>::element_type,
                typename std::pointer_traits<decltype(T5::collection)>::element_type,
                typename std::pointer_traits<decltype(T6::collection)>::element_type> {
public:
    template<typename U>
    auto &get() {
        return this->template access<typename std::pointer_traits<decltype(U::collection)>::element_type>();
    }
};

template<typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class Collection<T, T2, T3, T4, T5, T6, T7>
        : public VariadicDataClass<typename std::pointer_traits<decltype(T::collection)>::element_type,
                typename std::pointer_traits<decltype(T2::collection)>::element_type,
                typename std::pointer_traits<decltype(T3::collection)>::element_type,
                typename std::pointer_traits<decltype(T4::collection)>::element_type,
                typename std::pointer_traits<decltype(T5::collection)>::element_type,
                typename std::pointer_traits<decltype(T6::collection)>::element_type,
                typename std::pointer_traits<decltype(T7::collection)>::element_type> {
public:
    template<typename U>
    auto &get() {
        return this->template access<typename std::pointer_traits<decltype(U::collection)>::element_type>();
    }
};

template<typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class Collection<T, T2, T3, T4, T5, T6, T7, T8>
        : public VariadicDataClass<typename std::pointer_traits<decltype(T::collection)>::element_type,
                typename std::pointer_traits<decltype(T2::collection)>::element_type,
                typename std::pointer_traits<decltype(T3::collection)>::element_type,
                typename std::pointer_traits<decltype(T4::collection)>::element_type,
                typename std::pointer_traits<decltype(T5::collection)>::element_type,
                typename std::pointer_traits<decltype(T6::collection)>::element_type,
                typename std::pointer_traits<decltype(T7::collection)>::element_type,
                typename std::pointer_traits<decltype(T8::collection)>::element_type> {
public:
    template<typename U>
    auto &get() {
        return this->template access<typename std::pointer_traits<decltype(U::collection)>::element_type>();
    }
};

template<typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
class Collection<T, T2, T3, T4, T5, T6, T7, T8, T9>
        : public VariadicDataClass<typename std::pointer_traits<decltype(T::collection)>::element_type,
                typename std::pointer_traits<decltype(T2::collection)>::element_type,
                typename std::pointer_traits<decltype(T3::collection)>::element_type,
                typename std::pointer_traits<decltype(T4::collection)>::element_type,
                typename std::pointer_traits<decltype(T5::collection)>::element_type,
                typename std::pointer_traits<decltype(T6::collection)>::element_type,
                typename std::pointer_traits<decltype(T7::collection)>::element_type,
                typename std::pointer_traits<decltype(T8::collection)>::element_type,
                typename std::pointer_traits<decltype(T9::collection)>::element_type> {
public:
    template<typename U>
    auto &get() {
        return this->template access<typename std::pointer_traits<decltype(U::collection)>::element_type>();
    }
};

template<typename T, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
class Collection<T, T2, T3, T4, T5, T6, T7, T8, T9, T10>
        : public VariadicDataClass<typename std::pointer_traits<decltype(T::collection)>::element_type,
                typename std::pointer_traits<decltype(T2::collection)>::element_type,
                typename std::pointer_traits<decltype(T3::collection)>::element_type,
                typename std::pointer_traits<decltype(T4::collection)>::element_type,
                typename std::pointer_traits<decltype(T5::collection)>::element_type,
                typename std::pointer_traits<decltype(T6::collection)>::element_type,
                typename std::pointer_traits<decltype(T7::collection)>::element_type,
                typename std::pointer_traits<decltype(T8::collection)>::element_type,
                typename std::pointer_traits<decltype(T9::collection)>::element_type,
                typename std::pointer_traits<decltype(T10::collection)>::element_type> {
public:
    template<typename U>
    auto &get() {
        return this->template access<typename std::pointer_traits<decltype(U::collection)>::element_type>();
    }
};

} // namespace sigmf

#endif //LIBSIGMF_COLLECTION_H
