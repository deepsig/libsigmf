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

#ifndef LIBSIGMF_FLATBUFFERS_TO_JSON_VISITOR_H
#define LIBSIGMF_FLATBUFFERS_TO_JSON_VISITOR_H

#include "variadic_data_class.h"
#include <flatbuffers/minireflect.h>
#include <flatbuffers/flatbuffers.h>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;


/*
 * Designed to be run by our TypeIterator-- it adds fields to a flatbuffer (fbb) it owns by reading from a json
 * object given in its ctor. After the TypeIterator runs the flatbuffer can be made in to a specific flatbuffer
 * object or shipped somewhere
 */
struct FromSigMFVisitor : public flatbuffers::IterationVisitor {
    flatbuffers::uoffset_t _start;
    flatbuffers::uoffset_t _stop;
    json narrowest_json;
    std::string p;
    std::string last_field_name;
    flatbuffers::FlatBufferBuilder fbb;
    flatbuffers::voffset_t last_offset;
    flatbuffers::Offset<void> last_fb_offset;


    explicit FromSigMFVisitor(std::string namespace_prefix, const json &j)
            : p(namespace_prefix), last_offset(0), _start(0), _stop(0) {
        fbb = flatbuffers::FlatBufferBuilder();
    }

    void StartSequence() override {
        _start = fbb.StartTable();
    }

    void EndSequence() override {
        _stop = fbb.EndTable(_start);
    }

    void Field(size_t field_idx, size_t set_idx, flatbuffers::ElementaryType e_type,
               bool is_vector, const flatbuffers::TypeTable *type_table,
               const char *name, const uint8_t *val, json jj) {
        narrowest_json = jj;
        last_field_name = name;
        last_offset = flatbuffers::FieldIndexToOffset(static_cast<flatbuffers::voffset_t>(field_idx));
    }

    template<typename T>
    void Named(T x, const char *name) {
        try {
            fbb.AddElement(last_offset, narrowest_json.at(p + last_field_name).get<T>(), T{});
        } catch (nlohmann::detail::out_of_range &e) {
        }
    }

    void UType(uint8_t x, const char *name) override { Named(x, name); }

    // void Bool(bool x) override { s += x ? "true" : "false"; }
    void Char(int8_t x, const char *name) override { Named(x, name); }

    void UChar(uint8_t x, const char *name) override { Named(x, name); }

    void Short(int16_t x, const char *name) override { Named(x, name); }

    void UShort(uint16_t x, const char *name) override { Named(x, name); }

    void Int(int32_t x, const char *name) override { Named(x, name); }

    void UInt(uint32_t x, const char *name) override { Named(x, name); }

    void Long(int64_t x) override {
        try {
            fbb.AddElement(last_offset, narrowest_json.at(p + last_field_name).get<int64_t>(), int64_t(0));
        } catch (nlohmann::detail::out_of_range &e) {
        }
    }

    void ULong(uint64_t x) override {
        try {
            fbb.AddElement(last_offset, narrowest_json.at(p + last_field_name).get<uint64_t>(), uint64_t(0));
        } catch (nlohmann::detail::out_of_range &e) {
        }
    }

    void Float(float x) override {
        try {
            fbb.AddElement(last_offset, narrowest_json.at(p + last_field_name).get<float>(), 0.f);
        } catch (nlohmann::detail::out_of_range &e) {
        }
    }

    void Double(double x) override {
        try {
            fbb.AddElement(last_offset, narrowest_json.at(p + last_field_name).get<double>(), 0.0);
        } catch (nlohmann::detail::out_of_range &e) {
        }
    }

    void String(const struct flatbuffers::String *str) override {
        try {
            auto strval = narrowest_json.at(p + last_field_name).get<std::string>();
            last_fb_offset = flatbuffers::Offset<void>(fbb.CreateString(strval).o);
        } catch (nlohmann::detail::out_of_range &e) {
            last_fb_offset.o = 0;
        };
    }

    void StartVector() override {
        std::cout << "the visitor was called on startvector... not implemented" << std::endl;

    }

    void EndVector() override {
        std::cout << "the visitor was called on endvector... not implemented" << std::endl;
    }

