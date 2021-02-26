/*
 * Train BDT for SR definition.
 *
 * ./BDT_train <indir> <outdir> <channel=had|lep>
 *
 * cd to <outdir> before running.
 */

#include "include/utility.h"
#include "TMVA/Tools.h"
#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TString.h"
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TCut.h"
#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

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
vector<TString> nt_resbkg_had = {
    "GluGluHToGG",
    "VBFHToGG",
    "VHToGG",
    "ttHJetToGG",
};
vector<TString> nt_resbkg_lep = {
    "GluGluHToGG",
    "VBFHToGG",
    "VHToGG",
    "ttHJetToGG",
};
vector<TString> nt_nrbkg_had = {
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
};
vector<TString> nt_nrbkg_lep = {
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
};

int main(int argc, char **argv)
{
    // Command line arguments
    TString indir(argv[1]);
    TString outdir(argv[2]);
    TString ch(argv[3]);
    vector<TString> *nt_sig, *nt_resbkg, *nt_nrbkg;
    if (ch=="had") {
        nt_sig = &nt_sig_had;
        nt_resbkg = &nt_resbkg_had;
        nt_nrbkg = &nt_nrbkg_had;
    } else if (ch=="lep") {
        nt_sig = &nt_sig_lep;
        nt_resbkg = &nt_resbkg_lep;
        nt_nrbkg = &nt_nrbkg_lep;
    } else {
        cout << "[ERROR] Invalid channel\n";
        exit(1);
    }

    // Read input files
    cout << "[INFO] Reading input dir: " << indir << endl;
    TChain *Tsig = new TChain("T");
    TChain *Tresbkg = new TChain("T");
    TChain *Tnrbkg = new TChain("T");
    for (int i=0; i<nt_sig->size(); ++i)  Tsig->Add( indir+'/'+nt_sig->at(i)+".root" );
    for (int i=0; i<nt_resbkg->size(); ++i)  Tresbkg->Add( indir+'/'+nt_resbkg->at(i)+".root" );
    for (int i=0; i<nt_nrbkg->size(); ++i)  Tnrbkg->Add( indir+'/'+nt_nrbkg->at(i)+".root" );
    TTree *Tsig_train = Tsig->CopyTree("evt_no%3==0");
    TTree *Tsig_test = Tsig->CopyTree("evt_no%3==1");
    TTree *Tresbkg_train = Tresbkg->CopyTree("evt_no%3==0");
    TTree *Tresbkg_test = Tresbkg->CopyTree("evt_no%3==1");
    TTree *Tnrbkg_train = Tnrbkg->CopyTree("evt_no%2==0");
    TTree *Tnrbkg_test = Tnrbkg->CopyTree("evt_no%2==1");

    // Load TMVA library
    TMVA::Tools::Instance();

    // Create TMVA dataloader
    TMVA::DataLoader *dataloader = new TMVA::DataLoader("dataset");

    // Global weight for signal / bkg events (not used, set to 1)
    double signalWeight = 1;
    double bkgWeight = 1;

    // Add input trees to dataloader
    dataloader->AddSignalTree(Tsig_train, signalWeight, "Training");
    dataloader->AddSignalTree(Tsig_test, signalWeight, "Test");
    dataloader->AddBackgroundTree(Tresbkg_train, bkgWeight, "Training");
    dataloader->AddBackgroundTree(Tresbkg_test, bkgWeight, "Test");
    dataloader->AddBackgroundTree(Tnrbkg_train, bkgWeight, "Training");
    dataloader->AddBackgroundTree(Tnrbkg_test, bkgWeight, "Test");

    // Set discriminating variables
    // photons
    dataloader->AddVariable("pho1_ptOverMgg", 'F');
    dataloader->AddVariable("pho1_eta", 'F');
    dataloader->AddVariable("pho1_PixelSeed", 'F');
    dataloader->AddVariable("pho2_ptOverMgg", 'F');
    dataloader->AddVariable("pho2_eta", 'F');
    dataloader->AddVariable("pho2_PixelSeed", 'F');
    dataloader->AddVariable("pho_maxIDMVA", 'F');
    dataloader->AddVariable("pho_minIDMVA", 'F');
    // diphoton
    dataloader->AddVariable("dipho_ptOverMgg", 'F');
    dataloader->AddVariable("dipho_eta", 'F');
    dataloader->AddVariable("dipho_dPhi", 'F');
    dataloader->AddVariable("dipho_dR", 'F');
    dataloader->AddVariable("dipho_heliAngle", 'F');
    // leptons
    if (ch=="lep") {
        dataloader->AddVariable("lepton_ID", 'F');
        dataloader->AddVariable("lepton_pt", 'F');
        dataloader->AddVariable("lepton_eta", 'F');
        dataloader->AddVariable("lepton_ntight", 'F');
    }
    // jets
    dataloader->AddVariable("jet1_pt", 'F');
    dataloader->AddVariable("jet1_eta", 'F');
    dataloader->AddVariable("jet1_btag", 'F');
    dataloader->AddVariable("jet2_pt", 'F');
    dataloader->AddVariable("jet2_eta", 'F');
    dataloader->AddVariable("jet2_btag", 'F');
    dataloader->AddVariable("jet3_pt", 'F');
    dataloader->AddVariable("jet3_eta", 'F');
    dataloader->AddVariable("jet3_btag", 'F');
    if (ch=="had") {
        dataloader->AddVariable("jet4_pt", 'F');
        dataloader->AddVariable("jet4_eta", 'F');
        dataloader->AddVariable("jet4_btag", 'F');
    }
    dataloader->AddVariable("jet_btag1", 'F');
    dataloader->AddVariable("jet_btag2", 'F');
    dataloader->AddVariable("jet_N", 'F');
    dataloader->AddVariable("jet_Ht", 'F');
    // MET
    dataloader->AddVariable("met_Pt", 'F');
    // reco TT
    dataloader->AddVariable("TT_tqH_ptOverM", 'F');
    dataloader->AddVariable("TT_tqH_eta", 'F');
    dataloader->AddVariable("TT_dR_tqH_H", 'F');
    dataloader->AddVariable("TT_dR_qH", 'F');
    dataloader->AddVariable("TT_dR_tt", 'F');
    dataloader->AddVariable("TT_tbW_pt", 'F');
    dataloader->AddVariable("TT_tbW_eta", 'F');
    dataloader->AddVariable("TT_tbW_M", 'F');
    dataloader->AddVariable("TT_dR_tbW_H", 'F');
    dataloader->AddVariable("TT_score", 'F');
    // reco ST
    dataloader->AddVariable("ST_tbW_pt", 'F');
    dataloader->AddVariable("ST_tbW_eta", 'F');
    dataloader->AddVariable("ST_tbW_M", 'F');
    dataloader->AddVariable("ST_dR_tbW_H", 'F');
    dataloader->AddVariable("ST_score", 'F');

    // Set individual event weight
    dataloader->SetSignalWeightExpression("evtwei_central*evtwei_scale");
    dataloader->SetBackgroundWeightExpression("evtwei_central*evtwei_scale");

    // Cut on input events and set dataloader options
    TCut sigcut = "";
    TCut bkgcut = "";
    dataloader->PrepareTrainingAndTestTree(sigcut, bkgcut, "V:NormMode=EqualNumEvents");

    // Create TMVA factory
    TString factory_name;
    if (ch=="had")  factory_name = "SRhad";
    else  factory_name = "SRlep";
    TFile *fSummary = new TFile( outdir+"/result_"+factory_name+".root", "recreate" );
    TMVA::Factory *factory = new TMVA::Factory(factory_name, fSummary, "V:AnalysisType=Classification");

    // Book MVA method
    if (ch=="had") {
        factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDT",
                "H:V:NTrees=800:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.4:IgnoreNegWeightsInTraining");
    } else {
        factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDT",
                "H:V:NTrees=800:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.4:IgnoreNegWeightsInTraining");
    }

    // Start training model
    cout << "[INFO] Start training!\n";
    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();

    fSummary->Close();
    cout << "[INFO] End of training!\n";

    delete factory;
    delete dataloader;
    cout << "[INFO] Output files are saved at: " << outdir << endl;

    return 0;
}
