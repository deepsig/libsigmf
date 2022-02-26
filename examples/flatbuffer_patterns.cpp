/*
 *    Copyright 2019, 2022 DeepSig Inc.
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

#include "flatbuffers/minireflect.h" // For flatbuffers::FlatBufferToString
#include "sigmf_core_generated.h"
#include "testing_protocols_generated.h"
#include <iostream>
#include <stdint.h>

using namespace sigmf;

int main() {
    //
    // The standard way of dealing with flatbuffers...
    //
    // You have to create a FlatBufferBuilder which manages the memory and has generic mutators for all of flatbuffer's internal types
    flatbuffers::FlatBufferBuilder fbb;
    // This is a Builder for our type-- you need to give it a FlatBufferBuilder
    // This basically knows how to translate our named fields in to flatbuffer's internal types/field indexes
    core::AnnotationBuilder abb(fbb);
    // Now we can set some field
    abb.add_freq_lower_edge(30.0);

    // When you're done adding data you finish your buffer
    flatbuffers::Offset<core::Annotation> offset_in_buffer = abb.Finish();
    // Now you have to finish the original buffer by telling it the offset<core::Annotation> (basically how big our type is)
    fbb.Finish(offset_in_buffer);

    // Now you can get an object back from the original FlatBufferBuilder using a templated GetRoot...
    const core::Annotation *anno = flatbuffers::GetRoot<core::Annotation>(fbb.GetBufferPointer());
    // And inspect the values you put it
    std::cout << anno->freq_lower_edge().value_or(-1) << std::endl;

    //
    // Adding more complex types (strings, composite types,... )
    //
    flatbuffers::FlatBufferBuilder composite_fbb;
    uint32_t vvv[] = {1, 2, 3, 4, 5, 6, 5, 4, 3, 2, 1};
    std::cout << "vec has size " << sizeof(vvv) / sizeof(uint32_t) << std::endl;
    auto vecoffset = composite_fbb.CreateVector<uint32_t>(vvv, sizeof(vvv) / sizeof(uint32_t));

    auto stroffset = composite_fbb.CreateString("testing one two");
    std::vector<flatbuffers::Offset<flatbuffers::String> > nativevecstr = {composite_fbb.CreateString("one"),
                                                                           composite_fbb.CreateString("two"),
                                                                           composite_fbb.CreateString("thre")};

    auto vecofstrings = composite_fbb.CreateVector(nativevecstr);
    // need Offset<Vector<Offset<String> > > >

    testing_vecsBuilder vecbuilder(composite_fbb);
    vecbuilder.add_myvecfortesting(vecoffset);
    vecbuilder.add_name(stroffset);
    auto testvecadded = vecbuilder.Finish();
    composite_fbb.Finish(testvecadded);

    const testing_vecs *vecs = flatbuffers::GetRoot<testing_vecs>(composite_fbb.GetBufferPointer());
    std::cout << "O: " << flatbuffers::FlatBufferToString(composite_fbb.GetBufferPointer(), testing_vecsTypeTable())
              << std::endl;



    //
    // Now there is also the object API which we use exclusively in libsigmf....
    //
    core::AnnotationT annotation_object;
    annotation_object.generator = "this is much easier";
    annotation_object.sample_count = 1;
    // To get a transportable (encoded) flatbuffer you need to pack it with a flatbuffer...
    flatbuffers::FlatBufferBuilder fbb2;
    // (A core::Annotation is a core::AnnotationT::TableType, but it's exposed through the object API)
    // From the object, you need to pack it in to a FlatBufferBuilder and Finish that
    auto offset = core::AnnotationT::TableType::Pack(fbb2, &annotation_object); // this is a static method
    fbb2.Finish(offset);
    std::cout << "presumably we would transmit starting at fbb2.GetBufferPointer() for " << fbb2.GetSize() << " B"
              << std::endl;
    std::cout << "O: " << flatbuffers::FlatBufferToString(fbb2.GetBufferPointer(),
                                                          core::AnnotationT::TableType::MiniReflectTypeTable())
              << std::endl;
    // If you want to access that buffer as a core::Annotation...
    const core::AnnotationT::TableType *packed_anno = flatbuffers::GetRoot<core::AnnotationT::TableType>(
            fbb2.GetBufferPointer());
    std::cout << packed_anno->generator()->c_str() << std::endl;


    //
    // For the more generic building case we can just use the enums to fill the fbb
    //
    flatbuffers::FlatBufferBuilder generic_fbb;
    auto strloc = generic_fbb.CreateString("glarble");
    auto startoftable = generic_fbb.StartTable();
    generic_fbb.AddElement(core::AnnotationT::TableType::VT_FREQ_LOWER_EDGE, double(4.0), double(0.0));
    generic_fbb.AddElement(core::AnnotationT::TableType::VT_FREQ_UPPER_EDGE, double(5.0), double(0.0));
    generic_fbb.AddOffset(core::AnnotationT::TableType::VT_GENERATOR, strloc);
    auto endoftable = generic_fbb.EndTable(startoftable);
    generic_fbb.Finish(flatbuffers::Offset<core::Annotation>(endoftable));
    const core::AnnotationT::TableType *generic_anno = flatbuffers::GetRoot<core::AnnotationT::TableType>(
            generic_fbb.GetBufferPointer());
    std::cout << generic_anno->freq_lower_edge().value_or(-1) << std::endl;
    std::cout << generic_anno->freq_upper_edge().value_or(-1) << std::endl;
    std::cout << generic_anno->generator()->c_str() << std::endl;

}
