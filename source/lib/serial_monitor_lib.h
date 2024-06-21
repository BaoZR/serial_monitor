
/**
 * @file serial_monitor_lib.h
 * @brief Library for monitoring physical insertion and removal of serial ports, can only be used under win32
 * @details The monitoring window has a fixed name, so please do not call it repeatedly.
 * @author cuicui
 * @date 2023-09-04
 * @version 0.1
 * @copyright MIT
 */



#ifndef _SERIAL_MONITOR_LIB_H_
#define _SERIAL_MONITOR_LIB_H_



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


#define DEVICE_NEW                              (1)     /* find new device */
#define DEVICE_DELETE                           (2)     /* device deleted */



#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief The status callback function of the monitor needs to be passed into the monitor_init initialization function. When a serial device is inserted or pulled out, it returns device status information.
 * @param id Serial device ID
 * @param status New device found or old device deleted
 *               DEVICE_NEW 
 *               DEVICE_DELETE
 * @param friendly_name Return the friendly serial name when discovering new devices, and return the previous friendly serial name when devices deleted
 */
typedef void (*device_change_progress)(
    IN const char* id,
    IN int status,
    IN const char* friendly_name);

/**
 * @brief Used to initialize the monitoring thread. Because the listening is single-window, please don't call it repeatedly.
 * @param device_change_progress Require incoming callback function
 *  
 */
void SERIAL_MONITOR_API monitor_init(
    IN device_change_progress progress_cb
);

/** @brief Used to destroy the listener.
 * 
 */
void SERIAL_MONITOR_API monitor_terminate();


#ifdef __cplusplus
}
#endif // __cplusplus

#endif