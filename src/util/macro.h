#pragma once

#if defined(__GNUC__) || defined(__clang__)
    #define DISCARD_WARN __attribute__((warn_unused_result))
#else
    #define DISCARD_WARN
#endif

// specify that a function takes printf-style arguments and to perform compile-time checks for format string consistency.
#if defined(__GNUC__) || defined(__clang__)
    #define C_PRINTF(formatStrIndex, argsStartIndex) __attribute__((__format__(printf, formatStrIndex, argsStartIndex)))
#else
    #define C_PRINTF(formatStrIndex, argsStartIndex)
#endif

#if defined(__GNUC__) || defined(__clang__)
    #define C_SCANF(formatStrIndex, argsStartIndex) __attribute__((__format__(scanf, formatStrIndex, argsStartIndex)))
#else
    #define C_SCANF(formatStrIndex, argsStarIndex)
#endif 