# --- bitloopSimulation.cmake ---

set(BITLOOP_MAIN_SOURCE		"${CMAKE_CURRENT_LIST_DIR}/src/bitloop_main.cpp"	CACHE INTERNAL "")
set(BITLOOP_BUILD_DIR		"${CMAKE_CURRENT_BINARY_DIR}"						CACHE INTERNAL "")
set(AUTOGEN_SIM_INCLUDES	"${BITLOOP_BUILD_DIR}/bitloop_simulations.h"		CACHE INTERNAL "")

# Begin auto-generated header file
file(WRITE "${AUTOGEN_SIM_INCLUDES}" "// Auto‑generated list of simulations\n")


function(bitloop_add_simulation sim_name)
	# collect the other args as source files
	set(SIM_SOURCES ${ARGN})

	if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
		# top-level (executable) 
		set(_TARGET ${sim_name})

		if(NOT SIM_SOURCES)
			set(SIM_SOURCES ${BITLOOP_MAIN_SOURCE}) # no user sources (e.g. superbuild), just use the framework main()
			set(SIM_SOURCES_PROVIDED FALSE)
		else()
			list(APPEND SIM_SOURCES ${BITLOOP_MAIN_SOURCE}) # sources provided by user, so append the framework main()
			set(SIM_SOURCES_PROVIDED TRUE)
		endif()

		add_executable(${_TARGET} ${SIM_SOURCES})
	else()
		# nested (library)
		set(_TARGET ${sim_name}_lib)
		set(SIM_SOURCES_PROVIDED TRUE)

		add_library(${_TARGET} ${SIM_SOURCES})
		add_library(${sim_name}::${sim_name} ALIAS ${sim_name}_lib) # Export an alias so consumers can link as sim::sim
	endif()

	target_include_directories(${_TARGET} PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${BITLOOP_BUILD_DIR}>"
    )
	target_link_libraries(${_TARGET} PRIVATE bitloop::bitloop)

	# Set target for dependencies to link against
	set(CONSUMER_TARGET ${_TARGET} PARENT_SCOPE)

	# Append #include to the auto-generated header
	if (SIM_SOURCES_PROVIDED)
		file(APPEND "${AUTOGEN_SIM_INCLUDES}" "#include \"${sim_name}.h\"\n")
	ENDIF()

	# Copy data files to the build directory
	set(_SRC_DATA "${CMAKE_CURRENT_SOURCE_DIR}/data")
	set(_DST_DATA "${CMAKE_CURRENT_BINARY_DIR}/data/${sim_name}")

	if (EXISTS "${_SRC_DATA}")
		add_custom_command(TARGET ${_TARGET} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E make_directory "${_DST_DATA}"
			COMMAND ${CMAKE_COMMAND} -E copy_directory "${_SRC_DATA}" "${_DST_DATA}"
			COMMENT "Copying data for ${sim_name}"
		)
	endif()
endfunction()

function(bitloop_add_dependency DEP_PATH)
	get_filename_component(_SIM_DIR "${DEP_PATH}" REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

	# Check dependency directory exists
	if(NOT IS_DIRECTORY "${_SIM_DIR}")
		message(FATAL_ERROR "bitloop_add_dep: '${DEP_PATH}' is not a dir")
	endif()

	# Grab simulation name from path
	get_filename_component(sim_name "${_SIM_DIR}" NAME)

	if(NOT TARGET ${sim_name}::${sim_name})
		# Only called once, includes dependency project, which in turn calls:  bitloop_add_simulation()
		add_subdirectory(${_SIM_DIR} "${CMAKE_BINARY_DIR}/${sim_name}_build")
	endif()

	target_include_directories(${CONSUMER_TARGET} PRIVATE
        "$<BUILD_INTERFACE:${_SIM_DIR}>"
        "$<BUILD_INTERFACE:${BITLOOP_BUILD_DIR}>"
    )

	# Link dependency into our target
	target_link_libraries(${CONSUMER_TARGET} PUBLIC ${sim_name}::${sim_name})

	# Also copy that dependency’s data/ into our data/<dep_name>
	set(_DEP_DATA "${_SIM_DIR}/data")
	set(_DST_DATA "${CMAKE_CURRENT_BINARY_DIR}/data/${sim_name}")

	if(EXISTS "${_DEP_DATA}")
		add_custom_command(TARGET ${CONSUMER_TARGET} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E make_directory "${_DST_DATA}"
			COMMAND ${CMAKE_COMMAND} -E copy_directory "${_DEP_DATA}" "${_DST_DATA}"
			COMMENT "Copying data for dependency ${sim_name}"
		)
	endif()
endfunction()
