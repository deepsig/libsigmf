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


#include <libsigmf/src/variadic_data_class.h>
#include <flatbuffers_json_visitor.h>
#include <flatbuffers/minireflect.h>

#ifndef OMNISIG_FLATBUFFERS_TYPE_TO_JSON_H
#define OMNISIG_FLATBUFFERS_TYPE_TO_JSON_H

using json = nlohmann::json;

template<typename R>
static json to_json(const typename R::NativeTableType &obj, bool include_defaults) {

    R *ttype;

    flatbuffers::FlatBufferBuilder fbb;
    auto loc = ttype->Pack(fbb, &obj);
    fbb.Finish(loc);

    auto bfrptr = fbb.GetBufferPointer();
    auto rtptr = flatbuffers::GetRoot<uint8_t>(bfrptr);
    json asjson = FlatBufferToJson(rtptr, ttype->MiniReflectTypeTable(), "", include_defaults);

    return asjson;
}

template<typename R>
static typename R::NativeTableType from_json(json json_value) {
    R *ttype;
    typename R::NativeTableType return_object;

    auto reflection_table = ttype->MiniReflectTypeTable();

    FromSigMFVisitor dumb_ripoff_visitor("", json_value);
    IterateType(reflection_table, &dumb_ripoff_visitor, json_value);
    dumb_ripoff_visitor.fbb.Finish(flatbuffers::Offset<void>(dumb_ripoff_visitor._stop));

    auto *annoptr = flatbuffers::GetRoot<R>(dumb_ripoff_visitor.fbb.GetBufferPointer());
    annoptr->UnPackTo(&return_object);

    return return_object;
}

#endif //OMNISIG_FLATBUFFERS_TYPE_TO_JSON_H
