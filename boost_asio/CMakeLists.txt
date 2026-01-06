project(boost_asio_exercise)

set(SOURCE_EXE main.cpp)
add_executable(main ${SOURCE_EXE})
target_compile_options(main PRIVATE -Werror -Wall -Wextra)
target_link_libraries(main boost_thread)
