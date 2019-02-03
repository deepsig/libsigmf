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

#ifndef LIBSIGMF_VARIADICDATACLASS_H
#define LIBSIGMF_VARIADICDATACLASS_H

#include "flatbuffers_json_visitor.h"
#include <tuple>
#include <string>
#include <typeinfo>
#include <cxxabi.h>

namespace sigmf {


    template<class T1, class T2>
    struct SameType {
        static const bool value = false;
    };

    template<class T>
    struct SameType<T, T> {
        static const bool value = true;
    };

    template<size_t T1, size_t T2>
    struct ReachedEnd {
        static const bool value = false;
    };

    template<size_t T>
    struct ReachedEnd<T, T> {
        static const bool value = true;
    };


    template<typename... SigMFNamespaces>
    class VariadicDataClass {
    public:
        /**
         * Access a reference to the (first) element of the tuple that is of type T
         * @tparam T
         * @return
         */
        template<typename T>
        T &access() {
            return MatchingField<0, T, sigmftypes, SigMFNamespaceOfType<0, T>::value>::get(sigmf_namespaces);
        }

        /**
         * Overload of access that accepts an object of type T rather than type T as template param
         * @tparam T
         * @return
         */
        template<typename T>
        T &access(T) {
            return access<T>();
        }

        template<typename T>
        T &operator[](T) {
            return access<T>();
        }

        /**
         * return a json object representing the data class with namespaces to match sigmf-spec
         * @return
         */
        json to_json() const {
            constexpr auto tuple_size = std::tuple_size<sigmftypes>::value;
            constexpr auto at_the_end = ReachedEnd<0, tuple_size>::value;
            json r = TupleIterator<0, sigmftypes, at_the_end>::to_json(sigmf_namespaces);
            return r;
        }

        /**
         * fill in data elements from a json object
         * @return
         */
        void from_json(json j) {
            constexpr auto tuple_size = std::tuple_size<sigmftypes>::value;
            constexpr auto at_the_end = ReachedEnd<0, tuple_size>::value;
            TupleIterator<0, sigmftypes, at_the_end>::from_json(sigmf_namespaces, j);
        }

    private:
        typedef std::tuple<SigMFNamespaces...> sigmftypes;
        sigmftypes sigmf_namespaces;

        /**
         * Convenience class for iterating over every element in Tuple at compile time.
         * @tparam Index
         * @tparam Size
         * @tparam Tuple
         * @tparam End
         */
        template<size_t Index, class Tuple, bool End = false>
        struct TupleIterator {
            /**
             * Stringify the object's namespace (using cxxabi from compiler) so we can use the literal
             * c++ namespace as a prefix in the sigmf record (the sigmf namespace)
             * @tparam T
             * @param ttype
             * @return
             */
            template<typename T>
            static std::string get_namespace(const T *ttype) {
                // TODO: can we do this once at compile-time/ctor, store result, and just look it up?
                std::string mangled_name = typeid(ttype).name();
                size_t size_of_demangled_name = 1024;
                char *demangled_name = static_cast<char *>(malloc(size_of_demangled_name));
                int status;
                demangled_name = abi::__cxa_demangle(mangled_name.c_str(), demangled_name, &size_of_demangled_name,
                                                     &status);
                auto demangled_string = std::string(demangled_name);
                free(static_cast<void *>(demangled_name));
                auto namespace_end = demangled_string.find("::");
                auto namespace_part = std::string("");
                if (namespace_end) {
                    namespace_part = demangled_string.substr(0, namespace_end + 1);
                }
                return namespace_part;
            }

