#include "usermode.hpp"
#include "jyme02_can_control.hpp"
#include "controller.hpp"
#include "inv_mpu.h"
#include <stdio.h>
#include <string.h>

using namespace std;

extern CAN_HandleTypeDef hcan1;

extern UART_HandleTypeDef huart1;

extern float Me02_Data[3];

Controller CusControl = Controller(Me02_Data, nullptr);

Attitue Current;

// 初始化结束后开始时执行
void Start()
{
    int i = 0;
    if (Jyme02_Can_Init(&hcan1, CAN_FilterFIFO0)) {
        printf("Jyme02_Can_Init Failed\n");
    }
    if (i = mpu_dmp_init()) {
        printf("mpu_init Failed:%d\n", i);
    }
    HAL_Delay(2000);
    SetAllReadMode();
    HAL_Delay(500);
    Zero_Me02_Data();
    HAL_Delay(500);
    CusControl.UpdateAttitue();
    CusControl.UpdateAttitue();
    CusControl.ZeroController();
}

char a[300];

// 每帧执行
void Update()
{

    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) != GPIO_PIN_SET) {
        Zero_Me02_Data();
        CusControl.ZeroController();
    }
    CusControl.UpdateAttitue();
    Current = CusControl.GetAttitue();
    printf("x= %0.5f,y= %0.5f,z= %0.5f,pitch= %0.5f,roll= %0.5f ,yaw= %0.5f\n",
           Current.Position.Get().x, Current.Position.Get().y, Current.Position.Get().z,
           Current.EularAngle.Get().x, Current.EularAngle.Get().y, Current.EularAngle.Get().z);
    // printf("%0.5f,%0.5f,%0.5f,0\n",
    //        Current.EularAngle.Get().x, Current.EularAngle.Get().y, Current.EularAngle.Get().z);

    // HAL_UART_Transmit(&huart1, (uint8_t *)a, 300, 3);
}
