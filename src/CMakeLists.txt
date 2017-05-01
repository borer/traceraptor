set(target traceraptor)

list(APPEND CUSTOM_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(examples)

set(sources
  Image.cpp
  BBox.cpp
  BVH.cpp
  SceneLoader.cpp
)

list(APPEND CUSTOM_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}")

# Build library
message("generating library ${target}")
add_library(${target} STATIC ${sources}
)

target_include_directories(${target}
    PRIVATE
    ${CUSTOM_INCLUDE_DIRECTORIES}
)

set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
)

target_compile_definitions(${target}
    PRIVATE
    ${DEFAULT_COMPILE_DEFINITIONS}
)

target_compile_options(${target}
    PRIVATE
    ${DEFAULT_COMPILE_OPTIONS}
)
