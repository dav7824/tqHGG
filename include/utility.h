#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <vector>
#include <string>
using namespace std;

void ParseCStringList(const char*, vector<string>&);
double GetHistVal(const char*, const char*);
double GetNormFactor(const char*);

#endif
