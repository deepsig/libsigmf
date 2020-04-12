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

#include "libsigmf/sigmf.h"
#include <nlohmann/json.hpp>
#include <iostream>

int main() {

    std::string starting_str(R"({
    "annotations": [
        {
            "core:description": "one fish",
            "core:sample_count": 1
        },
        {
            "core:description": "two fish",
            "core:sample_count": 2
        },
        {
            "core:description": "red fish",
            "core:sample_count": 3
        },
        {
            "core:description": "blue fish",
            "core:sample_count": 4
        }
    ],
    "captures": [
        {
            "core:sample_start": 42
        }
    ],
    "global": {
        "core:author": "DeepSig",
        "core:description": "Round-trip from json -> libsigmf -> back"
    }
})");

    auto as_json = json::parse(starting_str);
    sigmf::SigMF<sigmf::Global<core::DescrT>,
            sigmf::Capture<core::DescrT>,
            sigmf::Annotation<core::DescrT> > roundtripstuff = as_json;


    auto back_to_json = json(roundtripstuff);

    std::cout << back_to_json.dump(4) << std::endl;
    assert(back_to_json.dump(4) == starting_str);
}