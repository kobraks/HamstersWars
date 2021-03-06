cmake_minimum_required( VERSION 2.8 )
project( glfw-cxx )

include(${CMAKE_SOURCE_DIR}/cmake/Config.cmake)

set( CMAKE_BUILD_TYPE Release CACHE STRING "Build type." FORCE )
set( GLFW-CXX_BUILD_SHARED_LIBS TRUE CACHE BOOL "Build shared llibraries." )
set( GLFW-CXX_BUILD_DOCS TRUE CACHE BOOL "Build Doxygen documentation." )
set( GLFW-CXX_BUILD_EXAMPLES TRUE CACHE BOOL "Build glfw-cxx examples." )

if(CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/;${CMAKE_MODULE_PATH}")

find_package( GLFW3 REQUIRED )

if(UNIX)
	include_directories(${GLFW_INCLUDE_DIRS})
elseif(GLFW_FOUND)
	include_directories(${GLFW_INCLUDE_DIRS})
else()
	set(GLFW_ROOT "" CACHE PATH "GLFW top-level directory")
	message("\n-> GLFW directory not found. Set GLFW_ROOT to GLFW's top-level path;")
	message("--> Containing \"include\"  and \"lib\" directories.")
endif()

if( GLFW-CXX_BUILD_SHARED_LIBS )
        set( LIB_TYPE SHARED )
else()
        set( LIB_TYPE STATIC )
endif()

set(SRC_DIR "src")
set(INC_DIR "include")
set(TEST_DIR "test")

set(
	SOURCES
	${INC_DIR}/${CMAKE_PROJECT_NAME}/glfw-cxx.hpp
	${INC_DIR}/${CMAKE_PROJECT_NAME}/Monitor.hpp
	${INC_DIR}/${CMAKE_PROJECT_NAME}/Window.hpp
	${INC_DIR}/${CMAKE_PROJECT_NAME}/Time.hpp
	${INC_DIR}/${CMAKE_PROJECT_NAME}/Version.hpp
	${INC_DIR}/${CMAKE_PROJECT_NAME}/Context.hpp
	${INC_DIR}/${CMAKE_PROJECT_NAME}/Error.hpp
	${INC_DIR}/${CMAKE_PROJECT_NAME}/VideoMode.hpp
	${INC_DIR}/${CMAKE_PROJECT_NAME}/Vector2.hpp
	${INC_DIR}/${CMAKE_PROJECT_NAME}/GammaRamp.hpp
	${INC_DIR}/${CMAKE_PROJECT_NAME}/Init.hpp
	${INC_DIR}/${CMAKE_PROJECT_NAME}/Event.hpp
	${SRC_DIR}/${CMAKE_PROJECT_NAME}/Monitor.cpp
	${SRC_DIR}/${CMAKE_PROJECT_NAME}/Time.cpp
	${SRC_DIR}/${CMAKE_PROJECT_NAME}/Window.cpp
	${SRC_DIR}/${CMAKE_PROJECT_NAME}/Version.cpp
	${SRC_DIR}/${CMAKE_PROJECT_NAME}/Context.cpp
	${SRC_DIR}/${CMAKE_PROJECT_NAME}/Error.cpp
	${SRC_DIR}/${CMAKE_PROJECT_NAME}/VideoMode.cpp
	${SRC_DIR}/${CMAKE_PROJECT_NAME}/GammaRamp.cpp
	${SRC_DIR}/${CMAKE_PROJECT_NAME}/Init.cpp
	${SRC_DIR}/${CMAKE_PROJECT_NAME}/WindowCallbacks.cpp
	${SRC_DIR}/${CMAKE_PROJECT_NAME}/EventCallbacks.cpp
)

include_directories( ${INC_DIR} )

add_library( ${CMAKE_PROJECT_NAME} ${LIB_TYPE} ${SOURCES} )

if( GLFW-CXX_BUILD_DOCS )
    configure_file(${CMAKE_SOURCE_DIR}/docs/Doxyfile.in ${CMAKE_SOURCE_DIR}/docs/Doxyfile @ONLY)
    set(DOXYGEN_SKIP_DOT TRUE)
    find_package(Doxygen)
	add_subdirectory( ${CMAKE_SOURCE_DIR}/docs )
endif()


if( GLFW-CXX_BUILD_EXAMPLES )
	include(${CMAKE_SOURCE_DIR}/examples/CMakeLists.txt)
endif()

install(
    DIRECTORY include/${CMAKE_PROJECT_NAME}
    DESTINATION include
    COMPONENT devel
)

install(
    TARGETS ${CMAKE_PROJECT_NAME}
    LIBRARY DESTINATION lib COMPONENT bin
    ARCHIVE DESTINATION lib COMPONENT dev
)
