
/**
 * @file serial_monitor_lib.h
 * @brief 用于监控串口插入和拔出的库
 * @details 监控过程为单窗口请勿重复调用。
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

#define RETURN_SUCCESS                          (0)     /* 返回成功 */
#define RETURN_ERROR                            (-1)    /* 未知错误 */
#define RETURN_ERROR_INIT                       (-1000) /* 初始化失败 */
#define RETURN_ERROR_ARGUMENT_INVALID           (-1001) /* 参数非法 */
#define RETURN_ERROR_MEMORY_INSUFFICIENT        (-1002) /* 内存不足 */

#define NEW_DEVICE                              (1)     /* 新的设备 */
#define DELETE_DEVICE                           (2)     /* 被删除的设备 */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief 监控的状态回调函数，用于初始化，串口设备插入，串口设备拔出时，返回状态，需传入monitor_init函数中
 * @param id 串口设备的ID
 * @param status 发现新的设备或者该设备已删除
 *               NEW_DEVICE 
 *               DELETE_DEVICE
 * @param friendly_name 发现新的设备时返回友好的串口名字，删除设备时返回空字符串
 */
typedef void (*device_change_progress)(
    IN std::string id,
    IN int status,
    IN std::string friendly_name);

/**
 * @brief 用于初始化监听线程，监听是单窗口，请勿重复调用
 * @param device_change_progress 传入的回调函数
 *  
 */
int SERIAL_MONITOR_API monitor_init(
    IN device_change_progress progress_cb
);

/** @brief 用于关闭监听
 * 
 */
int SERIAL_MONITOR_API monitor_terminate();


#ifdef __cplusplus
}
#endif // __cplusplus

#endif