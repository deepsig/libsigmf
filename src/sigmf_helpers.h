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

#ifndef SIGMF_HELPERS_H
#define SIGMF_HELPERS_H

#include "sigmf.h"
#include "sigmf_core_generated.h"
#include "sigmf_antenna_generated.h"
#include "sigmf_capture_details_generated.h"
#include "sigmf_signal_generated.h"
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
        if (segment_sample_start < new_capture.template get<core::DescrT>().sample_start.value_or(0)) {
            new_capture.template get<core::DescrT>().sample_start =
                new_capture.template get<core::DescrT>().sample_start.value_or(0) - segment_sample_start;
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

        if (!original_annotation_md.sample_start.has_value()) {
            continue;  // this is not a valid annotation, skip it
        }

        if (original_annotation_md.sample_start.value() >= (segment_sample_start + segment_sample_count)) {
            // annotations are in sorted order, if above is true, all other annotations will be out of bounds
            break;
        } else if ((original_annotation_md.sample_start.value() + original_annotation_md.sample_count.value_or(0)) >
                   segment_sample_start) {
            auto new_annotation = *original_annotation;
            uint64_t start_offset = std::max(original_annotation_md.sample_start.value(), segment_sample_start);
            uint64_t count_from_new_offset =
                    original_annotation_md.sample_start.value() + original_annotation_md.sample_count.value_or(0) - start_offset;
            uint64_t new_start = start_offset - segment_sample_start;
            uint64_t new_count = std::min(count_from_new_offset, segment_sample_count - new_start);

            auto first_applicable_capture = get_first_of_sigmf_vector(new_start, new_md.captures);
            auto new_sample_last = new_start + new_count - 1;
            auto last_applicable_capture = get_first_of_sigmf_vector(new_sample_last, new_md.captures);

            if (first_applicable_capture != last_applicable_capture) {
                auto next_applicable_capture = first_applicable_capture + 1;

                new_count = next_applicable_capture->template get<core::DescrT>().sample_start.value()
                            - new_start;

                auto implicit_annotation = *original_annotation;
                implicit_annotation.template get<core::DescrT>().sample_start =
                        next_applicable_capture->template get<core::DescrT>().sample_start.value() +
                        segment_sample_start;

                original_annotation_md.sample_count =
                        implicit_annotation.template get<core::DescrT>().sample_start.value() -
                        original_annotation_md.sample_start.value();

                implicit_annotation.template get<core::DescrT>().sample_count =
                        implicit_annotation.template get<sigmf::core::DescrT>().sample_count.value_or(0) -
                        original_annotation_md.sample_count.value_or(0);
                if (implicit_annotation.template get<core::DescrT>().sample_start.value() <
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
static std::unique_ptr<sigmf::SigMF<sigmf::Global<core::DescrT, antenna::DescrT, capture_details::DescrT, signal::DescrT>,
                                    sigmf::Capture<core::DescrT, antenna::DescrT, capture_details::DescrT, signal::DescrT>,
                                    sigmf::Annotation<core::DescrT, antenna::DescrT, capture_details::DescrT, signal::DescrT>>>
        metadata_file_to_json(const std::ifstream &meta_file) {

    std::ostringstream meta_buffer;
    meta_buffer << meta_file.rdbuf();
    auto sigmf_md = std::make_unique<sigmf::SigMF<
                        sigmf::Global<core::DescrT, antenna::DescrT, capture_details::DescrT, signal::DescrT>,
                        sigmf::Capture<core::DescrT, antenna::DescrT, capture_details::DescrT, signal::DescrT>,
                        sigmf::Annotation<core::DescrT, antenna::DescrT, capture_details::DescrT, signal::DescrT>>>();

    *sigmf_md = json::parse(meta_buffer.str());
    return sigmf_md;
}

/**
 *
 * Returns the size of a single data sample in bytes given the datatype string
 *
 * @param datatype_string
 * @return size of a sample in bytes
 */
static uint32_t get_sample_size(std::string dtype_str)
{
    uint32_t sample_size;
    // possible bit widths are 64, 32, 16, 8
    if (dtype_str.find("64") != std::string::npos) {
        sample_size = 8;
    } else if (dtype_str.find("32") != std::string::npos) {
        sample_size = 4;
    } else if (dtype_str.find("16") != std::string::npos) {
        sample_size = 2;
    } else if (dtype_str.find("8") != std::string::npos) {
        sample_size = 1;
    } else {
        throw std::invalid_argument("Invalid datatype string.");
    }

    if (dtype_str.at(0) == 'c') {
        sample_size *= 2;
    }
    return sample_size;
}

struct SigMFCaptureBoundary {
    uint64_t start_byte;
    uint64_t stop_byte;
};

/**
 *
 * Returns the file byte start offset for a given captures array
 *
 * @param captures_vector SigMF captures vector
 * @param index specific capture to return information for
 * @param sample_size size of one sample in bytes
 */
template<typename CaptureType>
static SigMFCaptureBoundary get_capture_range(sigmf::SigMFVector<CaptureType> &captures_vector, int index, int sample_size) {
    if (index >= captures_vector.size()) {
        // index exceeds captures vector length, no data to read
        return SigMFCaptureBoundary{0,0};
    }
    SigMFCaptureBoundary bounds;
    bounds.start_byte = captures_vector[0].template get<core::DescrT>().header_bytes.value_or(0);
    for (auto ii = 1; ii <= index; ii++) {
        bounds.start_byte += captures_vector[ii].template get<core::DescrT>().header_bytes.value_or(0);
    }
    bounds.start_byte += captures_vector[index].template get<core::DescrT>().sample_start * sample_size;
    bounds.stop_byte = bounds.start_byte + captures_vector[index].template get<core::DescrT>().sample_start * sample_size;
    return bounds;
}

} // namespace sigmf

#endif //OMNISIG_SIGMF_HELPERS_H
