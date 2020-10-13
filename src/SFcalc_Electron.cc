#include "TFile.h"
#include "TH2F.h"
#include "TTree.h"
#include "TString.h"
#include "TLorentzVector.h"

#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fin_name = argv[1];
    TString fSF_reco0_name = argv[2]; // SF file for electron reco (ET < 20 GeV)
    TString fSF_reco1_name = argv[3]; // SF file for electron reco (ET > 20 GeV)
    TString fSF_id_name = argv[4]; // SF file for electron ID

    // SF file for electron reco (ET < 20 GeV)
    TFile *fSF_reco0 = new TFile(fSF_reco0_name);
    TH2F *hSF_reco0 = (TH2F*)fSF_reco0->Get("EGamma_SF2D");
    // SF file for electron reco (ET > 20 GeV)
    TFile *fSF_reco1 = new TFile(fSF_reco1_name);
    TH2F *hSF_reco1 = (TH2F*)fSF_reco1->Get("EGamma_SF2D");
    // SF file for electron ID
    TFile *fSF_id = new TFile(fSF_id_name);
    TH2F *hSF_id = (TH2F*)fSF_id->Get("EGamma_SF2D");

    // Read input file
    cout << "[INFO] Input file: " << fin_name << endl;
    TFile *fin = new TFile(fin_name, "update");
    TTree *Tin = (TTree*)fin->Get("T");
    // Input variables
    int Elec_Size = 0;
    vector<float> *Elec_Pt = 0;
    vector<float> *Elec_Eta = 0;
    vector<float> *Elec_Phi = 0;
    vector<float> *Elec_Energy = 0;
    // Set input branches
    Tin->SetBranchStatus("*", 0);
    Tin->SetBranchStatus("ElecInfo.Size", 1);
    Tin->SetBranchStatus("ElecInfo.Pt", 1);
    Tin->SetBranchStatus("ElecInfo.Eta", 1);
    Tin->SetBranchStatus("ElecInfo.Phi", 1);
    Tin->SetBranchStatus("ElecInfo.Energy", 1);
    Tin->SetBranchAddress("ElecInfo.Size", &Elec_Size);
    Tin->SetBranchAddress("ElecInfo.Pt", &Elec_Pt);
    Tin->SetBranchAddress("ElecInfo.Eta", &Elec_Eta);
    Tin->SetBranchAddress("ElecInfo.Phi", &Elec_Phi);
    Tin->SetBranchAddress("ElecInfo.Energy", &Elec_Energy);

    TTree *Tout = new TTree("SF_Elec", "");
    // Output variables
    float SF_Elec_Reco = 0;
    float SF_Elec_ID = 0;
    // Set output branches
    Tout->Branch("Reco", &SF_Elec_Reco);
    Tout->Branch("ID", &SF_Elec_ID);

    TH2F *hSF_reco = 0;
    TLorentzVector electron_vec;

    // Start event loop
    for (int evt=0; evt<Tin->GetEntries(); ++evt)
    {
        Tin->GetEntry(evt);

        SF_Elec_Reco = 1.;
        SF_Elec_ID = 1.;

        // Calculate SF for each electron
        // Start electron loop
        for (int i=0; i<Elec_Size; ++i) {
            electron_vec.SetPtEtaPhiE(Elec_Pt->at(i), Elec_Eta->at(i), Elec_Phi->at(i), Elec_Energy->at(i));
            if (electron_vec.Et() < 20) hSF_reco = hSF_reco0;
            else hSF_reco = hSF_reco1;

            SF_Elec_Reco *= hSF_reco->GetBinContent( hSF_reco->FindBin(Elec_Eta->at(i), Elec_Pt->at(i)) );
            SF_Elec_ID *= hSF_id->GetBinContent( hSF_id->FindBin(Elec_Eta->at(i), Elec_Pt->at(i)) );
        }
        // End electron loop

        Tout->Fill();
    }
    // End event loop

    fin->WriteTObject(Tout, "", "Overwrite");
    fin->Close();
    fSF_reco0->Close();
    fSF_reco1->Close();
    fSF_id->Close();

    cout << "[INFO] Calculated SF tree is saved in input file\n";

    return 0;
}
