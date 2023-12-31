cmake_minimum_required(VERSION 3.2)

include(ExternalProject)

project(pv112)

if(UNIX)
    find_package(OpenGL REQUIRED)
    find_package(X11 REQUIRED)
    find_package(Threads REQUIRED)
endif()

# GLFW library
set(GLFW_INSTALL_DIR "${CMAKE_BINARY_DIR}/glfw")
set(GLFW_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw/include")
set(GLFW_LIBRARIES   "${GLFW_INSTALL_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}glfw3${CMAKE_STATIC_LIBRARY_SUFFIX}")
ExternalProject_Add(glfw
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw"

    DOWNLOAD_COMMAND ""
    UPDATE_COMMAND ""

    CMAKE_ARGS
        "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/glfw"
        "-DGLFW_BUILD_EXAMPLES=OFF"
        "-DGLFW_BUILD_TESTS=OFF"
        "-DGLFW_BUILD_DOCS=OFF"
        "-DBUILD_SHARED_LIBS=OFF"

    BUILD_BYPRODUCTS ${GLFW_LIBRARIES}
)

if(UNIX)
    set(GLFW_LIBRARIES ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${X11_LIBRARIES} ${X11_Xrandr_LIB} ${X11_Xcursor_LIB} ${X11_Xinerama_LIB} ${X11_Xxf86vm_LIB} ${CMAKE_THREAD_LIBS_INIT})
endif()

# GLAD library
set(GLAD_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include")
add_library(glad "${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/src/glad.c")
target_include_directories(glad PRIVATE "${GLAD_INCLUDE_DIR}")
set(GLAD_LIBARIES)
if(UNIX)
    set(GLAD_LIBRARIES "glad" ${CMAKE_DL_LIBS})
else()
    set(GLAD_LIBRARIES glad)
endif()

# Single header libraries
set(SINGLE_HEADER_LIBS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/single_header")

# Configure compiler flags for our project
if (CMAKE_CXX_COMPILER_ID MATCHES Clang OR ${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  # using regular Clang or AppleClang or GCC
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
  # using Visual Studio C++
  set(CMAKE_CXX_FLAGS "/std:c++17 /permissive- /W3 /EHsc")
endif()

# PV112 Framework
set(FRAMEWORK_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/framework/include")
set(FRAMEWORK_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/framework/src")
add_library(framework
	"${FRAMEWORK_INCLUDE_DIR}/utilities.hpp"
	"${FRAMEWORK_SRC_DIR}/utilities.cpp"
	"${FRAMEWORK_INCLUDE_DIR}/program.hpp"
	"${FRAMEWORK_SRC_DIR}/program.cpp"
	"${FRAMEWORK_INCLUDE_DIR}/mesh.hpp"
	"${FRAMEWORK_SRC_DIR}/mesh.cpp"
	"${FRAMEWORK_INCLUDE_DIR}/rotate_camera.hpp"
	"${FRAMEWORK_SRC_DIR}/rotate_camera.cpp"
		"${FRAMEWORK_INCLUDE_DIR}/camera.hpp"
	"${FRAMEWORK_SRC_DIR}/camera.cpp"
	"${FRAMEWORK_INCLUDE_DIR}/texture.hpp"
	"${FRAMEWORK_SRC_DIR}/texture.cpp"
	
)
add_dependencies(framework glfw glad)
target_link_libraries(framework 
	${GLFW_LIBRARIES}
	${GLAD_LIBRARIES}
)
target_include_directories(framework 
	PRIVATE ${GLFW_INCLUDE_DIR}
	PRIVATE ${GLAD_INCLUDE_DIR}
	PRIVATE ${FRAMEWORK_INCLUDE_DIR}
	PRIVATE ${SINGLE_HEADER_LIBS_INCLUDE_DIR}
	PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/objects"
)

# YOUR PROJECT
add_executable(final
	src/main.cpp 
	src/application.cpp
	src/audio.cpp
)

add_dependencies(final 
	glfw 
	glad 
	framework
)

target_link_libraries(final 
	${GLFW_LIBRARIES} 
	${GLAD_LIBRARIES} 
	framework
)

target_include_directories(final 
	PRIVATE ${GLFW_INCLUDE_DIR}
	PRIVATE ${GLAD_INCLUDE_DIR}
	PRIVATE ${SINGLE_HEADER_LIBS_INCLUDE_DIR}
	PRIVATE ${FRAMEWORK_INCLUDE_DIR}
)

# You can use configure_file to copy all the files needed to run the application into the build folder.
# That way you don't have to configure the IDE to run the program from inside the source folder.
# configure_file("${CMAKE_CURRENT_SOURCE_DIR}/FILL_IN" "${CMAKE_CURRENT_BINARY_DIR}/FILL_IN" COPYONLY)


set(SHADERS ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
set(SHADERS_BINARY ${CMAKE_CURRENT_BINARY_DIR}/shaders)
configure_file(${SHADERS}/draw_lights.vert ${SHADERS_BINARY}/draw_lights.vert COPYONLY)
configure_file(${SHADERS}/draw_lights.frag ${SHADERS_BINARY}/draw_lights.frag COPYONLY)
configure_file(${SHADERS}/draw_object.vert ${SHADERS_BINARY}/draw_object.vert COPYONLY)
configure_file(${SHADERS}/draw_object.frag ${SHADERS_BINARY}/draw_object.frag COPYONLY)
configure_file(${SHADERS}/draw_teapots.vert ${SHADERS_BINARY}/draw_teapots.vert COPYONLY)
configure_file(${SHADERS}/draw_teapots.frag ${SHADERS_BINARY}/draw_teapots.frag COPYONLY)

configure_file(${SHADERS}/draw_object_normal_textured.vert ${SHADERS_BINARY}/draw_object_normal_textured.vert COPYONLY)
configure_file(${SHADERS}/draw_object_normal_textured.frag ${SHADERS_BINARY}/draw_object_normal_textured.frag COPYONLY)
configure_file(${SHADERS}/draw_object_textured.vert ${SHADERS_BINARY}/draw_object_textured.vert COPYONLY)
configure_file(${SHADERS}/draw_object_textured.frag ${SHADERS_BINARY}/draw_object_textured.frag COPYONLY)
configure_file(${SHADERS}/postprocess.vert ${SHADERS_BINARY}/postprocess.vert COPYONLY)
configure_file(${SHADERS}/postprocess.frag ${SHADERS_BINARY}/postprocess.frag COPYONLY)

set(IMAGES ${CMAKE_CURRENT_SOURCE_DIR}/images)
set(IMAGES_BINARY ${CMAKE_CURRENT_BINARY_DIR}/images)
configure_file(${IMAGES}/black.png ${IMAGES_BINARY}/black.png COPYONLY)
configure_file(${IMAGES}/floor_normal_map.jpg ${IMAGES_BINARY}/floor_normal_map.jpg COPYONLY)
configure_file(${IMAGES}/floor.jpg ${IMAGES_BINARY}/floor.jpg COPYONLY)
configure_file(${IMAGES}/wall_normal_map.jpg ${IMAGES_BINARY}/wall_normal_map.jpg COPYONLY)
configure_file(${IMAGES}/wall.jpg ${IMAGES_BINARY}/wall.jpg COPYONLY)
configure_file(${IMAGES}/el.jpg ${IMAGES_BINARY}/el.jpg COPYONLY)
configure_file(${IMAGES}/el_normal_map.jpg ${IMAGES_BINARY}/el_normal_map.jpg COPYONLY)
configure_file(${IMAGES}/cover.jpg ${IMAGES_BINARY}/cover.jpg COPYONLY)

set(OBJECTS ${CMAKE_CURRENT_SOURCE_DIR}/objects)
set(OBJECTS_BINARY ${CMAKE_CURRENT_BINARY_DIR}/objects)
configure_file(${OBJECTS}/clock.obj ${OBJECTS_BINARY}/clock.obj COPYONLY)
configure_file(${OBJECTS}/clock.mtl ${OBJECTS_BINARY}/clock.mtl COPYONLY)
configure_file(${OBJECTS}/cube_man.obj ${OBJECTS_BINARY}/cube_man.obj COPYONLY)
configure_file(${OBJECTS}/cube_man.mtl ${OBJECTS_BINARY}/cube_man.mtl COPYONLY)
configure_file(${OBJECTS}/Zeleznak.jpg ${OBJECTS_BINARY}/Zeleznak.jpg COPYONLY)
configure_file(${OBJECTS}/mdas.obj ${OBJECTS_BINARY}/mdas.obj COPYONLY)
configure_file(${OBJECTS}/beer.obj ${OBJECTS_BINARY}/beer.obj COPYONLY)
configure_file(${OBJECTS}/beer.jpeg ${OBJECTS_BINARY}/beer.jpeg COPYONLY)
configure_file(${OBJECTS}/ZeleznakNormalsMap.jpg ${OBJECTS_BINARY}/ZeleznakNormalsMap.jpg COPYONLY)
configure_file(${OBJECTS}/wall.obj ${OBJECTS_BINARY}/wall.obj COPYONLY)
configure_file(${OBJECTS}/car.obj ${OBJECTS_BINARY}/car.obj COPYONLY)
configure_file(${OBJECTS}/electro.obj ${OBJECTS_BINARY}/electro.obj COPYONLY)


set(MUSIC ${CMAKE_CURRENT_SOURCE_DIR}/music)
set(MUSIC_BINARY ${CMAKE_CURRENT_BINARY_DIR}/music)
configure_file(${MUSIC}/Kahoot.mp3 ${MUSIC_BINARY}/Kahoot.mp3 COPYONLY)
configure_file(${MUSIC}/kahoot_notes.txt ${MUSIC_BINARY}/kahoot_notes.txt COPYONLY)
