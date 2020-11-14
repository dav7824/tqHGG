#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fin_name = argv[1];

    // Read file
    cout << "[INFO] Read file: " << fin_name << endl;
    TFile *fin = new TFile(fin_name, "update");
    TTree *T = (TTree*)fin->Get("T");
    float genweight = 0;
    float Nscale = 0;
    float norm = 0;
    float SF_pileup = 0;
    float SF_btag = 0;
    float SF_photon = 0;
    float SF_ElecReco = 0;
    float SF_ElecID = 0;
    float SF_MuonID = 0;
    float SF_MuonIso = 0;
    T->SetBranchAddress("genweight", &genweight);
    T->SetBranchAddress("Nscale", &Nscale);
    T->SetBranchAddress("norm", &norm);
    T->SetBranchAddress("SF_pileup", &SF_pileup);
    T->SetBranchAddress("SF_btag", &SF_btag);
    T->SetBranchAddress("SF_photon", &SF_photon);
    T->SetBranchAddress("SF_ElecReco", &SF_ElecReco);
    T->SetBranchAddress("SF_ElecID", &SF_ElecID);
    T->SetBranchAddress("SF_MuonID", &SF_MuonID);
    T->SetBranchAddress("SF_MuonIso", &SF_MuonIso);
    // New tree
    float evt_weight = 0;
    TTree *T_ = T->CloneTree(0);
    T_->Branch("evt_weight", &evt_weight);

    // Event loop
    for (int evt=0; evt<T->GetEntries(); ++evt)
    {
        T->GetEntry(evt);

        evt_weight = genweight * Nscale * norm * SF_pileup * SF_btag * SF_photon *
            SF_ElecReco * SF_ElecID * SF_MuonID * SF_MuonIso;
        T_->Fill();
    }

    fin->WriteTObject(T_, "", "Overwrite");
    fin->Close();
    cout << "[INFO] Complete!\n";

    return 0;
}
