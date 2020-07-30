// Fill and save histograms for given sample.
//
// Usage:
//   ./FillHist <fin> <tree_list> <fout> <channel> <weight_expr>

#include "include/utility.h"

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"

#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

int main(int argc, char **argv)
{
	// Get the arguments
	TString fin_name = argv[1];
	char *tree_list = argv[2];
	TString fout_name = argv[3];
	TString channel = argv[4];
	TString weight = argv[5];
	if (channel!="had" && channel!="lep" && channel!="all") {
		cout << "[ERROR] Invalid channel!\n";
		exit(1);
	}
	vector<TString> trees;
	ParseCStringList(tree_list, trees);

	// Read input file
	cout << "Reading input files: " << fin_name << endl;
	TFile *fin = new TFile(fin_name);
	TTree *inTree = (TTree*)fin->Get(trees[0]);
	for (int i=1; i<trees.size(); ++i) inTree->AddFriend(trees[i]);

	// Create & fill histograms
	/* INSERT CODE : Start */
	/* Finish */

	// Save histograms
	cout << "Writing output file: " << fout_name << endl;
	TFile *fout = new TFile(fout_name, "update");
	for (int i=0; i<hists.size(); ++i) fout->WriteTObject(hists.at(i), "", "Overwrite");
	fout->Close();

	fin->Close();
	cout << "Completed!\n";
	return 0;
}
