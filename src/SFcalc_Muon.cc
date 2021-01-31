/*
 * Usage:
 *   ./SFcalc_Muon <fin> <ch=had|lep> <SF_ID> <SF_ISO> <makeSyst=syst|nosyst>
 */

#include "TFile.h"
#include "TH2D.h"
#include "TTree.h"
#include "TString.h"
#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fin_name = argv[1];
    TString ch = argv[2];
    TString fSF_ID_name = argv[3];
    TString fSF_ISO_name = argv[4];
    TString makeSyst_ = argv[5];
    if (ch!="had" && ch!="lep") {
        cout << "[ERROR] Invalid channel\n";
        exit(1);
    }
    bool makeSyst;
    if (makeSyst_ == "syst") makeSyst = true;
    else makeSyst = false;

    // SF file for muon ID
    TFile *fSF_ID = new TFile(fSF_ID_name);
    TH2D *hSF_ID = (TH2D*)fSF_ID->Get("NUM_MediumID_DEN_genTracks_pt_abseta");
    // SF file for muon ISO
    TFile *fSF_ISO = new TFile(fSF_ISO_name);
    TH2D *hSF_ISO = (TH2D*)fSF_ISO->Get("NUM_LooseRelIso_DEN_MediumID_pt_abseta");

    // Read input file
    TFile *fin = new TFile(fin_name, "update");
    TTree *Tin = (TTree*)fin->Get("T");
    // Used variables
    int Muon_Size = 0;
    vector<float> *Muon_Pt = 0;
    vector<float> *Muon_Eta = 0;
    vector<float> *Muon_Phi = 0;
    vector<float> *Muon_Energy = 0;
    // Set branches
    Tin->SetBranchAddress("MuonInfo.Size", &Muon_Size);
    Tin->SetBranchAddress("MuonInfo.Pt", &Muon_Pt);
    Tin->SetBranchAddress("MuonInfo.Eta", &Muon_Eta);
    Tin->SetBranchAddress("MuonInfo.Phi", &Muon_Phi);
    Tin->SetBranchAddress("MuonInfo.Energy", &Muon_Energy);

    TTree *Tout = Tin->CloneTree(0);
    // New variables
    float SF_Muon = -999;
    float SF_Muon_IdUp = -999;
    float SF_Muon_IdDown = -999;
    float SF_Muon_IsoUp = -999;
    float SF_Muon_IsoDown = -999;
    // Set branches
    Tout->Branch("SF_Muon", &SF_Muon);
    Tout->Branch("SF_Muon_IdUp", &SF_Muon_IdUp);
    Tout->Branch("SF_Muon_IdDown", &SF_Muon_IdDown);
    Tout->Branch("SF_Muon_IsoUp", &SF_Muon_IsoUp);
    Tout->Branch("SF_Muon_IsoDown", &SF_Muon_IsoDown);

    // Start event loop
    for (int evt=0; evt<Tin->GetEntries(); ++evt)
    {
        Tin->GetEntry(evt);

        SF_Muon = 1;
        if (makeSyst) {
            SF_Muon_IdUp = 1;
            SF_Muon_IdDown = 1;
            SF_Muon_IsoUp = 1;
            SF_Muon_IsoDown = 1;
        }

        if (ch == "had") {
            Tout->Fill();
            continue;
        }

        // Calculate SF for each muon
        // Start muon loop
        for (int i=0; i<Muon_Size; ++i) {
            // If the muon has Pt in these ranges, take its SF as 1.  (It's outside the SF histograms)
            if (Muon_Pt->at(i) <= 20 || Muon_Pt->at(i) >= 120) continue;

            // Get SF
            int ibin_id = hSF_ID->FindBin(Muon_Pt->at(i), fabs(Muon_Eta->at(i)));
            int ibin_iso = hSF_ISO->FindBin(Muon_Pt->at(i), fabs(Muon_Eta->at(i)));
            float scale_id = hSF_ID->GetBinContent( ibin_id );
            float scale_iso = hSF_ISO->GetBinContent( ibin_iso );

            SF_Muon *= scale_id * scale_iso;
            if (makeSyst) {
                float scale_id_err = hSF_ID->GetBinError( ibin_id );
                float scale_iso_err = hSF_ISO->GetBinError( ibin_iso );
                SF_Muon_IdUp *= (scale_id + scale_id_err) * scale_iso;
                SF_Muon_IdDown *= (scale_id - scale_id_err) * scale_iso;
                SF_Muon_IsoUp *= scale_id * (scale_iso + scale_iso_err);
                SF_Muon_IsoDown *= scale_id * (scale_iso - scale_iso_err);
            }
        }
        // End muon loop

        Tout->Fill();
    }
    // End event loop

    fin->WriteTObject(Tout, "", "Overwrite");
    fin->Close();
    fSF_ID->Close();
    fSF_ISO->Close();

    return 0;
}
