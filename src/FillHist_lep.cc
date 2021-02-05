#include "include/utility.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include <iostream>
#include <vector>
#include <map>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fin_name(argv[1]);
    TString fout_name(argv[2]);

    // Read input tree
    TFile *fin = new TFile(fin_name);
    TTree *T = (TTree*)fin->Get("T");

    // Create output histograms
    TFile *fout = new TFile(fout_name, "recreate");
    map<TString, TH1D*> hists;
    // Event info
    CreateHist(hists, "evt_Nvtx", "# of vertices", 10, 0, 80, "");
    CreateHist(hists, "evt_rho", "#rho", 10, 0, 60, "");
    // DiPhoton
    CreateHist(hists, "dipho_mass", "diphoton mass", 8, 100, 180, "GeV");
    CreateHist(hists, "dipho_pt", "diphoton pt", 10, 0, 260, "GeV");
    CreateHist(hists, "dipho_mva", "diphoton MVA", 10, -1, 1, "");
    CreateHist(hists, "dipho_idmva1", "#gamma IDMVA high", 10, -1, 1, "");
    CreateHist(hists, "dipho_idmva2", "#gamma IDMVA low", 10, -1, 1, "");
    // Leading photon
    CreateHist(hists, "pho1_pt", "lead #gamma pt", 10, 35, 245, "GeV");
    CreateHist(hists, "pho1_eta", "lead #gamma #eta", 10, -2.5, 2.5, "");
    CreateHist(hists, "pho1_phi", "lead #gamma #phi", 10, -3.5, 3.5, "");
    CreateHist(hists, "pho1_E", "lead #gamma energy", 10, 35, 585, "GeV");
    CreateHist(hists, "pho1_idmva", "lead #gamma IDMVA", 10, -1, 1, "");
    CreateHist(hists, "pho1_PixelSeed", "lead #gamma pixel seed", 2, 0, 2, "");
    // Subleading photon
    CreateHist(hists, "pho2_pt", "sublead #gamma pt", 10, 25, 125, "GeV");
    CreateHist(hists, "pho2_eta", "sublead #gamma #eta", 10, -2.5, 2.5, "");
    CreateHist(hists, "pho2_phi", "sublead #gamma #phi", 10, -3.5, 3.5, "");
    CreateHist(hists, "pho2_E", "sublead #gamma energy", 10, 25, 305, "GeV");
    CreateHist(hists, "pho2_idmva", "sublead #gamma IDMVA", 10, -1, 1, "");
    CreateHist(hists, "pho2_PixelSeed", "sublead #gamma pixel seed", 2, 0, 2, "");
    // Jet
    CreateHist(hists, "jet_N", "# of jets", 13, -0.5, 12.5, "");
    CreateHist(hists, "jet_pt", "jet pt", 10, 25, 205, "GeV");
    CreateHist(hists, "jet_eta", "jet #eta", 10, -2.4, 2.4, "");
    CreateHist(hists, "jet_phi", "jet #phi", 10, -3.5, 3.5, "");
    CreateHist(hists, "jet_E", "jet energy", 10, 25, 505, "GeV");
    CreateHist(hists, "jet_btag", "jet b-tag", 10, 0, 1, "");
    CreateHist(hists, "jet1_pt", "jet1 pt", 10, 25, 205, "GeV");
    CreateHist(hists, "jet1_eta", "jet1 #eta", 10, -2.4, 2.4, "");
    CreateHist(hists, "jet1_phi", "jet1 #phi", 10, -3.5, 3.5, "");
    CreateHist(hists, "jet1_E", "jet1 energy", 10, 25, 505, "GeV");
    CreateHist(hists, "jet1_btag", "jet1 b-tag", 10, 0, 1, "");
    CreateHist(hists, "jet2_pt", "jet2 pt", 10, 25, 205, "GeV");
    CreateHist(hists, "jet2_eta", "jet2 #eta", 10, -2.4, 2.4, "");
    CreateHist(hists, "jet2_phi", "jet2 #phi", 10, -3.5, 3.5, "");
    CreateHist(hists, "jet2_E", "jet2 energy", 10, 25, 505, "GeV");
    CreateHist(hists, "jet2_btag", "jet2 b-tag", 10, 0, 1, "");
    CreateHist(hists, "jet_btag1", "jet max b-tag", 10, 0, 1, "");
    CreateHist(hists, "jet_btag2", "jet 2nd max b-tag", 10, 0, 1, "");
    // MET
    CreateHist(hists, "met_pt", "MET pt", 10, 0, 170, "GeV");
    CreateHist(hists, "met_phi", "MET #phi", 10, -3.5, 3.5, "");
    // Electron
    CreateHist(hists, "elec_N", "# of electrons", 7, -0.5, 6.5, "");
    CreateHist(hists, "elec_Q", "electron charge", 5, -2.5, 2.5, "");
    CreateHist(hists, "elec_pt", "electron pt", 10, 5, 125, "GeV");
    CreateHist(hists, "elec_eta", "electron #eta", 10, -2.4, 2.4, "");
    CreateHist(hists, "elec_phi", "electron #phi", 10, -3.5, 3.5, "");
    CreateHist(hists, "elec_E", "electron energy", 10, 5, 325, "GeV");
    // Muon
    CreateHist(hists, "muon_N", "# of #mu", 7, -0.5, 6.5, "");
    CreateHist(hists, "muon_Q", "muon charge", 5, -2.5, 2.5, "");
    CreateHist(hists, "muon_pt", "muon pt", 10, 5, 125, "GeV");
    CreateHist(hists, "muon_eta", "muon #eta", 10, -2.4, 2.4, "");
    CreateHist(hists, "muon_phi", "muon #phi", 10, -3.5, 3.5, "");
    CreateHist(hists, "muon_E", "muon energy", 10, 5, 325, "GeV");
    CreateHist(hists, "muon_iso", "muon isolation", 10, 0, 0.25, "");
    // Lepton
    CreateHist(hists, "lep_N", "# of leptons", 7, -0.5, 6.5, "");
    CreateHist(hists, "lep_Q", "lepton charge", 5, -2.5, 2.5, "");
    CreateHist(hists, "lep_pt", "lepton pt", 10, 5, 125, "GeV");
    CreateHist(hists, "lep_eta", "lepton #eta", 10, -2.4, 2.4, "");
    CreateHist(hists, "lep_phi", "lepton #phi", 10, -3.5, 3.5, "");
    CreateHist(hists, "lep_E", "lepton energy", 10, 5, 325, "GeV");
    // Leading lepton
    CreateHist(hists, "lep1_Q", "leading lepton charge", 5, -2.5, 2.5, "");
    CreateHist(hists, "lep1_pt", "leading lepton pt", 10, 5, 125, "GeV");
    CreateHist(hists, "lep1_eta", "leading lepton #eta", 10, -2.4, 2.4, "");
    CreateHist(hists, "lep1_phi", "leading lepton #phi", 10, -3.5, 3.5, "");
    CreateHist(hists, "lep1_E", "leading lepton energy", 10, 5, 325, "GeV");

    // Fill histograms
    TString expr_wei = "evtwei_central";
    // Event info
    FillHist(T, "EvtInfo.NVtx", "evt_Nvtx", expr_wei);
    FillHist(T, "EvtInfo.Rho", "evt_rho", expr_wei);
    // Diphoton
    FillHist(T, "DiPhoInfo.mass", "dipho_mass", expr_wei);
    FillHist(T, "DiPhoInfo.pt", "dipho_pt", expr_wei);
    FillHist(T, "DiPhoInfo.diphotonMVA", "dipho_mva", expr_wei);
    FillHist(T, "max(DiPhoInfo.leadIDMVA,DiPhoInfo.subleadIDMVA)", "dipho_idmva1", expr_wei);
    FillHist(T, "min(DiPhoInfo.leadIDMVA,DiPhoInfo.subleadIDMVA)", "dipho_idmva2", expr_wei);
    // Leading photon
    FillHist(T, "DiPhoInfo.leadPt", "pho1_pt", expr_wei);
    FillHist(T, "DiPhoInfo.leadEta", "pho1_eta", expr_wei);
    FillHist(T, "DiPhoInfo.leadPhi", "pho1_phi", expr_wei);
    FillHist(T, "DiPhoInfo.leadE", "pho1_E", expr_wei);
    FillHist(T, "DiPhoInfo.leadIDMVA", "pho1_idmva", expr_wei);
    FillHist(T, "DiPhoInfo.leadhasPixelSeed", "pho1_PixelSeed", expr_wei);
    // Subleading photon
    FillHist(T, "DiPhoInfo.subleadPt", "pho2_pt", expr_wei);
    FillHist(T, "DiPhoInfo.subleadEta", "pho2_eta", expr_wei);
    FillHist(T, "DiPhoInfo.subleadPhi", "pho2_phi", expr_wei);
    FillHist(T, "DiPhoInfo.subleadE", "pho2_E", expr_wei);
    FillHist(T, "DiPhoInfo.subleadIDMVA", "pho2_idmva", expr_wei);
    FillHist(T, "DiPhoInfo.subleadhasPixelSeed", "pho2_PixelSeed", expr_wei);
    // Jet
    FillHist(T, "jets_size", "jet_N", expr_wei);
    FillHist(T, "JetInfo.Pt", "jet_pt", expr_wei);
    FillHist(T, "JetInfo.Eta", "jet_eta", expr_wei);
    FillHist(T, "JetInfo.Phi", "jet_phi", expr_wei);
    FillHist(T, "JetInfo.Energy", "jet_E", expr_wei);
    FillHist(T, "JetInfo.pfDeepCSVJetTags_probb+JetInfo.pfDeepCSVJetTags_probbb", "jet_btag", expr_wei);
    FillHist(T, "JetInfo.Pt[0]", "jet1_pt", expr_wei);
    FillHist(T, "JetInfo.Eta[0]", "jet1_eta", expr_wei);
    FillHist(T, "JetInfo.Phi[0]", "jet1_phi", expr_wei);
    FillHist(T, "JetInfo.Energy[0]", "jet1_E", expr_wei);
    FillHist(T, "JetInfo.pfDeepCSVJetTags_probb[0]+JetInfo.pfDeepCSVJetTags_probbb[0]", "jet1_btag", expr_wei);
    FillHist(T, "JetInfo.Pt[1]", "jet2_pt", expr_wei, "jets_size>=2");
    FillHist(T, "JetInfo.Eta[1]", "jet2_eta", expr_wei, "jets_size>=2");
    FillHist(T, "JetInfo.Phi[1]", "jet2_phi", expr_wei, "jets_size>=2");
    FillHist(T, "JetInfo.Energy[1]", "jet2_E", expr_wei, "jets_size>=2");
    FillHist(T, "JetInfo.pfDeepCSVJetTags_probb[1]+JetInfo.pfDeepCSVJetTags_probbb[1]", "jet2_btag", expr_wei, "jets_size>=2");
    // MET
    FillHist(T, "MetInfo.Pt", "met_pt", expr_wei);
    FillHist(T, "MetInfo.Phi", "met_phi", expr_wei);
    // Electron
    FillHist(T, "ElecInfo.Size", "elec_N", expr_wei);
    FillHist(T, "ElecInfo.Charge", "elec_Q", expr_wei);
    FillHist(T, "ElecInfo.Pt", "elec_pt", expr_wei);
    FillHist(T, "ElecInfo.Eta", "elec_eta", expr_wei);
    FillHist(T, "ElecInfo.Phi", "elec_phi", expr_wei);
    FillHist(T, "ElecInfo.Energy", "elec_E", expr_wei);
    // Muon
    FillHist(T, "MuonInfo.Size", "muon_N", expr_wei);
    FillHist(T, "MuonInfo.Charge", "muon_Q", expr_wei);
    FillHist(T, "MuonInfo.Pt", "muon_pt", expr_wei);
    FillHist(T, "MuonInfo.Eta", "muon_eta", expr_wei);
    FillHist(T, "MuonInfo.Phi", "muon_phi", expr_wei);
    FillHist(T, "MuonInfo.Energy", "muon_E", expr_wei);
    FillHist(T, "MuonInfo.PFIsoDeltaBetaCorrR04", "muon_iso", expr_wei);
    // Lepton
    FillHist(T, "ElecInfo.Size+MuonInfo.Size", "lep_N", expr_wei);
    hists["lep_Q"]->Add(hists["elec_Q"]);  hists["lep_Q"]->Add(hists["muon_Q"]);
    hists["lep_pt"]->Add(hists["elec_pt"]);  hists["lep_pt"]->Add(hists["muon_pt"]);
    hists["lep_eta"]->Add(hists["elec_eta"]);  hists["lep_eta"]->Add(hists["muon_eta"]);
    hists["lep_phi"]->Add(hists["elec_phi"]);  hists["lep_phi"]->Add(hists["muon_phi"]);
    hists["lep_E"]->Add(hists["elec_E"]);  hists["lep_E"]->Add(hists["muon_E"]);

    // Fill histograms with loop
    // Set tree variables
    float evtwei_central = 0;
    // Electron
    int ElecInfo_Size = 0;
    vector<int> *ElecInfo_Charge = 0;
    vector<float> *ElecInfo_Pt = 0;
    vector<float> *ElecInfo_Eta = 0;
    vector<float> *ElecInfo_Phi = 0;
    vector<float> *ElecInfo_Energy = 0;
    // Muon
    int MuonInfo_Size = 0;
    vector<int> *MuonInfo_Charge = 0;
    vector<float> *MuonInfo_Pt = 0;
    vector<float> *MuonInfo_Eta = 0;
    vector<float> *MuonInfo_Phi = 0;
    vector<float> *MuonInfo_Energy = 0;
    // Jet
    int jets_size = 0;
    vector<float> *Jet_probb = 0;
    vector<float> *Jet_probbb = 0;
    // Set tree branches
    T->SetBranchAddress("evtwei_central", &evtwei_central);
    // Electron
    T->SetBranchAddress("ElecInfo.Size", &ElecInfo_Size);
    T->SetBranchAddress("ElecInfo.Charge", &ElecInfo_Charge);
    T->SetBranchAddress("ElecInfo.Pt", &ElecInfo_Pt);
    T->SetBranchAddress("ElecInfo.Eta", &ElecInfo_Eta);
    T->SetBranchAddress("ElecInfo.Phi", &ElecInfo_Phi);
    T->SetBranchAddress("ElecInfo.Energy", &ElecInfo_Energy);
    // Muon
    T->SetBranchAddress("MuonInfo.Size", &MuonInfo_Size);
    T->SetBranchAddress("MuonInfo.Charge", &MuonInfo_Charge);
    T->SetBranchAddress("MuonInfo.Pt", &MuonInfo_Pt);
    T->SetBranchAddress("MuonInfo.Eta", &MuonInfo_Eta);
    T->SetBranchAddress("MuonInfo.Phi", &MuonInfo_Phi);
    T->SetBranchAddress("MuonInfo.Energy", &MuonInfo_Energy);
    // Jet
    T->SetBranchAddress("jets_size", &jets_size);
    T->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probb", &Jet_probb);
    T->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probbb", &Jet_probbb);

    // Start event loop
    for (int evt=0; evt<T->GetEntries(); ++evt)
    {
        T->GetEntry(evt);

        if (MuonInfo_Size == 0) {
            hists["lep1_Q"]->Fill(ElecInfo_Charge->at(0), evtwei_central);
            hists["lep1_pt"]->Fill(ElecInfo_Pt->at(0), evtwei_central);
            hists["lep1_eta"]->Fill(ElecInfo_Eta->at(0), evtwei_central);
            hists["lep1_phi"]->Fill(ElecInfo_Phi->at(0), evtwei_central);
            hists["lep1_E"]->Fill(ElecInfo_Energy->at(0), evtwei_central);
        }
        else if (ElecInfo_Size == 0) {
            hists["lep1_Q"]->Fill(MuonInfo_Charge->at(0), evtwei_central);
            hists["lep1_pt"]->Fill(MuonInfo_Pt->at(0), evtwei_central);
            hists["lep1_eta"]->Fill(MuonInfo_Eta->at(0), evtwei_central);
            hists["lep1_phi"]->Fill(MuonInfo_Phi->at(0), evtwei_central);
            hists["lep1_E"]->Fill(MuonInfo_Energy->at(0), evtwei_central);
        }
        else {
            if (ElecInfo_Pt->at(0) >= MuonInfo_Pt->at(0)) {
                hists["lep1_Q"]->Fill(ElecInfo_Charge->at(0), evtwei_central);
                hists["lep1_pt"]->Fill(ElecInfo_Pt->at(0), evtwei_central);
                hists["lep1_eta"]->Fill(ElecInfo_Eta->at(0), evtwei_central);
                hists["lep1_phi"]->Fill(ElecInfo_Phi->at(0), evtwei_central);
                hists["lep1_E"]->Fill(ElecInfo_Energy->at(0), evtwei_central);
            }
            else {
                hists["lep1_Q"]->Fill(MuonInfo_Charge->at(0), evtwei_central);
                hists["lep1_pt"]->Fill(MuonInfo_Pt->at(0), evtwei_central);
                hists["lep1_eta"]->Fill(MuonInfo_Eta->at(0), evtwei_central);
                hists["lep1_phi"]->Fill(MuonInfo_Phi->at(0), evtwei_central);
                hists["lep1_E"]->Fill(MuonInfo_Energy->at(0), evtwei_central);
            }
        }

        // Create vector of jet b-tag scores
        vector<float> Jet_btag(jets_size);
        for (int i=0; i<Jet_btag.size(); ++i)  Jet_btag[i] = Jet_probb->at(i)+Jet_probbb->at(i);

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

        // Fill histograms
        hists["jet_btag1"]->Fill(Jet_btag[0], evtwei_central);
        if (jets_size >= 2) hists["jet_btag2"]->Fill(Jet_btag[1], evtwei_central);
    } // End of event loop

    // Save histograms
    for (map<TString,TH1D*>::iterator it=hists.begin(); it!=hists.end(); ++it)  it->second->Write();
    fout->Close();
    fin->Close();

    return 0;
}
