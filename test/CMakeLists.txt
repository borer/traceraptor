include_directories(${CUSTOM_INCLUDE_DIRECTORIES})

file(GLOB TEST_SOURCES *.cpp)
file(GLOB TRACERAPTOR_SOURCES ../src/*.cpp)

#set enable testing
enable_testing()

#adding tests foreach test source file
foreach(filename ${TEST_SOURCES})
	
	#get the file name of the source file
	if(WIN32) 
		string(REGEX REPLACE ".*\\" "" testname "${filename}" )
	else(WIN32)
		string(REGEX REPLACE ".*/" "" testname "${filename}" )
	endif(WIN32)

	#remove .cpp extension
	string(REGEX REPLACE ".cpp" "" testname "${testname}" )

	#create executable
	add_executable(${testname} ${OBJECTS} ${TRACERAPTOR_SOURCES} ${filename})

    set_target_properties(${testname}
        PROPERTIES
        ${DEFAULT_PROJECT_OPTIONS}
    )
    
    target_compile_definitions(${testname}
        PRIVATE
        ${DEFAULT_COMPILE_DEFINITIONS}
    )
    
    target_compile_options(${testname}
        PRIVATE
        ${DEFAULT_COMPILE_OPTIONS}
    )
	
	#configure for testing
	add_test(${testname} ${testname})

	message("${testname} ${filename}")
endforeach(filename)