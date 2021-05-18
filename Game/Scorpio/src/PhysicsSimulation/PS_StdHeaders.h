#ifndef __PS_StdHeaders_H__
#define __PS_StdHeaders_H__

#include <cassert>

// STL containers
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <map>
#include <set>
#include <string>
#include <bitset>

// Note - not in the original STL, but exists in SGI STL and STLport
#if defined(__GNUC__) && !defined(STLPORT)
#   include <ext/hash_map>
#   include <ext/hash_set>
#else
#   include <hash_set>
#   include <hash_map>
#endif

// STL algorithms & functions
#include <algorithm>
#include <functional>
#include <limits>

// C++ Stream stuff
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

#endif // __PS_StdHeaders_H__
