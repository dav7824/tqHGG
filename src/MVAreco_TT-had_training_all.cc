// Train the models for MVA reconstruction of TT hadronic signal.
//
// input arguments:
// 1. string that describes the MVA settings

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
    TString tag_MVA = argv[1];

    // read input datasets
    cout << "[INFO] Reading training & test datasets...\n";
    TString indir = "/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_TT-had/training/";
    TString outdir = indir + "All/";
    TString inTree_name = "flashggStdTree";

    int Nsample = 4;
    TString sample[Nsample] = {
	"TT_FCNC-TtoHJ_aThad_hct", "TT_FCNC-TtoHJ_aThad_hut", "TT_FCNC-aTtoHJ_Thad_hct", "TT_FCNC-aTtoHJ_Thad_hut"
    };

    int Ntreetype = 4;
    TString fin_name[Ntreetype] = {
	"signal_training", "signal_test",
	"bkg_training", "bkg_test"
    };
    TChain *inTree[Ntreetype];
    for (int i=0; i<Ntreetype; ++i) {
	inTree[i] = new TChain(inTree_name);
	for (int j=0; j<Nsample; ++j) {
	    inTree[i]->Add( indir + sample[j] + "/" + fin_name[i] + ".root" );
	}
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
    TFile *fTrainSummary = new TFile(outdir + "training_summary_" + tag_MVA + ".root", "recreate");
    TMVA::Factory *factory = new TMVA::Factory("MVAreco_TT-had", fTrainSummary, "V:AnalysisType=Classification");

    /* book MVA methods */
    // BDT
    /*factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT",
	    "H:V:IgnoreNegWeightsInTraining:NTrees=1000:BoostType=AdaBoost:AdaBoostBeta=0.75");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG",
	    "H:V:IgnoreNegWeightsInTraining:NTrees=1000:BoostType=Grad:Shrinkage=1.5");*/
    // BDT_v2
    /*factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT+D",
	    "H:V:IgnoreNegWeightsInTraining:NTrees=1000:BoostType=AdaBoost:AdaBoostBeta=0.75:VarTransform=D");
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG+D",
	    "H:V:IgnoreNegWeightsInTraining:NTrees=1000:BoostType=Grad:Shrinkage=1.5:VarTransform=D");*/
    // ANN
    /*factory->BookMethod( dataloader, TMVA::Types::kMLP, "ANN",
	    "H:V:IgnoreNegWeightsInTraining:VarTransform=N");*/
    // Cuts
    /*factory->BookMethod( dataloader, TMVA::Types::kCuts, "Cuts",
	    "H:V:FitMethod=MC:EffSel:VarProp=FSmart:SampleSize=200000:VarTransform=D,P");*/
    // Fisher
    factory->BookMethod( dataloader, TMVA::Types::kFisher, "Fisher",
	    "H:V:VarTransform=G");

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
