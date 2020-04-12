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

#ifndef LIBSIGMF_SIGMF_H
#define LIBSIGMF_SIGMF_H

#include "global.h"
#include "capture.h"
#include "annotation.h"
#include "json_wrap.h"
#include <nlohmann/json.hpp>
#include "sigmf_forward.h"
#include "sigmf_core_generated.h"
#include <vector>


namespace sigmf {

    /*
     * This makes conversion between json types and SigMF types work out of the box
     */

    template<typename GlobalType, typename CaptureType, typename AnnotationType>
    json to_json(const SigMF<GlobalType, CaptureType, AnnotationType> t) {
        json j;
        j["global"] = t.global.to_json();
        j["captures"] = t.captures;
        j["annotations"] = t.annotations;
        return j;
    }

    template<typename GlobalType, typename CaptureType, typename AnnotationType>
    void to_json(json &j, const SigMF<GlobalType, CaptureType, AnnotationType> t) {
        j["global"] = t.global.to_json();
        j["captures"] = t.captures;
        j["annotations"] = t.annotations;
    }

    template<typename GlobalType, typename CaptureType, typename AnnotationType>
    SigMF<GlobalType, CaptureType, AnnotationType> from_json(const json &j) {
        SigMF<GlobalType, CaptureType, AnnotationType> t;
        t.global.from_json(j["global"]);
        t.captures.clear();
        t.annotations.clear();
        for (auto &element : j["annotations"]) {
            AnnotationType a;
            a.from_json(element);
            t.annotations.emplace_back(a);
        }
        for (auto &element : j["captures"]) {
            CaptureType c;
            c.from_json(element);
            t.captures.emplace_back(c);
        }
        return t;
    }

    template<typename GlobalType, typename CaptureType, typename AnnotationType>
    void from_json(const json &j, SigMF<GlobalType, CaptureType, AnnotationType> &t) {
            t.global.from_json(j["global"]);
            t.captures.clear();
            t.annotations.clear();
            for (auto &element : j["annotations"]) {
                AnnotationType a;
                a.from_json(element);
                t.annotations.emplace_back(a);
            }
            for (auto &element : j["captures"]) {
                CaptureType c;
                c.from_json(element);
                t.captures.emplace_back(c);
            }
        }
    }


#endif //LIBSIGMF_SIGMF_H
