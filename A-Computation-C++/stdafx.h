#ifndef _STDAFX_H
#define _STDAFX_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <stack>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <set>
#include <ctime>
#include <queue>
#include <functional>
#include <algorithm>
#include <random>
#include <list>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdint.h>
#include <bitset>
#include <stack>

#ifdef WIN32
#else
#include <sys/time.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#define  _mkdir mkdir
#define  _access access
#define  ASSERT assert
#define _stati64 stat64
#define _fseeki64 fseeko
#define _FILE_OFFSET_BITS 64  
#endif

typedef unsigned char uchar;
typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned int uint;
const std::string AIDS_Query = "d:\\data\\query\\AIDS";
using namespace std;
#endif
