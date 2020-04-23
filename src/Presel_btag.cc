// Preselection:
//   Require at least 1 b-tagged jet.
//
// Usage:
//   ./Presel_btag <input_file> <input_tree> <output_file> <year> <working_point>

#include "include/utility.h"

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TString.h"

#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char **argv)
{
	// Get input arguments
	TString fin_name = argv[1];
	TString inTree_name = argv[2];
	TString fout_name = argv[3];
	TString year = argv[4];
	TString wp = argv[5];

	// Create TTree for input n-tuple
	cout << "Openning input file: " << fin_name << endl;
	TChain *inTree = new TChain(inTree_name);
	inTree->Add(fin_name);

	// Create TTree for output n-tuple
	cout << "Creating output file: " << fout_name << endl;
	TFile *fout = new TFile(fout_name, "recreate");
	TTree *outTree = inTree->CloneTree(0);

	// Get b-tag cut value
	float cut_btag = GetBtagCut(year, wp);

	// Set tree variables
	int jets_size = 0;
	vector<float> *JetInfo_pfDeepCSVJetTags_probb = 0;
	vector<float> *JetInfo_pfDeepCSVJetTags_probbb = 0;
	inTree->SetBranchAddress("jets_size", &jets_size);
	inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probb", &JetInfo_pfDeepCSVJetTags_probb);
	inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probbb", &JetInfo_pfDeepCSVJetTags_probbb);

	// Loop over input TTree to do preselection
	cout << "Start processing events...\n";
	for (long long evt=0; evt<inTree->GetEntries(); ++evt)
	{
		inTree->GetEntry(evt);

		for (int i=0; i<jets_size; ++i)
			if ( JetInfo_pfDeepCSVJetTags_probb->at(i) + JetInfo_pfDeepCSVJetTags_probbb->at(i) > cut_btag ) {
				outTree->Fill();
				break;
			}
	} // End of preselection loop over input TTree
	cout << "Completed processing all events.\n";

	// Write output TTree to output TFile
	fout->cd();
	outTree->Write();
	// Close output TFile
	fout->Close();

	cout << "Output is saved at: " << fout_name << endl;

	return 0;
}