    void Element(size_t i, flatbuffers::ElementaryType /*type*/,
                 const flatbuffers::TypeTable * /*type_table*/, const uint8_t * /*val*/) override {}
};


template<typename T>
flatbuffers::Offset<void> json_vector_to_flatbuffer(flatbuffers::FlatBufferBuilder &fbb, const json &jvec) {
    size_t dtype_size = sizeof(T);
    std::vector<T> tmpvec(jvec.size());
    for (auto elem = jvec.begin(); elem != jvec.end(); ++elem) {
        tmpvec.emplace_back(elem->get<T>());
    }
    return flatbuffers::Offset<void>(fbb.CreateVector(tmpvec).o);

}

inline flatbuffers::Offset<void>
json_vector_to_chararray(flatbuffers::FlatBufferBuilder &fbb, const json &jvec, flatbuffers::ElementaryType type) {
    size_t dtype_size;
    size_t num_elements = jvec.size();
    switch (type) {
        case flatbuffers::ET_UTYPE: {
            auto rvec = jvec.get<std::vector<uint8_t> >();
            return flatbuffers::Offset<void>(fbb.CreateVector(rvec).o);
        }
        case flatbuffers::ET_BOOL: {
            auto rvec = jvec.get<std::vector<uint8_t> >();
            return flatbuffers::Offset<void>(fbb.CreateVector(rvec).o);
        }
        case flatbuffers::ET_CHAR: {
            auto rvec = jvec.get<std::vector<int8_t> >();
            return flatbuffers::Offset<void>(fbb.CreateVector(rvec).o);
        }
        case flatbuffers::ET_UCHAR: {
            auto rvec = jvec.get<std::vector<uint8_t> >();
            return flatbuffers::Offset<void>(fbb.CreateVector(rvec).o);
        }
        case flatbuffers::ET_SHORT:
            return json_vector_to_flatbuffer<int16_t>(fbb, jvec);
        case flatbuffers::ET_USHORT: {
            return json_vector_to_flatbuffer<uint16_t>(fbb, jvec);
        }
        case flatbuffers::ET_INT:
            return json_vector_to_flatbuffer<int32_t>(fbb, jvec);
        case flatbuffers::ET_UINT: {
            return json_vector_to_flatbuffer<uint32_t>(fbb, jvec);
        }
        case flatbuffers::ET_LONG:
            return json_vector_to_flatbuffer<int64_t>(fbb, jvec);
        case flatbuffers::ET_ULONG: {
            return json_vector_to_flatbuffer<uint64_t>(fbb, jvec);
        }
        case flatbuffers::ET_FLOAT: {
            return json_vector_to_flatbuffer<float>(fbb, jvec);
        }
        case flatbuffers::ET_DOUBLE: {
            return json_vector_to_flatbuffer<double>(fbb, jvec);
        }
        default:
            throw std::runtime_error("libsigmf cannot make a vector of this type yet");
    }
}


/**
 * Iterate through a typetable-- I'll be honest here. This is kind of bullshit. We need to create all of
 * the types like Strings, Lists, Vectors, and other flatbuffer types before we create our table. I'm not
 * sure there's a better way to fill in the buffer than iterate over it twice-- i wonder how flatbuffers
 * does this internally (look at Parsers). We're just going to construct shit in the fbb in the first loop
 * and then fill in the fixed-width fields. I know this does the string creation correctly, but I don't
 * know that lists(Vectors) or other tables/structs will work. The Vectors and Tables will probably require
 * some recursion
 *
 * @param type_table the table to iterate over
 * @param visitor the visitor responsible for creating objects and adding fields to its internal flatbufferbuilder
 */
