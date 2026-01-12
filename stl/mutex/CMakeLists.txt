project(stl_mutex)

set(CPP_SOURCES main.cpp)

add_executable(main ${CPP_SOURCES})

target_compile_options(main PRIVATE -Werror -Wall -Wextra -g -O0)

target_link_libraries(pthread)
