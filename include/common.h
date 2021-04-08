#ifndef __COMMON_H__
#define __COMMON_H__

#include "datatype.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PAYLOAD_LENGTH 11
#define PAYLOAD_NEGOTIATION_HEADER 0xA0
#define PAYLOAD_DATA_HEADER 0xA1

struct PairPayload {
    byte header;
    byte hopping[5];
    byte address[5];
};

struct DataPayload {
    byte header;
    byte throttle;
    byte steering;
    byte reserved[8];
};

#ifdef __cplusplus
}
#endif

#endif /* __COMMON_H__ */