cmake_minimum_required(VERSION 3.1)
project(orbit_test)

set(CMAKE_CXX_STANDARD 11)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Threads REQUIRED)

add_executable(orbit_test
	main.cxx
)
target_compile_definitions(orbit_test PRIVATE
        GL_GLEXT_PROTOTYPES
)
target_compile_options(orbit_test PRIVATE
        -Wall -Wextra
)
target_include_directories(orbit_test PUBLIC
        ${OPENGL_INCLUDE_DIR}
        ${SDL2_INCLUDE_DIRS}
        ${GLUT_INCLUDE_DIR}
)
target_link_libraries(orbit_test
        ${CMAKE_THREAD_LIBS_INIT}
        ${OPENGL_LIBRARIES}
        ${SDL2_LIBRARIES}
        ${GLUT_LIBRARIES}
)
