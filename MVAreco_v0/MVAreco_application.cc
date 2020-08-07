/*
 * Apply trained MVA to event permutations of signal MC test samples or bkg MC.
 *
 * Usage:
 *   ./MVAreco_application <input.root> <weight_file.xml> <output.root> <n-tuple_type=sig|bkg> <reco_type=TT|ST> <channel=had|lep> <train_tag>
 *
 * Make sure the options <n-tuple_type>, <reco_type> and <channel> are consistent with the contents of <input.root> and <weight_file.xml>.
 */

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

#include "TString.h"
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"

#include <iostream>
#include <cstdlib>
using namespace std;

int main(int argc, char **argv)
{
	// Get command line arguments
	TString fin_name        = argv[1];
	TString fweight_name    = argv[2];
	TString fout_name       = argv[3];
	TString nt_type         = argv[4];
	TString reco_type       = argv[5];
	TString channel         = argv[6];
	TString train_tag       = argv[7];
	if (nt_type!="sig" && nt_type!="bkg") {
		cout << "[ERROR] Invalid n-tuple type\n";
		exit(1);
	}
	if (reco_type!="TT" && reco_type!="ST") {
		cout << "[ERROR] Invalid reconstruction type\n";
		exit(1);
	}
	if (channel!="had" && channel!="lep") {
		cout << "[ERROR] Invalid channel\n";
		exit(1);
	}
	cout << "[INFO] Start application of trained MVA:\n";
	cout << "--Type of reconstruction: " << reco_type << endl;
	cout << "--Channel:                " << channel << endl;
	cout << "--Training tag:           " << train_tag << endl;

	// Read permutation tree
	cout << "[INFO] Reading input file: " << fin_name << endl;
	TChain *inTree = 0;
	if (nt_type == "bkg") {
		if (reco_type == "TT") inTree = new TChain("T_TT");
		else if (reco_type == "ST") inTree = new TChain("T_ST");
	} else inTree = new TChain("T_test");
	inTree->Add( fin_name );

	/* Input tree variables */
	int NPerm = 0;
	int idxPerm = 0;
	int match = 0;
	// photons
	float LeadPho_Pt = 0;
	float LeadPho_Eta = 0;
	float LeadPho_Phi = 0;
	float LeadPho_IDMVA = 0;
	float SubleadPho_Pt = 0;
	float SubleadPho_Eta = 0;
	float SubleadPho_Phi = 0;
	float SubleadPho_IDMVA = 0;
	// b-jet
	int bJet_idx = 0;
	float bJet_Pt = 0;
	float bJet_Eta = 0;
	float bJet_Phi = 0;
	float bJet_btag = 0;
	// M1 jet (TT)
	int M1Jet_idx = 0;
	float M1Jet_Pt = 0;
	float M1Jet_Eta = 0;
	float M1Jet_Phi = 0;
	float M1Jet_btag = 0;
	// WJets (had)
	int WJet1_idx = 0;
	float WJet1_Pt = 0;
	float WJet1_Eta = 0;
	float WJet1_Phi = 0;
	float WJet1_btag = 0;
	int WJet2_idx = 0;
	float WJet2_Pt = 0;
	float WJet2_Eta = 0;
	float WJet2_Phi = 0;
	float WJet2_btag = 0;
	// Lepton (lep)
	int lep_idx = 0;
	int lep_ID = 0;  float lep_ID_ = 0;
	float lep_Pt = 0;
	float lep_Eta = 0;
	float lep_Phi = 0;

	// Set input tree branches
	inTree->SetBranchAddress("NPerm", &NPerm);
	inTree->SetBranchAddress("idxPerm", &idxPerm);
	inTree->SetBranchAddress("match", &match);
	inTree->SetBranchAddress("LeadPho_Pt", &LeadPho_Pt);
	inTree->SetBranchAddress("LeadPho_Eta", &LeadPho_Eta);
	inTree->SetBranchAddress("LeadPho_Phi", &LeadPho_Phi);
	inTree->SetBranchAddress("LeadPho_IDMVA", &LeadPho_IDMVA);
	inTree->SetBranchAddress("SubleadPho_Pt", &SubleadPho_Pt);
	inTree->SetBranchAddress("SubleadPho_Eta", &SubleadPho_Eta);
	inTree->SetBranchAddress("SubleadPho_Phi", &SubleadPho_Phi);
	inTree->SetBranchAddress("SubleadPho_IDMVA", &SubleadPho_IDMVA);
	inTree->SetBranchAddress("bJet_idx", &bJet_idx);
	inTree->SetBranchAddress("bJet_Pt", &bJet_Pt);
	inTree->SetBranchAddress("bJet_Eta", &bJet_Eta);
	inTree->SetBranchAddress("bJet_Phi", &bJet_Phi);
	inTree->SetBranchAddress("bJet_btag", &bJet_btag);
	if (reco_type == "TT") {
		inTree->SetBranchAddress("M1Jet_idx", &M1Jet_idx);
		inTree->SetBranchAddress("M1Jet_Pt", &M1Jet_Pt);
		inTree->SetBranchAddress("M1Jet_Eta", &M1Jet_Eta);
		inTree->SetBranchAddress("M1Jet_Phi", &M1Jet_Phi);
		inTree->SetBranchAddress("M1Jet_btag", &M1Jet_btag);
	}
	if (channel == "had") {
		inTree->SetBranchAddress("WJet1_idx", &WJet1_idx);
		inTree->SetBranchAddress("WJet1_Pt", &WJet1_Pt);
		inTree->SetBranchAddress("WJet1_Eta", &WJet1_Eta);
		inTree->SetBranchAddress("WJet1_Phi", &WJet1_Phi);
		inTree->SetBranchAddress("WJet1_btag", &WJet1_btag);
		inTree->SetBranchAddress("WJet2_idx", &WJet2_idx);
		inTree->SetBranchAddress("WJet2_Pt", &WJet2_Pt);
		inTree->SetBranchAddress("WJet2_Eta", &WJet2_Eta);
		inTree->SetBranchAddress("WJet2_Phi", &WJet2_Phi);
		inTree->SetBranchAddress("WJet2_btag", &WJet2_btag);
	} else if (channel == "lep") {
		inTree->SetBranchAddress("lep_idx", &lep_idx);
		inTree->SetBranchAddress("lep_ID", &lep_ID);
		inTree->SetBranchAddress("lep_Pt", &lep_Pt);
		inTree->SetBranchAddress("lep_Eta", &lep_Eta);
		inTree->SetBranchAddress("lep_Phi", &lep_Phi);
	}

	// Create output tree
	cout << "[INFO] Creating output file: " << fout_name << endl;
	TFile *fout = new TFile( fout_name, "update" );
	TTree *outTree = inTree->CloneTree(0);
	if (reco_type == "TT") outTree->SetName("T_TT");
	else outTree->SetName("T_ST");

	/* Additional output tree variables */
	float MVA_score = 0;

	outTree->Branch("MVA_score", &MVA_score);

	// Load TMVA library
	TMVA::Tools::Instance();

	// Create TMVA reader
	cout << "[INFO] Creating TMVA reader...\n";
	TMVA::Reader *reader = new TMVA::Reader("V");

	// Set discriminating variables
	reader->AddVariable("LeadPho_Pt", &LeadPho_Pt);
	reader->AddVariable("LeadPho_Eta", &LeadPho_Eta);
	reader->AddVariable("LeadPho_Phi", &LeadPho_Phi);
	reader->AddVariable("LeadPho_IDMVA", &LeadPho_IDMVA);
	reader->AddVariable("SubleadPho_Pt", &SubleadPho_Pt);
	reader->AddVariable("SubleadPho_Eta", &SubleadPho_Eta);
	reader->AddVariable("SubleadPho_Phi", &SubleadPho_Phi);
	reader->AddVariable("SubleadPho_IDMVA", &SubleadPho_IDMVA);
	reader->AddVariable("bJet_Pt", &bJet_Pt);
	reader->AddVariable("bJet_Eta", &bJet_Eta);
	reader->AddVariable("bJet_Phi", &bJet_Phi);
	reader->AddVariable("bJet_btag", &bJet_btag);
	if (reco_type == "TT") {
		reader->AddVariable("M1Jet_Pt", &M1Jet_Pt);
		reader->AddVariable("M1Jet_Eta", &M1Jet_Eta);
		reader->AddVariable("M1Jet_Phi", &M1Jet_Phi);
		reader->AddVariable("M1Jet_btag", &M1Jet_btag);
	}
	if (channel == "had") {
		reader->AddVariable("WJet1_Pt", &WJet1_Pt);
		reader->AddVariable("WJet1_Eta", &WJet1_Eta);
		reader->AddVariable("WJet1_Phi", &WJet1_Phi);
		reader->AddVariable("WJet1_btag", &WJet1_btag);
		reader->AddVariable("WJet2_Pt", &WJet2_Pt);
		reader->AddVariable("WJet2_Eta", &WJet2_Eta);
		reader->AddVariable("WJet2_Phi", &WJet2_Phi);
		reader->AddVariable("WJet2_btag", &WJet2_btag);
	}
	else if (channel == "lep") {
		reader->AddVariable("lep_ID", &lep_ID_);
		reader->AddVariable("lep_Pt", &lep_Pt);
		reader->AddVariable("lep_Eta", &lep_Eta);
		reader->AddVariable("lep_Phi", &lep_Phi);
	}

	// Book MVA methods
	cout << "[INFO] Getting weight file: " << fweight_name << endl;
	reader->BookMVA(train_tag, fweight_name);

	int tot_perm = inTree->GetEntries();
	int tot_evt = 0;
	int tot_evt_match = 0;

	int best_perm = -1;
	float best_score = -999;
	float score = 0;

	// Start permutation loop
	cout << "Processing events...\n";
	for (int perm=0; perm<inTree->GetEntries(); ++perm) {
		inTree->GetEntry(perm);

		if (channel == "lep") lep_ID_ = (float)lep_ID;
		score = reader->EvaluateMVA( train_tag );
		if (score > best_score) {
			best_score = score;
			best_perm = perm;
		}

		if (idxPerm == NPerm-1) {
			inTree->GetEntry(best_perm);
			MVA_score = best_score;
			outTree->Fill();

			best_perm = -1;
			best_score = -999;

			++tot_evt;
			if (nt_type == "sig" && match == 1) ++tot_evt_match;
		}
	} // End of permutation loop

	fout->Write();
	fout->Close();
	delete reader;
	cout << "[INFO] Completed!\n";

	cout << "\n[Summary]\n";
	cout << "--Number of permutations processed: " << tot_perm << endl;
	cout << "--Number of events: " << tot_evt << endl;
	if (nt_type == "sig") {
		cout << "--Number of matched events: " << tot_evt_match << endl;
		cout << "--Reconstruction matching rate: " << (float)tot_evt_match / tot_evt << endl;
	}
	cout << endl;

	return 0;
}