inline void IterateType(const flatbuffers::TypeTable *type_table, FromSigMFVisitor *visitor, json original_json) {
    const uint8_t *prev_val = nullptr;
    const uint8_t val[8] = {};
    auto comosite_type_offsets = std::map<size_t, flatbuffers::Offset<void> >();
    // Generate the strings up front (because they must be created before our Table in the flatbuffer)
    for (size_t i = 0; i < type_table->num_elems; i++) {
        auto type_code = type_table->type_codes[i];
        auto type = static_cast<flatbuffers::ElementaryType>(type_code.base_type);
        auto is_vector = type_code.is_vector != 0;
        auto ref_idx = type_code.sequence_ref;
        const flatbuffers::TypeTable *ref = nullptr;
        if (ref_idx >= 0) { ref = type_table->type_refs[ref_idx](); }
        auto name = type_table->names ? type_table->names[i] : nullptr;
        // The thing is, at least on this iteration we don't care about making real elementary types, just the
        // ones that need an offset
        if (type < flatbuffers::ET_STRING) {
            if (is_vector) {
                auto vector_offset = json_vector_to_chararray(visitor->fbb, original_json[name], type);
                auto void_offset = flatbuffers::Offset<void>(vector_offset.o);
                comosite_type_offsets[i] = void_offset;
            }
        } else if (type == flatbuffers::ET_STRING) {
            if (is_vector) {
                std::vector<flatbuffers::Offset<void> > vecofstrings;

                for (const auto &item : original_json[name]) {
                    std::cout << item << std::endl;

                    auto strval = item.get<std::string>();
                    auto last_fb_offset = flatbuffers::Offset<void>(visitor->fbb.CreateString(strval).o);

                    vecofstrings.emplace_back(last_fb_offset);
                }
                auto vecoffset = flatbuffers::Offset<void>(visitor->fbb.CreateVector(vecofstrings).o);
                comosite_type_offsets[i] = vecoffset;
            } else {
                visitor->Field(i, 0, type, is_vector, ref, name, val, original_json);
                IterateValue(type, val, ref, prev_val, -1, visitor);
                comosite_type_offsets[i] = visitor->last_fb_offset;
            }
        } else if (type == flatbuffers::ET_SEQUENCE) {
            if (is_vector) {
                std::vector<flatbuffers::Offset<void> > vecofstrings;

                for (const auto &item : original_json[name]) {
                    std::cout << item << std::endl;

                    auto seq_typetable = type_table->type_refs[ref_idx]();
                    IterateType(seq_typetable, visitor, item);
                    auto vecelement = flatbuffers::Offset<void>(visitor->_stop);

                    vecofstrings.emplace_back(vecelement);
                }
                auto vecoffset = flatbuffers::Offset<void>(visitor->fbb.CreateVector(vecofstrings).o);
                comosite_type_offsets[i] = vecoffset;
            } else {
                auto seq_typetable = type_table->type_refs[ref_idx]();
                IterateType(seq_typetable, visitor, original_json[name]);
                comosite_type_offsets[i] = flatbuffers::Offset<void>(visitor->_stop);
            }
        }

    }
    // Now make the table and fill in all of our types
    size_t set_idx = 0;
    visitor->StartSequence();
    for (size_t i = 0; i < type_table->num_elems; i++) {
        auto type_code = type_table->type_codes[i];
        auto type = static_cast<flatbuffers::ElementaryType>(type_code.base_type);
        auto is_vector = type_code.is_vector != 0;
        auto ref_idx = type_code.sequence_ref;
        const flatbuffers::TypeTable *ref = nullptr;
        if (ref_idx >= 0) { ref = type_table->type_refs[ref_idx](); }
        auto name = type_table->names ? type_table->names[i] : nullptr;
        if (is_vector) {
            auto this_voffset = flatbuffers::FieldIndexToOffset(static_cast<flatbuffers::voffset_t >(i));
            visitor->fbb.AddOffset(this_voffset, comosite_type_offsets.at(i));
        } else if (type == flatbuffers::ET_STRING) {
            auto this_voffset = flatbuffers::FieldIndexToOffset(static_cast<flatbuffers::voffset_t>(i));
            visitor->fbb.AddOffset(this_voffset, comosite_type_offsets.at(i));
        } else if (type == flatbuffers::ET_SEQUENCE) {
            auto this_voffset = flatbuffers::FieldIndexToOffset(static_cast<flatbuffers::voffset_t>(i));
            visitor->fbb.AddOffset(this_voffset, comosite_type_offsets.at(i));
        } else {
            visitor->Field(i, set_idx, type, is_vector, ref, name, val, original_json);
            set_idx++;
            IterateValue(type, val, ref, prev_val, -1, visitor);
        }
    }
    visitor->EndSequence();
}


