/*
 * Add tree variables.
 * Take variables from main event tree and add them to BDT variable tree.
 * New BDT tree will overwrite the old one.
 * Currently added: Mgg
 *
 * Usage:
 *   ./AddTreeVar <fin_main> <fin_BDT>
 */

#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include <iostream>
#include <cstdlib>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fin1_name = argv[1];
    TString fin2_name = argv[2];
    // Get input files
    cout << "[INFO] Main: " << fin1_name << endl;
    cout << "[INFO] BDT: " << fin2_name << endl;
    TFile *fin1 = new TFile(fin1_name);
    TFile *fin2 = new TFile(fin2_name, "update");
    TTree *T1 = (TTree*)fin1->Get("T");
    TTree *T2 = (TTree*)fin2->Get("T");
    // Set main tree branches
    float DiPhoInfo_mass = 0;
    T1->SetBranchStatus("*", 0);
    T1->SetBranchStatus("DiPhoInfo.mass", 1);
    T1->SetBranchAddress("DiPhoInfo.mass", &DiPhoInfo_mass);

    if (T1->GetEntries() != T2->GetEntries()) {
        cout << "[ERROR] Event number problem\n";
        exit(1);
    }

    // Set new tree
    TTree *T2_ = T2->CloneTree(0);
    T2_->Branch("Mgg", &DiPhoInfo_mass);

    // Start event loop
    for (int evt=0; evt<T1->GetEntries(); ++evt) {
        T1->GetEntry(evt);
        T2->GetEntry(evt);
        T2_->Fill();
    } // End of event loop

    // Save result
    fin2->WriteTObject(T2_, "", "Overwrite");
    fin2->Close();
    fin1->Close();
    cout << "[INFO] Complete!\n";

    return 0;
}
