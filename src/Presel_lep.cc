// Preselection:
//   Select leptonic events.

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TString.h"

#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
    // Get input arguments
    TString fin_name = argv[1];
    TString inTree_name = argv[2];
    TString fout_name = argv[3];

    // Create TTree for input n-tuple
    cout << "Openning input file: " << fin_name << endl;
    TChain *inTree = new TChain(inTree_name);
    inTree->Add(fin_name);

    // Create TTree for output n-tuple
    cout << "Creating output file: " << fout_name << endl;
    TFile *fout = new TFile(fout_name, "recreate");
    TTree *outTree = inTree->CopyTree("ElecInfo.Size+MuonInfo.Size>=1 && jets_size>=1");

    // Write output TTree to output TFile
    outTree->Write();
    // Close output TFile
    fout->Close();

    cout << "Completed!\n";

    return 0;
}
