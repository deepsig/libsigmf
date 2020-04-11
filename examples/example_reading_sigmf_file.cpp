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

#include "sigmf_core_generated.h"
#include "sigmf_antenna_generated.h"
#include "libsigmf/sigmf.h"
#include "libsigmf/sigmf_helpers.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    std::cout << "Reading file " << argv[1] << std::endl;;
    auto meta_fstream = std::ifstream(argv[1]);
    auto record = sigmf::metadata_file_to_json(meta_fstream);

    std::cout << "The record we read is: \n" << 
        record->to_json().dump(2) << std::endl;
}