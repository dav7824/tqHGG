#include "utility.h"

#include "TFile.h"
#include "TH1D.h"
#include "TKey.h"

int main(int argc, char **argv)
{
    // get command line arguments
    char *input = argv[1];
    char *normFile = argv[2];
    char *output = argv[3];

    TFile *fin = new TFile(input);
    TFile *fout = new TFile(output, "recreate");

    // get input histograms
    TKey *key = 0;
    TIter nextkey(fin->GetListOfKeys());
    while ((key = (TKey*)nextkey())) {
	TH1D *hist = (TH1D*)key->ReadObj();
	hist->Scale( GetNormFactor(normFile) );
	fout->cd();
	hist->Write();
    }

    fout->Close();
    fin->Close();

    return 0;
}
