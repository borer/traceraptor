list(APPEND CUSTOM_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/integrators")

set(sources
  Image.cpp
  BBox.cpp
  BVH.cpp
  Triangle.cpp
  SceneLoader.cpp
  integrators/Integrator.cpp
)

# Build library
message("generating library ${traceraptorLib}")
add_library(${traceraptorLib} STATIC ${sources}
)

target_include_directories(${traceraptorLib}
    PRIVATE
    ${CUSTOM_INCLUDE_DIRECTORIES}
)

set_target_properties(${traceraptorLib}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
)

target_compile_definitions(${traceraptorLib}
    PRIVATE
    ${DEFAULT_COMPILE_DEFINITIONS}
)

target_compile_options(${traceraptorLib}
    PRIVATE
    ${DEFAULT_COMPILE_OPTIONS}
)
