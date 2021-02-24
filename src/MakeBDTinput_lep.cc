/*
 * Calculate BDT input variables for leptonic channel.
 *
 * Usage:
 *   ./MakeBDTinput_lep <fevt> <freco> <fout> <fnorm> <syst|nosyst> <evtwei_scale>
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
    TString fevt_name = argv[1];
    TString freco_name = argv[2];
    TString fout_name = argv[3];
    char *fnorm_name = argv[4];
    TString syst_ = argv[5];
    int evtwei_scale = atoi(argv[6]);
    bool syst = false;
    if (syst_ == "syst")  syst = true;

    // Get normalization
    float norm = GetNormFactor(fnorm_name);

    // Get input trees
    TFile *fevt = new TFile(fevt_name);
    TTree *Tevt = (TTree*)fevt->Get("T");
    TFile *freco = new TFile(freco_name);
    TTree *Treco_TT = (TTree*)freco->Get("Treco_TT");
    TTree *Treco_ST = (TTree*)freco->Get("Treco_ST");

    // Input variables
    int EvtInfo_EvtNo = 0;
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
    float MetInfo_Phi = 0;
    // Event weight
    // Central
    float evtwei_central = 0;
    // Photon syst
    float evtwei_DiPhoLooseMvaSFUp = 0;
    float evtwei_DiPhoLooseMvaSFDown = 0;
    float evtwei_DiPhoPreselSFUp = 0;
    float evtwei_DiPhoPreselSFDown = 0;
    float evtwei_DiPhoelectronVetoSFUp = 0;
    float evtwei_DiPhoelectronVetoSFDown = 0;
    float evtwei_DiPhoTriggerWeightUp = 0;
    float evtwei_DiPhoTriggerWeightDown = 0;
    // Electron syst
    float evtwei_ElecRecoUp = 0;
    float evtwei_ElecRecoDown = 0;
    float evtwei_ElecIdUp = 0;
    float evtwei_ElecIdDown = 0;
    // Muon syst
    float evtwei_MuonIdUp = 0;
    float evtwei_MuonIdDown = 0;
    float evtwei_MuonIsoUp = 0;
    float evtwei_MuonIsoDown = 0;
    // b-tag syst
    float evtwei_btagjesUp = 0;
    float evtwei_btagjesDown = 0;
    float evtwei_btaglfUp = 0;
    float evtwei_btaglfDown = 0;
    float evtwei_btaghfUp = 0;
    float evtwei_btaghfDown = 0;
    float evtwei_btaglfstats1Up = 0;
    float evtwei_btaglfstats1Down = 0;
    float evtwei_btaglfstats2Up = 0;
    float evtwei_btaglfstats2Down = 0;
    float evtwei_btaghfstats1Up = 0;
    float evtwei_btaghfstats1Down = 0;
    float evtwei_btaghfstats2Up = 0;
    float evtwei_btaghfstats2Down = 0;
    float evtwei_btagcferr1Up = 0;
    float evtwei_btagcferr1Down = 0;
    float evtwei_btagcferr2Up = 0;
    float evtwei_btagcferr2Down = 0;
    // TT reco
    // b-jet
    int TT_bJet_idx = 0;
    float TT_bJet_Pt = 0;
    float TT_bJet_Eta = 0;
    float TT_bJet_btag = 0;
    // M1 jet
    int TT_M1Jet_idx = 0;
    float TT_M1Jet_Pt = 0;
    float TT_M1Jet_Eta = 0;
    float TT_M1Jet_btag = 0;
    // W jet 1
    int TT_WJet1_idx = 0;
    float TT_WJet1_Pt = 0;
    float TT_WJet1_Eta = 0;
    float TT_WJet1_btag = 0;
    // W jet 2
    int TT_WJet2_idx = 0;
    float TT_WJet2_Pt = 0;
    float TT_WJet2_Eta = 0;
    float TT_WJet2_btag = 0;
    // lepton
    int TT_lep_idx = 0;
    float TT_lep_ID = 0;
    float TT_lep_Pt = 0;
    float TT_lep_Eta = 0;
    // nu
    float TT_nu_Pz = 0;
    // MVA score
    float TT_MVA_score = 0;
    // ST reco
    // b-jet
    int ST_bJet_idx = 0;
    float ST_bJet_Pt = 0;
    float ST_bJet_Eta = 0;
    float ST_bJet_btag = 0;
    // W jet 1
    int ST_WJet1_idx = 0;
    float ST_WJet1_Pt = 0;
    float ST_WJet1_Eta = 0;
    float ST_WJet1_btag = 0;
    // W jet 2
    int ST_WJet2_idx = 0;
    float ST_WJet2_Pt = 0;
    float ST_WJet2_Eta = 0;
    float ST_WJet2_btag = 0;
    // lepton
    int ST_lep_idx = 0;
    float ST_lep_ID = 0;
    float ST_lep_Pt = 0;
    float ST_lep_Eta = 0;
    // nu
    float ST_nu_Pz = 0;
    // MVA score
    float ST_MVA_score = 0;

    // Set branches
    Tevt->SetBranchAddress("EvtInfo.EvtNo", &EvtInfo_EvtNo);
    // leading photon
    Tevt->SetBranchAddress("DiPhoInfo.leadPt", &DiPhoInfo_leadPt);
    Tevt->SetBranchAddress("DiPhoInfo.leadEta", &DiPhoInfo_leadEta);
    Tevt->SetBranchAddress("DiPhoInfo.leadPhi", &DiPhoInfo_leadPhi);
    Tevt->SetBranchAddress("DiPhoInfo.leadE", &DiPhoInfo_leadE);
    Tevt->SetBranchAddress("DiPhoInfo.leadIDMVA", &DiPhoInfo_leadIDMVA);
    Tevt->SetBranchAddress("DiPhoInfo.leadhasPixelSeed", &DiPhoInfo_leadhasPixelSeed);
    // subleading photon
    Tevt->SetBranchAddress("DiPhoInfo.subleadPt", &DiPhoInfo_subleadPt);
    Tevt->SetBranchAddress("DiPhoInfo.subleadEta", &DiPhoInfo_subleadEta);
    Tevt->SetBranchAddress("DiPhoInfo.subleadPhi", &DiPhoInfo_subleadPhi);
    Tevt->SetBranchAddress("DiPhoInfo.subleadE", &DiPhoInfo_subleadE);
    Tevt->SetBranchAddress("DiPhoInfo.subleadIDMVA", &DiPhoInfo_subleadIDMVA);
    Tevt->SetBranchAddress("DiPhoInfo.subleadhasPixelSeed", &DiPhoInfo_subleadhasPixelSeed);
    // diphoton
    Tevt->SetBranchAddress("DiPhoInfo.mass", &DiPhoInfo_mass);
    Tevt->SetBranchAddress("DiPhoInfo.pt", &DiPhoInfo_pt);
    // electrons
    Tevt->SetBranchAddress("ElecInfo.Size", &ElecInfo_Size);
    Tevt->SetBranchAddress("ElecInfo.Charge", &ElecInfo_Charge);
    Tevt->SetBranchAddress("ElecInfo.Pt", &ElecInfo_Pt);
    Tevt->SetBranchAddress("ElecInfo.Eta", &ElecInfo_Eta);
    Tevt->SetBranchAddress("ElecInfo.Phi", &ElecInfo_Phi);
    Tevt->SetBranchAddress("ElecInfo.Energy", &ElecInfo_Energy);
    Tevt->SetBranchAddress("ElecInfo.EGMCutBasedIDTight", &ElecInfo_EGMCutBasedIDTight);
    // muons
    Tevt->SetBranchAddress("MuonInfo.Size", &MuonInfo_Size);
    Tevt->SetBranchAddress("MuonInfo.Charge", &MuonInfo_Charge);
    Tevt->SetBranchAddress("MuonInfo.Pt", &MuonInfo_Pt);
    Tevt->SetBranchAddress("MuonInfo.Eta", &MuonInfo_Eta);
    Tevt->SetBranchAddress("MuonInfo.Phi", &MuonInfo_Phi);
    Tevt->SetBranchAddress("MuonInfo.Energy", &MuonInfo_Energy);
    Tevt->SetBranchAddress("MuonInfo.CutBasedIdTight", &MuonInfo_CutBasedIdTight);
    // jets
    Tevt->SetBranchAddress("jets_size", &jets_size);
    Tevt->SetBranchAddress("JetInfo.Pt", &JetInfo_Pt);
    Tevt->SetBranchAddress("JetInfo.Eta", &JetInfo_Eta);
    Tevt->SetBranchAddress("JetInfo.Phi", &JetInfo_Phi);
    Tevt->SetBranchAddress("JetInfo.Energy", &JetInfo_Energy);
    Tevt->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probb", &JetInfo_pfDeepCSVJetTags_probb);
    Tevt->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probbb", &JetInfo_pfDeepCSVJetTags_probbb);
    // MET
    Tevt->SetBranchAddress("MetInfo.Pt", &MetInfo_Pt);
    Tevt->SetBranchAddress("MetInfo.Phi", &MetInfo_Phi);
    // Event weight
    // Central
    Tevt->SetBranchAddress("evtwei_central", &evtwei_central);
    // Photon syst
    Tevt->SetBranchAddress("evtwei_DiPhoLooseMvaSFUp", &evtwei_DiPhoLooseMvaSFUp);
    Tevt->SetBranchAddress("evtwei_DiPhoLooseMvaSFDown", &evtwei_DiPhoLooseMvaSFDown);
    Tevt->SetBranchAddress("evtwei_DiPhoPreselSFUp", &evtwei_DiPhoPreselSFUp);
    Tevt->SetBranchAddress("evtwei_DiPhoPreselSFDown", &evtwei_DiPhoPreselSFDown);
    Tevt->SetBranchAddress("evtwei_DiPhoelectronVetoSFUp", &evtwei_DiPhoelectronVetoSFUp);
    Tevt->SetBranchAddress("evtwei_DiPhoelectronVetoSFDown", &evtwei_DiPhoelectronVetoSFDown);
    Tevt->SetBranchAddress("evtwei_DiPhoTriggerWeightUp", &evtwei_DiPhoTriggerWeightUp);
    Tevt->SetBranchAddress("evtwei_DiPhoTriggerWeightDown", &evtwei_DiPhoTriggerWeightDown);
    // Electron syst
    Tevt->SetBranchAddress("evtwei_ElecRecoUp", &evtwei_ElecRecoUp);
    Tevt->SetBranchAddress("evtwei_ElecRecoDown", &evtwei_ElecRecoDown);
    Tevt->SetBranchAddress("evtwei_ElecIdUp", &evtwei_ElecIdUp);
    Tevt->SetBranchAddress("evtwei_ElecIdDown", &evtwei_ElecIdDown);
    // Muon syst
    Tevt->SetBranchAddress("evtwei_MuonIdUp", &evtwei_MuonIdUp);
    Tevt->SetBranchAddress("evtwei_MuonIdDown", &evtwei_MuonIdDown);
    Tevt->SetBranchAddress("evtwei_MuonIsoUp", &evtwei_MuonIsoUp);
    Tevt->SetBranchAddress("evtwei_MuonIsoDown", &evtwei_MuonIsoDown);
    // b-tag syst
    Tevt->SetBranchAddress("evtwei_btagjesUp", &evtwei_btagjesUp);
    Tevt->SetBranchAddress("evtwei_btagjesDown", &evtwei_btagjesDown);
    Tevt->SetBranchAddress("evtwei_btaglfUp", &evtwei_btaglfUp);
    Tevt->SetBranchAddress("evtwei_btaglfDown", &evtwei_btaglfDown);
    Tevt->SetBranchAddress("evtwei_btaghfUp", &evtwei_btaghfUp);
    Tevt->SetBranchAddress("evtwei_btaghfDown", &evtwei_btaghfDown);
    Tevt->SetBranchAddress("evtwei_btaglfstats1Up", &evtwei_btaglfstats1Up);
    Tevt->SetBranchAddress("evtwei_btaglfstats1Down", &evtwei_btaglfstats1Down);
    Tevt->SetBranchAddress("evtwei_btaglfstats2Up", &evtwei_btaglfstats2Up);
    Tevt->SetBranchAddress("evtwei_btaglfstats2Down", &evtwei_btaglfstats2Down);
    Tevt->SetBranchAddress("evtwei_btaghfstats1Up", &evtwei_btaghfstats1Up);
    Tevt->SetBranchAddress("evtwei_btaghfstats1Down", &evtwei_btaghfstats1Down);
    Tevt->SetBranchAddress("evtwei_btaghfstats2Up", &evtwei_btaghfstats2Up);
    Tevt->SetBranchAddress("evtwei_btaghfstats2Down", &evtwei_btaghfstats2Down);
    Tevt->SetBranchAddress("evtwei_btagcferr1Up", &evtwei_btagcferr1Up);
    Tevt->SetBranchAddress("evtwei_btagcferr1Down", &evtwei_btagcferr1Down);
    Tevt->SetBranchAddress("evtwei_btagcferr2Up", &evtwei_btagcferr2Up);
    Tevt->SetBranchAddress("evtwei_btagcferr2Down", &evtwei_btagcferr2Down);
    // TT reco
    // b-jet
    Treco_TT->SetBranchAddress("bJet_idx", &TT_bJet_idx);
    Treco_TT->SetBranchAddress("bJet_Pt", &TT_bJet_Pt);
    Treco_TT->SetBranchAddress("bJet_Eta", &TT_bJet_Eta);
    Treco_TT->SetBranchAddress("bJet_btag", &TT_bJet_btag);
    // M1 jet
    Treco_TT->SetBranchAddress("M1Jet_idx", &TT_M1Jet_idx);
    Treco_TT->SetBranchAddress("M1Jet_Pt", &TT_M1Jet_Pt);
    Treco_TT->SetBranchAddress("M1Jet_Eta", &TT_M1Jet_Eta);
    Treco_TT->SetBranchAddress("M1Jet_btag", &TT_M1Jet_btag);
    // W jet 1
    Treco_TT->SetBranchAddress("WJet1_idx", &TT_WJet1_idx);
    Treco_TT->SetBranchAddress("WJet1_Pt", &TT_WJet1_Pt);
    Treco_TT->SetBranchAddress("WJet1_Eta", &TT_WJet1_Eta);
    Treco_TT->SetBranchAddress("WJet1_btag", &TT_WJet1_btag);
    // W jet 2
    Treco_TT->SetBranchAddress("WJet2_idx", &TT_WJet2_idx);
    Treco_TT->SetBranchAddress("WJet2_Pt", &TT_WJet2_Pt);
    Treco_TT->SetBranchAddress("WJet2_Eta", &TT_WJet2_Eta);
    Treco_TT->SetBranchAddress("WJet2_btag", &TT_WJet2_btag);
    // lepton
    Treco_TT->SetBranchAddress("lep_idx", &TT_lep_idx);
    Treco_TT->SetBranchAddress("lep_ID", &TT_lep_ID);
    Treco_TT->SetBranchAddress("lep_Pt", &TT_lep_Pt);
    Treco_TT->SetBranchAddress("lep_Eta", &TT_lep_Eta);
    // nu
    Treco_TT->SetBranchAddress("nu_Pz", &TT_nu_Pz);
    // MVA score
    Treco_TT->SetBranchAddress("MVA_score", &TT_MVA_score);
    // ST reco
    // b-jet
    Treco_ST->SetBranchAddress("bJet_idx", &ST_bJet_idx);
    Treco_ST->SetBranchAddress("bJet_Pt", &ST_bJet_Pt);
    Treco_ST->SetBranchAddress("bJet_Eta", &ST_bJet_Eta);
    Treco_ST->SetBranchAddress("bJet_btag", &ST_bJet_btag);
    // W jet 1
    Treco_ST->SetBranchAddress("WJet1_idx", &ST_WJet1_idx);
    Treco_ST->SetBranchAddress("WJet1_Pt", &ST_WJet1_Pt);
    Treco_ST->SetBranchAddress("WJet1_Eta", &ST_WJet1_Eta);
    Treco_ST->SetBranchAddress("WJet1_btag", &ST_WJet1_btag);
    // W jet 2
    Treco_ST->SetBranchAddress("WJet2_idx", &ST_WJet2_idx);
    Treco_ST->SetBranchAddress("WJet2_Pt", &ST_WJet2_Pt);
    Treco_ST->SetBranchAddress("WJet2_Eta", &ST_WJet2_Eta);
    Treco_ST->SetBranchAddress("WJet2_btag", &ST_WJet2_btag);
    // lepton
    Treco_ST->SetBranchAddress("lep_idx", &ST_lep_idx);
    Treco_ST->SetBranchAddress("lep_ID", &ST_lep_ID);
    Treco_ST->SetBranchAddress("lep_Pt", &ST_lep_Pt);
    Treco_ST->SetBranchAddress("lep_Eta", &ST_lep_Eta);
    // nu
    Treco_ST->SetBranchAddress("nu_Pz", &ST_nu_Pz);
    // MVA score
    Treco_ST->SetBranchAddress("MVA_score", &ST_MVA_score);

    // Create output tree
    TFile *fout = new TFile(fout_name, "recreate");
    TTree *Tout = new TTree("T", "");

    // Output variables
    int evt_no = 0;
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
    float jet4_pt = 0;
    float jet4_eta = 0;
    float jet4_btag = 0;
    float jet_btag1 = 0;
    float jet_btag2 = 0;
    float jet_N = 0;
    float jet_Ht = 0;
    // MET
    float met_Pt = 0;
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
    Tout->Branch("evt_no", &evt_no);
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
    Tout->Branch("jet4_pt", &jet4_pt);
    Tout->Branch("jet4_eta", &jet4_eta);
    Tout->Branch("jet4_btag", &jet4_btag);
    Tout->Branch("jet_btag1", &jet_btag1);
    Tout->Branch("jet_btag2", &jet_btag2);
    Tout->Branch("jet_N", &jet_N);
    Tout->Branch("jet_Ht", &jet_Ht);
    // MET
    Tout->Branch("met_Pt", &met_Pt);
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
    // Event weight
    Tout->Branch("evtwei_scale", &evtwei_scale);
    // Central
    Tout->Branch("evtwei_central", &evtwei_central);
    // Photon syst
    Tout->Branch("evtwei_DiPhoLooseMvaSFUp", &evtwei_DiPhoLooseMvaSFUp);
    Tout->Branch("evtwei_DiPhoLooseMvaSFDown", &evtwei_DiPhoLooseMvaSFDown);
    Tout->Branch("evtwei_DiPhoPreselSFUp", &evtwei_DiPhoPreselSFUp);
    Tout->Branch("evtwei_DiPhoPreselSFDown", &evtwei_DiPhoPreselSFDown);
    Tout->Branch("evtwei_DiPhoelectronVetoSFUp", &evtwei_DiPhoelectronVetoSFUp);
    Tout->Branch("evtwei_DiPhoelectronVetoSFDown", &evtwei_DiPhoelectronVetoSFDown);
    Tout->Branch("evtwei_DiPhoTriggerWeightUp", &evtwei_DiPhoTriggerWeightUp);
    Tout->Branch("evtwei_DiPhoTriggerWeightDown", &evtwei_DiPhoTriggerWeightDown);
    // Electron syst
    Tout->Branch("evtwei_ElecRecoUp", &evtwei_ElecRecoUp);
    Tout->Branch("evtwei_ElecRecoDown", &evtwei_ElecRecoDown);
    Tout->Branch("evtwei_ElecIdUp", &evtwei_ElecIdUp);
    Tout->Branch("evtwei_ElecIdDown", &evtwei_ElecIdDown);
    // Muon syst
    Tout->Branch("evtwei_MuonIdUp", &evtwei_MuonIdUp);
    Tout->Branch("evtwei_MuonIdDown", &evtwei_MuonIdDown);
    Tout->Branch("evtwei_MuonIsoUp", &evtwei_MuonIsoUp);
    Tout->Branch("evtwei_MuonIsoDown", &evtwei_MuonIsoDown);
    // b-tag syst
    Tout->Branch("evtwei_btagjesUp", &evtwei_btagjesUp);
    Tout->Branch("evtwei_btagjesDown", &evtwei_btagjesDown);
    Tout->Branch("evtwei_btaglfUp", &evtwei_btaglfUp);
    Tout->Branch("evtwei_btaglfDown", &evtwei_btaglfDown);
    Tout->Branch("evtwei_btaghfUp", &evtwei_btaghfUp);
    Tout->Branch("evtwei_btaghfDown", &evtwei_btaghfDown);
    Tout->Branch("evtwei_btaglfstats1Up", &evtwei_btaglfstats1Up);
    Tout->Branch("evtwei_btaglfstats1Down", &evtwei_btaglfstats1Down);
    Tout->Branch("evtwei_btaglfstats2Up", &evtwei_btaglfstats2Up);
    Tout->Branch("evtwei_btaglfstats2Down", &evtwei_btaglfstats2Down);
    Tout->Branch("evtwei_btaghfstats1Up", &evtwei_btaghfstats1Up);
    Tout->Branch("evtwei_btaghfstats1Down", &evtwei_btaghfstats1Down);
    Tout->Branch("evtwei_btaghfstats2Up", &evtwei_btaghfstats2Up);
    Tout->Branch("evtwei_btaghfstats2Down", &evtwei_btaghfstats2Down);
    Tout->Branch("evtwei_btagcferr1Up", &evtwei_btagcferr1Up);
    Tout->Branch("evtwei_btagcferr1Down", &evtwei_btagcferr1Down);
    Tout->Branch("evtwei_btagcferr2Up", &evtwei_btagcferr2Up);
    Tout->Branch("evtwei_btagcferr2Down", &evtwei_btagcferr2Down);

    // 4-momenta
    TLorentzVector leadPho, subleadPho;
    TLorentzVector diphoton;
    TLorentzVector TT_bJet, TT_M1Jet, TT_lep, TT_nu;
    TLorentzVector TT_tqH, TT_tbW, TT_W;
    TLorentzVector ST_bJet, ST_lep, ST_nu;
    TLorentzVector ST_tbW, ST_W;

    // Start event loop
    for (int evt=0; evt<Tevt->GetEntries(); ++evt)
    {
        Tevt->GetEntry(evt);
        Treco_TT->GetEntry(evt);
        Treco_ST->GetEntry(evt);

        // Set 4-momenta
        // photons
        leadPho.SetPtEtaPhiE(DiPhoInfo_leadPt, DiPhoInfo_leadEta, DiPhoInfo_leadPhi, DiPhoInfo_leadE);
        subleadPho.SetPtEtaPhiE(DiPhoInfo_subleadPt, DiPhoInfo_subleadEta, DiPhoInfo_subleadPhi, DiPhoInfo_subleadE);
        diphoton = leadPho + subleadPho;
        // TT reco
        if (TT_bJet_idx >= 0) { // If evt can be reconstructed
            float TT_bJet_Phi = JetInfo_Phi->at(TT_bJet_idx);
            float TT_bJet_E = JetInfo_Energy->at(TT_bJet_idx);
            float TT_M1Jet_Phi = JetInfo_Phi->at(TT_M1Jet_idx);
            float TT_M1Jet_E = JetInfo_Energy->at(TT_M1Jet_idx);
            float TT_lep_Phi = 0;
            float TT_lep_E = 0;
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
            float TT_nu_Px = MetInfo_Pt * TMath::Cos(MetInfo_Phi);
            float TT_nu_Py = MetInfo_Pt * TMath::Sin(MetInfo_Phi);
            TT_bJet.SetPtEtaPhiE(TT_bJet_Pt, TT_bJet_Eta, TT_bJet_Phi, TT_bJet_E);
            TT_M1Jet.SetPtEtaPhiE(TT_M1Jet_Pt, TT_M1Jet_Eta, TT_M1Jet_Phi, TT_M1Jet_E);
            TT_lep.SetPtEtaPhiE(TT_lep_Pt, TT_lep_Eta, TT_lep_Phi, TT_lep_E);
            TT_nu.SetXYZM(TT_nu_Px, TT_nu_Py, TT_nu_Pz, 0);
            TT_W = TT_lep + TT_nu;
            TT_tbW = TT_W + TT_bJet;
            TT_tqH = diphoton + TT_M1Jet;
        }
        // ST reco
        float ST_bJet_Phi = JetInfo_Phi->at(ST_bJet_idx);
        float ST_bJet_E = JetInfo_Energy->at(ST_bJet_idx);
        float ST_lep_Phi = 0;
        float ST_lep_E = 0;
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
        float ST_nu_Px = MetInfo_Pt * TMath::Cos(MetInfo_Phi);
        float ST_nu_Py = MetInfo_Pt * TMath::Sin(MetInfo_Phi);
        ST_bJet.SetPtEtaPhiE(ST_bJet_Pt, ST_bJet_Eta, ST_bJet_Phi, ST_bJet_E);
        ST_lep.SetPtEtaPhiE(ST_lep_Pt, ST_lep_Eta, ST_lep_Phi, ST_lep_E);
        ST_nu.SetXYZM(ST_nu_Px, ST_nu_Py, ST_nu_Pz, 0);
        ST_W = ST_lep + ST_nu;
        ST_tbW = ST_W + ST_bJet;

        // Calculate output values
        evt_no = EvtInfo_EvtNo;

        // photons
        pho1_ptOverMgg = DiPhoInfo_leadPt / DiPhoInfo_mass;
        pho1_eta = DiPhoInfo_leadEta;
        pho1_PixelSeed = (float)DiPhoInfo_leadhasPixelSeed;
        pho2_ptOverMgg = DiPhoInfo_subleadPt / DiPhoInfo_mass;
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
        dipho_ptOverMgg = DiPhoInfo_pt / DiPhoInfo_mass;
        dipho_eta = diphoton.Eta();
        // |cos(dPhi)|
        dipho_dPhi = fabs(DiPhoInfo_leadPhi - DiPhoInfo_subleadPhi);
        if (dipho_dPhi > TMath::Pi()) dipho_dPhi = 2*TMath::Pi() - dipho_dPhi;
        dipho_dPhi = fabs(TMath::Cos(dipho_dPhi));
        // dR
        dipho_dR = leadPho.DeltaR(subleadPho);
        // |cos(helicity angle)|
        TVector3 dipho_v = diphoton.BoostVector(); // velocity of diphoton
        TLorentzVector leadpho_boost(leadPho);
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
        jet4_pt = -999;
        jet4_eta = -999;
        jet4_btag = -999;
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
        // Jet number
        jet_N = (float)jets_size;
        // Jet H_t
        jet_Ht = 0;
        for (int i=0; i<jets_size; ++i) jet_Ht += JetInfo_Pt->at(i);

        // MET
        met_Pt = MetInfo_Pt;

        // reco TT
        if (TT_bJet_idx >= 0) { // If the event can be reconstructed
            TT_tqH_ptOverM = TT_tqH.Pt() / TT_tqH.M();
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

        // Event weight
        // Central
        evtwei_central *= norm;
        if (syst) {
            // Photon syst
            evtwei_DiPhoLooseMvaSFUp *= norm;
            evtwei_DiPhoLooseMvaSFDown *= norm;
            evtwei_DiPhoPreselSFUp *= norm;
            evtwei_DiPhoPreselSFDown *= norm;
            evtwei_DiPhoelectronVetoSFUp *= norm;
            evtwei_DiPhoelectronVetoSFDown *= norm;
            evtwei_DiPhoTriggerWeightUp *= norm;
            evtwei_DiPhoTriggerWeightDown *= norm;
            // Electron syst
            evtwei_ElecRecoUp *= norm;
            evtwei_ElecRecoDown *= norm;
            evtwei_ElecIdUp *= norm;
            evtwei_ElecIdDown *= norm;
            // Muon syst
            evtwei_MuonIdUp *= norm;
            evtwei_MuonIdDown *= norm;
            evtwei_MuonIsoUp *= norm;
            evtwei_MuonIsoDown *= norm;
            // b-tag syst
            evtwei_btagjesUp *= norm;
            evtwei_btagjesDown *= norm;
            evtwei_btaglfUp *= norm;
            evtwei_btaglfDown *= norm;
            evtwei_btaghfUp *= norm;
            evtwei_btaghfDown *= norm;
            evtwei_btaglfstats1Up *= norm;
            evtwei_btaglfstats1Down *= norm;
            evtwei_btaglfstats2Up *= norm;
            evtwei_btaglfstats2Down *= norm;
            evtwei_btaghfstats1Up *= norm;
            evtwei_btaghfstats1Down *= norm;
            evtwei_btaghfstats2Up *= norm;
            evtwei_btaghfstats2Down *= norm;
            evtwei_btagcferr1Up *= norm;
            evtwei_btagcferr1Down *= norm;
            evtwei_btagcferr2Up *= norm;
            evtwei_btagcferr2Down *= norm;
        }

        Tout->Fill();
    } // End of event loop

    // Save results
    fout->WriteTObject(Tout);
    fout->Close();
    freco->Close();
    fevt->Close();

    return 0;
}
