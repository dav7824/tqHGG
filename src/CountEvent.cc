#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"

#include <iostream>
using namespace std;

void CountEvent(const char *input, const char *treeName, const char *output)
{
    cout << "Openning input file: " << input << endl;

    TFile *fin = new TFile(input);
    TTree *T = (TTree*)fin->Get(treeName);

    double yield = T->GetEntries();

    TFile *fout = new TFile(output, "recreate");
    TH1D *hist = new TH1D("yield", "", 1, 0., 1.);
    hist->SetBinContent(1, yield);
    hist->Write();
    fout->Close();
    fin->Close();

    cout << "Output is saved at: " << output << endl;
}

int main(int argc, char **argv)
{
    CountEvent(argv[1], argv[2], argv[3]);
    return 0;
}
