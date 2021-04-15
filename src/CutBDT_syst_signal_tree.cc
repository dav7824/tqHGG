/*
 * Calculate signal yields after BDT cut, for given channel, coupling, and
 * systematic uncertainty (both upward & downward variation).
 * Diphoton mass is required to be between [100,180]. Only events wtih evt_no
 * % 3 == 2 are saved and the weights are scaled by 3.
 * Results are appended to output text file.
 *
 * Usage:
 *   ./CutBDT_syst_signal_tree <indir> <coupling=hut|hct> <channel=had|lep> <syst_unc> <fout>
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
// Hut signal
vector<TString> fin_Hut_TT_had = {
    "TT_FCNC-TtoHJ_aThad_hut",
    "TT_FCNC-aTtoHJ_Thad_hut",
};
vector<TString> fin_Hut_ST_had = {
    "ST_FCNC-TH_Thad_hut",
};
vector<TString> fin_Hut_TT_lep = {
    "TT_FCNC-TtoHJ_aTlep_hut",
    "TT_FCNC-aTtoHJ_Tlep_hut",
};
vector<TString> fin_Hut_ST_lep = {
    "ST_FCNC-TH_Tlep_hut",
};
// Hct signal
vector<TString> fin_Hct_TT_had = {
    "TT_FCNC-TtoHJ_aThad_hct",
    "TT_FCNC-aTtoHJ_Thad_hct",
};
vector<TString> fin_Hct_ST_had = {
    "ST_FCNC-TH_Thad_hct",
};
vector<TString> fin_Hct_TT_lep = {
    "TT_FCNC-TtoHJ_aTlep_hct",
    "TT_FCNC-aTtoHJ_Tlep_hct",
};
vector<TString> fin_Hct_ST_lep = {
    "ST_FCNC-TH_Tlep_hct",
};

// Sum of central weights without systematics
float cenwei_Hut_TT_had = 0.633;
float cenwei_Hut_ST_had = 0.102;
float cenwei_Hut_TT_lep = 1.055;
float cenwei_Hut_ST_lep = 0.194;
float cenwei_Hct_TT_had = 0.560;
float cenwei_Hct_ST_had = 0.018;
float cenwei_Hct_TT_lep = 0.959;
float cenwei_Hct_ST_lep = 0.034;

// BDT score cuts
float BDT_cut_had = 0.318;
float BDT_cut_lep = 0.1756;


float ProcessTree(TString dir, vector<TString> &fins, TString ch)
{
    // Read file
    TChain T("T");
    for (int i=0; i<fins.size(); ++i)  T.Add( dir+fins.at(i)+".root" );

    // Tree variables
    int evt_no = 0;
    int evtwei_scale = 0;
    float dipho_mass = 0;
    float BDT_score = 0;
    float evtwei_central = 0;
    // Set branches
    T.SetBranchAddress("evt_no", &evt_no);
    T.SetBranchAddress("evtwei_scale", &evtwei_scale);
    T.SetBranchAddress("dipho_mass", &dipho_mass);
    T.SetBranchAddress("BDT_score", &BDT_score);
    T.SetBranchAddress("evtwei_central", &evtwei_central);

    float BDT_cut = 0;
    if (ch=="had")  BDT_cut = BDT_cut_had;
    else BDT_cut = BDT_cut_lep;

    // Start event loop
    float sumwei = 0;
    for (int evt=0; evt<T.GetEntries(); ++evt) {
        T.GetEntry(evt);

        if (dipho_mass < 100 || dipho_mass > 180)  continue;
        if (evt_no % 3 != 2)  continue;
        if (BDT_score < BDT_cut)  continue;

        evtwei_central *= evtwei_scale;

        sumwei += evtwei_central;
    } // End of event loop

    return sumwei;
} // End of function ProcessTree


int main(int argc, char **argv)
{
    // Get command line arguments
    TString indir_name(argv[1]);
    TString coupling(argv[2]);
    TString channel(argv[3]);
    TString syst_name(argv[4]);
    TString fout_name(argv[5]);
    // Set directory names
    TString treedir[2];  // Directories containing systematic root files
    treedir[0] = indir_name + '/' + syst_name + "Up/";  // 0th: upward variation tree directory
    treedir[1] = indir_name + '/' + syst_name + "Down/";  // 1st: downward variation tree directory
    for (int i=0; i<2; ++i)
        if (gSystem->AccessPathName(treedir[i])) {
            cout << "[ERROR] Input dir does not exist\n";
            exit(1);
        }
    // Set sample names
    vector<TString> *fin_TT, *fin_ST;
    if (coupling == "hut" && channel == "had") {
        fin_TT = &fin_Hut_TT_had;
        fin_ST = &fin_Hut_ST_had;
    } else if (coupling == "hut" && channel == "lep") {
        fin_TT = &fin_Hut_TT_lep;
        fin_ST = &fin_Hut_ST_lep;
    } else if (coupling == "hct" && channel == "had") {
        fin_TT = &fin_Hct_TT_had;
        fin_ST = &fin_Hct_ST_had;
    } else if (coupling == "hct" && channel == "lep") {
        fin_TT = &fin_Hct_TT_lep;
        fin_ST = &fin_Hct_ST_lep;
    } else {
        cout << "[ERROR] Invalid coupling or channel\n";
        exit(1);
    }

    // Process trees
    float sumwei_TT[2], sumwei_ST[2];  // 0th: upward syst, 1st: downward syst
    sumwei_TT[0] = ProcessTree(treedir[0], *fin_TT, channel);  // TT, upward
    sumwei_TT[1] = ProcessTree(treedir[1], *fin_TT, channel);  // TT, downward
    sumwei_ST[0] = ProcessTree(treedir[0], *fin_ST, channel);  // ST, upward
    sumwei_ST[1] = ProcessTree(treedir[1], *fin_ST, channel);  // ST, downward

    // Divide systematic yields by central yields
    if (coupling == "hut" && channel == "had") {
        for (int i=0; i<2; ++i) {
            sumwei_TT[i] /= cenwei_Hut_TT_had;
            sumwei_ST[i] /= cenwei_Hut_ST_had;
        }
    } else if (coupling == "hut" && channel == "lep") {
        for (int i=0; i<2; ++i) {
            sumwei_TT[i] /= cenwei_Hut_TT_lep;
            sumwei_ST[i] /= cenwei_Hut_ST_lep;
        }
    } else if (coupling == "hct" && channel == "had") {
        for (int i=0; i<2; ++i) {
            sumwei_TT[i] /= cenwei_Hct_TT_had;
            sumwei_ST[i] /= cenwei_Hct_ST_had;
        }
    } else {
        for (int i=0; i<2; ++i) {
            sumwei_TT[i] /= cenwei_Hct_TT_lep;
            sumwei_ST[i] /= cenwei_Hct_ST_lep;
        }
    }

    // Print result to text file
    FILE *fout = fopen( fout_name.Data(), "a" );
    fprintf( fout, "%25s  lnN  %.3f/%.3f  %.3f/%.3f\n",
            syst_name.Data(), sumwei_TT[1], sumwei_TT[0], sumwei_ST[1], sumwei_ST[0] );
    fclose(fout);

    return 0;
}
