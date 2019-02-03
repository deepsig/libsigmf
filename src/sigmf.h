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

#include "variadic_data_class.h"
#include "global.h"
#include "capture.h"
#include "annotation.h"
#include <vector>
#include <sigmf_core_generated.h>
#include <iostream>


namespace sigmf {

    template<typename T>
    class SigMFVector : public std::vector<T> {
    public:
        T &create_new() {
            T new_element;
            this->emplace_back(new_element);
            return this->back();
        }
    };

    template<typename GlobalType, typename CaptureType, typename AnnotationType>
    struct SigMF {
        GlobalType global;
        SigMFVector<CaptureType> captures;
        SigMFVector<AnnotationType> annotations;
    };

    template<typename GlobalType, typename CaptureType, typename AnnotationType>
    void to_json(json &j, const SigMF<GlobalType, CaptureType, AnnotationType> t) {
        j["global"] = t.global.to_json();
        j["captures"] = t.captures;
        j["annotations"] = t.annotations;
    }

    template<typename GlobalType, typename CaptureType, typename AnnotationType>
    void from_json(const json &j, SigMF<GlobalType, CaptureType, AnnotationType> &t) {
        t.global.from_json(j["global"]);
        t.captures.clear();
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
