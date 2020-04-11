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

#include "json_wrap.h"
#include <flatbuffers/minireflect.h>
#include <flatbuffers/flatbuffers.h>


/*
 * Designed to be run by our TypeIterator-- it adds fields to a flatbuffer (fbb) it owns by reading from a json
 * object given in its ctor. After the TypeIterator runs the flatbuffer can be made in to a specific flatbuffer
 * object or shipped somewhere
 */
struct FromSigMFVisitor : public flatbuffers::IterationVisitor {
    flatbuffers::uoffset_t _start;
    flatbuffers::uoffset_t _stop;
    std::unique_ptr<json> narrowest_json;
    std::string p;
    std::string last_field_name;
    flatbuffers::FlatBufferBuilder fbb;
    flatbuffers::voffset_t last_offset;
    flatbuffers::Offset<void> last_fb_offset;

    explicit FromSigMFVisitor(std::string namespace_prefix, const json &j);

    void StartSequence() override;

    void EndSequence() override;

    void Field(size_t field_idx, size_t set_idx, flatbuffers::ElementaryType e_type,
               bool is_vector, const flatbuffers::TypeTable *type_table,
               const char *name, const uint8_t *val, json jj);

    template<typename T>
    void Named(T x, const char *name);

    void UType(uint8_t x, const char *name) override;

    // void Bool(bool x) override { s += x ? "true" : "false"; }
    virtual void Char(int8_t x, const char *name) override;

    virtual void UChar(uint8_t x, const char *name) override;

    virtual void Short(int16_t x, const char *name) override;

    virtual void UShort(uint16_t x, const char *name) override;

    virtual void Int(int32_t x, const char *name) override;

    virtual void UInt(uint32_t x, const char *name) override;

    virtual void Long(int64_t x) override;

    virtual void ULong(uint64_t x) override;

    virtual void Float(float x) override;

    virtual void Double(double x) override;

    virtual void String(const struct flatbuffers::String *str) override;

    virtual void StartVector() override;

    virtual void EndVector() override;

    virtual void Element(size_t i, flatbuffers::ElementaryType /*type*/,
                 const flatbuffers::TypeTable * /*type_table*/, const uint8_t * /*val*/) override;
};

flatbuffers::Offset<void>
json_vector_to_chararray(flatbuffers::FlatBufferBuilder &fbb, const json &jvec, flatbuffers::ElementaryType type);

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
void IterateType(const flatbuffers::TypeTable *type_table, FromSigMFVisitor *visitor, json original_json);

// forward declare so we can create objects as fields
json
FlatBufferToJson(const uint8_t *buffer_root, const flatbuffers::TypeTable *typetable, const std::string &ns_prefix,
                 bool include_defaults = false);

json
flatbuffer_field_to_json(const uint8_t *val,
                         flatbuffers::ElementaryType type,
                         const flatbuffers::TypeTable *tt = nullptr,
                         const std::string &ns_prefix = "",
                         bool include_defaults = false);


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
json
FlatBufferToJson(const uint8_t *buffer_root, const flatbuffers::TypeTable *typetable, const std::string &ns_prefix,
                 bool include_defaults);


#endif //LIBSIGMF_FLATBUFFERS_TO_JSON_VISITOR_H
