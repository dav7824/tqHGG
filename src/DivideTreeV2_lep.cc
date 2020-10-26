/*
 * Get test events (of MVA reconstruction) in the original event tree, for signal MC.
 * The V2 code also divides the SF trees.
 * Usage:
 *   ./DivideTree <fevt_in> <fevt_out> <fperm>
 *
 * <fevt_in>:  Input file with original event & SF trees
 * <fevt_out>: Output file with only MVA reco test events
 * <fperm>:    File with permutation tree
 */

#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fevt_in_name = argv[1];
    TString fevt_out_name = argv[2];
    TString fperm_name = argv[3];

    // Get input event file
    cout << "[INFO] Get input event file: " << fevt_in_name << endl;
    TFile *fevt_in = new TFile(fevt_in_name);
    TTree *Tevt_in = (TTree*)fevt_in->Get("T");
    TTree *Tpu_in = (TTree*)fevt_in->Get("SF_pileup");
    TTree *Tbtag_in = (TTree*)fevt_in->Get("SF_btag");
    TTree *Telec_in = (TTree*)fevt_in->Get("SF_Elec");
    TTree *Tmuon_in = (TTree*)fevt_in->Get("SF_Muon");
    // Get perm file
    cout << "[INFO] Get permutation file: " << fperm_name << endl;
    TFile *fperm = new TFile(fperm_name);
    TTree *Tperm = (TTree*)fperm->Get("TPerm_test");
    // Get output event file
    TFile *fevt_out = new TFile(fevt_out_name, "recreate");
    TTree *Tevt_out = Tevt_in->CloneTree(0);
    TTree *Tpu_out = Tpu_in->CloneTree(0);
    TTree *Tbtag_out = Tbtag_in->CloneTree(0);
    TTree *Telec_out = Telec_in->CloneTree(0);
    TTree *Tmuon_out = Tmuon_in->CloneTree(0);

    // Get the starting event index
    int Evt_idx = 0;
    Tperm->SetBranchAddress("Evt_idx", &Evt_idx);
    Tperm->GetEntry(0);
    int start_idx = Evt_idx;

    // Copy events to new tree
    for (int evt=start_idx; evt<Tevt_in->GetEntries(); ++evt)
    {
        Tevt_in->GetEntry(evt);
        Tpu_in->GetEntry(evt);
        Tbtag_in->GetEntry(evt);
        Telec_in->GetEntry(evt);
        Tmuon_in->GetEntry(evt);

        Tevt_out->Fill();
        Tpu_out->Fill();
        Tbtag_out->Fill();
        Telec_out->Fill();
        Tmuon_out->Fill();
    }

    fevt_out->Write();
    fevt_out->Close();
    fperm->Close();
    fevt_in->Close();

    cout << "[INFO] Save output file: " << fevt_out_name << endl;

    return 0;
}
