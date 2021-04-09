#ifndef __PCA8695_H__
#define __PCA8695_H__

#include "datatype.h"

#ifdef __cplusplus
extern "C" {
#endif

void Pca9685Init();
void Pca9685SetPwmFrequency(byte frequency);
void Pca9685Reset();
void Pca9685SetPin(byte pinNum, word turnOnSteps);

#ifdef __cplusplus
}
#endif

#endif /* __PCA8695_H__ */