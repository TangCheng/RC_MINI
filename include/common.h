#ifndef __COMMON_H__
#define __COMMON_H__

#include "datatype.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PAYLOAD_LENGTH sizeof(struct Payload)
#define PAYLOAD_NEGOTIATION_HEADER 0xA0
#define PAYLOAD_CONTROL_DATA_HEADER 0xA1

struct Payload {
    byte header;
    union {
        struct {
            byte channel[5];
            byte address[5];
        };
        struct {
            byte throttle;
            byte steering;
        };
    };
};

#ifdef __cplusplus
}
#endif

#endif /* __COMMON_H__ */