/*
 * Divide a tree as 3 parts.
 *
 * Usage:
 *   ./DivideSamples_v2 <fin> <tree> <fout1> <fout2> <fout3>
 */

#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
    // Command line arguments
    TString fin_name = argv[1];
    TString treename = argv[2];
    TString fout1_name = argv[3];
    TString fout2_name = argv[4];
    TString fout3_name = argv[5];

    // Get input tree
    cout << "[INFO] Openning file: " << fin_name << endl;
    TFile *fin = new TFile(fin_name);
    TTree *Tin = (TTree*)fin->Get(treename);
    // Create new trees
    TFile *fout1 = new TFile(fout1_name, "update");
    TTree *Tout1 = Tin->CloneTree(0);
    TFile *fout2 = new TFile(fout2_name, "update");
    TTree *Tout2 = Tin->CloneTree(0);
    TFile *fout3 = new TFile(fout3_name, "update");
    TTree *Tout3 = Tin->CloneTree(0);

    // Start event loop
    for (int evt=0; evt<Tin->GetEntries(); ++evt)
    {
        Tin->GetEntry(evt);

        if (evt % 3 == 0) Tout1->Fill();
        else if (evt % 3 == 1) Tout2->Fill();
        else Tout3->Fill();
    }
    // end of event loop

    // Save new trees
    fout1->cd(); fout1->WriteTObject(Tout1, "", "Overwrite"); fout1->Close();
    fout2->cd(); fout2->WriteTObject(Tout2, "", "Overwrite"); fout2->Close();
    fout3->cd(); fout3->WriteTObject(Tout3, "", "Overwrite"); fout3->Close();
    fin->Close();

    cout << "[INFO] Save output file 1: " << fout1_name << endl;
    cout << "[INFO] Save output file 2: " << fout2_name << endl;
    cout << "[INFO] Save output file 3: " << fout3_name << endl;

    return 0;
}
