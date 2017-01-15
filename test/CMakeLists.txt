include_directories(. ../src)
include_directories(${CUSTOM_INCLUDE_DIRECTORIES})

file(GLOB TEST_SOURCES *.c)

#set enable testing
enable_testing()
#add_definitions("${DEFAULT_COMPILE_OPTIONS}")

#adding tests foreach test source file
foreach(filename ${TEST_SOURCES})
	
	#get the file name of the source file
	if(WIN32) 
		string(REGEX REPLACE ".*\\" "" testname "${filename}" )
	else(WIN32)
		string(REGEX REPLACE ".*/" "" testname "${filename}" )
	endif(WIN32)

	#remove .c extension
	string(REGEX REPLACE ".c" "" testname "${testname}" )

	#create executable
	add_executable(${testname} ${OBJECTS} ${filename})
	#configure for testing
	add_test(${testname} ${testname})

	message("${testname} ${filename}")
endforeach(filename)