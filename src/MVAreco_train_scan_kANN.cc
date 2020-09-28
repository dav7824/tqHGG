/*
 * Run MVA training on permutations for signal reconstruction.
 * This code is for scanning hyperparameter for ANN.
 * Usage:
 *   ./MVAreco_train_scan_kANN <fin> <outdir> <recotype=TT|ST> <channel=had|lep> <method_name> \
 *           <opt_Ncycle> <opt_layers> <opt_err> <opt_learn> <opt_decay> <opt_batch> <opt_teststep> <opt_conv> <opt_convstep>
 *
 * <fin>:       Root file containing training & test permutation trees of signal MC
 * <outdir>:    Directory to save the result root file
 * <recotype>:  Signal type to be run (TT or ST)
 * <channel>:   Channel to be run (had or lep)
 * <method_name>:  Decide the name of the training configuration.
 *                 The advised format is: <TT|ST><had|lep>_<tag>, for clearity.
 * <opt_Ncycle>:   (TMVA ANN option) number of training epochs
 * <opt_layers>:   (TMVA ANN option) NN hidden layers
 * <opt_err>:      (TMVA ANN option) loss function
 * <opt_learn>:    (TMVA ANN option) learning rate
 * <opt_decay>:    (TMVA ANN option) decay rate of learning rate
 * <opt_batch>:    (TMVA ANN option) batch size
 * <opt_teststep>: (TMVA ANN option) frequency of test for overtraining & convergence
 * <opt_conv>:     (TMVA ANN option) threshold of improvement for convergence test
 * <opt_convstep>: (TMVA ANN option) number of improvement for convergence
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
	TString fin_name = argv[1];
	TString outdir = argv[2];
	TString recotype = argv[3];
	TString channel = argv[4];
	TString method_name = argv[5];
	TString opt_Ncycle = argv[6];
	TString opt_layers = argv[7];
	TString opt_err = argv[8];
	TString opt_learn = argv[9];
	TString opt_decay = argv[10];
	TString opt_batch = argv[11];
	TString opt_teststep = argv[12];
	TString opt_conv = argv[13];
	TString opt_convstep = argv[14];

	if (recotype != "TT" && recotype != "ST") {
		cout << "[ERROR] Invalid reconstruction type\n";
		exit(1);
	}
	if (channel != "had" && channel != "lep") {
		cout << "[ERROR] Invalid channel\n";
		exit(1);
	}
	cout << "[Settings]\n";
	cout << "NCycles = " << opt_Ncycle << endl;
	cout << "HiddenLayers = " << opt_layers << endl;
	cout << "EstimatorType = " << opt_err << endl;
	cout << "LearningRate = " << opt_learn << endl;
	cout << "DecayRate = " << opt_decay << endl;
	cout << "BatchSize = " << opt_batch << endl;
	cout << "TestRate = " << opt_teststep << endl;
	cout << "ConvergenceImprove = " << opt_conv << endl;
	cout << "ConvergenceTests = " << opt_convstep << endl;
	cout << endl;

	// Read input trees
	cout << "[INFO] Openning input files: " << fin_name << endl;
	TChain *TPerm_train = new TChain("TPerm_train");
	TChain *TPerm_test = new TChain("TPerm_test");
	TPerm_train->Add(fin_name);
	TPerm_test->Add(fin_name);

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
	TCut sigcut = "match == 1";
	TCut bkgcut = "match == 0";
	dataloader->PrepareTrainingAndTestTree(sigcut, bkgcut, "V:NormMode=EqualNumEvents");

	// Create TMVA factory
	TString factory_name = "ANN";
	TFile *fSummary = new TFile( outdir+"/result_ANN_"+method_name+".root", "recreate" );
	TMVA::Factory *factory = new TMVA::Factory(factory_name, fSummary, "V:AnalysisType=Classification");

	// Book MVA method
	TString spec("H:V:IgnoreNegWeightsInTraining:VarTransform=N:");
	spec += TString("NCycles=") + opt_Ncycle + ':';
	spec += TString("HiddenLayers=") + opt_layers + ':';
	spec += TString("EstimatorType=") + opt_err + ':';
	spec += TString("LearningRate=") + opt_learn + ':';
	spec += TString("DecayRate=") + opt_decay + ':';
	spec += TString("BPMode=batch:");
	spec += TString("BatchSize=") + opt_batch + ':';
	spec += TString("TestRate=") + opt_teststep + ':';
	spec += TString("ConvergenceImprove=") + opt_conv + ':';
	spec += TString("ConvergenceTests=") + opt_convstep;
	factory->BookMethod(dataloader, TMVA::Types::kMLP, method_name, spec);

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
