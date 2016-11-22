set(target traceraptor)

set(sources
  main.cpp
  Image.cpp
)

list(APPEND CUSTOM_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}")

# Build executable
add_executable(${target}
  ${sources}
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
