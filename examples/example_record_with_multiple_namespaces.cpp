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

#include "sigmf_core_generated.h"
#include "sigmf_antenna_generated.h"
#include "sigmf_capture_details_generated.h"
#include "sigmf_signal_generated.h"
#include "sigmf.h"
#include "testing_protocols_generated.h"

using namespace sigmf;

int main() {

    /*
     * Create a record with 2 namespace available in it. Note that antenna namespace doesnt have capture segments.
     * Also use this to show off using the sigmf::Global, sigmf::Capture, and sigmf::Annotation classes. These
     * classes are light wrappers around the sigmf::VariadicDataClass but make it easier to express intent of
     * what kind of stuff (in sigmf-lingo) the underlying class is supposed to hold.
     */
    sigmf::SigMF<sigmf::Global<core::DescrT, antenna::DescrT>,
            sigmf::Capture<core::DescrT>,
            sigmf::Annotation<core::DescrT, antenna::DescrT, capture_details::DescrT, signal::DescrT> > latest_record;
    latest_record.global.access<core::GlobalT>().author = "Nathan";
    latest_record.global.access<core::GlobalT>().description = "Example of creating a new record";
    latest_record.global.access<core::GlobalT>().sample_rate = 1.0;
    latest_record.global.access<antenna::GlobalT>().mobile = true;
    latest_record.global.access<antenna::GlobalT>().gain = 40.0;
    latest_record.global.access<antenna::GlobalT>().low_frequency = 600e6;
    latest_record.global.access<antenna::GlobalT>().high_frequency = 1200e6;
    latest_record.global.access<antenna::GlobalT>().vertical_gain_pattern = {-40,-40,-40,0,10,0,-40,-40,-40};

    // Add a capture segment
    auto antenna_capture = sigmf::Capture<core::DescrT>();
    antenna_capture.get<core::DescrT>().frequency = 870e6;
    antenna_capture.get<core::DescrT>().global_index = 0;
    latest_record.captures.emplace_back(antenna_capture);

    auto &fancy_capture = latest_record.captures.create_new();
    auto &fancy_cap_core = fancy_capture.get<core::DescrT>();
    fancy_cap_core.datetime = "the future";
    fancy_cap_core.sample_start = 9001;


    // Add some annotations (sigmf::core_annotations is typedef of core::AnnotationT, so they're interchangeable)
    // This example uses the core::AnnotationT to access data elements which is more using the VariadicDataClass interface
    auto anno2 = sigmf::Annotation<core::DescrT, antenna::DescrT, capture_details::DescrT, signal::DescrT>();
    anno2.access<core::AnnotationT>().sample_count = 500000;
    anno2.access<core::AnnotationT>().description = "Annotation 1";
    anno2.access<core::AnnotationT>().generator = "libsigmf";
    anno2.access<core::AnnotationT>().description = "Woah!";
    latest_record.annotations.emplace_back(anno2);

    // This example shows off using the Annotation-specific interface where we know it's an annotation, so we
    // get annotation field from the underlying DescrT... This uses a little bit of syntactic sugar on top of
    // the VariadicDataClass and basically you don't have to repeat "annotation" in your get/access method.
    auto anno3 = sigmf::Annotation<core::DescrT, antenna::DescrT, capture_details::DescrT, signal::DescrT>();
    anno3.get<core::DescrT>().sample_count = 600000;
    anno3.get<core::DescrT>().description = "Annotation 2";
    anno3.get<core::DescrT>().generator = "libsigmf";
    anno3.get<core::DescrT>().description = "Pretty easy";
    anno3.get<antenna::DescrT>().elevation_angle = 4.2;
    // here are some examples of how to initialize subtable fields:
    signal::signal_detailT detail_obj;
    detail_obj.type = std::string("analog");
    detail_obj.duplexing = std::string("fdm");
    anno3.get<signal::DescrT>().detail = std::make_shared<signal::signal_detailT>(detail_obj);
    core::geojson_pointT anno3_geo;
    anno3_geo.type = "point";
    anno3_geo.coordinates = std::vector<double>( { 98, 123.4, 1e-9} );
    signal::signal_emitterT emitter_obj;
    emitter_obj.manufacturer = std::string("deepsig");
    emitter_obj.power_tx = 27.1;
    emitter_obj.geolocation = std::make_shared<core::geojson_pointT>(anno3_geo);
    anno3.get<signal::DescrT>().emitter = std::make_shared<signal::signal_emitterT>(emitter_obj);
    // You can also drop in this syntactic acid using this interface which I personally don't really like because
    // it mixes real calls with macros without it being obvious and doesn't really feel like c++
    anno3.sigmfns(antenna).azimuth_angle = 0.1;
    anno3.get<antenna::DescrT>().polarization = "circular";
    anno3.get<capture_details::DescrT>().SNRdB = 12.34;

    latest_record.annotations.emplace_back(anno3);


    auto expected_json = R"({
  "annotations": [
    {
      "core:description": "Woah!",
      "core:generator": "libsigmf",
      "core:sample_count": 500000
    },
    {
      "antenna:azimuth_angle": 0.1,
      "antenna:elevation_angle": 4.2,
      "antenna:polarization": "circular",
      "capture_details:SNRdB": 12.34,
      "core:description": "Pretty easy",
      "core:generator": "libsigmf",
      "core:sample_count": 600000,
      "signal:detail": {
        "duplexing": "fdm",
        "type": "analog"
      },
      "signal:emitter": {
        "geolocation": {
          "coordinates": [
            98.0,
            123.4,
            1e-09
          ],
          "type": "point"
        },
        "manufacturer": "deepsig",
        "power_tx": 27.1
      }
    }
  ],
  "captures": [
    {
      "core:frequency": 870000000.0,
      "core:global_index": 0
    },
    {
      "core:datetime": "the future",
      "core:sample_start": 9001
    }
  ],
  "global": {
    "antenna:gain": 40.0,
    "antenna:high_frequency": 1200000000.0,
    "antenna:low_frequency": 600000000.0,
    "antenna:mobile": true,
    "antenna:vertical_gain_pattern": [
      -40.0,
      -40.0,
      -40.0,
      0.0,
      10.0,
      0.0,
      -40.0,
      -40.0,
      -40.0
    ],
    "core:author": "Nathan",
    "core:description": "Example of creating a new record",
    "core:sample_rate": 1.0
  }
})";

    // std::cout << "JSON Record:\n" << json(latest_record).dump(2) << std::endl;
    assert(expected_json == json(latest_record).dump(2));

    std::cout << "example_record_with_multiple_namespaces passed" << std::endl;
    return 0; // assert passed, we're good :+1:
}

