/*
 * Calculate BDT input variables for leptonic channel.
 * Required trees: T, SF_xxx, Treco_TT, Treco_ST
 *
 * Usage:
 *   ./MakeBDTinput_lep <fin> <fout> <fnorm> <N_scale>
 *
 * <N_scale>: (int) number to be multiplied to event weight
 */

#include "include/utility.h"

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TLorentzVector.h"

#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fin_name = argv[1];
    TString fout_name = argv[2];
    char *fnorm_name = argv[3];
    float Nscale = atoi(argv[4]);

    // Get normalization
    float norm = GetNormFactor(fnorm_name);

    // Get input trees
    cout << "[INFO] Open: " << fin_name << endl;
    TFile *fin = new TFile(fin_name);
    TTree *T = (TTree*)fin->Get("T");
    TTree *Tpu = (TTree*)fin->Get("SF_pileup");
    TTree *Tbtag = (TTree*)fin->Get("SF_btag");
    TTree *Telec = (TTree*)fin->Get("SF_Elec");
    TTree *Tmuon = (TTree*)fin->Get("SF_Muon");
    TTree *Treco_TT = (TTree*)fin->Get("Treco_TT");
    TTree *Treco_ST = (TTree*)fin->Get("Treco_ST");
    // Input variables
    // Weight & SF
    float genweight = 0;
    float SF_pileup = 0;
    float SF_btag = 0;
    float SF_photon = 0;
    float SF_ElecReco = 0;
    float SF_ElecID = 0;
    float SF_MuonID = 0;
    float SF_MuonIso = 0;
    // leading photon
    float DiPhoInfo_leadPt = 0;
    float DiPhoInfo_leadEta = 0;
    float DiPhoInfo_leadPhi = 0;
    float DiPhoInfo_leadE = 0;
    float DiPhoInfo_leadIDMVA = 0;
    bool DiPhoInfo_leadhasPixelSeed = 0;
    // subleading photon
    float DiPhoInfo_subleadPt = 0;
    float DiPhoInfo_subleadEta = 0;
    float DiPhoInfo_subleadPhi = 0;
    float DiPhoInfo_subleadE = 0;
    float DiPhoInfo_subleadIDMVA = 0;
    bool DiPhoInfo_subleadhasPixelSeed = 0;
    // diphoton
    float DiPhoInfo_mass = 0;
    float DiPhoInfo_pt = 0;
    // electrons
    int ElecInfo_Size = 0;
    vector<int> *ElecInfo_Charge = 0;
    vector<float> *ElecInfo_Pt = 0;
    vector<float> *ElecInfo_Eta = 0;
    vector<float> *ElecInfo_Phi = 0;
    vector<float> *ElecInfo_Energy = 0;
    vector<bool> *ElecInfo_EGMCutBasedIDTight = 0;
    // muons
    int MuonInfo_Size = 0;
    vector<int> *MuonInfo_Charge = 0;
    vector<float> *MuonInfo_Pt = 0;
    vector<float> *MuonInfo_Eta = 0;
    vector<float> *MuonInfo_Phi = 0;
    vector<float> *MuonInfo_Energy = 0;
    vector<bool> *MuonInfo_CutBasedIdTight = 0;
    // jets
    int jets_size = 0;
    vector<float> *JetInfo_Pt = 0;
    vector<float> *JetInfo_Eta = 0;
    vector<float> *JetInfo_Phi = 0;
    vector<float> *JetInfo_Energy = 0;
    vector<float> *JetInfo_pfDeepCSVJetTags_probb = 0;
    vector<float> *JetInfo_pfDeepCSVJetTags_probbb = 0;
    // MET
    float MetInfo_Pt = 0;
    float MetInfo_Px = 0;
    float MetInfo_Py = 0;
    // TT reco
    int TT_bJet_idx = 0;
    float TT_bJet_Pt = 0;
    float TT_bJet_Eta = 0;
    float TT_bJet_btag = 0;
    int TT_M1Jet_idx = 0;
    float TT_M1Jet_Pt = 0;
    float TT_M1Jet_Eta = 0;
    float TT_M1Jet_btag = 0;
    int TT_lep_idx = 0;
    float TT_lep_ID = 0;
    float TT_lep_Pt = 0;
    float TT_lep_Eta = 0;
    int TT_nu_Pz_real = 0;
    float TT_nu_Pz_L = 0;
    float TT_nu_Pz_S = 0;
    float TT_nu_Pz_M = 0;
    float TT_MVA_score = 0;
    // ST reco
    int ST_bJet_idx = 0;
    float ST_bJet_Pt = 0;
    float ST_bJet_Eta = 0;
    float ST_bJet_btag = 0;
    int ST_lep_idx = 0;
    float ST_lep_ID = 0;
    float ST_lep_Pt = 0;
    float ST_lep_Eta = 0;
    int ST_nu_Pz_real = 0;
    float ST_nu_Pz_L = 0;
    float ST_nu_Pz_S = 0;
    float ST_nu_Pz_M = 0;
    float ST_MVA_score = 0;
    // Set branches
    // Weight & SF
    T->SetBranchAddress("EvtInfo.genweight", &genweight);
    Tpu->SetBranchAddress("SF_pileup", &SF_pileup);
    Tbtag->SetBranchAddress("SF_btag", &SF_btag);
    Telec->SetBranchAddress("Reco", &SF_ElecReco);
    Telec->SetBranchAddress("ID", &SF_ElecID);
    Tmuon->SetBranchAddress("ID", &SF_MuonID);
    Tmuon->SetBranchAddress("ISO", &SF_MuonIso);
    T->SetBranchAddress("DiPhoInfo.centralWeight", &SF_photon);
    // leading photon
    T->SetBranchAddress("DiPhoInfo.leadPt", &DiPhoInfo_leadPt);
    T->SetBranchAddress("DiPhoInfo.leadEta", &DiPhoInfo_leadEta);
    T->SetBranchAddress("DiPhoInfo.leadPhi", &DiPhoInfo_leadPhi);
    T->SetBranchAddress("DiPhoInfo.leadE", &DiPhoInfo_leadE);
    T->SetBranchAddress("DiPhoInfo.leadIDMVA", &DiPhoInfo_leadIDMVA);
    T->SetBranchAddress("DiPhoInfo.leadhasPixelSeed", &DiPhoInfo_leadhasPixelSeed);
    // subleading photon
    T->SetBranchAddress("DiPhoInfo.subleadPt", &DiPhoInfo_subleadPt);
    T->SetBranchAddress("DiPhoInfo.subleadEta", &DiPhoInfo_subleadEta);
    T->SetBranchAddress("DiPhoInfo.subleadPhi", &DiPhoInfo_subleadPhi);
    T->SetBranchAddress("DiPhoInfo.subleadE", &DiPhoInfo_subleadE);
    T->SetBranchAddress("DiPhoInfo.subleadIDMVA", &DiPhoInfo_subleadIDMVA);
    T->SetBranchAddress("DiPhoInfo.subleadhasPixelSeed", &DiPhoInfo_subleadhasPixelSeed);
    // diphoton
    T->SetBranchAddress("DiPhoInfo.mass", &DiPhoInfo_mass);
    T->SetBranchAddress("DiPhoInfo.pt", &DiPhoInfo_pt);
    // electrons
    T->SetBranchAddress("ElecInfo.Size", &ElecInfo_Size);
    T->SetBranchAddress("ElecInfo.Charge", &ElecInfo_Charge);
    T->SetBranchAddress("ElecInfo.Pt", &ElecInfo_Pt);
    T->SetBranchAddress("ElecInfo.Eta", &ElecInfo_Eta);
    T->SetBranchAddress("ElecInfo.Phi", &ElecInfo_Phi);
    T->SetBranchAddress("ElecInfo.Energy", &ElecInfo_Energy);
    T->SetBranchAddress("ElecInfo.EGMCutBasedIDTight", &ElecInfo_EGMCutBasedIDTight);
    // muons
    T->SetBranchAddress("MuonInfo.Size", &MuonInfo_Size);
    T->SetBranchAddress("MuonInfo.Charge", &MuonInfo_Charge);
    T->SetBranchAddress("MuonInfo.Pt", &MuonInfo_Pt);
    T->SetBranchAddress("MuonInfo.Eta", &MuonInfo_Eta);
    T->SetBranchAddress("MuonInfo.Phi", &MuonInfo_Phi);
    T->SetBranchAddress("MuonInfo.Energy", &MuonInfo_Energy);
    T->SetBranchAddress("MuonInfo.CutBasedIdTight", &MuonInfo_CutBasedIdTight);
    // jets
    T->SetBranchAddress("jets_size", &jets_size);
    T->SetBranchAddress("JetInfo.Pt", &JetInfo_Pt);
    T->SetBranchAddress("JetInfo.Eta", &JetInfo_Eta);
    T->SetBranchAddress("JetInfo.Phi", &JetInfo_Phi);
    T->SetBranchAddress("JetInfo.Energy", &JetInfo_Energy);
    T->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probb", &JetInfo_pfDeepCSVJetTags_probb);
    T->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probbb", &JetInfo_pfDeepCSVJetTags_probbb);
    // MET
    T->SetBranchAddress("MetInfo.Pt", &MetInfo_Pt);
    T->SetBranchAddress("MetInfo.Px", &MetInfo_Px);
    T->SetBranchAddress("MetInfo.Py", &MetInfo_Py);
    // TT reco
    Treco_TT->SetBranchAddress("bJet_idx", &TT_bJet_idx);
    Treco_TT->SetBranchAddress("bJet_Pt", &TT_bJet_Pt);
    Treco_TT->SetBranchAddress("bJet_Eta", &TT_bJet_Eta);
    Treco_TT->SetBranchAddress("bJet_btag", &TT_bJet_btag);
    Treco_TT->SetBranchAddress("M1Jet_idx", &TT_M1Jet_idx);
    Treco_TT->SetBranchAddress("M1Jet_Pt", &TT_M1Jet_Pt);
    Treco_TT->SetBranchAddress("M1Jet_Eta", &TT_M1Jet_Eta);
    Treco_TT->SetBranchAddress("M1Jet_btag", &TT_M1Jet_btag);
    Treco_TT->SetBranchAddress("lep_idx", &TT_lep_idx);
    Treco_TT->SetBranchAddress("lep_ID", &TT_lep_ID);
    Treco_TT->SetBranchAddress("lep_Pt", &TT_lep_Pt);
    Treco_TT->SetBranchAddress("lep_Eta", &TT_lep_Eta);
    Treco_TT->SetBranchAddress("nu_Pz_real", &TT_nu_Pz_real);
    Treco_TT->SetBranchAddress("nu_Pz_L", &TT_nu_Pz_L);
    Treco_TT->SetBranchAddress("nu_Pz_S", &TT_nu_Pz_S);
    Treco_TT->SetBranchAddress("nu_Pz_M", &TT_nu_Pz_M);
    Treco_TT->SetBranchAddress("MVA_score", &TT_MVA_score);
    // ST reco
    Treco_ST->SetBranchAddress("bJet_idx", &ST_bJet_idx);
    Treco_ST->SetBranchAddress("bJet_Pt", &ST_bJet_Pt);
    Treco_ST->SetBranchAddress("bJet_Eta", &ST_bJet_Eta);
    Treco_ST->SetBranchAddress("bJet_btag", &ST_bJet_btag);
    Treco_ST->SetBranchAddress("lep_idx", &ST_lep_idx);
    Treco_ST->SetBranchAddress("lep_ID", &ST_lep_ID);
    Treco_ST->SetBranchAddress("lep_Pt", &ST_lep_Pt);
    Treco_ST->SetBranchAddress("lep_Eta", &ST_lep_Eta);
    Treco_ST->SetBranchAddress("nu_Pz_real", &ST_nu_Pz_real);
    Treco_ST->SetBranchAddress("nu_Pz_L", &ST_nu_Pz_L);
    Treco_ST->SetBranchAddress("nu_Pz_S", &ST_nu_Pz_S);
    Treco_ST->SetBranchAddress("nu_Pz_M", &ST_nu_Pz_M);
    Treco_ST->SetBranchAddress("MVA_score", &ST_MVA_score);

    // Create output tree
    TFile *fout = new TFile(fout_name, "recreate");
    TTree *Tout = new TTree("T", "");
    // Output variables
    int evt_idx = 0;
    float genweight_ = 0;
    float SF_pileup_ = 0;
    float SF_btag_ = 0;
    float SF_photon_ = 0;
    float SF_ElecReco_ = 0;
    float SF_ElecID_ = 0;
    float SF_MuonID_ = 0;
    float SF_MuonIso_ = 0;
    // photon
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
    float jet_btag1 = 0;
    float jet_btag2 = 0;
    float njets = 0;
    float H_t = 0;
    // MET
    float Et_miss = 0;
    // reco TT
    float TT_tqH_ptOverM = 0; // top (tqH) pt over inv mass
    float TT_tqH_eta = 0; // top (tqH) eta
    float TT_dR_tqH_H = 0; // angle top (tqH) & H
    float TT_dR_qH = 0; // angle q (tqH) & H
    float TT_dR_tt = 0; // angle top (tqH) & top (tbW)
    float TT_tbW_pt = 0; // top (tbW) pt
    float TT_tbW_eta = 0; // top (tbW) eta
    float TT_tbW_M = 0; // top (tbW) inv mass
    float TT_dR_tbW_H = 0; // angle top (tbW) & H
    float TT_score = 0;
    // reco ST
    float ST_tbW_pt = 0; // top (tbW) pt
    float ST_tbW_eta = 0; // top (tbW) eta
    float ST_tbW_M = 0; // top (tbW) inv mass
    float ST_dR_tbW_H = 0; // angle top (tbW) & H
    float ST_score = 0;
    // Set branches
    Tout->Branch("evt_idx", &evt_idx);
    Tout->Branch("Nscale", &Nscale);
    Tout->Branch("norm", &norm);
    Tout->Branch("genweight", &genweight_);
    Tout->Branch("SF_pileup", &SF_pileup_);
    Tout->Branch("SF_btag", &SF_btag_);
    Tout->Branch("SF_photon", &SF_photon_);
    Tout->Branch("SF_ElecReco", &SF_ElecReco_);
    Tout->Branch("SF_ElecID", &SF_ElecID_);
    Tout->Branch("SF_MuonID", &SF_MuonID_);
    Tout->Branch("SF_MuonIso", &SF_MuonIso_);
    // photons
    Tout->Branch("pho1_ptOverMgg", &pho1_ptOverMgg);
    Tout->Branch("pho1_eta", &pho1_eta);
    Tout->Branch("pho1_PixelSeed", &pho1_PixelSeed);
    Tout->Branch("pho2_ptOverMgg", &pho2_ptOverMgg);
    Tout->Branch("pho2_eta", &pho2_eta);
    Tout->Branch("pho2_PixelSeed", &pho2_PixelSeed);
    Tout->Branch("pho_maxIDMVA", &pho_maxIDMVA);
    Tout->Branch("pho_minIDMVA", &pho_minIDMVA);
    // diphoton
    Tout->Branch("dipho_ptOverMgg", &dipho_ptOverMgg);
    Tout->Branch("dipho_eta", &dipho_eta);
    Tout->Branch("dipho_dPhi", &dipho_dPhi);
    Tout->Branch("dipho_dR", &dipho_dR);
    Tout->Branch("dipho_heliAngle", &dipho_heliAngle);
    // leptons
    Tout->Branch("lepton_ID", &lepton_ID);
    Tout->Branch("lepton_pt", &lepton_pt);
    Tout->Branch("lepton_eta", &lepton_eta);
    Tout->Branch("lepton_ntight", &lepton_ntight);
    // jets
    Tout->Branch("jet1_pt", &jet1_pt);
    Tout->Branch("jet1_eta", &jet1_eta);
    Tout->Branch("jet1_btag", &jet1_btag);
    Tout->Branch("jet2_pt", &jet2_pt);
    Tout->Branch("jet2_eta", &jet2_eta);
    Tout->Branch("jet2_btag", &jet2_btag);
    Tout->Branch("jet3_pt", &jet3_pt);
    Tout->Branch("jet3_eta", &jet3_eta);
    Tout->Branch("jet3_btag", &jet3_btag);
    Tout->Branch("jet_btag1", &jet_btag1);
    Tout->Branch("jet_btag2", &jet_btag2);
    Tout->Branch("njets", &njets);
    Tout->Branch("H_t", &H_t);
    // MET
    Tout->Branch("Et_miss", &Et_miss);
    // reco TT
    Tout->Branch("TT_tqH_ptOverM", &TT_tqH_ptOverM);
    Tout->Branch("TT_tqH_eta", &TT_tqH_eta);
    Tout->Branch("TT_dR_tqH_H", &TT_dR_tqH_H);
    Tout->Branch("TT_dR_qH", &TT_dR_qH);
    Tout->Branch("TT_dR_tt", &TT_dR_tt);
    Tout->Branch("TT_tbW_pt", &TT_tbW_pt);
    Tout->Branch("TT_tbW_eta", &TT_tbW_eta);
    Tout->Branch("TT_tbW_M", &TT_tbW_M);
    Tout->Branch("TT_dR_tbW_H", &TT_dR_tbW_H);
    Tout->Branch("TT_score", &TT_score);
    // reco ST
    Tout->Branch("ST_tbW_pt", &ST_tbW_pt);
    Tout->Branch("ST_tbW_eta", &ST_tbW_eta);
    Tout->Branch("ST_tbW_M", &ST_tbW_M);
    Tout->Branch("ST_dR_tbW_H", &ST_dR_tbW_H);
    Tout->Branch("ST_score", &ST_score);

    // 4-momenta
    TLorentzVector leadpho, subleadpho;
    TLorentzVector diphoton;
    TLorentzVector TT_bJet, TT_M1Jet, TT_lep, TT_nu;
    TLorentzVector TT_tqH, TT_tbW, TT_W;
    TLorentzVector ST_bJet, ST_lep, ST_nu;
    TLorentzVector ST_tbW, ST_W;

    // Start event loop
    for (int evt=0; evt<T->GetEntries(); ++evt)
    {
        T->GetEntry(evt);
        Tpu->GetEntry(evt);
        Tbtag->GetEntry(evt);
        Telec->GetEntry(evt);
        Tmuon->GetEntry(evt);
        Treco_TT->GetEntry(evt);
        Treco_ST->GetEntry(evt);

        // Set 4-momenta
        // photons
        leadpho.SetPtEtaPhiE(DiPhoInfo_leadPt, DiPhoInfo_leadEta, DiPhoInfo_leadPhi, DiPhoInfo_leadE);
        subleadpho.SetPtEtaPhiE(DiPhoInfo_subleadPt, DiPhoInfo_subleadEta, DiPhoInfo_subleadPhi, DiPhoInfo_subleadE);
        diphoton = leadpho + subleadpho;
        // TT reco
        if (TT_bJet_idx >= 0) { // If evt can be reconstructed
            float TT_bJet_Phi = JetInfo_Phi->at(TT_bJet_idx);
            float TT_bJet_E = JetInfo_Energy->at(TT_bJet_idx);
            float TT_M1Jet_Phi = JetInfo_Phi->at(TT_M1Jet_idx);
            float TT_M1Jet_E = JetInfo_Energy->at(TT_M1Jet_idx);
            float TT_lep_Phi = 0;
            float TT_lep_E = 0;
            float TT_nu_Pz = 0;
            if (fabs(TT_lep_ID) == 11) {
                TT_lep_Phi = ElecInfo_Phi->at(TT_lep_idx);
                TT_lep_E = ElecInfo_Energy->at(TT_lep_idx);
            } else if (fabs(TT_lep_ID) == 13) {
                TT_lep_Phi = MuonInfo_Phi->at(TT_lep_idx);
                TT_lep_E = MuonInfo_Energy->at(TT_lep_idx);
            } else {
                cout << "[ERROR] Invalid lepton ID\n";
                exit(1);
            }
            if (TT_nu_Pz_real == 1) {
                TT_nu_Pz = (fabs(TT_nu_Pz_L) > fabs(TT_nu_Pz_S)) ? TT_nu_Pz_S : TT_nu_Pz_L;
            } else if (TT_nu_Pz_real == 0) {
                TT_nu_Pz = TT_nu_Pz_M;
            } else {
                cout << "[ERROR] Invalid nu_Pz_real\n";
                exit(1);
            }
            TT_bJet.SetPtEtaPhiE(TT_bJet_Pt, TT_bJet_Eta, TT_bJet_Phi, TT_bJet_E);
            TT_M1Jet.SetPtEtaPhiE(TT_M1Jet_Pt, TT_M1Jet_Eta, TT_M1Jet_Phi, TT_M1Jet_E);
            TT_lep.SetPtEtaPhiE(TT_lep_Pt, TT_lep_Eta, TT_lep_Phi, TT_lep_E);
            TT_nu.SetXYZM(MetInfo_Px, MetInfo_Py, TT_nu_Pz, 0);
            TT_W = TT_lep + TT_nu;
            TT_tbW = TT_W + TT_bJet;
            TT_tqH = diphoton + TT_M1Jet;
        }
        // ST reco
        float ST_bJet_Phi = JetInfo_Phi->at(ST_bJet_idx);
        float ST_bJet_E = JetInfo_Energy->at(ST_bJet_idx);
        float ST_lep_Phi = 0;
        float ST_lep_E = 0;
        float ST_nu_Pz = 0;
        if (fabs(ST_lep_ID) == 11) {
            ST_lep_Phi = ElecInfo_Phi->at(ST_lep_idx);
            ST_lep_E = ElecInfo_Energy->at(ST_lep_idx);
        } else if (fabs(ST_lep_ID) == 13) {
            ST_lep_Phi = MuonInfo_Phi->at(ST_lep_idx);
            ST_lep_E = MuonInfo_Energy->at(ST_lep_idx);
        } else {
            cout << "[ERROR] Invalid lepton ID\n";
            exit(1);
        }
        if (ST_nu_Pz_real == 1) {
            ST_nu_Pz = (fabs(ST_nu_Pz_L) > fabs(ST_nu_Pz_S)) ? ST_nu_Pz_S : ST_nu_Pz_L;
        } else if (ST_nu_Pz_real == 0) {
            ST_nu_Pz = ST_nu_Pz_M;
        } else {
            cout << "[ERROR] Invalid nu_Pz_real\n";
            exit(1);
        }
        ST_bJet.SetPtEtaPhiE(ST_bJet_Pt, ST_bJet_Eta, ST_bJet_Phi, ST_bJet_E);
        ST_lep.SetPtEtaPhiE(ST_lep_Pt, ST_lep_Eta, ST_lep_Phi, ST_lep_E);
        ST_nu.SetXYZM(MetInfo_Px, MetInfo_Py, ST_nu_Pz, 0);
        ST_W = ST_lep + ST_nu;
        ST_tbW = ST_W + ST_bJet;

        // Calculate output values
        evt_idx = evt;
        genweight_ = genweight;
        SF_pileup_ = SF_pileup;
        SF_btag_ = SF_btag;
        SF_photon_ = SF_photon;
        SF_ElecReco_ = SF_ElecReco;
        SF_ElecID_ = SF_ElecID;
        SF_MuonID_ = SF_MuonID;
        SF_MuonIso_ = SF_MuonIso;
        // photons
        pho1_ptOverMgg = DiPhoInfo_leadPt/DiPhoInfo_mass;
        pho1_eta = DiPhoInfo_leadEta;
        pho1_PixelSeed = (float)DiPhoInfo_leadhasPixelSeed;
        pho2_ptOverMgg = DiPhoInfo_subleadPt/DiPhoInfo_mass;
        pho2_eta = DiPhoInfo_subleadEta;
        pho2_PixelSeed = (float)DiPhoInfo_subleadhasPixelSeed;
        if (DiPhoInfo_leadIDMVA > DiPhoInfo_subleadIDMVA) {
            pho_maxIDMVA = DiPhoInfo_leadIDMVA;
            pho_minIDMVA = DiPhoInfo_subleadIDMVA;
        } else {
            pho_maxIDMVA = DiPhoInfo_subleadIDMVA;
            pho_minIDMVA = DiPhoInfo_leadIDMVA;
        }

        // diphoton
        dipho_ptOverMgg = DiPhoInfo_pt/DiPhoInfo_mass;
        dipho_eta = diphoton.Eta();
        // |cos(dPhi)|
        dipho_dPhi = fabs(DiPhoInfo_leadPhi - DiPhoInfo_subleadPhi);
        if (dipho_dPhi > TMath::Pi()) dipho_dPhi = 2*TMath::Pi() - dipho_dPhi;
        dipho_dPhi = fabs(TMath::Cos(dipho_dPhi));
        // dR
        dipho_dR = leadpho.DeltaR(subleadpho);
        // |cos(helicity angle)|
        TVector3 dipho_v = diphoton.BoostVector(); // velocity of diphoton
        TLorentzVector leadpho_boost(leadpho);
        // Note: Boost() perform transformation of 4-vectors from "rod frame" to "original frame"
        // Here we need transformation from "original" to "rod", so the extra minus sign
        // See: https://root.cern/doc/master/classTLorentzVector.html
        leadpho_boost.Boost(-dipho_v);
        dipho_heliAngle = (diphoton.Vect()).Angle(leadpho_boost.Vect()); // angle btw spacial components
        dipho_heliAngle = fabs( TMath::Cos(dipho_heliAngle) );

        // leptons
        if (MuonInfo_Size == 0) {
            if (ElecInfo_Charge->at(0) > 0) lepton_ID = -11;
            else lepton_ID = 11;
            lepton_pt = ElecInfo_Pt->at(0);
            lepton_eta = ElecInfo_Eta->at(0);
        } else if (ElecInfo_Size == 0) {
            if (MuonInfo_Charge->at(0) > 0) lepton_ID = -13;
            else lepton_ID = 13;
            lepton_pt = MuonInfo_Pt->at(0);
            lepton_eta = MuonInfo_Eta->at(0);
        } else {
            if (ElecInfo_Pt->at(0) > MuonInfo_Pt->at(0)) {
                if (ElecInfo_Charge->at(0) > 0) lepton_ID = -11;
                else lepton_ID = 11;
                lepton_pt = ElecInfo_Pt->at(0);
                lepton_eta = ElecInfo_Eta->at(0);
            } else {
                if (MuonInfo_Charge->at(0) > 0) lepton_ID = -13;
                else lepton_ID = 13;
                lepton_pt = MuonInfo_Pt->at(0);
                lepton_eta = MuonInfo_Eta->at(0);
            }
        }
        // lepton_ntight
        lepton_ntight = 0;
        for (int i=0; i<ElecInfo_Size; ++i)
            if (ElecInfo_EGMCutBasedIDTight->at(i)) lepton_ntight += 1;
        for (int i=0; i<MuonInfo_Size; ++i)
            if (MuonInfo_CutBasedIdTight->at(i)) lepton_ntight += 1;

        // jets
        vector<float> Jet_btag(jets_size);
        for (int i=0; i<jets_size; ++i)
            Jet_btag[i] = JetInfo_pfDeepCSVJetTags_probb->at(i)+JetInfo_pfDeepCSVJetTags_probbb->at(i);
        jet1_pt = JetInfo_Pt->at(0);
        jet1_eta = JetInfo_Eta->at(0);
        jet1_btag = Jet_btag[0];
        jet2_pt = (jets_size > 1) ? JetInfo_Pt->at(1) : -999;
        jet2_eta = (jets_size > 1) ? JetInfo_Eta->at(1) : -999;
        jet2_btag = (jets_size > 1) ? Jet_btag[1] : -999;
        jet3_pt = (jets_size > 2) ? JetInfo_Pt->at(2) : -999;
        jet3_eta = (jets_size > 2) ? JetInfo_Eta->at(2) : -999;
        jet3_btag = (jets_size > 2) ? Jet_btag[2] : -999;
        // Perform insertion sort of b-tag scores
        for (int i=1; i!=Jet_btag.size(); ++i) {
            float key = Jet_btag[i];
            int j = i-1;
            while (j>=0 && Jet_btag[j]<key) {
                Jet_btag[j+1] = Jet_btag[j];
                --j;
            }
            Jet_btag[j+1] = key;
        }
        jet_btag1 = Jet_btag[0];
        jet_btag2 = (jets_size > 1) ? Jet_btag[1] : -999;
        njets = (float)jets_size;
        H_t = 0;
        for (int i=0; i<jets_size; ++i) H_t += JetInfo_Pt->at(i);

        // MET
        Et_miss = MetInfo_Pt;

        // reco TT
        if (TT_bJet_idx >= 0) { // If the event can be reconstructed
            TT_tqH_ptOverM = TT_tqH.Pt()/TT_tqH.M();
            TT_tqH_eta = TT_tqH.Eta();
            TT_dR_tqH_H = TT_tqH.DeltaR(diphoton);
            TT_dR_qH = diphoton.DeltaR(TT_M1Jet);
            TT_dR_tt = TT_tqH.DeltaR(TT_tbW);
            TT_tbW_pt = TT_tbW.Pt();
            TT_tbW_eta = TT_tbW.Eta();
            TT_tbW_M = TT_tbW.M();
            TT_dR_tbW_H = TT_tbW.DeltaR(diphoton);
            TT_score = TT_MVA_score;
        } else {
            TT_tqH_ptOverM = -999;
            TT_tqH_eta = -999;
            TT_dR_tqH_H = -999;
            TT_dR_qH = -999;
            TT_dR_tt = -999;
            TT_tbW_pt = -999;
            TT_tbW_eta = -999;
            TT_tbW_M = -999;
            TT_dR_tbW_H = -999;
            TT_score = -999;
        }

        // reco ST
        ST_tbW_pt = ST_tbW.Pt();
        ST_tbW_eta = ST_tbW.Eta();
        ST_tbW_M = ST_tbW.M();
        ST_dR_tbW_H = ST_tbW.DeltaR(diphoton);
        ST_score = ST_MVA_score;

        Tout->Fill();
    } // End of event loop

    // Save results
    fout->WriteTObject(Tout);
    fout->Close();
    fin->Close();
    cout << "[INFO] Output saved: " << fout_name << endl;

    return 0;
}
