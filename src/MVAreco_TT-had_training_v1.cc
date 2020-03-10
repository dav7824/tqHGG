// Train the models for MVA reconstruction of TT hadronic signal.
//
// input arguments:
// 1. path to input directory
// 2. name of input TTree
// 3. path to output directory

#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

#include "TFile.h"
#include "TChain.h"
#include "TString.h"
#include "TCut.h"

#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
    // get input arguments
    TString indir = argv[1];
    TString inTree_name = argv[2];
    TString outdir = argv[3];

    // read input datasets
    cout << "[INFO] Reading training & test datasets from: " << indir << endl;
    int Nfin = 4;
    TString fin_name[Nfin] = {
	"signal_training", "signal_test",
	"bkg_training", "bkg_test"
    };
    TChain *inTree[Nfin];
    for (int i=0; i<Nfin; ++i) {
	inTree[i] = new TChain(inTree_name);
	inTree[i]->Add( indir + "/" + fin_name[i] + ".root" );
    }
    cout << "[INFO] Create dataloader...\n";

    // load TMVA library
    TMVA::Tools::Instance();

    // declare TMVA dataloader
    TMVA::DataLoader *dataloader = new TMVA::DataLoader("dataset");

    // global weight for signal / bkg events (not used, set to 1)
    double signalWeight = 1;
    double bkgWeight = 1;

    // add input trees to dataloader
    dataloader->AddSignalTree(inTree[0], signalWeight, "Training");
    dataloader->AddSignalTree(inTree[1], signalWeight, "Test");
    dataloader->AddBackgroundTree(inTree[2], bkgWeight, "Training");
    dataloader->AddBackgroundTree(inTree[3], bkgWeight, "Test");

    // set individual event weight
    dataloader->SetSignalWeightExpression("EvtInfo.genweight");
    dataloader->SetBackgroundWeightExpression("EvtInfo.genweight");

    // set discriminating variables
    dataloader->AddVariable("DiPhoInfo.leadPt", 'F');
    dataloader->AddVariable("DiPhoInfo.leadEta", 'F');
    dataloader->AddVariable("DiPhoInfo.leadPhi", 'F');
    //dataloader->AddVariable("DiPhoInfo.leadE", 'F');
    dataloader->AddVariable("DiPhoInfo.leadIDMVA", 'F');
    dataloader->AddVariable("DiPhoInfo.subleadPt", 'F');
    dataloader->AddVariable("DiPhoInfo.subleadEta", 'F');
    dataloader->AddVariable("DiPhoInfo.subleadPhi", 'F');
    //dataloader->AddVariable("DiPhoInfo.subleadE", 'F');
    dataloader->AddVariable("DiPhoInfo.subleadIDMVA", 'F');
    dataloader->AddVariable("bJet_M2_Pt", 'F');
    dataloader->AddVariable("bJet_M2_Eta", 'F');
    dataloader->AddVariable("bJet_M2_Phi", 'F');
    //dataloader->AddVariable("bJet_M2_E", 'F');
    dataloader->AddVariable("bJet_M2_btag", 'F');
    dataloader->AddVariable("Jet_M1_Pt", 'F');
    dataloader->AddVariable("Jet_M1_Eta", 'F');
    dataloader->AddVariable("Jet_M1_Phi", 'F');
    //dataloader->AddVariable("Jet_M1_E", 'F');
    dataloader->AddVariable("Jet_M1_btag", 'F');
    dataloader->AddVariable("Jet1_W_Pt", 'F');
    dataloader->AddVariable("Jet1_W_Eta", 'F');
    dataloader->AddVariable("Jet1_W_Phi", 'F');
    //dataloader->AddVariable("Jet1_W_E", 'F');
    dataloader->AddVariable("Jet1_W_btag", 'F');
    dataloader->AddVariable("Jet2_W_Pt", 'F');
    dataloader->AddVariable("Jet2_W_Eta", 'F');
    dataloader->AddVariable("Jet2_W_Phi", 'F');
    //dataloader->AddVariable("Jet2_W_E", 'F');
    dataloader->AddVariable("Jet2_W_btag", 'F');

    // cut on input events and set dataloader options
    TCut sigcut = "";
    TCut bkgcut = "";
    dataloader->PrepareTrainingAndTestTree(sigcut, bkgcut, "V:NormMode=NumEvents");
    cout << "[INFO] Dataloader successfully set!\n";

    // declare TMVA factory
    TFile *fTrainSummary = new TFile(outdir + "/training_summary.root", "recreate");
    TMVA::Factory *factory = new TMVA::Factory("MVAreco_TT-had", fTrainSummary, "V:AnalysisType=Classification");

    /* book MVA methods */
    // v1
    // BDT
    /*TString opt_method = "H:V:BoostType=AdaBoost:IgnoreNegWeightsInTraining:";
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT_a",
	    opt_method + "NTrees=600:AdaBoostBeta=0.25");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT_b",
	    opt_method + "NTrees=600:AdaBoostBeta=0.5");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT_c",
	    opt_method + "NTrees=600:AdaBoostBeta=0.75");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT_d",
	    opt_method + "NTrees=800:AdaBoostBeta=0.25");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT_e",
	    opt_method + "NTrees=800:AdaBoostBeta=0.5");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT_f",
	    opt_method + "NTrees=800:AdaBoostBeta=0.75");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT_g",
	    opt_method + "NTrees=1000:AdaBoostBeta=0.25");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT_h",
	    opt_method + "NTrees=1000:AdaBoostBeta=0.5");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT_i",
	    opt_method + "NTrees=1000:AdaBoostBeta=0.75");*/
    // v5
    // BDT
    /*TString opt_method = "H:V:IgnoreNegWeightsInTraining:";
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT+D",
	    opt_method + "BoostType=AdaBoost:NTrees=1000:AdaBoostBeta=0.75:VarTransform=D");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT+B",
	    opt_method + "BoostType=AdaBoost:NTrees=1000:AdaBoostBeta=0.75:UseBaggedBoost");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT+D+B",
	    opt_method + "BoostType=AdaBoost:NTrees=1000:AdaBoostBeta=0.75:VarTransform=D:UseBaggedBoost");*/
    // v6
    // BDT
    /*TString opt_method = "H:V:BoostType=Grad:IgnoreNegWeightsInTraining:";
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG_a",
	    opt_method + "NTrees=600:Shrinkage=0.5");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG_b",
	    opt_method + "NTrees=600:Shrinkage=1");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG_c",
	    opt_method + "NTrees=600:Shrinkage=1.5");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG_d",
	    opt_method + "NTrees=800:Shrinkage=0.5");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG_e",
	    opt_method + "NTrees=800:Shrinkage=1");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG_f",
	    opt_method + "NTrees=800:Shrinkage=1.5");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG_g",
	    opt_method + "NTrees=1000:Shrinkage=0.5");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG_h",
	    opt_method + "NTrees=1000:Shrinkage=1");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG_i",
	    opt_method + "NTrees=1000:Shrinkage=1.5");*/
    // v7
    // BDT
    /*TString opt_method = "H:V:BoostType=AdaBoost:IgnoreNegWeightsInTraining:";
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT_1",
	    opt_method + "NTrees=800:AdaBoostBeta=0.05");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT_2",
	    opt_method + "NTrees=800:AdaBoostBeta=0.2");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT_3",
	    opt_method + "NTrees=800:AdaBoostBeta=0.5");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT_4",
	    opt_method + "NTrees=800:AdaBoostBeta=1.25");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT_5",
	    opt_method + "NTrees=800:AdaBoostBeta=5");*/
    // v8
    // BDT
    TString opt_method = "H:V:BoostType=Grad:IgnoreNegWeightsInTraining:";
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG_1",
	    opt_method + "NTrees=800:Shrinkage=0.1");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG_2",
	    opt_method + "NTrees=800:Shrinkage=0.4");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG_3",
	    opt_method + "NTrees=800:Shrinkage=1");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG_4",
	    opt_method + "NTrees=800:Shrinkage=2.5");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG_5",
	    opt_method + "NTrees=800:Shrinkage=10");

    // start training model
    cout << "[INFO] Start training!\n";
    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();

    fTrainSummary->Close();
    cout << "[INFO] End of training!\n";

    delete factory;
    delete dataloader;
    cout << "[INFO] Output files are saved at: " << outdir << endl;

    return 0;
}
