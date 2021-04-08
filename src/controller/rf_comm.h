#ifndef __RF_COMM_H__
#define __RF_COMM_H__

#include "datatype.h"

#ifdef __cplusplus
extern "C" {
#endif

void CommunicationInit();
bool PairWithReceiver();
void SendControlData(byte throttle, byte steering);

#ifdef __cplusplus
}
#endif

#endif /* __RF_COMM_H__ */