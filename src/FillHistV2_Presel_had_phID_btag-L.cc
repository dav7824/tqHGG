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
    CreateHist(hists, "evt_Nvtx", "# of vertices", 20, 0, 80, "");
    CreateHist(hists, "evt_rho", "#rho", 20, 0, 60, "");
    // DiPhoton
    CreateHist(hists, "dipho_mass", "diphoton mass", 16, 100, 180, "GeV");
    CreateHist(hists, "dipho_pt", "diphoton pt", 20, 0, 360, "GeV");
    CreateHist(hists, "dipho_mva", "diphoton MVA", 20, -1, 1, "");
    CreateHist(hists, "dipho_idmva1", "#gamma IDMVA high", 20, -1, 1, "");
    CreateHist(hists, "dipho_idmva2", "#gamma IDMVA low", 20, -1, 1, "");
    // Leading photon
    CreateHist(hists, "pho1_pt", "lead #gamma pt", 20, 35, 275, "GeV");
    CreateHist(hists, "pho1_eta", "lead #gamma #eta", 20, -2.5, 2.5, "");
    CreateHist(hists, "pho1_phi", "lead #gamma #phi", 20, -3.5, 3.5, "");
    CreateHist(hists, "pho1_E", "lead #gamma energy", 20, 35, 735, "GeV");
    CreateHist(hists, "pho1_idmva", "lead #gamma IDMVA", 20, -1, 1, "");
    CreateHist(hists, "pho1_PixelSeed", "lead #gamma pixel seed", 2, 0, 2, "");
    // Subleading photon
    CreateHist(hists, "pho2_pt", "sublead #gamma pt", 20, 25, 145, "GeV");
    CreateHist(hists, "pho2_eta", "sublead #gamma #eta", 20, -2.5, 2.5, "");
    CreateHist(hists, "pho2_phi", "sublead #gamma #phi", 20, -3.5, 3.5, "");
    CreateHist(hists, "pho2_E", "sublead #gamma energy", 20, 25, 325, "GeV");
    CreateHist(hists, "pho2_idmva", "sublead #gamma IDMVA", 20, -1, 1, "");
    CreateHist(hists, "pho2_PixelSeed", "sublead #gamma pixel seed", 2, 0, 2, "");
    // Jet
    CreateHist(hists, "jet_N", "# of jets", 13, -0.5, 12.5, "");
    CreateHist(hists, "jet_pt", "jet pt", 20, 25, 225, "GeV");
    CreateHist(hists, "jet_eta", "jet #eta", 20, -2.4, 2.4, "");
    CreateHist(hists, "jet_phi", "jet #phi", 20, -3.5, 3.5, "");
    CreateHist(hists, "jet_M", "jet mass", 20, 0, 40, "GeV");
    CreateHist(hists, "jet_E", "jet energy", 20, 25, 505, "GeV");
    CreateHist(hists, "jet_btag", "jet b-tag", 20, 0, 1, "");
    CreateHist(hists, "jet1_pt", "jet1 pt", 20, 25, 225, "GeV");
    CreateHist(hists, "jet1_eta", "jet1 #eta", 20, -2.4, 2.4, "");
    CreateHist(hists, "jet1_phi", "jet1 #phi", 20, -3.5, 3.5, "");
    CreateHist(hists, "jet1_M", "jet1 mass", 20, 0, 40, "GeV");
    CreateHist(hists, "jet1_E", "jet1 energy", 20, 25, 505, "GeV");
    CreateHist(hists, "jet1_btag", "jet1 b-tag", 20, 0, 1, "");
    CreateHist(hists, "jet2_pt", "jet2 pt", 20, 25, 225, "GeV");
    CreateHist(hists, "jet2_eta", "jet2 #eta", 20, -2.4, 2.4, "");
    CreateHist(hists, "jet2_phi", "jet2 #phi", 20, -3.5, 3.5, "");
    CreateHist(hists, "jet2_M", "jet2 mass", 20, 0, 40, "GeV");
    CreateHist(hists, "jet2_E", "jet2 energy", 20, 25, 505, "GeV");
    CreateHist(hists, "jet2_btag", "jet2 b-tag", 20, 0, 1, "");
    CreateHist(hists, "jet3_pt", "jet3 pt", 20, 25, 225, "GeV");
    CreateHist(hists, "jet3_eta", "jet3 #eta", 20, -2.4, 2.4, "");
    CreateHist(hists, "jet3_phi", "jet3 #phi", 20, -3.5, 3.5, "");
    CreateHist(hists, "jet3_M", "jet3 mass", 20, 0, 40, "GeV");
    CreateHist(hists, "jet3_E", "jet3 energy", 20, 25, 505, "GeV");
    CreateHist(hists, "jet3_btag", "jet3 b-tag", 20, 0, 1, "");
    CreateHist(hists, "jet4_pt", "jet4 pt", 20, 25, 225, "GeV");
    CreateHist(hists, "jet4_eta", "jet4 #eta", 20, -2.4, 2.4, "");
    CreateHist(hists, "jet4_phi", "jet4 #phi", 20, -3.5, 3.5, "");
    CreateHist(hists, "jet4_M", "jet4 mass", 20, 0, 40, "GeV");
    CreateHist(hists, "jet4_E", "jet4 energy", 20, 25, 505, "GeV");
    CreateHist(hists, "jet4_btag", "jet4 b-tag", 20, 0, 1, "");
    CreateHist(hists, "jet_btag1", "jet max b-tag", 20, 0, 1, "");
    CreateHist(hists, "jet_btag2", "jet 2nd max b-tag", 20, 0, 1, "");
    // MET
    CreateHist(hists, "met_pt", "MET pt", 20, 0, 140, "GeV");
    CreateHist(hists, "met_phi", "MET #phi", 20, -3.5, 3.5, "");
    CreateHist(hists, "met_px", "MET px", 20, -150, 150, "GeV");
    CreateHist(hists, "met_py", "MET py", 20, -150, 150, "GeV");
    CreateHist(hists, "met_sumET", "MET E_{T}^{sum}", 20, 0, 4000, "GeV");

    // If input tree contains no event, save empty histograms and end
    if (Tin->GetEntries() == 0) {
        cout << "The tree contains 0 entries.  Save empty histograms.\n";
        for (map<TString,TH1D*>::iterator it=hists.begin(); it!=hists.end(); ++it)  it->second->Write();
        fout->Close();
        fin->Close();

        return 0;
    }

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
    FillHist(Tin, "JetInfo.Pt[1]", "jet2_pt", weight);
    FillHist(Tin, "JetInfo.Eta[1]", "jet2_eta", weight);
    FillHist(Tin, "JetInfo.Phi[1]", "jet2_phi", weight);
    FillHist(Tin, "JetInfo.Mass[1]", "jet2_M", weight);
    FillHist(Tin, "JetInfo.Energy[1]", "jet2_E", weight);
    FillHist(Tin, "JetInfo.pfDeepCSVJetTags_probb[1]+JetInfo.pfDeepCSVJetTags_probbb[1]", "jet2_btag", weight);
    FillHist(Tin, "JetInfo.Pt[2]", "jet3_pt", weight);
    FillHist(Tin, "JetInfo.Eta[2]", "jet3_eta", weight);
    FillHist(Tin, "JetInfo.Phi[2]", "jet3_phi", weight);
    FillHist(Tin, "JetInfo.Mass[2]", "jet3_M", weight);
    FillHist(Tin, "JetInfo.Energy[2]", "jet3_E", weight);
    FillHist(Tin, "JetInfo.pfDeepCSVJetTags_probb[2]+JetInfo.pfDeepCSVJetTags_probbb[2]", "jet3_btag", weight);
    FillHist(Tin, "JetInfo.Pt[3]", "jet4_pt", weight, "jets_size>=4");
    FillHist(Tin, "JetInfo.Eta[3]", "jet4_eta", weight, "jets_size>=4");
    FillHist(Tin, "JetInfo.Phi[3]", "jet4_phi", weight, "jets_size>=4");
    FillHist(Tin, "JetInfo.Mass[3]", "jet4_M", weight, "jets_size>=4");
    FillHist(Tin, "JetInfo.Energy[3]", "jet4_E", weight, "jets_size>=4");
    FillHist(Tin, "JetInfo.pfDeepCSVJetTags_probb[3]+JetInfo.pfDeepCSVJetTags_probbb[3]", "jet4_btag", weight, "jets_size>=4");
    // MET
    FillHist(Tin, "MetInfo.Pt", "met_pt", weight);
    FillHist(Tin, "MetInfo.Phi", "met_phi", weight);
    FillHist(Tin, "MetInfo.Px", "met_px", weight);
    FillHist(Tin, "MetInfo.Py", "met_py", weight);
    FillHist(Tin, "MetInfo.SumET", "met_sumET", weight);

    // Fill histograms with loop
    SFsource.SetTreeBranches(Tin);
    // Set tree variables
    // Jet
    int jets_size = 0;
    vector<float> *Jet_probb = 0;
    vector<float> *Jet_probbb = 0;
    // Set tree branches
    // Jet
    Tin->SetBranchAddress("jets_size", &jets_size);
    Tin->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probb", &Jet_probb);
    Tin->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probbb", &Jet_probbb);

    // Start event loop
    for (int evt=0; evt<Tin->GetEntries(); ++evt)
    {
        Tin->GetEntry(evt);
        float evt_weight = SFsource.GetWeight();

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
        hists["jet_btag2"]->Fill(Jet_btag[1], evt_weight);
    } // End of event loop

    // Save histograms
    for (map<TString,TH1D*>::iterator it=hists.begin(); it!=hists.end(); ++it)  it->second->Write();
    fout->Close();
    fin->Close();

    return 0;
}
