
#ifndef _STDINT_H
#define _STDINT_H    1

// So we don't include Tapwave files]
#define __STDINT_H__

typedef unsigned char u_int8_t;
typedef unsigned short int u_int16_t;
typedef unsigned long int u_int32_t;

typedef signed char int8_t;
typedef unsigned char uint8_t;

typedef signed short int16_t;
typedef unsigned short uint16_t;

typedef signed long int32_t;
typedef unsigned long uint32_t;

#ifdef _MSC_VER
typedef signed __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
typedef signed long long int64_t;
typedef unsigned long long uint64_t;
#endif

typedef signed long intptr_t;
typedef unsigned long uintptr_t;

#endif /* stdint.h */
