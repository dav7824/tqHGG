#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include <iostream>
#include <cstdlib>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fin_name = argv[1];
    TString ch = argv[2];
    if (ch!="had" && ch!="lep") {
        cout << "[ERROR] Invalid channel!\n";
        exit(1);
    }
    bool is_lep = false;
    if (ch == "lep") is_lep = true;

    // Get file
    TFile *fin = new TFile(fin_name, "update");
    TTree *T = (TTree*)fin->Get("T");

    float SF_val = 1;

    // Create dummy trees
    TTree *SF_pileup = 0;
    TTree *SF_btag = 0;
    TTree *SF_Elec = 0;
    TTree *SF_Muon = 0;
    SF_pileup = new TTree("SF_pileup", "");
    SF_btag = new TTree("SF_btag", "");
    if (is_lep) {
        SF_Elec = new TTree("SF_Elec", "");
        SF_Muon = new TTree("SF_Muon", "");
    }
    // Set branches
    SF_pileup->Branch("SF_pileup", &SF_val);
    SF_btag->Branch("SF_btag", &SF_val);
    if (is_lep) {
        SF_Elec->Branch("Reco", &SF_val);
        SF_Elec->Branch("ID", &SF_val);
        SF_Muon->Branch("ID", &SF_val);
        SF_Muon->Branch("ISO", &SF_val);
    }

    // Start event loop
    for (int evt=0; evt<T->GetEntries(); ++evt)
    {
        SF_pileup->Fill();
        SF_btag->Fill();
        if (is_lep) {
            SF_Elec->Fill();
            SF_Muon->Fill();
        }
    } // End of event loop

    // Save trees
    fin->WriteTObject(SF_pileup, "", "Overwrite");
    fin->WriteTObject(SF_btag, "", "Overwrite");
    if (is_lep) {
        fin->WriteTObject(SF_Elec, "", "Overwrite");
        fin->WriteTObject(SF_Muon, "", "Overwrite");
    }

    fin->Close();

    return 0;
}
