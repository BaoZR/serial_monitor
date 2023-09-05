project(serial_monitor_test)

set(SOURCE_FILE_LIB_TEST
${CMAKE_SOURCE_DIR}/source/serial_monitor_lib.h
${CMAKE_SOURCE_DIR}/source/utils.h
${CMAKE_SOURCE_DIR}/test/main_test.cc
)

add_executable(serial_monitor_test ${SOURCE_FILE_LIB_TEST})
target_link_libraries(serial_monitor_test Setupapi serial_monitor_lib)
target_include_directories(serial_monitor_test 
    PUBLIC
${CMAKE_SOURCE_DIR}/source/)

set_target_properties(serial_monitor_test PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

if(MSVC)
    target_compile_options(serial_monitor_test PRIVATE "/utf-8")
    target_compile_definitions(serial_monitor_test PRIVATE "_UNICODE" "UNICODE")
endif()
