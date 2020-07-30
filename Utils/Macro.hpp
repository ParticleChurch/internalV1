#pragma once
//Abusive macros for dealing with radian/degree conversion
#define RAD2DEG(x) (x) * 360 / (M_PI * 2)
#define DEG2RAD(x) (x) * (M_PI * 2) / 360

//Abusive macros for patternscanning
#define INRANGE(x,a,b) (x >= a && x <= b) 
#define GetBits(x) (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define GetByte(x) (GetBits(x[0]) << 4 | GetBits(x[1]))