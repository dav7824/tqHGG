#ifndef _SCALEFACTOR_H_
#define _SCALEFACTOR_H_

#include "TreeReader.h"

#include "TFile.h"
#include "TH2.h"

#include <vector>
using namespace std;

void PrepareSF(vector<TFile*>&, vector<TH2*>&);
void ApplySF(TreeReader&, vector<TH2*>&, float&);
void CloseSF(vector<TFile*>&);

#endif
