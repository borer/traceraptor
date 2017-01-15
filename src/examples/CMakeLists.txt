set(executable_name example)

set(sources
  main.cpp
)

# Build executable
add_executable(${executable_name} ${sources})

target_link_libraries(${executable_name} ${target})

message("genarating ${binary_name} with library ${target}")

target_include_directories(${executable_name}
    PRIVATE
    ${CUSTOM_INCLUDE_DIRECTORIES}
)

set_target_properties(${executable_name}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
)

target_compile_definitions(${executable_name}
    PRIVATE
    ${DEFAULT_COMPILE_DEFINITIONS}
)

target_compile_options(${executable_name}
    PRIVATE
    ${DEFAULT_COMPILE_OPTIONS}
)
