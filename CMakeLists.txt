cmake_minimum_required (VERSION 3.8)

if(POLICY CMP0091)
	cmake_policy(SET CMP0091 NEW) 
endif (POLICY CMP0091)

project(serial_monitor)


set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILE
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

add_executable(serial_monitor ${SOURCE_FILE})
target_link_libraries(serial_monitor Setupapi Comctl32)

set_target_properties(serial_monitor PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

if(MSVC)
    target_compile_options(serial_monitor PRIVATE "/utf-8")
    target_compile_definitions(serial_monitor PRIVATE "_UNICODE" "UNICODE")
endif()

