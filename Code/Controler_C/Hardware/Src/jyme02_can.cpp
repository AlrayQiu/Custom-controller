/**
 *************************************************
 * @file        jyme02_can
 * @brief       jyme02_can_常用操作
 * @author      Alray
 * @date        2023/10/28
 * @version     0.1
 *************************************************
 */

/*================*/
// 宏定义
// 寄存器地址
#define Jm_RegAddr_Save    0x00
#define Jm_RegAddr_Rsw     0x02
#define Jm_RegAddr_RetRate 0x03
#define Jm_RegAddr_Baud    0x04
#define Jm_RegAddr_EnMode  0x10
#define Jm_RegAddr_AST     0x17
#define Jm_RegAddr_DevAddr 0x1A
//

// 设置管理
#define setUnlock      can_write_reg(addr, 0x69u, (0xB5u << 8) | 0x88)
#define saveSettings   can_write_reg(addr, Jm_RegAddr_Save, (0x00u << 8 | 0x00u))
#define saveSettings_n can_write_reg(newaddr, Jm_RegAddr_Save, (0x00u << 8 | 0x00u))
//
// 宏定义结束

/*===============*/
// 头文件包含
#include "jyme02_can.hpp"
#include "jyme02_can_control.hpp"
// 头文件包含结束

/*===============*/
// 私有参数定义
// can
CAN_HandleTypeDef *jymecan;
CAN_TxHeaderTypeDef txHeader;
CAN_RxHeaderTypeDef rxHeader;
CAN_FilterTypeDef filter;

uint8_t txData[5] = {0xff, 0xaa, 0, 0, 0};
uint8_t rxData[8];

uint32_t txmailbox;
uint32_t jymefifo;

float Me02_Zero_Data[3];
float Me02_Raw_Data[3];
float Me02_Data[3];
// 私有参数定义结束

/*===============*/
// 外部参数引用

// 外部参数引用结束

/**
 ***********************************
 * @def     can_read_reg(Device_Addr addr, Reg_Addr regaddr, uint8_t* buffer)
 * @brief   读取设备寄存器地址
 * @param   addr        设备地址
 * @param   regaddr     寄存器地址
 * @param   buffer      接收内容缓存
 * @return  null
 ***********************************
 */
void can_read_reg(Device_Addr addr, Reg_Addr regaddr, uint16_t buffer)
{
    // todo
}

/**
 ***********************************
 * @def     can_write_reg(Device_Addr addr,
 *              Reg_Addr regaddr, uint8_t* buffer,uint16_t size)
 * @brief   写入设备寄存器地址
 * @param   addr        设备地址
 * @param   regaddr     寄存器地址
 * @param   buffer      写入内容
 * @param   size        写入内容大小
 * @return  null
 ***********************************
 */
void can_write_reg(Device_Addr addr, Reg_Addr regaddr, uint16_t buffer)
{
    txHeader.StdId = addr;
    txData[2]      = regaddr;
    txData[3]      = buffer;
    txData[4]      = buffer >> 8;
    HAL_CAN_AddTxMessage(jymecan, &txHeader, txData, &txmailbox);
}

/**
 ***********************************
 * @def     Jyme02_Can_Init(CAN_HandleTypeDef* phcan,uint32_t filterfifo)
 * @brief   传感器初始化
 * @param   phcan       can句柄地址
 * @param   filterfifo 过滤器关联邮箱
 * @return  (BOOL)      成功初始化
 ***********************************
 */
BOOL Jyme02_Can_Init(CAN_HandleTypeDef *phcan, uint32_t filterfifo)
{
    jymecan = phcan;

    // can_txHeader预设
    txHeader.IDE = CAN_ID_STD;   // 标准帧
    txHeader.DLC = 5;            // 5字节输出
    txHeader.RTR = CAN_RTR_DATA; // 数据帧

    // can_filter
    filter.FilterBank  = 0;                     // 过滤器12
    filter.FilterMode  = CAN_FILTERMODE_IDMASK; // 设为列表模式
    filter.FilterScale = CAN_FILTERSCALE_32BIT; // 滤波器位宽为两个16位

    filter.FilterIdHigh         = 0x0000; // 基本ID放入到STID中
    filter.FilterIdLow          = 0x0000;
    filter.FilterMaskIdHigh     = 0;                // 这里设置接收任意ID的CAN数据
    filter.FilterMaskIdLow      = 0;                // 0表示接收来自发送端任意ID的数据
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0; // 接收到的报文放入到FIFO0中

    filter.FilterActivation = CAN_FILTER_ENABLE; // 激活过滤器

    uint8_t res = 0;

    if (HAL_OK != HAL_CAN_ConfigFilter(jymecan, &filter)) {
        res = 1;
        goto end;
    }
    if (HAL_OK != HAL_CAN_Start(jymecan)) {
        res = 1;
        // goto end;
    }
    if (filterfifo == CAN_FilterFIFO0) {
        if (HAL_OK != HAL_CAN_ActivateNotification(jymecan, CAN_IT_RX_FIFO0_MSG_PENDING)) {
            res = 1;
            goto end;
        }
    } else
        HAL_CAN_ActivateNotification(jymecan, CAN_IT_RX_FIFO1_MSG_PENDING);

end:
    return res;
}

