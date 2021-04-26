#pragma once

// CONFIGURATION MACROS DEFAULT VALUES

#ifndef PROTOCOL_ITEM_NUMBER
#define PROTOCOL_ITEM_NUMBER 2
#endif

#ifndef PROTOCOL_DEBUG_MODE
#define PROTOCOL_DEBUG_MODE 0
#endif

#ifndef PROTOCOL_INPUT_LINE_SIZE
#define PROTOCOL_INPUT_LINE_SIZE 140
#endif

#ifndef PROTOCOL_OUTPUT_LINE_SIZE
#define PROTOCOL_OUTPUT_LINE_SIZE 140
#endif

// JSON PARAMETRABLE AUTO RESPONSES :

#ifndef PROTOCOL_JSON_AKNOWLEDGE_STR
#define PROTOCOL_JSON_AKNOWLEDGE_STR "Ak"
#endif

#ifndef PROTOCOL_JSON_NOT_AKNOWLEDGE_STR
#define PROTOCOL_JSON_NOT_AKNOWLEDGE_STR "Na"
#endif

// BINARY PARAMETRABLE AUTO RESPONSES :

#ifndef PROTOCOL_BINARY_AKNOWLEDGE_BYTE
#define PROTOCOL_BINARY_AKNOWLEDGE_BYTE 0x41 // = A
#endif

#ifndef PROTOCOL_BINARY_NOT_AKNOWLEDGE_BYTE
#define PROTOCOL_BINARY_NOT_AKNOWLEDGE_BYTE 0x6E // = n
#endif

/// BINARY LINE START CHARACTERS : (in hex format)

#ifndef PROTOCOL_BINARY_LINE_START
#define PROTOCOL_BINARY_LINE_START 0x23 // = #
#endif

#ifndef PROTOCOL_BINARY_AKNOWLEDGE_START
#define PROTOCOL_BINARY_AKNOWLEDGE_START 0x40 // = @
#endif

/// RESPONSE WAIT TIMEOUTS : (in microseconds)

#ifndef PROTOCOL_TIMEOUT_JSON_AK
#define PROTOCOL_TIMEOUT_JSON_AK 10000
#endif

#ifndef PROTOCOL_TIMEOUT_BINARY_AK
#define PROTOCOL_TIMEOUT_BINARY_AK 4000 
#endif

#ifndef PROTOCOL_TIMEOUT_MAX_BYTE_INTERVAL
#define PROTOCOL_TIMEOUT_MAX_BYTE_INTERVAL 1000 
#endif