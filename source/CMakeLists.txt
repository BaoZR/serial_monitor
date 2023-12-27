
project(serial_monitor_demo)
set(SOURCE_FILE_DEMO
${CMAKE_SOURCE_DIR}/source/lib/simple_notify_window.cc
${CMAKE_SOURCE_DIR}/source/lib/simple_notify_window.h
${CMAKE_SOURCE_DIR}/source/lib/device_info.cc
${CMAKE_SOURCE_DIR}/source/lib/device_info.h
${CMAKE_SOURCE_DIR}/source/lib/enum_device.h
${CMAKE_SOURCE_DIR}/source/lib/enum_device.cc
${CMAKE_SOURCE_DIR}/source/lib/serial_monitor_utils.cc
${CMAKE_SOURCE_DIR}/source/lib/serial_monitor_utils.h
${CMAKE_SOURCE_DIR}/source/demo/panel.cc
${CMAKE_SOURCE_DIR}/source/demo/panel.h
${CMAKE_SOURCE_DIR}/source/demo/main.cc
${CMAKE_SOURCE_DIR}/source/demo/display_window.h
${CMAKE_SOURCE_DIR}/source/demo/display_window.cc
)

add_executable(serial_monitor_demo ${SOURCE_FILE_DEMO})
target_link_libraries(serial_monitor_demo Setupapi Comctl32)
target_include_directories(serial_monitor_demo
PRIVATE
${CMAKE_SOURCE_DIR}/source/lib/)

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
${CMAKE_SOURCE_DIR}/source/lib/simple_notify_window.cc
${CMAKE_SOURCE_DIR}/source/lib/simple_notify_window.h
${CMAKE_SOURCE_DIR}/source/lib/serial_monitor_lib.cc
${CMAKE_SOURCE_DIR}/source/lib/serial_monitor_lib.h
${CMAKE_SOURCE_DIR}/source/lib/enum_device.cc
${CMAKE_SOURCE_DIR}/source/lib/enum_device.h
${CMAKE_SOURCE_DIR}/source/lib/device_info.cc
${CMAKE_SOURCE_DIR}/source/lib/device_info.h
${CMAKE_SOURCE_DIR}/source/lib/serial_monitor_utils.cc
${CMAKE_SOURCE_DIR}/source/lib/serial_monitor_utils.h
${CMAKE_SOURCE_DIR}/source/lib/device_changed.h
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


