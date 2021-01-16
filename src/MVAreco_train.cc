/*
 * Run MVA training on permutations for signal reconstruction.
 * Usage:
 *   ./MVAreco_train <fin_train> <fin_test> <outdir> <recotype=TT|ST> <channel=had|lep>
 *
 * <fin_train>:   Root file containing training tree
 * <fin_test>:    Root file containing test tree
 * <outdir>:      Directory to save the result root file
 * <recotype>:    Signal type to be run (TT or ST)
 * <channel>:     Channel to be run (had or lep)
 *
 * Besides the root file, TMVA will produce weight files in current working directory.
 * cd to the output path you want before running this code.
 */

#include "TMVA/Tools.h"
#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TString.h"
#include "TChain.h"
#include "TFile.h"
#include "TCut.h"
#include <iostream>
#include <cstdlib>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fin_train_name = argv[1];
    TString fin_test_name = argv[2];
    TString outdir = argv[3];
    TString recotype = argv[4];
    TString channel = argv[5];

    if (recotype != "TT" && recotype != "ST") {
        cout << "[ERROR] Invalid reconstruction type\n";
        exit(1);
    }
    if (channel != "had" && channel != "lep") {
        cout << "[ERROR] Invalid channel\n";
        exit(1);
    }

    // Read input trees
    cout << "[INFO] Openning training input files: " << fin_train_name << endl;
    cout << "[INFO] Openning testing input files: " << fin_test_name << endl;
    TString treename;
    if (recotype == "TT")  treename = "TPerm_TT";
    else  treename = "TPerm_ST";
    TChain *TPerm_train = new TChain(treename);
    TChain *TPerm_test = new TChain(treename);
    TPerm_train->Add(fin_train_name);
    TPerm_test->Add(fin_test_name);

    // Load TMVA library
    TMVA::Tools::Instance();

    // Create TMVA dataloader
    TMVA::DataLoader *dataloader = new TMVA::DataLoader("dataset");

    // Global weight for signal / bkg events (not used, set to 1)
    double signalWeight = 1;
    double bkgWeight = 1;

    // Add input trees to dataloader
    dataloader->AddSignalTree(TPerm_train, signalWeight, "Training");
    dataloader->AddSignalTree(TPerm_test, signalWeight, "Test");
    dataloader->AddBackgroundTree(TPerm_train, bkgWeight, "Training");
    dataloader->AddBackgroundTree(TPerm_test, bkgWeight, "Test");

    // Set discriminating variables
    // Obj variables
    dataloader->AddVariable("bJet_Pt", 'F');
    dataloader->AddVariable("bJet_Eta", 'F');
    dataloader->AddVariable("bJet_btag", 'F');
    if (recotype == "TT") {
        dataloader->AddVariable("M1Jet_Pt", 'F');
        dataloader->AddVariable("M1Jet_Eta", 'F');
        dataloader->AddVariable("M1Jet_btag", 'F');
    }
    if (channel == "had") {
        dataloader->AddVariable("WJet1_Pt", 'F');
        dataloader->AddVariable("WJet1_Eta", 'F');
        dataloader->AddVariable("WJet1_btag", 'F');
        dataloader->AddVariable("WJet2_Pt", 'F');
        dataloader->AddVariable("WJet2_Eta", 'F');
        dataloader->AddVariable("WJet2_btag", 'F');
    } else if (channel == "lep") {
        dataloader->AddVariable("lep_ID", 'F');
        dataloader->AddVariable("lep_Pt", 'F');
        dataloader->AddVariable("lep_Eta", 'F');
    }
    // Inv. mass & relative angles
    if (recotype=="TT" && channel=="had") {
        dataloader->AddVariable("M1", 'F');
        dataloader->AddVariable("M2", 'F');
        dataloader->AddVariable("MW", 'F');
        dataloader->AddVariable("dR_qH", 'F');
        dataloader->AddVariable("dR_bW", 'F');
        dataloader->AddVariable("dR_tt", 'F');
        dataloader->AddVariable("dR_qq", 'F');
    } else if (recotype=="ST" && channel=="had") {
        dataloader->AddVariable("M1", 'F');
        dataloader->AddVariable("MW", 'F');
        dataloader->AddVariable("dR_bW", 'F');
        dataloader->AddVariable("dR_tH", 'F');
        dataloader->AddVariable("dR_qq", 'F');
    } else if (recotype=="TT" && channel=="lep") {
        dataloader->AddVariable("M1", 'F');
        dataloader->AddVariable("dR_qH", 'F');
        dataloader->AddVariable("dR_lb", 'F');
        dataloader->AddVariable("dR_lt", 'F');
    } else if (recotype=="ST" && channel=="lep") {
        dataloader->AddVariable("dR_lb", 'F');
        dataloader->AddVariable("dR_lH", 'F');
    }
    if (channel=="lep") {
        dataloader->AddVariable("dPhi_bMET", 'F');
        dataloader->AddVariable("Met_Pt", 'F');
    }

    // Set individual event weight
    dataloader->SetSignalWeightExpression("Evt_genweight");
    dataloader->SetBackgroundWeightExpression("Evt_genweight");

    // Cut on input events and set dataloader options
    TCut sigcut = "Perm_match == 1";
    TCut bkgcut = "Perm_match == 0";
    dataloader->PrepareTrainingAndTestTree(sigcut, bkgcut, "V:NormMode=EqualNumEvents");

    // Create TMVA factory
    TString factory_name = recotype + channel;
    TFile *fSummary = new TFile( outdir+"/result_"+factory_name+".root", "recreate" );
    TMVA::Factory *factory = new TMVA::Factory(factory_name, fSummary, "V:AnalysisType=Classification");

    // Book MVA method
    if (recotype == "TT" && channel == "had")
        factory->BookMethod(dataloader, TMVA::Types::kMLP, "ANN",
                "H:V:IgnoreNegWeightsInTraining:VarTransform=N:NCycles=500:HiddenLayers=N,N-1:LearningRate=0.02:DecayRate=0.01");
    else if (recotype == "ST" && channel == "had")
        factory->BookMethod(dataloader, TMVA::Types::kMLP, "ANN",
                "H:V:IgnoreNegWeightsInTraining:VarTransform=N:NCycles=500:HiddenLayers=N,N-1:LearningRate=0.02:DecayRate=0.01");
    else if (recotype == "TT" && channel == "lep")
        factory->BookMethod(dataloader, TMVA::Types::kMLP, "ANN",
                "H:V:IgnoreNegWeightsInTraining:VarTransform=N:NCycles=500:HiddenLayers=N,N-1:LearningRate=0.02:DecayRate=0.01");
    else
        factory->BookMethod(dataloader, TMVA::Types::kMLP, "ANN",
                "H:V:IgnoreNegWeightsInTraining:VarTransform=N:NCycles=500:HiddenLayers=N,N-1:LearningRate=0.02:DecayRate=0.01");

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
