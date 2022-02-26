/*
 *    Copyright 2019, 2021, 2022 DeepSig Inc.
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

namespace sigmf {

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
    bool subtable;
    flatbuffers::voffset_t last_offset;
    std::string last_field_name;
    flatbuffers::FlatBufferBuilder fbb;
    flatbuffers::Offset<void> last_fb_offset;


    explicit FromSigMFVisitor(std::string namespace_prefix, const json &j)
            : _start(0), _stop(0), p(namespace_prefix), subtable(false), last_offset(0)  {
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
        // subtables do not have the namespace prefixed to the field name
        last_field_name = subtable ? name : p + name;
        last_offset = flatbuffers::FieldIndexToOffset(static_cast<flatbuffers::voffset_t>(field_idx));
    }

    template<typename T>
    void Named(T x, const char *name) {
        try {
            fbb.AddElement(last_offset, narrowest_json.at(last_field_name).get<T>());
        } catch (nlohmann::detail::out_of_range &e) {
        }
    }

    void UType(uint8_t x, const char *name) override { Named(x, name); }

    void Bool(bool x) override {
        try {
            fbb.AddElement(last_offset, narrowest_json.at(last_field_name).get<bool>());
        } catch (nlohmann::detail::out_of_range &e) {
        }
    }

    void Char(int8_t x, const char *name) override { Named(x, name); }

    void UChar(uint8_t x, const char *name) override { Named(x, name); }

    void Short(int16_t x, const char *name) override { Named(x, name); }

    void UShort(uint16_t x, const char *name) override { Named(x, name); }

    void Int(int32_t x, const char *name) override { Named(x, name); }

    void UInt(uint32_t x, const char *name) override { Named(x, name); }

    void Long(int64_t x) override {
        try {
            fbb.AddElement(last_offset, narrowest_json.at(last_field_name).get<int64_t>());
        } catch (nlohmann::detail::out_of_range &e) {
        }
    }

    void ULong(uint64_t x) override {
        try {
            fbb.AddElement(last_offset, narrowest_json.at(last_field_name).get<uint64_t>());
        } catch (nlohmann::detail::out_of_range &e) {
        }
    }

    void Float(float x) override {
        try {
            fbb.AddElement(last_offset, narrowest_json.at(last_field_name).get<float>());
        } catch (nlohmann::detail::out_of_range &e) {
        }
    }

    void Double(double x) override {
        try {
            fbb.AddElement(last_offset, narrowest_json.at(last_field_name).get<double>());
        } catch (nlohmann::detail::out_of_range &e) {
        }
    }

    void String(const struct flatbuffers::String *str) override {
        try {
            auto strval = narrowest_json.at(last_field_name).get<std::string>();
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
    std::vector<T> tmpvec;
    tmpvec.reserve(jvec.size());
    for (auto elem = jvec.begin(); elem != jvec.end(); ++elem) {
        tmpvec.emplace_back(elem->get<T>());
    }
    return flatbuffers::Offset<void>(fbb.CreateVector(tmpvec).o);

}

inline flatbuffers::Offset<void>
json_vector_to_chararray(flatbuffers::FlatBufferBuilder &fbb, const json &jvec, flatbuffers::ElementaryType type) {
    //size_t num_elements = jvec.size();
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
inline void IterateType(const flatbuffers::TypeTable *type_table, FromSigMFVisitor *visitor, json original_json, bool subtable=false) {
    if (original_json.is_null()) return;  // empty table, not a problem
    const uint8_t *prev_val = nullptr;
    const uint8_t val[8] = {};
    auto comosite_type_offsets = std::map<size_t, flatbuffers::Offset<void> >();
    // Generate the strings up front (because they must be created before our Table in the flatbuffer)
    for (size_t i = 0; i < type_table->num_elems; i++) {
        auto type_code = type_table->type_codes[i];
        auto type = static_cast<flatbuffers::ElementaryType>(type_code.base_type);
        auto is_vector = type_code.is_repeating;
        auto ref_idx = type_code.sequence_ref;
        const flatbuffers::TypeTable *ref = nullptr;
        if (ref_idx >= 0) { ref = type_table->type_refs[ref_idx](); }
        auto name = type_table->names ? type_table->names[i] : nullptr;
        visitor->subtable = subtable;
        // The thing is, at least on this iteration we don't care about making real elementary types, just the
        // ones that need an offset
        auto field_name = subtable ? name : visitor->p + name;
        if (type < flatbuffers::ET_STRING) {
            if (is_vector) {
                auto vector_offset = json_vector_to_chararray(visitor->fbb, original_json[field_name], type);
                auto void_offset = flatbuffers::Offset<void>(vector_offset.o);
                comosite_type_offsets[i] = void_offset;
            }
        } else if (type == flatbuffers::ET_STRING) {
            if (is_vector) {
                std::vector<flatbuffers::Offset<void> > vecofstrings;

                for (const auto &item : original_json[field_name]) {
                    auto strval = item.get<std::string>();
                    auto last_fb_offset = flatbuffers::Offset<void>(visitor->fbb.CreateString(strval).o);

                    vecofstrings.emplace_back(last_fb_offset);
                }
                auto vecoffset = flatbuffers::Offset<void>(visitor->fbb.CreateVector(vecofstrings).o);
                comosite_type_offsets[i] = vecoffset;
            } else {
                if (!original_json[field_name].is_null()) {
                    visitor->Field(i, 0, type, is_vector, ref, name, val, original_json);
                    IterateValue(type, val, ref, prev_val, -1, visitor);
                    comosite_type_offsets[i] = visitor->last_fb_offset;
                }
            }
        } else if (type == flatbuffers::ET_SEQUENCE) {
            if (is_vector) {
                std::vector<flatbuffers::Offset<void> > vecoftables;
                for (const auto &item : original_json[field_name]) {
                    if (!item.is_null()) {
                        auto seq_typetable = type_table->type_refs[ref_idx]();

                        IterateType(seq_typetable, visitor, item, true);
                        auto vecelement = flatbuffers::Offset<void>(visitor->_stop);
                        vecoftables.emplace_back(vecelement);
                    }
                }
                auto vecoffset = flatbuffers::Offset<void>(visitor->fbb.CreateVector(vecoftables).o);
                comosite_type_offsets[i] = vecoffset;
            } else {
                if (!original_json[field_name].is_null()) {
                    auto seq_typetable = type_table->type_refs[ref_idx]();

                    IterateType(seq_typetable, visitor, original_json[field_name], true);
                    comosite_type_offsets[i] = flatbuffers::Offset<void>(visitor->_stop);
                }
            }
        }
    }

    // Now make the table and fill in all of our types
    size_t set_idx = 0;
    visitor->StartSequence();
    for (size_t i = 0; i < type_table->num_elems; i++) {
        auto type_code = type_table->type_codes[i];
        auto type = static_cast<flatbuffers::ElementaryType>(type_code.base_type);
        auto is_vector = type_code.is_repeating;
        auto ref_idx = type_code.sequence_ref;
        const flatbuffers::TypeTable *ref = nullptr;
        if (ref_idx >= 0) { ref = type_table->type_refs[ref_idx](); }
        auto name = type_table->names ? type_table->names[i] : nullptr;
        if (is_vector) {
            auto this_voffset = flatbuffers::FieldIndexToOffset(static_cast<flatbuffers::voffset_t >(i));
            visitor->fbb.AddOffset(this_voffset, comosite_type_offsets.at(i));
        } else if (type == flatbuffers::ET_STRING) {
            try {
                auto this_voffset = flatbuffers::FieldIndexToOffset(static_cast<flatbuffers::voffset_t>(i));
                visitor->fbb.AddOffset(this_voffset, comosite_type_offsets.at(i));
            } catch (std::out_of_range &e) { // this is ok... field wasn't present in json, not deserialized
            }
        } else if (type == flatbuffers::ET_SEQUENCE) {
            try {
                auto this_voffset = flatbuffers::FieldIndexToOffset(static_cast<flatbuffers::voffset_t>(i));
                visitor->fbb.AddOffset(this_voffset, comosite_type_offsets.at(i));
            } catch (std::out_of_range &e) { // this is ok... field wasn't present in json, not deserialized
            }
        } else {
            visitor->Field(i, set_idx, type, is_vector, ref, name, val, original_json);
            set_idx++;
            IterateValue(type, val, ref, prev_val, -1, visitor);
        }
    }
    visitor->EndSequence();
}

template<typename T>
inline json FlatBufferVectorToJson(const flatbuffers::Vector<T> *fbv) {
    std::vector<T> vec;
    for (auto i = fbv->begin(); i != fbv->end(); i++) { vec.push_back(*i); }
    return json(vec);
}

// forward declare so we can create objects as fields
inline json
FlatBufferToJson(const uint8_t *buffer_root, const flatbuffers::TypeTable *typetable, const std::string &ns_prefix = "");

// TODO: function args are redundant since implementation of optional scalars... cleanup
inline json
flatbuffer_field_to_json(const uint8_t *buffer_root,
                         const size_t elem,
                         const flatbuffers::TypeTable *typetable = nullptr,
                         const std::string &ns_prefix = "") {

    const flatbuffers::Table *tbl = reinterpret_cast<const flatbuffers::Table *>(buffer_root);
    auto type_code = typetable->type_codes[elem];
    auto type = static_cast<flatbuffers::ElementaryType>(type_code.base_type);
    auto is_vector = type_code.is_repeating;
    auto buf_voffset = static_cast<flatbuffers::voffset_t>(elem);
    auto field_offset = flatbuffers::FieldIndexToOffset(buf_voffset);
    const uint8_t *val = tbl->GetAddressOf(field_offset);

    switch (type) {
        case flatbuffers::ET_UTYPE: {
            if (is_vector) {
                return FlatBufferVectorToJson(tbl->GetPointer<const flatbuffers::Vector<uint8_t> *>(field_offset));
            } else {
                std::optional<uint8_t> tval = tbl->GetOptional<uint8_t,uint8_t>(field_offset);
                return tval.has_value() ? json(*tval) : json{};
            }
        }
        case flatbuffers::ET_BOOL: {
            if (is_vector) {
                return FlatBufferVectorToJson(tbl->GetPointer<const flatbuffers::Vector<bool> *>(field_offset));
            } else {
                std::optional<bool> tval = tbl->GetOptional<bool,bool>(field_offset);
                return tval.has_value() ? json(*tval) : json{};
            }
        }
        case flatbuffers::ET_CHAR: {
            if (is_vector) {
                return FlatBufferVectorToJson(tbl->GetPointer<const flatbuffers::Vector<uint8_t> *>(field_offset));
            } else {
                std::optional<int8_t> tval = tbl->GetOptional<int8_t,int8_t>(field_offset);
                return tval.has_value() ? json(*tval) : json{};
            }
        }
        case flatbuffers::ET_UCHAR: {
            if (is_vector) {
                return FlatBufferVectorToJson(tbl->GetPointer<const flatbuffers::Vector<uint8_t> *>(field_offset));
            } else {
                std::optional<uint8_t> tval = tbl->GetOptional<uint8_t,uint8_t>(field_offset);
                return tval.has_value() ? json(*tval) : json{};
            }
        }
        case flatbuffers::ET_SHORT: {
            if (is_vector) {
                return FlatBufferVectorToJson(tbl->GetPointer<const flatbuffers::Vector<int16_t> *>(field_offset));
            } else {
                std::optional<int16_t> tval = tbl->GetOptional<int16_t,int16_t>(field_offset);
                return tval.has_value() ? json(*tval) : json{};
            }
        }
        case flatbuffers::ET_USHORT: {
            if (is_vector) {
                return FlatBufferVectorToJson(tbl->GetPointer<const flatbuffers::Vector<uint16_t> *>(field_offset));
            } else {
                std::optional<uint16_t> tval = tbl->GetOptional<uint16_t,uint16_t>(field_offset);
                return tval.has_value() ? json(*tval) : json{};
            }
        }
        case flatbuffers::ET_INT: {
            if (is_vector) {
                return FlatBufferVectorToJson(tbl->GetPointer<const flatbuffers::Vector<int32_t> *>(field_offset));
            } else {
                std::optional<int32_t> tval = tbl->GetOptional<int32_t,int32_t>(field_offset);
                return tval.has_value() ? json(*tval) : json{};
            }
        }
        case flatbuffers::ET_UINT: {
            if (is_vector) {
                return FlatBufferVectorToJson(tbl->GetPointer<const flatbuffers::Vector<uint32_t> *>(field_offset));
            } else {
                std::optional<uint32_t> tval = tbl->GetOptional<uint32_t,uint32_t>(field_offset);
                return tval.has_value() ? json(*tval) : json{};
            }
        }
        case flatbuffers::ET_LONG: {
            if (is_vector) {
                return FlatBufferVectorToJson(tbl->GetPointer<const flatbuffers::Vector<int64_t> *>(field_offset));
            } else {
                std::optional<int64_t> tval = tbl->GetOptional<int64_t,int64_t>(field_offset);
                return tval.has_value() ? json(*tval) : json{};
            }
        }
        case flatbuffers::ET_ULONG: {
            if (is_vector) {
                return FlatBufferVectorToJson(tbl->GetPointer<const flatbuffers::Vector<uint64_t> *>(field_offset));
            } else {
                std::optional<uint64_t> tval = tbl->GetOptional<uint64_t,uint64_t>(field_offset);
                return tval.has_value() ? json(*tval) : json{};
            }
        }
        case flatbuffers::ET_FLOAT: {
            if (is_vector) {
                return FlatBufferVectorToJson(tbl->GetPointer<const flatbuffers::Vector<float> *>(field_offset));
            } else {
                std::optional<float> tval = tbl->GetOptional<float,float>(field_offset);
                return tval.has_value() ? json(*tval) : json{};
            }
        }
        case flatbuffers::ET_DOUBLE: {
            if (is_vector) {
                return FlatBufferVectorToJson(tbl->GetPointer<const flatbuffers::Vector<double> *>(field_offset));
            } else {
                std::optional<double> tval = tbl->GetOptional<double,double>(field_offset);
                return tval.has_value() ? json(*tval) : json{};
            }
        }
        case flatbuffers::ET_STRING: {
            if (is_vector) {
                const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *fbv =
                    tbl->GetPointer<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *>(field_offset);
                std::vector<std::string> vec;
                for (auto i = fbv->begin(); i != fbv->end(); i++) {
                    std::string s = i->str();
                    vec.push_back(s);
                }
                return json(vec);
            } else {
                auto tval = tbl->GetPointer<const flatbuffers::String *>(field_offset);
                return tval ? json(tval->c_str()) : json{};
            }
        }
        case flatbuffers::ET_SEQUENCE: {
            switch (typetable->st) {
                case flatbuffers::ST_TABLE:
                    if (is_vector) {
                        const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::Table>> *fbv =
                            tbl->GetPointer<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::Table>> *>(field_offset);
                        std::vector<json> vec;
                        for (auto i = fbv->begin(); i != fbv->end(); i++) {
                            vec.push_back(FlatBufferToJson(reinterpret_cast<const uint8_t *>(*i), typetable->type_refs[type_code.sequence_ref]()));
                        }
                        return json(vec);
                    } else {
                        val += flatbuffers::ReadScalar<flatbuffers::uoffset_t>(val);
                        return FlatBufferToJson(val, typetable->type_refs[type_code.sequence_ref]());
                    }
                default:
                    // Have not implemented structs, unions, or enums which are all supported by flatbuffers
                    // schema and would be great to have, but dont make as much sense in json-land. They're
                    // on the todo list...
                    break;
            }
        }
        default:
            // Nothing else should be here...
            break;
    }
    return json{};
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
        }
        if (val) {
            json lval = flatbuffer_field_to_json(buffer_root, i, typetable, ns_prefix);
            if (!lval.is_null()) {  // check this since empty subtables will pass `if (val)`
                json_object[ns_prefix + name] = lval;
            }
        } else {
            // There is no value in this field, this is OK as libsigmf does not enforce field presence
        }
    }
    return json_object;
}

} // namespace sigmf

#endif //LIBSIGMF_FLATBUFFERS_TO_JSON_VISITOR_H
