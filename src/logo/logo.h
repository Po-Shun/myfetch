#pragma once
#include <stdint.h>

// padding setup
#define LOGO_PADDING_RIGHT 4
#define LOGO_PADDING_CHAR ' '
#define ANSI_ESCAPE_CODE "\e["

// color definition
#define COLOR_RED "31"
#define COLOR_WHITE "37"


typedef struct my_fetch_logo
{
    const char* lines;
    const char* names[9];
    const char* colors[9];
    const char* colorKeys;
    const char* colorTitle;
} my_fetch_logo;

typedef struct logo_state
{
    uint32_t logoHeight;
} logo_state;
extern const my_fetch_logo* logoBuiltin[];
static logo_state state; // Global Singletion
my_fetch_logo* setupLogoBuiltin(const char* osID);
void LogoPrint(const char* data);
void InitLogoState(logo_state* state);