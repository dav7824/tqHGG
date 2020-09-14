/*
 * Adjust tree variables for hadronic permutation trees v2.
 * Permutation tree v3 includes a few more variables for leptonic reconstruction.
 * Although these variables are not used for hadronic signals, they are added to v3 hadronic trees with null values
 * so that the tree formats are the same for both hadronic and leptonic trees.
 *
 * Usage:
 *   ./MVAreco_AdjustPermTree <fin> <fout> <nt_type=sig|bkg>
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
	TString fin_name = argv[1];
	TString fout_name = argv[2];
	TString nt_type = argv[3];

	if (nt_type!="sig" && nt_type!="bkg") {
		cout << "[ERROR] Invalid n-tuple type!\n";
		exit(1);
	}

	// Get perm trees
	TFile *fin = new TFile(fin_name);
	TTree *Tin[2];
	if (nt_type=="sig") {
		Tin[0] = (TTree*)fin->Get("TPerm_train");
		Tin[1] = (TTree*)fin->Get("TPerm_test");
	}
	else {
		Tin[0] = (TTree*)fin->Get("TPerm_TT");
		Tin[1] = (TTree*)fin->Get("TPerm_ST");
	}
	// Create output file
	TFile *fout = new TFile(fout_name, "recreate");
	TTree *Tout[2];

	// New tree vairables
	float dPhi_bMET = -999;
	float Met_Pt = -999;

	// Start tree loop
	for (int i=0; i<2; ++i) {
		// Set new tree
		Tout[i] = Tin[i]->CloneTree(0);
		Tout[i]->Branch("dPhi_bMET", &dPhi_bMET);
		Tout[i]->Branch("Met_Pt", &Met_Pt);

		// Start event loop
		for (int evt=0; evt<Tin[i]->GetEntries(); ++evt) {
			Tin[i]->GetEntry(evt);
			Tout[i]->Fill();
		} // End event loop
	} // End tree loop

	fout->Write();
	fout->Close();
	fin->Close();

	return 0;
}
