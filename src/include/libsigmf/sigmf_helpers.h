/*
 * Copyright (C) 2019 DeepSig Inc. All rights reserved.
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

#ifndef SIGMF_HELPERS_H
#define SIGMF_HELPERS_H

#include "sigmf.h"
#include <fstream>


namespace sigmf {
    /**
     *
     * Sort elements of SigMF Vector
     *
     * Standard in place sort using sample_start as the comparand
     *
     * @param sigmf_vector SigMF vector to be sorted
     */
    template<typename SigMFMemberType>
    void sort_sigmf_vector(sigmf::SigMFVector<SigMFMemberType> &sigmf_vector) {
        std::sort(sigmf_vector.begin(), sigmf_vector.end(),
                  [](typename sigmf::SigMFVector<SigMFMemberType>::value_type first,
                     typename sigmf::SigMFVector<SigMFMemberType>::value_type second) -> bool {
                      return (first.template get<core::DescrT>().sample_start <
                              second.template get<core::DescrT>().sample_start);
                  });
    }

    /**
     *
     * Get SigMF element applicable to sample
     *
     * NOTE: SigMF Vector should be validated before calling this function
     *
     * @param sample_number The sample_start of target annotation
     * @param sigmf_vector Reference to non-empty SigMF vector
     * @return Iterator to SigMF element
     *
     */
    template<typename VectorT, typename ComparisonT>
    typename VectorT::iterator get_first_of_sigmf_vector(ComparisonT sample_number, VectorT &sigmf_vector) {
        // get iterator to first SigMF item with a sample_start greater than the reference sample_start
        // NOTE: if no capture sample_start is greater then `upper_bound === sigmf_vector.end()`
        if(sigmf_vector.empty()) {
            throw std::runtime_error("Invalid memory access attempted on SigMF vector.");
        }

        auto upper_bound = std::upper_bound(sigmf_vector.begin(), sigmf_vector.end(), sample_number,
                                            [](ComparisonT sample_number_to_compare,
                                               typename VectorT::value_type first
                                            ) -> bool {
                                                return sample_number_to_compare <
                                                       first.template get<core::DescrT>().sample_start;
                                            }
        );

        // return the capture right before the upper_bound
        return (upper_bound - 1);
    }

    /**
     *
     * Get a subset of SigMF data that applies to a specific number of samples,
     * and normalized to a sample_start, from a larger set of SigMF data
     *
     * @param input_md Full set of SigMF data
     * @param segment_sample_start Starting sample to normalize
     * @param segment_sample_count Number of samples in subset
     *
     * @return Applicable subset of SigMF data
     *
     */
    template<typename G, typename C, typename A>
    sigmf::SigMF<G, C, A>
    get_sigmf_in_range(sigmf::SigMF<G, C, A> &input_md, uint64_t segment_sample_start, uint64_t segment_sample_count) {
        sigmf::SigMF<G, C, A> new_md;
        // Copy global data to latest_metadata
        new_md.global = input_md.global;

        // Copy the normalized captures in range to the latest_metadata
        sort_sigmf_vector(input_md.captures);
        auto applicable_capture = get_first_of_sigmf_vector(segment_sample_start, input_md.captures);
        while (applicable_capture != input_md.captures.end() &&
               (applicable_capture->template get<core::DescrT>().sample_start <
                (segment_sample_start + segment_sample_count))) {
            auto new_capture = *applicable_capture;
            if (segment_sample_start < new_capture.template get<core::DescrT>().sample_start) {
                new_capture.template get<core::DescrT>().sample_start -= segment_sample_start;
            } else {
                new_capture.template get<core::DescrT>().sample_start = 0;
            }
            new_md.captures.emplace_back(new_capture);
            ++applicable_capture;
        }

        // Copy the annotations in range to the latest_metadata
        sort_sigmf_vector(input_md.annotations);
        auto original_annotation = input_md.annotations.begin();
        while (original_annotation != input_md.annotations.end()) {
            bool increment_iterator = true;
            core::AnnotationT &original_annotation_md = original_annotation->template get<core::DescrT>();
            if (original_annotation_md.sample_start >= (segment_sample_start + segment_sample_count)) {
                // annotations are in sorted order, if above is true, all other annotations will be out of bounds
                break;
            } else if ((original_annotation_md.sample_start + original_annotation_md.sample_count) >
                       segment_sample_start) {
                auto new_annotation = *original_annotation;
                uint64_t start_offset = std::max(original_annotation_md.sample_start, segment_sample_start);
                uint64_t count_from_new_offset =
                        original_annotation_md.sample_start + original_annotation_md.sample_count - start_offset;
                uint64_t new_start = start_offset - segment_sample_start;
                uint64_t new_count = std::min(count_from_new_offset, segment_sample_count - new_start);

                auto first_applicable_capture = get_first_of_sigmf_vector(new_start, new_md.captures);
                auto new_sample_last = new_start + new_count - 1;
                auto last_applicable_capture = get_first_of_sigmf_vector(new_sample_last, new_md.captures);

                if (first_applicable_capture != last_applicable_capture) {
                    auto next_applicable_capture = first_applicable_capture + 1;

                    new_count = next_applicable_capture->template get<core::DescrT>().sample_start
                                - new_start;

                    auto implicit_annotation = *original_annotation;
                    implicit_annotation.template get<core::DescrT>().sample_start =
                            next_applicable_capture->template get<core::DescrT>().sample_start + segment_sample_start;

                    original_annotation_md.sample_count =
                            implicit_annotation.template get<core::DescrT>().sample_start -
                            original_annotation_md.sample_start;

                    implicit_annotation.template get<core::DescrT>().sample_count -=
                            original_annotation_md.sample_count;
                    if (implicit_annotation.template get<core::DescrT>().sample_start <
                        (segment_sample_start + segment_sample_count)) {
                        original_annotation = input_md.annotations.insert((original_annotation + 1),
                                                                          implicit_annotation);
                        increment_iterator = false;
                    }
                }
                new_annotation.template get<core::DescrT>().sample_start = new_start;
                new_annotation.template get<core::DescrT>().sample_count = new_count;
                new_md.annotations.emplace_back(new_annotation);
            }
            if (increment_iterator) {
                original_annotation++;
            }
        }
        return std::move(new_md);
    }

    /**
     *
     * Read data from SigMF file into SigMF object
     *
     * @param meta_file Constant reference input file stream
     * @return Pointer to SigMF object
     *
     */
    static sigmf::SigMF<sigmf::Global<core::DescrT>,
            sigmf::Capture<core::DescrT>,
            sigmf::Annotation<core::DescrT>> metadata_file_to_json(const std::ifstream &meta_file) {

        std::ostringstream meta_buffer;
        meta_buffer << meta_file.rdbuf();
        auto sigmf_md = sigmf::SigMF<sigmf::Global<core::DescrT>,
                sigmf::Capture<core::DescrT>,
                sigmf::Annotation<core::DescrT>>();
        
        from_json(json::parse(meta_buffer.str()), sigmf_md);
        return sigmf_md;
    }

} // namespace sigmf

#endif //OMNISIG_SIGMF_HELPERS_H
