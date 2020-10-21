/*
 *  Calculate event yield of each dataset.
 *
 *  Usage:
 *    ./CalcYield <fin_list> <tree_list> <fnorm_name> <fout_name> <SF_flags>
 */

#include "include/utility.h"
#include "include/SFhelper.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TString.h"

#include <vector>
#include <bitset>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    char *fin_list = argv[1];
    char *tree_list = argv[2];
    TString fnorm_name = argv[3];
    TString fout_name = argv[4];
    bitset<SFhelper::Nsf> SF_flags(argv[5]);

    vector<TString> fins;
    vector<TString> trees_tmp;
    vector<vector<TString>> trees;
    ParseCStringList(fin_list, fins);
    ParseCStringList(tree_list, trees_tmp);
    trees.resize(fins.size());
    for (int i=0; i<fins.size(); ++i)
        ParseCStringList(trees_tmp[i].Data(), trees[i], '+');

    // Read input trees
    TFile *fin = new TFile(fins[0]);
    TTree *Tin = (TTree*)fin->Get(trees[0][0]);
    for (int i=1; i<trees[0].size(); ++i)
        Tin->AddFriend(trees[0][i].Data());
    for (int i=1; i<fins.size(); ++i)
        for (int j=0; j<trees[i].size(); ++j)
            Tin->AddFriend(trees[i][j].Data(), fins[i].Data());

    // If no event in the tree, save yield = 0 and end
    if (Tin->GetEntries() == 0) {
        cout << "No entry in the tree! Save yield as 0\n";
        TFile *fout = new TFile(fout_name, "recreate");
        TH1D *h_yield = new TH1D("yield", "", 1, 0, 1);  h_yield->SetBinContent(1, 0);
        TH1D *h_yield2 = new TH1D("yield2", "", 1, 0, 1);  h_yield2->SetBinContent(1, 0);
        fout->Write();
        fout->Close();
        fin->Close();

        return 0;
    }

    Tin->SetBranchStatus("*", 0);
    SFhelper SFsource(SF_flags);
    SFsource.SetTreeBranches(Tin);

    float weight = 0;
    float sumweight = 0;
    float sumweight2 = 0;

    // Sum all event weight
    for (int evt=0; evt<Tin->GetEntries(); ++evt) {
        Tin->GetEntry(evt);
        weight = SFsource.GetWeight();
        sumweight += weight;
        sumweight2 += weight * weight;
    } // End of event loop

    // Yield normalization
    float nf = GetNormFactor(fnorm_name.Data());
    float yield = sumweight * nf;
    float yield2 = sumweight2 * nf * nf;

    // Save results
    TFile *fout = new TFile(fout_name, "recreate");
    TH1D *h_yield = new TH1D("yield", "", 1, 0, 1);  h_yield->SetBinContent(1, yield);
    TH1D *h_yield2 = new TH1D("yield2", "", 1, 0, 1);  h_yield2->SetBinContent(1, yield2);
    fout->Write();
    fout->Close();
    fin->Close();

    return 0;
}