// forward declare so we can create objects as fields
inline json
FlatBufferToJson(const uint8_t *buffer_root, const flatbuffers::TypeTable *typetable, const std::string &ns_prefix);

json
flatbuffer_field_to_json(const uint8_t *val, flatbuffers::ElementaryType type,
                         const flatbuffers::TypeTable *tt = nullptr,
                         const std::string &ns_prefix = "") {
    switch (type) {
        case flatbuffers::ET_UTYPE: {
            auto tval = *reinterpret_cast<const uint8_t *>(val);
            return json(tval);
            break;
        }
        case flatbuffers::ET_BOOL: {
            auto tval = json(*reinterpret_cast<const uint8_t *>(val) != 0);
            return json(tval);
            break;
        }
        case flatbuffers::ET_CHAR: {
            auto tval = *reinterpret_cast<const int8_t *>(val);
            return json(tval);
            break;
        }
        case flatbuffers::ET_UCHAR: {
            auto tval = *reinterpret_cast<const uint8_t *>(val);
            return json(tval);
            break;
        }
        case flatbuffers::ET_SHORT: {
            auto tval = *reinterpret_cast<const int16_t *>(val);
            return json(tval);
            break;
        }
        case flatbuffers::ET_USHORT: {
            auto tval = *reinterpret_cast<const uint16_t *>(val);
            return json(tval);
            break;
        }
        case flatbuffers::ET_INT: {
            auto tval = *reinterpret_cast<const int32_t *>(val);
            return json(tval);
            break;
        }
        case flatbuffers::ET_UINT: {
            auto tval = *reinterpret_cast<const uint32_t *>(val);
            return json(tval);
            break;
        }
        case flatbuffers::ET_LONG: {
            auto tval = *reinterpret_cast<const int64_t *>(val);
            return json(tval);
            break;
        }
        case flatbuffers::ET_ULONG: {
            auto tval = *reinterpret_cast<const uint64_t *>(val);
            return json(tval);
            break;
        }
        case flatbuffers::ET_FLOAT: {
            auto tval = *reinterpret_cast<const float *>(val);
            return json(tval);
            break;
        }
        case flatbuffers::ET_DOUBLE: {
            auto tval = *reinterpret_cast<const double *>(val);
            return json(tval);
            break;
        }
        case flatbuffers::ET_STRING: {
            val += flatbuffers::ReadScalar<flatbuffers::uoffset_t>(val);
            auto tval = reinterpret_cast<const flatbuffers::String *>(val);
            return json(tval->c_str());
            break;
        }
        case flatbuffers::ET_SEQUENCE: {
            switch (tt->st) {
                case flatbuffers::ST_TABLE:
                    std::cout << "we need to make a table to put in our table man" << std::endl;
                    val += flatbuffers::ReadScalar<flatbuffers::uoffset_t>(val);
                    return FlatBufferToJson(val, tt, ns_prefix);
                    // Have not implemented structs, unions, or enums which are all supported by flatbuffers schema
                    // and would be great to have, but dont make as much sense in json-land. They're on the todo list...
                    // here is what flatbuffers does internally (see IterateValue in minireflect.h)
                default:
                    throw std::runtime_error(
                            "libsigmf has not implemented flatbuffer struct, union, or enum to json yet");
//                case flatbuffers::ST_STRUCT: IterateObject(val, type_table, visitor); break;
//                case flatbuffers::ST_UNION: {
//                    val += flatbuffers::ReadScalar<flatbuffers::uoffset_t>(val);
//                            FLATBUFFERS_ASSERT(prev_val);
//                    auto union_type = *prev_val;  // Always a uint8_t.
//                    if (vector_index >= 0) {
//                        auto type_vec = reinterpret_cast<const Vector<uint8_t> *>(prev_val);
//                        union_type = type_vec->Get(static_cast<flatbuffers::uoffset_t>(vector_index));
//                    }
//                    auto type_code_idx =
//                            flatbuffers::LookupEnum(union_type, type_table->values, type_table->num_elems);
//                    if (type_code_idx >= 0 &&
//                        type_code_idx < static_cast<int32_t>(type_table->num_elems)) {
//                        auto type_code = type_table->type_codes[type_code_idx];
//                        switch (type_code.base_type) {
//                            case flatbuffers::ET_SEQUENCE: {
//                                auto ref = type_table->type_refs[type_code.sequence_ref]();
//                                IterateObject(val, ref, visitor);
//                                break;
//                            }
//                            case flatbuffers::ET_STRING:
//                                visitor->String(reinterpret_cast<const flatbuffers::String *>(val));
//                                break;
//                            default: visitor->Unknown(val);
//                        }
//                    } else {
//                        visitor->Unknown(val);
//                    }
//                    break;
//                }
//                case flatbuffers::ST_ENUM: FLATBUFFERS_ASSERT(false); break;

            }
        }
    }
}


