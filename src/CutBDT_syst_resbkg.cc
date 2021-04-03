/*
 * Calculate resonant bkg yields after BDT cut, for given channel and
 * systematic uncertainty (both upward & downward variation).
 * Diphoton mass is required to be between [100,180]. Only events wtih evt_no
 * % 3 == 2 are saved and the weights are scaled by 3.
 * Results are appended to output text file.
 *
 * Usage:
 *   ./CutBDT_syst_resbkg <indir> <channel=had|lep> <syst_unc> <fout>
 */

#include "TSystem.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
using namespace std;

// Input file names
vector<TString> fins = {
    "GluGluHToGG", "VBFHToGG", "VHToGG", "ttHJetToGG"
};

// Sum of central weights without systematics
float cenwei_had = 0.818;
float cenwei_lep = 1.008;

// BDT score cuts
float BDT_cut_had = 0.318;
float BDT_cut_lep = 0.1756;


void ProcessTree(TTree *T, float *sumwei, TString ch, TString syst_name)
{
    // Tree variables
    int evt_no = 0;
    int evtwei_scale = 0;
    float dipho_mass = 0;
    float BDT_score = 0;
    float evtwei_up = 0;
    float evtwei_down = 0;
    TString branch_up = "evtwei_"+syst_name+"Up";
    TString branch_down = "evtwei_"+syst_name+"Down";
    // Set branches
    T->SetBranchAddress("evt_no", &evt_no);
    T->SetBranchAddress("evtwei_scale", &evtwei_scale);
    T->SetBranchAddress("dipho_mass", &dipho_mass);
    T->SetBranchAddress("BDT_score", &BDT_score);
    T->SetBranchAddress(branch_up, &evtwei_up);
    T->SetBranchAddress(branch_down, &evtwei_down);

    float BDT_cut = 0;
    if (ch=="had")  BDT_cut = BDT_cut_had;
    else BDT_cut = BDT_cut_lep;

    // Start event loop
    sumwei[0] = 0;
    sumwei[1] = 0;
    for (int evt=0; evt<T->GetEntries(); ++evt) {
        T->GetEntry(evt);

        if (dipho_mass < 100 || dipho_mass > 180)  continue;
        if (evt_no % 3 != 2)  continue;
        if (BDT_score < BDT_cut)  continue;

        evtwei_up *= evtwei_scale;
        evtwei_down *= evtwei_scale;

        sumwei[0] += evtwei_up; // 0th saves upward syst
        sumwei[1] += evtwei_down; // 1st saves downward syst
    } // End of event loop
} // End of function ProcessTree


int main(int argc, char **argv)
{
    // Get command line arguments
    TString indir_name(argv[1]);
    TString channel(argv[2]);
    TString syst_name(argv[3]);
    TString fout_name(argv[4]);
    // Set file names
    if (gSystem->AccessPathName(indir_name)) {
        cout << "[ERROR] Input dir does not exist\n";
        exit(1);
    }
    if (channel != "had" && channel != "lep") {
        cout << "[ERROR] Invalid channel\n";
        exit(1);
    }

    // Read input file
    TChain *T = new TChain("T");
    for (int i=0; i<fins.size(); ++i)
        T->Add( indir_name + '/' + fins.at(i) + ".root" );

    // Process trees
    float sumwei[2];
    ProcessTree( T, sumwei, channel, syst_name );
    // Divide systematic yields by central yields
    if (channel == "had") {
        sumwei[0] /= cenwei_had;
        sumwei[1] /= cenwei_had;
    } else {
        sumwei[0] /= cenwei_lep;
        sumwei[1] /= cenwei_lep;
    }

    // Print result to text file
    FILE *fout = fopen( fout_name.Data(), "a" );
    fprintf( fout, "%25s  lnN  %.3f/%.3f\n",
            syst_name.Data(), sumwei[1], sumwei[0] );
    fclose(fout);

    return 0;
}
