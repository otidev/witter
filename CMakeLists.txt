cmake_minimum_required(VERSION 3.7)

project(Witter)


# Windows specific

set(SDL2_DIR CACHE STRING "SDL2 source directory (in here you should find lib, include, bin)")
set(SDL2_TTF_DIR CACHE STRING "SDL2 source directory (in here you should find lib, include, bin)")
set(ENET_DIR CACHE STRING "SDL2 source directory (in here you should find enet.lib)")

set(SDL2_INCLUDE_DIRS "${SDL2_DIR}/include/SDL2")
set(SDL2_LIBRARIES "${SDL2_DIR}/lib/libSDL2.dll.a;${SDL2_DIR}/lib/libSDL2main.a")
set(SDL2_TTF_INCLUDE_DIRS "${SDL2_TTF_DIR}/include/SDL2")
set(SDL2_TTF_LIBRARIES "${SDL2_TTF_DIR}/lib/libSDL2_ttf.dll.a")
set(ENET_INCLUDE_DIRS "${ENET_DIR}/include/")
set(ENET_LIBRARIES "${ENET_DIR}/enet.lib")

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_TTF_INCLUDE_DIRS})
include_directories(${ENET_INCLUDE_DIRS})
include_directories(src/include)

add_executable(
	Witter
	src/Font.c
	src/Program.c
	src/Window.c
	src/ChatClient.c
	src/ChatServer.c
)
target_compile_options(Witter PRIVATE -Wall -std=c99)
target_link_libraries(Witter ${SDL2_LIBRARIES} ${ENET_LIBRARIES} -lwinmm -lws2_32 ${SDL2_TTF_LIBRARIES})

add_executable(
	WitterServ
	src/Font.c
	src/ServerProgram.c
	src/Window.c
	src/ChatClient.c
	src/ChatServer.c
)
target_compile_options(WitterServ PRIVATE -Wall -std=c99)
target_link_libraries(WitterServ ${SDL2_LIBRARIES} ${ENET_LIBRARIES} -lwinmm -lws2_32 ${SDL2_TTF_LIBRARIES})

add_custom_command(TARGET Witter POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${SDL2_TTF_DIR}/bin/SDL2_ttf.dll"
	$<TARGET_FILE_DIR:Witter>)

add_custom_command(TARGET Witter POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"${SDL2_DIR}/bin/SDL2.dll"
	$<TARGET_FILE_DIR:Witter>)
	
file(COPY ${CMAKE_CURRENT_LIST_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
