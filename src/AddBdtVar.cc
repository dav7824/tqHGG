/*
 * Add variables to BDT tree if they are not included in MakeBDTinput_*.cc.
 * Read both event tree & BDT tree, and the original BDT tree will be overwrite.
 *
 * Usage:
 *   ./AddBdtVar <file_event> <file_BDT>
 */

#include "TString.h"
#include "TTree.h"
#include "TFile.h"
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fevt_name(argv[1]);
    TString fbdt_name(argv[2]);

    // Read input files
    TFile *fevt = new TFile(fevt_name);
    TTree *Tevt = (TTree*)fevt->Get("T");
    TFile *fbdt = new TFile(fbdt_name, "update");
    TTree *Tbdt = (TTree*)fbdt->Get("T");

    // Set input tree
    float dipho_mass = 0;
    Tevt->SetBranchAddress("DiPhoInfo.mass", &dipho_mass);
    // Clone new BDT tree
    TTree *Tbdt_ = Tbdt->CloneTree(0);
    Tbdt_->Branch("dipho_mass", &dipho_mass);

    // Start event loop
    for (int evt=0; evt<Tevt->GetEntries(); ++evt)
    {
        Tevt->GetEntry(evt);
        Tbdt->GetEntry(evt);
        Tbdt_->Fill();
    } // End event loop

    fbdt->WriteTObject(Tbdt_, "", "Overwrite");
    fbdt->Close();
    fevt->Close();

    return 0;
}
