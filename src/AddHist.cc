#include "utility.h"

#include "TFile.h"
#include "TH1D.h"
#include "TKey.h"

#include <vector>
#include <string>
using namespace std;

int main(int argc, char **argv)
{
    // get command line arguments
    vector<string> input;    ParseCStringList(argv[1], input);
    char *output = argv[2];

    vector<TFile*> fin;
    fin.resize(input.size());
    for (int i=0; i<fin.size(); ++i) {
	fin[i] = new TFile(input[i].c_str());
    }
    TFile *fout = new TFile(output, "recreate");

    // get input histograms
    TKey *key = 0;
    TIter nextkey(fin[0]->GetListOfKeys());
    while ((key = (TKey*)nextkey())) {
	TH1D *hist = (TH1D*)key->ReadObj();
	for (int i=1; i<fin.size(); ++i) {
	    TH1D *hist_ = (TH1D*)fin[i]->Get( key->GetName() );
	    hist->Add(hist_);
	}
	hist->Write();
    }

    fout->Close();
    for (int i=0; i<fin.size(); ++i) fin[i]->Close();

    return 0;
}
