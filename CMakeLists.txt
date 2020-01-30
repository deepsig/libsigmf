
########################################################################
# Setup project
########################################################################
cmake_minimum_required(VERSION 3.14)
project(libsigmf LANGUAGES CXX)

# c++14 used for auto return type for functions without having to do the -> declaration
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH cmake)
option(ENABLE_EXAMPLES "Enable building of examples" ON)

include(GNUInstallDirs)

set(LIBSIGMF_TARGET_NAME               ${PROJECT_NAME})
set(LIBSIGMF_CONFIG_INSTALL_DIR        "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}" CACHE INTERNAL "")
set(LIBSIGMF_INCLUDE_INSTALL_DIR       "${CMAKE_INSTALL_INCLUDEDIR}")
set(LIBSIGMF_TARGETS_EXPORT_NAME       "${PROJECT_NAME}Targets")
set(LIBSIGMF_CMAKE_CONFIG_TEMPLATE     "cmake/config.cmake.in")
set(LIBSIGMF_CMAKE_CONFIG_DIR          "${CMAKE_CURRENT_BINARY_DIR}")
set(LIBSIGMF_CMAKE_PROJECT_CONFIG_FILE "${LIBSIGMF_CMAKE_CONFIG_DIR}/${PROJECT_NAME}Config.cmake")
set(LIBSIGMF_CMAKE_PROJECT_TARGETS_FILE "${LIBSIGMF_CMAKE_CONFIG_DIR}/${PROJECT_NAME}Targets.cmake")

########################################################################
# Dependency management
########################################################################
set(libsigmf_deps "")

# flatbuffers
find_package(Flatbuffers REQUIRED)
list(APPEND libsigmf_deps flatbuffers::flatbuffers_shared)

# nlohmann_json
find_package(nlohmann_json REQUIRED)

########################################################################
# Use flatbuffers to generate SigMF headers
########################################################################
function(generate_sigmf_header generate_sigmf_target FBS_FILE OUTPUT_LOCATION)
    get_filename_component(generated_output_name ${FBS_FILE} NAME_WE)
    message(STATUS "Flatbuffers generated ${generated_output_name}")
    add_custom_command(OUTPUT ${OUTPUT_LOCATION}/${generated_output_name}_generated.h
            COMMAND flatc -c --reflect-types --reflect-names --gen-object-api -o "${OUTPUT_LOCATION}/" "${FBS_FILE}"
            COMMENT "Building C++ header for flatbuffers definition ${FBS_FILE}"
            WORKING_DIRECTORY .
            DEPENDS "${FBS_FILE}"
    )
    add_custom_target(generate_sigmf_target_${generate_sigmf_target}
            DEPENDS ${OUTPUT_LOCATION}/${generated_output_name}_generated.h
            )
    add_library(${generate_sigmf_target} INTERFACE)
    add_dependencies(${generate_sigmf_target} generate_sigmf_target_${generate_sigmf_target} flatc)
    target_include_directories(${generate_sigmf_target} INTERFACE "${OUTPUT_LOCATION}/")
endfunction(generate_sigmf_header)


########################################################################
# Create targets for generating default namespace definitions
########################################################################
generate_sigmf_header(generated_core_ns
  "${CMAKE_CURRENT_SOURCE_DIR}/sigmf_protocols/sigmf_core.fbs"
  "${CMAKE_CURRENT_BINARY_DIR}/include"
)
generate_sigmf_header(generated_antenna_ns
  "${CMAKE_CURRENT_SOURCE_DIR}/sigmf_protocols/sigmf_antenna.fbs"
  "${CMAKE_CURRENT_BINARY_DIR}/include"
)
generate_sigmf_header(generated_testing_ns
  "${CMAKE_CURRENT_SOURCE_DIR}/sigmf_protocols/testing_protocols.fbs"
  "${CMAKE_CURRENT_BINARY_DIR}/include"
  )

# We also carry around pre-generated headers so downstream doesn't need to build flatc
set(LIBSIGMF_GEN_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/sigmf_protocols")

########################################################################
# Our interface target that downstream have to use
########################################################################
add_library(libsigmf INTERFACE)
target_include_directories(libsigmf INTERFACE
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include/sigmf/fbs>
  $<BUILD_INTERFACE:${LIBSIGMF_GEN_HEADERS}>
)
target_link_libraries(libsigmf INTERFACE
  ${libsigmf_deps}
)
########################################################################
# Ensure that our protocol headers are generated before libsigmf dep
########################################################################
add_dependencies(libsigmf libsigmf_genheaders)
add_custom_target(libsigmf_genheaders DEPENDS
        generate_sigmf_target_generated_core_ns
        generate_sigmf_target_generated_antenna_ns
        generate_sigmf_target_generated_testing_ns
)

if (ENABLE_EXAMPLES)
  add_subdirectory(examples)
endif (ENABLE_EXAMPLES)

add_custom_target(makedocs
        COMMAND mkdocs build
        COMMENT "Building documentation website"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/${LIBSIGMF_CMAKE_CONFIG_TEMPLATE}
  ${LIBSIGMF_CMAKE_PROJECT_CONFIG_FILE}
  @ONLY
)

########################################################################
# Install SigMF headers
########################################################################
install(  # install flatbuf proto defs
  DIRECTORY ${LIBSIGMF_GEN_HEADERS}/
  DESTINATION include/sigmf/fbs
  FILES_MATCHING PATTERN "*.fbs)")
install(  # install generated headers
  DIRECTORY ${LIBSIGMF_GEN_HEADERS}/
  DESTINATION include/sigmf
  FILES_MATCHING PATTERN "*.h")
install(  # install original headers
  DIRECTORY ${CMAKE_SOURCE_DIR}/src/
  DESTINATION include/sigmf
  FILES_MATCHING PATTERN "*.h")

########################################################################
# Install cmake configuration foo
########################################################################
install(
  FILES ${LIBSIGMF_CMAKE_PROJECT_CONFIG_FILE}
  DESTINATION ${LIBSIGMF_CONFIG_INSTALL_DIR}
)
install(TARGETS libsigmf
  EXPORT ${LIBSIGMF_TARGETS_EXPORT_NAME}
)
install(EXPORT ${LIBSIGMF_TARGETS_EXPORT_NAME}
  FILE ${LIBSIGMF_TARGETS_EXPORT_NAME}.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION ${LIBSIGMF_CONFIG_INSTALL_DIR}
  )
