// Prepare training and test datasets for MVA reconstruction of ttbar signal, in hadronic channel, using 2017 signal MC.
//
// Do MC truth matching of photons, light jets and b-tagged jets, for each possible configuration.
// Regard matched configurations as signal (correct reconstruction), unmatched configurations as background (wrong reconstruction).
//
// input arguments:
// 1. path of input root file
// 2. name of input TTree
// 3. path of output directory

#include "TFile.h"
#include "TChain.h"
#include "TString.h"
#include "TTree.h"
#include "TLorentzVector.h"

#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

float deepCSV_wp = 0.8001;    // deepCSV tight working point

int main(int argc, char **argv)
{
    // get input arguments
    char *fin_name = argv[1];
    char *inTree_name = argv[2];
    TString outdir = argv[3];

    cout << "[INFO] Start processing n-tuple: " << fin_name << endl;

    // read input root files
    TChain *inTree = new TChain(inTree_name);
    inTree->Add(fin_name);

    // create output file
    int Nfout = 5;
    TFile *fout[Nfout];
    fout[0] = new TFile(outdir + "/signal_training.root", "recreate");
    fout[1] = new TFile(outdir + "/signal_test.root", "recreate");
    fout[2] = new TFile(outdir + "/bkg_training.root", "recreate");
    fout[3] = new TFile(outdir + "/bkg_test.root", "recreate");
    fout[4] = new TFile(outdir + "/mix_test.root", "recreate");
    TTree *outTree[Nfout];
    for (int i=0; i<Nfout; ++i) { fout[i]->cd();  outTree[i] = new TTree("flashggStdTree", ""); }

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

    // additional variables for output tree
    int idx_perm = 0;
    int N_perm = 0;
    bool truth_matched = 0;
    int idx_Wjet1 = 0;
    int idx_Wjet2 = 0;
    int idx_M1jet = 0;
    int idx_M2bjet = 0;
    float InvMass_W = 0;
    float InvMass_M1 = 0;
    float InvMass_M2 = 0;
    // additional variables for output tree (for MVA)
    float bJet_M2_Pt = 0;
    float bJet_M2_Eta = 0;
    float bJet_M2_Phi = 0;
    float bJet_M2_E = 0;
    float bJet_M2_btag = 0;
    float Jet_M1_Pt = 0;
    float Jet_M1_Eta = 0;
    float Jet_M1_Phi = 0;
    float Jet_M1_E = 0;
    float Jet_M1_btag = 0;
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

    // set output tree branches
    for (int i=0; i<Nfout; ++i) {
	outTree[i]->Branch("EvtInfo.NPu", &EvtInfo_NPu);
	outTree[i]->Branch("EvtInfo.NVtx", &EvtInfo_NVtx);
	outTree[i]->Branch("EvtInfo.passTrigger", &EvtInfo_passTrigger);
	outTree[i]->Branch("EvtInfo.genweight", &EvtInfo_genweight);
	outTree[i]->Branch("EvtInfo.Rho", &EvtInfo_Rho);
	outTree[i]->Branch("EvtInfo.PVz", &EvtInfo_PVz);
	outTree[i]->Branch("EvtInfo.BSsigmaz", &EvtInfo_BSsigmaz);
	outTree[i]->Branch("EvtInfo.Flag_HBHENoiseFilter", &EvtInfo_Flag_HBHENoiseFilter);
	outTree[i]->Branch("EvtInfo.Flag_HBHENoiseIsoFilter", &EvtInfo_Flag_HBHENoiseIsoFilter);
	outTree[i]->Branch("EvtInfo.Flag_EcalDeadCellTriggerPrimitiveFilter", &EvtInfo_Flag_EcalDeadCellTriggerPrimitiveFilter);
	outTree[i]->Branch("EvtInfo.Flag_goodVertices", &EvtInfo_Flag_goodVertices);
	outTree[i]->Branch("EvtInfo.Flag_globalSuperTightHalo2016Filter", &EvtInfo_Flag_globalSuperTightHalo2016Filter);
	outTree[i]->Branch("EvtInfo.Flag_BadPFMuonFilter", &EvtInfo_Flag_BadPFMuonFilter);
	outTree[i]->Branch("EvtInfo.Flag_eeBadScFilter", &EvtInfo_Flag_eeBadScFilter);
	outTree[i]->Branch("EvtInfo.ecalBadCalibReducedMINIAODFilter", &EvtInfo_ecalBadCalibReducedMINIAODFilter);
	outTree[i]->Branch("DiPhoInfo.mass", &DiPhoInfo_mass);
	outTree[i]->Branch("DiPhoInfo.pt", &DiPhoInfo_pt);
	outTree[i]->Branch("DiPhoInfo.leadPt", &DiPhoInfo_leadPt);
	outTree[i]->Branch("DiPhoInfo.leadEta", &DiPhoInfo_leadEta);
	outTree[i]->Branch("DiPhoInfo.leadPhi", &DiPhoInfo_leadPhi);
	outTree[i]->Branch("DiPhoInfo.leadE", &DiPhoInfo_leadE);
	outTree[i]->Branch("DiPhoInfo.leadEtaSC", &DiPhoInfo_leadEtaSC);
	outTree[i]->Branch("DiPhoInfo.leadPhiSC", &DiPhoInfo_leadPhiSC);
	outTree[i]->Branch("DiPhoInfo.leadsigEOverE", &DiPhoInfo_leadsigEOverE);
	outTree[i]->Branch("DiPhoInfo.leadR9", &DiPhoInfo_leadR9);
	outTree[i]->Branch("DiPhoInfo.leadsieie", &DiPhoInfo_leadsieie);
	outTree[i]->Branch("DiPhoInfo.leadhoe", &DiPhoInfo_leadhoe);
	outTree[i]->Branch("DiPhoInfo.leadIDMVA", &DiPhoInfo_leadIDMVA);
	outTree[i]->Branch("DiPhoInfo.leadIsEB", &DiPhoInfo_leadIsEB);
	outTree[i]->Branch("DiPhoInfo.leadIsEE", &DiPhoInfo_leadIsEE);
	outTree[i]->Branch("DiPhoInfo.leadhasPixelSeed", &DiPhoInfo_leadhasPixelSeed);
	outTree[i]->Branch("DiPhoInfo.leadGenMatch", &DiPhoInfo_leadGenMatch);
	outTree[i]->Branch("DiPhoInfo.leadGenMatchType", &DiPhoInfo_leadGenMatchType);
	outTree[i]->Branch("DiPhoInfo.subleadPt", &DiPhoInfo_subleadPt);
	outTree[i]->Branch("DiPhoInfo.subleadEta", &DiPhoInfo_subleadEta);
	outTree[i]->Branch("DiPhoInfo.subleadPhi", &DiPhoInfo_subleadPhi);
	outTree[i]->Branch("DiPhoInfo.subleadE", &DiPhoInfo_subleadE);
	outTree[i]->Branch("DiPhoInfo.subleadEtaSC", &DiPhoInfo_subleadEtaSC);
	outTree[i]->Branch("DiPhoInfo.subleadPhiSC", &DiPhoInfo_subleadPhiSC);
	outTree[i]->Branch("DiPhoInfo.subleadsigEOverE", &DiPhoInfo_subleadsigEOverE);
	outTree[i]->Branch("DiPhoInfo.subleadR9", &DiPhoInfo_subleadR9);
	outTree[i]->Branch("DiPhoInfo.subleadsieie", &DiPhoInfo_subleadsieie);
	outTree[i]->Branch("DiPhoInfo.subleadhoe", &DiPhoInfo_subleadhoe);
	outTree[i]->Branch("DiPhoInfo.subleadIDMVA", &DiPhoInfo_subleadIDMVA);
	outTree[i]->Branch("DiPhoInfo.subleadIsEB", &DiPhoInfo_subleadIsEB);
	outTree[i]->Branch("DiPhoInfo.subleadIsEE", &DiPhoInfo_subleadIsEE);
	outTree[i]->Branch("DiPhoInfo.subleadhasPixelSeed", &DiPhoInfo_subleadhasPixelSeed);
	outTree[i]->Branch("DiPhoInfo.subleadGenMatch", &DiPhoInfo_subleadGenMatch);
	outTree[i]->Branch("DiPhoInfo.subleadGenMatchType", &DiPhoInfo_subleadGenMatchType);
	outTree[i]->Branch("DiPhoInfo.diphotonMVA", &DiPhoInfo_diphotonMVA);
	outTree[i]->Branch("DiPhoInfo.SelectedVz", &DiPhoInfo_SelectedVz);
	outTree[i]->Branch("DiPhoInfo.GenVz", &DiPhoInfo_GenVz);
	outTree[i]->Branch("DiPhoInfo.centralWeight", &DiPhoInfo_centralWeight);
	outTree[i]->Branch("ElecInfo.Size", &ElecInfo_Size);
	outTree[i]->Branch("ElecInfo.Charge", &ElecInfo_Charge);
	outTree[i]->Branch("ElecInfo.Pt", &ElecInfo_Pt);
	outTree[i]->Branch("ElecInfo.Eta", &ElecInfo_Eta);
	outTree[i]->Branch("ElecInfo.Phi", &ElecInfo_Phi);
	outTree[i]->Branch("ElecInfo.Energy", &ElecInfo_Energy);
	outTree[i]->Branch("ElecInfo.EtaSC", &ElecInfo_EtaSC);
	outTree[i]->Branch("ElecInfo.PhiSC", &ElecInfo_PhiSC);
	outTree[i]->Branch("ElecInfo.GsfTrackDz", &ElecInfo_GsfTrackDz);
	outTree[i]->Branch("ElecInfo.GsfTrackDxy", &ElecInfo_GsfTrackDxy);
	outTree[i]->Branch("ElecInfo.EGMCutBasedIDVeto", &ElecInfo_EGMCutBasedIDVeto);
	outTree[i]->Branch("ElecInfo.EGMCutBasedIDLoose", &ElecInfo_EGMCutBasedIDLoose);
	outTree[i]->Branch("ElecInfo.EGMCutBasedIDMedium", &ElecInfo_EGMCutBasedIDMedium);
	outTree[i]->Branch("ElecInfo.EGMCutBasedIDTight", &ElecInfo_EGMCutBasedIDTight);
	outTree[i]->Branch("ElecInfo.passConvVeto", &ElecInfo_passConvVeto);
	outTree[i]->Branch("ElecInfo.fggPhoVeto", &ElecInfo_fggPhoVeto);
	outTree[i]->Branch("ElecInfo.EnergyCorrFactor", &ElecInfo_EnergyCorrFactor);
	outTree[i]->Branch("ElecInfo.EnergyPostCorrErr", &ElecInfo_EnergyPostCorrErr);
	outTree[i]->Branch("ElecInfo.GenMatch", &ElecInfo_GenMatch);
	outTree[i]->Branch("ElecInfo.GenPdgID", &ElecInfo_GenPdgID);
	outTree[i]->Branch("ElecInfo.GenPt", &ElecInfo_GenPt);
	outTree[i]->Branch("ElecInfo.GenEta", &ElecInfo_GenEta);
	outTree[i]->Branch("ElecInfo.GenPhi", &ElecInfo_GenPhi);
	outTree[i]->Branch("MuonInfo.Size", &MuonInfo_Size);
	outTree[i]->Branch("MuonInfo.Charge", &MuonInfo_Charge);
	outTree[i]->Branch("MuonInfo.MuonType", &MuonInfo_MuonType);
	outTree[i]->Branch("MuonInfo.Pt", &MuonInfo_Pt);
	outTree[i]->Branch("MuonInfo.Eta", &MuonInfo_Eta);
	outTree[i]->Branch("MuonInfo.Phi", &MuonInfo_Phi);
	outTree[i]->Branch("MuonInfo.Energy", &MuonInfo_Energy);
	outTree[i]->Branch("MuonInfo.BestTrackDz", &MuonInfo_BestTrackDz);
	outTree[i]->Branch("MuonInfo.BestTrackDxy", &MuonInfo_BestTrackDxy);
	outTree[i]->Branch("MuonInfo.PFIsoDeltaBetaCorrR04", &MuonInfo_PFIsoDeltaBetaCorrR04);
	outTree[i]->Branch("MuonInfo.TrackerBasedIsoR03", &MuonInfo_TrackerBasedIsoR03);
	outTree[i]->Branch("MuonInfo.CutBasedIdMedium", &MuonInfo_CutBasedIdMedium);
	outTree[i]->Branch("MuonInfo.CutBasedIdTight", &MuonInfo_CutBasedIdTight);
	outTree[i]->Branch("MuonInfo.CutBasedIdTight_bestVtx", &MuonInfo_CutBasedIdTight_bestVtx);
	outTree[i]->Branch("MuonInfo.GenMatch", &MuonInfo_GenMatch);
	outTree[i]->Branch("MuonInfo.GenPdgID", &MuonInfo_GenPdgID);
	outTree[i]->Branch("MuonInfo.GenPt", &MuonInfo_GenPt);
	outTree[i]->Branch("MuonInfo.GenEta", &MuonInfo_GenEta);
	outTree[i]->Branch("MuonInfo.GenPhi", &MuonInfo_GenPhi);
	outTree[i]->Branch("jets_size", &jets_size);
	outTree[i]->Branch("JetInfo.Pt", &JetInfo_Pt);
	outTree[i]->Branch("JetInfo.Eta", &JetInfo_Eta);
	outTree[i]->Branch("JetInfo.Phi", &JetInfo_Phi);
	outTree[i]->Branch("JetInfo.Mass", &JetInfo_Mass);
	outTree[i]->Branch("JetInfo.Energy", &JetInfo_Energy);
	outTree[i]->Branch("JetInfo.PtRaw", &JetInfo_PtRaw);
	outTree[i]->Branch("JetInfo.QGL", &JetInfo_QGL);
	outTree[i]->Branch("JetInfo.RMS", &JetInfo_RMS);
	outTree[i]->Branch("JetInfo.puJetIdMVA", &JetInfo_puJetIdMVA);
	outTree[i]->Branch("JetInfo.passesPuJetIdLoose", &JetInfo_passesPuJetIdLoose);
	outTree[i]->Branch("JetInfo.passesPuJetIdMedium", &JetInfo_passesPuJetIdMedium);
	outTree[i]->Branch("JetInfo.passesPuJetIdTight", &JetInfo_passesPuJetIdTight);
	outTree[i]->Branch("JetInfo.GenJetMatch", &JetInfo_GenJetMatch);
	outTree[i]->Branch("JetInfo.pfCombinedInclusiveSecondaryVertexV2BJetTags", &JetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags);
	outTree[i]->Branch("JetInfo.pfCombinedMVAV2BJetTags", &JetInfo_pfCombinedMVAV2BJetTags);
	outTree[i]->Branch("JetInfo.pfDeepCSVJetTags_probb", &JetInfo_pfDeepCSVJetTags_probb);
	outTree[i]->Branch("JetInfo.pfDeepCSVJetTags_probbb", &JetInfo_pfDeepCSVJetTags_probbb);
	outTree[i]->Branch("JetInfo.pfDeepCSVJetTags_probc", &JetInfo_pfDeepCSVJetTags_probc);
	outTree[i]->Branch("JetInfo.pfDeepCSVJetTags_probudsg", &JetInfo_pfDeepCSVJetTags_probudsg);
	outTree[i]->Branch("JetInfo.pfDeepFlavourJetTags_probb", &JetInfo_pfDeepFlavourJetTags_probb);
	outTree[i]->Branch("JetInfo.pfDeepFlavourJetTags_probbb", &JetInfo_pfDeepFlavourJetTags_probbb);
	outTree[i]->Branch("JetInfo.pfDeepFlavourJetTags_probc", &JetInfo_pfDeepFlavourJetTags_probc);
	outTree[i]->Branch("JetInfo.pfDeepFlavourJetTags_probuds", &JetInfo_pfDeepFlavourJetTags_probuds);
	outTree[i]->Branch("JetInfo.pfDeepFlavourJetTags_probg", &JetInfo_pfDeepFlavourJetTags_probg);
	outTree[i]->Branch("JetInfo.pfDeepFlavourJetTags_problepb", &JetInfo_pfDeepFlavourJetTags_problepb);
	outTree[i]->Branch("JetInfo.JECScale", &JetInfo_JECScale);
	outTree[i]->Branch("JetInfo.JERScale", &JetInfo_JERScale);
	outTree[i]->Branch("JetInfo.GenPartonMatch", &JetInfo_GenPartonMatch);
	outTree[i]->Branch("JetInfo.GenPt", &JetInfo_GenPt);
	outTree[i]->Branch("JetInfo.GenEta", &JetInfo_GenEta);
	outTree[i]->Branch("JetInfo.GenPhi", &JetInfo_GenPhi);
	outTree[i]->Branch("JetInfo.GenPdgID", &JetInfo_GenPdgID);
	outTree[i]->Branch("JetInfo.GenFlavor", &JetInfo_GenFlavor);
	outTree[i]->Branch("JetInfo.GenHadronFlavor", &JetInfo_GenHadronFlavor);
	outTree[i]->Branch("MetInfo.Pt", &MetInfo_Pt);
	outTree[i]->Branch("MetInfo.Phi", &MetInfo_Phi);
	outTree[i]->Branch("MetInfo.Px", &MetInfo_Px);
	outTree[i]->Branch("MetInfo.Py", &MetInfo_Py);
	outTree[i]->Branch("MetInfo.SumET", &MetInfo_SumET);
	outTree[i]->Branch("GenPartInfo.size", &GenPartInfo_size);
	outTree[i]->Branch("GenPartInfo.Pt", &GenPartInfo_Pt);
	outTree[i]->Branch("GenPartInfo.Eta", &GenPartInfo_Eta);
	outTree[i]->Branch("GenPartInfo.Phi", &GenPartInfo_Phi);
	outTree[i]->Branch("GenPartInfo.Mass", &GenPartInfo_Mass);
	outTree[i]->Branch("GenPartInfo.PdgID", &GenPartInfo_PdgID);
	outTree[i]->Branch("GenPartInfo.Status", &GenPartInfo_Status);
	outTree[i]->Branch("GenPartInfo.nMo", &GenPartInfo_nMo);
	outTree[i]->Branch("GenPartInfo.nDa", &GenPartInfo_nDa);
	outTree[i]->Branch("GenPartInfo.isHardProcess", &GenPartInfo_isHardProcess);
	outTree[i]->Branch("GenPartInfo.fromHardProcessFinalState", &GenPartInfo_fromHardProcessFinalState);
	outTree[i]->Branch("GenPartInfo.isPromptFinalState", &GenPartInfo_isPromptFinalState);
	outTree[i]->Branch("GenPartInfo.isDirectPromptTauDecayProductFinalState", &GenPartInfo_isDirectPromptTauDecayProductFinalState);
	outTree[i]->Branch("GenPartInfo.MomPdgID", &GenPartInfo_MomPdgID);
	outTree[i]->Branch("GenPartInfo.MomStatus", &GenPartInfo_MomStatus);
	outTree[i]->Branch("GenPartInfo.MomPt", &GenPartInfo_MomPt);
	outTree[i]->Branch("GenPartInfo.MomEta", &GenPartInfo_MomEta);
	outTree[i]->Branch("GenPartInfo.MomPhi", &GenPartInfo_MomPhi);
	outTree[i]->Branch("GenPartInfo.MomMass", &GenPartInfo_MomMass);
	outTree[i]->Branch("idx_perm", &idx_perm);
	outTree[i]->Branch("N_perm", &N_perm);
	outTree[i]->Branch("truth_matched", &truth_matched);
	outTree[i]->Branch("idx_Wjet1", &idx_Wjet1);
	outTree[i]->Branch("idx_Wjet2", &idx_Wjet2);
	outTree[i]->Branch("idx_M1jet", &idx_M1jet);
	outTree[i]->Branch("idx_M2bjet", &idx_M2bjet);
	outTree[i]->Branch("InvMass_W", &InvMass_W);
	outTree[i]->Branch("InvMass_M1", &InvMass_M1);
	outTree[i]->Branch("InvMass_M2", &InvMass_M2);
	outTree[i]->Branch("bJet_M2_Pt", &bJet_M2_Pt);
	outTree[i]->Branch("bJet_M2_Eta", &bJet_M2_Eta);
	outTree[i]->Branch("bJet_M2_Phi", &bJet_M2_Phi);
	outTree[i]->Branch("bJet_M2_E", &bJet_M2_E);
	outTree[i]->Branch("bJet_M2_btag", &bJet_M2_btag);
	outTree[i]->Branch("Jet_M1_Pt", &Jet_M1_Pt);
	outTree[i]->Branch("Jet_M1_Eta", &Jet_M1_Eta);
	outTree[i]->Branch("Jet_M1_Phi", &Jet_M1_Phi);
	outTree[i]->Branch("Jet_M1_E", &Jet_M1_E);
	outTree[i]->Branch("Jet_M1_btag", &Jet_M1_btag);
	outTree[i]->Branch("Jet1_W_Pt", &Jet1_W_Pt);
	outTree[i]->Branch("Jet1_W_Eta", &Jet1_W_Eta);
	outTree[i]->Branch("Jet1_W_Phi", &Jet1_W_Phi);
	outTree[i]->Branch("Jet1_W_E", &Jet1_W_E);
	outTree[i]->Branch("Jet1_W_btag", &Jet1_W_btag);
	outTree[i]->Branch("Jet2_W_Pt", &Jet2_W_Pt);
	outTree[i]->Branch("Jet2_W_Eta", &Jet2_W_Eta);
	outTree[i]->Branch("Jet2_W_Phi", &Jet2_W_Phi);
	outTree[i]->Branch("Jet2_W_E", &Jet2_W_E);
	outTree[i]->Branch("Jet2_W_btag", &Jet2_W_btag);
    }

    cout << "Output tree is successfully set\n";

    int Nevt_in = inTree->GetEntries();
    int Nevt_goodJetNum = 0;
    int Nevt_goodGenInfo = 0;
    int Nevt_training = 0;
    int Nevt_test = 0;
    int Nevt_outSigTraining = 0;
    int Nevt_outSigTest = 0;
    int Nevt_outBkgTraining = 0;
    int Nevt_outBkgTest = 0;

    // loop through input tree events
    for (long evt=0; evt<inTree->GetEntries(); ++evt) {
	if (evt % 10000 == 0) cout << "---Processing event " << evt << "...\n";
	inTree->GetEntry(evt);

	// categorize reco light jets and b-jets
	vector<int> idx_light_jets;
	vector<int> idx_bjets;
	for (int i=0; i<jets_size; ++i) {
	    if (JetInfo_pfDeepCSVJetTags_probb->at(i) + JetInfo_pfDeepCSVJetTags_probbb->at(i) > deepCSV_wp)
		idx_bjets.push_back(i);
	    else
		idx_light_jets.push_back(i);
	}
	// if number of reco light jets < 3 or number of reco b jet < 1, it's impossible to reconstruct the event
	if (idx_light_jets.size() < 3 || idx_bjets.size() < 1) continue;

	Nevt_goodJetNum += 1;

	// construct 4-momentum of gen particles
	vector<TLorentzVector> gen_gammaFromH;
	vector<TLorentzVector> gen_qFromW;
	vector<TLorentzVector> gen_qFromM1;
	vector<TLorentzVector> gen_bFromM2;
	for (int i=0; i<GenPartInfo_size; ++i) {
	    TLorentzVector tmp;
	    // test for gamma from Higgs
	    if (GenPartInfo_PdgID->at(i) == 22 && GenPartInfo_MomPdgID->at(i) == 25) {
		tmp.SetPtEtaPhiM(GenPartInfo_Pt->at(i), GenPartInfo_Eta->at(i), GenPartInfo_Phi->at(i), GenPartInfo_Mass->at(i));
		gen_gammaFromH.push_back(tmp);
	    }
	    // test for light quarks from W
	    if (fabs(GenPartInfo_PdgID->at(i)) >= 1 && fabs(GenPartInfo_PdgID->at(i)) <= 4 && fabs(GenPartInfo_MomPdgID->at(i)) == 24) {
		tmp.SetPtEtaPhiM(GenPartInfo_Pt->at(i), GenPartInfo_Eta->at(i), GenPartInfo_Phi->at(i), GenPartInfo_Mass->at(i));
		gen_qFromW.push_back(tmp);
	    }
	    // test for light quark from FCNC top
	    if ((fabs(GenPartInfo_PdgID->at(i)) == 2 || fabs(GenPartInfo_PdgID->at(i)) == 4) && fabs(GenPartInfo_MomPdgID->at(i)) == 6) {
		tmp.SetPtEtaPhiM(GenPartInfo_Pt->at(i), GenPartInfo_Eta->at(i), GenPartInfo_Phi->at(i), GenPartInfo_Mass->at(i));
		gen_qFromM1.push_back(tmp);
	    }
	    // test for b quark from SM top
	    if (fabs(GenPartInfo_PdgID->at(i)) == 5 && fabs(GenPartInfo_MomPdgID->at(i)) == 6) {
		tmp.SetPtEtaPhiM(GenPartInfo_Pt->at(i), GenPartInfo_Eta->at(i), GenPartInfo_Phi->at(i), GenPartInfo_Mass->at(i));
		gen_bFromM2.push_back(tmp);
	    }
	}
	// check number of particles
	// For the following cases, I don't know how to do gen matching so these events are not used in training & testing MVA. These events are very rare.
	if (gen_gammaFromH.size() != 2 || gen_qFromW.size() != 2 || gen_qFromM1.size() != 1 || gen_bFromM2.size() != 1) continue;

	Nevt_goodGenInfo += 1;

	if (evt < Nevt_in/2) Nevt_training += 1;
	else Nevt_test += 1;

	/* start MC truth matching */
	// match the 2 photons
	TLorentzVector reco_leadPho, reco_subleadPho; // 4-momentum of reco photon
	reco_leadPho.SetPtEtaPhiE(DiPhoInfo_leadPt, DiPhoInfo_leadEta, DiPhoInfo_leadPhi, DiPhoInfo_leadE);
	reco_subleadPho.SetPtEtaPhiE(DiPhoInfo_subleadPt, DiPhoInfo_subleadEta, DiPhoInfo_subleadPhi, DiPhoInfo_subleadE);
	// if the photons are not matched, the whole event is unmatched regardless of jet permutations
	bool photon_matched = 1;
	if ( (gen_gammaFromH[0].DeltaR(reco_leadPho) > 0.4 || gen_gammaFromH[1].DeltaR(reco_subleadPho) > 0.4) &&
		(gen_gammaFromH[0].DeltaR(reco_subleadPho) > 0.4 || gen_gammaFromH[1].DeltaR(reco_leadPho) > 0.4) ) photon_matched = 0;

	// build 4-momenta of all reco jets
	vector<TLorentzVector> reco_jets;
	for (int i=0; i<jets_size; ++i) {
	    reco_jets.push_back( TLorentzVector() );
	    reco_jets[i].SetPtEtaPhiE(JetInfo_Pt->at(i), JetInfo_Eta->at(i), JetInfo_Phi->at(i), JetInfo_Energy->at(i));
	}

	// jet matching
	// calculate number of jet permutations
	N_perm = idx_bjets.size() * idx_light_jets.size() * (idx_light_jets.size()-1) * (idx_light_jets.size()-2) / 2;
	idx_perm = 1;
	// process all jet permutations
	for (int n=0; n<idx_bjets.size(); ++n) { // loop through reco b-jets (matching b-quark from M2)
	    for (int i=0; i<idx_light_jets.size(); ++i) { // loop through reco light jets (matching quark from M1)
		for (int j=0; j<idx_light_jets.size()-1; ++j) { // loop through reco light jets (matching quark1 from W)
		    if (j == i) continue;
		    for (int k=j+1; k<idx_light_jets.size(); ++k) { // loop through reco light jets (matching quark2 from W)
			if (k == i) continue;

			// get the 4-momenta of jets
			TLorentzVector &reco_M2bjet = reco_jets[ idx_bjets[n] ];
			TLorentzVector &reco_M1jet = reco_jets[ idx_light_jets[i] ];
			TLorentzVector &reco_Wjet1 = reco_jets[ idx_light_jets[j] ];
			TLorentzVector &reco_Wjet2 = reco_jets[ idx_light_jets[k] ];

			// reconstruction of W, M1, M2
			TLorentzVector reco_W = reco_Wjet1 + reco_Wjet2;
			TLorentzVector reco_M1 = reco_M1jet + reco_leadPho + reco_subleadPho;
			TLorentzVector reco_M2 = reco_W + reco_M2bjet;

			// match b-jet
			bool bquark_matched = reco_M2bjet.DeltaR( gen_bFromM2[0] ) <= 0.4;

			// match light jets
			bool light_quark_matched = reco_M1jet.DeltaR( gen_qFromM1[0] ) <= 0.4 &&
				((reco_Wjet1.DeltaR( gen_qFromW[0] ) <= 0.4 && reco_Wjet2.DeltaR( gen_qFromW[1] ) <= 0.4) ||
				 (reco_Wjet1.DeltaR( gen_qFromW[1] ) <= 0.4 && reco_Wjet2.DeltaR( gen_qFromW[0] ) <= 0.4));

			// fill the branches
			idx_M2bjet = n;
			idx_M1jet = i;
			idx_Wjet1 = j;
			idx_Wjet2 = k;
			InvMass_W = reco_W.M();
			InvMass_M1 = reco_M1.M();
			InvMass_M2 = reco_M2.M();
			bJet_M2_Pt = JetInfo_Pt->at( idx_bjets[n] );
			bJet_M2_Eta = JetInfo_Eta->at( idx_bjets[n] );
			bJet_M2_Phi = JetInfo_Phi->at( idx_bjets[n] );
			bJet_M2_E = JetInfo_Energy->at( idx_bjets[n] );
			bJet_M2_btag = JetInfo_pfDeepCSVJetTags_probb->at( idx_bjets[n] ) + JetInfo_pfDeepCSVJetTags_probbb->at( idx_bjets[n] );
			Jet_M1_Pt = JetInfo_Pt->at( idx_light_jets[i] );
			Jet_M1_Eta = JetInfo_Eta->at( idx_light_jets[i] );
			Jet_M1_Phi = JetInfo_Phi->at( idx_light_jets[i] );
			Jet_M1_E = JetInfo_Energy->at( idx_light_jets[i] );
			Jet_M1_btag = JetInfo_pfDeepCSVJetTags_probb->at( idx_light_jets[i] ) + JetInfo_pfDeepCSVJetTags_probbb->at( idx_light_jets[i] );
			Jet1_W_Pt = JetInfo_Pt->at( idx_light_jets[j] );
			Jet1_W_Eta = JetInfo_Eta->at( idx_light_jets[j] );
			Jet1_W_Phi = JetInfo_Phi->at( idx_light_jets[j] );
			Jet1_W_E = JetInfo_Energy->at( idx_light_jets[j] );
			Jet1_W_btag = JetInfo_pfDeepCSVJetTags_probb->at( idx_light_jets[j] ) + JetInfo_pfDeepCSVJetTags_probbb->at( idx_light_jets[j] );
			Jet2_W_Pt = JetInfo_Pt->at( idx_light_jets[k] );
			Jet2_W_Eta = JetInfo_Eta->at( idx_light_jets[k] );
			Jet2_W_Phi = JetInfo_Phi->at( idx_light_jets[k] );
			Jet2_W_E = JetInfo_Energy->at( idx_light_jets[k] );
			Jet2_W_btag = JetInfo_pfDeepCSVJetTags_probb->at( idx_light_jets[k] ) + JetInfo_pfDeepCSVJetTags_probbb->at( idx_light_jets[k] );

			truth_matched = photon_matched && bquark_matched && light_quark_matched;

			// fill the event to output tree
			if (truth_matched) {
			    if (evt < Nevt_in / 2) { // fill signal training tree
				outTree[0]->Fill();
				Nevt_outSigTraining += 1;
			    } else { // fill signal test tree
				outTree[1]->Fill();
				outTree[4]->Fill();
				Nevt_outSigTest += 1;
			    }
			} else {
			    if (evt < Nevt_in / 2) { // fill bkg training tree
				outTree[2]->Fill();
				Nevt_outBkgTraining += 1;
			    } else { // fill bkg test tree
				outTree[3]->Fill();
				outTree[4]->Fill();
				Nevt_outBkgTest += 1;
			    }
			}

			// increment permutation index
			idx_perm += 1;
		    } // end of reco light jet loop (matching quark2 from W)
		} // end of reco light jet loop (matching quark1 from W)
	    } // end of reco light jet loop (matching quark from M1)
	} // end of reco b-jet loop (matching b-quark from M2)

    } // end of input tree event loop

    // save output tree
    for (int i=0; i<Nfout; ++i) { fout[i]->cd();  outTree[i]->Write();  fout[i]->Close(); }
    cout << "[INFO] Output trees are saved at: " << outdir << endl;

    cout << "\n[Summary]\n";
    cout << "---Number of input events: " << Nevt_in << endl;
    cout << "---Number of events with good jet number: " << Nevt_goodJetNum << endl;
    cout << "---Number of events with good gen particle info: " << Nevt_goodGenInfo << endl;
    cout << "---Number of events for training: " << Nevt_training << endl;
    cout << "---Number of events for testing: " << Nevt_test << endl;
    cout << "---Number of permutations in signal training output: " << Nevt_outSigTraining << endl;
    cout << "---Number of permutations in signal test output: " << Nevt_outSigTest << endl;
    cout << "---Number of permutations in background training output: " << Nevt_outBkgTraining << endl;
    cout << "---Number of permutations in background test output: " << Nevt_outBkgTest << endl;

    return 0;
}