#ifndef JYME02_CAN_CONTROL_HPP
#define JYME02_CAN_CONTROL_HPP

#include "jyme02_can.hpp"

#define ME02_T 0x50
#define ME02_R 0x61
#define ME02_L 0x72

#ifdef __cplusplus
extern "C" {
#endif
void Renew_Me02_Data(Device_Addr Addr, float Data);
void Zero_Me02_Data();
void SetAllReadMode();

#ifdef __cplusplus
}
#endif
#endif // ! JYME02_CAN_CONTROL_HPP