/**
 ***********************************
 * @def     Jyme02_Can_ReadAngle(Device_Addr addr)
 * @brief   传感器角度读取
 * @param   addr        设备地址
 * @return  (float)     角度制角度值[0,360)
 ***********************************
 */
float Jyme02_Can_Read_AngleMode(Device_Addr *addr)
{
    HAL_CAN_GetRxMessage(jymecan, CAN_RX_FIFO0, &rxHeader, rxData);
    *addr = rxHeader.StdId;
    if (rxData[1] == 0x56)
        return Me02_Raw_Data[*addr & 0x0f];
    uint32_t angle_t = (((uint32_t)rxData[3]) << 8) | rxData[2];
    HAL_CAN_ActivateNotification(jymecan, CAN_IT_RX_FIFO0_MSG_PENDING);
    return angle_t / 20860.7568f;
}

/**
 ***********************************
 * @def     Jyme02_Can_Set_Addr(Device_Addr addr,Device_Addr newaddr)
 * @brief   设备地址修改
 * @param   addr        设备地址
 * @param   newaddr     设置后的地址
 * @return  null
 **********************************
 */
void Jyme02_Can_Set_Addr(Device_Addr addr, Device_Addr newaddr)
{
    setUnlock;
    can_write_reg(addr, Jm_RegAddr_DevAddr, newaddr);
    saveSettings;
    saveSettings_n;
}

/**
 **********************************
 * @def     Jyme02_Can_Set_Baud(Device_Addr addr,uint16_t baud)
 * @brief   设备修改波特率
 * @param   addr        设备地址
 * @param   baud        波特率
 * @return  null
 *********************************
 */
void Jyme02_Can_Set_Baud(Device_Addr addr, uint16_t baud)
{
    setUnlock;
    can_write_reg(addr, Jm_RegAddr_Baud, baud);
    saveSettings;
}

/**
 *********************************
 * @def     Jyme02_Can_Set_Mode(Device_Addr addr,JmMode mode)
 * @brief   设置运行模式
 * @param   addr        设备地址
 * @param   mode        模式
 * @return  null
 *********************************
 */
void Jyme02_Can_Set_Mode(Device_Addr addr, JmMode mode)
{
    setUnlock;
    can_write_reg(addr, Jm_RegAddr_EnMode, mode);
    saveSettings;
}

/**
 *********************************
 * @def     Jyme02_Can_Set_Speed(Device_Addr addr,JYME02_REFRATE rate)
 * @brief   设置设备采样率
 * @param   addr        设备地址
 * @param   rate        速率（Hz）
 * @return  null
 *********************************
 */
void Jyme02_Can_Set_Speed(Device_Addr addr, JYME02_REFRATE rate)
{
    setUnlock;
    can_write_reg(addr, Jm_RegAddr_RetRate, rate);
    saveSettings;
}

/**
 *********************************
 * @def     Jyme02_Can_Set_RefInc(Device_Addr addr,JYME_02_RSW  inc)
 * @brief   设置回传内容
 * @param   addr        设备地址
 * @param   inc         包含内容
 * @return  null
 *********************************
 */
void Jyme02_Can_Set_RefInc(Device_Addr addr, JYME02_RSW inc)
{
    setUnlock;
    can_write_reg(addr, Jm_RegAddr_Rsw, inc);
    saveSettings;
}

/**
 *********************************
 * @def     Renew_Me02_Data(Device_Addr Addr, float angle)
 * @brief   更新数据
 * @param   addr        设备地址
 * @param   angle       角度值
 * @return  null
 *********************************
 */
void Renew_Me02_Data(Device_Addr Addr, float angle)
{
    Me02_Raw_Data[Addr & 0x0f] = angle;
    Me02_Data[Addr & 0x0f]     = angle - Me02_Zero_Data[Addr & 0x0f];
}

/**
 *********************************
 * @def     Zero_Me02_Data(Device_Addr Addr, float angle)
 * @brief   置零
 * @param   addr        设备地址
 * @param   angle       角度值
 * @return  null
 *********************************
 */
void Zero_Me02_Data()
{
    Me02_Zero_Data[0] = Me02_Raw_Data[0];
    Me02_Zero_Data[1] = Me02_Raw_Data[1];
    Me02_Zero_Data[2] = Me02_Raw_Data[2];
}

void SetAllReadMode()
{
    Jyme02_Can_Set_RefInc(ME02_T, JYME02_RSW_A);
    Jyme02_Can_Set_RefInc(ME02_L, JYME02_RSW_A);
    Jyme02_Can_Set_RefInc(ME02_R, JYME02_RSW_A);
}
