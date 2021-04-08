#ifndef __UTILS_H__
#define __UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define HUNDREDS_PLACE(x) (x / 100)
#define HUNDREDS_PLACE_CHAR(x) (HUNDREDS_PLACE(x) + '0')
#define TENS_PLACE(x) ((x % 100) / 10)
#define TENS_PLACE_CHAR(x) (TENS_PLACE(x) + '0')
#define ONES_PLACE(x) (x % 10) 
#define ONES_PLACE_CHAR(x) (ONES_PLACE(x) + '0')

#ifdef __cplusplus
}
#endif

#endif /* __UTILS_H__ */