/*
 * Calculate BDT input variables for hadronic channel.
 * Required trees: T, SF_xxx, Treco_TT, Treco_ST
 *
 * Usage:
 *   ./MakeBDTinput_had <fin> <fout> <fnorm> <N_scale>
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
    TTree *Treco_TT = (TTree*)fin->Get("Treco_TT");
    TTree *Treco_ST = (TTree*)fin->Get("Treco_ST");
    // Input variables
    // Weight & SF
    float genweight = 0;
    float SF_pileup = 0;
    float SF_btag = 0;
    float SF_photon = 0;
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
    // TT reco
    int TT_bJet_idx = 0;
    float TT_bJet_Pt = 0;
    float TT_bJet_Eta = 0;
    float TT_bJet_btag = 0;
    int TT_M1Jet_idx = 0;
    float TT_M1Jet_Pt = 0;
    float TT_M1Jet_Eta = 0;
    float TT_M1Jet_btag = 0;
    int TT_WJet1_idx = 0;
    float TT_WJet1_Pt = 0;
    float TT_WJet1_Eta = 0;
    float TT_WJet1_btag = 0;
    int TT_WJet2_idx = 0;
    float TT_WJet2_Pt = 0;
    float TT_WJet2_Eta = 0;
    float TT_WJet2_btag = 0;
    float TT_MVA_score = 0;
    // ST reco
    int ST_bJet_idx = 0;
    float ST_bJet_Pt = 0;
    float ST_bJet_Eta = 0;
    float ST_bJet_btag = 0;
    int ST_WJet1_idx = 0;
    float ST_WJet1_Pt = 0;
    float ST_WJet1_Eta = 0;
    float ST_WJet1_btag = 0;
    int ST_WJet2_idx = 0;
    float ST_WJet2_Pt = 0;
    float ST_WJet2_Eta = 0;
    float ST_WJet2_btag = 0;
    float ST_MVA_score = 0;
    // Set branches
    // Weight & SF
    T->SetBranchAddress("EvtInfo.genweight", &genweight);
    Tpu->SetBranchAddress("SF_pileup", &SF_pileup);
    Tbtag->SetBranchAddress("SF_btag", &SF_btag);
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
    // TT reco
    Treco_TT->SetBranchAddress("bJet_idx", &TT_bJet_idx);
    Treco_TT->SetBranchAddress("bJet_Pt", &TT_bJet_Pt);
    Treco_TT->SetBranchAddress("bJet_Eta", &TT_bJet_Eta);
    Treco_TT->SetBranchAddress("bJet_btag", &TT_bJet_btag);
    Treco_TT->SetBranchAddress("M1Jet_idx", &TT_M1Jet_idx);
    Treco_TT->SetBranchAddress("M1Jet_Pt", &TT_M1Jet_Pt);
    Treco_TT->SetBranchAddress("M1Jet_Eta", &TT_M1Jet_Eta);
    Treco_TT->SetBranchAddress("M1Jet_btag", &TT_M1Jet_btag);
    Treco_TT->SetBranchAddress("WJet1_idx", &TT_WJet1_idx);
    Treco_TT->SetBranchAddress("WJet1_Pt", &TT_WJet1_Pt);
    Treco_TT->SetBranchAddress("WJet1_Eta", &TT_WJet1_Eta);
    Treco_TT->SetBranchAddress("WJet1_btag", &TT_WJet1_btag);
    Treco_TT->SetBranchAddress("WJet2_idx", &TT_WJet2_idx);
    Treco_TT->SetBranchAddress("WJet2_Pt", &TT_WJet2_Pt);
    Treco_TT->SetBranchAddress("WJet2_Eta", &TT_WJet2_Eta);
    Treco_TT->SetBranchAddress("WJet2_btag", &TT_WJet2_btag);
    Treco_TT->SetBranchAddress("MVA_score", &TT_MVA_score);;
    // ST reco
    Treco_ST->SetBranchAddress("bJet_idx", &ST_bJet_idx);
    Treco_ST->SetBranchAddress("bJet_Pt", &ST_bJet_Pt);
    Treco_ST->SetBranchAddress("bJet_Eta", &ST_bJet_Eta);
    Treco_ST->SetBranchAddress("bJet_btag", &ST_bJet_btag);
    Treco_ST->SetBranchAddress("WJet1_idx", &ST_WJet1_idx);
    Treco_ST->SetBranchAddress("WJet1_Pt", &ST_WJet1_Pt);
    Treco_ST->SetBranchAddress("WJet1_Eta", &ST_WJet1_Eta);
    Treco_ST->SetBranchAddress("WJet1_btag", &ST_WJet1_btag);
    Treco_ST->SetBranchAddress("WJet2_idx", &ST_WJet2_idx);
    Treco_ST->SetBranchAddress("WJet2_Pt", &ST_WJet2_Pt);
    Treco_ST->SetBranchAddress("WJet2_Eta", &ST_WJet2_Eta);
    Treco_ST->SetBranchAddress("WJet2_btag", &ST_WJet2_btag);
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
    Tout->Branch("jet4_pt", &jet4_pt);
    Tout->Branch("jet4_eta", &jet4_eta);
    Tout->Branch("jet4_btag", &jet4_btag);
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
    TLorentzVector TT_bJet, TT_M1Jet, TT_WJet1, TT_WJet2;
    TLorentzVector TT_tqH, TT_tbW, TT_W;
    TLorentzVector ST_bJet, ST_WJet1, ST_WJet2;
    TLorentzVector ST_tbW, ST_W;

    // Start event loop
    for (int evt=0; evt<T->GetEntries(); ++evt)
    {
        T->GetEntry(evt);
        Tpu->GetEntry(evt);
        Tbtag->GetEntry(evt);
        Treco_TT->GetEntry(evt);
        Treco_ST->GetEntry(evt);

        // Set 4-momenta
        // photons
        leadpho.SetPtEtaPhiE(DiPhoInfo_leadPt, DiPhoInfo_leadEta, DiPhoInfo_leadPhi, DiPhoInfo_leadE);
        subleadpho.SetPtEtaPhiE(DiPhoInfo_subleadPt, DiPhoInfo_subleadEta, DiPhoInfo_subleadPhi, DiPhoInfo_subleadE);
        diphoton = leadpho + subleadpho;
        // TT reco
        if (TT_bJet_idx >= 0) {
            float TT_bJet_Phi = JetInfo_Phi->at(TT_bJet_idx);
            float TT_bJet_E = JetInfo_Energy->at(TT_bJet_idx);
            float TT_M1Jet_Phi = JetInfo_Phi->at(TT_M1Jet_idx);
            float TT_M1Jet_E = JetInfo_Energy->at(TT_M1Jet_idx);
            float TT_WJet1_Phi = JetInfo_Phi->at(TT_WJet1_idx);
            float TT_WJet1_E = JetInfo_Energy->at(TT_WJet1_idx);
            float TT_WJet2_Phi = JetInfo_Phi->at(TT_WJet2_idx);
            float TT_WJet2_E = JetInfo_Energy->at(TT_WJet2_idx);
            TT_bJet.SetPtEtaPhiE(TT_bJet_Pt, TT_bJet_Eta, TT_bJet_Phi, TT_bJet_E);
            TT_M1Jet.SetPtEtaPhiE(TT_M1Jet_Pt, TT_M1Jet_Eta, TT_M1Jet_Phi, TT_M1Jet_E);
            TT_WJet1.SetPtEtaPhiE(TT_WJet1_Pt, TT_WJet1_Eta, TT_WJet1_Phi, TT_WJet1_E);
            TT_WJet2.SetPtEtaPhiE(TT_WJet2_Pt, TT_WJet2_Eta, TT_WJet2_Phi, TT_WJet2_E);
            TT_W = TT_WJet1 + TT_WJet2;
            TT_tbW = TT_W + TT_bJet;
            TT_tqH = diphoton + TT_M1Jet;
        }
        // ST reco
        float ST_bJet_Phi = JetInfo_Phi->at(ST_bJet_idx);
        float ST_bJet_E = JetInfo_Energy->at(ST_bJet_idx);
        float ST_WJet1_Phi = JetInfo_Phi->at(ST_WJet1_idx);
        float ST_WJet1_E = JetInfo_Energy->at(ST_WJet1_idx);
        float ST_WJet2_Phi = JetInfo_Phi->at(ST_WJet2_idx);
        float ST_WJet2_E = JetInfo_Energy->at(ST_WJet2_idx);
        ST_bJet.SetPtEtaPhiE(ST_bJet_Pt, ST_bJet_Eta, ST_bJet_Phi, ST_bJet_E);
        ST_WJet1.SetPtEtaPhiE(ST_WJet1_Pt, ST_WJet1_Eta, ST_WJet1_Phi, ST_WJet1_E);
        ST_WJet2.SetPtEtaPhiE(ST_WJet2_Pt, ST_WJet2_Eta, ST_WJet2_Phi, ST_WJet2_E);
        ST_W = ST_WJet1 + ST_WJet2;
        ST_tbW = ST_W + ST_bJet;

        // Calculate output values
        evt_idx = evt;
        genweight_ = genweight;
        SF_pileup_ = SF_pileup;
        SF_btag_ = SF_btag;
        SF_photon_ = SF_photon;
        SF_ElecReco_ = 1;
        SF_ElecID_ = 1;
        SF_MuonID_ = 1;
        SF_MuonIso_ = 1;
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

        // jets
        vector<float> Jet_btag(jets_size);
        for (int i=0; i<jets_size; ++i)
            Jet_btag[i] = JetInfo_pfDeepCSVJetTags_probb->at(i)+JetInfo_pfDeepCSVJetTags_probbb->at(i);
        jet1_pt = JetInfo_Pt->at(0);
        jet1_eta = JetInfo_Eta->at(0);
        jet1_btag = Jet_btag[0];
        jet2_pt = JetInfo_Pt->at(1);
        jet2_eta = JetInfo_Eta->at(1);
        jet2_btag = Jet_btag[1];
        jet3_pt = JetInfo_Pt->at(2);
        jet3_eta = JetInfo_Eta->at(2);
        jet3_btag = Jet_btag[2];
        jet4_pt = (jets_size > 3) ? JetInfo_Pt->at(3) : -999;
        jet4_eta = (jets_size > 3) ? JetInfo_Eta->at(3) : -999;
        jet4_btag = (jets_size > 3) ? Jet_btag[3] : -999;
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
        jet_btag2 = Jet_btag[1];
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
