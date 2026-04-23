include(FetchContent)

# --- SDL3 Core Configuration ---
set(SDL_SHARED ON CACHE BOOL "Build SDL shared library" FORCE)
set(SDL_STATIC OFF CACHE BOOL "Build SDL static library" FORCE)
#set(SDL_TESTS OFF CACHE BOOL "Build SDL tests" FORCE)
#set(SDL_INSTALL OFF CACHE BOOL "Disable SDL install rules" FORCE)

# --- SDL3_image Configuration ---
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build SDL_image shared library" FORCE)
# Disable AVIF to avoid the NASM requirement
set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)

# TODO: configuration for SDL3_ttf and SDL3_mixer?

# Fetch SDL3
FetchContent_Declare(
	SDL3
	GIT_REPOSITORY https://github.com/libsdl-org/SDL
	GIT_TAG release-3.4.4
)
FetchContent_MakeAvailable(SDL3)

# Fetch SDL3_image
FetchContent_Declare(
    SDL3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
    GIT_TAG release-3.4.2
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(SDL3_image)

# Fetch SDL3_ttf
FetchContent_Declare(
    SDL3_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf
    GIT_TAG release-3.2.2
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(SDL3_ttf)

# Fetch SDL3_mixer
FetchContent_Declare(
    SDL3_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer
    GIT_TAG release-3.2.0
    OVERRIDE_FIND_PACKAGE
)
#FetchContent_MakeAvailable(SDL3_mixer)
