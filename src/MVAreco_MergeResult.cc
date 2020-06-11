/*
 * Merge TT tree & ST tree produced by MVAreco_application into single tree, and fill in the events
 * that can't be reconstructed.
 * The tree produced by this code can be used as a friend of the event tree.
 *
 * Usage:
 *   ./MVAreco_MergeResult <event root file> <permutation root file> <output root file> \
 *                         <n-tuple type=TT|ST|bkg> <channel=had|lep> <method>
 *
 *   <event root file>:       Root file with the tree used to generate event permutations
 *   <permutation root file>: Root file with output from MVAreco_application
 *   <output root file>:      Name of output file
 *   <method>:                Name of MVA method used in reconstruction
 *                            (used as the name of output tree)
 */

#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include <iostream>
#include <cstdlib>
using namespace std;

int main(int argc, char **argv)
{
	// Get command line arguments
	TString fevt_name = argv[1];
	TString fperm_name = argv[2];
	TString fout_name = argv[3];
	TString nt_type = argv[4];
	TString channel = argv[5];
	TString method = argv[6];  // This string is used as the name of output tree
	if (nt_type!="TT" && nt_type!="ST" && nt_type!="bkg") {
		cout << "[ERROR] Invalid n-tuple type\n";
		exit(1);
	}
	if (channel!="had" && channel!="lep") {
		cout << "[ERROR] Invalid channel\n";
		exit(1);
	}

	// Read event tree & calculate # of events
	TFile *fevt = new TFile( fevt_name );
	TTree *Tevt = (TTree*)fevt->Get("T");
	int Nevt = Tevt->GetEntries();
	fevt->Close();

	// Read permutation trees
	cout << "[INFO] Reading input file: " << fperm_name << endl;
	TFile *fperm = new TFile( fperm_name );
	TTree *Tperm[2] = {0};    // Tperm[0]: TT permutations    Tperm[1]: ST permutations
	bool run_TT = false;
	bool run_ST = false;
	if (nt_type == "TT" || nt_type == "bkg") {
		Tperm[0] = (TTree*)fperm->Get("T_TT");
		run_TT = true;
	}
	if (nt_type == "ST" || nt_type == "bkg") {
		Tperm[1] = (TTree*)fperm->Get("T_ST");
		run_ST = true;
	}

	// Input tree variables
	// idx 0: for TT    idx 1: for ST
	int Evt_idx[2] = {0};
	int match[2] = {0};
	int bJet_idx[2] = {0};
	float bJet_Pt[2] = {0};
	float bJet_Eta[2] = {0};
	float bJet_Phi[2] = {0};
	float bJet_btag[2] = {0};
	int M1Jet_idx = 0;
	float M1Jet_Pt = 0;
	float M1Jet_Eta = 0;
	float M1Jet_Phi = 0;
	float M1Jet_btag = 0;
	int WJet1_idx[2] = {0};
	float WJet1_Pt[2] = {0};
	float WJet1_Eta[2] = {0};
	float WJet1_Phi[2] = {0};
	float WJet1_btag[2] = {0};
	int WJet2_idx[2] = {0};
	float WJet2_Pt[2] = {0};
	float WJet2_Eta[2] = {0};
	float WJet2_Phi[2] = {0};
	float WJet2_btag[2] = {0};
	int lep_idx[2] = {0};
	int lep_ID[2] = {0};
	float lep_Pt[2] = {0};
	float lep_Eta[2] = {0};
	float lep_Phi[2] = {0};
	float MVA_score[2] = {0};

	// Set input tree branches
	for (int i=0; i<2; ++i) {
		if (nt_type == "TT" && i == 1) continue;
		if (nt_type == "ST" && i == 0) continue;
		Tperm[i]->SetBranchAddress("Evt_idx", &Evt_idx[i]);
		Tperm[i]->SetBranchAddress("match", &match[i]);

		Tperm[i]->SetBranchAddress("bJet_idx", &bJet_idx[i]);
		Tperm[i]->SetBranchAddress("bJet_Pt", &bJet_Pt[i]);
		Tperm[i]->SetBranchAddress("bJet_Eta", &bJet_Eta[i]);
		Tperm[i]->SetBranchAddress("bJet_Phi", &bJet_Phi[i]);
		Tperm[i]->SetBranchAddress("bJet_btag", &bJet_btag[i]);
		if (i == 0) {
			Tperm[i]->SetBranchAddress("M1Jet_idx", &M1Jet_idx);
			Tperm[i]->SetBranchAddress("M1Jet_Pt", &M1Jet_Pt);
			Tperm[i]->SetBranchAddress("M1Jet_Eta", &M1Jet_Eta);
			Tperm[i]->SetBranchAddress("M1Jet_Phi", &M1Jet_Phi);
			Tperm[i]->SetBranchAddress("M1Jet_btag", &M1Jet_btag);
		}
		if (channel == "had") {
			Tperm[i]->SetBranchAddress("WJet1_idx", &WJet1_idx[i]);
			Tperm[i]->SetBranchAddress("WJet1_Pt", &WJet1_Pt[i]);
			Tperm[i]->SetBranchAddress("WJet1_Eta", &WJet1_Eta[i]);
			Tperm[i]->SetBranchAddress("WJet1_Phi", &WJet1_Phi[i]);
			Tperm[i]->SetBranchAddress("WJet1_btag", &WJet1_btag[i]);
			Tperm[i]->SetBranchAddress("WJet2_idx", &WJet2_idx[i]);
			Tperm[i]->SetBranchAddress("WJet2_Pt", &WJet2_Pt[i]);
			Tperm[i]->SetBranchAddress("WJet2_Eta", &WJet2_Eta[i]);
			Tperm[i]->SetBranchAddress("WJet2_Phi", &WJet2_Phi[i]);
			Tperm[i]->SetBranchAddress("WJet2_btag", &WJet2_btag[i]);
		}
		else if (channel == "lep") {
			Tperm[i]->SetBranchAddress("lep_idx", &lep_idx[i]);
			Tperm[i]->SetBranchAddress("lep_ID", &lep_ID[i]);
			Tperm[i]->SetBranchAddress("lep_Pt", &lep_Pt[i]);
			Tperm[i]->SetBranchAddress("lep_Eta", &lep_Eta[i]);
			Tperm[i]->SetBranchAddress("lep_Phi", &lep_Phi[i]);
		}
		Tperm[i]->SetBranchAddress("MVA_score", &MVA_score[i]);
	}

	// Create output file
	cout << "[INFO] Creating output tree: " << fout_name << endl;
	TFile *fout = new TFile( fout_name, "recreate" );
	TTree *Tout = new TTree( Form("T_MVAreco_%s", method.Data()), "" );

	// Output tree variables
	// TT
	int TT_match = 0;
	int TT_bJet_idx = 0;
	float TT_bJet_Pt = 0;
	float TT_bJet_Eta = 0;
	float TT_bJet_Phi = 0;
	float TT_bJet_btag = 0;
	int TT_M1Jet_idx = 0;
	float TT_M1Jet_Pt = 0;
	float TT_M1Jet_Eta = 0;
	float TT_M1Jet_Phi = 0;
	float TT_M1Jet_btag = 0;
	int TT_WJet1_idx = 0;
	float TT_WJet1_Pt = 0;
	float TT_WJet1_Eta = 0;
	float TT_WJet1_Phi = 0;
	float TT_WJet1_btag = 0;
	int TT_WJet2_idx = 0;
	float TT_WJet2_Pt = 0;
	float TT_WJet2_Eta = 0;
	float TT_WJet2_Phi = 0;
	float TT_WJet2_btag = 0;
	int TT_lep_idx = 0;
	int TT_lep_ID = 0;
	float TT_lep_Pt = 0;
	float TT_lep_Eta = 0;
	float TT_lep_Phi = 0;
	float TT_score = 0;
	// ST
	int ST_match = 0;
	int ST_bJet_idx = 0;
	float ST_bJet_Pt = 0;
	float ST_bJet_Eta = 0;
	float ST_bJet_Phi = 0;
	float ST_bJet_btag = 0;
	int ST_WJet1_idx = 0;
	float ST_WJet1_Pt = 0;
	float ST_WJet1_Eta = 0;
	float ST_WJet1_Phi = 0;
	float ST_WJet1_btag = 0;
	int ST_WJet2_idx = 0;
	float ST_WJet2_Pt = 0;
	float ST_WJet2_Eta = 0;
	float ST_WJet2_Phi = 0;
	float ST_WJet2_btag = 0;
	int ST_lep_idx = 0;
	int ST_lep_ID = 0;
	float ST_lep_Pt = 0;
	float ST_lep_Eta = 0;
	float ST_lep_Phi = 0;
	float ST_score = 0;

	// Set output tree branches
	// TT
	Tout->Branch("TT_match", &TT_match);
	Tout->Branch("TT_bJet_idx", &TT_bJet_idx);
	Tout->Branch("TT_bJet_Pt", &TT_bJet_Pt);
	Tout->Branch("TT_bJet_Eta", &TT_bJet_Eta);
	Tout->Branch("TT_bJet_Phi", &TT_bJet_Phi);
	Tout->Branch("TT_bJet_btag", &TT_bJet_btag);
	Tout->Branch("TT_M1Jet_idx", &TT_M1Jet_idx);
	Tout->Branch("TT_M1Jet_Pt", &TT_M1Jet_Pt);
	Tout->Branch("TT_M1Jet_Eta", &TT_M1Jet_Eta);
	Tout->Branch("TT_M1Jet_Phi", &TT_M1Jet_Phi);
	Tout->Branch("TT_M1Jet_btag", &TT_M1Jet_btag);
	Tout->Branch("TT_WJet1_idx", &TT_WJet1_idx);
	Tout->Branch("TT_WJet1_Pt", &TT_WJet1_Pt);
	Tout->Branch("TT_WJet1_Eta", &TT_WJet1_Eta);
	Tout->Branch("TT_WJet1_Phi", &TT_WJet1_Phi);
	Tout->Branch("TT_WJet1_btag", &TT_WJet1_btag);
	Tout->Branch("TT_WJet2_idx", &TT_WJet2_idx);
	Tout->Branch("TT_WJet2_Pt", &TT_WJet2_Pt);
	Tout->Branch("TT_WJet2_Eta", &TT_WJet2_Eta);
	Tout->Branch("TT_WJet2_Phi", &TT_WJet2_Phi);
	Tout->Branch("TT_WJet2_btag", &TT_WJet2_btag);
	Tout->Branch("TT_lep_idx", &TT_lep_idx);
	Tout->Branch("TT_lep_ID", &TT_lep_ID);
	Tout->Branch("TT_lep_Pt", &TT_lep_Pt);
	Tout->Branch("TT_lep_Eta", &TT_lep_Eta);
	Tout->Branch("TT_lep_Phi", &TT_lep_Phi);
	Tout->Branch("TT_score", &TT_score);
	// ST
	Tout->Branch("ST_match", &ST_match);
	Tout->Branch("ST_bJet_idx", &ST_bJet_idx);
	Tout->Branch("ST_bJet_Pt", &ST_bJet_Pt);
	Tout->Branch("ST_bJet_Eta", &ST_bJet_Eta);
	Tout->Branch("ST_bJet_Phi", &ST_bJet_Phi);
	Tout->Branch("ST_bJet_btag", &ST_bJet_btag);
	Tout->Branch("ST_WJet1_idx", &ST_WJet1_idx);
	Tout->Branch("ST_WJet1_Pt", &ST_WJet1_Pt);
	Tout->Branch("ST_WJet1_Eta", &ST_WJet1_Eta);
	Tout->Branch("ST_WJet1_Phi", &ST_WJet1_Phi);
	Tout->Branch("ST_WJet1_btag", &ST_WJet1_btag);
	Tout->Branch("ST_WJet2_idx", &ST_WJet2_idx);
	Tout->Branch("ST_WJet2_Pt", &ST_WJet2_Pt);
	Tout->Branch("ST_WJet2_Eta", &ST_WJet2_Eta);
	Tout->Branch("ST_WJet2_Phi", &ST_WJet2_Phi);
	Tout->Branch("ST_WJet2_btag", &ST_WJet2_btag);
	Tout->Branch("ST_lep_idx", &ST_lep_idx);
	Tout->Branch("ST_lep_ID", &ST_lep_ID);
	Tout->Branch("ST_lep_Pt", &ST_lep_Pt);
	Tout->Branch("ST_lep_Eta", &ST_lep_Eta);
	Tout->Branch("ST_lep_Phi", &ST_lep_Phi);
	Tout->Branch("ST_score", &ST_score);

	int idx_TT = 0;  // perm index for TT perm tree
	int idx_ST = 0;  // perm index for ST perm tree
	if (run_TT) Tperm[0]->GetEntry(idx_TT);
	if (run_ST) Tperm[1]->GetEntry(idx_ST);

	// Start event loop
	for (int evt=0; evt<Nevt; ++evt) {
		// Reset output variables
		// TT
		TT_match = -1;
		TT_bJet_idx = -1;
		TT_bJet_Pt = -999;
		TT_bJet_Eta = -999;
		TT_bJet_Phi = -999;
		TT_bJet_btag = -999;
		TT_M1Jet_idx = -1;
		TT_M1Jet_Pt = -999;
		TT_M1Jet_Eta = -999;
		TT_M1Jet_Phi = -999;
		TT_M1Jet_btag = -999;
		TT_WJet1_idx = -1;
		TT_WJet1_Pt = -999;
		TT_WJet1_Eta = -999;
		TT_WJet1_Phi = -999;
		TT_WJet1_btag = -999;
		TT_WJet2_idx = -1;
		TT_WJet2_Pt = -999;
		TT_WJet2_Eta = -999;
		TT_WJet2_Phi = -999;
		TT_WJet2_btag = -999;
		TT_lep_idx = -1;
		TT_lep_ID = -999;
		TT_lep_Pt = -999;
		TT_lep_Eta = -999;
		TT_lep_Phi = -999;
		TT_score = -999;
		// ST
		ST_match = -1;
		ST_bJet_idx = -1;
		ST_bJet_Pt = -999;
		ST_bJet_Eta = -999;
		ST_bJet_Phi = -999;
		ST_bJet_btag = -999;
		ST_WJet1_idx = -1;
		ST_WJet1_Pt = -999;
		ST_WJet1_Eta = -999;
		ST_WJet1_Phi = -999;
		ST_WJet1_btag = -999;
		ST_WJet2_idx = -1;
		ST_WJet2_Pt = -999;
		ST_WJet2_Eta = -999;
		ST_WJet2_Phi = -999;
		ST_WJet2_btag = -999;
		ST_lep_idx = -1;
		ST_lep_ID = -999;
		ST_lep_Pt = -999;
		ST_lep_Eta = -999;
		ST_lep_Phi = -999;
		ST_score = -999;

		// If TT perm tree contains this event index, copy the perm to output
		if (run_TT && evt == Evt_idx[0]) {
			if (channel == "had") {
				TT_match = match[0];
				TT_bJet_idx = bJet_idx[0];
				TT_bJet_Pt = bJet_Pt[0];
				TT_bJet_Eta = bJet_Eta[0];
				TT_bJet_Phi = bJet_Phi[0];
				TT_bJet_btag = bJet_btag[0];
				TT_M1Jet_idx = M1Jet_idx;
				TT_M1Jet_Pt = M1Jet_Pt;
				TT_M1Jet_Eta = M1Jet_Eta;
				TT_M1Jet_Phi = M1Jet_Phi;
				TT_M1Jet_btag = M1Jet_btag;
				TT_WJet1_idx = WJet1_idx[0];
				TT_WJet1_Pt = WJet1_Pt[0];
				TT_WJet1_Eta = WJet1_Eta[0];
				TT_WJet1_Phi = WJet1_Phi[0];
				TT_WJet1_btag = WJet1_btag[0];
				TT_WJet2_idx = WJet2_idx[0];
				TT_WJet2_Pt = WJet2_Pt[0];
				TT_WJet2_Eta = WJet2_Eta[0];
				TT_WJet2_Phi = WJet2_Phi[0];
				TT_WJet2_btag = WJet2_btag[0];
				TT_score = MVA_score[0];
			}
			else if (channel == "lep") {
				TT_match = match[0];
				TT_bJet_idx = bJet_idx[0];
				TT_bJet_Pt = bJet_Pt[0];
				TT_bJet_Eta = bJet_Eta[0];
				TT_bJet_Phi = bJet_Phi[0];
				TT_bJet_btag = bJet_btag[0];
				TT_M1Jet_idx = M1Jet_idx;
				TT_M1Jet_Pt = M1Jet_Pt;
				TT_M1Jet_Eta = M1Jet_Eta;
				TT_M1Jet_Phi = M1Jet_Phi;
				TT_M1Jet_btag = M1Jet_btag;
				TT_lep_idx = lep_idx[0];
				TT_lep_ID = lep_ID[0];
				TT_lep_Pt = lep_Pt[0];
				TT_lep_Eta = lep_Eta[0];
				TT_lep_Phi = lep_Phi[0];
				TT_score = MVA_score[0];
			}
			// Get the next perm from perm tree
			if (idx_TT != Tperm[0]->GetEntries()-1) {
				++idx_TT;
				Tperm[0]->GetEntry(idx_TT);
			}
		}
		// If ST perm tree contains this event index, copy the perm to output
		if (run_ST && evt == Evt_idx[1]) {
			if (channel == "had") {
				ST_match = match[1];
				ST_bJet_idx = bJet_idx[1];
				ST_bJet_Pt = bJet_Pt[1];
				ST_bJet_Eta = bJet_Eta[1];
				ST_bJet_Phi = bJet_Phi[1];
				ST_bJet_btag = bJet_btag[1];
				ST_WJet1_idx = WJet1_idx[1];
				ST_WJet1_Pt = WJet1_Pt[1];
				ST_WJet1_Eta = WJet1_Eta[1];
				ST_WJet1_Phi = WJet1_Phi[1];
				ST_WJet1_btag = WJet1_btag[1];
				ST_WJet2_idx = WJet2_idx[1];
				ST_WJet2_Pt = WJet2_Pt[1];
				ST_WJet2_Eta = WJet2_Eta[1];
				ST_WJet2_Phi = WJet2_Phi[1];
				ST_WJet2_btag = WJet2_btag[1];
				ST_score = MVA_score[1];
			}
			else if (channel == "lep") {
				ST_match = match[1];
				ST_bJet_idx = bJet_idx[1];
				ST_bJet_Pt = bJet_Pt[1];
				ST_bJet_Eta = bJet_Eta[1];
				ST_bJet_Phi = bJet_Phi[1];
				ST_bJet_btag = bJet_btag[1];
				ST_lep_idx = lep_idx[1];
				ST_lep_ID = lep_ID[1];
				ST_lep_Pt = lep_Pt[1];
				ST_lep_Eta = lep_Eta[1];
				ST_lep_Phi = lep_Phi[1];
				ST_score = MVA_score[1];
			}
			// Get the next perm from perm tree
			if (idx_ST != Tperm[1]->GetEntries()-1) {
				++idx_ST;
				Tperm[1]->GetEntry(idx_ST);
			}
		}

		Tout->Fill();
	} // End of event loop

	fperm->Close();
	fout->Write();  fout->Close();
	cout << "[INFO] Completed!\n";
	cout << "[INFO] # of output events: " << Nevt << endl << endl;

	return 0;
}
