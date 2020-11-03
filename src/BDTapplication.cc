/*
 * Apply trained BDT to samples.
 *
 * Usage:
 *   ./BDTapplication <fin> <fweight> <fout> <ch>
 */

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include <iostream>
#include <cstdlib>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fin_name = argv[1];
    TString fweight_name = argv[2];
    TString fout_name = argv[3];
    TString ch = argv[4];
    if (ch!="had" && ch!="lep") {
        cout << "[ERROR] Invalid channel!\n";
        exit(1);
    }

    // Read input
    cout << "[INFO] Openning file: " << fin_name << endl;
    TFile *fin = new TFile(fin_name);
    TTree *Tin = (TTree*)fin->Get("T");

    // Input tree variables for BDT
    // photons
    float pho1_ptOverMgg = 0;
    float pho1_eta = 0;
    float pho1_PixelSeed = 0;
    float pho2_ptOverMgg = 0;
    float pho2_eta = 0;
    float pho2_PixelSeed = 0;
    float pho_maxIDMVA = 0;
    float pho_minIDMVA = 0;
    // diphoton
    float dipho_ptOverMgg = 0;
    float dipho_eta = 0;
    float dipho_dPhi = 0;
    float dipho_dR = 0;
    float dipho_heliAngle = 0;
    // leptons
    float lepton_ID = 0;
    float lepton_pt = 0;
    float lepton_eta = 0;
    float lepton_ntight = 0;
    // jets
    float jet1_pt = 0;
    float jet1_eta = 0;
    float jet1_btag = 0;
    float jet2_pt = 0;
    float jet2_eta = 0;
    float jet2_btag = 0;
    float jet3_pt = 0;
    float jet3_eta = 0;
    float jet3_btag = 0;
    float jet4_pt = 0;
    float jet4_eta = 0;
    float jet4_btag = 0;
    float jet_btag1 = 0;
    float jet_btag2 = 0;
    float njets = 0;
    float H_t = 0;
    // MET
    float Et_miss = 0;
    // reco TT
    float TT_tqH_ptOverM = 0;
    float TT_tqH_eta = 0;
    float TT_dR_tqH_H = 0;
    float TT_dR_qH = 0;
    float TT_dR_tt = 0;
    float TT_tbW_pt = 0;
    float TT_tbW_eta = 0;
    float TT_tbW_M = 0;
    float TT_dR_tbW_H = 0;
    float TT_score = 0;
    // reco ST
    float ST_tbW_pt = 0;
    float ST_tbW_eta = 0;
    float ST_tbW_M = 0;
    float ST_dR_tbW_H = 0;
    float ST_score = 0;

    // Set branches
    // photons
    Tin->SetBranchAddress("pho1_ptOverMgg", &pho1_ptOverMgg);
    Tin->SetBranchAddress("pho1_eta", &pho1_eta);
    Tin->SetBranchAddress("pho1_PixelSeed", &pho1_PixelSeed);
    Tin->SetBranchAddress("pho2_ptOverMgg", &pho2_ptOverMgg);
    Tin->SetBranchAddress("pho2_eta", &pho2_eta);
    Tin->SetBranchAddress("pho2_PixelSeed", &pho2_PixelSeed);
    Tin->SetBranchAddress("pho_maxIDMVA", &pho_maxIDMVA);
    Tin->SetBranchAddress("pho_minIDMVA", &pho_minIDMVA);
    // diphoton
    Tin->SetBranchAddress("dipho_ptOverMgg", &dipho_ptOverMgg);
    Tin->SetBranchAddress("dipho_eta", &dipho_eta);
    Tin->SetBranchAddress("dipho_dPhi", &dipho_dPhi);
    Tin->SetBranchAddress("dipho_dR", &dipho_dR);
    Tin->SetBranchAddress("dipho_heliAngle", &dipho_heliAngle);
    // leptons
    if (ch == "lep") {
        Tin->SetBranchAddress("lepton_ID", &lepton_ID);
        Tin->SetBranchAddress("lepton_pt", &lepton_pt);
        Tin->SetBranchAddress("lepton_eta", &lepton_eta);
        Tin->SetBranchAddress("lepton_ntight", &lepton_ntight);
    }
    // jets
    Tin->SetBranchAddress("jet1_pt", &jet1_pt);
    Tin->SetBranchAddress("jet1_eta", &jet1_eta);
    Tin->SetBranchAddress("jet1_btag", &jet1_btag);
    Tin->SetBranchAddress("jet2_pt", &jet2_pt);
    Tin->SetBranchAddress("jet2_eta", &jet2_eta);
    Tin->SetBranchAddress("jet2_btag", &jet2_btag);
    Tin->SetBranchAddress("jet3_pt", &jet3_pt);
    Tin->SetBranchAddress("jet3_eta", &jet3_eta);
    Tin->SetBranchAddress("jet3_btag", &jet3_btag);
    if (ch == "had") {
        Tin->SetBranchAddress("jet4_pt", &jet4_pt);
        Tin->SetBranchAddress("jet4_eta", &jet4_eta);
        Tin->SetBranchAddress("jet4_btag", &jet4_btag);
    }
    Tin->SetBranchAddress("jet_btag1", &jet_btag1);
    Tin->SetBranchAddress("jet_btag2", &jet_btag2);
    Tin->SetBranchAddress("njets", &njets);
    Tin->SetBranchAddress("H_t", &H_t);
    // MET
    Tin->SetBranchAddress("Et_miss", &Et_miss);
    // reco TT
    Tin->SetBranchAddress("TT_tqH_ptOverM", &TT_tqH_ptOverM);
    Tin->SetBranchAddress("TT_tqH_eta", &TT_tqH_eta);
    Tin->SetBranchAddress("TT_dR_tqH_H", &TT_dR_tqH_H);
    Tin->SetBranchAddress("TT_dR_qH", &TT_dR_qH);
    Tin->SetBranchAddress("TT_dR_tt", &TT_dR_tt);
    Tin->SetBranchAddress("TT_tbW_pt", &TT_tbW_pt);
    Tin->SetBranchAddress("TT_tbW_eta", &TT_tbW_eta);
    Tin->SetBranchAddress("TT_tbW_M", &TT_tbW_M);
    Tin->SetBranchAddress("TT_dR_tbW_H", &TT_dR_tbW_H);
    Tin->SetBranchAddress("TT_score", &TT_score);
    // reco ST
    Tin->SetBranchAddress("ST_tbW_pt", &ST_tbW_pt);
    Tin->SetBranchAddress("ST_tbW_eta", &ST_tbW_eta);
    Tin->SetBranchAddress("ST_tbW_M", &ST_tbW_M);
    Tin->SetBranchAddress("ST_dR_tbW_H", &ST_dR_tbW_H);
    Tin->SetBranchAddress("ST_score", &ST_score);

    // Create output tree
    TFile *fout = new TFile(fout_name, "recreate");
    TTree *Tout = Tin->CloneTree(0);
    // BDT score
    float BDT_score = 0;
    Tout->Branch("BDT_score", &BDT_score);

    // Load TMVA library
    TMVA::Tools::Instance();

    // Create TMVA reader
    TMVA::Reader *reader = new TMVA::Reader("V");

    // Set discriminating variables
    // photons
    reader->AddVariable("pho1_ptOverMgg", &pho1_ptOverMgg);
    reader->AddVariable("pho1_eta", &pho1_eta);
    reader->AddVariable("pho1_PixelSeed", &pho1_PixelSeed);
    reader->AddVariable("pho2_ptOverMgg", &pho2_ptOverMgg);
    reader->AddVariable("pho2_eta", &pho2_eta);
    reader->AddVariable("pho2_PixelSeed", &pho2_PixelSeed);
    reader->AddVariable("pho_maxIDMVA", &pho_maxIDMVA);
    reader->AddVariable("pho_minIDMVA", &pho_minIDMVA);
    // diphoton
    reader->AddVariable("dipho_ptOverMgg", &dipho_ptOverMgg);
    reader->AddVariable("dipho_eta", &dipho_eta);
    reader->AddVariable("dipho_dPhi", &dipho_dPhi);
    reader->AddVariable("dipho_dR", &dipho_dR);
    reader->AddVariable("dipho_heliAngle", &dipho_heliAngle);
    // leptons
    if (ch == "lep") {
        reader->AddVariable("lepton_ID", &lepton_ID);
        reader->AddVariable("lepton_pt", &lepton_pt);
        reader->AddVariable("lepton_eta", &lepton_eta);
        reader->AddVariable("lepton_ntight", &lepton_ntight);
    }
    // jets
    reader->AddVariable("jet1_pt", &jet1_pt);
    reader->AddVariable("jet1_eta", &jet1_eta);
    reader->AddVariable("jet1_btag", &jet1_btag);
    reader->AddVariable("jet2_pt", &jet2_pt);
    reader->AddVariable("jet2_eta", &jet2_eta);
    reader->AddVariable("jet2_btag", &jet2_btag);
    reader->AddVariable("jet3_pt", &jet3_pt);
    reader->AddVariable("jet3_eta", &jet3_eta);
    reader->AddVariable("jet3_btag", &jet3_btag);
    if (ch == "had") {
        reader->AddVariable("jet4_pt", &jet4_pt);
        reader->AddVariable("jet4_eta", &jet4_eta);
        reader->AddVariable("jet4_btag", &jet4_btag);
    }
    reader->AddVariable("jet_btag1", &jet_btag1);
    reader->AddVariable("jet_btag2", &jet_btag2);
    reader->AddVariable("njets", &njets);
    reader->AddVariable("H_t", &H_t);
    // MET
    reader->AddVariable("Et_miss", &Et_miss);
    // reco TT
    reader->AddVariable("TT_tqH_ptOverM", &TT_tqH_ptOverM);
    reader->AddVariable("TT_tqH_eta", &TT_tqH_eta);
    reader->AddVariable("TT_dR_tqH_H", &TT_dR_tqH_H);
    reader->AddVariable("TT_dR_qH", &TT_dR_qH);
    reader->AddVariable("TT_dR_tt", &TT_dR_tt);
    reader->AddVariable("TT_tbW_pt", &TT_tbW_pt);
    reader->AddVariable("TT_tbW_eta", &TT_tbW_eta);
    reader->AddVariable("TT_tbW_M", &TT_tbW_M);
    reader->AddVariable("TT_dR_tbW_H", &TT_dR_tbW_H);
    reader->AddVariable("TT_score", &TT_score);
    // reco ST
    reader->AddVariable("ST_tbW_pt", &ST_tbW_pt);
    reader->AddVariable("ST_tbW_eta", &ST_tbW_eta);
    reader->AddVariable("ST_tbW_M", &ST_tbW_M);
    reader->AddVariable("ST_dR_tbW_H", &ST_dR_tbW_H);
    reader->AddVariable("ST_score", &ST_score);

    // Read MVA weight file
    TString mva_name = "BDT";
    cout << "[INFO] Set weight file: " << fweight_name << endl;
    reader->BookMVA(mva_name, fweight_name);

    // Start event loop
    for (int evt=0; evt<Tin->GetEntries(); ++evt) {
        Tin->GetEntry(evt);

        BDT_score = reader->EvaluateMVA(mva_name);
        Tout->Fill();
    } // End of event loop

    // Save result
    fout->Write();
    fout->Close();
    fin->Close();
    delete reader;

    cout << "[INFO] Output tree saved: " << fout_name << endl;

    return 0;
}