            /**
             * Iterate through the types in tuple (namespaces) creating a json object with field
             * names prefixed by the object's owning namespace
             * @param tp
             * @return
             */
            static json to_json(const Tuple &tp) {
                typename std::tuple_element<Index, sigmftypes>::type::TableType *ttype;
                auto &flatbuffers_type = std::get<Index>(tp);
                auto reflection_table = ttype->MiniReflectTypeTable();
                std::string namespace_part = get_namespace(ttype);

                flatbuffers::FlatBufferBuilder fbb;
                auto loc = ttype->Pack(fbb, &flatbuffers_type);
                fbb.Finish(loc);

                auto bfrptr = fbb.GetBufferPointer();
                auto rtptr = flatbuffers::GetRoot<uint8_t>(bfrptr);
                json asjson = FlatBufferToJson(rtptr, reflection_table, namespace_part);

                constexpr auto next = Index + 1;
                constexpr auto size = std::tuple_size<sigmftypes>::value;
                json existing_vals = TupleIterator<next, Tuple, ReachedEnd<next, size>::value>::to_json(tp);

                for (auto &val : asjson.items()) {
                    existing_vals[val.key()] = val.value();
                }
                return existing_vals;
            }

            /**
             * From json with namespaces ("core:fieldname", core is the namespace) fill in the corresponding fields
             * of our tuple'd objects
             * @param tp
             * @param j
             */
            static void from_json(Tuple &tp, const json j) {
                typename std::tuple_element<Index, sigmftypes>::type::TableType *ttype;
                auto &flatbuffers_type = std::get<Index>(tp);
                auto reflection_table = ttype->MiniReflectTypeTable();
                std::string namespace_part = get_namespace(ttype);

                FromSigMFVisitor dumb_ripoff_visitor(namespace_part, j);
                IterateType(reflection_table, &dumb_ripoff_visitor, j);

                dumb_ripoff_visitor.fbb.Finish(flatbuffers::Offset<void>(dumb_ripoff_visitor._stop));

                auto *annoptr = flatbuffers::GetRoot<typename std::tuple_element<Index, sigmftypes>::type::TableType>(
                        dumb_ripoff_visitor.fbb.GetBufferPointer());
                annoptr->UnPackTo(&flatbuffers_type);

                constexpr auto next = Index + 1;
                constexpr auto size = std::tuple_size<sigmftypes>::value;
                TupleIterator<next, Tuple, ReachedEnd<next, size>::value>::from_json(tp, j);
            }
        };

        /**
         * Template specialization for reaching the end of compile-time loop over Tuple
         * @tparam Index
         * @tparam Size
         * @tparam Tuple
         */
        template<size_t Index, class Tuple>
        struct TupleIterator<Index, Tuple, true> {
            static json to_json(const Tuple &tp) {
                json r;
                return r;
            }

            static void from_json(const Tuple &tp, json j) {
            }
        };

        /**
         * Convenience class for determining if the Nth element of sigmftypes is of type T. Will have bool ::value
         * @tparam N
         * @tparam T
         */
        template<int N, typename T>
        struct SigMFNamespaceOfType : SameType<T, typename std::tuple_element<N, sigmftypes>::type> {
        };

        /**
         * MatchingField::get to compile-time loop over sigmf_namespaces until the current index N is of type T
         * at which point it calls the end-condition (because of the template specialization)
         * @tparam N
         * @tparam T
         * @tparam Tuple
         * @tparam Match
         */
        template<int N, class T, class Tuple, bool Match = false>
        struct MatchingField {
            static T &get(const Tuple &tp) {
                return MatchingField<N + 1, T, Tuple, SigMFNamespaceOfType<N + 1, T>::value>::get(tp);
            }
        };

        /**
         * MatchingField::get template-specializes the loop condition (true) to match when the object in Tuple at
         * index N is of type T.
         * @tparam N
         * @tparam T
         * @tparam Tuple
         */
        template<int N, class T, class Tuple>
        struct MatchingField<N, T, Tuple, true> {
            static T &get(const Tuple &tp) {
                return const_cast<T &>(std::get<N>(tp));
            }
        };
    };

    template<typename... SigMFNamespaces>
    void to_json(json &j, const VariadicDataClass<SigMFNamespaces...> t) {
        j = t.to_json();
    }

    template<typename... SigMFNamespaces>
    void from_json(json &j, const VariadicDataClass<SigMFNamespaces...> t) {
        t.from_json(j);
    }

}

#endif
