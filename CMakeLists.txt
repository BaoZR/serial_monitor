cmake_minimum_required (VERSION 3.8)

if(POLICY CMP0091)
	cmake_policy(SET CMP0091 NEW) 
endif (POLICY CMP0091)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)

project(serial_monitor_demo)
set(SOURCE_FILE_DEMO
${CMAKE_CURRENT_SOURCE_DIR}/source/simple_notify_window.cc
${CMAKE_CURRENT_SOURCE_DIR}/source/simple_notify_window.h
${CMAKE_CURRENT_SOURCE_DIR}/source/display_window.h
${CMAKE_CURRENT_SOURCE_DIR}/source/display_window.cc
${CMAKE_CURRENT_SOURCE_DIR}/source/device_info.cc
${CMAKE_CURRENT_SOURCE_DIR}/source/device_info.h
${CMAKE_CURRENT_SOURCE_DIR}/source/device_changed.h
${CMAKE_CURRENT_SOURCE_DIR}/source/enum_device.h
${CMAKE_CURRENT_SOURCE_DIR}/source/enum_device.cc
${CMAKE_CURRENT_SOURCE_DIR}/source/main.cc
${CMAKE_CURRENT_SOURCE_DIR}/source/utils.cc
${CMAKE_CURRENT_SOURCE_DIR}/source/utils.h
${CMAKE_CURRENT_SOURCE_DIR}/source/panel.cc
${CMAKE_CURRENT_SOURCE_DIR}/source/panel.h
)

add_executable(serial_monitor_demo ${SOURCE_FILE_DEMO})
target_link_libraries(serial_monitor_demo Setupapi Comctl32)

set_target_properties(serial_monitor_demo PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

if(MSVC)
    target_compile_options(serial_monitor_demo PRIVATE "/utf-8")
    target_compile_definitions(serial_monitor_demo PRIVATE "_UNICODE" "UNICODE")
endif()

########################################################
project(serial_monitor_lib)

set(SOURCE_FILE_LIB
${CMAKE_CURRENT_SOURCE_DIR}/source/simple_notify_window.cc
${CMAKE_CURRENT_SOURCE_DIR}/source/simple_notify_window.h
${CMAKE_CURRENT_SOURCE_DIR}/source/serial_monitor_lib.cc
${CMAKE_CURRENT_SOURCE_DIR}/source/serial_monitor_lib.h
${CMAKE_CURRENT_SOURCE_DIR}/source/enum_device.cc
${CMAKE_CURRENT_SOURCE_DIR}/source/enum_device.h
${CMAKE_CURRENT_SOURCE_DIR}/source/device_info.cc
${CMAKE_CURRENT_SOURCE_DIR}/source/device_info.h
${CMAKE_CURRENT_SOURCE_DIR}/source/utils.cc
${CMAKE_CURRENT_SOURCE_DIR}/source/utils.h
)

add_library(serial_monitor_lib SHARED ${SOURCE_FILE_LIB})
target_link_libraries(serial_monitor_lib Setupapi)

set_target_properties(serial_monitor_lib PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

if(MSVC)
    target_compile_options(serial_monitor_lib PRIVATE "/utf-8")
    target_compile_definitions(serial_monitor_lib PRIVATE "_UNICODE" "UNICODE")
endif()



###############################

add_subdirectory(test)

