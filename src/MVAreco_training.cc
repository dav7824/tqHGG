/*
 * Training MVA using signal MC for event reconstruction.
 *
 * Usage:
 *   MVAreco_training <input_file> <output_dir> <n-tuple_type=TT|ST> <channel=had|lep> <tag>
 *
 * Output files are created under <output_dir>, including
 * (1) summary file
 *   "TrainSummary_[TT|ST][had|lep]_[tag].root"
 * (2) weight files
 *   "dataset/weights/[TT|ST][had|lep]_[MVA method].weights.xml"
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
	TString nt_type = argv[3];
	TString channel = argv[4];
	TString train_tag = argv[5];

	if (nt_type != "TT" && nt_type != "ST") {
		cout << "[ERROR] Invalid n-tuple type\n";
		exit(1);
	}
	if (channel != "had" && channel != "lep") {
		cout << "[ERROR] Invalid channel\n";
		exit(1);
	}

	// Read input trees
	cout << "[INFO] Openning input files: " << fin_name << endl;
	TChain *T_train = new TChain("T_train");
	TChain *T_test = new TChain("T_test");
	T_train->Add(fin_name);
	T_test->Add(fin_name);

	// Load TMVA library
	TMVA::Tools::Instance();

	// Declare TMVA dataloader
	cout << "[INFO] Creating dataloader...\n";
	TMVA::DataLoader *dataloader = new TMVA::DataLoader("dataset");

	// Global weight for signal / bkg events (not used, set to 1)
	double signalWeight = 1;
	double bkgWeight = 1;

	// Add input trees to dataloader
	dataloader->AddSignalTree(T_train, signalWeight, "Training");
	dataloader->AddSignalTree(T_test, signalWeight, "Test");
	dataloader->AddBackgroundTree(T_train, bkgWeight, "Training");
	dataloader->AddBackgroundTree(T_test, bkgWeight, "Test");

	// Set discriminating variables
	dataloader->AddVariable("LeadPho_Pt", 'F');
	dataloader->AddVariable("LeadPho_Eta", 'F');
	dataloader->AddVariable("LeadPho_Phi", 'F');
	dataloader->AddVariable("LeadPho_IDMVA", 'F');
	dataloader->AddVariable("SubleadPho_Pt", 'F');
	dataloader->AddVariable("SubleadPho_Eta", 'F');
	dataloader->AddVariable("SubleadPho_Phi", 'F');
	dataloader->AddVariable("SubleadPho_IDMVA", 'F');
	dataloader->AddVariable("bJet_Pt", 'F');
	dataloader->AddVariable("bJet_Eta", 'F');
	dataloader->AddVariable("bJet_Phi", 'F');
	dataloader->AddVariable("bJet_btag", 'F');
	if (nt_type == "TT") {
		dataloader->AddVariable("M1Jet_Pt", 'F');
		dataloader->AddVariable("M1Jet_Eta", 'F');
		dataloader->AddVariable("M1Jet_Phi", 'F');
		dataloader->AddVariable("M1Jet_btag", 'F');
	}
	if (channel == "had") {
		dataloader->AddVariable("WJet1_Pt", 'F');
		dataloader->AddVariable("WJet1_Eta", 'F');
		dataloader->AddVariable("WJet1_Phi", 'F');
		dataloader->AddVariable("WJet1_btag", 'F');
		dataloader->AddVariable("WJet2_Pt", 'F');
		dataloader->AddVariable("WJet2_Eta", 'F');
		dataloader->AddVariable("WJet2_Phi", 'F');
		dataloader->AddVariable("WJet2_btag", 'F');
	} else if (channel == "lep") {
		dataloader->AddVariable("lep_ID", 'I');
		dataloader->AddVariable("lep_Pt", 'F');
		dataloader->AddVariable("lep_Eta", 'F');
		dataloader->AddVariable("lep_Phi", 'F');
	}

	// Set individual event weight
	dataloader->SetSignalWeightExpression("Evt_genweight");
	dataloader->SetBackgroundWeightExpression("Evt_genweight");

	// Cut on input events and set dataloader options
	TCut sigcut = "match == 1";
	TCut bkgcut = "match == 0";
	dataloader->PrepareTrainingAndTestTree(sigcut, bkgcut, "V:NormMode=EqualNumEvents");

	// Create TMVA factory
	cout << "[INFO] Creating TMVA factory...\n";
	TString factory_name = nt_type + channel;
	TFile *fSummary = new TFile( outdir+"/TrainSummary_"+factory_name+'_'+train_tag+".root", "recreate" );
	TMVA::Factory *factory = new TMVA::Factory(factory_name, fSummary, "V:AnalysisType=Classification");

	/* Book MVA methods */
	//factory->BookMethod( dataloader, TMVA::Types::kFisher, "Fisher", "H:V:VarTransform=G" );
	/*factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT",
		"H:V:IgnoreNegWeightsInTraining:NTrees=800:BoostType=AdaBoost:AdaBoostBeta=0.5");*/
	/*factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDTG",
		"H:V:IgnoreNegWeightsInTraining:NTrees=800:BoostType=Grad:Shrinkage=1");*/
	factory->BookMethod( dataloader, TMVA::Types::kMLP, "ANN",
			"H:V:IgnoreNegWeightsInTraining:VarTransform=N" );

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
