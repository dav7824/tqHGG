#include "include/Paths.h"

vector<TString> nt_sig_had = {
    "TT_FCNC-TtoHJ_aThad_hct",
    "TT_FCNC-aTtoHJ_Thad_hct",
    "TT_FCNC-TtoHJ_aThad_hut",
    "TT_FCNC-aTtoHJ_Thad_hut",
    "ST_FCNC-TH_Thad_hct",
    "ST_FCNC-TH_Thad_hut",
};
vector<TString> nt_sig_lep = {
    "TT_FCNC-TtoHJ_aTlep_hct",
    "TT_FCNC-aTtoHJ_Tlep_hct",
    "TT_FCNC-TtoHJ_aTlep_hut",
    "TT_FCNC-aTtoHJ_Tlep_hut",
    "ST_FCNC-TH_Tlep_hct",
    "ST_FCNC-TH_Tlep_hut",
};
vector<TString> nt_bkg_had = {
    "DiPhotonJetsBox_scaled",
    "Data-driven_QCD",
    "TGJets",
    "tW_antitop",
    "tW_top",
    "TTGG",
    "TTGJets",
    "TTJets",
    "TTWJetsToLNu",
    "TTZToLLNuNu",
    "DYJetsToLL",
    "WGToLNuG",
    "ZGToLLG",
    "WW",
    "WZ",
    "ZZ",
    "GluGluHToGG",
    "VBFHToGG",
    "VHToGG",
    "ttHJetToGG",
};
vector<TString> nt_bkg_lep = {
    "DiPhotonJetsBox",
    "GJet_Pt-20to40_MGG-80toInf",
    "GJet_Pt-20toInf_MGG-40to80",
    "GJet_Pt-40toInf_MGG-80toInf",
    "QCD_Pt-30toInf_MGG-40to80",
    "QCD_Pt-40toInf_MGG-80toInf",
    "TGJets",
    "tW_antitop",
    "tW_top",
    "TTGG",
    "TTGJets",
    "TTJets",
    "TTWJetsToLNu",
    "TTZToLLNuNu",
    "DYJetsToLL",
    "WGToLNuG",
    "ZGToLLG",
    "WW",
    "WZ",
    "ZZ",
    "GluGluHToGG",
    "VBFHToGG",
    "VHToGG",
    "ttHJetToGG",
};

void MakeHist(TTree *T, TString hist_name, float BDT_lo, float BDT_hi)
{
    // Set tree branches
    float evtwei_central = 0;
    float BDT_score = 0;
    T->SetBranchAddress("evtwei_central", &evtwei_central);
    T->SetBranchAddress("BDT_score", &BDT_score);
    // Create output histogram
    TFile *fout = new TFile("BDT_eff_v2.root", "update");
    TH1F *hh = new TH1F(hist_name, "", 200, BDT_lo, BDT_hi);
    // Fill histogram
    for (int evt=0; evt<T->GetEntries(); ++evt) {
        T->GetEntry(evt);
        hh->Fill(BDT_score, evtwei_central);
    }
    // Save result
    fout->WriteTObject(hh, "", "Overwrite");
    fout->Close();
}

void ProcessTree(TString indir, vector<TString> &nt, TString hist_name, float BDT_lo, float BDT_hi)
{
    TChain *T = new TChain("T");
    for (int i=0; i<nt.size(); ++i)  T->Add( indir+nt.at(i)+".root" );
    MakeHist(T, hist_name, BDT_lo, BDT_hi);
    delete T;
}

void BDT_eff_v2()
{
    TString indir_had = dir_res + "BDT_input_had/";
    TString indir_lep = dir_res + "BDT_input_lep/";

    //ProcessTree(indir_had, nt_sig_had, "BDT_had_sig", -0.6, 0.6);
    ProcessTree(indir_had, nt_bkg_had, "BDT_had_bkg", -0.6, 0.6);
    //ProcessTree(indir_lep, nt_sig_lep, "BDT_lep_sig", -0.5, 0.5);
    //ProcessTree(indir_lep, nt_bkg_lep, "BDT_lep_bkg", -0.5, 0.5);
}
