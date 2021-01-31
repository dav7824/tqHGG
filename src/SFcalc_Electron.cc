/*
 * Usage:
 *   ./SFcalc_Electron <fin> <ch=had|lep> <SF_reco0> <SF_reco1> <SF_ID> <makeSyst=syst|nosyst>
 */

#include "TFile.h"
#include "TH2F.h"
#include "TTree.h"
#include "TString.h"
#include "TLorentzVector.h"
#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fin_name = argv[1];
    TString ch = argv[2];
    TString fSF_reco0_name = argv[3]; // SF file for electron reco (ET < 20 GeV)
    TString fSF_reco1_name = argv[4]; // SF file for electron reco (ET > 20 GeV)
    TString fSF_id_name = argv[5]; // SF file for electron ID
    TString makeSyst_ = argv[6];
    if (ch!="had" && ch!="lep") {
        cout << "[ERROR] Invalid channel\n";
        exit(1);
    }
    bool makeSyst;
    if (makeSyst_ == "syst") makeSyst = true;
    else makeSyst = false;

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
    TFile *fin = new TFile(fin_name, "update");
    TTree *Tin = (TTree*)fin->Get("T");
    // Used variables
    int Elec_Size = 0;
    vector<float> *Elec_Pt = 0;
    vector<float> *Elec_Eta = 0;
    vector<float> *Elec_Phi = 0;
    vector<float> *Elec_Energy = 0;
    // Set branches
    Tin->SetBranchAddress("ElecInfo.Size", &Elec_Size);
    Tin->SetBranchAddress("ElecInfo.Pt", &Elec_Pt);
    Tin->SetBranchAddress("ElecInfo.Eta", &Elec_Eta);
    Tin->SetBranchAddress("ElecInfo.Phi", &Elec_Phi);
    Tin->SetBranchAddress("ElecInfo.Energy", &Elec_Energy);

    TTree *Tout = Tin->CloneTree(0);
    // New variables
    float SF_Elec = -999;
    float SF_Elec_RecoUp = -999;
    float SF_Elec_RecoDown = -999;
    float SF_Elec_IdUp = -999;
    float SF_Elec_IdDown = -999;
    // Set branches
    Tout->Branch("SF_Elec", &SF_Elec);
    Tout->Branch("SF_Elec_RecoUp", &SF_Elec_RecoUp);
    Tout->Branch("SF_Elec_RecoDown", &SF_Elec_RecoDown);
    Tout->Branch("SF_Elec_IdUp", &SF_Elec_IdUp);
    Tout->Branch("SF_Elec_IdDown", &SF_Elec_IdDown);

    TH2F *hSF_reco = 0;
    TLorentzVector electron_vec;

    // Start event loop
    for (int evt=0; evt<Tin->GetEntries(); ++evt)
    {
        Tin->GetEntry(evt);

        SF_Elec = 1;
        if (makeSyst) {
            SF_Elec_RecoUp = 1;
            SF_Elec_RecoDown = 1;
            SF_Elec_IdUp = 1;
            SF_Elec_IdDown = 1;
        }

        if (ch == "had") {
            Tout->Fill();
            continue;
        }

        // Calculate SF for each electron
        // Start electron loop
        for (int i=0; i<Elec_Size; ++i) {
            // If the electron is out of range, set SF as 1
            if (Elec_Pt->at(i) <= 10 || Elec_Pt->at(i) >= 500) continue;
            // Get SF
            electron_vec.SetPtEtaPhiE(Elec_Pt->at(i), Elec_Eta->at(i), Elec_Phi->at(i), Elec_Energy->at(i));
            if (electron_vec.Et() < 20) hSF_reco = hSF_reco0;
            else hSF_reco = hSF_reco1;

            int ibin_reco = hSF_reco->FindBin(Elec_Eta->at(i), Elec_Pt->at(i));
            int ibin_id = hSF_id->FindBin(Elec_Eta->at(i), Elec_Pt->at(i));
            float scale_reco = hSF_reco->GetBinContent( ibin_reco );
            float scale_id = hSF_id->GetBinContent( ibin_id );

            SF_Elec *= scale_reco * scale_id;
            if (makeSyst) {
                float scale_reco_err = hSF_reco->GetBinError( ibin_reco );
                float scale_id_err = hSF_id->GetBinError( ibin_id );
                SF_Elec_RecoUp *= (scale_reco + scale_reco_err) * scale_id;
                SF_Elec_RecoDown *= (scale_reco - scale_reco_err) * scale_id;
                SF_Elec_IdUp *= scale_reco * (scale_id + scale_id_err);
                SF_Elec_IdDown *= scale_reco * (scale_id - scale_id_err);
            }
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

    return 0;
}
/*
 * Electron reco low ET:
 *   Pt: 10~20 GeV
 *   Eta: -2.5~2.5
 * Electron reco high ET:
 *   Pt: 20~500 GeV
 *   Eta: -2.5~2.5
 * Electron ID:
 *   Pt: 10~500 GeV
 *   Eta: -2.5~2.5
 */
