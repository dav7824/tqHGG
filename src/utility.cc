#include "utility.h"

#include "TFile.h"
#include "TH1D.h"

#include <vector>
#include <string>
using namespace std;

// Take a pointer based string which contains a list of fields seperated by commas, and convert it to a vector of strings
// For predictable results, provide at least one field and all fields be non-empty
void ParseCStringList(const char *input, vector<string> &output)
{
    int idx = 0;
    char read = '\0';
    char sep = ',';
    string tmp_str;

    do {
	read = input[idx++];
	if (read == sep || read == '\0') {
	    output.push_back(tmp_str);
	    tmp_str.clear();
	    if (read == '\0') break;
	}
	else tmp_str += read;
    } while (true);
}

double GetHistVal(const char *fileName, const char *histName)
{
    TFile *fin = new TFile(fileName);
    TH1D *hist = (TH1D*)fin->Get(histName);
    double val = hist->GetBinContent(1);

    fin->Close();

    return val;
}

double GetNormFactor(const char *fileName)
{
    return GetHistVal(fileName, "normfactor");
}
