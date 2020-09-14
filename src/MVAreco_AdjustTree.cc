/*
 * Adjust tree variables for v2 hadronic reconstruction trees.
 * v3 tree includes a few more variables for leptonic reconstruction.
 * Although these variables are not used for hadronic signals, they are added to v3 hadronic trees with null values
 * so that tree formats are the same for both hadronic and leptonic trees.
 *
 * Usage:
 *   ./MVAreco_AdjustTree <fin> <fout> <recotype=TT|ST>
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
	TString recotype = argv[3];

	if (recotype!="TT" && recotype!="ST") {
		cout << "[ERROR] Invalid reconstruction type!\n";
		exit(1);
	}

	// Get input tree
	TFile *fin = new TFile(fin_name);
	TString treename;
	if (recotype=="TT")  treename = "Treco_TT";
	else  treename = "Treco_ST";
	TTree *Tin = (TTree*)fin->Get(treename);
	// Create output file
	TFile *fout = new TFile(fout_name, "update");
	TTree *Tout = Tin->CloneTree(0);

	// New tree vairables
	float dPhi_bMET = -999;
	float Met_Pt = -999;
	Tout->Branch("dPhi_bMET", &dPhi_bMET);
	Tout->Branch("Met_Pt", &Met_Pt);

	// Start event loop
	for (int evt=0; evt<Tin->GetEntries(); ++evt) {
		Tin->GetEntry(evt);
		Tout->Fill();
	} // End event loop

	fout->Write();
	fout->Close();
	fin->Close();

	return 0;
}
