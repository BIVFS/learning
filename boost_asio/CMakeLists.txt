project(boost_asio_exercise)

set(SOURCE_SYNC_IO sync_main.cpp)
set(SOURCE_ASYNC_IO async_main.cpp)
add_executable(sync_main ${SOURCE_SYNC_IO})
add_executable(async_main ${SOURCE_ASYNC_IO})

target_compile_options(sync_main PRIVATE -Werror -Wall -Wextra)
target_compile_options(async_main PRIVATE -Werror -Wall -Wextra)

target_link_libraries(sync_main boost_thread boost_chrono)
target_link_libraries(async_main boost_thread boost_chrono)
