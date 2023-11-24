#ifndef USERMODE_HPP
#define USERMODE_HPP

#define Debug 1

#define FPS   200

#define print(a, ...) \
    if (Debug) printf(a, __VA_ARGS__);

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif
void Start();
void Update();
void Awake();
#ifdef __cplusplus
}
#endif

#endif // !USERMODE_HPP