find_program(GLSLANGVALIDATOR_EXECUTABLE NAMES glslangValidator glslangValidator.exe DOC "Path to glslangValidator executable.")
mark_as_advanced(GLSLANGVALIDATOR_EXECUTABLE)
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GLSLANGVALIDATOR REQUIRED_VARS GLSLANGVALIDATOR_EXECUTABLE)
if(GLSLANGVALIDATOR_FOUND)
	function(glslangvalidator_generate_cpp SRCS HDRS)
		set(${SRCS})
		set(${HDRS})
		foreach(FIL ${ARGN})
			get_filename_component(FIL_ABS ${FIL} REALPATH)
			get_filename_component(FIL_WE ${FIL} NAME_WE)
			get_filename_component(FIL_EXT ${FIL} EXT)
			string(REGEX REPLACE "\\." "" FIL_EXT ${FIL_EXT})
			set(HDR "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}_${FIL_EXT}.h")
			set(SRC "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}_${FIL_EXT}.cpp")
			add_custom_command(
				OUTPUT ${HDR} ${SRC}
				COMMAND "C:\\Program Files\\CMake\\bin\\cmake.exe"
					"-DFIL:FILEPATH=${FIL_ABS}"
					"-DGLSLANGVALIDATOR_EXECUTABLE:FILEPATH=${GLSLANGVALIDATOR_EXECUTABLE}"
					"-DOUTPUT_DIR:PATH=${CMAKE_CURRENT_BINARY_DIR}"
					"-P"
					"${CMAKE_CURRENT_SOURCE_DIR}/cmake/GLSLANGValidator_generator.cmake"
				DEPENDS ${FIL_ABS} ${GLSLANGVALIDATOR_EXECUTABLE}
			)
			list(APPEND ${SRCS} ${SRC})
			list(APPEND ${HDRS} ${HDR})
		endforeach()
		set_source_files_properties(${${SRCS}} ${${HDRS}} PROPERTIES GENERATED TRUE)
		set(${SRCS} ${${SRCS}} PARENT_SCOPE)
		set(${HDRS} ${${HDRS}} PARENT_SCOPE)
	endfunction()
endif()
