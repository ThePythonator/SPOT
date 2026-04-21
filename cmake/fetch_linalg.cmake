include(FetchContent)

# Fetch linalg
FetchContent_Declare(
	linalg
	GIT_REPOSITORY https://github.com/sgorsten/linalg
	GIT_TAG v2.2
)

FetchContent_MakeAvailable(linalg)

if (NOT TARGET linalg::linalg)
	add_library(linalg_interface INTERFACE)
	target_include_directories(linalg_interface SYSTEM INTERFACE "${linalg_SOURCE_DIR}")

	add_library(linalg::linalg ALIAS linalg_interface)
endif()
