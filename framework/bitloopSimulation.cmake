# --- at the top of your cmake module ---


# --- bitloopSimulation.cmake ---

set(BITLOOP_MAIN_SOURCE		"${CMAKE_CURRENT_LIST_DIR}/src/bitloop_main.cpp"	CACHE INTERNAL "")
set(BITLOOP_BUILD_DIR		"${CMAKE_CURRENT_BINARY_DIR}"						CACHE INTERNAL "")
set(AUTOGEN_SIM_INCLUDES	"${BITLOOP_BUILD_DIR}/bitloop_simulations.h"		CACHE INTERNAL "")

set(BITLOOP_DATA_DEPENDENCIES "" CACHE INTERNAL "Ordered list of dependency data directories")

set_property(DIRECTORY PROPERTY BITLOOP_DEPENDENCY_DIRS "")

# Begin auto-generated header file
file(WRITE "${AUTOGEN_SIM_INCLUDES}" "// Auto‑generated list of simulations\n")

function(apply_common_emscripten_settings _TARGET)
	# O3 for WASM
	target_compile_options(${_TARGET} PRIVATE 
		"-O3"
		"-sUSE_PTHREADS=1"
		"-pthread"
		"-matomics"
		"-mbulk-memory"
	)

	target_link_options(${_TARGET} PRIVATE
		"-sUSE_SDL=3"
		"-sUSE_WEBGL2=1"
		"-sFULL_ES3=1"
		"-sALLOW_MEMORY_GROWTH=1"
		"-sUSE_PTHREADS=1"
		"-sPTHREAD_POOL_SIZE=16"
	)
endfunction()

function(apply_main_emscripten_settings _TARGET)
	target_link_options(${_TARGET} PRIVATE
		"--shell-file=${CMAKE_SOURCE_DIR}/static/bitloop.html"
		"--embed-file=${CMAKE_BINARY_DIR}/data@/data"
	)

	set_target_properties(${_TARGET} PROPERTIES
		SUFFIX ".html"
	)
endfunction()

