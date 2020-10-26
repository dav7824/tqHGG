#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fin_name = argv[1];
    TString sample = argv[2];
    TString treename = argv[3];

    TFile *fin = new TFile(fin_name);
    TTree *Tin = (TTree*)fin->Get(treename);

    cout << sample << "\t\t" << Tin->GetEntries() << endl;

    fin->Close();

    return 0;
}