/**
 * A function to iterate through a flatbuffer that is described by the type and build up a json object to return.
 * The flatbuffer should have already been Finished, and the result of FlatBufferBuilder::Finish() -> GetRoot should
 * be the buffer_root. See variadic_data_class where we do the following:
 *                 auto bfrptr = fbb.GetBufferPointer();
 *                 auto rtptr = flatbuffers::GetRoot<uint8_t>(bfrptr);
 *
 * The gist of this is to get information about each field inside of a flatbuffers Table, then for each field, check
 * if there is a value. If so, serialize it to json and shove its value inside the json_object we want to return
 * using the name of the field as the key.
 */
inline json
FlatBufferToJson(const uint8_t *buffer_root, const flatbuffers::TypeTable *typetable, const std::string &ns_prefix) {
    json json_object;
    const auto obj = reinterpret_cast<const flatbuffers::Table *>(buffer_root);
    for (size_t i = 0; i < typetable->num_elems; i++) {
        // Gather all of the underlying info about this element in table
        auto type_code = typetable->type_codes[i];
        auto type = static_cast<flatbuffers::ElementaryType>(type_code.base_type);
        auto is_vector = type_code.is_vector != 0;
        auto ref_idx = type_code.sequence_ref;
        const flatbuffers::TypeTable *ref = nullptr;
        if (ref_idx >= 0) { ref = typetable->type_refs[ref_idx](); }
        auto name = typetable->names ? typetable->names[i] : nullptr;
        const uint8_t *val = nullptr;

        // Fetch the actual value of this field now and stick it inside our json object
        if (typetable->st == flatbuffers::ST_TABLE) {
            auto field_offset = flatbuffers::FieldIndexToOffset(static_cast<flatbuffers::voffset_t>(i));
            val = obj->GetAddressOf(field_offset);
        } else {
            // when does this get triggered? Probably when someone tries to pass a non-flattbuffers object
            // to us. Let's throw an exception so it'll be nice and obvious with a good test-case when this
            // happens.
            throw std::runtime_error(
                    "libsigmf::FlatBufferToJson was called with a non-table type. Please file an issue with your fbs schema and input data.");
            // Here is what flatbuffers does internally: val = obj + typetable->values[i];
        }
        if (val) {
            const flatbuffers::TypeTable *ttptr = nullptr;
            if (ref_idx >= 0) {
                ttptr = typetable->type_refs[ref_idx]();
            }
            if (is_vector) {
                val += flatbuffers::ReadScalar<flatbuffers::uoffset_t>(val);
                auto vec = reinterpret_cast<const flatbuffers::Vector<uint8_t> *>(val);
                auto elem_ptr = vec->Data();
                for (size_t j = 0; j < vec->size(); j++) {
                    json_object[ns_prefix + name].push_back(flatbuffer_field_to_json(elem_ptr, type, ttptr));
                    elem_ptr += InlineSize(type, ref);
                }
            } else {
                json lval = flatbuffer_field_to_json(val, type, ttptr);
                json_object[ns_prefix + name] = lval;
            }
        } else {
            // There is no value in this field. This is OK except maybe we want to figure out how to enforce
            // required fields. Although maybe flatbuffers schema language and the flatbuffersbuilders have
            // already enforced such things.
        }
    }
    return json_object;
}


#endif //LIBSIGMF_FLATBUFFERS_TO_JSON_VISITOR_H
