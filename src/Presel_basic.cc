// Preselection:
//   Trigger:
//     Diphoton triggers
//   Photon:
//     (1) pt > 35(25) GeV
//     (2) |eta| < 2.5, excluding [1.4442, 1.566] (EE-EB gap)
//   Diphoton:
//     Mgg > 100 GeV
//   Electron:
//     (1) Cut-based medium ID
//     (2) pt > 10 GeV
//     (3) |eta| < 2.4, excluding [1.4442, 1.566] (EE-EB gap)
//     (4) delta_R(electron, photon) > 0.2
//   Muon:
//     (1) Cut-based medium ID
//     (2) pt > 5 GeV
//     (3) |eta| < 2.4
//     (4) Relative PF isolation (R=0.4) < 0.25, (loose WP)
//     (5) delta_R(muon, photon) > 0.2
//   Jet:
//     (1) pt > 25 GeV
//     (2) |eta| < 2.4
//     (3) delta_R(lepton/photon, jet) > 0.4

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TString.h"

#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

int main(int argc, char **argv)
{
    // Get input arguments
    TString fin_name = argv[1];
    TString inTree_name = argv[2];
    TString fout_name = argv[3];

    // Create TTree for input n-tuple
    cout << "Openning input file: " << fin_name << endl;
    TChain *inTree = new TChain(inTree_name);
    inTree->Add(fin_name);

    // Set input tree branch status
    inTree->SetBranchStatus("*", 0);
    inTree->SetBranchStatus("EvtInfo.NPu", 1);
    inTree->SetBranchStatus("EvtInfo.NVtx", 1);
    inTree->SetBranchStatus("EvtInfo.passTrigger", 1);
    inTree->SetBranchStatus("EvtInfo.genweight", 1);
    inTree->SetBranchStatus("EvtInfo.Rho", 1);
    inTree->SetBranchStatus("DiPhoInfo.mass", 1);
    inTree->SetBranchStatus("DiPhoInfo.pt", 1);
    inTree->SetBranchStatus("DiPhoInfo.leadPt", 1);
    inTree->SetBranchStatus("DiPhoInfo.leadEta", 1);
    inTree->SetBranchStatus("DiPhoInfo.leadPhi", 1);
    inTree->SetBranchStatus("DiPhoInfo.leadE", 1);
    inTree->SetBranchStatus("DiPhoInfo.leadIDMVA", 1);
    inTree->SetBranchStatus("DiPhoInfo.leadhasPixelSeed", 1);
    inTree->SetBranchStatus("DiPhoInfo.leadGenMatch", 1);
    inTree->SetBranchStatus("DiPhoInfo.leadGenMatchType", 1);
    inTree->SetBranchStatus("DiPhoInfo.subleadPt", 1);
    inTree->SetBranchStatus("DiPhoInfo.subleadEta", 1);
    inTree->SetBranchStatus("DiPhoInfo.subleadPhi", 1);
    inTree->SetBranchStatus("DiPhoInfo.subleadE", 1);
    inTree->SetBranchStatus("DiPhoInfo.subleadIDMVA", 1);
    inTree->SetBranchStatus("DiPhoInfo.subleadhasPixelSeed", 1);
    inTree->SetBranchStatus("DiPhoInfo.subleadGenMatch", 1);
    inTree->SetBranchStatus("DiPhoInfo.subleadGenMatchType", 1);
    inTree->SetBranchStatus("DiPhoInfo.diphotonMVA", 1);
    inTree->SetBranchStatus("DiPhoInfo.centralWeight", 1);
    inTree->SetBranchStatus("ElecInfo.Size", 1);
    inTree->SetBranchStatus("ElecInfo.Charge", 1);
    inTree->SetBranchStatus("ElecInfo.Pt", 1);
    inTree->SetBranchStatus("ElecInfo.Eta", 1);
    inTree->SetBranchStatus("ElecInfo.Phi", 1);
    inTree->SetBranchStatus("ElecInfo.Energy", 1);
    inTree->SetBranchStatus("ElecInfo.EGMCutBasedIDMedium", 1);
    inTree->SetBranchStatus("ElecInfo.EGMCutBasedIDTight", 1);
    inTree->SetBranchStatus("MuonInfo.Size", 1);
    inTree->SetBranchStatus("MuonInfo.Charge", 1);
    inTree->SetBranchStatus("MuonInfo.MuonType", 1);
    inTree->SetBranchStatus("MuonInfo.Pt", 1);
    inTree->SetBranchStatus("MuonInfo.Eta", 1);
    inTree->SetBranchStatus("MuonInfo.Phi", 1);
    inTree->SetBranchStatus("MuonInfo.Energy", 1);
    inTree->SetBranchStatus("MuonInfo.PFIsoDeltaBetaCorrR04", 1);
    inTree->SetBranchStatus("MuonInfo.CutBasedIdMedium", 1);
    inTree->SetBranchStatus("MuonInfo.CutBasedIdTight", 1);
    inTree->SetBranchStatus("MuonInfo.CutBasedIdTight_bestVtx", 1);
    inTree->SetBranchStatus("jets_size", 1);
    inTree->SetBranchStatus("JetInfo.Pt", 1);
    inTree->SetBranchStatus("JetInfo.Eta", 1);
    inTree->SetBranchStatus("JetInfo.Phi", 1);
    inTree->SetBranchStatus("JetInfo.Mass", 1);
    inTree->SetBranchStatus("JetInfo.Energy", 1);
    inTree->SetBranchStatus("JetInfo.pfDeepCSVJetTags_probb", 1);
    inTree->SetBranchStatus("JetInfo.pfDeepCSVJetTags_probbb", 1);
    inTree->SetBranchStatus("JetInfo.pfDeepCSVJetTags_probc", 1);
    inTree->SetBranchStatus("JetInfo.pfDeepCSVJetTags_probudsg", 1);
    inTree->SetBranchStatus("JetInfo.JECScale", 1);
    inTree->SetBranchStatus("JetInfo.JERScale", 1);
    inTree->SetBranchStatus("MetInfo.Pt", 1);
    inTree->SetBranchStatus("MetInfo.Phi", 1);
    inTree->SetBranchStatus("MetInfo.Px", 1);
    inTree->SetBranchStatus("MetInfo.Py", 1);
    inTree->SetBranchStatus("MetInfo.SumET", 1);
    inTree->SetBranchStatus("GenPartInfo.size", 1);
    inTree->SetBranchStatus("GenPartInfo.Pt", 1);
    inTree->SetBranchStatus("GenPartInfo.Eta", 1);
    inTree->SetBranchStatus("GenPartInfo.Phi", 1);
    inTree->SetBranchStatus("GenPartInfo.Mass", 1);
    inTree->SetBranchStatus("GenPartInfo.PdgID", 1);
    inTree->SetBranchStatus("GenPartInfo.Status", 1);
    inTree->SetBranchStatus("GenPartInfo.isHardProcess", 1);
    inTree->SetBranchStatus("GenPartInfo.fromHardProcessFinalState", 1);
    inTree->SetBranchStatus("GenPartInfo.isPromptFinalState", 1);
    inTree->SetBranchStatus("GenPartInfo.isDirectPromptTauDecayProductFinalState", 1);
    inTree->SetBranchStatus("GenPartInfo.MomPdgID", 1);
    inTree->SetBranchStatus("GenPartInfo.MomStatus", 1);
    inTree->SetBranchStatus("GenPartInfo.MomPt", 1);
    inTree->SetBranchStatus("GenPartInfo.MomEta", 1);
    inTree->SetBranchStatus("GenPartInfo.MomPhi", 1);
    inTree->SetBranchStatus("GenPartInfo.MomMass", 1);

    // Create TTree for output n-tuple
    cout << "Creating output file: " << fout_name << endl;
    TFile *fout = new TFile(fout_name, "recreate");
    TTree *outTree = inTree->CloneTree(0);
    outTree->SetName("T");

    // input branches
    bool EvtInfo_passTrigger = 0;
    float DiPhoInfo_mass = 0;
    float DiPhoInfo_leadPt = 0;
    float DiPhoInfo_leadEta = 0;
    float DiPhoInfo_leadPhi = 0;
    float DiPhoInfo_leadE = 0;
    float DiPhoInfo_subleadPt = 0;
    float DiPhoInfo_subleadEta = 0;
    float DiPhoInfo_subleadPhi = 0;
    float DiPhoInfo_subleadE = 0;
    int ElecInfo_Size = 0;
    vector<int> *ElecInfo_Charge = 0;
    vector<float> *ElecInfo_Pt = 0;
    vector<float> *ElecInfo_Eta = 0;
    vector<float> *ElecInfo_Phi = 0;
    vector<float> *ElecInfo_Energy = 0;
    vector<bool> *ElecInfo_EGMCutBasedIDMedium = 0;
    vector<bool> *ElecInfo_EGMCutBasedIDTight = 0;
    int MuonInfo_Size = 0;
    vector<int> *MuonInfo_Charge = 0;
    vector<float> *MuonInfo_MuonType = 0;
    vector<float> *MuonInfo_Pt = 0;
    vector<float> *MuonInfo_Eta = 0;
    vector<float> *MuonInfo_Phi = 0;
    vector<float> *MuonInfo_Energy = 0;
    vector<float> *MuonInfo_PFIsoDeltaBetaCorrR04 = 0;
    vector<bool> *MuonInfo_CutBasedIdMedium = 0;
    vector<bool> *MuonInfo_CutBasedIdTight = 0;
    vector<bool> *MuonInfo_CutBasedIdTight_bestVtx = 0;
    int jets_size = 0;
    vector<float> *JetInfo_Pt = 0;
    vector<float> *JetInfo_Eta = 0;
    vector<float> *JetInfo_Phi = 0;
    vector<float> *JetInfo_Mass = 0;
    vector<float> *JetInfo_Energy = 0;
    vector<float> *JetInfo_pfDeepCSVJetTags_probb = 0;
    vector<float> *JetInfo_pfDeepCSVJetTags_probbb = 0;
    vector<float> *JetInfo_pfDeepCSVJetTags_probc = 0;
    vector<float> *JetInfo_pfDeepCSVJetTags_probudsg = 0;
    vector<float> *JetInfo_JECScale = 0;
    vector<float> *JetInfo_JERScale = 0;
    inTree->SetBranchAddress("EvtInfo.passTrigger", &EvtInfo_passTrigger);
    inTree->SetBranchAddress("DiPhoInfo.mass", &DiPhoInfo_mass);
    inTree->SetBranchAddress("DiPhoInfo.leadPt", &DiPhoInfo_leadPt);
    inTree->SetBranchAddress("DiPhoInfo.leadEta", &DiPhoInfo_leadEta);
    inTree->SetBranchAddress("DiPhoInfo.leadPhi", &DiPhoInfo_leadPhi);
    inTree->SetBranchAddress("DiPhoInfo.leadE", &DiPhoInfo_leadE);
    inTree->SetBranchAddress("DiPhoInfo.subleadPt", &DiPhoInfo_subleadPt);
    inTree->SetBranchAddress("DiPhoInfo.subleadEta", &DiPhoInfo_subleadEta);
    inTree->SetBranchAddress("DiPhoInfo.subleadPhi", &DiPhoInfo_subleadPhi);
    inTree->SetBranchAddress("DiPhoInfo.subleadE", &DiPhoInfo_subleadE);
    inTree->SetBranchAddress("ElecInfo.Size", &ElecInfo_Size);
    inTree->SetBranchAddress("ElecInfo.Charge", &ElecInfo_Charge);
    inTree->SetBranchAddress("ElecInfo.Pt", &ElecInfo_Pt);
    inTree->SetBranchAddress("ElecInfo.Eta", &ElecInfo_Eta);
    inTree->SetBranchAddress("ElecInfo.Phi", &ElecInfo_Phi);
    inTree->SetBranchAddress("ElecInfo.Energy", &ElecInfo_Energy);
    inTree->SetBranchAddress("ElecInfo.EGMCutBasedIDMedium", &ElecInfo_EGMCutBasedIDMedium);
    inTree->SetBranchAddress("ElecInfo.EGMCutBasedIDTight", &ElecInfo_EGMCutBasedIDTight);
    inTree->SetBranchAddress("MuonInfo.Size", &MuonInfo_Size);
    inTree->SetBranchAddress("MuonInfo.Charge", &MuonInfo_Charge);
    inTree->SetBranchAddress("MuonInfo.MuonType", &MuonInfo_MuonType);
    inTree->SetBranchAddress("MuonInfo.Pt", &MuonInfo_Pt);
    inTree->SetBranchAddress("MuonInfo.Eta", &MuonInfo_Eta);
    inTree->SetBranchAddress("MuonInfo.Phi", &MuonInfo_Phi);
    inTree->SetBranchAddress("MuonInfo.Energy", &MuonInfo_Energy);
    inTree->SetBranchAddress("MuonInfo.PFIsoDeltaBetaCorrR04", &MuonInfo_PFIsoDeltaBetaCorrR04);
    inTree->SetBranchAddress("MuonInfo.CutBasedIdMedium", &MuonInfo_CutBasedIdMedium);
    inTree->SetBranchAddress("MuonInfo.CutBasedIdTight", &MuonInfo_CutBasedIdTight);
    inTree->SetBranchAddress("MuonInfo.CutBasedIdTight_bestVtx", &MuonInfo_CutBasedIdTight_bestVtx);
    inTree->SetBranchAddress("jets_size", &jets_size);
    inTree->SetBranchAddress("JetInfo.Pt", &JetInfo_Pt);
    inTree->SetBranchAddress("JetInfo.Eta", &JetInfo_Eta);
    inTree->SetBranchAddress("JetInfo.Phi", &JetInfo_Phi);
    inTree->SetBranchAddress("JetInfo.Mass", &JetInfo_Mass);
    inTree->SetBranchAddress("JetInfo.Energy", &JetInfo_Energy);
    inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probb", &JetInfo_pfDeepCSVJetTags_probb);
    inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probbb", &JetInfo_pfDeepCSVJetTags_probbb);
    inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probc", &JetInfo_pfDeepCSVJetTags_probc);
    inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probudsg", &JetInfo_pfDeepCSVJetTags_probudsg);
    inTree->SetBranchAddress("JetInfo.JECScale", &JetInfo_JECScale);
    inTree->SetBranchAddress("JetInfo.JERScale", &JetInfo_JERScale);

    // output branches
    int ElecInfo_Size_ = 0;
    vector<int> *ElecInfo_Charge_ = 0;
    vector<float> *ElecInfo_Pt_ = 0;
    vector<float> *ElecInfo_Eta_ = 0;
    vector<float> *ElecInfo_Phi_ = 0;
    vector<float> *ElecInfo_Energy_ = 0;
    vector<bool> *ElecInfo_EGMCutBasedIDMedium_ = 0;
    vector<bool> *ElecInfo_EGMCutBasedIDTight_ = 0;
    int MuonInfo_Size_ = 0;
    vector<int> *MuonInfo_Charge_ = 0;
    vector<float> *MuonInfo_MuonType_ = 0;
    vector<float> *MuonInfo_Pt_ = 0;
    vector<float> *MuonInfo_Eta_ = 0;
    vector<float> *MuonInfo_Phi_ = 0;
    vector<float> *MuonInfo_Energy_ = 0;
    vector<float> *MuonInfo_PFIsoDeltaBetaCorrR04_ = 0;
    vector<bool> *MuonInfo_CutBasedIdMedium_ = 0;
    vector<bool> *MuonInfo_CutBasedIdTight_ = 0;
    vector<bool> *MuonInfo_CutBasedIdTight_bestVtx_ = 0;
    int jets_size_ = 0;
    vector<float> *JetInfo_Pt_ = 0;
    vector<float> *JetInfo_Eta_ = 0;
    vector<float> *JetInfo_Phi_ = 0;
    vector<float> *JetInfo_Mass_ = 0;
    vector<float> *JetInfo_Energy_ = 0;
    vector<float> *JetInfo_pfDeepCSVJetTags_probb_ = 0;
    vector<float> *JetInfo_pfDeepCSVJetTags_probbb_ = 0;
    vector<float> *JetInfo_pfDeepCSVJetTags_probc_ = 0;
    vector<float> *JetInfo_pfDeepCSVJetTags_probudsg_ = 0;
    vector<float> *JetInfo_JECScale_ = 0;
    vector<float> *JetInfo_JERScale_ = 0;
    outTree->SetBranchAddress("ElecInfo.Size", &ElecInfo_Size_);
    outTree->SetBranchAddress("ElecInfo.Charge", &ElecInfo_Charge_);
    outTree->SetBranchAddress("ElecInfo.Pt", &ElecInfo_Pt_);
    outTree->SetBranchAddress("ElecInfo.Eta", &ElecInfo_Eta_);
    outTree->SetBranchAddress("ElecInfo.Phi", &ElecInfo_Phi_);
    outTree->SetBranchAddress("ElecInfo.Energy", &ElecInfo_Energy_);
    outTree->SetBranchAddress("ElecInfo.EGMCutBasedIDMedium", &ElecInfo_EGMCutBasedIDMedium_);
    outTree->SetBranchAddress("ElecInfo.EGMCutBasedIDTight", &ElecInfo_EGMCutBasedIDTight_);
    outTree->SetBranchAddress("MuonInfo.Size", &MuonInfo_Size_);
    outTree->SetBranchAddress("MuonInfo.Charge", &MuonInfo_Charge_);
    outTree->SetBranchAddress("MuonInfo.MuonType", &MuonInfo_MuonType_);
    outTree->SetBranchAddress("MuonInfo.Pt", &MuonInfo_Pt_);
    outTree->SetBranchAddress("MuonInfo.Eta", &MuonInfo_Eta_);
    outTree->SetBranchAddress("MuonInfo.Phi", &MuonInfo_Phi_);
    outTree->SetBranchAddress("MuonInfo.Energy", &MuonInfo_Energy_);
    outTree->SetBranchAddress("MuonInfo.PFIsoDeltaBetaCorrR04", &MuonInfo_PFIsoDeltaBetaCorrR04_);
    outTree->SetBranchAddress("MuonInfo.CutBasedIdMedium", &MuonInfo_CutBasedIdMedium_);
    outTree->SetBranchAddress("MuonInfo.CutBasedIdTight", &MuonInfo_CutBasedIdTight_);
    outTree->SetBranchAddress("MuonInfo.CutBasedIdTight_bestVtx", &MuonInfo_CutBasedIdTight_bestVtx_);
    outTree->SetBranchAddress("jets_size", &jets_size_);
    outTree->SetBranchAddress("JetInfo.Pt", &JetInfo_Pt_);
    outTree->SetBranchAddress("JetInfo.Eta", &JetInfo_Eta_);
    outTree->SetBranchAddress("JetInfo.Phi", &JetInfo_Phi_);
    outTree->SetBranchAddress("JetInfo.Mass", &JetInfo_Mass_);
    outTree->SetBranchAddress("JetInfo.Energy", &JetInfo_Energy_);
    outTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probb", &JetInfo_pfDeepCSVJetTags_probb_);
    outTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probbb", &JetInfo_pfDeepCSVJetTags_probbb_);
    outTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probc", &JetInfo_pfDeepCSVJetTags_probc_);
    outTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probudsg", &JetInfo_pfDeepCSVJetTags_probudsg_);
    outTree->SetBranchAddress("JetInfo.JECScale", &JetInfo_JECScale_);
    outTree->SetBranchAddress("JetInfo.JERScale", &JetInfo_JERScale_);

    // Loop over input TTree to do preselection
    cout << "Start processing events...\n";
    for (long long evt=0; evt<inTree->GetEntries(); ++evt)
    {
        inTree->GetEntry(evt);

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
        MuonInfo_MuonType_->clear();
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
        JetInfo_Mass_->clear();
        JetInfo_Energy_->clear();
        JetInfo_pfDeepCSVJetTags_probb_->clear();
        JetInfo_pfDeepCSVJetTags_probbb_->clear();
        JetInfo_pfDeepCSVJetTags_probc_->clear();
        JetInfo_pfDeepCSVJetTags_probudsg_->clear();
        JetInfo_JECScale_->clear();
        JetInfo_JERScale_->clear();

        // Create 4-momenta of leading photon and subleading photon (used when calculating deltaR with other objects)
        TLorentzVector leadPho, subleadPho;
        leadPho.SetPtEtaPhiE(DiPhoInfo_leadPt, DiPhoInfo_leadEta, DiPhoInfo_leadPhi, DiPhoInfo_leadE);
        subleadPho.SetPtEtaPhiE(DiPhoInfo_subleadPt, DiPhoInfo_subleadEta, DiPhoInfo_subleadPhi, DiPhoInfo_subleadE);

        // Create vectors of 4-momenta of selected leptons (used when calculating deltaR with jets)
        vector<TLorentzVector*> electrons;
        vector<TLorentzVector*> muons;

        /* BEGIN: Select electrons */
        for (int i=0; i<ElecInfo_Size; ++i)
        {
            // Selection of electrons
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
        /* END: Select electrons */

        /* BEGIN: Select muons */
        for (int i=0; i<MuonInfo_Size; ++i)
        {
            // Selection of muons
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
            MuonInfo_MuonType_->push_back( MuonInfo_MuonType->at(i) );
            MuonInfo_Pt_->push_back( MuonInfo_Pt->at(i) );
            MuonInfo_Eta_->push_back( MuonInfo_Eta->at(i) );
            MuonInfo_Phi_->push_back( MuonInfo_Phi->at(i) );
            MuonInfo_Energy_->push_back( MuonInfo_Energy->at(i) );
            MuonInfo_PFIsoDeltaBetaCorrR04_->push_back( MuonInfo_PFIsoDeltaBetaCorrR04->at(i) );
            MuonInfo_CutBasedIdMedium_->push_back( MuonInfo_CutBasedIdMedium->at(i) );
            MuonInfo_CutBasedIdTight_->push_back( MuonInfo_CutBasedIdTight->at(i) );
            MuonInfo_CutBasedIdTight_bestVtx_->push_back( MuonInfo_CutBasedIdTight_bestVtx->at(i) );
        }
        /* END: Select muons */

        /* BEGIN: Select jets */
        for (int i=0; i<jets_size; ++i)
        {
            // Selection of jets
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
            JetInfo_Mass_->push_back( JetInfo_Mass->at(i) );
            JetInfo_Energy_->push_back( JetInfo_Energy->at(i) );
            JetInfo_pfDeepCSVJetTags_probb_->push_back( JetInfo_pfDeepCSVJetTags_probb->at(i) );
            JetInfo_pfDeepCSVJetTags_probbb_->push_back( JetInfo_pfDeepCSVJetTags_probbb->at(i) );
            JetInfo_pfDeepCSVJetTags_probc_->push_back( JetInfo_pfDeepCSVJetTags_probc->at(i) );
            JetInfo_pfDeepCSVJetTags_probudsg_->push_back( JetInfo_pfDeepCSVJetTags_probudsg->at(i) );
            JetInfo_JECScale_->push_back( JetInfo_JECScale->at(i) );
            JetInfo_JERScale_->push_back( JetInfo_JERScale->at(i) );
        }
        /* END: Select jets */

        // Clean up
        for (int i=0; i<electrons.size(); ++i) delete electrons[i];
        for (int i=0; i<muons.size(); ++i) delete muons[i];

        // If the event passes the preselection, fill it to output TTree
        outTree->Fill();
        // Process the next event
    } // End of preselection loop over input TTree
    cout << "Completed processing all events.\n";

    // Write output TTree to output TFile
    fout->cd();
    outTree->Write();
    // Close output TFile
    fout->Close();

    cout << "Output is saved at: " << fout_name << endl;

    return 0;
}
