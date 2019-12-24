// Apply part 1 of preselection on 2017 signal MC n-tuples.
// Preselections:
//   Trigger:
//     Diphoton triggers
//   Photon:
//     (1) pt > 35(25) GeV
//     (2) |eta| < 2.5, excluding [1.4442, 1.566] (EE-EB gap)
//   Diphoton:
//     100 < Mgg < 180 GeV, blind signal region 120 < Mgg < 130 GeV
//   Electron:
//     (1) Cut-based medium ID
//     (2) pt > 20 GeV
//     (3) |eta| < 2.4, excluding [1.4442, 1.566] (EE-EB gap)
//     (4) delta_R(electron, photon) > 0.3
//   Muon:
//     (1) Cut-based tight ID
//     (2) pt > 20 GeV
//     (3) |eta| < 2.4
//     (4) Relative PF isolation (R=0.4) < 0.25, (loose WP)
//     (5) delta_R(muon, photon) > 0.3
//   Jet:
//     (1) pt > 25 GeV
//     (2) |eta| < 2.4
//     (3) delta_R(lepton/photon, jet) > 0.4

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TLorentzVector.h"

#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

int main(int argc, char **argv)
{
    // get input arguments
    char *fin_name = argv[1];
    char *inTree_name = argv[2];
    char *fout_name = argv[3];

    cout << "[INFO] Start processing n-tuple: " << fin_name << endl;

    // read input root files
    TChain *inTree = new TChain(inTree_name);
    inTree->Add(fin_name);

    // create output file
    TFile *fout = new TFile(fout_name, "recreate");
    TTree *outTree = new TTree("flashggStdTree", "");

    // declare input tree variables
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

    cout << "Input tree is successfully set\n";

    // declare & initialize output tree variables
    // only variables that will be modified in preselection need output variables here
    int ElecInfo_Size_sel = 0;
    vector<int> *ElecInfo_Charge_sel = new vector<int>();
    vector<float> *ElecInfo_Pt_sel = new vector<float>();
    vector<float> *ElecInfo_Eta_sel = new vector<float>();
    vector<float> *ElecInfo_Phi_sel = new vector<float>();
    vector<float> *ElecInfo_Energy_sel = new vector<float>();
    vector<float> *ElecInfo_EtaSC_sel = new vector<float>();
    vector<float> *ElecInfo_PhiSC_sel = new vector<float>();
    vector<float> *ElecInfo_GsfTrackDz_sel = new vector<float>();
    vector<float> *ElecInfo_GsfTrackDxy_sel = new vector<float>();
    vector<bool> *ElecInfo_EGMCutBasedIDVeto_sel = new vector<bool>();
    vector<bool> *ElecInfo_EGMCutBasedIDLoose_sel = new vector<bool>();
    vector<bool> *ElecInfo_EGMCutBasedIDMedium_sel = new vector<bool>();
    vector<bool> *ElecInfo_EGMCutBasedIDTight_sel = new vector<bool>();
    vector<bool> *ElecInfo_passConvVeto_sel = new vector<bool>();
    vector<bool> *ElecInfo_fggPhoVeto_sel = new vector<bool>();
    vector<float> *ElecInfo_EnergyCorrFactor_sel = new vector<float>();
    vector<float> *ElecInfo_EnergyPostCorrErr_sel = new vector<float>();
    vector<bool> *ElecInfo_GenMatch_sel = new vector<bool>();
    vector<int> *ElecInfo_GenPdgID_sel = new vector<int>();
    vector<float> *ElecInfo_GenPt_sel = new vector<float>();
    vector<float> *ElecInfo_GenEta_sel = new vector<float>();
    vector<float> *ElecInfo_GenPhi_sel = new vector<float>();
    int MuonInfo_Size_sel = 0;
    vector<int> *MuonInfo_Charge_sel = new vector<int>();
    vector<float> *MuonInfo_MuonType_sel = new vector<float>();
    vector<float> *MuonInfo_Pt_sel = new vector<float>();
    vector<float> *MuonInfo_Eta_sel = new vector<float>();
    vector<float> *MuonInfo_Phi_sel = new vector<float>();
    vector<float> *MuonInfo_Energy_sel = new vector<float>();
    vector<float> *MuonInfo_BestTrackDz_sel = new vector<float>();
    vector<float> *MuonInfo_BestTrackDxy_sel = new vector<float>();
    vector<float> *MuonInfo_PFIsoDeltaBetaCorrR04_sel = new vector<float>();
    vector<float> *MuonInfo_TrackerBasedIsoR03_sel = new vector<float>();
    vector<bool> *MuonInfo_CutBasedIdMedium_sel = new vector<bool>();
    vector<bool> *MuonInfo_CutBasedIdTight_sel = new vector<bool>();
    vector<bool> *MuonInfo_CutBasedIdTight_bestVtx_sel = new vector<bool>();
    vector<bool> *MuonInfo_GenMatch_sel = new vector<bool>();
    vector<int> *MuonInfo_GenPdgID_sel = new vector<int>();
    vector<float> *MuonInfo_GenPt_sel = new vector<float>();
    vector<float> *MuonInfo_GenEta_sel = new vector<float>();
    vector<float> *MuonInfo_GenPhi_sel = new vector<float>();
    int jets_size_sel = 0;
    vector<float> *JetInfo_Pt_sel = new vector<float>();
    vector<float> *JetInfo_Eta_sel = new vector<float>();
    vector<float> *JetInfo_Phi_sel = new vector<float>();
    vector<float> *JetInfo_Mass_sel = new vector<float>();
    vector<float> *JetInfo_Energy_sel = new vector<float>();
    vector<float> *JetInfo_PtRaw_sel = new vector<float>();
    vector<float> *JetInfo_QGL_sel = new vector<float>();
    vector<float> *JetInfo_RMS_sel = new vector<float>();
    vector<float> *JetInfo_puJetIdMVA_sel = new vector<float>();
    vector<bool> *JetInfo_passesPuJetIdLoose_sel = new vector<bool>();
    vector<bool> *JetInfo_passesPuJetIdMedium_sel = new vector<bool>();
    vector<bool> *JetInfo_passesPuJetIdTight_sel = new vector<bool>();
    vector<bool> *JetInfo_GenJetMatch_sel = new vector<bool>();
    vector<float> *JetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags_sel = new vector<float>();
    vector<float> *JetInfo_pfCombinedMVAV2BJetTags_sel = new vector<float>();
    vector<float> *JetInfo_pfDeepCSVJetTags_probb_sel = new vector<float>();
    vector<float> *JetInfo_pfDeepCSVJetTags_probbb_sel = new vector<float>();
    vector<float> *JetInfo_pfDeepCSVJetTags_probc_sel = new vector<float>();
    vector<float> *JetInfo_pfDeepCSVJetTags_probudsg_sel = new vector<float>();
    vector<float> *JetInfo_pfDeepFlavourJetTags_probb_sel = new vector<float>();
    vector<float> *JetInfo_pfDeepFlavourJetTags_probbb_sel = new vector<float>();
    vector<float> *JetInfo_pfDeepFlavourJetTags_probc_sel = new vector<float>();
    vector<float> *JetInfo_pfDeepFlavourJetTags_probuds_sel = new vector<float>();
    vector<float> *JetInfo_pfDeepFlavourJetTags_probg_sel = new vector<float>();
    vector<float> *JetInfo_pfDeepFlavourJetTags_problepb_sel = new vector<float>();
    vector<float> *JetInfo_JECScale_sel = new vector<float>();
    vector<float> *JetInfo_JERScale_sel = new vector<float>();
    vector<bool> *JetInfo_GenPartonMatch_sel = new vector<bool>();
    vector<float> *JetInfo_GenPt_sel = new vector<float>();
    vector<float> *JetInfo_GenEta_sel = new vector<float>();
    vector<float> *JetInfo_GenPhi_sel = new vector<float>();
    vector<int> *JetInfo_GenPdgID_sel = new vector<int>();
    vector<int> *JetInfo_GenFlavor_sel = new vector<int>();
    vector<int> *JetInfo_GenHadronFlavor_sel = new vector<int>();

    // set output tree branches
    // set branches that will be changed to xxx_sel variables
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
    outTree->Branch("ElecInfo.Size", &ElecInfo_Size_sel);
    outTree->Branch("ElecInfo.Charge", &ElecInfo_Charge_sel);
    outTree->Branch("ElecInfo.Pt", &ElecInfo_Pt_sel);
    outTree->Branch("ElecInfo.Eta", &ElecInfo_Eta_sel);
    outTree->Branch("ElecInfo.Phi", &ElecInfo_Phi_sel);
    outTree->Branch("ElecInfo.Energy", &ElecInfo_Energy_sel);
    outTree->Branch("ElecInfo.EtaSC", &ElecInfo_EtaSC_sel);
    outTree->Branch("ElecInfo.PhiSC", &ElecInfo_PhiSC_sel);
    outTree->Branch("ElecInfo.GsfTrackDz", &ElecInfo_GsfTrackDz_sel);
    outTree->Branch("ElecInfo.GsfTrackDxy", &ElecInfo_GsfTrackDxy_sel);
    outTree->Branch("ElecInfo.EGMCutBasedIDVeto", &ElecInfo_EGMCutBasedIDVeto_sel);
    outTree->Branch("ElecInfo.EGMCutBasedIDLoose", &ElecInfo_EGMCutBasedIDLoose_sel);
    outTree->Branch("ElecInfo.EGMCutBasedIDMedium", &ElecInfo_EGMCutBasedIDMedium_sel);
    outTree->Branch("ElecInfo.EGMCutBasedIDTight", &ElecInfo_EGMCutBasedIDTight_sel);
    outTree->Branch("ElecInfo.passConvVeto", &ElecInfo_passConvVeto_sel);
    outTree->Branch("ElecInfo.fggPhoVeto", &ElecInfo_fggPhoVeto_sel);
    outTree->Branch("ElecInfo.EnergyCorrFactor", &ElecInfo_EnergyCorrFactor_sel);
    outTree->Branch("ElecInfo.EnergyPostCorrErr", &ElecInfo_EnergyPostCorrErr_sel);
    outTree->Branch("ElecInfo.GenMatch", &ElecInfo_GenMatch_sel);
    outTree->Branch("ElecInfo.GenPdgID", &ElecInfo_GenPdgID_sel);
    outTree->Branch("ElecInfo.GenPt", &ElecInfo_GenPt_sel);
    outTree->Branch("ElecInfo.GenEta", &ElecInfo_GenEta_sel);
    outTree->Branch("ElecInfo.GenPhi", &ElecInfo_GenPhi_sel);
    outTree->Branch("MuonInfo.Size", &MuonInfo_Size_sel);
    outTree->Branch("MuonInfo.Charge", &MuonInfo_Charge_sel);
    outTree->Branch("MuonInfo.MuonType", &MuonInfo_MuonType_sel);
    outTree->Branch("MuonInfo.Pt", &MuonInfo_Pt_sel);
    outTree->Branch("MuonInfo.Eta", &MuonInfo_Eta_sel);
    outTree->Branch("MuonInfo.Phi", &MuonInfo_Phi_sel);
    outTree->Branch("MuonInfo.Energy", &MuonInfo_Energy_sel);
    outTree->Branch("MuonInfo.BestTrackDz", &MuonInfo_BestTrackDz_sel);
    outTree->Branch("MuonInfo.BestTrackDxy", &MuonInfo_BestTrackDxy_sel);
    outTree->Branch("MuonInfo.PFIsoDeltaBetaCorrR04", &MuonInfo_PFIsoDeltaBetaCorrR04_sel);
    outTree->Branch("MuonInfo.TrackerBasedIsoR03", &MuonInfo_TrackerBasedIsoR03_sel);
    outTree->Branch("MuonInfo.CutBasedIdMedium", &MuonInfo_CutBasedIdMedium_sel);
    outTree->Branch("MuonInfo.CutBasedIdTight", &MuonInfo_CutBasedIdTight_sel);
    outTree->Branch("MuonInfo.CutBasedIdTight_bestVtx", &MuonInfo_CutBasedIdTight_bestVtx_sel);
    outTree->Branch("MuonInfo.GenMatch", &MuonInfo_GenMatch_sel);
    outTree->Branch("MuonInfo.GenPdgID", &MuonInfo_GenPdgID_sel);
    outTree->Branch("MuonInfo.GenPt", &MuonInfo_GenPt_sel);
    outTree->Branch("MuonInfo.GenEta", &MuonInfo_GenEta_sel);
    outTree->Branch("MuonInfo.GenPhi", &MuonInfo_GenPhi_sel);
    outTree->Branch("jets_size", &jets_size_sel);
    outTree->Branch("JetInfo.Pt", &JetInfo_Pt_sel);
    outTree->Branch("JetInfo.Eta", &JetInfo_Eta_sel);
    outTree->Branch("JetInfo.Phi", &JetInfo_Phi_sel);
    outTree->Branch("JetInfo.Mass", &JetInfo_Mass_sel);
    outTree->Branch("JetInfo.Energy", &JetInfo_Energy_sel);
    outTree->Branch("JetInfo.PtRaw", &JetInfo_PtRaw_sel);
    outTree->Branch("JetInfo.QGL", &JetInfo_QGL_sel);
    outTree->Branch("JetInfo.RMS", &JetInfo_RMS_sel);
    outTree->Branch("JetInfo.puJetIdMVA", &JetInfo_puJetIdMVA_sel);
    outTree->Branch("JetInfo.passesPuJetIdLoose", &JetInfo_passesPuJetIdLoose_sel);
    outTree->Branch("JetInfo.passesPuJetIdMedium", &JetInfo_passesPuJetIdMedium_sel);
    outTree->Branch("JetInfo.passesPuJetIdTight", &JetInfo_passesPuJetIdTight_sel);
    outTree->Branch("JetInfo.GenJetMatch", &JetInfo_GenJetMatch_sel);
    outTree->Branch("JetInfo.pfCombinedInclusiveSecondaryVertexV2BJetTags", &JetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags_sel);
    outTree->Branch("JetInfo.pfCombinedMVAV2BJetTags", &JetInfo_pfCombinedMVAV2BJetTags_sel);
    outTree->Branch("JetInfo.pfDeepCSVJetTags_probb", &JetInfo_pfDeepCSVJetTags_probb_sel);
    outTree->Branch("JetInfo.pfDeepCSVJetTags_probbb", &JetInfo_pfDeepCSVJetTags_probbb_sel);
    outTree->Branch("JetInfo.pfDeepCSVJetTags_probc", &JetInfo_pfDeepCSVJetTags_probc_sel);
    outTree->Branch("JetInfo.pfDeepCSVJetTags_probudsg", &JetInfo_pfDeepCSVJetTags_probudsg_sel);
    outTree->Branch("JetInfo.pfDeepFlavourJetTags_probb", &JetInfo_pfDeepFlavourJetTags_probb_sel);
    outTree->Branch("JetInfo.pfDeepFlavourJetTags_probbb", &JetInfo_pfDeepFlavourJetTags_probbb_sel);
    outTree->Branch("JetInfo.pfDeepFlavourJetTags_probc", &JetInfo_pfDeepFlavourJetTags_probc_sel);
    outTree->Branch("JetInfo.pfDeepFlavourJetTags_probuds", &JetInfo_pfDeepFlavourJetTags_probuds_sel);
    outTree->Branch("JetInfo.pfDeepFlavourJetTags_probg", &JetInfo_pfDeepFlavourJetTags_probg_sel);
    outTree->Branch("JetInfo.pfDeepFlavourJetTags_problepb", &JetInfo_pfDeepFlavourJetTags_problepb_sel);
    outTree->Branch("JetInfo.JECScale", &JetInfo_JECScale_sel);
    outTree->Branch("JetInfo.JERScale", &JetInfo_JERScale_sel);
    outTree->Branch("JetInfo.GenPartonMatch", &JetInfo_GenPartonMatch_sel);
    outTree->Branch("JetInfo.GenPt", &JetInfo_GenPt_sel);
    outTree->Branch("JetInfo.GenEta", &JetInfo_GenEta_sel);
    outTree->Branch("JetInfo.GenPhi", &JetInfo_GenPhi_sel);
    outTree->Branch("JetInfo.GenPdgID", &JetInfo_GenPdgID_sel);
    outTree->Branch("JetInfo.GenFlavor", &JetInfo_GenFlavor_sel);
    outTree->Branch("JetInfo.GenHadronFlavor", &JetInfo_GenHadronFlavor_sel);
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

    cout << "Output tree is successfully set\n";

    // loop through input tree events
    for (long evt=0; evt<inTree->GetEntries(); ++evt) {
	if (evt % 100000 == 0) cout << "---Processing event " << evt << "...\n";
	inTree->GetEntry(evt);

	// select whole events with the following cuts
	if (!EvtInfo_passTrigger) continue;

	if (DiPhoInfo_mass < 100 || DiPhoInfo_mass > 180) continue;
	if (DiPhoInfo_mass > 120 && DiPhoInfo_mass < 130) continue;

	if (DiPhoInfo_leadPt < 35 || DiPhoInfo_subleadPt < 25) continue;
	if (fabs(DiPhoInfo_leadEta) > 2.5) continue;
	if (fabs(DiPhoInfo_subleadEta) > 2.5) continue;
	if (fabs(DiPhoInfo_leadEta) > 1.4442 && fabs(DiPhoInfo_leadEta) < 1.566) continue;
	if (fabs(DiPhoInfo_subleadEta) > 1.4442 && fabs(DiPhoInfo_subleadEta) < 1.566) continue;

	// if the event passes the above cuts, select leptons and jets in the event and fill the output tree

	// reset the output variables
	ElecInfo_Size_sel = 0;
	ElecInfo_Charge_sel->clear();
	ElecInfo_Pt_sel->clear();
	ElecInfo_Eta_sel->clear();
	ElecInfo_Phi_sel->clear();
	ElecInfo_Energy_sel->clear();
	ElecInfo_EtaSC_sel->clear();
	ElecInfo_PhiSC_sel->clear();
	ElecInfo_GsfTrackDz_sel->clear();
	ElecInfo_GsfTrackDxy_sel->clear();
	ElecInfo_EGMCutBasedIDVeto_sel->clear();
	ElecInfo_EGMCutBasedIDLoose_sel->clear();
	ElecInfo_EGMCutBasedIDMedium_sel->clear();
	ElecInfo_EGMCutBasedIDTight_sel->clear();
	ElecInfo_passConvVeto_sel->clear();
	ElecInfo_fggPhoVeto_sel->clear();
	ElecInfo_EnergyCorrFactor_sel->clear();
	ElecInfo_EnergyPostCorrErr_sel->clear();
	ElecInfo_GenMatch_sel->clear();
	ElecInfo_GenPdgID_sel->clear();
	ElecInfo_GenPt_sel->clear();
	ElecInfo_GenEta_sel->clear();
	ElecInfo_GenPhi_sel->clear();
	MuonInfo_Size_sel = 0;
	MuonInfo_Charge_sel->clear();
	MuonInfo_MuonType_sel->clear();
	MuonInfo_Pt_sel->clear();
	MuonInfo_Eta_sel->clear();
	MuonInfo_Phi_sel->clear();
	MuonInfo_Energy_sel->clear();
	MuonInfo_BestTrackDz_sel->clear();
	MuonInfo_BestTrackDxy_sel->clear();
	MuonInfo_PFIsoDeltaBetaCorrR04_sel->clear();
	MuonInfo_TrackerBasedIsoR03_sel->clear();
	MuonInfo_CutBasedIdMedium_sel->clear();
	MuonInfo_CutBasedIdTight_sel->clear();
	MuonInfo_CutBasedIdTight_bestVtx_sel->clear();
	MuonInfo_GenMatch_sel->clear();
	MuonInfo_GenPdgID_sel->clear();
	MuonInfo_GenPt_sel->clear();
	MuonInfo_GenEta_sel->clear();
	MuonInfo_GenPhi_sel->clear();
	jets_size_sel = 0;
	JetInfo_Pt_sel->clear();
	JetInfo_Eta_sel->clear();
	JetInfo_Phi_sel->clear();
	JetInfo_Mass_sel->clear();
	JetInfo_Energy_sel->clear();
	JetInfo_PtRaw_sel->clear();
	JetInfo_QGL_sel->clear();
	JetInfo_RMS_sel->clear();
	JetInfo_puJetIdMVA_sel->clear();
	JetInfo_passesPuJetIdLoose_sel->clear();
	JetInfo_passesPuJetIdMedium_sel->clear();
	JetInfo_passesPuJetIdTight_sel->clear();
	JetInfo_GenJetMatch_sel->clear();
	JetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags_sel->clear();
	JetInfo_pfCombinedMVAV2BJetTags_sel->clear();
	JetInfo_pfDeepCSVJetTags_probb_sel->clear();
	JetInfo_pfDeepCSVJetTags_probbb_sel->clear();
	JetInfo_pfDeepCSVJetTags_probc_sel->clear();
	JetInfo_pfDeepCSVJetTags_probudsg_sel->clear();
	JetInfo_pfDeepFlavourJetTags_probb_sel->clear();
	JetInfo_pfDeepFlavourJetTags_probbb_sel->clear();
	JetInfo_pfDeepFlavourJetTags_probc_sel->clear();
	JetInfo_pfDeepFlavourJetTags_probuds_sel->clear();
	JetInfo_pfDeepFlavourJetTags_probg_sel->clear();
	JetInfo_pfDeepFlavourJetTags_problepb_sel->clear();
	JetInfo_JECScale_sel->clear();
	JetInfo_JERScale_sel->clear();
	JetInfo_GenPartonMatch_sel->clear();
	JetInfo_GenPt_sel->clear();
	JetInfo_GenEta_sel->clear();
	JetInfo_GenPhi_sel->clear();
	JetInfo_GenPdgID_sel->clear();
	JetInfo_GenFlavor_sel->clear();
	JetInfo_GenHadronFlavor_sel->clear();

	// create 4-momentum of leading photon and subleading photon
	TLorentzVector leadPho, subleadPho;
	leadPho.SetPtEtaPhiE(DiPhoInfo_leadPt, DiPhoInfo_leadEta, DiPhoInfo_leadPhi, DiPhoInfo_leadE);
	subleadPho.SetPtEtaPhiE(DiPhoInfo_subleadPt, DiPhoInfo_subleadEta, DiPhoInfo_subleadPhi, DiPhoInfo_subleadE);

	// create vector of 4-momentum for selected leptons
	vector<TLorentzVector*> electrons;
	vector<TLorentzVector*> muons;

	// electron preselection
	for (int i=0; i<ElecInfo_Size; ++i) {
	    if (!ElecInfo_EGMCutBasedIDMedium->at(i)) continue;
	    if (ElecInfo_Pt->at(i) < 20) continue;
	    if (fabs(ElecInfo_Eta->at(i)) > 2.4) continue;
	    if (fabs(ElecInfo_Eta->at(i)) > 1.4442 && fabs(ElecInfo_Eta->at(i)) < 1.566) continue;

	    TLorentzVector *electron_tmp = new TLorentzVector();
	    electron_tmp->SetPtEtaPhiE(ElecInfo_Pt->at(i), ElecInfo_Eta->at(i), ElecInfo_Phi->at(i), ElecInfo_Energy->at(i));
	    if (electron_tmp->DeltaR(leadPho) < 0.3 || electron_tmp->DeltaR(subleadPho) < 0.3) { delete electron_tmp; continue; }

	    // save the electrons passing preselection
	    ElecInfo_Size_sel += 1;
	    ElecInfo_Charge_sel->push_back(ElecInfo_Charge->at(i));
	    ElecInfo_Pt_sel->push_back(ElecInfo_Pt->at(i));
	    ElecInfo_Eta_sel->push_back(ElecInfo_Eta->at(i));
	    ElecInfo_Phi_sel->push_back(ElecInfo_Phi->at(i));
	    ElecInfo_Energy_sel->push_back(ElecInfo_Energy->at(i));
	    ElecInfo_EtaSC_sel->push_back(ElecInfo_EtaSC->at(i));
	    ElecInfo_PhiSC_sel->push_back(ElecInfo_PhiSC->at(i));
	    ElecInfo_GsfTrackDz_sel->push_back(ElecInfo_GsfTrackDz->at(i));
	    ElecInfo_GsfTrackDxy_sel->push_back(ElecInfo_GsfTrackDxy->at(i));
	    ElecInfo_EGMCutBasedIDVeto_sel->push_back(ElecInfo_EGMCutBasedIDVeto->at(i));
	    ElecInfo_EGMCutBasedIDLoose_sel->push_back(ElecInfo_EGMCutBasedIDLoose->at(i));
	    ElecInfo_EGMCutBasedIDMedium_sel->push_back(ElecInfo_EGMCutBasedIDMedium->at(i));
	    ElecInfo_EGMCutBasedIDTight_sel->push_back(ElecInfo_EGMCutBasedIDTight->at(i));
	    ElecInfo_passConvVeto_sel->push_back(ElecInfo_passConvVeto->at(i));
	    ElecInfo_fggPhoVeto_sel->push_back(ElecInfo_fggPhoVeto->at(i));
	    ElecInfo_EnergyCorrFactor_sel->push_back(ElecInfo_EnergyCorrFactor->at(i));
	    ElecInfo_EnergyPostCorrErr_sel->push_back(ElecInfo_EnergyPostCorrErr->at(i));
	    ElecInfo_GenMatch_sel->push_back(ElecInfo_GenMatch->at(i));
	    ElecInfo_GenPdgID_sel->push_back(ElecInfo_GenPdgID->at(i));
	    ElecInfo_GenPt_sel->push_back(ElecInfo_GenPt->at(i));
	    ElecInfo_GenEta_sel->push_back(ElecInfo_GenEta->at(i));
	    ElecInfo_GenPhi_sel->push_back(ElecInfo_GenPhi->at(i));

	    electrons.push_back(electron_tmp);
	} // end of electron loop

	// muon preselection
	for (int i=0; i<MuonInfo_Size; ++i) {
	    if (!MuonInfo_CutBasedIdTight->at(i)) continue;
	    if (MuonInfo_Pt->at(i) < 20) continue;
	    if (fabs(MuonInfo_Eta->at(i)) > 2.4) continue;
	    if (MuonInfo_PFIsoDeltaBetaCorrR04->at(i) > 0.25) continue;

	    TLorentzVector *muon_tmp = new TLorentzVector();
	    muon_tmp->SetPtEtaPhiE(MuonInfo_Pt->at(i), MuonInfo_Eta->at(i), MuonInfo_Phi->at(i), MuonInfo_Energy->at(i));
	    if (muon_tmp->DeltaR(leadPho) < 0.3 || muon_tmp->DeltaR(subleadPho) < 0.3) { delete muon_tmp; continue; }

	    // save the muons passing preselection
	    MuonInfo_Size_sel += 1;
	    MuonInfo_Charge_sel->push_back(MuonInfo_Charge->at(i));
	    MuonInfo_MuonType_sel->push_back(MuonInfo_MuonType->at(i));
	    MuonInfo_Pt_sel->push_back(MuonInfo_Pt->at(i));
	    MuonInfo_Eta_sel->push_back(MuonInfo_Eta->at(i));
	    MuonInfo_Phi_sel->push_back(MuonInfo_Phi->at(i));
	    MuonInfo_Energy_sel->push_back(MuonInfo_Energy->at(i));
	    MuonInfo_BestTrackDz_sel->push_back(MuonInfo_BestTrackDz->at(i));
	    MuonInfo_BestTrackDxy_sel->push_back(MuonInfo_BestTrackDxy->at(i));
	    MuonInfo_PFIsoDeltaBetaCorrR04_sel->push_back(MuonInfo_PFIsoDeltaBetaCorrR04->at(i));
	    MuonInfo_TrackerBasedIsoR03_sel->push_back(MuonInfo_TrackerBasedIsoR03->at(i));
	    MuonInfo_CutBasedIdMedium_sel->push_back(MuonInfo_CutBasedIdMedium->at(i));
	    MuonInfo_CutBasedIdTight_sel->push_back(MuonInfo_CutBasedIdTight->at(i));
	    MuonInfo_CutBasedIdTight_bestVtx_sel->push_back(MuonInfo_CutBasedIdTight_bestVtx->at(i));
	    MuonInfo_GenMatch_sel->push_back(MuonInfo_GenMatch->at(i));
	    MuonInfo_GenPdgID_sel->push_back(MuonInfo_GenPdgID->at(i));
	    MuonInfo_GenPt_sel->push_back(MuonInfo_GenPt->at(i));
	    MuonInfo_GenEta_sel->push_back(MuonInfo_GenEta->at(i));
	    MuonInfo_GenPhi_sel->push_back(MuonInfo_GenPhi->at(i));

	    muons.push_back(muon_tmp);
	} // end of muon loop

	// jet preselection
	for (int i=0; i<jets_size; ++i) {
	    if (JetInfo_Pt->at(i) < 25) continue;
	    if (fabs(JetInfo_Eta->at(i)) > 2.4) continue;

	    TLorentzVector jet_tmp;
	    jet_tmp.SetPtEtaPhiE(JetInfo_Pt->at(i), JetInfo_Eta->at(i), JetInfo_Phi->at(i), JetInfo_Energy->at(i));

	    if (jet_tmp.DeltaR(leadPho) < 0.4 || jet_tmp.DeltaR(subleadPho) < 0.4) continue;

	    bool jet_is_iso = true;
	    for (int j=0; j<electrons.size(); ++j)
		if (jet_tmp.DeltaR(*electrons.at(j)) < 0.4) {
		    jet_is_iso = false;
		    break;
		}
	    if (!jet_is_iso) continue;
	    for (int j=0; j<muons.size(); ++j)
		if (jet_tmp.DeltaR(*muons.at(j)) < 0.4) {
		    jet_is_iso = false;
		    break;
		}
	    if (!jet_is_iso) continue;

	    // save the jets passing preselection
	    jets_size_sel += 1;
	    JetInfo_Pt_sel->push_back(JetInfo_Pt->at(i));
	    JetInfo_Eta_sel->push_back(JetInfo_Eta->at(i));
	    JetInfo_Phi_sel->push_back(JetInfo_Phi->at(i));
	    JetInfo_Mass_sel->push_back(JetInfo_Mass->at(i));
	    JetInfo_Energy_sel->push_back(JetInfo_Energy->at(i));
	    JetInfo_PtRaw_sel->push_back(JetInfo_PtRaw->at(i));
	    JetInfo_QGL_sel->push_back(JetInfo_QGL->at(i));
	    JetInfo_RMS_sel->push_back(JetInfo_RMS->at(i));
	    JetInfo_puJetIdMVA_sel->push_back(JetInfo_puJetIdMVA->at(i));
	    JetInfo_passesPuJetIdLoose_sel->push_back(JetInfo_passesPuJetIdLoose->at(i));
	    JetInfo_passesPuJetIdMedium_sel->push_back(JetInfo_passesPuJetIdMedium->at(i));
	    JetInfo_passesPuJetIdTight_sel->push_back(JetInfo_passesPuJetIdTight->at(i));
	    JetInfo_GenJetMatch_sel->push_back(JetInfo_GenJetMatch->at(i));
	    JetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags_sel->push_back(JetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags->at(i));
	    JetInfo_pfCombinedMVAV2BJetTags_sel->push_back(JetInfo_pfCombinedMVAV2BJetTags->at(i));
	    JetInfo_pfDeepCSVJetTags_probb_sel->push_back(JetInfo_pfDeepCSVJetTags_probb->at(i));
	    JetInfo_pfDeepCSVJetTags_probbb_sel->push_back(JetInfo_pfDeepCSVJetTags_probbb->at(i));
	    JetInfo_pfDeepCSVJetTags_probc_sel->push_back(JetInfo_pfDeepCSVJetTags_probc->at(i));
	    JetInfo_pfDeepCSVJetTags_probudsg_sel->push_back(JetInfo_pfDeepCSVJetTags_probudsg->at(i));
	    JetInfo_pfDeepFlavourJetTags_probb_sel->push_back(JetInfo_pfDeepFlavourJetTags_probb->at(i));
	    JetInfo_pfDeepFlavourJetTags_probbb_sel->push_back(JetInfo_pfDeepFlavourJetTags_probbb->at(i));
	    JetInfo_pfDeepFlavourJetTags_probc_sel->push_back(JetInfo_pfDeepFlavourJetTags_probc->at(i));
	    JetInfo_pfDeepFlavourJetTags_probuds_sel->push_back(JetInfo_pfDeepFlavourJetTags_probuds->at(i));
	    JetInfo_pfDeepFlavourJetTags_probg_sel->push_back(JetInfo_pfDeepFlavourJetTags_probg->at(i));
	    JetInfo_pfDeepFlavourJetTags_problepb_sel->push_back(JetInfo_pfDeepFlavourJetTags_problepb->at(i));
	    JetInfo_JECScale_sel->push_back(JetInfo_JECScale->at(i));
	    JetInfo_JERScale_sel->push_back(JetInfo_JERScale->at(i));
	    JetInfo_GenPartonMatch_sel->push_back(JetInfo_GenPartonMatch->at(i));
	    JetInfo_GenPt_sel->push_back(JetInfo_GenPt->at(i));
	    JetInfo_GenEta_sel->push_back(JetInfo_GenEta->at(i));
	    JetInfo_GenPhi_sel->push_back(JetInfo_GenPhi->at(i));
	    JetInfo_GenPdgID_sel->push_back(JetInfo_GenPdgID->at(i));
	    JetInfo_GenFlavor_sel->push_back(JetInfo_GenFlavor->at(i));
	    JetInfo_GenHadronFlavor_sel->push_back(JetInfo_GenHadronFlavor->at(i));
	} // end of jet loop

	for (int i=0; i<electrons.size(); ++i) delete electrons[i];
	for (int i=0; i<muons.size(); ++i) delete muons[i];

	// preselection for this event is done
	// fill the event to output tree
	outTree->Fill();
    } // end of input tree event loop

    // save output tree
    fout->cd();
    outTree->Write();
    fout->Close();
    cout << "[INFO] Output tree is saved at: " << fout_name << endl;

    return 0;
}
