
aux_source_directory(./ test_source)
add_executable(test ${test_source})
target_include_directories(test PRIVATE ${RolUI_INCLUDE_DIR})
target_link_libraries(test RolUI-static)
target_compile_options(test PRIVATE -std=c++17)
