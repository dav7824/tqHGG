/*
 * Fill histograms for BDT input variables.
 * Usage:
 *   ./FillHist_BDT_had <fin> <fout> <expr_weight> <expr_cut>
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
    TString expr_weight = argv[3];
    TString expr_cut = argv[4];

    // Get input tree
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
    CreateHist(hists, "pho_maxIDMVA", "max #gamma IDMVA", 20, -0.7, 1, "");
    CreateHist(hists, "pho_minIDMVA", "min #gamma IDMVA", 20, -0.7, 1, "");
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
    CreateHist(hists, "jet_N", "N_{jet}", 10, -0.5, 9.5, "");
    CreateHist(hists, "jet_Ht", "H_{T}", 20, 75, 775, "GeV");
    // MET
    CreateHist(hists, "met_Pt", "MET pt", 20, 0, 160, "GeV");
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
    //CreateHist(hists, "BDT_score", "BDT hadronic", 20, -0.6, 0.6, "");

    // Fill histograms without loop
    // photons
    FillHist(T, "pho1_ptOverMgg", "pho1_ptOverMgg", expr_weight, expr_cut);
    FillHist(T, "pho1_eta", "pho1_eta", expr_weight, expr_cut);
    FillHist(T, "pho1_PixelSeed", "pho1_PixelSeed", expr_weight, expr_cut);
    FillHist(T, "pho2_ptOverMgg", "pho2_ptOverMgg", expr_weight, expr_cut);
    FillHist(T, "pho2_eta", "pho2_eta", expr_weight, expr_cut);
    FillHist(T, "pho2_PixelSeed", "pho2_PixelSeed", expr_weight, expr_cut);
    FillHist(T, "pho_maxIDMVA", "pho_maxIDMVA", expr_weight, expr_cut);
    FillHist(T, "pho_minIDMVA", "pho_minIDMVA", expr_weight, expr_cut);
    // diphoton
    FillHist(T, "dipho_ptOverMgg", "dipho_ptOverMgg", expr_weight, expr_cut);
    FillHist(T, "dipho_eta", "dipho_eta", expr_weight, expr_cut);
    FillHist(T, "dipho_dPhi", "dipho_dPhi", expr_weight, expr_cut);
    FillHist(T, "dipho_dR", "dipho_dR", expr_weight, expr_cut);
    FillHist(T, "dipho_heliAngle", "dipho_heliAngle", expr_weight, expr_cut);
    // jets
    FillHist(T, "jet1_pt", "jet1_pt", expr_weight, expr_cut);
    FillHist(T, "jet1_eta", "jet1_eta", expr_weight, expr_cut);
    FillHist(T, "jet1_btag", "jet1_btag", expr_weight, expr_cut);
    FillHist(T, "jet2_pt", "jet2_pt", expr_weight, expr_cut);
    FillHist(T, "jet2_eta", "jet2_eta", expr_weight, expr_cut);
    FillHist(T, "jet2_btag", "jet2_btag", expr_weight, expr_cut);
    FillHist(T, "jet3_pt", "jet3_pt", expr_weight, expr_cut);
    FillHist(T, "jet3_eta", "jet3_eta", expr_weight, expr_cut);
    FillHist(T, "jet3_btag", "jet3_btag", expr_weight, expr_cut);
    FillHist(T, "jet4_pt", "jet4_pt", expr_weight, expr_cut);
    FillHist(T, "jet4_eta", "jet4_eta", expr_weight, expr_cut);
    FillHist(T, "jet4_btag", "jet4_btag", expr_weight, expr_cut);
    FillHist(T, "jet_btag1", "jet_btag1", expr_weight, expr_cut);
    FillHist(T, "jet_btag2", "jet_btag2", expr_weight, expr_cut);
    FillHist(T, "jet_N", "jet_N", expr_weight, expr_cut);
    FillHist(T, "jet_Ht", "jet_Ht", expr_weight, expr_cut);
    // MET
    FillHist(T, "met_Pt", "met_Pt", expr_weight, expr_cut);
    // reco TT
    FillHist(T, "TT_tqH_ptOverM", "TT_tqH_ptOverM", expr_weight, expr_cut);
    FillHist(T, "TT_tqH_eta", "TT_tqH_eta", expr_weight, expr_cut);
    FillHist(T, "TT_dR_tqH_H", "TT_dR_tqH_H", expr_weight, expr_cut);
    FillHist(T, "TT_dR_qH", "TT_dR_qH", expr_weight, expr_cut);
    FillHist(T, "TT_dR_tt", "TT_dR_tt", expr_weight, expr_cut);
    FillHist(T, "TT_tbW_pt", "TT_tbW_pt", expr_weight, expr_cut);
    FillHist(T, "TT_tbW_eta", "TT_tbW_eta", expr_weight, expr_cut);
    FillHist(T, "TT_tbW_M", "TT_tbW_M", expr_weight, expr_cut);
    FillHist(T, "TT_dR_tbW_H", "TT_dR_tbW_H", expr_weight, expr_cut);
    FillHist(T, "TT_score", "TT_score", expr_weight, expr_cut);
    // reco ST
    FillHist(T, "ST_tbW_pt", "ST_tbW_pt", expr_weight, expr_cut);
    FillHist(T, "ST_tbW_eta", "ST_tbW_eta", expr_weight, expr_cut);
    FillHist(T, "ST_tbW_M", "ST_tbW_M", expr_weight, expr_cut);
    FillHist(T, "ST_dR_tbW_H", "ST_dR_tbW_H", expr_weight, expr_cut);
    FillHist(T, "ST_score", "ST_score", expr_weight, expr_cut);
    // BDT score
    //FillHist(T, "BDT_score", "BDT_score", expr_weight, expr_cut);

    // Save histograms
    for (map<TString,TH1D*>::iterator it=hists.begin(); it!=hists.end(); ++it)  it->second->Write();
    fout->Close();
    fin->Close();

    return 0;
}
