
cmake_minimum_required(VERSION 3.0)
project(libsigmf CXX)

# c++14 used for auto return type for functions without having to do the -> declaration
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH cmake)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/flatbuffers
        ${CMAKE_CURRENT_BINARY_DIR}/flatbuffers-build
        EXCLUDE_FROM_ALL)

function(generate_sigmf_header generate_sigmf_target FBS_FILE OUTPUT_LOCATION)
    add_custom_target(generate_sigmf_target_${generate_sigmf_target}
            COMMAND flatc -c --reflect-types --reflect-names --gen-object-api -o "${OUTPUT_LOCATION}/" "${FBS_FILE}"
            COMMENT "Building C++ header for flatbuffers definition ${FBS_FILE}"
            WORKING_DIRECTORY .)
    add_library(${generate_sigmf_target} INTERFACE)
    add_dependencies(${generate_sigmf_target} generate_sigmf_target_${generate_sigmf_target} flatc)
    target_include_directories(${generate_sigmf_target} INTERFACE "${OUTPUT_LOCATION}/")
endfunction(generate_sigmf_header)


# TODO: check that these directories exist and submodule init them if not
set(JSON_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/json/single_include")
set(FLATBUFFERS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/flatbuffers/include")

# Create targets for generating the namespace definitions that we ship by default (using this requiers building flatc)
generate_sigmf_header(generated_core_ns "${CMAKE_CURRENT_SOURCE_DIR}/sigmf_protocols/sigmf_core.fbs" "${CMAKE_CURRENT_BINARY_DIR}/include")
generate_sigmf_header(generated_antenna_ns "${CMAKE_CURRENT_SOURCE_DIR}/sigmf_protocols/sigmf_antenna.fbs" "${CMAKE_CURRENT_BINARY_DIR}/include")
generate_sigmf_header(generated_testing_ns "${CMAKE_CURRENT_SOURCE_DIR}/sigmf_protocols/testing_protocols.fbs" "${CMAKE_CURRENT_BINARY_DIR}/include")
# We also carry around pre-generated headers so downstream doesn't need to build flatc
set(SIGMF_GEN_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/sigmf_protocols")

# Our interface target that downstream should ideally use target_link_libraries( ) with to get access to our include dirs
add_library(libsigmf INTERFACE)
target_include_directories(libsigmf INTERFACE ${JSON_INCLUDE_DIR} ${FLATBUFFERS_INCLUDE_DIR} ${SIGMF_GEN_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Ensure that our protocol headers are generated before libsigmf dependency is satisfied (i.e. so examples have them)
add_dependencies(libsigmf libsigmf_genheaders)
add_custom_target(libsigmf_genheaders DEPENDS
        generate_sigmf_target_generated_core_ns
        generate_sigmf_target_generated_antenna_ns
        generate_sigmf_target_generated_testing_ns
        )

add_subdirectory(examples)

add_custom_target(makedocs
        COMMAND mkdocs build
        COMMENT "Building documentation website"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/SigmfConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/SigmfConfig.cmake
        @ONLY
)

# TODO: can we install flatc with a different binary name?  sigmf-flatc?
#install(TARGETS flatc DESTINATION bin)
INSTALL(  # install original headers
        DIRECTORY ${CMAKE_SOURCE_DIR}/src/
        DESTINATION include/sigmf
        FILES_MATCHING PATTERN "*.h*")
INSTALL(  # install flatbuffers headers
        DIRECTORY ${CMAKE_SOURCE_DIR}/external/flatbuffers/include/flatbuffers/
        DESTINATION include/sigmf/external/flatbuffers/
        FILES_MATCHING PATTERN "*.h*")
INSTALL(  # install nlohmann headers
        DIRECTORY ${CMAKE_SOURCE_DIR}/external/json/include/nlohmann/
        DESTINATION include/sigmf/external/nlohmann/
        FILES_MATCHING PATTERN "*.hpp*")
INSTALL(  # install flatbuf proto defs
        DIRECTORY ${CMAKE_SOURCE_DIR}/sigmf_protocols/
        DESTINATION include/sigmf/fbs
        FILES_MATCHING PATTERN "*.fbs*")
INSTALL(  # install generated headers
        DIRECTORY ${CMAKE_BINARY_DIR}/include/
        DESTINATION include/sigmf/
        FILES_MATCHING PATTERN "*.h*")
INSTALL(
        FILES ${CMAKE_BINARY_DIR}/cmake/SigmfConfig.cmake
        DESTINATION cmake/sigmf
)
