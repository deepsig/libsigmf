//
// Copyright (C) 2019 DeepSig Inc. All rights reserved.
//
// This document is the property of DeepSig Inc.
// It is considered confidential and proprietary
//
// This document may not be reproduced or transmitted in any form
// in whole or in part, without the express written permission of
// DeepSig Inc.
//

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
