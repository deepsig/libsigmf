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

#include "sigmf.h"
#include <nlohmann/json.hpp>

using namespace sigmf;

int main() {


/*
 * this test will fail right now because the annotation core:sample_count field
 * is still not truly optional (default = 0). adding `=null` to this field will
 * make this test pass demonstrating the std::optional scalar field operation.
 */


    std::string starting_str(R"({
    "annotations": [
        {
            "core:description": "no fish",
            "core:sample_count": 0,
            "core:sample_start": 0
        },
        {
            "core:description": "one fish",
            "core:sample_count": 1,
            "core:sample_start": 1
        },
        {
            "core:description": "two fish",
            "core:sample_count": 2,
            "core:sample_start": 2
        },
        {
            "core:description": "red fish",
            "core:freq_lower_edge": 0.0,
            "core:freq_upper_edge": 100000000.0,
            "core:sample_count": 3,
            "core:sample_start": 3
        },
        {
            "core:description": "blue fish",
            "core:sample_count": 4,
            "core:sample_start": 4
        }
    ],
    "captures": [
        {
            "core:frequency": 99999.1,
            "core:sample_start": 0
        },
        {
            "core:frequency": 99999.2,
            "core:sample_start": 42
        }
    ],
    "global": {
        "antenna:mobile": false,
        "antenna:vertical_gain_pattern": [
            0.0,
            1.0,
            6.0,
            10.0,
            6.0,
            1.0,
            0.0
        ],
        "core:author": "DeepSig",
        "core:description": "Round-trip from json -> libsigmf -> back",
        "core:extensions": [
            {
                "name": "fake_extension_1",
                "optional": false,
                "version": "0.1.2.3.4.5.6"
            },
            {
                "name": "another fake ext",
                "optional": true,
                "version": "2"
            }
        ],
        "core:geolocation": {
            "coordinates": [
                12.34,
                5.678,
                9.0
            ],
            "type": "point"
        },
        "core:sample_rate": 1000000.0
    }
})");

    auto as_json = json::parse(starting_str);
    sigmf::SigMF<sigmf::Global<core::DescrT, antenna::DescrT>,
                 sigmf::Capture<core::DescrT, antenna::DescrT>,
                 sigmf::Annotation<core::DescrT, antenna::DescrT> > roundtripstuff = as_json;


    auto back_to_json = json(roundtripstuff);

    std::cout << back_to_json.dump(4) << std::endl;
    assert(back_to_json.dump(4) == starting_str);

    std::cout << "example_sigmf_json_roundtrip passed" << std::endl;
    return 0; // assert passed, we're good :+1:
}
