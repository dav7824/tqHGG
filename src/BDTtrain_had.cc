/*
 * Train BDT for SR definition (hadronic).
 *
 * ./BDTtrain_had <fins_sig_train> <fins_sig_test> <fins_bkg_train> <fins_bkg_test> <indir_train> <indir_test> <outdir>
 *
 * cd to <outdir> before running.
 */

#include "include/utility.h"

#include "TMVA/Tools.h"
#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TString.h"
#include "TChain.h"
#include "TFile.h"
#include "TCut.h"

#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char **argv)
{
    // Command line arguments
    char   *fins_sig_train_name_ = argv[1];
    char   *fins_sig_test_name_ = argv[2];
    char   *fins_bkg_train_name_ = argv[3];
    char   *fins_bkg_test_name_ = argv[4];
    TString indir_train = argv[5];
    TString indir_test = argv[6];
    TString outdir = argv[7];

    vector<TString> fins_sig_train_name;
    vector<TString> fins_sig_test_name;
    vector<TString> fins_bkg_train_name;
    vector<TString> fins_bkg_test_name;
    ParseCStringList(fins_sig_train_name_, fins_sig_train_name);
    ParseCStringList(fins_sig_test_name_, fins_sig_test_name);
    ParseCStringList(fins_bkg_train_name_, fins_bkg_train_name);
    ParseCStringList(fins_bkg_test_name_, fins_bkg_test_name);
    int nf_sig_train = fins_sig_train_name.size();
    int nf_sig_test  = fins_sig_test_name.size();
    int nf_bkg_train = fins_bkg_train_name.size();
    int nf_bkg_test  = fins_bkg_test_name.size();

    // Read input files
    cout << "[INFO] Read training signal files:\n";
    TChain *Tsig_train = new TChain("T");
    for (int i=0; i<nf_sig_train; ++i) {
        cout << fins_sig_train_name[i] << "  ";
        if (i%4 == 3) cout << endl;
        Tsig_train->Add(indir_train+"/"+fins_sig_train_name[i]+".root");
    }
    cout << "\n[INFO] Read testing signal files:\n";
    TChain *Tsig_test = new TChain("T");
    for (int i=0; i<nf_sig_test; ++i) {
        cout << fins_sig_test_name[i] << "  ";
        if (i%4 == 3) cout << endl;
        Tsig_test->Add(indir_test+"/"+fins_sig_test_name[i]+".root");
    }
    cout << "\n[INFO] Read training bkg files:\n";
    TChain *Tbkg_train = new TChain("T");
    for (int i=0; i<nf_bkg_train; ++i) {
        cout << fins_bkg_train_name[i] << "  ";
        if (i%4 == 3) cout << endl;
        Tbkg_train->Add(indir_train+"/"+fins_bkg_train_name[i]+".root");
    }
    cout << "\n[INFO] Read testing bkg files:\n";
    TChain *Tbkg_test = new TChain("T");
    for (int i=0; i<nf_bkg_test; ++i) {
        cout << fins_bkg_test_name[i] << "  ";
        if (i%4 == 3) cout << endl;
        Tbkg_test->Add(indir_test+"/"+fins_bkg_test_name[i]+".root");
    }
    cout << endl;

    // Load TMVA library
    TMVA::Tools::Instance();

    // Create TMVA dataloader
    cout << "[INFO] Setting TMVA::DataLoader\n";
    TMVA::DataLoader *dataloader = new TMVA::DataLoader("dataset");

    // Global weight for signal / bkg events (not used, set to 1)
    double signalWeight = 1;
    double bkgWeight = 1;

    // Add input trees to dataloader
    dataloader->AddSignalTree(Tsig_train, signalWeight, "Training");
    dataloader->AddSignalTree(Tsig_test, signalWeight, "Test");
    dataloader->AddBackgroundTree(Tbkg_train, bkgWeight, "Training");
    dataloader->AddBackgroundTree(Tbkg_test, bkgWeight, "Test");

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
    dataloader->AddVariable("jet4_pt", 'F');
    dataloader->AddVariable("jet4_eta", 'F');
    dataloader->AddVariable("jet4_btag", 'F');
    dataloader->AddVariable("jet_btag1", 'F');
    dataloader->AddVariable("jet_btag2", 'F');
    dataloader->AddVariable("njets", 'F');
    dataloader->AddVariable("H_t", 'F');
    // MET
    dataloader->AddVariable("Et_miss", 'F');
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
    dataloader->SetSignalWeightExpression(
            "genweight*norm*Nscale*SF_pileup*SF_btag*SF_photon");
    dataloader->SetBackgroundWeightExpression(
            "genweight*norm*Nscale*SF_pileup*SF_btag*SF_photon");

    // Cut on input events and set dataloader options
    TCut sigcut = "";
    TCut bkgcut = "";
    dataloader->PrepareTrainingAndTestTree(sigcut, bkgcut, "V:NormMode=EqualNumEvents");

    // Create TMVA factory
    cout << "[INFO] Setting TMVA::Factory\n";
    TString factory_name = "hadronic_SR";
    TFile *fSummary = new TFile( outdir+"/result_"+factory_name+".root", "recreate" );
    TMVA::Factory *factory = new TMVA::Factory(factory_name, fSummary, "V:AnalysisType=Classification");

    // Book MVA method
    // BDT_v1
    /*factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDT",
            "H:V:NTrees=800:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.2");*/
    // BDT_v2 (ignore negative weights)
    /*factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDT",
            "H:V:NTrees=800:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.2:IgnoreNegWeightsInTraining");*/
    // BDT_v3
    factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDT",
            "H:V:NTrees=800:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.4:IgnoreNegWeightsInTraining");

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
