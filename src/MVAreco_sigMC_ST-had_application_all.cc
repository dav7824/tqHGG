// Applying trained models for reconstruction of hadronic ST signal to signal MC test sample.
//
// input arguments:
// 1. path to input root file
// 2. name of input TTree
// 3. name of TMVA method
// 4. path to TMVA weight file
// 5. path to output root file

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TString.h"

#include <iostream>
#include <vector>
using namespace std;

// cut on MVA score
double cut_MVA = -999;

int main(int argc, char **argv)
{
    // get input arguments
    TString fin_name = argv[1];
    TString inTree_name = argv[2];
    TString method = argv[3];
    TString fweight_name = argv[4];
    TString fout_name = argv[5];

    cout << "[INFO] Reading root file: " << fin_name << endl;

    // read input root file
    TChain *inTree = new TChain(inTree_name);
    inTree->Add(fin_name);

    // create output root file
    TFile *fout = new TFile(fout_name, "recreate");
    TTree *outTree = new TTree("flashggStdTree", "");

    // declare tree variables
    float EvtInfo_NPu = 0;
    int EvtInfo_NVtx = 0;
    bool EvtInfo_passTrigger = 0;
    float EvtInfo_genweight = 0;
    float EvtInfo_Rho = 0;
    float EvtInfo_PVz = 0;
    float EvtInfo_BSsigmaz = 0;
    bool EvtInfo_Flag_HBHENoiseFilter = 0;
    bool EvtInfo_Flag_HBHENoiseIsoFilter = 0;
    bool EvtInfo_Flag_EcalDeadCellTriggerPrimitiveFilter = 0;
    bool EvtInfo_Flag_goodVertices = 0;
    bool EvtInfo_Flag_globalSuperTightHalo2016Filter = 0;
    bool EvtInfo_Flag_BadPFMuonFilter = 0;
    bool EvtInfo_Flag_eeBadScFilter = 0;
    bool EvtInfo_ecalBadCalibReducedMINIAODFilter = 0;
    float DiPhoInfo_mass = 0;
    float DiPhoInfo_pt = 0;
    float DiPhoInfo_leadPt = 0;
    float DiPhoInfo_leadEta = 0;
    float DiPhoInfo_leadPhi = 0;
    float DiPhoInfo_leadE = 0;
    float DiPhoInfo_leadEtaSC = 0;
    float DiPhoInfo_leadPhiSC = 0;
    float DiPhoInfo_leadsigEOverE = 0;
    float DiPhoInfo_leadR9 = 0;
    float DiPhoInfo_leadsieie = 0;
    float DiPhoInfo_leadhoe = 0;
    float DiPhoInfo_leadIDMVA = 0;
    bool DiPhoInfo_leadIsEB = 0;
    bool DiPhoInfo_leadIsEE = 0;
    bool DiPhoInfo_leadhasPixelSeed = 0;
    bool DiPhoInfo_leadGenMatch = 0;
    int DiPhoInfo_leadGenMatchType = 0;
    float DiPhoInfo_subleadPt = 0;
    float DiPhoInfo_subleadEta = 0;
    float DiPhoInfo_subleadPhi = 0;
    float DiPhoInfo_subleadE = 0;
    float DiPhoInfo_subleadEtaSC = 0;
    float DiPhoInfo_subleadPhiSC = 0;
    float DiPhoInfo_subleadsigEOverE = 0;
    float DiPhoInfo_subleadR9 = 0;
    float DiPhoInfo_subleadsieie = 0;
    float DiPhoInfo_subleadhoe = 0;
    float DiPhoInfo_subleadIDMVA = 0;
    bool DiPhoInfo_subleadIsEB = 0;
    bool DiPhoInfo_subleadIsEE = 0;
    bool DiPhoInfo_subleadhasPixelSeed = 0;
    bool DiPhoInfo_subleadGenMatch = 0;
    int DiPhoInfo_subleadGenMatchType = 0;
    float DiPhoInfo_diphotonMVA = 0;
    float DiPhoInfo_SelectedVz = 0;
    float DiPhoInfo_GenVz = 0;
    float DiPhoInfo_centralWeight = 0;
    int ElecInfo_Size = 0;
    vector<int> *ElecInfo_Charge = 0;
    vector<float> *ElecInfo_Pt = 0;
    vector<float> *ElecInfo_Eta = 0;
    vector<float> *ElecInfo_Phi = 0;
    vector<float> *ElecInfo_Energy = 0;
    vector<float> *ElecInfo_EtaSC = 0;
    vector<float> *ElecInfo_PhiSC = 0;
    vector<float> *ElecInfo_GsfTrackDz = 0;
    vector<float> *ElecInfo_GsfTrackDxy = 0;
    vector<bool> *ElecInfo_EGMCutBasedIDVeto = 0;
    vector<bool> *ElecInfo_EGMCutBasedIDLoose = 0;
    vector<bool> *ElecInfo_EGMCutBasedIDMedium = 0;
    vector<bool> *ElecInfo_EGMCutBasedIDTight = 0;
    vector<bool> *ElecInfo_passConvVeto = 0;
    vector<bool> *ElecInfo_fggPhoVeto = 0;
    vector<float> *ElecInfo_EnergyCorrFactor = 0;
    vector<float> *ElecInfo_EnergyPostCorrErr = 0;
    vector<bool> *ElecInfo_GenMatch = 0;
    vector<int> *ElecInfo_GenPdgID = 0;
    vector<float> *ElecInfo_GenPt = 0;
    vector<float> *ElecInfo_GenEta = 0;
    vector<float> *ElecInfo_GenPhi = 0;
    int MuonInfo_Size = 0;
    vector<int> *MuonInfo_Charge = 0;
    vector<float> *MuonInfo_MuonType = 0;
    vector<float> *MuonInfo_Pt = 0;
    vector<float> *MuonInfo_Eta = 0;
    vector<float> *MuonInfo_Phi = 0;
    vector<float> *MuonInfo_Energy = 0;
    vector<float> *MuonInfo_BestTrackDz = 0;
    vector<float> *MuonInfo_BestTrackDxy = 0;
    vector<float> *MuonInfo_PFIsoDeltaBetaCorrR04 = 0;
    vector<float> *MuonInfo_TrackerBasedIsoR03 = 0;
    vector<bool> *MuonInfo_CutBasedIdMedium = 0;
    vector<bool> *MuonInfo_CutBasedIdTight = 0;
    vector<bool> *MuonInfo_CutBasedIdTight_bestVtx = 0;
    vector<bool> *MuonInfo_GenMatch = 0;
    vector<int> *MuonInfo_GenPdgID = 0;
    vector<float> *MuonInfo_GenPt = 0;
    vector<float> *MuonInfo_GenEta = 0;
    vector<float> *MuonInfo_GenPhi = 0;
    int jets_size = 0;
    vector<float> *JetInfo_Pt = 0;
    vector<float> *JetInfo_Eta = 0;
    vector<float> *JetInfo_Phi = 0;
    vector<float> *JetInfo_Mass = 0;
    vector<float> *JetInfo_Energy = 0;
    vector<float> *JetInfo_PtRaw = 0;
    vector<float> *JetInfo_QGL = 0;
    vector<float> *JetInfo_RMS = 0;
    vector<float> *JetInfo_puJetIdMVA = 0;
    vector<bool> *JetInfo_passesPuJetIdLoose = 0;
    vector<bool> *JetInfo_passesPuJetIdMedium = 0;
    vector<bool> *JetInfo_passesPuJetIdTight = 0;
    vector<bool> *JetInfo_GenJetMatch = 0;
    vector<float> *JetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags = 0;
    vector<float> *JetInfo_pfCombinedMVAV2BJetTags = 0;
    vector<float> *JetInfo_pfDeepCSVJetTags_probb = 0;
    vector<float> *JetInfo_pfDeepCSVJetTags_probbb = 0;
    vector<float> *JetInfo_pfDeepCSVJetTags_probc = 0;
    vector<float> *JetInfo_pfDeepCSVJetTags_probudsg = 0;
    vector<float> *JetInfo_pfDeepFlavourJetTags_probb = 0;
    vector<float> *JetInfo_pfDeepFlavourJetTags_probbb = 0;
    vector<float> *JetInfo_pfDeepFlavourJetTags_probc = 0;
    vector<float> *JetInfo_pfDeepFlavourJetTags_probuds = 0;
    vector<float> *JetInfo_pfDeepFlavourJetTags_probg = 0;
    vector<float> *JetInfo_pfDeepFlavourJetTags_problepb = 0;
    vector<float> *JetInfo_JECScale = 0;
    vector<float> *JetInfo_JERScale = 0;
    vector<bool> *JetInfo_GenPartonMatch = 0;
    vector<float> *JetInfo_GenPt = 0;
    vector<float> *JetInfo_GenEta = 0;
    vector<float> *JetInfo_GenPhi = 0;
    vector<int> *JetInfo_GenPdgID = 0;
    vector<int> *JetInfo_GenFlavor = 0;
    vector<int> *JetInfo_GenHadronFlavor = 0;
    float MetInfo_Pt = 0;
    float MetInfo_Phi = 0;
    float MetInfo_Px = 0;
    float MetInfo_Py = 0;
    float MetInfo_SumET = 0;
    int GenPartInfo_size = 0;
    vector<float> *GenPartInfo_Pt = 0;
    vector<float> *GenPartInfo_Eta = 0;
    vector<float> *GenPartInfo_Phi = 0;
    vector<float> *GenPartInfo_Mass = 0;
    vector<int> *GenPartInfo_PdgID = 0;
    vector<int> *GenPartInfo_Status = 0;
    vector<int> *GenPartInfo_nMo = 0;
    vector<int> *GenPartInfo_nDa = 0;
    vector<bool> *GenPartInfo_isHardProcess = 0;
    vector<bool> *GenPartInfo_fromHardProcessFinalState = 0;
    vector<bool> *GenPartInfo_isPromptFinalState = 0;
    vector<bool> *GenPartInfo_isDirectPromptTauDecayProductFinalState = 0;
    vector<int> *GenPartInfo_MomPdgID = 0;
    vector<int> *GenPartInfo_MomStatus = 0;
    vector<float> *GenPartInfo_MomPt = 0;
    vector<float> *GenPartInfo_MomEta = 0;
    vector<float> *GenPartInfo_MomPhi = 0;
    vector<float> *GenPartInfo_MomMass = 0;
    int idx_perm = 0;
    int N_perm = 0;
    bool truth_matched = 0;
    int idx_Wjet1 = 0;
    int idx_Wjet2 = 0;
    int idx_M1bjet = 0;
    float InvMass_W = 0;
    float InvMass_M1 = 0;
    // variables for MVA score calculation
    float bJet_M1_Pt = 0;
    float bJet_M1_Eta = 0;
    float bJet_M1_Phi = 0;
    float bJet_M1_E = 0;
    float bJet_M1_btag = 0;
    float Jet1_W_Pt = 0;
    float Jet1_W_Eta = 0;
    float Jet1_W_Phi = 0;
    float Jet1_W_E = 0;
    float Jet1_W_btag = 0;
    float Jet2_W_Pt = 0;
    float Jet2_W_Eta = 0;
    float Jet2_W_Phi = 0;
    float Jet2_W_E = 0;
    float Jet2_W_btag = 0;

    // additional variables for output tree
    float SThad_recoMVA = 0;

    // set input tree branches
    inTree->SetBranchAddress("EvtInfo.NPu", &EvtInfo_NPu);
    inTree->SetBranchAddress("EvtInfo.NVtx", &EvtInfo_NVtx);
    inTree->SetBranchAddress("EvtInfo.passTrigger", &EvtInfo_passTrigger);
    inTree->SetBranchAddress("EvtInfo.genweight", &EvtInfo_genweight);
    inTree->SetBranchAddress("EvtInfo.Rho", &EvtInfo_Rho);
    inTree->SetBranchAddress("EvtInfo.PVz", &EvtInfo_PVz);
    inTree->SetBranchAddress("EvtInfo.BSsigmaz", &EvtInfo_BSsigmaz);
    inTree->SetBranchAddress("EvtInfo.Flag_HBHENoiseFilter", &EvtInfo_Flag_HBHENoiseFilter);
    inTree->SetBranchAddress("EvtInfo.Flag_HBHENoiseIsoFilter", &EvtInfo_Flag_HBHENoiseIsoFilter);
    inTree->SetBranchAddress("EvtInfo.Flag_EcalDeadCellTriggerPrimitiveFilter", &EvtInfo_Flag_EcalDeadCellTriggerPrimitiveFilter);
    inTree->SetBranchAddress("EvtInfo.Flag_goodVertices", &EvtInfo_Flag_goodVertices);
    inTree->SetBranchAddress("EvtInfo.Flag_globalSuperTightHalo2016Filter", &EvtInfo_Flag_globalSuperTightHalo2016Filter);
    inTree->SetBranchAddress("EvtInfo.Flag_BadPFMuonFilter", &EvtInfo_Flag_BadPFMuonFilter);
    inTree->SetBranchAddress("EvtInfo.Flag_eeBadScFilter", &EvtInfo_Flag_eeBadScFilter);
    inTree->SetBranchAddress("EvtInfo.ecalBadCalibReducedMINIAODFilter", &EvtInfo_ecalBadCalibReducedMINIAODFilter);
    inTree->SetBranchAddress("DiPhoInfo.mass", &DiPhoInfo_mass);
    inTree->SetBranchAddress("DiPhoInfo.pt", &DiPhoInfo_pt);
    inTree->SetBranchAddress("DiPhoInfo.leadPt", &DiPhoInfo_leadPt);
    inTree->SetBranchAddress("DiPhoInfo.leadEta", &DiPhoInfo_leadEta);
    inTree->SetBranchAddress("DiPhoInfo.leadPhi", &DiPhoInfo_leadPhi);
    inTree->SetBranchAddress("DiPhoInfo.leadE", &DiPhoInfo_leadE);
    inTree->SetBranchAddress("DiPhoInfo.leadEtaSC", &DiPhoInfo_leadEtaSC);
    inTree->SetBranchAddress("DiPhoInfo.leadPhiSC", &DiPhoInfo_leadPhiSC);
    inTree->SetBranchAddress("DiPhoInfo.leadsigEOverE", &DiPhoInfo_leadsigEOverE);
    inTree->SetBranchAddress("DiPhoInfo.leadR9", &DiPhoInfo_leadR9);
    inTree->SetBranchAddress("DiPhoInfo.leadsieie", &DiPhoInfo_leadsieie);
    inTree->SetBranchAddress("DiPhoInfo.leadhoe", &DiPhoInfo_leadhoe);
    inTree->SetBranchAddress("DiPhoInfo.leadIDMVA", &DiPhoInfo_leadIDMVA);
    inTree->SetBranchAddress("DiPhoInfo.leadIsEB", &DiPhoInfo_leadIsEB);
    inTree->SetBranchAddress("DiPhoInfo.leadIsEE", &DiPhoInfo_leadIsEE);
    inTree->SetBranchAddress("DiPhoInfo.leadhasPixelSeed", &DiPhoInfo_leadhasPixelSeed);
    inTree->SetBranchAddress("DiPhoInfo.leadGenMatch", &DiPhoInfo_leadGenMatch);
    inTree->SetBranchAddress("DiPhoInfo.leadGenMatchType", &DiPhoInfo_leadGenMatchType);
    inTree->SetBranchAddress("DiPhoInfo.subleadPt", &DiPhoInfo_subleadPt);
    inTree->SetBranchAddress("DiPhoInfo.subleadEta", &DiPhoInfo_subleadEta);
    inTree->SetBranchAddress("DiPhoInfo.subleadPhi", &DiPhoInfo_subleadPhi);
    inTree->SetBranchAddress("DiPhoInfo.subleadE", &DiPhoInfo_subleadE);
    inTree->SetBranchAddress("DiPhoInfo.subleadEtaSC", &DiPhoInfo_subleadEtaSC);
    inTree->SetBranchAddress("DiPhoInfo.subleadPhiSC", &DiPhoInfo_subleadPhiSC);
    inTree->SetBranchAddress("DiPhoInfo.subleadsigEOverE", &DiPhoInfo_subleadsigEOverE);
    inTree->SetBranchAddress("DiPhoInfo.subleadR9", &DiPhoInfo_subleadR9);
    inTree->SetBranchAddress("DiPhoInfo.subleadsieie", &DiPhoInfo_subleadsieie);
    inTree->SetBranchAddress("DiPhoInfo.subleadhoe", &DiPhoInfo_subleadhoe);
    inTree->SetBranchAddress("DiPhoInfo.subleadIDMVA", &DiPhoInfo_subleadIDMVA);
    inTree->SetBranchAddress("DiPhoInfo.subleadIsEB", &DiPhoInfo_subleadIsEB);
    inTree->SetBranchAddress("DiPhoInfo.subleadIsEE", &DiPhoInfo_subleadIsEE);
    inTree->SetBranchAddress("DiPhoInfo.subleadhasPixelSeed", &DiPhoInfo_subleadhasPixelSeed);
    inTree->SetBranchAddress("DiPhoInfo.subleadGenMatch", &DiPhoInfo_subleadGenMatch);
    inTree->SetBranchAddress("DiPhoInfo.subleadGenMatchType", &DiPhoInfo_subleadGenMatchType);
    inTree->SetBranchAddress("DiPhoInfo.diphotonMVA", &DiPhoInfo_diphotonMVA);
    inTree->SetBranchAddress("DiPhoInfo.SelectedVz", &DiPhoInfo_SelectedVz);
    inTree->SetBranchAddress("DiPhoInfo.GenVz", &DiPhoInfo_GenVz);
    inTree->SetBranchAddress("DiPhoInfo.centralWeight", &DiPhoInfo_centralWeight);
    inTree->SetBranchAddress("ElecInfo.Size", &ElecInfo_Size);
    inTree->SetBranchAddress("ElecInfo.Charge", &ElecInfo_Charge);
    inTree->SetBranchAddress("ElecInfo.Pt", &ElecInfo_Pt);
    inTree->SetBranchAddress("ElecInfo.Eta", &ElecInfo_Eta);
    inTree->SetBranchAddress("ElecInfo.Phi", &ElecInfo_Phi);
    inTree->SetBranchAddress("ElecInfo.Energy", &ElecInfo_Energy);
    inTree->SetBranchAddress("ElecInfo.EtaSC", &ElecInfo_EtaSC);
    inTree->SetBranchAddress("ElecInfo.PhiSC", &ElecInfo_PhiSC);
    inTree->SetBranchAddress("ElecInfo.GsfTrackDz", &ElecInfo_GsfTrackDz);
    inTree->SetBranchAddress("ElecInfo.GsfTrackDxy", &ElecInfo_GsfTrackDxy);
    inTree->SetBranchAddress("ElecInfo.EGMCutBasedIDVeto", &ElecInfo_EGMCutBasedIDVeto);
    inTree->SetBranchAddress("ElecInfo.EGMCutBasedIDLoose", &ElecInfo_EGMCutBasedIDLoose);
    inTree->SetBranchAddress("ElecInfo.EGMCutBasedIDMedium", &ElecInfo_EGMCutBasedIDMedium);
    inTree->SetBranchAddress("ElecInfo.EGMCutBasedIDTight", &ElecInfo_EGMCutBasedIDTight);
    inTree->SetBranchAddress("ElecInfo.passConvVeto", &ElecInfo_passConvVeto);
    inTree->SetBranchAddress("ElecInfo.fggPhoVeto", &ElecInfo_fggPhoVeto);
    inTree->SetBranchAddress("ElecInfo.EnergyCorrFactor", &ElecInfo_EnergyCorrFactor);
    inTree->SetBranchAddress("ElecInfo.EnergyPostCorrErr", &ElecInfo_EnergyPostCorrErr);
    inTree->SetBranchAddress("ElecInfo.GenMatch", &ElecInfo_GenMatch);
    inTree->SetBranchAddress("ElecInfo.GenPdgID", &ElecInfo_GenPdgID);
    inTree->SetBranchAddress("ElecInfo.GenPt", &ElecInfo_GenPt);
    inTree->SetBranchAddress("ElecInfo.GenEta", &ElecInfo_GenEta);
    inTree->SetBranchAddress("ElecInfo.GenPhi", &ElecInfo_GenPhi);
    inTree->SetBranchAddress("MuonInfo.Size", &MuonInfo_Size);
    inTree->SetBranchAddress("MuonInfo.Charge", &MuonInfo_Charge);
    inTree->SetBranchAddress("MuonInfo.MuonType", &MuonInfo_MuonType);
    inTree->SetBranchAddress("MuonInfo.Pt", &MuonInfo_Pt);
    inTree->SetBranchAddress("MuonInfo.Eta", &MuonInfo_Eta);
    inTree->SetBranchAddress("MuonInfo.Phi", &MuonInfo_Phi);
    inTree->SetBranchAddress("MuonInfo.Energy", &MuonInfo_Energy);
    inTree->SetBranchAddress("MuonInfo.BestTrackDz", &MuonInfo_BestTrackDz);
    inTree->SetBranchAddress("MuonInfo.BestTrackDxy", &MuonInfo_BestTrackDxy);
    inTree->SetBranchAddress("MuonInfo.PFIsoDeltaBetaCorrR04", &MuonInfo_PFIsoDeltaBetaCorrR04);
    inTree->SetBranchAddress("MuonInfo.TrackerBasedIsoR03", &MuonInfo_TrackerBasedIsoR03);
    inTree->SetBranchAddress("MuonInfo.CutBasedIdMedium", &MuonInfo_CutBasedIdMedium);
    inTree->SetBranchAddress("MuonInfo.CutBasedIdTight", &MuonInfo_CutBasedIdTight);
    inTree->SetBranchAddress("MuonInfo.CutBasedIdTight_bestVtx", &MuonInfo_CutBasedIdTight_bestVtx);
    inTree->SetBranchAddress("MuonInfo.GenMatch", &MuonInfo_GenMatch);
    inTree->SetBranchAddress("MuonInfo.GenPdgID", &MuonInfo_GenPdgID);
    inTree->SetBranchAddress("MuonInfo.GenPt", &MuonInfo_GenPt);
    inTree->SetBranchAddress("MuonInfo.GenEta", &MuonInfo_GenEta);
    inTree->SetBranchAddress("MuonInfo.GenPhi", &MuonInfo_GenPhi);
    inTree->SetBranchAddress("jets_size", &jets_size);
    inTree->SetBranchAddress("JetInfo.Pt", &JetInfo_Pt);
    inTree->SetBranchAddress("JetInfo.Eta", &JetInfo_Eta);
    inTree->SetBranchAddress("JetInfo.Phi", &JetInfo_Phi);
    inTree->SetBranchAddress("JetInfo.Mass", &JetInfo_Mass);
    inTree->SetBranchAddress("JetInfo.Energy", &JetInfo_Energy);
    inTree->SetBranchAddress("JetInfo.PtRaw", &JetInfo_PtRaw);
    inTree->SetBranchAddress("JetInfo.QGL", &JetInfo_QGL);
    inTree->SetBranchAddress("JetInfo.RMS", &JetInfo_RMS);
    inTree->SetBranchAddress("JetInfo.puJetIdMVA", &JetInfo_puJetIdMVA);
    inTree->SetBranchAddress("JetInfo.passesPuJetIdLoose", &JetInfo_passesPuJetIdLoose);
    inTree->SetBranchAddress("JetInfo.passesPuJetIdMedium", &JetInfo_passesPuJetIdMedium);
    inTree->SetBranchAddress("JetInfo.passesPuJetIdTight", &JetInfo_passesPuJetIdTight);
    inTree->SetBranchAddress("JetInfo.GenJetMatch", &JetInfo_GenJetMatch);
    inTree->SetBranchAddress("JetInfo.pfCombinedInclusiveSecondaryVertexV2BJetTags", &JetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags);
    inTree->SetBranchAddress("JetInfo.pfCombinedMVAV2BJetTags", &JetInfo_pfCombinedMVAV2BJetTags);
    inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probb", &JetInfo_pfDeepCSVJetTags_probb);
    inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probbb", &JetInfo_pfDeepCSVJetTags_probbb);
    inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probc", &JetInfo_pfDeepCSVJetTags_probc);
    inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probudsg", &JetInfo_pfDeepCSVJetTags_probudsg);
    inTree->SetBranchAddress("JetInfo.pfDeepFlavourJetTags_probb", &JetInfo_pfDeepFlavourJetTags_probb);
    inTree->SetBranchAddress("JetInfo.pfDeepFlavourJetTags_probbb", &JetInfo_pfDeepFlavourJetTags_probbb);
    inTree->SetBranchAddress("JetInfo.pfDeepFlavourJetTags_probc", &JetInfo_pfDeepFlavourJetTags_probc);
    inTree->SetBranchAddress("JetInfo.pfDeepFlavourJetTags_probuds", &JetInfo_pfDeepFlavourJetTags_probuds);
    inTree->SetBranchAddress("JetInfo.pfDeepFlavourJetTags_probg", &JetInfo_pfDeepFlavourJetTags_probg);
    inTree->SetBranchAddress("JetInfo.pfDeepFlavourJetTags_problepb", &JetInfo_pfDeepFlavourJetTags_problepb);
    inTree->SetBranchAddress("JetInfo.JECScale", &JetInfo_JECScale);
    inTree->SetBranchAddress("JetInfo.JERScale", &JetInfo_JERScale);
    inTree->SetBranchAddress("JetInfo.GenPartonMatch", &JetInfo_GenPartonMatch);
    inTree->SetBranchAddress("JetInfo.GenPt", &JetInfo_GenPt);
    inTree->SetBranchAddress("JetInfo.GenEta", &JetInfo_GenEta);
    inTree->SetBranchAddress("JetInfo.GenPhi", &JetInfo_GenPhi);
    inTree->SetBranchAddress("JetInfo.GenPdgID", &JetInfo_GenPdgID);
    inTree->SetBranchAddress("JetInfo.GenFlavor", &JetInfo_GenFlavor);
    inTree->SetBranchAddress("JetInfo.GenHadronFlavor", &JetInfo_GenHadronFlavor);
    inTree->SetBranchAddress("MetInfo.Pt", &MetInfo_Pt);
    inTree->SetBranchAddress("MetInfo.Phi", &MetInfo_Phi);
    inTree->SetBranchAddress("MetInfo.Px", &MetInfo_Px);
    inTree->SetBranchAddress("MetInfo.Py", &MetInfo_Py);
    inTree->SetBranchAddress("MetInfo.SumET", &MetInfo_SumET);
    inTree->SetBranchAddress("GenPartInfo.size", &GenPartInfo_size);
    inTree->SetBranchAddress("GenPartInfo.Pt", &GenPartInfo_Pt);
    inTree->SetBranchAddress("GenPartInfo.Eta", &GenPartInfo_Eta);
    inTree->SetBranchAddress("GenPartInfo.Phi", &GenPartInfo_Phi);
    inTree->SetBranchAddress("GenPartInfo.Mass", &GenPartInfo_Mass);
    inTree->SetBranchAddress("GenPartInfo.PdgID", &GenPartInfo_PdgID);
    inTree->SetBranchAddress("GenPartInfo.Status", &GenPartInfo_Status);
    inTree->SetBranchAddress("GenPartInfo.nMo", &GenPartInfo_nMo);
    inTree->SetBranchAddress("GenPartInfo.nDa", &GenPartInfo_nDa);
    inTree->SetBranchAddress("GenPartInfo.isHardProcess", &GenPartInfo_isHardProcess);
    inTree->SetBranchAddress("GenPartInfo.fromHardProcessFinalState", &GenPartInfo_fromHardProcessFinalState);
    inTree->SetBranchAddress("GenPartInfo.isPromptFinalState", &GenPartInfo_isPromptFinalState);
    inTree->SetBranchAddress("GenPartInfo.isDirectPromptTauDecayProductFinalState", &GenPartInfo_isDirectPromptTauDecayProductFinalState);
    inTree->SetBranchAddress("GenPartInfo.MomPdgID", &GenPartInfo_MomPdgID);
    inTree->SetBranchAddress("GenPartInfo.MomStatus", &GenPartInfo_MomStatus);
    inTree->SetBranchAddress("GenPartInfo.MomPt", &GenPartInfo_MomPt);
    inTree->SetBranchAddress("GenPartInfo.MomEta", &GenPartInfo_MomEta);
    inTree->SetBranchAddress("GenPartInfo.MomPhi", &GenPartInfo_MomPhi);
    inTree->SetBranchAddress("GenPartInfo.MomMass", &GenPartInfo_MomMass);
    inTree->SetBranchAddress("idx_perm", &idx_perm);
    inTree->SetBranchAddress("N_perm", &N_perm);
    inTree->SetBranchAddress("truth_matched", &truth_matched);
    inTree->SetBranchAddress("idx_Wjet1", &idx_Wjet1);
    inTree->SetBranchAddress("idx_Wjet2", &idx_Wjet2);
    inTree->SetBranchAddress("idx_M1bjet", &idx_M1bjet);
    inTree->SetBranchAddress("InvMass_W", &InvMass_W);
    inTree->SetBranchAddress("InvMass_M1", &InvMass_M1);
    inTree->SetBranchAddress("bJet_M1_Pt", &bJet_M1_Pt);
    inTree->SetBranchAddress("bJet_M1_Eta", &bJet_M1_Eta);
    inTree->SetBranchAddress("bJet_M1_Phi", &bJet_M1_Phi);
    inTree->SetBranchAddress("bJet_M1_E", &bJet_M1_E);
    inTree->SetBranchAddress("bJet_M1_btag", &bJet_M1_btag);
    inTree->SetBranchAddress("Jet1_W_Pt", &Jet1_W_Pt);
    inTree->SetBranchAddress("Jet1_W_Eta", &Jet1_W_Eta);
    inTree->SetBranchAddress("Jet1_W_Phi", &Jet1_W_Phi);
    inTree->SetBranchAddress("Jet1_W_E", &Jet1_W_E);
    inTree->SetBranchAddress("Jet1_W_btag", &Jet1_W_btag);
    inTree->SetBranchAddress("Jet2_W_Pt", &Jet2_W_Pt);
    inTree->SetBranchAddress("Jet2_W_Eta", &Jet2_W_Eta);
    inTree->SetBranchAddress("Jet2_W_Phi", &Jet2_W_Phi);
    inTree->SetBranchAddress("Jet2_W_E", &Jet2_W_E);
    inTree->SetBranchAddress("Jet2_W_btag", &Jet2_W_btag);

    cout << "Input tree is successfully set.\n";

    // set output tree branches
    outTree->Branch("EvtInfo.NPu", &EvtInfo_NPu);
    outTree->Branch("EvtInfo.NVtx", &EvtInfo_NVtx);
    outTree->Branch("EvtInfo.passTrigger", &EvtInfo_passTrigger);
    outTree->Branch("EvtInfo.genweight", &EvtInfo_genweight);
    outTree->Branch("EvtInfo.Rho", &EvtInfo_Rho);
    outTree->Branch("EvtInfo.PVz", &EvtInfo_PVz);
    outTree->Branch("EvtInfo.BSsigmaz", &EvtInfo_BSsigmaz);
    outTree->Branch("EvtInfo.Flag_HBHENoiseFilter", &EvtInfo_Flag_HBHENoiseFilter);
    outTree->Branch("EvtInfo.Flag_HBHENoiseIsoFilter", &EvtInfo_Flag_HBHENoiseIsoFilter);
    outTree->Branch("EvtInfo.Flag_EcalDeadCellTriggerPrimitiveFilter", &EvtInfo_Flag_EcalDeadCellTriggerPrimitiveFilter);
    outTree->Branch("EvtInfo.Flag_goodVertices", &EvtInfo_Flag_goodVertices);
    outTree->Branch("EvtInfo.Flag_globalSuperTightHalo2016Filter", &EvtInfo_Flag_globalSuperTightHalo2016Filter);
    outTree->Branch("EvtInfo.Flag_BadPFMuonFilter", &EvtInfo_Flag_BadPFMuonFilter);
    outTree->Branch("EvtInfo.Flag_eeBadScFilter", &EvtInfo_Flag_eeBadScFilter);
    outTree->Branch("EvtInfo.ecalBadCalibReducedMINIAODFilter", &EvtInfo_ecalBadCalibReducedMINIAODFilter);
    outTree->Branch("DiPhoInfo.mass", &DiPhoInfo_mass);
    outTree->Branch("DiPhoInfo.pt", &DiPhoInfo_pt);
    outTree->Branch("DiPhoInfo.leadPt", &DiPhoInfo_leadPt);
    outTree->Branch("DiPhoInfo.leadEta", &DiPhoInfo_leadEta);
    outTree->Branch("DiPhoInfo.leadPhi", &DiPhoInfo_leadPhi);
    outTree->Branch("DiPhoInfo.leadE", &DiPhoInfo_leadE);
    outTree->Branch("DiPhoInfo.leadEtaSC", &DiPhoInfo_leadEtaSC);
    outTree->Branch("DiPhoInfo.leadPhiSC", &DiPhoInfo_leadPhiSC);
    outTree->Branch("DiPhoInfo.leadsigEOverE", &DiPhoInfo_leadsigEOverE);
    outTree->Branch("DiPhoInfo.leadR9", &DiPhoInfo_leadR9);
    outTree->Branch("DiPhoInfo.leadsieie", &DiPhoInfo_leadsieie);
    outTree->Branch("DiPhoInfo.leadhoe", &DiPhoInfo_leadhoe);
    outTree->Branch("DiPhoInfo.leadIDMVA", &DiPhoInfo_leadIDMVA);
    outTree->Branch("DiPhoInfo.leadIsEB", &DiPhoInfo_leadIsEB);
    outTree->Branch("DiPhoInfo.leadIsEE", &DiPhoInfo_leadIsEE);
    outTree->Branch("DiPhoInfo.leadhasPixelSeed", &DiPhoInfo_leadhasPixelSeed);
    outTree->Branch("DiPhoInfo.leadGenMatch", &DiPhoInfo_leadGenMatch);
    outTree->Branch("DiPhoInfo.leadGenMatchType", &DiPhoInfo_leadGenMatchType);
    outTree->Branch("DiPhoInfo.subleadPt", &DiPhoInfo_subleadPt);
    outTree->Branch("DiPhoInfo.subleadEta", &DiPhoInfo_subleadEta);
    outTree->Branch("DiPhoInfo.subleadPhi", &DiPhoInfo_subleadPhi);
    outTree->Branch("DiPhoInfo.subleadE", &DiPhoInfo_subleadE);
    outTree->Branch("DiPhoInfo.subleadEtaSC", &DiPhoInfo_subleadEtaSC);
    outTree->Branch("DiPhoInfo.subleadPhiSC", &DiPhoInfo_subleadPhiSC);
    outTree->Branch("DiPhoInfo.subleadsigEOverE", &DiPhoInfo_subleadsigEOverE);
    outTree->Branch("DiPhoInfo.subleadR9", &DiPhoInfo_subleadR9);
    outTree->Branch("DiPhoInfo.subleadsieie", &DiPhoInfo_subleadsieie);
    outTree->Branch("DiPhoInfo.subleadhoe", &DiPhoInfo_subleadhoe);
    outTree->Branch("DiPhoInfo.subleadIDMVA", &DiPhoInfo_subleadIDMVA);
    outTree->Branch("DiPhoInfo.subleadIsEB", &DiPhoInfo_subleadIsEB);
    outTree->Branch("DiPhoInfo.subleadIsEE", &DiPhoInfo_subleadIsEE);
    outTree->Branch("DiPhoInfo.subleadhasPixelSeed", &DiPhoInfo_subleadhasPixelSeed);
    outTree->Branch("DiPhoInfo.subleadGenMatch", &DiPhoInfo_subleadGenMatch);
    outTree->Branch("DiPhoInfo.subleadGenMatchType", &DiPhoInfo_subleadGenMatchType);
    outTree->Branch("DiPhoInfo.diphotonMVA", &DiPhoInfo_diphotonMVA);
    outTree->Branch("DiPhoInfo.SelectedVz", &DiPhoInfo_SelectedVz);
    outTree->Branch("DiPhoInfo.GenVz", &DiPhoInfo_GenVz);
    outTree->Branch("DiPhoInfo.centralWeight", &DiPhoInfo_centralWeight);
    outTree->Branch("ElecInfo.Size", &ElecInfo_Size);
    outTree->Branch("ElecInfo.Charge", &ElecInfo_Charge);
    outTree->Branch("ElecInfo.Pt", &ElecInfo_Pt);
    outTree->Branch("ElecInfo.Eta", &ElecInfo_Eta);
    outTree->Branch("ElecInfo.Phi", &ElecInfo_Phi);
    outTree->Branch("ElecInfo.Energy", &ElecInfo_Energy);
    outTree->Branch("ElecInfo.EtaSC", &ElecInfo_EtaSC);
    outTree->Branch("ElecInfo.PhiSC", &ElecInfo_PhiSC);
    outTree->Branch("ElecInfo.GsfTrackDz", &ElecInfo_GsfTrackDz);
    outTree->Branch("ElecInfo.GsfTrackDxy", &ElecInfo_GsfTrackDxy);
    outTree->Branch("ElecInfo.EGMCutBasedIDVeto", &ElecInfo_EGMCutBasedIDVeto);
    outTree->Branch("ElecInfo.EGMCutBasedIDLoose", &ElecInfo_EGMCutBasedIDLoose);
    outTree->Branch("ElecInfo.EGMCutBasedIDMedium", &ElecInfo_EGMCutBasedIDMedium);
    outTree->Branch("ElecInfo.EGMCutBasedIDTight", &ElecInfo_EGMCutBasedIDTight);
    outTree->Branch("ElecInfo.passConvVeto", &ElecInfo_passConvVeto);
    outTree->Branch("ElecInfo.fggPhoVeto", &ElecInfo_fggPhoVeto);
    outTree->Branch("ElecInfo.EnergyCorrFactor", &ElecInfo_EnergyCorrFactor);
    outTree->Branch("ElecInfo.EnergyPostCorrErr", &ElecInfo_EnergyPostCorrErr);
    outTree->Branch("ElecInfo.GenMatch", &ElecInfo_GenMatch);
    outTree->Branch("ElecInfo.GenPdgID", &ElecInfo_GenPdgID);
    outTree->Branch("ElecInfo.GenPt", &ElecInfo_GenPt);
    outTree->Branch("ElecInfo.GenEta", &ElecInfo_GenEta);
    outTree->Branch("ElecInfo.GenPhi", &ElecInfo_GenPhi);
    outTree->Branch("MuonInfo.Size", &MuonInfo_Size);
    outTree->Branch("MuonInfo.Charge", &MuonInfo_Charge);
    outTree->Branch("MuonInfo.MuonType", &MuonInfo_MuonType);
    outTree->Branch("MuonInfo.Pt", &MuonInfo_Pt);
    outTree->Branch("MuonInfo.Eta", &MuonInfo_Eta);
    outTree->Branch("MuonInfo.Phi", &MuonInfo_Phi);
    outTree->Branch("MuonInfo.Energy", &MuonInfo_Energy);
    outTree->Branch("MuonInfo.BestTrackDz", &MuonInfo_BestTrackDz);
    outTree->Branch("MuonInfo.BestTrackDxy", &MuonInfo_BestTrackDxy);
    outTree->Branch("MuonInfo.PFIsoDeltaBetaCorrR04", &MuonInfo_PFIsoDeltaBetaCorrR04);
    outTree->Branch("MuonInfo.TrackerBasedIsoR03", &MuonInfo_TrackerBasedIsoR03);
    outTree->Branch("MuonInfo.CutBasedIdMedium", &MuonInfo_CutBasedIdMedium);
    outTree->Branch("MuonInfo.CutBasedIdTight", &MuonInfo_CutBasedIdTight);
    outTree->Branch("MuonInfo.CutBasedIdTight_bestVtx", &MuonInfo_CutBasedIdTight_bestVtx);
    outTree->Branch("MuonInfo.GenMatch", &MuonInfo_GenMatch);
    outTree->Branch("MuonInfo.GenPdgID", &MuonInfo_GenPdgID);
    outTree->Branch("MuonInfo.GenPt", &MuonInfo_GenPt);
    outTree->Branch("MuonInfo.GenEta", &MuonInfo_GenEta);
    outTree->Branch("MuonInfo.GenPhi", &MuonInfo_GenPhi);
    outTree->Branch("jets_size", &jets_size);
    outTree->Branch("JetInfo.Pt", &JetInfo_Pt);
    outTree->Branch("JetInfo.Eta", &JetInfo_Eta);
    outTree->Branch("JetInfo.Phi", &JetInfo_Phi);
    outTree->Branch("JetInfo.Mass", &JetInfo_Mass);
    outTree->Branch("JetInfo.Energy", &JetInfo_Energy);
    outTree->Branch("JetInfo.PtRaw", &JetInfo_PtRaw);
    outTree->Branch("JetInfo.QGL", &JetInfo_QGL);
    outTree->Branch("JetInfo.RMS", &JetInfo_RMS);
    outTree->Branch("JetInfo.puJetIdMVA", &JetInfo_puJetIdMVA);
    outTree->Branch("JetInfo.passesPuJetIdLoose", &JetInfo_passesPuJetIdLoose);
    outTree->Branch("JetInfo.passesPuJetIdMedium", &JetInfo_passesPuJetIdMedium);
    outTree->Branch("JetInfo.passesPuJetIdTight", &JetInfo_passesPuJetIdTight);
    outTree->Branch("JetInfo.GenJetMatch", &JetInfo_GenJetMatch);
    outTree->Branch("JetInfo.pfCombinedInclusiveSecondaryVertexV2BJetTags", &JetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags);
    outTree->Branch("JetInfo.pfCombinedMVAV2BJetTags", &JetInfo_pfCombinedMVAV2BJetTags);
    outTree->Branch("JetInfo.pfDeepCSVJetTags_probb", &JetInfo_pfDeepCSVJetTags_probb);
    outTree->Branch("JetInfo.pfDeepCSVJetTags_probbb", &JetInfo_pfDeepCSVJetTags_probbb);
    outTree->Branch("JetInfo.pfDeepCSVJetTags_probc", &JetInfo_pfDeepCSVJetTags_probc);
    outTree->Branch("JetInfo.pfDeepCSVJetTags_probudsg", &JetInfo_pfDeepCSVJetTags_probudsg);
    outTree->Branch("JetInfo.pfDeepFlavourJetTags_probb", &JetInfo_pfDeepFlavourJetTags_probb);
    outTree->Branch("JetInfo.pfDeepFlavourJetTags_probbb", &JetInfo_pfDeepFlavourJetTags_probbb);
    outTree->Branch("JetInfo.pfDeepFlavourJetTags_probc", &JetInfo_pfDeepFlavourJetTags_probc);
    outTree->Branch("JetInfo.pfDeepFlavourJetTags_probuds", &JetInfo_pfDeepFlavourJetTags_probuds);
    outTree->Branch("JetInfo.pfDeepFlavourJetTags_probg", &JetInfo_pfDeepFlavourJetTags_probg);
    outTree->Branch("JetInfo.pfDeepFlavourJetTags_problepb", &JetInfo_pfDeepFlavourJetTags_problepb);
    outTree->Branch("JetInfo.JECScale", &JetInfo_JECScale);
    outTree->Branch("JetInfo.JERScale", &JetInfo_JERScale);
    outTree->Branch("JetInfo.GenPartonMatch", &JetInfo_GenPartonMatch);
    outTree->Branch("JetInfo.GenPt", &JetInfo_GenPt);
    outTree->Branch("JetInfo.GenEta", &JetInfo_GenEta);
    outTree->Branch("JetInfo.GenPhi", &JetInfo_GenPhi);
    outTree->Branch("JetInfo.GenPdgID", &JetInfo_GenPdgID);
    outTree->Branch("JetInfo.GenFlavor", &JetInfo_GenFlavor);
    outTree->Branch("JetInfo.GenHadronFlavor", &JetInfo_GenHadronFlavor);
    outTree->Branch("MetInfo.Pt", &MetInfo_Pt);
    outTree->Branch("MetInfo.Phi", &MetInfo_Phi);
    outTree->Branch("MetInfo.Px", &MetInfo_Px);
    outTree->Branch("MetInfo.Py", &MetInfo_Py);
    outTree->Branch("MetInfo.SumET", &MetInfo_SumET);
    outTree->Branch("GenPartInfo.size", &GenPartInfo_size);
    outTree->Branch("GenPartInfo.Pt", &GenPartInfo_Pt);
    outTree->Branch("GenPartInfo.Eta", &GenPartInfo_Eta);
    outTree->Branch("GenPartInfo.Phi", &GenPartInfo_Phi);
    outTree->Branch("GenPartInfo.Mass", &GenPartInfo_Mass);
    outTree->Branch("GenPartInfo.PdgID", &GenPartInfo_PdgID);
    outTree->Branch("GenPartInfo.Status", &GenPartInfo_Status);
    outTree->Branch("GenPartInfo.nMo", &GenPartInfo_nMo);
    outTree->Branch("GenPartInfo.nDa", &GenPartInfo_nDa);
    outTree->Branch("GenPartInfo.isHardProcess", &GenPartInfo_isHardProcess);
    outTree->Branch("GenPartInfo.fromHardProcessFinalState", &GenPartInfo_fromHardProcessFinalState);
    outTree->Branch("GenPartInfo.isPromptFinalState", &GenPartInfo_isPromptFinalState);
    outTree->Branch("GenPartInfo.isDirectPromptTauDecayProductFinalState", &GenPartInfo_isDirectPromptTauDecayProductFinalState);
    outTree->Branch("GenPartInfo.MomPdgID", &GenPartInfo_MomPdgID);
    outTree->Branch("GenPartInfo.MomStatus", &GenPartInfo_MomStatus);
    outTree->Branch("GenPartInfo.MomPt", &GenPartInfo_MomPt);
    outTree->Branch("GenPartInfo.MomEta", &GenPartInfo_MomEta);
    outTree->Branch("GenPartInfo.MomPhi", &GenPartInfo_MomPhi);
    outTree->Branch("GenPartInfo.MomMass", &GenPartInfo_MomMass);
    outTree->Branch("idx_perm", &idx_perm);
    outTree->Branch("N_perm", &N_perm);
    outTree->Branch("truth_matched", &truth_matched);
    outTree->Branch("idx_Wjet1", &idx_Wjet1);
    outTree->Branch("idx_Wjet2", &idx_Wjet2);
    outTree->Branch("idx_M1bjet", &idx_M1bjet);
    outTree->Branch("InvMass_W", &InvMass_W);
    outTree->Branch("InvMass_M1", &InvMass_M1);
    outTree->Branch("bJet_M1_Pt", &bJet_M1_Pt);
    outTree->Branch("bJet_M1_Eta", &bJet_M1_Eta);
    outTree->Branch("bJet_M1_Phi", &bJet_M1_Phi);
    outTree->Branch("bJet_M1_E", &bJet_M1_E);
    outTree->Branch("bJet_M1_btag", &bJet_M1_btag);
    outTree->Branch("Jet1_W_Pt", &Jet1_W_Pt);
    outTree->Branch("Jet1_W_Eta", &Jet1_W_Eta);
    outTree->Branch("Jet1_W_Phi", &Jet1_W_Phi);
    outTree->Branch("Jet1_W_E", &Jet1_W_E);
    outTree->Branch("Jet1_W_btag", &Jet1_W_btag);
    outTree->Branch("Jet2_W_Pt", &Jet2_W_Pt);
    outTree->Branch("Jet2_W_Eta", &Jet2_W_Eta);
    outTree->Branch("Jet2_W_Phi", &Jet2_W_Phi);
    outTree->Branch("Jet2_W_E", &Jet2_W_E);
    outTree->Branch("Jet2_W_btag", &Jet2_W_btag);
    // additional variables for output tree
    outTree->Branch("SThad_recoMVA", &SThad_recoMVA);

    cout << "Output tree is successfully set.\n";

    // load TMVA library
    TMVA::Tools::Instance();

    // create TMVA reader
    TMVA::Reader *reader = new TMVA::Reader("V");

    // set discriminating variables
    reader->AddVariable("DiPhoInfo.leadPt", &DiPhoInfo_leadPt);
    reader->AddVariable("DiPhoInfo.leadEta", &DiPhoInfo_leadEta);
    reader->AddVariable("DiPhoInfo.leadPhi", &DiPhoInfo_leadPhi);
    //reader->AddVariable("DiPhoInfo.leadE", &DiPhoInfo_leadE);
    reader->AddVariable("DiPhoInfo.leadIDMVA", &DiPhoInfo_leadIDMVA);
    reader->AddVariable("DiPhoInfo.subleadPt", &DiPhoInfo_subleadPt);
    reader->AddVariable("DiPhoInfo.subleadEta", &DiPhoInfo_subleadEta);
    reader->AddVariable("DiPhoInfo.subleadPhi", &DiPhoInfo_subleadPhi);
    //reader->AddVariable("DiPhoInfo.subleadE", &DiPhoInfo_subleadE);
    reader->AddVariable("DiPhoInfo.subleadIDMVA", &DiPhoInfo_subleadIDMVA);
    reader->AddVariable("bJet_M1_Pt", &bJet_M1_Pt);
    reader->AddVariable("bJet_M1_Eta", &bJet_M1_Eta);
    reader->AddVariable("bJet_M1_Phi", &bJet_M1_Phi);
    //reader->AddVariable("bJet_M1_E", &bJet_M1_E);
    reader->AddVariable("bJet_M1_btag", &bJet_M1_btag);
    reader->AddVariable("Jet1_W_Pt", &Jet1_W_Pt);
    reader->AddVariable("Jet1_W_Eta", &Jet1_W_Eta);
    reader->AddVariable("Jet1_W_Phi", &Jet1_W_Phi);
    //reader->AddVariable("Jet1_W_E", &Jet1_W_E);
    reader->AddVariable("Jet1_W_btag", &Jet1_W_btag);
    reader->AddVariable("Jet2_W_Pt", &Jet2_W_Pt);
    reader->AddVariable("Jet2_W_Eta", &Jet2_W_Eta);
    reader->AddVariable("Jet2_W_Phi", &Jet2_W_Phi);
    //reader->AddVariable("Jet2_W_E", &Jet2_W_E);
    reader->AddVariable("Jet2_W_btag", &Jet2_W_btag);

    // book MVA methods
    reader->BookMVA(method, fweight_name);

    cout << "[INFO] MVA reader is successfully set!\n";

    double score = 0;
    double best_score = -999;
    long best_perm = -1;

    long Nevt_in = inTree->GetEntries();
    long Nevt_out = 0;
    long Nevt_out_sig = 0;
    long Nevt_out_bkg = 0;

    // loop through each event
    for (long evt=0; evt<inTree->GetEntries(); ++evt) {
	if (evt % 10000 == 0) cout << "Processing event #" << evt << "...\n";
	inTree->GetEntry(evt);

	score = reader->EvaluateMVA(method);
	if (score > cut_MVA && score > best_score) {
	    best_score = score;
	    best_perm = evt;
	}

	if ( (idx_perm == N_perm) && (best_perm >= 0) ) {
	    inTree->GetEntry(best_perm);
	    SThad_recoMVA = best_score;
	    outTree->Fill();
	    Nevt_out += 1;
	    if (truth_matched) Nevt_out_sig += 1;
	    else Nevt_out_bkg += 1;

	    best_score = -999;
	    best_perm = -1;
	}
    } // end of event loop

    fout->cd();
    outTree->Write();
    fout->Close();
    cout << "[INFO] Output tree is saved at: " << fout_name << endl;

    cout << "\n[Summary]\n";
    cout << "---Number of input permutations: " << Nevt_in << endl;
    cout << "---Number of output events with method \"" << method <<  "\": " << Nevt_out << endl;
    cout << "---Number of output truth matched events with method \"" << method << "\": " << Nevt_out_sig << endl;
    cout << "---Number of output not truth matched events with method \"" << method << "\": " << Nevt_out_bkg << endl;

    delete reader;

    return 0;
}
