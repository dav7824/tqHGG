/*
 * Divide trees using tree event index.
 *
 * Usage:
 *   ./DivideSamples_v4 <fin> <vec_tree> <vec_fout>
 */

#include "include/utility.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char **argv)
{
    // Command line arguments
    TString fin_name = argv[1];
    vector<TString> treename;  ParseCStringList(argv[2], treename);
    vector<TString> fout_name;  ParseCStringList(argv[3], fout_name);
    int ntree = treename.size();
    int nfout = fout_name.size();

    // Get input tree
    cout << "[INFO] Openning file: " << fin_name << endl;
    TFile *fin = new TFile(fin_name);
    vector<TTree*> Tin;
    for (int i=0; i<ntree; ++i)  Tin.push_back( (TTree*)fin->Get(treename[i]) );

    // Create new trees
    vector<TFile*> fout(nfout);
    vector<vector<TTree*>> Tout(nfout);
    for (int ifout=0; ifout<nfout; ++ifout) {
        fout[ifout] = new TFile(fout_name[ifout], "recreate");
        vector<TTree*> Tout_(ntree);
        for (int itree=0; itree<ntree; ++itree) Tout_[itree] = Tin[itree]->CloneTree(0);
        Tout[ifout] = Tout_;
    }

    // Start event loop
    for (int evt=0; evt<Tin[0]->GetEntries(); ++evt)
    {
        for (int itree=0; itree<ntree; ++itree) Tin[itree]->GetEntry(evt);

        int ifout = evt % nfout;
        for (int itree=0; itree<ntree; ++itree) Tout[ifout][itree]->Fill();
    }
    // end of event loop

    // Save new trees
    for (int ifout = 0; ifout<nfout; ++ifout) {
        cout << "[INFO] Saved output file " << ifout << ": " << fout_name[ifout] << "  (" << Tout[ifout][0]->GetEntries() << ")" << endl;
        for (int itree=0; itree<ntree; ++itree)
            fout[ifout]->WriteTObject(Tout[ifout][itree]);
        fout[ifout]->Close();
    }
    fin->Close();

    return 0;
}
