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

#ifndef LIBSIGMF_SIGMF_H
#define LIBSIGMF_SIGMF_H

#include "variadic_data_class.h"
#include "global.h"
#include "capture.h"
#include "annotation.h"
#include "collection.h"
#include "sigmf_core_generated.h"
#include "sigmf_antenna_generated.h"
#include "sigmf_capture_details_generated.h"
#include "sigmf_signal_generated.h"
#include <vector>
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

    /**
     * Export the record to a JSON object
     */
    json to_json() const {
        json j;
        j["global"] = global.to_json();
        j["captures"] = captures;
        j["annotations"] = annotations;
        return j;
    }

    /**
     * Write over the fields with a new record from a JSON object
     */
    void from_json(const json &j) {
        global.from_json(j["global"]);
        captures.clear();
        annotations.clear();
        for (auto &element : j["annotations"]) {
            AnnotationType a;
            a.from_json(element);
            annotations.emplace_back(a);
        }
        for (auto &element : j["captures"]) {
            CaptureType c;
            c.from_json(element);
            captures.emplace_back(c);
        }
    }
};

template<typename CollectionType>
struct SigMFCollection {
    CollectionType collection;

    /**
     * Export the record to a JSON object
     */
    json to_json() const {
        json j;
        j["collection"] = collection.to_json();
        return j;
    }

    /**
     * Write over the fields with a new record from a JSON object
     */
    void from_json(const json &j) {
        collection.from_json(j["collection"]);
    }
};

/*
 * This makes conversion between json types and SigMF types work out of the box
 */

template<typename GlobalType, typename CaptureType, typename AnnotationType>
void to_json(json &j, const SigMF<GlobalType, CaptureType, AnnotationType> &t) {
    j = t.to_json();
}

template<typename GlobalType, typename CaptureType, typename AnnotationType>
void from_json(const json &j, SigMF<GlobalType, CaptureType, AnnotationType> &t) {
    t.from_json(j);
}

template<typename CollectionType>
void to_json(json &j, const SigMFCollection<CollectionType> &t) {
    j = t.to_json();
}

template<typename CollectionType>
void from_json(const json &j, SigMFCollection<CollectionType> &t) {
    t.from_json(j);
}

} // namespace sigmf

#endif //LIBSIGMF_SIGMF_H
