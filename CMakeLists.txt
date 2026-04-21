cmake_minimum_required(VERSION 3.31)

project(SPOT VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/fetch_SDL3.cmake)
include(cmake/fetch_linalg.cmake)

set(SPOT_HEADERS
	"spot.hpp"

	"colour.hpp"
	"graphics.hpp"
	"graphics_context.hpp"
	"maths.hpp"
	"sdl_utils.hpp"
	"spritesheet.hpp"
	"texture.hpp"
)

list(TRANSFORM SPOT_HEADERS PREPEND "include/spot/")

set(SPOT_SOURCES
	"spot.cpp"

	"colour.cpp"
	"graphics_context.cpp"
	"sdl_utils.cpp"
	"spritesheet.cpp"
	"texture.cpp"
)

list(TRANSFORM SPOT_SOURCES PREPEND "${SPOT_SOURCE_DIR}/src/")

add_library(SPOT STATIC)

target_sources(SPOT
	PRIVATE
		${SPOT_SOURCES}
	PUBLIC
		FILE_SET HEADERS
		BASE_DIRS "include"
		FILES ${SPOT_HEADERS}
)

# Define the alias for external use
add_library(SPOT::SPOT ALIAS SPOT)

target_link_libraries(SPOT PUBLIC
	SDL3::SDL3
	SDL3_image::SDL3_image-shared
    linalg::linalg
)
