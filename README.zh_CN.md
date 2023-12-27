# serial_monitor
en [English](README.md) | zh_CN [简体中文](readme/README.zh_CN.md)

用于在windows上监听串口设备的插入和拔出,原理是开了个线程监听windows消息

发布了一个用于演示效果的[demo](releases/tag/demo)和一个用于使用的[lib库](releases/tag/lib)

![image](demo.gif)

核心头文件在[serial_monitor_lib](/source/lib/serial_monitor_lib.h)
# 函数介绍：
函数名     | 介绍
-------- | -----
device_change_progress | 监控的状态回调函数，用于初始化，串口设备插入，串口设备拔出时，返回状态，需传入monitor_init函数中
monitor_init | 用于初始化监听线程，监听是单窗口，请勿重复调用
monitor_terminate | 用于关闭销毁监听
