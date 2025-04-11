#pragma once

#include "nrf.h"
#include "common_func.h"

// Thread-safe version of ADALOG that uses critical sections
#define SAFE_ADALOG(tag, ...) \
  do { \
    uint32_t primask = __get_PRIMASK(); \
    __disable_irq(); \
    if ( tag ) PRINTF("[%-6s] ", tag); \
    PRINTF(__VA_ARGS__); \
    PRINTF("\r\n"); \
    if (!primask) __enable_irq(); \
  } while(0)

// Thread-safe version of ADALOG_BUFFER that uses critical sections  
#define SAFE_ADALOG_BUFFER(_tag, _buf, _n) \
  do { \
    uint32_t primask = __get_PRIMASK(); \
    __disable_irq(); \
    const char * _xtag = _tag; \
    if ( _xtag ) PRINTF("%-6s: len = %d\r\n", _xtag, _n); \
    dbgDumpMemory(_buf, 1, _n, true); \
    if (!primask) __enable_irq(); \
  } while(0) 