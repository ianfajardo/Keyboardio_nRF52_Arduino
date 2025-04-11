/**************************************************************************/
/*!
    @file     debug_config.h
    @author   Cascade AI

    @brief    Configuration for debug output redirection to Serial
*/
/**************************************************************************/

#ifndef _DEBUG_CONFIG_H_
#define _DEBUG_CONFIG_H_

#include <Arduino.h>



// Redirect printf to Serial
#ifdef PRINTF
#undef PRINTF
#endif

#define PRINTF(...) Serial.printf(__VA_ARGS__)

#endif /* _DEBUG_CONFIG_H_ */
