/*
 * Make fitting samples from BDT input samples.
 * For MCs, only preserve events with evt_no%3==2.
 * BDT score is cut by optimized result.
 * Events are required to have 100 <= Mgg <= 180 GeV.
 * Event yield is calculated.
 *
 * Usage:
 *   ./CutBDTscore <fin> <fout> <channel> <isdata=MC|data>
 */

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fin_name(argv[1]);
    TString fout_name(argv[2]);
    TString ch(argv[3]);
    TString isdata_(argv[4]);
    bool isdata = false;
    if (isdata_ == "data")  isdata = true;

    // Set BDT cut
    float BDT_cut;
    if (ch == "had")  BDT_cut = 0.318;
    else if (ch == "lep")  BDT_cut = 0.1756;
    else {
        cout << "[ERROR] Invalid channel\n";
        exit(1);
    }

    // Read input file
    TFile *fin = new TFile(fin_name);
    TTree *Tin = (TTree*)fin->Get("T");
    // Input tree variables
    int evt_no = 0;
    int evtwei_scale = 0;
    float evtwei_central = 0;
    float BDT_score = 0;
    float dipho_mass = 0;
    // Set input tree branches
    Tin->SetBranchAddress("evt_no", &evt_no);
    Tin->SetBranchAddress("evtwei_scale", &evtwei_scale);
    Tin->SetBranchAddress("evtwei_central", &evtwei_central);
    Tin->SetBranchAddress("BDT_score", &BDT_score);
    Tin->SetBranchAddress("dipho_mass", &dipho_mass);

    // Create output file
    TFile *fout = new TFile(fout_name, "recreate");
    TTree *Tout = new TTree("T", "");
    // Set output tree branches
    Tout->Branch("evtwei_central", &evtwei_central);
    Tout->Branch("dipho_mass", &dipho_mass);

    // Set yield counter
    float tot_yield = 0;
    float yield_err = 0;

    // Start event loop
    for (int evt=0; evt<Tin->GetEntries(); ++evt) {
        Tin->GetEntry(evt);
        if (dipho_mass < 100 || dipho_mass > 180)  continue;
        if (!isdata && evt_no%3!=2)  continue;
        if (BDT_score < BDT_cut)  continue;

        evtwei_central *= evtwei_scale;

        tot_yield += evtwei_central;
        yield_err += evtwei_central * evtwei_central;

        Tout->Fill();
    } // End event loop

    Tout->Write();
    fout->Close();
    fin->Close();

    // Print event yield
    yield_err = sqrt(yield_err);
    cout << "[INFO] Event yield = " << tot_yield <<  " +- " << yield_err << endl;

    return 0;
}
