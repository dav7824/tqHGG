/*
 * Adjust tree variables for v2 hadronic reconstruction trees.
 * v3 tree includes a few more variables for leptonic reconstruction.
 * Although these variables are not used for hadronic signals, they are added to v3 hadronic trees with null values
 * so that tree formats are the same for both hadronic and leptonic trees.
 *
 * Usage:
 *   ./MVAreco_AdjustTree <fin> <fout>
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

	// Get input tree
	TFile *fin = new TFile(fin_name);
	TTree *Tin[2];
	Tin[0] = (TTree*)fin->Get("Treco_TT");
	Tin[1] = (TTree*)fin->Get("Treco_ST");
	// Create output file
	TFile *fout = new TFile(fout_name, "recreate");
	TTree *Tout[2];
	float dPhi_bMET = -999;
	float Met_Pt = -999;

	// Start tree loop
	for (int i=0; i<2; ++i) {
		Tout[i] = Tin[i]->CloneTree(0);
		Tout[i]->Branch("dPhi_bMET", &dPhi_bMET);
		Tout[i]->Branch("Met_Pt", &Met_Pt);

		// Start event loop
		for (int evt=0; evt<Tin[i]->GetEntries(); ++evt) {
			Tin[i]->GetEntry(evt);
			Tout[i]->Fill();
		} // End event loop
	} // End of tree loop

	fout->Write();
	fout->Close();
	fin->Close();

	return 0;
}
