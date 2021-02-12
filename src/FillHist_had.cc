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
    CreateHist(hists, "evt_Nvtx", "# of vertices", 20, 0, 80, "");
    CreateHist(hists, "evt_rho", "#rho", 20, 0, 60, "");
    // DiPhoton
    CreateHist(hists, "dipho_mass", "diphoton mass", 16, 100, 180, "GeV");
    CreateHist(hists, "dipho_pt", "diphoton pt", 20, 0, 360, "GeV");
    CreateHist(hists, "dipho_mva", "diphoton MVA", 20, -1, 1, "");
    CreateHist(hists, "dipho_idmva1", "#gamma IDMVA high", 20, -0.7, 1, "");
    CreateHist(hists, "dipho_idmva2", "#gamma IDMVA low", 20, -0.7, 1, "");
    // Leading photon
    CreateHist(hists, "pho1_pt", "lead #gamma pt", 20, 35, 275, "GeV");
    CreateHist(hists, "pho1_eta", "lead #gamma #eta", 20, -2.5, 2.5, "");
    CreateHist(hists, "pho1_phi", "lead #gamma #phi", 20, -3.5, 3.5, "");
    CreateHist(hists, "pho1_E", "lead #gamma energy", 20, 35, 735, "GeV");
    CreateHist(hists, "pho1_idmva", "lead #gamma IDMVA", 20, -0.7, 1, "");
    CreateHist(hists, "pho1_PixelSeed", "lead #gamma pixel seed", 2, 0, 2, "");
    // Subleading photon
    CreateHist(hists, "pho2_pt", "sublead #gamma pt", 20, 25, 145, "GeV");
    CreateHist(hists, "pho2_eta", "sublead #gamma #eta", 20, -2.5, 2.5, "");
    CreateHist(hists, "pho2_phi", "sublead #gamma #phi", 20, -3.5, 3.5, "");
    CreateHist(hists, "pho2_E", "sublead #gamma energy", 20, 25, 325, "GeV");
    CreateHist(hists, "pho2_idmva", "sublead #gamma IDMVA", 20, -0.7, 1, "");
    CreateHist(hists, "pho2_PixelSeed", "sublead #gamma pixel seed", 2, 0, 2, "");
    // Jet
    CreateHist(hists, "jet_N", "# of jets", 13, -0.5, 12.5, "");
    CreateHist(hists, "jet_pt", "jet pt", 20, 25, 225, "GeV");
    CreateHist(hists, "jet_eta", "jet #eta", 20, -2.4, 2.4, "");
    CreateHist(hists, "jet_phi", "jet #phi", 20, -3.5, 3.5, "");
    CreateHist(hists, "jet_E", "jet energy", 20, 25, 505, "GeV");
    CreateHist(hists, "jet_btag", "jet b-tag", 20, 0, 1, "");
    CreateHist(hists, "jet1_pt", "jet1 pt", 20, 25, 225, "GeV");
    CreateHist(hists, "jet1_eta", "jet1 #eta", 20, -2.4, 2.4, "");
    CreateHist(hists, "jet1_phi", "jet1 #phi", 20, -3.5, 3.5, "");
    CreateHist(hists, "jet1_E", "jet1 energy", 20, 25, 505, "GeV");
    CreateHist(hists, "jet1_btag", "jet1 b-tag", 20, 0, 1, "");
    CreateHist(hists, "jet2_pt", "jet2 pt", 20, 25, 225, "GeV");
    CreateHist(hists, "jet2_eta", "jet2 #eta", 20, -2.4, 2.4, "");
    CreateHist(hists, "jet2_phi", "jet2 #phi", 20, -3.5, 3.5, "");
    CreateHist(hists, "jet2_E", "jet2 energy", 20, 25, 505, "GeV");
    CreateHist(hists, "jet2_btag", "jet2 b-tag", 20, 0, 1, "");
    CreateHist(hists, "jet3_pt", "jet3 pt", 20, 25, 225, "GeV");
    CreateHist(hists, "jet3_eta", "jet3 #eta", 20, -2.4, 2.4, "");
    CreateHist(hists, "jet3_phi", "jet3 #phi", 20, -3.5, 3.5, "");
    CreateHist(hists, "jet3_E", "jet3 energy", 20, 25, 505, "GeV");
    CreateHist(hists, "jet3_btag", "jet3 b-tag", 20, 0, 1, "");
    CreateHist(hists, "jet4_pt", "jet4 pt", 20, 25, 225, "GeV");
    CreateHist(hists, "jet4_eta", "jet4 #eta", 20, -2.4, 2.4, "");
    CreateHist(hists, "jet4_phi", "jet4 #phi", 20, -3.5, 3.5, "");
    CreateHist(hists, "jet4_E", "jet4 energy", 20, 25, 505, "GeV");
    CreateHist(hists, "jet4_btag", "jet4 b-tag", 20, 0, 1, "");
    CreateHist(hists, "jet_btag1", "jet max b-tag", 20, 0, 1, "");
    CreateHist(hists, "jet_btag2", "jet 2nd max b-tag", 20, 0, 1, "");
    // MET
    CreateHist(hists, "met_pt", "MET pt", 20, 0, 140, "GeV");
    CreateHist(hists, "met_phi", "MET #phi", 20, -3.5, 3.5, "");

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
    FillHist(T, "JetInfo.Pt[1]", "jet2_pt", expr_wei);
    FillHist(T, "JetInfo.Eta[1]", "jet2_eta", expr_wei);
    FillHist(T, "JetInfo.Phi[1]", "jet2_phi", expr_wei);
    FillHist(T, "JetInfo.Energy[1]", "jet2_E", expr_wei);
    FillHist(T, "JetInfo.pfDeepCSVJetTags_probb[1]+JetInfo.pfDeepCSVJetTags_probbb[1]", "jet2_btag", expr_wei);
    FillHist(T, "JetInfo.Pt[2]", "jet3_pt", expr_wei);
    FillHist(T, "JetInfo.Eta[2]", "jet3_eta", expr_wei);
    FillHist(T, "JetInfo.Phi[2]", "jet3_phi", expr_wei);
    FillHist(T, "JetInfo.Energy[2]", "jet3_E", expr_wei);
    FillHist(T, "JetInfo.pfDeepCSVJetTags_probb[2]+JetInfo.pfDeepCSVJetTags_probbb[2]", "jet3_btag", expr_wei);
    FillHist(T, "JetInfo.Pt[3]", "jet4_pt", expr_wei, "jets_size>=4");
    FillHist(T, "JetInfo.Eta[3]", "jet4_eta", expr_wei, "jets_size>=4");
    FillHist(T, "JetInfo.Phi[3]", "jet4_phi", expr_wei, "jets_size>=4");
    FillHist(T, "JetInfo.Energy[3]", "jet4_E", expr_wei, "jets_size>=4");
    FillHist(T, "JetInfo.pfDeepCSVJetTags_probb[3]+JetInfo.pfDeepCSVJetTags_probbb[3]", "jet4_btag", expr_wei, "jets_size>=4");
    // MET
    FillHist(T, "MetInfo.Pt", "met_pt", expr_wei);
    FillHist(T, "MetInfo.Phi", "met_phi", expr_wei);

    // Set tree variables
    float evtwei_central = 0;
    int jets_size = 0;
    vector<float> *Jet_probb = 0;
    vector<float> *Jet_probbb = 0;
    // Set tree branches
    T->SetBranchAddress("evtwei_central", &evtwei_central);
    T->SetBranchAddress("jets_size", &jets_size);
    T->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probb", &Jet_probb);
    T->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probbb", &Jet_probbb);

    // Start event loop
    for (int evt=0; evt<T->GetEntries(); ++evt)
    {
        T->GetEntry(evt);

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
        hists["jet_btag2"]->Fill(Jet_btag[1], evtwei_central);
    } // End of event loop

    // Save histograms
    for (map<TString,TH1D*>::iterator it=hists.begin(); it!=hists.end(); ++it)  it->second->Write();
    fout->Close();
    fin->Close();

    return 0;
}
