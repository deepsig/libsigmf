
########################################################################
# Setup project
########################################################################
cmake_minimum_required(VERSION 3.0)
project(libsigmf VERSION 1.0.2 LANGUAGES CXX)

# c++17 used for supporting flatbuffers optional scalar fields via std::optional<T>
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH cmake)
option(ENABLE_EXAMPLES "Enable building of examples" ON)
option(USE_SYSTEM_JSON "Attempt to use the system installed nlohmann JSON library" OFF)
option(USE_SYSTEM_FLATBUFFERS "Attempt to use the system installed flatbuffers library and flatc" OFF)

include(GNUInstallDirs)

set(LIBSIGMF_TARGET_NAME               ${PROJECT_NAME})
set(LIBSIGMF_CONFIG_INSTALL_DIR        "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}" CACHE INTERNAL "")
set(LIBSIGMF_INCLUDE_INSTALL_DIR       "${CMAKE_INSTALL_INCLUDEDIR}")
set(LIBSIGMF_TARGETS_EXPORT_NAME       "${PROJECT_NAME}Targets")
set(LIBSIGMF_CMAKE_CONFIG_TEMPLATE     "cmake/config.cmake.in")
set(LIBSIGMF_CMAKE_CONFIG_DIR          "${CMAKE_CURRENT_BINARY_DIR}")
set(LIBSIGMF_CMAKE_PROJECT_CONFIG_FILE "${LIBSIGMF_CMAKE_CONFIG_DIR}/${PROJECT_NAME}Config.cmake")
set(LIBSIGMF_CMAKE_VERSION_CONFIG_FILE "${LIBSIGMF_CMAKE_CONFIG_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(LIBSIGMF_CMAKE_PROJECT_TARGETS_FILE "${LIBSIGMF_CMAKE_CONFIG_DIR}/${PROJECT_NAME}Targets.cmake")

########################################################################
# Dependency management
#
# Prefer system-installed versions, but revert to using submodules
########################################################################
set(libsigmf_deps "")

# flatbuffers
if (${USE_SYSTEM_FLATBUFFERS})
  find_package(Flatbuffers REQUIRED)
endif (${USE_SYSTEM_FLATBUFFERS})

if (NOT ${USE_SYSTEM_FLATBUFFERS})
  message(STATUS "Flatbuffers was not found. Submodule will be used. This will require building flatc")
  set(FLATBUFFERS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/flatbuffers/include")
  # Initialize Flatbuffers submodule if unititialized
  set(FLATBUFFERS_INCLUDE_DIR 
    ${CMAKE_CURRENT_SOURCE_DIR}/external/flatbuffers/include
    CACHE PATH "flatbuffers include directory")

  if (NOT IS_DIRECTORY ${FLATBUFFERS_INCLUDE_DIR})
      message("Flatbuffers submodule not found! Downloading...")
      execute_process(COMMAND git submodule update --init -- external/flatbuffers
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  endif(NOT IS_DIRECTORY ${FLATBUFFERS_INCLUDE_DIR})

  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/flatbuffers
          ${CMAKE_CURRENT_BINARY_DIR}/flatbuffers-build
          EXCLUDE_FROM_ALL)
endif (NOT ${USE_SYSTEM_FLATBUFFERS})

# nlohmann_json
if (${USE_SYSTEM_JSON})
  find_package(nlohmann_json REQUIRED)
endif (${USE_SYSTEM_JSON})

if (NOT ${USE_SYSTEM_JSON})
  message(STATUS "Nlohmann JSON was not found. Submodule will be used.")
  # Initialize Json submodule if unititialized
  set(JSON_INCLUDE_DIR
	  ${CMAKE_CURRENT_SOURCE_DIR}/external/json/include
	  CACHE PATH "json include directory")

  if (NOT IS_DIRECTORY ${JSON_INCLUDE_DIR})
      message("Json submodule not found! Downloading...")
      execute_process(COMMAND git submodule update --init -- external/json
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  endif(NOT IS_DIRECTORY ${JSON_INCLUDE_DIR})
endif(NOT ${USE_SYSTEM_JSON})

########################################################################
# Use flatbuffers to generate SigMF headers
########################################################################
function(generate_sigmf_header generate_sigmf_target FBS_FILE OUTPUT_LOCATION)
    get_filename_component(generated_output_name ${FBS_FILE} NAME_WE)
    message(STATUS "Flatbuffers generated ${generated_output_name}")
    if( ${ARGC} GREATER 3)
        set(FLATC_EXTRA_ARGS ${ARGN})
        string(REPLACE " " ";" FLATC_EXTRA_ARGS ${FLATC_EXTRA_ARGS})
    else()
        set(FLATC_EXTRA_ARGS "")
    endif()
    add_custom_command(OUTPUT ${OUTPUT_LOCATION}/${generated_output_name}_generated.h
            COMMAND flatc ${FLATC_EXTRA_ARGS} -c --cpp-ptr-type std::shared_ptr --reflect-types --reflect-names --gen-object-api -o "${OUTPUT_LOCATION}/" "${FBS_FILE}"
            COMMENT "Building C++ header for flatbuffers definition ${FBS_FILE} ${ARGC} ${ARGN}"
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
generate_sigmf_header(generated_adsb_ns
  "${CMAKE_CURRENT_SOURCE_DIR}/sigmf_protocols/sigmf_adsb.fbs"
  "${CMAKE_CURRENT_BINARY_DIR}/include"
)
  generate_sigmf_header(generated_antenna_ns
  "${CMAKE_CURRENT_SOURCE_DIR}/sigmf_protocols/sigmf_antenna.fbs"
  "${CMAKE_CURRENT_BINARY_DIR}/include"
)
generate_sigmf_header(generated_capture_details_ns
  "${CMAKE_CURRENT_SOURCE_DIR}/sigmf_protocols/sigmf_capture_details.fbs"
  "${CMAKE_CURRENT_BINARY_DIR}/include"
)
generate_sigmf_header(generated_signal_ns
  "${CMAKE_CURRENT_SOURCE_DIR}/sigmf_protocols/sigmf_signal.fbs"
  "${CMAKE_CURRENT_BINARY_DIR}/include"
)
generate_sigmf_header(generated_spatial_ns
  "${CMAKE_CURRENT_SOURCE_DIR}/sigmf_protocols/sigmf_spatial.fbs"
  "${CMAKE_CURRENT_BINARY_DIR}/include"
)
generate_sigmf_header(generated_wifi_ns
  "${CMAKE_CURRENT_SOURCE_DIR}/sigmf_protocols/sigmf_wifi.fbs"
  "${CMAKE_CURRENT_BINARY_DIR}/include"
)
  generate_sigmf_header(generated_testing_ns
  "${CMAKE_CURRENT_SOURCE_DIR}/sigmf_protocols/testing_protocols.fbs"
  "${CMAKE_CURRENT_BINARY_DIR}/include"
  )

# We also carry around pre-generated headers so downstream doesn't need to build flatc
set(LIBSIGMF_PREGEN_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/sigmf_protocols")

########################################################################
# Our interface target that downstream have to use
########################################################################
add_library(libsigmf INTERFACE)
target_include_directories(libsigmf INTERFACE
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include/sigmf/fbs>
  $<BUILD_INTERFACE:${LIBSIGMF_PREGEN_HEADERS}>
)

if (${Flatbuffers_FOUND})
  # System flatbuffers can import this target
else()
  target_include_directories(libsigmf INTERFACE
    $<BUILD_INTERFACE:${FLATBUFFERS_INCLUDE_DIR}>
  )
endif(${Flatbuffers_FOUND})

if (${nlohmann_json_FOUND})
  # TODO: add target_link_libraries interface to nlohmann json
  # when we use the system version
else()
  target_include_directories(libsigmf INTERFACE
    $<BUILD_INTERFACE:${JSON_INCLUDE_DIR}>
  )
endif(${nlohmann_json_FOUND})


########################################################################
# Ensure that our protocol headers are generated before libsigmf dep
########################################################################
add_dependencies(libsigmf libsigmf_genheaders)
add_custom_target(libsigmf_genheaders DEPENDS
        generate_sigmf_target_generated_core_ns
        # extension namespaces:
        generate_sigmf_target_generated_adsb_ns
        generate_sigmf_target_generated_antenna_ns
        generate_sigmf_target_generated_capture_details_ns
        generate_sigmf_target_generated_spatial_ns
        generate_sigmf_target_generated_signal_ns
        generate_sigmf_target_generated_wifi_ns
        generate_sigmf_target_generated_testing_ns
)

if (ENABLE_EXAMPLES)
  add_subdirectory(examples)
endif (ENABLE_EXAMPLES)

add_custom_target(makedocs
        COMMAND mkdocs build
        COMMENT "Building documentation website"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${LIBSIGMF_CMAKE_VERSION_CONFIG_FILE} COMPATIBILITY SameMajorVersion
)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/${LIBSIGMF_CMAKE_CONFIG_TEMPLATE}
  ${LIBSIGMF_CMAKE_PROJECT_CONFIG_FILE}
  @ONLY
)

########################################################################
# Install SigMF headers
########################################################################
install(  # install flatbuf proto defs
  DIRECTORY ${LIBSIGMF_PREGEN_HEADERS}/
  DESTINATION include/sigmf/fbs
  FILES_MATCHING PATTERN "*.fbs")
install(  # install pregenerated headers
  DIRECTORY ${LIBSIGMF_PREGEN_HEADERS}/
  DESTINATION include/sigmf
  FILES_MATCHING PATTERN "*.h")
install(  # if headers were built, replace pregenerated headers with those
  DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/
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
        ${LIBSIGMF_CMAKE_VERSION_CONFIG_FILE}
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
