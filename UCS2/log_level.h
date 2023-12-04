#ifndef __LOG_LEVEL_
#define __LOG_LEVEL_

#include <stdio.h>


#define INFO_LOG_ON 1
#define DEBUG_LOG_ON 0
#define ERROR_LOG_ON 0


#if INFO_LOG_ON
#define INFO_LOG(fmt,arg...)           printf(fmt,##arg)
#else 
#define INFO_LOG(...)
#endif

#if DEBUG_LOG_ON
#define INFO_LOG(fmt,arg...)           printf(fmt,##arg)
#define DEBUG_LOG(fmt,arg...)          printf("FILE NAME:"__FILE__ "\r\n""LINE:%d\r\n"fmt"\r\n\n",__LINE__,##arg)
#else 
#define DEBUG_LOG(...)
#endif

#if ERROR_LOG_ON
#define INFO_LOG(fmt,arg...)           printf(fmt,##arg)
#define DEBUG_LOG(fmt,arg...)          printf("FILE NAME:"__FILE__ "\r\n""LINE:%d\r\n"fmt"\r\n\n",__LINE__,##arg)
#define ERROR_LOG(fmt,arg...)          printf("FILE NAME:"__FILE__ "\r\n""LINE:%d\r\n"fmt"\r\n\n",__LINE__,##arg)
#else 
#define ERROR_LOG(...)
#endif

#endif /*__LOG_LEVEL_*/
