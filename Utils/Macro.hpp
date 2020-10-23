#pragma once
//Abusive macros for dealing with radian/degree conversion
#define RAD2DEG(x) ((x) * 180.0 / M_PI )
#define DEG2RAD(x) ((x) * M_PI / 180.0 )

//Abusive macros for patternscanning
#define INRANGE(x, a, b) ((x) >= (a) && (x) <= (b)) 
//bruh tf are these
#define GetBits(x) (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0)) 
#define GetByte(x) (GetBits(x[0]) << 4 | GetBits(x[1]))