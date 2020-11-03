/*
 * Fill histograms for BDT input variables.
 * Usage:
 *   ./FillHist_BDT_had <fin> <fout> <weight_expr>
 *
 * <weight_expr>: Event weight expression by tree branches
 */

#include "include/utility.h"

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"

#include <iostream>
#include <cstdlib>
#include <map>
using namespace std;

int main(int argc, char **argv)
{
    // Command line arguments
    TString fin_name = argv[1];
    TString fout_name = argv[2];
    TString weight_expr = argv[3];

    // Get input tree
    cout << "[INFO] Read input file: " << fin_name << endl;
    cout << "Weight expression: " << weight_expr << endl;
    TFile *fin = new TFile(fin_name);
    TTree *T = (TTree*)fin->Get("T");

    // Output file
    TFile *fout = new TFile(fout_name, "recreate");
    map<TString, TH1D*> hists;
    // Create histograms
    // photons
    CreateHist(hists, "pho1_ptOverMgg", "lead #gamma pt/M_{#gamma#gamma}", 20, 0, 1.8, "");
    CreateHist(hists, "pho1_eta", "lead #gamma #eta", 20, -2.5, 2.5, "");
    CreateHist(hists, "pho1_PixelSeed", "lead #gamma pixel seed", 2, 0, 2, "");
    CreateHist(hists, "pho2_ptOverMgg", "sublead #gamma pt/M_{#gamma#gamma}", 20, 0, 0.8, "");
    CreateHist(hists, "pho2_eta", "sublead #gamma #eta", 20, -2.5, 2.5, "");
    CreateHist(hists, "pho2_PixelSeed", "sublead #gamma pixel seed", 2, 0, 2, "");
    CreateHist(hists, "pho_maxIDMVA", "max #gamma IDMVA", 20, -1, 1, "");
    CreateHist(hists, "pho_minIDMVA", "min #gamma IDMVA", 20, -1, 1, "");
    // diphoton
    CreateHist(hists, "dipho_ptOverMgg", "p_{T}^{#gamma#gamma}/M_{#gamma#gamma}", 20, 0, 1.8, "");
    CreateHist(hists, "dipho_eta", "#eta_{#gamma#gamma}", 20, -5, 5, "");
    CreateHist(hists, "dipho_dPhi", "|cos(#Delta#phi_{#gamma#gamma})|", 20, 0, 1, "");
    CreateHist(hists, "dipho_dR", "#DeltaR_{#gamma#gamma}", 20, 0, 6, "");
    CreateHist(hists, "dipho_heliAngle", "|cos(helicity angle(#theta))|", 20, 0, 1, "");
    // jets
    CreateHist(hists, "jet1_pt", "jet1 pt", 20, 25, 345, "GeV");
    CreateHist(hists, "jet1_eta", "jet1 #eta", 20, -2.4, 2.4, "");
    CreateHist(hists, "jet1_btag", "jet1 b-tag", 20, 0, 1, "");
    CreateHist(hists, "jet2_pt", "jet2 pt", 20, 25, 205, "GeV");
    CreateHist(hists, "jet2_eta", "jet2 #eta", 20, -2.4, 2.4, "");
    CreateHist(hists, "jet2_btag", "jet2 b-tag", 20, 0, 1, "");
    CreateHist(hists, "jet3_pt", "jet3 pt", 20, 25, 105, "GeV");
    CreateHist(hists, "jet3_eta", "jet3 #eta", 20, -2.4, 2.4, "");
    CreateHist(hists, "jet3_btag", "jet3 b-tag", 20, 0, 1, "");
    CreateHist(hists, "jet4_pt", "jet4 pt", 20, 25, 85, "GeV");
    CreateHist(hists, "jet4_eta", "jet4 #eta", 20, -2.4, 2.4, "");
    CreateHist(hists, "jet4_btag", "jet4 b-tag", 20, 0, 1, "");
    CreateHist(hists, "jet_btag1", "jet max b-tag", 20, 0, 1, "");
    CreateHist(hists, "jet_btag2", "jet 2nd max b-tag", 20, 0, 1, "");
    CreateHist(hists, "njets", "N_jet", 10, -0.5, 9.5, "");
    CreateHist(hists, "H_t", "H_T", 20, 75, 775, "GeV");
    // MET
    CreateHist(hists, "Et_miss", "MET pt", 20, 0, 160, "GeV");
    // reco TT
    CreateHist(hists, "TT_tqH_ptOverM", "TT top(tqH) pt/M", 20, 0, 1.6, "");
    CreateHist(hists, "TT_tqH_eta", "TT top(tqH) #eta", 20, -5, 5, "");
    CreateHist(hists, "TT_dR_tqH_H", "TT #DeltaR(top(tqH),H)", 20, 0, 6, "");
    CreateHist(hists, "TT_dR_qH", "TT #DeltaR(q,H)", 20, 0, 6, "");
    CreateHist(hists, "TT_dR_tt", "TT #DeltaR(t,#bar{t})", 20, 0, 7, "");
    CreateHist(hists, "TT_tbW_pt", "TT top(tbW) pt", 20, 0, 500, "GeV");
    CreateHist(hists, "TT_tbW_eta", "TT top(tbW) #eta", 20, -5, 5, "");
    CreateHist(hists, "TT_tbW_M", "TT top(tbW) M", 20, 0, 700, "GeV");
    CreateHist(hists, "TT_dR_tbW_H", "TT #DeltaR(top(tbW),H)", 20, 0, 7, "");
    CreateHist(hists, "TT_score", "TT reco score", 20, 0, 1, "");
    // reco ST
    CreateHist(hists, "ST_tbW_pt", "ST top(tbW) pt", 20, 0, 500, "GeV");
    CreateHist(hists, "ST_tbW_eta", "ST top(tbW) #eta", 20, -5, 5, "");
    CreateHist(hists, "ST_tbW_M", "ST top(tbW) M", 20, 0, 700, "GeV");
    CreateHist(hists, "ST_dR_tbW_H", "ST #DeltaR(top(tbW),H)", 20, 0, 7, "");
    CreateHist(hists, "ST_score", "ST reco score", 20, 0, 1, "");
    // BDT score
    CreateHist(hists, "BDT_score", "BDT hadronic", 20, -0.6, 0.6, "");

    // If input tree contains no event, save empty histograms and end
    if (T->GetEntries() == 0) {
        cout << "The tree contains 0 entries.  Save empty histograms.\n";
        for (map<TString,TH1D*>::iterator it=hists.begin(); it!=hists.end(); ++it)  it->second->Write();
        fout->Close();
        fin->Close();

        return 0;
    }

    // Fill histograms without loop
    TString weight = weight_expr;
    // photons
    FillHist(T, "pho1_ptOverMgg", "pho1_ptOverMgg", weight);
    FillHist(T, "pho1_eta", "pho1_eta", weight);
    FillHist(T, "pho1_PixelSeed", "pho1_PixelSeed", weight);
    FillHist(T, "pho2_ptOverMgg", "pho2_ptOverMgg", weight);
    FillHist(T, "pho2_eta", "pho2_eta", weight);
    FillHist(T, "pho2_PixelSeed", "pho2_PixelSeed", weight);
    FillHist(T, "pho_maxIDMVA", "pho_maxIDMVA", weight);
    FillHist(T, "pho_minIDMVA", "pho_minIDMVA", weight);
    // diphoton
    FillHist(T, "dipho_ptOverMgg", "dipho_ptOverMgg", weight);
    FillHist(T, "dipho_eta", "dipho_eta", weight);
    FillHist(T, "dipho_dPhi", "dipho_dPhi", weight);
    FillHist(T, "dipho_dR", "dipho_dR", weight);
    FillHist(T, "dipho_heliAngle", "dipho_heliAngle", weight);
    // jets
    FillHist(T, "jet1_pt", "jet1_pt", weight);
    FillHist(T, "jet1_eta", "jet1_eta", weight);
    FillHist(T, "jet1_btag", "jet1_btag", weight);
    FillHist(T, "jet2_pt", "jet2_pt", weight);
    FillHist(T, "jet2_eta", "jet2_eta", weight);
    FillHist(T, "jet2_btag", "jet2_btag", weight);
    FillHist(T, "jet3_pt", "jet3_pt", weight);
    FillHist(T, "jet3_eta", "jet3_eta", weight);
    FillHist(T, "jet3_btag", "jet3_btag", weight);
    FillHist(T, "jet4_pt", "jet4_pt", weight);
    FillHist(T, "jet4_eta", "jet4_eta", weight);
    FillHist(T, "jet4_btag", "jet4_btag", weight);
    FillHist(T, "jet_btag1", "jet_btag1", weight);
    FillHist(T, "jet_btag2", "jet_btag2", weight);
    FillHist(T, "njets", "njets", weight);
    FillHist(T, "H_t", "H_t", weight);
    // MET
    FillHist(T, "Et_miss", "Et_miss", weight);
    // reco TT
    FillHist(T, "TT_tqH_ptOverM", "TT_tqH_ptOverM", weight);
    FillHist(T, "TT_tqH_eta", "TT_tqH_eta", weight);
    FillHist(T, "TT_dR_tqH_H", "TT_dR_tqH_H", weight);
    FillHist(T, "TT_dR_qH", "TT_dR_qH", weight);
    FillHist(T, "TT_dR_tt", "TT_dR_tt", weight);
    FillHist(T, "TT_tbW_pt", "TT_tbW_pt", weight);
    FillHist(T, "TT_tbW_eta", "TT_tbW_eta", weight);
    FillHist(T, "TT_tbW_M", "TT_tbW_M", weight);
    FillHist(T, "TT_dR_tbW_H", "TT_dR_tbW_H", weight);
    FillHist(T, "TT_score", "TT_score", weight);
    // reco ST
    FillHist(T, "ST_tbW_pt", "ST_tbW_pt", weight);
    FillHist(T, "ST_tbW_eta", "ST_tbW_eta", weight);
    FillHist(T, "ST_tbW_M", "ST_tbW_M", weight);
    FillHist(T, "ST_dR_tbW_H", "ST_dR_tbW_H", weight);
    FillHist(T, "ST_score", "ST_score", weight);
    // BDT score
    FillHist(T, "BDT_score", "BDT_score", weight);

    // Save histograms
    for (map<TString,TH1D*>::iterator it=hists.begin(); it!=hists.end(); ++it)  it->second->Write();
    fout->Close();
    fin->Close();
    cout << "[INFO] Save output: " << fout_name << endl;

    return 0;
}
