/*
 * Select events in photon MVA ID sideband, for data-driven description of QCD in hadronic channel.
 *
 * Usage:
 *   ./Presel_had_phIDSB <fin> <tree> <fout>
 *
 * Requirements:
 * 1. IDMVA_max>-0.7 and -0.7>=IDMVA_min>-0.9
 * 2. At least 1 b-tagged jet (DeepCSV loose ID)
 */

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include <iostream>
#include <vector>
using namespace std;

float btag_cut = 0.1522;  // 2017 loose WP

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fin_name(argv[1]);
    TString inTree_name(argv[2]);
    TString fout_name(argv[3]);

    // Get input tree
    cout << "[INFO] Open file: " << fin_name << endl;
    TChain *inTree = new TChain(inTree_name);
    inTree->Add(fin_name);
    // Set branches
    float DiPho_leadIDMVA = 0;
    float DiPho_subleadIDMVA = 0;
    int jets_size = 0;
    vector<float> *Jet_probb = 0;
    vector<float> *Jet_probbb = 0;
    inTree->SetBranchAddress("DiPhoInfo.leadIDMVA", &DiPho_leadIDMVA);
    inTree->SetBranchAddress("DiPhoInfo.subleadIDMVA", &DiPho_subleadIDMVA);
    inTree->SetBranchAddress("jets_size", &jets_size);
    inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probb", &Jet_probb);
    inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probbb", &Jet_probbb);

    // Create output tree
    TFile *fout = new TFile(fout_name, "recreate");
    TTree *outTree = inTree->CloneTree(0);

    // Start event loop
    for (int evt=0; evt<inTree->GetEntries(); ++evt)
    {
        inTree->GetEntry(evt);

        float idmva_max, idmva_min;
        if (DiPho_leadIDMVA > DiPho_subleadIDMVA) {
            idmva_max = DiPho_leadIDMVA;
            idmva_min = DiPho_subleadIDMVA;
        } else {
            idmva_max = DiPho_subleadIDMVA;
            idmva_min = DiPho_leadIDMVA;
        }

        // Select events in photon MVA ID sideband
        if (!(idmva_max > -0.7 && -0.7 >= idmva_min && idmva_min > -0.9))
            continue;

        // Require at least 1 b-tagged jet
        for (int i=0; i<jets_size; ++i)
            if (Jet_probb->at(i) + Jet_probbb->at(i) > btag_cut) {
                outTree->Fill();
                break;
            }
    } // End event loop

    fout->Write();
    fout->Close();
    cout << "[INFO] Save result: " << fout_name << endl;

    return 0;
}
