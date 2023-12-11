#ifndef DEBUG_H_
#define DEBUG_H_
//----------------------------------------------------------------------------------
#include <stdio.h>
//----------------------------------------------------------------------------------
#ifdef  DEBUG
#define DEBUG_LEVEL 3
#else
#define DEBUG_LEVEL 1
#endif
//----------------------------------------------------------------------------------
#ifdef DEBUG
#define DEBUGSTR(x)         printf(x)
#define DEBUGOUT(...)		printf(__VA_ARGS__)
#endif
//----------------------------------------------------------------------------------
#if (DEBUG_LEVEL > 0)
#define  DBGErr(...)        printf("ERROR: ") ;\
														printf(__VA_ARGS__);\
                            printf("\n");
#else
#define DBGErr(...)
#endif
//----------------------------------------------------------------------------------
#if (DEBUG_LEVEL > 1)

#define  DBGLog(...)     printf(__VA_ARGS__);\

#else
#define DBGLog(...)
#endif
//----------------------------------------------------------------------------------
#if (DEBUG_LEVEL > 2)
#define  DBGInfo(...)  printf(__VA_ARGS__)
#else
#define DBGInfo(...)
#endif
//----------------------------------------------------------------------------------
#endif
