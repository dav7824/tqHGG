// Fill and save histograms for given sample.
//
// Usage:
//   ./FilHistV2_xxx <fin_list> <tree_list> <fout> <SF_flags>

#include "include/utility.h"
#include "include/SFhelper.h"

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"

#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <bitset>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    char *fin_list = argv[1];
    char *tree_list = argv[2];
    TString fout_name = argv[3];
    bitset<SFhelper::Nsf> SF_flags(argv[4]);

    vector<TString> fins;
    vector<TString> trees_tmp;
    vector<vector<TString>> trees;
    ParseCStringList(fin_list, fins);
    ParseCStringList(tree_list, trees_tmp);
    trees.resize(fins.size());
    for (int i=0; i<fins.size(); ++i)
        ParseCStringList(trees_tmp[i].Data(), trees[i], '+');

    // Read input trees
    TFile *fin = new TFile(fins[0]);
    TTree *Tin = (TTree*)fin->Get(trees[0][0]);
    for (int i=1; i<trees[0].size(); ++i)
        Tin->AddFriend(trees[0][i].Data());
    for (int i=1; i<fins.size(); ++i)
        for (int j=0; j<trees[i].size(); ++j)
            Tin->AddFriend(trees[i][j].Data(), fins[i].Data());

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
    CreateHist(hists, "jet_M", "jet mass", 10, 0, 40, "GeV");
    CreateHist(hists, "jet_E", "jet energy", 10, 25, 505, "GeV");
    CreateHist(hists, "jet_btag", "jet b-tag", 10, 0, 1, "");
    CreateHist(hists, "jet1_pt", "jet1 pt", 10, 25, 205, "GeV");
    CreateHist(hists, "jet1_eta", "jet1 #eta", 10, -2.4, 2.4, "");
    CreateHist(hists, "jet1_phi", "jet1 #phi", 10, -3.5, 3.5, "");
    CreateHist(hists, "jet1_M", "jet1 mass", 10, 0, 40, "GeV");
    CreateHist(hists, "jet1_E", "jet1 energy", 10, 25, 505, "GeV");
    CreateHist(hists, "jet1_btag", "jet1 b-tag", 10, 0, 1, "");
    CreateHist(hists, "jet2_pt", "jet2 pt", 10, 25, 205, "GeV");
    CreateHist(hists, "jet2_eta", "jet2 #eta", 10, -2.4, 2.4, "");
    CreateHist(hists, "jet2_phi", "jet2 #phi", 10, -3.5, 3.5, "");
    CreateHist(hists, "jet2_M", "jet2 mass", 10, 0, 40, "GeV");
    CreateHist(hists, "jet2_E", "jet2 energy", 10, 25, 505, "GeV");
    CreateHist(hists, "jet2_btag", "jet2 b-tag", 10, 0, 1, "");
    CreateHist(hists, "jet_btag1", "jet max b-tag", 10, 0, 1, "");
    CreateHist(hists, "jet_btag2", "jet 2nd max b-tag", 10, 0, 1, "");
    // MET
    CreateHist(hists, "met_pt", "MET pt", 10, 0, 170, "GeV");
    CreateHist(hists, "met_phi", "MET #phi", 10, -3.5, 3.5, "");
    CreateHist(hists, "met_px", "MET px", 10, -180, 180, "GeV");
    CreateHist(hists, "met_py", "MET py", 10, -180, 180, "GeV");
    CreateHist(hists, "met_sumET", "MET E_{T}^{sum}", 10, 0, 4000, "GeV");
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

    // Fill histograms without loop
    SFhelper SFsource(SF_flags);
    TString weight = SFsource.GetWeightExpression();
    // Event info
    FillHist(Tin, "EvtInfo.NVtx", "evt_Nvtx", weight);
    FillHist(Tin, "EvtInfo.Rho", "evt_rho", weight);
    // Diphoton
    FillHist(Tin, "DiPhoInfo.mass", "dipho_mass", weight);
    FillHist(Tin, "DiPhoInfo.pt", "dipho_pt", weight);
    FillHist(Tin, "DiPhoInfo.diphotonMVA", "dipho_mva", weight);
    FillHist(Tin, "max(DiPhoInfo.leadIDMVA,DiPhoInfo.subleadIDMVA)", "dipho_idmva1", weight);
    FillHist(Tin, "min(DiPhoInfo.leadIDMVA,DiPhoInfo.subleadIDMVA)", "dipho_idmva2", weight);
    // Leading photon
    FillHist(Tin, "DiPhoInfo.leadPt", "pho1_pt", weight);
    FillHist(Tin, "DiPhoInfo.leadEta", "pho1_eta", weight);
    FillHist(Tin, "DiPhoInfo.leadPhi", "pho1_phi", weight);
    FillHist(Tin, "DiPhoInfo.leadE", "pho1_E", weight);
    FillHist(Tin, "DiPhoInfo.leadIDMVA", "pho1_idmva", weight);
    FillHist(Tin, "DiPhoInfo.leadhasPixelSeed", "pho1_PixelSeed", weight);
    // Subleading photon
    FillHist(Tin, "DiPhoInfo.subleadPt", "pho2_pt", weight);
    FillHist(Tin, "DiPhoInfo.subleadEta", "pho2_eta", weight);
    FillHist(Tin, "DiPhoInfo.subleadPhi", "pho2_phi", weight);
    FillHist(Tin, "DiPhoInfo.subleadE", "pho2_E", weight);
    FillHist(Tin, "DiPhoInfo.subleadIDMVA", "pho2_idmva", weight);
    FillHist(Tin, "DiPhoInfo.subleadhasPixelSeed", "pho2_PixelSeed", weight);
    // Jet
    FillHist(Tin, "jets_size", "jet_N", weight);
    FillHist(Tin, "JetInfo.Pt", "jet_pt", weight);
    FillHist(Tin, "JetInfo.Eta", "jet_eta", weight);
    FillHist(Tin, "JetInfo.Phi", "jet_phi", weight);
    FillHist(Tin, "JetInfo.Mass", "jet_M", weight);
    FillHist(Tin, "JetInfo.Energy", "jet_E", weight);
    FillHist(Tin, "JetInfo.pfDeepCSVJetTags_probb+JetInfo.pfDeepCSVJetTags_probbb", "jet_btag", weight);
    FillHist(Tin, "JetInfo.Pt[0]", "jet1_pt", weight);
    FillHist(Tin, "JetInfo.Eta[0]", "jet1_eta", weight);
    FillHist(Tin, "JetInfo.Phi[0]", "jet1_phi", weight);
    FillHist(Tin, "JetInfo.Mass[0]", "jet1_M", weight);
    FillHist(Tin, "JetInfo.Energy[0]", "jet1_E", weight);
    FillHist(Tin, "JetInfo.pfDeepCSVJetTags_probb[0]+JetInfo.pfDeepCSVJetTags_probbb[0]", "jet1_btag", weight);
    FillHist(Tin, "JetInfo.Pt[1]", "jet2_pt", weight, "jets_size>=2");
    FillHist(Tin, "JetInfo.Eta[1]", "jet2_eta", weight, "jets_size>=2");
    FillHist(Tin, "JetInfo.Phi[1]", "jet2_phi", weight, "jets_size>=2");
    FillHist(Tin, "JetInfo.Mass[1]", "jet2_M", weight, "jets_size>=2");
    FillHist(Tin, "JetInfo.Energy[1]", "jet2_E", weight, "jets_size>=2");
    FillHist(Tin, "JetInfo.pfDeepCSVJetTags_probb[1]+JetInfo.pfDeepCSVJetTags_probbb[1]", "jet2_btag", weight, "jets_size>=2");
    // MET
    FillHist(Tin, "MetInfo.Pt", "met_pt", weight);
    FillHist(Tin, "MetInfo.Phi", "met_phi", weight);
    FillHist(Tin, "MetInfo.Px", "met_px", weight);
    FillHist(Tin, "MetInfo.Py", "met_py", weight);
    FillHist(Tin, "MetInfo.SumET", "met_sumET", weight);
    // Electron
    FillHist(Tin, "ElecInfo.Size", "elec_N", weight);
    FillHist(Tin, "ElecInfo.Charge", "elec_Q", weight);
    FillHist(Tin, "ElecInfo.Pt", "elec_pt", weight);
    FillHist(Tin, "ElecInfo.Eta", "elec_eta", weight);
    FillHist(Tin, "ElecInfo.Phi", "elec_phi", weight);
    FillHist(Tin, "ElecInfo.Energy", "elec_E", weight);
    // Muon
    FillHist(Tin, "MuonInfo.Size", "muon_N", weight);
    FillHist(Tin, "MuonInfo.Charge", "muon_Q", weight);
    FillHist(Tin, "MuonInfo.Pt", "muon_pt", weight);
    FillHist(Tin, "MuonInfo.Eta", "muon_eta", weight);
    FillHist(Tin, "MuonInfo.Phi", "muon_phi", weight);
    FillHist(Tin, "MuonInfo.Energy", "muon_E", weight);
    FillHist(Tin, "MuonInfo.PFIsoDeltaBetaCorrR04", "muon_iso", weight);
    // Lepton
    FillHist(Tin, "ElecInfo.Size+MuonInfo.Size", "lep_N", weight);
    hists["lep_Q"]->Add(hists["elec_Q"]);  hists["lep_Q"]->Add(hists["muon_Q"]);
    hists["lep_pt"]->Add(hists["elec_pt"]);  hists["lep_pt"]->Add(hists["muon_pt"]);
    hists["lep_eta"]->Add(hists["elec_eta"]);  hists["lep_eta"]->Add(hists["muon_eta"]);
    hists["lep_phi"]->Add(hists["elec_phi"]);  hists["lep_phi"]->Add(hists["muon_phi"]);
    hists["lep_E"]->Add(hists["elec_E"]);  hists["lep_E"]->Add(hists["muon_E"]);

    // Fill histograms with loop
    SFsource.SetTreeBranches(Tin);
    // Set tree variables
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
    // Electron
    Tin->SetBranchAddress("ElecInfo.Size", &ElecInfo_Size);
    Tin->SetBranchAddress("ElecInfo.Charge", &ElecInfo_Charge);
    Tin->SetBranchAddress("ElecInfo.Pt", &ElecInfo_Pt);
    Tin->SetBranchAddress("ElecInfo.Eta", &ElecInfo_Eta);
    Tin->SetBranchAddress("ElecInfo.Phi", &ElecInfo_Phi);
    Tin->SetBranchAddress("ElecInfo.Energy", &ElecInfo_Energy);
    // Muon
    Tin->SetBranchAddress("MuonInfo.Size", &MuonInfo_Size);
    Tin->SetBranchAddress("MuonInfo.Charge", &MuonInfo_Charge);
    Tin->SetBranchAddress("MuonInfo.Pt", &MuonInfo_Pt);
    Tin->SetBranchAddress("MuonInfo.Eta", &MuonInfo_Eta);
    Tin->SetBranchAddress("MuonInfo.Phi", &MuonInfo_Phi);
    Tin->SetBranchAddress("MuonInfo.Energy", &MuonInfo_Energy);
    // Jet
    Tin->SetBranchAddress("jets_size", &jets_size);
    Tin->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probb", &Jet_probb);
    Tin->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probbb", &Jet_probbb);

    // Start event loop
    for (int evt=0; evt<Tin->GetEntries(); ++evt)
    {
        Tin->GetEntry(evt);
        float evt_weight = SFsource.GetWeight();

        if (MuonInfo_Size == 0) {
            hists["lep1_Q"]->Fill(ElecInfo_Charge->at(0), evt_weight);
            hists["lep1_pt"]->Fill(ElecInfo_Pt->at(0), evt_weight);
            hists["lep1_eta"]->Fill(ElecInfo_Eta->at(0), evt_weight);
            hists["lep1_phi"]->Fill(ElecInfo_Phi->at(0), evt_weight);
            hists["lep1_E"]->Fill(ElecInfo_Energy->at(0), evt_weight);
        }
        else if (ElecInfo_Size == 0) {
            hists["lep1_Q"]->Fill(MuonInfo_Charge->at(0), evt_weight);
            hists["lep1_pt"]->Fill(MuonInfo_Pt->at(0), evt_weight);
            hists["lep1_eta"]->Fill(MuonInfo_Eta->at(0), evt_weight);
            hists["lep1_phi"]->Fill(MuonInfo_Phi->at(0), evt_weight);
            hists["lep1_E"]->Fill(MuonInfo_Energy->at(0), evt_weight);
        }
        else {
            if (ElecInfo_Pt->at(0) >= MuonInfo_Pt->at(0)) {
                hists["lep1_Q"]->Fill(ElecInfo_Charge->at(0), evt_weight);
                hists["lep1_pt"]->Fill(ElecInfo_Pt->at(0), evt_weight);
                hists["lep1_eta"]->Fill(ElecInfo_Eta->at(0), evt_weight);
                hists["lep1_phi"]->Fill(ElecInfo_Phi->at(0), evt_weight);
                hists["lep1_E"]->Fill(ElecInfo_Energy->at(0), evt_weight);
            }
            else {
                hists["lep1_Q"]->Fill(MuonInfo_Charge->at(0), evt_weight);
                hists["lep1_pt"]->Fill(MuonInfo_Pt->at(0), evt_weight);
                hists["lep1_eta"]->Fill(MuonInfo_Eta->at(0), evt_weight);
                hists["lep1_phi"]->Fill(MuonInfo_Phi->at(0), evt_weight);
                hists["lep1_E"]->Fill(MuonInfo_Energy->at(0), evt_weight);
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
        hists["jet_btag1"]->Fill(Jet_btag[0], evt_weight);
        if (jets_size >= 2) hists["jet_btag2"]->Fill(Jet_btag[1], evt_weight);
    } // End of event loop

    for (map<TString,TH1D*>::iterator it=hists.begin(); it!=hists.end(); ++it)  it->second->Write();
    fout->Close();
    fin->Close();

    return 0;
}