macro(bitloop_new_project sim_name)
	# collect the other args as source files
	set(SIM_SOURCES ${ARGN})

	if (NOT BL_ROOT_PROJECT)
		set(BL_ROOT_PROJECT ${CMAKE_CURRENT_SOURCE_DIR})
	endif()
	
	#message(STATUS "${sim_name} - CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}   CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}    BL_ROOT_PROJECT: ${BL_ROOT_PROJECT}")

	#if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	if (CMAKE_CURRENT_SOURCE_DIR STREQUAL BL_ROOT_PROJECT)
		# top-level (executable) 
		set(_TARGET ${sim_name})

		if(NOT SIM_SOURCES)
			set(SIM_SOURCES ${BITLOOP_MAIN_SOURCE}) # no user sources (e.g. superbuild), just use the framework main()
			set(SIM_SOURCES_PROVIDED FALSE)
		else()
			list(APPEND SIM_SOURCES ${BITLOOP_MAIN_SOURCE}) # sources provided by user, so append the framework main()
			set(SIM_SOURCES_PROVIDED TRUE)
		endif()

		#message(STATUS "Configuring ${sim_name} as ROOT PROJECT")


		add_executable(${_TARGET} ${SIM_SOURCES})

		#if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
		#	add_custom_target(run_server ALL
		#		COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target ${_TARGET}.html
		#		COMMAND emrun --no_browser --port 8000 ${CMAKE_BINARY_DIR}/${_TARGET}.html
		#		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		#		COMMENT "Building and serving ${_TARGET}.html on http://localhost:8000"
		#		USES_TERMINAL
		#	  )
		#endif()
	else()
		# nested (library)
		set(_TARGET ${sim_name}_lib)
		set(SIM_SOURCES_PROVIDED TRUE)

		#message(STATUS "Configuring ${sim_name} as CHILD")

		#if(NOT SIM_SOURCES)
		#	set(SIM_SOURCES_PROVIDED FALSE)
		#else()
		#	set(SIM_SOURCES_PROVIDED TRUE)
		#endif()

		add_library(${_TARGET} ${SIM_SOURCES})
		add_library(${sim_name}::${sim_name} ALIAS ${sim_name}_lib) # Export an alias so consumers can link as sim::sim
	endif()

	target_include_directories(${_TARGET} PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${BITLOOP_BUILD_DIR}>"
    )
	target_link_libraries(${_TARGET} PRIVATE bitloop::bitloop)

	# Set target for dependencies to link against
	#set(CONSUMER_TARGET ${_TARGET} PARENT_SCOPE)

	# Append #include to the auto-generated header
	if (SIM_SOURCES_PROVIDED)
		file(APPEND "${AUTOGEN_SIM_INCLUDES}" "#include \"${sim_name}.h\"\n")
	ENDIF()

	#if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	if (CMAKE_CURRENT_SOURCE_DIR STREQUAL BL_ROOT_PROJECT)
		msg(STATUS "")
		msg(STATUS "────────── Project Tree ──────────")
		msg(STATUS "[${sim_name}]")
	endif()

	msg_indent_push()

	get_property(_list DIRECTORY ${CMAKE_CURRENT_LIST_DIR} PROPERTY BITLOOP_DEPENDENCY_DIRS)
	foreach(dep_path IN LISTS _list)
		get_filename_component(dep_name ${dep_path} NAME)
		msg(STATUS "[${dep_name}]")
		_bitloop_add_dependency(${_TARGET} ${dep_path})
	endforeach()

	msg_indent_pop()

	# Add target /data (if provided)
	get_property(_data_dirs GLOBAL PROPERTY BITLOOP_DATA_DEPENDENCIES)
	if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/data")
		list(APPEND _data_dirs "${CMAKE_CURRENT_SOURCE_DIR}/data")
	endif()
	set_property(GLOBAL PROPERTY BITLOOP_DATA_DEPENDENCIES ${_data_dirs})

	if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
		apply_common_emscripten_settings(${_TARGET})
	endif()

	#if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	if (CMAKE_CURRENT_SOURCE_DIR STREQUAL BL_ROOT_PROJECT)
		msg(STATUS "")
		msg(STATUS "────────── Data Tree ──────────")
		get_property(_data_dirs GLOBAL PROPERTY BITLOOP_DATA_DEPENDENCIES)

		get_filename_component(BITLOOP_PARENT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" DIRECTORY)

		foreach(dep_path IN LISTS _data_dirs)
			file(RELATIVE_PATH rel_src "${BITLOOP_PARENT_DIR}" "${dep_path}")
			file(RELATIVE_PATH rel_dest "${BITLOOP_PARENT_DIR}" "${CMAKE_CURRENT_BINARY_DIR}")

			msg(STATUS "${rel_src}  ==> ${rel_dest}/data")

			add_custom_command(TARGET ${_TARGET} PRE_BUILD 
				COMMAND ${CMAKE_COMMAND} -E copy_directory 
					"${dep_path}" 
					"${CMAKE_CURRENT_BINARY_DIR}/data"
				COMMENT "Merging dependency data from ${dep_path}"
			)
		endforeach()
		msg(STATUS "")


		if (MSVC)
			# Windows
			set_target_properties(${_TARGET} PROPERTIES WIN32_EXECUTABLE TRUE)
		elseif (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
			# Emscripten
			apply_main_emscripten_settings(${_TARGET})
		endif()
	endif()

endmacro()

# Queues dependency path to be added when bitloop_new_project gets called
macro(bitloop_add_dependency DEP_PATH)
	get_filename_component(_SIM_DIR "${DEP_PATH}" REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
	get_property(_list DIRECTORY ${CMAKE_CURRENT_LIST_DIR} PROPERTY BITLOOP_DEPENDENCY_DIRS)
	list(APPEND _list ${_SIM_DIR})
	set_property(DIRECTORY ${CMAKE_CURRENT_LIST_DIR} PROPERTY BITLOOP_DEPENDENCY_DIRS "${_list}")
endmacro()

# Actually includes/links the dependency into the target (internal)
function(_bitloop_add_dependency _TARGET _SIM_DIR)
	# Grab simulation name from path
	get_filename_component(sim_name "${_SIM_DIR}" NAME)
	#message(STATUS "Grabbing name: ${sim_name}")

	if(NOT TARGET ${sim_name}::${sim_name})
		# Only called once, includes dependency project, which in turn calls:  bitloop_add_simulation()
		add_subdirectory(${_SIM_DIR} "${CMAKE_BINARY_DIR}/${sim_name}_build")
	endif()

	target_include_directories(${_TARGET} PRIVATE
        "$<BUILD_INTERFACE:${_SIM_DIR}>"
        "$<BUILD_INTERFACE:${BITLOOP_BUILD_DIR}>"
    )

	# Link dependency into our target
	target_link_libraries(${_TARGET} PUBLIC ${sim_name}::${sim_name})
endfunction()


# --- msg() helper with indentation push/pop support ---
set_property(GLOBAL PROPERTY MSG_INDENT_LEVEL 0)
function(msg_indent_push)
  get_property(_lvl GLOBAL PROPERTY MSG_INDENT_LEVEL)
  math(EXPR _lvl "${_lvl} + 1")
  set_property(GLOBAL PROPERTY MSG_INDENT_LEVEL "${_lvl}")
endfunction()
function(msg_indent_pop)
  get_property(_lvl GLOBAL PROPERTY MSG_INDENT_LEVEL)
  math(EXPR _lvl "${_lvl} - 1")
  if(_lvl LESS 0)  
    set(_lvl 0)
  endif()
  set_property(GLOBAL PROPERTY MSG_INDENT_LEVEL "${_lvl}")
endfunction()
function(msg)
  set(_status ${ARGV0})
  list(LENGTH ARGV _len)
  math(EXPR _count "${_len} - 1")
  if(_count GREATER 0)
    list(SUBLIST ARGV 1 ${_count} _parts)
    string(JOIN " " _text ${_parts})
  else()
    set(_text "")
  endif()
  get_property(_lvl GLOBAL PROPERTY MSG_INDENT_LEVEL)
  if(_lvl LESS 1)
    set(_indent "")
  else()
    set(_indent "")
    foreach(_i RANGE 1 ${_lvl})
      set(_indent "${_indent}   ")
    endforeach()
  endif()
  message(${_status} "${_indent}${_text}")
endfunction()

