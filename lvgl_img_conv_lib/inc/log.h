#ifndef __INCLUDE_LOG_H_
#define __INCLUDE_LOG_H_

#include <stdio.h>

#define INF_LOG(fmt, ...)       printf("[INF]" fmt "\n", ##__VA_ARGS__);
#define DBG_LOG(fmt, ...)       printf("[DBG]" fmt "\n", ##__VA_ARGS__);
#define WRN_LOG(fmt, ...)       printf("[WRN]" fmt "\n", ##__VA_ARGS__);
#define ERR_LOG(fmt, ...)       printf("[ERR]" fmt "\n", ##__VA_ARGS__);
#define TRC_LOG(fmt, ...)       printf("[TRC]" fmt "\n", ##__VA_ARGS__);


#endif // __INCLUDE_LOG_H_
