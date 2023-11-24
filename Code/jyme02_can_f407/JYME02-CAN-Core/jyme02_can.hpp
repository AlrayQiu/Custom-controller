/**
 ******************************************************
 * @file            jyme02_can_hpp
 * @brief           jyme02_can_cpp 头文件，定义了对
 *                  jyme02_can设备常用的操作
 *                  用户手册： https://wit-motion.yuque.com/wumwnr/docs/qcgu36
 * @author          Alray
 * @date            2023/10/28
 * @version         0.1
 ******************************************************
 */

/**
 * 说明书懒人版
 * 协议
 *      can通讯
 *      默认baud 250k
 *      默认回传 10hz
 *      默认地址 0x50
 * 发送相关
 *      标准帧
 *      数据帧
 *      帧大小：5byte
 *      帧格式: 0xFF|0xAA|reg_addr|datalow|datahight
 * 接收相关
 *      标准帧
 *      数据帧
 *      帧大小：8byte
 *      帧格式
 *          角度模式 0x55|unknow|angleLow|angleHight
 *                          |rotLow|rotHight|speedLow|speedHight|
 *          温度模式 0x55|unknow|temLow|temHight|00|00|00|00
 *      角度计算 :数值*360/32768
 */

#ifndef _JYME02_CAN_HPP
#define _JYME02_CAN_HPP

/*====================*/
// 宏定义
// RSW
#define JYME02_RSW_A  0x01 // 角度模式
#define JYME02_RSW_T  0x02 // 温度模式
#define JYME02_RSW_AT 0x03 // 全回模式
//

// RRATE
#define JYME02_REFRATE_0_1    0x00 // 0.1hz
#define JYME02_REFRATE_0_2    0x01 // 0.2hz
#define JYME02_REFRATE_0_5    0x02 // 0.5hz
#define JYME02_REFRATE_1      0x03 // 1hz
#define JYME02_REFRATE_2      0x04 // 2hz
#define JYME02_REFRATE_5      0x05 // 5hz
#define JYME02_REFRATE_10     0x06 // 10hz(默认)
#define JYME02_REFRATE_20     0x07 // 20hz
#define JYME02_REFRATE_50     0x08 // 50hz
#define JYME02_REFRATE_100    0x09 // 100hz
#define JYME02_REFRATE_125    0x0A // 125hz
#define JYME02_REFRATE_200    0x0B // 200hz
#define JYME02_REFRATE_1000   0x0C // 1000hz
#define JYME02_REFRATE_2000   0x0D // 2000hz
#define JYME02_REFRATE_SINGLE 0x0E // 单次回传
//

// Baud Rate
#define JYME02_BAUDRATE_1000 0x00 // 1000K
#define JYME02_BAUDRATE_800  0x01 // 800K
#define JYME02_BAUDRATE_500  0x02 // 500K
#define JYME02_BAUDRATE_400  0x03 // 400K
#define JYME02_BAUDRATE_250  0x04 // 250K
#define JYME02_BAUDRATE_200  0x05 // 200K
#define JYME02_BAUDRATE_125  0x06 // 125K(默认)
#define JYME02_BAUDRATE_100  0x07 // 100K
#define JYME02_BAUDRATE_80   0x08 // 80K
#define JYME02_BAUDRATE_50   0x09 // 50K
#define JYME02_BAUDRATE_40   0x0A // 40K
#define JYME02_BAUDRATE_20   0x0B // 20K
#define JYME02_BAUDRATE_10   0x0C // 10K
#define JYME02_BAUDRATE_5    0x0D // 5K
#define JYME02_BAUDRATE_3    0x0E // 3K
//

// Mode
#define JYME02_MODE_SINGLE 0x00 // 单圈模式
#define JYME02_MODE_MULTY  0x01 // 多圈模式
//

//
// 宏定义结束

/*====================*/
// 头文件导入
#include "main.h"
// 头文件导入结束

/*====================*/
// 类型定义
typedef uint8_t BOOL;
typedef uint16_t Device_Addr;
typedef uint8_t Reg_Addr;
typedef uint8_t JmMode;
typedef uint8_t JYME02_RSW;
typedef uint8_t JYME02_REFRATE;
// 类型定义结束

#ifdef __cplusplus
extern "C" {
#endif
/*====================*/
// 结构体定义

// 结构体定义结束

/*====================*/
// 函数定义
BOOL Jyme02_Can_Init(CAN_HandleTypeDef *phcan, uint32_t filterfifo);
float Jyme02_Can_Read_AngleMode(Device_Addr *addr);
void Jyme02_Can_Set_Addr(Device_Addr addr, Device_Addr newaddr);
void Jyme02_Can_Set_Baud(Device_Addr addr, uint16_t baud);
void Jyme02_Can_Set_Mode(Device_Addr addr, JmMode mode);
void Jyme02_Can_Set_Speed(Device_Addr addr, JYME02_REFRATE rate);
void Jyme02_Can_Set_RefInc(Device_Addr addr, JYME02_RSW inc);
// 函数定义结束
#ifdef __cplusplus
}
#endif

#endif