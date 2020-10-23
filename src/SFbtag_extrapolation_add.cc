/*
 * Add b-tagging extrapolation factor to the scale factor.
 * This code directly modifies the b-tag SF branch of the input tree, by multiplying it by
 * the previously calculated extrapolation factor (by SFbtag_extrapolation.cc).
 *
 * Usage:
 *   ./SFbtag_extrapolation_add <fin> <file_ex_factor>
 */

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"

#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char **argv)
{
    // Command line arguments
    TString fin_name = argv[1];
    TString fhist_name = argv[2];

    // Get extrapolation factor
    TFile *fhist = new TFile(fhist_name);
    TH1D *hh = (TH1D*)fhist->Get("btag_factor");
    float ex_factor = hh->GetBinContent(1);
    fhist->Close();

    // Open SF tree file
    cout << "[INFO] Processing file: " << fin_name << endl;
    TFile *fin = new TFile(fin_name, "update");
    TTree *Tbtag = (TTree*)fin->Get("SF_btag");
    float SF_evt = 0;
    Tbtag->SetBranchAddress("SF_btag", &SF_evt);

    // Create new tree
    TTree *Tbtag_ = Tbtag->CloneTree(0);
    float SF_evt_ = 0;
    Tbtag_->Branch("extrapolation", &ex_factor);
    Tbtag_->SetBranchAddress("SF_btag", &SF_evt_);

    // Start event loop
    for (int evt=0; evt<Tbtag->GetEntries(); ++evt)
    {
        Tbtag->GetEntry(evt);

        SF_evt_ = SF_evt * ex_factor;

        Tbtag_->Fill();
    } // End event loop

    // Save new tree
    fin->WriteTObject(Tbtag_, "", "Overwrite");
    fin->Close();

    cout << "[INFO] Complete!\n";

    return 0;
}
