# Function to add a demo project
function(add_spot_example NAME SOURCE_FILE)
	add_executable(${NAME} ${SOURCE_FILE})

	target_link_libraries(${NAME} PRIVATE SPOT::SPOT)

	set(GLOBAL_ASSET_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")

	if(IS_DIRECTORY "${GLOBAL_ASSET_DIR}")
		add_custom_command(TARGET ${NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			"${GLOBAL_ASSET_DIR}"
			"$<TARGET_FILE_DIR:${NAME}>/assets"
			COMMENT "Copying shared assets to ${NAME} output directory..."
		)
	endif()

	if(WIN32)
		add_custom_command(TARGET ${NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				$<TARGET_RUNTIME_DLLS:${NAME}>
				$<TARGET_FILE_DIR:${NAME}>
			COMMAND_EXPAND_LISTS
		)
	endif()

	if(APPLE)
		set_target_properties(${NAME} PROPERTIES 
			MACOSX_BUNDLE TRUE
			MACOSX_BUNDLE_BUNDLE_NAME "${NAME}"
		)
	endif()
endfunction()

add_spot_example(demo-tennis "tennis/tennis.cpp")
