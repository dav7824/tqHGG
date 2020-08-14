/*
 * Run MVA training on permutations for signal reconstruction.
 * Usage:
 *   ./MVAreco_train_xxx <fin> <outdir> <recotype> <channel> <train_tag>
 *
 * <fin>:       Root file containing training & test permutation trees of signal MC
 * <outdir>:    Directory to save the result root file
 * <recotype>:  Signal type to be run (TT or ST)
 * <channel>:   Channel to be run (had or lep)
 * <train_tag>: Name tag for TMVA factory root file
 *
 * Add the codes for booking MVA methods in src/MVAreco_methods.cc.
 * Use script/MVAreco_train.py to generate final source code and run.
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
	TString train_tag = argv[5];

	if (recotype != "TT" && recotype != "ST") {
		cout << "[ERROR] Invalid reconstruction type\n";
		exit(1);
	}
	if (channel != "had" && channel != "lep") {
		cout << "[ERROR] Invalid channel\n";
		exit(1);
	}

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
	if (recotype == "TT") {
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
		dataloader->AddVariable("lep_ID", 'F');
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
	TString factory_name = recotype + channel;
	TFile *fSummary = new TFile( outdir+"/result_"+factory_name+'_'+train_tag+".root", "recreate" );
	TMVA::Factory *factory = new TMVA::Factory(factory_name, fSummary, "V:AnalysisType=Classification");

	/* BOOK_MVA_METHOD */

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
