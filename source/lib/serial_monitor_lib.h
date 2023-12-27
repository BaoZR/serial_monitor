
/**
 * @file serial_monitor_lib.h
 * @brief 用于监控串口物理插入和物理拔出的库，只能在win32下使用
 * @details 监控窗口为固定名字的窗口，所以请勿重复调用。
 * @author cuicui
 * @date 2023-09-04
 * @version 0.1
 * @copyright MIT
 */



#ifndef _SERIAL_MONITOR_LIB_H_
#define _SERIAL_MONITOR_LIB_H_

#include <string>

#ifdef SERIAL_MONITOR_LIB_EXPORTS
#ifdef _WIN32
#define SERIAL_MONITOR_API __declspec(dllexport) __stdcall
#else
#define SERIAL_MONITOR_API __attribute__((visibility("default")))
#endif // _WIN32
#else
#ifdef _WIN32
#define SERIAL_MONITOR_API __declspec(dllimport) __stdcall
#else
#define SERIAL_MONITOR_API
#endif // _WIN32
#endif // SERIAL_MONITOR_LIB_EXPORTS

#define IN
#define OUT
#define INOUT


#define DEVICE_NEW                              (1)     /* 新的设备 */
#define DEVICE_DELETE                           (2)     /* 被删除的设备 */

typedef std::string DeviceId;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief 监控的状态回调函数，用于初始化，串口设备插入，串口设备拔出时，返回状态，需传入monitor_init函数中
 * @param id 串口设备的ID
 * @param status 发现新的设备或者该设备已删除
 *               DEVICE_NEW 
 *               DEVICE_DELETE
 * @param friendly_name 发现新的设备时返回友好的串口名字，删除设备时返回之前友好的串口名字
 */
typedef void (*device_change_progress)(
    IN const std::string& id,
    IN int status,
    IN const std::string& friendly_name);

/**
 * @brief 用于初始化监听线程，监听是单窗口，请勿重复调用
 * @param device_change_progress 传入的回调函数
 *  
 */
void SERIAL_MONITOR_API monitor_init(
    IN device_change_progress progress_cb
);

/** @brief 用于关闭销毁监听
 * 
 */
void SERIAL_MONITOR_API monitor_terminate();


#ifdef __cplusplus
}
#endif // __cplusplus

#endif