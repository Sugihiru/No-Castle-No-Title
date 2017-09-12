cmake_minimum_required(VERSION 3.0)
project(no_castle_no_title)

# Sources
file(GLOB SRC src/*.cpp)
add_executable(no_castle_no_title ${SRC})

# Includes
set(INCLUDE_DIR
    include)
target_include_directories(no_castle_no_title PUBLIC ${INCLUDE_DIR})

# External libs
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2.4.2 REQUIRED network audio graphics window system)
if (SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
	target_link_libraries(no_castle_no_title ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
else ()
    message(FATAL_ERROR "SFML not found")
endif ()


# Detect and add SFML
if(SFML_FOUND)
endif()

# Force C++11
target_compile_features(no_castle_no_title PUBLIC cxx_nullptr)

# Copy ressource directory
file(COPY ressource DESTINATION ${CMAKE_BINARY_DIR})
