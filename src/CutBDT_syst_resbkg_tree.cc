/*
 * Calculate resonant bkg yields after BDT cut, for given channel and
 * systematic uncertainty (both upward & downward variation).
 * Diphoton mass is required to be between [100,180]. Only events wtih evt_no
 * % 3 == 2 are saved and the weights are scaled by 3.
 * Results are appended to output text file.
 *
 * Usage:
 *   ./CutBDT_syst_resbkg_tree <indir> <channel=had|lep> <syst_unc> <fout>
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
    TString channel(argv[2]);
    TString syst_name(argv[3]);
    TString fout_name(argv[4]);
    // Set directory names
    TString treedir[2];  // Directories containing systematic root files
    treedir[0] = indir_name + '/' + syst_name + "Up/";  // 0th: upward variation tree directory
    treedir[1] = indir_name + '/' + syst_name + "Down/";  // 1st: downward variation tree directory
    for (int i=0; i<2; ++i)
        if (gSystem->AccessPathName(treedir[i])) {
            cout << "[ERROR] Input dir does not exist\n";
            exit(1);
        }
    if (channel != "had" && channel != "lep") {
        cout << "[ERROR] Invalid channel\n";
        exit(1);
    }

    // Process trees
    float sumwei[2];
    sumwei[0] = ProcessTree(treedir[0], fins, channel);  // Upward syst
    sumwei[1] = ProcessTree(treedir[1], fins, channel);  // Downward syst
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
