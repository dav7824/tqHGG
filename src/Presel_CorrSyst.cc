/*
1. Apply corrections, produce systematic trees.
2. Apply the first part of preselection.

######################
## Preselection     ##
######################
Trigger:
    Diphoton triggers
Photon:
    (1) pt > 35(25) GeV
    (2) |eta| < 2.5, excluding [1.4442, 1.566] (EE-EB gap)
Diphoton:
    Mgg > 100 GeV
Electron:
    (1) Cut-based medium ID
    (2) pt > 10 GeV
    (3) |eta| < 2.4, excluding [1.4442, 1.566] (EE-EB gap)
    (4) delta_R(electron, photon) > 0.2
Muon:
    (1) Cut-based medium ID
    (2) pt > 5 GeV
    (3) |eta| < 2.4
    (4) Relative PF isolation (R=0.4) < 0.25, (loose WP)
    (5) delta_R(muon, photon) > 0.2
Jet:
    (1) pt > 25 GeV
    (2) |eta| < 2.4
    (3) delta_R(lepton/photon, jet) > 0.4
*/

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TLorentzVector.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
using namespace std;

// Corrections & their systematics
enum Syst {
    Central,
    JECUp, JECDown,
    JERUp, JERDown,
    METJetEnUp, METJetEnDown,
    METJetResUp, METJetResDown,
    METUncEnUp, METUncEnDown,
    METPhoEnUp, METPhoEnDown,
};

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fin_ = argv[1];
    TString fout_ = argv[2];
    TString inTree_ = argv[3];
    TString outTree_ = argv[4];
    TString systtype_ = argv[5];

    // Set systematic type
    Syst systtype;
    if (systtype_ == "Central")
        systtype = Central;
    else if (systtype_ == "JECUp")
        systtype = JECUp;
    else if (systtype_ == "JECDown")
        systtype = JECDown;
    else if (systtype_ == "JERUp")
        systtype = JERUp;
    else if (systtype_ == "JERDown")
        systtype = JERDown;
    else if (systtype_ == "METJetEnUp")
        systtype = METJetEnUp;
    else if (systtype_ == "METJetEnDown")
        systtype = METJetEnDown;
    else if (systtype_ == "METJetResUp")
        systtype = METJetResUp;
    else if (systtype_ == "METJetResDown")
        systtype = METJetResDown;
    else if (systtype_ == "METUncEnUp")
        systtype = METUncEnUp;
    else if (systtype_ == "METUncEnDown")
        systtype = METUncEnDown;
    else if (systtype_ == "METPhoEnUp")
        systtype = METPhoEnUp;
    else if (systtype_ == "METPhoEnDown")
        systtype = METPhoEnDown;
    else {
        cout << "[ERROR] Invalid systematic unc type!\n";
        exit(1);
    }

    // Read input tree
    cout << "[INFO] Start processing: " << fin_ << endl;
    TChain *inTree = new TChain(inTree_);
    inTree->Add(fin_);

    // Create output tree
    TFile *fout = new TFile(fout_, "recreate");
    TTree *outTree = new TTree(outTree_, "");

    cout << "[INFO] Systematic type: " << systtype_ << endl;

    // Tree variables
    // Event
    float EvtInfo_NPu = 0;
    int EvtInfo_NVtx = 0;
    int EvtInfo_EvtNo = 0;
    bool EvtInfo_passTrigger = 0;
    float EvtInfo_genweight = 0;
    float EvtInfo_Rho = 0;
    // Photons
    float DiPhoInfo_mass = 0;
    float DiPhoInfo_pt = 0;
    float DiPhoInfo_leadPt = 0;
    float DiPhoInfo_leadEta = 0;
    float DiPhoInfo_leadPhi = 0;
    float DiPhoInfo_leadE = 0;
    float DiPhoInfo_leadIDMVA = 0;
    bool DiPhoInfo_leadhasPixelSeed = 0;
    int DiPhoInfo_leadGenMatchType = 0;
    float DiPhoInfo_subleadPt = 0;
    float DiPhoInfo_subleadEta = 0;
    float DiPhoInfo_subleadPhi = 0;
    float DiPhoInfo_subleadE = 0;
    float DiPhoInfo_subleadIDMVA = 0;
    bool DiPhoInfo_subleadhasPixelSeed = 0;
    int DiPhoInfo_subleadGenMatchType = 0;
    float DiPhoInfo_diphotonMVA = 0;
    // Photon scale factors
    // Central
    float DiPhoInfo_centralWeight = 0;
    // Systematics
    // For trees of other systematics these are set to null values
    float DiPhoInfo_LooseMvaSFUp = 0;
    float DiPhoInfo_LooseMvaSFDown = 0;
    float DiPhoInfo_PreselSFUp = 0;
    float DiPhoInfo_PreselSFDown = 0;
    float DiPhoInfo_electronVetoSFUp = 0;
    float DiPhoInfo_electronVetoSFDown = 0;
    float DiPhoInfo_TriggerWeightUp = 0;
    float DiPhoInfo_TriggerWeightDown = 0;
    // Electrons
    // Input
    int ElecInfo_Size = 0;
    vector<int> *ElecInfo_Charge = 0;
    vector<float> *ElecInfo_Pt = 0;
    vector<float> *ElecInfo_Eta = 0;
    vector<float> *ElecInfo_Phi = 0;
    vector<float> *ElecInfo_Energy = 0;
    vector<bool> *ElecInfo_EGMCutBasedIDMedium = 0;
    vector<bool> *ElecInfo_EGMCutBasedIDTight = 0;
    // Output
    int ElecInfo_Size_ = 0;
    vector<int> *ElecInfo_Charge_ = 0;
    vector<float> *ElecInfo_Pt_ = 0;
    vector<float> *ElecInfo_Eta_ = 0;
    vector<float> *ElecInfo_Phi_ = 0;
    vector<float> *ElecInfo_Energy_ = 0;
    vector<bool> *ElecInfo_EGMCutBasedIDMedium_ = 0;
    vector<bool> *ElecInfo_EGMCutBasedIDTight_ = 0;
    // Muons
    // Input
    int MuonInfo_Size = 0;
    vector<int> *MuonInfo_Charge = 0;
    vector<float> *MuonInfo_Pt = 0;
    vector<float> *MuonInfo_Eta = 0;
    vector<float> *MuonInfo_Phi = 0;
    vector<float> *MuonInfo_Energy = 0;
    vector<float> *MuonInfo_PFIsoDeltaBetaCorrR04 = 0;
    vector<bool> *MuonInfo_CutBasedIdMedium = 0;
    vector<bool> *MuonInfo_CutBasedIdTight = 0;
    vector<bool> *MuonInfo_CutBasedIdTight_bestVtx = 0;
    // Output
    int MuonInfo_Size_ = 0;
    vector<int> *MuonInfo_Charge_ = 0;
    vector<float> *MuonInfo_Pt_ = 0;
    vector<float> *MuonInfo_Eta_ = 0;
    vector<float> *MuonInfo_Phi_ = 0;
    vector<float> *MuonInfo_Energy_ = 0;
    vector<float> *MuonInfo_PFIsoDeltaBetaCorrR04_ = 0;
    vector<bool> *MuonInfo_CutBasedIdMedium_ = 0;
    vector<bool> *MuonInfo_CutBasedIdTight_ = 0;
    vector<bool> *MuonInfo_CutBasedIdTight_bestVtx_ = 0;
    // Jets
    // Input
    int jets_size = 0;
    vector<float> *JetInfo_Pt = 0;
    vector<float> *JetInfo_Eta = 0;
    vector<float> *JetInfo_Phi = 0;
    vector<float> *JetInfo_Energy = 0;
    vector<float> *JetInfo_pfDeepCSVJetTags_probb = 0;
    vector<float> *JetInfo_pfDeepCSVJetTags_probbb = 0;
    vector<float> *JetInfo_pfDeepCSVJetTags_probc = 0;
    vector<float> *JetInfo_pfDeepCSVJetTags_probudsg = 0;
    vector<float> *JetInfo_JECUnc = 0;
    vector<float> *JetInfo_JERScale = 0;
    vector<int> *JetInfo_GenHadronFlavor = 0;
    // Output
    int jets_size_ = 0;
    vector<float> *JetInfo_Pt_ = 0;
    vector<float> *JetInfo_Eta_ = 0;
    vector<float> *JetInfo_Phi_ = 0;
    vector<float> *JetInfo_Energy_ = 0;
    vector<float> *JetInfo_pfDeepCSVJetTags_probb_ = 0;
    vector<float> *JetInfo_pfDeepCSVJetTags_probbb_ = 0;
    vector<float> *JetInfo_pfDeepCSVJetTags_probc_ = 0;
    vector<float> *JetInfo_pfDeepCSVJetTags_probudsg_ = 0;
    vector<int> *JetInfo_GenHadronFlavor_ = 0;
    // MET
    float MetInfo_Pt = 0;
    float MetInfo_Phi = 0;
    // Gen-level objects
    int GenPartInfo_size = 0;
    vector<float> *GenPartInfo_Pt = 0;
    vector<float> *GenPartInfo_Eta = 0;
    vector<float> *GenPartInfo_Phi = 0;
    vector<float> *GenPartInfo_Mass = 0;
    vector<int> *GenPartInfo_PdgID = 0;
    vector<int> *GenPartInfo_Status = 0;
    vector<bool> *GenPartInfo_isHardProcess = 0;
    vector<bool> *GenPartInfo_fromHardProcessFinalState = 0;
    vector<bool> *GenPartInfo_isPromptFinalState = 0;
    vector<int> *GenPartInfo_MomPdgID = 0;
    vector<int> *GenPartInfo_MomStatus = 0;
    vector<float> *GenPartInfo_MomPt = 0;
    vector<float> *GenPartInfo_MomEta = 0;
    vector<float> *GenPartInfo_MomPhi = 0;
    vector<float> *GenPartInfo_MomMass = 0;

    // Set input tree branches
    // Event
    inTree->SetBranchAddress("EvtInfo.NPu", &EvtInfo_NPu);
    inTree->SetBranchAddress("EvtInfo.NVtx", &EvtInfo_NVtx);
    inTree->SetBranchAddress("EvtInfo.EvtNo", &EvtInfo_EvtNo);
    inTree->SetBranchAddress("EvtInfo.passTrigger", &EvtInfo_passTrigger);
    inTree->SetBranchAddress("EvtInfo.genweight", &EvtInfo_genweight);
    inTree->SetBranchAddress("EvtInfo.Rho", &EvtInfo_Rho);
    // Photons
    inTree->SetBranchAddress("DiPhoInfo.mass", &DiPhoInfo_mass);
    inTree->SetBranchAddress("DiPhoInfo.pt", &DiPhoInfo_pt);
    inTree->SetBranchAddress("DiPhoInfo.leadPt", &DiPhoInfo_leadPt);
    inTree->SetBranchAddress("DiPhoInfo.leadEta", &DiPhoInfo_leadEta);
    inTree->SetBranchAddress("DiPhoInfo.leadPhi", &DiPhoInfo_leadPhi);
    inTree->SetBranchAddress("DiPhoInfo.leadE", &DiPhoInfo_leadE);
    inTree->SetBranchAddress("DiPhoInfo.leadIDMVA", &DiPhoInfo_leadIDMVA);
    inTree->SetBranchAddress("DiPhoInfo.leadhasPixelSeed", &DiPhoInfo_leadhasPixelSeed);
    inTree->SetBranchAddress("DiPhoInfo.leadGenMatchType", &DiPhoInfo_leadGenMatchType);
    inTree->SetBranchAddress("DiPhoInfo.subleadPt", &DiPhoInfo_subleadPt);
    inTree->SetBranchAddress("DiPhoInfo.subleadEta", &DiPhoInfo_subleadEta);
    inTree->SetBranchAddress("DiPhoInfo.subleadPhi", &DiPhoInfo_subleadPhi);
    inTree->SetBranchAddress("DiPhoInfo.subleadE", &DiPhoInfo_subleadE);
    inTree->SetBranchAddress("DiPhoInfo.subleadIDMVA", &DiPhoInfo_subleadIDMVA);
    inTree->SetBranchAddress("DiPhoInfo.subleadhasPixelSeed", &DiPhoInfo_subleadhasPixelSeed);
    inTree->SetBranchAddress("DiPhoInfo.subleadGenMatchType", &DiPhoInfo_subleadGenMatchType);
    inTree->SetBranchAddress("DiPhoInfo.diphotonMVA", &DiPhoInfo_diphotonMVA);
    // Photon scale factors
    // Central
    inTree->SetBranchAddress("DiPhoInfo.centralWeight", &DiPhoInfo_centralWeight);
    // Systematics
    inTree->SetBranchAddress("DiPhoInfo.LooseMvaSFUp", &DiPhoInfo_LooseMvaSFUp);
    inTree->SetBranchAddress("DiPhoInfo.LooseMvaSFDown", &DiPhoInfo_LooseMvaSFDown);
    inTree->SetBranchAddress("DiPhoInfo.PreselSFUp", &DiPhoInfo_PreselSFUp);
    inTree->SetBranchAddress("DiPhoInfo.PreselSFDown", &DiPhoInfo_PreselSFDown);
    inTree->SetBranchAddress("DiPhoInfo.electronVetoSFUp", &DiPhoInfo_electronVetoSFUp);
    inTree->SetBranchAddress("DiPhoInfo.electronVetoSFDown", &DiPhoInfo_electronVetoSFDown);
    inTree->SetBranchAddress("DiPhoInfo.TriggerWeightUp", &DiPhoInfo_TriggerWeightUp);
    inTree->SetBranchAddress("DiPhoInfo.TriggerWeightDown", &DiPhoInfo_TriggerWeightDown);
    // Electrons
    inTree->SetBranchAddress("ElecInfo.Size", &ElecInfo_Size);
    inTree->SetBranchAddress("ElecInfo.Charge", &ElecInfo_Charge);
    inTree->SetBranchAddress("ElecInfo.Pt", &ElecInfo_Pt);
    inTree->SetBranchAddress("ElecInfo.Eta", &ElecInfo_Eta);
    inTree->SetBranchAddress("ElecInfo.Phi", &ElecInfo_Phi);
    inTree->SetBranchAddress("ElecInfo.Energy", &ElecInfo_Energy);
    inTree->SetBranchAddress("ElecInfo.EGMCutBasedIDMedium", &ElecInfo_EGMCutBasedIDMedium);
    inTree->SetBranchAddress("ElecInfo.EGMCutBasedIDTight", &ElecInfo_EGMCutBasedIDTight);
    // Muons
    inTree->SetBranchAddress("MuonInfo.Size", &MuonInfo_Size);
    inTree->SetBranchAddress("MuonInfo.Charge", &MuonInfo_Charge);
    inTree->SetBranchAddress("MuonInfo.Pt", &MuonInfo_Pt);
    inTree->SetBranchAddress("MuonInfo.Eta", &MuonInfo_Eta);
    inTree->SetBranchAddress("MuonInfo.Phi", &MuonInfo_Phi);
    inTree->SetBranchAddress("MuonInfo.Energy", &MuonInfo_Energy);
    inTree->SetBranchAddress("MuonInfo.PFIsoDeltaBetaCorrR04", &MuonInfo_PFIsoDeltaBetaCorrR04);
    inTree->SetBranchAddress("MuonInfo.CutBasedIdMedium", &MuonInfo_CutBasedIdMedium);
    inTree->SetBranchAddress("MuonInfo.CutBasedIdTight", &MuonInfo_CutBasedIdTight);
    inTree->SetBranchAddress("MuonInfo.CutBasedIdTight_bestVtx", &MuonInfo_CutBasedIdTight_bestVtx);
    // Jets
    inTree->SetBranchAddress("jets_size", &jets_size);
    inTree->SetBranchAddress("JetInfo.Pt", &JetInfo_Pt);
    inTree->SetBranchAddress("JetInfo.Eta", &JetInfo_Eta);
    inTree->SetBranchAddress("JetInfo.Phi", &JetInfo_Phi);
    inTree->SetBranchAddress("JetInfo.Energy", &JetInfo_Energy);
    inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probb", &JetInfo_pfDeepCSVJetTags_probb);
    inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probbb", &JetInfo_pfDeepCSVJetTags_probbb);
    inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probc", &JetInfo_pfDeepCSVJetTags_probc);
    inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probudsg", &JetInfo_pfDeepCSVJetTags_probudsg);
    // JEC
    if (systtype == JECUp || systtype == JECDown)
        inTree->SetBranchAddress("JetInfo.JECUnc", &JetInfo_JECUnc);
    // JER
    if (systtype == JERUp)
        inTree->SetBranchAddress("JetInfo.JERUp", &JetInfo_JERScale);
    else if (systtype == JERDown)
        inTree->SetBranchAddress("JetInfo.JERDown", &JetInfo_JERScale);
    else
        inTree->SetBranchAddress("JetInfo.JERScale", &JetInfo_JERScale);
    inTree->SetBranchAddress("JetInfo.GenHadronFlavor", &JetInfo_GenHadronFlavor);
    // MET
    if (systtype == METJetEnUp) {
        inTree->SetBranchAddress("MetInfo.CorrPtShiftJetEnUp", &MetInfo_Pt);
        inTree->SetBranchAddress("MetInfo.CorrPhiShiftJetEnUp", &MetInfo_Phi);
    }
    else if (systtype == METJetEnDown) {
        inTree->SetBranchAddress("MetInfo.CorrPtShiftJetEnDown", &MetInfo_Pt);
        inTree->SetBranchAddress("MetInfo.CorrPhiShiftJetEnDown", &MetInfo_Phi);
    }
    else if (systtype == METJetResUp) {
        inTree->SetBranchAddress("MetInfo.CorrPtShiftJetResUp", &MetInfo_Pt);
        inTree->SetBranchAddress("MetInfo.CorrPhiShiftJetResUp", &MetInfo_Phi);
    }
    else if (systtype == METJetResDown) {
        inTree->SetBranchAddress("MetInfo.CorrPtShiftJetResDown", &MetInfo_Pt);
        inTree->SetBranchAddress("MetInfo.CorrPhiShiftJetResDown", &MetInfo_Phi);
    }
    else if (systtype == METUncEnUp) {
        inTree->SetBranchAddress("MetInfo.CorrPtShiftUncEnUp", &MetInfo_Pt);
        inTree->SetBranchAddress("MetInfo.CorrPhiShiftUncEnUp", &MetInfo_Phi);
    }
    else if (systtype == METUncEnDown) {
        inTree->SetBranchAddress("MetInfo.CorrPtShiftUncEnDown", &MetInfo_Pt);
        inTree->SetBranchAddress("MetInfo.CorrPhiShiftUncEnDown", &MetInfo_Phi);
    }
    else if (systtype == METPhoEnUp) {
        inTree->SetBranchAddress("MetInfo.CorrPtShiftPhoEnUp", &MetInfo_Pt);
        inTree->SetBranchAddress("MetInfo.CorrPhiShiftPhoEnUp", &MetInfo_Phi);
    }
    else if (systtype == METPhoEnDown) {
        inTree->SetBranchAddress("MetInfo.CorrPtShiftPhoEnDown", &MetInfo_Pt);
        inTree->SetBranchAddress("MetInfo.CorrPhiShiftPhoEnDown", &MetInfo_Phi);
    }
    else {
        inTree->SetBranchAddress("MetInfo.Pt", &MetInfo_Pt);
        inTree->SetBranchAddress("MetInfo.Phi", &MetInfo_Phi);
    }
    // Gen-level objects
    inTree->SetBranchAddress("GenPartInfo.size", &GenPartInfo_size);
    inTree->SetBranchAddress("GenPartInfo.Pt", &GenPartInfo_Pt);
    inTree->SetBranchAddress("GenPartInfo.Eta", &GenPartInfo_Eta);
    inTree->SetBranchAddress("GenPartInfo.Phi", &GenPartInfo_Phi);
    inTree->SetBranchAddress("GenPartInfo.Mass", &GenPartInfo_Mass);
    inTree->SetBranchAddress("GenPartInfo.PdgID", &GenPartInfo_PdgID);
    inTree->SetBranchAddress("GenPartInfo.Status", &GenPartInfo_Status);
    inTree->SetBranchAddress("GenPartInfo.isHardProcess", &GenPartInfo_isHardProcess);
    inTree->SetBranchAddress("GenPartInfo.fromHardProcessFinalState", &GenPartInfo_fromHardProcessFinalState);
    inTree->SetBranchAddress("GenPartInfo.isPromptFinalState", &GenPartInfo_isPromptFinalState);
    inTree->SetBranchAddress("GenPartInfo.MomPdgID", &GenPartInfo_MomPdgID);
    inTree->SetBranchAddress("GenPartInfo.MomStatus", &GenPartInfo_MomStatus);
    inTree->SetBranchAddress("GenPartInfo.MomPt", &GenPartInfo_MomPt);
    inTree->SetBranchAddress("GenPartInfo.MomEta", &GenPartInfo_MomEta);
    inTree->SetBranchAddress("GenPartInfo.MomPhi", &GenPartInfo_MomPhi);
    inTree->SetBranchAddress("GenPartInfo.MomMass", &GenPartInfo_MomMass);

    // Set outupt tree branches
    // Event
    outTree->Branch("EvtInfo.NPu", &EvtInfo_NPu);
    outTree->Branch("EvtInfo.NVtx", &EvtInfo_NVtx);
    outTree->Branch("EvtInfo.EvtNo", &EvtInfo_EvtNo);
    outTree->Branch("EvtInfo.passTrigger", &EvtInfo_passTrigger);
    outTree->Branch("EvtInfo.genweight", &EvtInfo_genweight);
    outTree->Branch("EvtInfo.Rho", &EvtInfo_Rho);
    // Photons
    outTree->Branch("DiPhoInfo.mass", &DiPhoInfo_mass);
    outTree->Branch("DiPhoInfo.pt", &DiPhoInfo_pt);
    outTree->Branch("DiPhoInfo.leadPt", &DiPhoInfo_leadPt);
    outTree->Branch("DiPhoInfo.leadEta", &DiPhoInfo_leadEta);
    outTree->Branch("DiPhoInfo.leadPhi", &DiPhoInfo_leadPhi);
    outTree->Branch("DiPhoInfo.leadE", &DiPhoInfo_leadE);
    outTree->Branch("DiPhoInfo.leadIDMVA", &DiPhoInfo_leadIDMVA);
    outTree->Branch("DiPhoInfo.leadhasPixelSeed", &DiPhoInfo_leadhasPixelSeed);
    outTree->Branch("DiPhoInfo.leadGenMatchType", &DiPhoInfo_leadGenMatchType);
    outTree->Branch("DiPhoInfo.subleadPt", &DiPhoInfo_subleadPt);
    outTree->Branch("DiPhoInfo.subleadEta", &DiPhoInfo_subleadEta);
    outTree->Branch("DiPhoInfo.subleadPhi", &DiPhoInfo_subleadPhi);
    outTree->Branch("DiPhoInfo.subleadE", &DiPhoInfo_subleadE);
    outTree->Branch("DiPhoInfo.subleadIDMVA", &DiPhoInfo_subleadIDMVA);
    outTree->Branch("DiPhoInfo.subleadhasPixelSeed", &DiPhoInfo_subleadhasPixelSeed);
    outTree->Branch("DiPhoInfo.subleadGenMatchType", &DiPhoInfo_subleadGenMatchType);
    outTree->Branch("DiPhoInfo.diphotonMVA", &DiPhoInfo_diphotonMVA);
    // Photon scale photons
    // Central
    outTree->Branch("DiPhoInfo.centralWeight", &DiPhoInfo_centralWeight);
    // Systematics
    outTree->Branch("DiPhoInfo.LooseMvaSFUp", &DiPhoInfo_LooseMvaSFUp);
    outTree->Branch("DiPhoInfo.LooseMvaSFDown", &DiPhoInfo_LooseMvaSFDown);
    outTree->Branch("DiPhoInfo.PreselSFUp", &DiPhoInfo_PreselSFUp);
    outTree->Branch("DiPhoInfo.PreselSFDown", &DiPhoInfo_PreselSFDown);
    outTree->Branch("DiPhoInfo.electronVetoSFUp", &DiPhoInfo_electronVetoSFUp);
    outTree->Branch("DiPhoInfo.electronVetoSFDown", &DiPhoInfo_electronVetoSFDown);
    outTree->Branch("DiPhoInfo.TriggerWeightUp", &DiPhoInfo_TriggerWeightUp);
    outTree->Branch("DiPhoInfo.TriggerWeightDown", &DiPhoInfo_TriggerWeightDown);
    // Electrons
    outTree->Branch("ElecInfo.Size", &ElecInfo_Size_);
    outTree->Branch("ElecInfo.Charge", &ElecInfo_Charge_);
    outTree->Branch("ElecInfo.Pt", &ElecInfo_Pt_);
    outTree->Branch("ElecInfo.Eta", &ElecInfo_Eta_);
    outTree->Branch("ElecInfo.Phi", &ElecInfo_Phi_);
    outTree->Branch("ElecInfo.Energy", &ElecInfo_Energy_);
    outTree->Branch("ElecInfo.EGMCutBasedIDMedium", &ElecInfo_EGMCutBasedIDMedium_);
    outTree->Branch("ElecInfo.EGMCutBasedIDTight", &ElecInfo_EGMCutBasedIDTight_);
    // Muons
    outTree->Branch("MuonInfo.Size", &MuonInfo_Size_);
    outTree->Branch("MuonInfo.Charge", &MuonInfo_Charge_);
    outTree->Branch("MuonInfo.Pt", &MuonInfo_Pt_);
    outTree->Branch("MuonInfo.Eta", &MuonInfo_Eta_);
    outTree->Branch("MuonInfo.Phi", &MuonInfo_Phi_);
    outTree->Branch("MuonInfo.Energy", &MuonInfo_Energy_);
    outTree->Branch("MuonInfo.PFIsoDeltaBetaCorrR04", &MuonInfo_PFIsoDeltaBetaCorrR04_);
    outTree->Branch("MuonInfo.CutBasedIdMedium", &MuonInfo_CutBasedIdMedium_);
    outTree->Branch("MuonInfo.CutBasedIdTight", &MuonInfo_CutBasedIdTight_);
    outTree->Branch("MuonInfo.CutBasedIdTight_bestVtx", &MuonInfo_CutBasedIdTight_bestVtx_);
    // Jets
    outTree->Branch("jets_size", &jets_size_);
    outTree->Branch("JetInfo.Pt", &JetInfo_Pt_);
    outTree->Branch("JetInfo.Eta", &JetInfo_Eta_);
    outTree->Branch("JetInfo.Phi", &JetInfo_Phi_);
    outTree->Branch("JetInfo.Energy", &JetInfo_Energy_);
    outTree->Branch("JetInfo.pfDeepCSVJetTags_probb", &JetInfo_pfDeepCSVJetTags_probb_);
    outTree->Branch("JetInfo.pfDeepCSVJetTags_probbb", &JetInfo_pfDeepCSVJetTags_probbb_);
    outTree->Branch("JetInfo.pfDeepCSVJetTags_probc", &JetInfo_pfDeepCSVJetTags_probc_);
    outTree->Branch("JetInfo.pfDeepCSVJetTags_probudsg", &JetInfo_pfDeepCSVJetTags_probudsg_);
    outTree->Branch("JetInfo.GenHadronFlavor", &JetInfo_GenHadronFlavor_);
    // MET
    outTree->Branch("MetInfo.Pt", &MetInfo_Pt);
    outTree->Branch("MetInfo.Phi", &MetInfo_Phi);
    // Gen-level objects
    outTree->Branch("GenPartInfo.size", &GenPartInfo_size);
    outTree->Branch("GenPartInfo.Pt", &GenPartInfo_Pt);
    outTree->Branch("GenPartInfo.Eta", &GenPartInfo_Eta);
    outTree->Branch("GenPartInfo.Phi", &GenPartInfo_Phi);
    outTree->Branch("GenPartInfo.Mass", &GenPartInfo_Mass);
    outTree->Branch("GenPartInfo.PdgID", &GenPartInfo_PdgID);
    outTree->Branch("GenPartInfo.Status", &GenPartInfo_Status);
    outTree->Branch("GenPartInfo.isHardProcess", &GenPartInfo_isHardProcess);
    outTree->Branch("GenPartInfo.fromHardProcessFinalState", &GenPartInfo_fromHardProcessFinalState);
    outTree->Branch("GenPartInfo.isPromptFinalState", &GenPartInfo_isPromptFinalState);
    outTree->Branch("GenPartInfo.MomPdgID", &GenPartInfo_MomPdgID);
    outTree->Branch("GenPartInfo.MomStatus", &GenPartInfo_MomStatus);
    outTree->Branch("GenPartInfo.MomPt", &GenPartInfo_MomPt);
    outTree->Branch("GenPartInfo.MomEta", &GenPartInfo_MomEta);
    outTree->Branch("GenPartInfo.MomPhi", &GenPartInfo_MomPhi);
    outTree->Branch("GenPartInfo.MomMass", &GenPartInfo_MomMass);

    // Start event loop
    for (int evt=0; evt<inTree->GetEntries(); ++evt)
    {
        if (evt % 100000 == 0)
            cout << "Processing event " << evt << endl;
        // Load event
        inTree->GetEntry(evt);

        // Apply JEC systematic unc
        if (systtype == JECUp) {
            for (int i=0; i<jets_size; ++i) {
                JetInfo_Pt->at(i) *= (1. + JetInfo_JECUnc->at(i)) * JetInfo_JERScale->at(i);
                JetInfo_Energy->at(i) *= (1. + JetInfo_JECUnc->at(i)) * JetInfo_JERScale->at(i);
            }
        }
        else if (systtype == JECDown) {
            for (int i=0; i<jets_size; ++i) {
                JetInfo_Pt->at(i) *= (1. - JetInfo_JECUnc->at(i)) * JetInfo_JERScale->at(i);
                JetInfo_Energy->at(i) *= (1. - JetInfo_JECUnc->at(i)) * JetInfo_JERScale->at(i);
            }
        }
        else {
            for (int i=0; i<jets_size; ++i) {
                JetInfo_Pt->at(i) *= JetInfo_JERScale->at(i);
                JetInfo_Energy->at(i) *= JetInfo_JERScale->at(i);
            }
        }
        /* Note: The correct JER & MET are already loaded based on the specified systtype */

        //////////////////
        // Preselection //
        //////////////////

        // Selection cuts on whole events
        if (!EvtInfo_passTrigger) continue;

        if (DiPhoInfo_mass < 100) continue;

        if (DiPhoInfo_leadPt < 35) continue;
        if (DiPhoInfo_subleadPt < 25) continue;
        if (fabs(DiPhoInfo_leadEta) > 2.5) continue;
        if (fabs(DiPhoInfo_subleadEta) > 2.5) continue;
        if (fabs(DiPhoInfo_leadEta) > 1.4442 && fabs(DiPhoInfo_leadEta) < 1.566) continue;
        if (fabs(DiPhoInfo_subleadEta) > 1.4442 && fabs(DiPhoInfo_subleadEta) < 1.566) continue;

        // If the event passes the whole-event cuts, do selection on its physical objects and fill it to output TTree

        // Reset output branches
        ElecInfo_Size_ = 0;
        ElecInfo_Charge_->clear();
        ElecInfo_Pt_->clear();
        ElecInfo_Eta_->clear();
        ElecInfo_Phi_->clear();
        ElecInfo_Energy_->clear();
        ElecInfo_EGMCutBasedIDMedium_->clear();
        ElecInfo_EGMCutBasedIDTight_->clear();
        MuonInfo_Size_ = 0;
        MuonInfo_Charge_->clear();
        MuonInfo_Pt_->clear();
        MuonInfo_Eta_->clear();
        MuonInfo_Phi_->clear();
        MuonInfo_Energy_->clear();
        MuonInfo_PFIsoDeltaBetaCorrR04_->clear();
        MuonInfo_CutBasedIdMedium_->clear();
        MuonInfo_CutBasedIdTight_->clear();
        MuonInfo_CutBasedIdTight_bestVtx_->clear();
        jets_size_ = 0;
        JetInfo_Pt_->clear();
        JetInfo_Eta_->clear();
        JetInfo_Phi_->clear();
        JetInfo_Energy_->clear();
        JetInfo_pfDeepCSVJetTags_probb_->clear();
        JetInfo_pfDeepCSVJetTags_probbb_->clear();
        JetInfo_pfDeepCSVJetTags_probc_->clear();
        JetInfo_pfDeepCSVJetTags_probudsg_->clear();
        JetInfo_GenHadronFlavor_->clear();

        // Create 4-momenta of leading photon and subleading photon (used when calculating deltaR with other objects)
        TLorentzVector leadPho, subleadPho;
        leadPho.SetPtEtaPhiE(DiPhoInfo_leadPt, DiPhoInfo_leadEta, DiPhoInfo_leadPhi, DiPhoInfo_leadE);
        subleadPho.SetPtEtaPhiE(DiPhoInfo_subleadPt, DiPhoInfo_subleadEta, DiPhoInfo_subleadPhi, DiPhoInfo_subleadE);

        // Create vectors of 4-momenta of selected leptons (used when calculating deltaR with jets)
        vector<TLorentzVector*> electrons;
        vector<TLorentzVector*> muons;

        // Selection of electrons
        for (int i=0; i<ElecInfo_Size; ++i)
        {
            if (!ElecInfo_EGMCutBasedIDMedium->at(i)) continue;
            if (ElecInfo_Pt->at(i) < 10) continue;
            if (fabs(ElecInfo_Eta->at(i)) > 2.4) continue;
            if (fabs(ElecInfo_Eta->at(i)) > 1.4442 && fabs(ElecInfo_Eta->at(i)) < 1.566) continue;
            TLorentzVector *electron_tmp = new TLorentzVector();
            electron_tmp->SetPtEtaPhiE(ElecInfo_Pt->at(i), ElecInfo_Eta->at(i), ElecInfo_Phi->at(i), ElecInfo_Energy->at(i));
            // Cut on the angles between the electrons and photons
            if (electron_tmp->DeltaR(leadPho) < 0.2 || electron_tmp->DeltaR(subleadPho) < 0.2) { delete electron_tmp; continue; }
            electrons.push_back(electron_tmp);

            // Fill selected electrons to output vectors
            ElecInfo_Size_ += 1;
            ElecInfo_Charge_->push_back( ElecInfo_Charge->at(i) );
            ElecInfo_Pt_->push_back( ElecInfo_Pt->at(i) );
            ElecInfo_Eta_->push_back( ElecInfo_Eta->at(i) );
            ElecInfo_Phi_->push_back( ElecInfo_Phi->at(i) );
            ElecInfo_Energy_->push_back( ElecInfo_Energy->at(i) );
            ElecInfo_EGMCutBasedIDMedium_->push_back( ElecInfo_EGMCutBasedIDMedium->at(i) );
            ElecInfo_EGMCutBasedIDTight_->push_back( ElecInfo_EGMCutBasedIDTight->at(i) );
        }

        // Selection of muons
        for (int i=0; i<MuonInfo_Size; ++i)
        {
            if (!MuonInfo_CutBasedIdMedium->at(i)) continue;
            if (MuonInfo_Pt->at(i) < 5) continue;
            if (fabs(MuonInfo_Eta->at(i)) > 2.4) continue;
            if (MuonInfo_PFIsoDeltaBetaCorrR04->at(i) > 0.25) continue;
            TLorentzVector *muon_tmp = new TLorentzVector();
            muon_tmp->SetPtEtaPhiE(MuonInfo_Pt->at(i), MuonInfo_Eta->at(i), MuonInfo_Phi->at(i), MuonInfo_Energy->at(i));
            // Cut on the angles between the muons and photons
            if (muon_tmp->DeltaR(leadPho) < 0.2 || muon_tmp->DeltaR(subleadPho) < 0.2) { delete muon_tmp; continue; }
            muons.push_back(muon_tmp);

            // Fill selected muons to output vectors
            MuonInfo_Size_ += 1;
            MuonInfo_Charge_->push_back( MuonInfo_Charge->at(i) );
            MuonInfo_Pt_->push_back( MuonInfo_Pt->at(i) );
            MuonInfo_Eta_->push_back( MuonInfo_Eta->at(i) );
            MuonInfo_Phi_->push_back( MuonInfo_Phi->at(i) );
            MuonInfo_Energy_->push_back( MuonInfo_Energy->at(i) );
            MuonInfo_PFIsoDeltaBetaCorrR04_->push_back( MuonInfo_PFIsoDeltaBetaCorrR04->at(i) );
            MuonInfo_CutBasedIdMedium_->push_back( MuonInfo_CutBasedIdMedium->at(i) );
            MuonInfo_CutBasedIdTight_->push_back( MuonInfo_CutBasedIdTight->at(i) );
            MuonInfo_CutBasedIdTight_bestVtx_->push_back( MuonInfo_CutBasedIdTight_bestVtx->at(i) );
        }

        // Selection of jets
        for (int i=0; i<jets_size; ++i)
        {
            if (JetInfo_Pt->at(i) < 25) continue;
            if (fabs(JetInfo_Eta->at(i)) > 2.4) continue;
            TLorentzVector jet_tmp;
            jet_tmp.SetPtEtaPhiE(JetInfo_Pt->at(i), JetInfo_Eta->at(i), JetInfo_Phi->at(i), JetInfo_Energy->at(i));
            // Cut on the angles between the jets and photons
            if (jet_tmp.DeltaR(leadPho) < 0.4 || jet_tmp.DeltaR(subleadPho) < 0.4) continue;
            // Cut on the angles between the jets and selected leptons
            bool jet_is_iso = true;
            for (int j=0; j<electrons.size(); ++j)
                if (jet_tmp.DeltaR(*electrons.at(j)) < 0.4) { jet_is_iso = false; break; }
            if (!jet_is_iso) continue;
            for (int j=0; j<muons.size(); ++j)
                if (jet_tmp.DeltaR(*muons.at(j)) < 0.4) { jet_is_iso = false; break; }
            if (!jet_is_iso) continue;

            // Fill selected jets to output vector
            jets_size_ += 1;
            JetInfo_Pt_->push_back( JetInfo_Pt->at(i) );
            JetInfo_Eta_->push_back( JetInfo_Eta->at(i) );
            JetInfo_Phi_->push_back( JetInfo_Phi->at(i) );
            JetInfo_Energy_->push_back( JetInfo_Energy->at(i) );
            JetInfo_pfDeepCSVJetTags_probb_->push_back( JetInfo_pfDeepCSVJetTags_probb->at(i) );
            JetInfo_pfDeepCSVJetTags_probbb_->push_back( JetInfo_pfDeepCSVJetTags_probbb->at(i) );
            JetInfo_pfDeepCSVJetTags_probc_->push_back( JetInfo_pfDeepCSVJetTags_probc->at(i) );
            JetInfo_pfDeepCSVJetTags_probudsg_->push_back( JetInfo_pfDeepCSVJetTags_probudsg->at(i) );
            JetInfo_GenHadronFlavor_->push_back( JetInfo_GenHadronFlavor->at(i) );
        }

        // Clean up
        for (int i=0; i<electrons.size(); ++i) delete electrons[i];
        for (int i=0; i<muons.size(); ++i) delete muons[i];

        outTree->Fill();
    } // End of event loop

    // Save result
    fout->WriteTObject(outTree);
    fout->Close();
    cout << "[INFO] Output saved: " << fout_ << endl;

    return 0;
}
