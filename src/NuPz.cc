/*
 * Given reconstructed leptonic signal, reconstruct Pz of neutrino.
 * Usage:
 *   ./NuPz <event_file> <reco_file> <reco_type=TT|ST> <channel=had|lep>
 *
 * The new tree containing nu Pz info will overwrite original tree in <reco_file>.
 */

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
using namespace std;

float W_mass = 80.379;
/* mu_mass = 0.1 GeV */
// Mass of muon and electron can be neglected in computation compared to W mass

float CalcNuPz( TLorentzVector &lep, float met_Pt, float met_Phi );

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fevt_name = argv[1];
    TString freco_name = argv[2];
    TString recotype = argv[3];
    TString ch = argv[4];
    if (recotype!="TT" && recotype!="ST") {
        cout << "[ERROR] Invalid reconstruction type\n";
        exit(1);
    }
    if (ch!="had" && ch!="lep") {
        cout << "[ERROR] Invalid channel\n";
        exit(1);
    }

    // Read event tree
    TFile *fevt = new TFile( fevt_name );
    TTree *Tevt = (TTree*)fevt->Get("T");
    // Read reconstruction tree
    TFile *freco = new TFile( freco_name, "update" );
    TString Treco_name;
    if (recotype=="TT") Treco_name = "Treco_TT";
    else Treco_name = "Treco_ST";
    TTree *Treco = (TTree*)freco->Get(Treco_name);
    TTree *Treco_ = Treco->CloneTree(0); // New tree with additional vars

    // Tree variables
    // Event tree
    vector<float> *Elec_Phi = 0;
    vector<float> *Elec_Energy = 0;
    vector<float> *Muon_Phi = 0;
    vector<float> *Muon_Energy = 0;
    float Met_Pt = 0;
    float Met_Phi = 0;
    // Reconstruction tree
    int Perm_N = 0;
    int lep_idx = 0;
    float lep_ID = 0;
    float lep_Pt = 0;
    float lep_Eta = 0;
    // Reconstruction tree additional
    float nu_Pz = -999;

    // Set tree branches
    // Event tree
    Tevt->SetBranchAddress("ElecInfo.Phi", &Elec_Phi);
    Tevt->SetBranchAddress("ElecInfo.Energy", &Elec_Energy);
    Tevt->SetBranchAddress("MuonInfo.Phi", &Muon_Phi);
    Tevt->SetBranchAddress("MuonInfo.Energy", &Muon_Energy);
    Tevt->SetBranchAddress("MetInfo.Pt", &Met_Pt);
    Tevt->SetBranchAddress("MetInfo.Phi", &Met_Phi);
    // Reconstruction tree
    Treco->SetBranchAddress("Perm_N", &Perm_N);
    Treco->SetBranchAddress("lep_idx", &lep_idx);
    Treco->SetBranchAddress("lep_ID", &lep_ID);
    Treco->SetBranchAddress("lep_Pt", &lep_Pt);
    Treco->SetBranchAddress("lep_Eta", &lep_Eta);
    // Reconstruction tree additional
    Treco_->Branch("nu_Pz", &nu_Pz);

    // For hadronic samples, save null values as nu Pz
    if (ch == "had") {
        for (int evt=0; evt<Treco->GetEntries(); ++evt) {
            Treco->GetEntry(evt);
            Treco_->Fill();
        }
        fevt->Close();
        freco->WriteTObject(Treco_, "", "Overwrite");
        freco->Close();

        return 0;
    }

    TLorentzVector lep;
    float lep_Phi = 0;
    float lep_E = 0;

    // Start event loop
    for (int evt=0; evt<Tevt->GetEntries(); ++evt)
    {
        Tevt->GetEntry(evt);
        Treco->GetEntry(evt);

        // Initialize output variables
        nu_Pz = -999;

        // For events that can't be reconstructed, fill null values to output
        if (Perm_N < 0) { Treco_->Fill();  continue; }

        if ( fabs(lep_ID)==11 ) { // electrons
            lep_Phi = Elec_Phi->at( lep_idx );
            lep_E = Elec_Energy->at( lep_idx );
        }
        else { // fabs(lep_ID) == 13 (muons)
            lep_Phi = Muon_Phi->at( lep_idx );
            lep_E = Muon_Energy->at( lep_idx );
        }
        lep.SetPtEtaPhiE( lep_Pt, lep_Eta, lep_Phi, lep_E );

        // Calculate neutrino Pz
        nu_Pz = CalcNuPz( lep, Met_Pt, Met_Phi );

        Treco_->Fill();
    } // End of event loop

    fevt->Close();
    freco->WriteTObject(Treco_, "", "Overwrite");
    freco->Close();

    return 0;
}

float CalcNuPz(TLorentzVector &lep, float met_Pt, float met_Phi)
{
    float met_Px = met_Pt * TMath::Cos(met_Phi);
    float met_Py = met_Pt * TMath::Sin(met_Phi);

    float lep_Px = lep.Px();
    float lep_Py = lep.Py();
    float lep_Pz = lep.Pz();
    float lep_Pt = lep.Pt();
    float lep_E = lep.Energy();

    float A = pow(W_mass, 2)/2 + met_Px * lep_Px + met_Py * lep_Py;
    float D = 4 * ( pow(A,2) * (pow(lep_Pt,2) + pow(lep_Pz,2)) - pow(lep_E,2) * pow(lep_Pt,2) * (pow(met_Px,2) + pow(met_Py,2)) );

    if (D >= 0) {
        // For the case of 2 real roots, take the one with smaller abs value as nu Pz
        float sol_max = ( 2*A*lep_Pz + sqrt(D) ) / ( 2*pow(lep_Pt,2) );
        float sol_min = ( 2*A*lep_Pz - sqrt(D) ) / ( 2*pow(lep_Pt,2) );
        return ( fabs(sol_max) > fabs(sol_min) ) ? sol_min : sol_max;
    } else {
        // For the case of no real root, take the real part of solution as nu Pz
        return ( 2*A*lep_Pz ) / ( 2*pow(lep_Pt,2) );
    }
}
