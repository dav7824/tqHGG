/*
 * Calculate event yield for a bkg MC category.
 * Usage:
 *   ./AddYields <category_name> <file_list> <in_dir> <output.root> <summary.txt>
 */

#include "include/utility.h"

#include "TFile.h"
#include "TH1D.h"
#include "TString.h"

#include <cstdio>
#include <vector>
#include <cmath>
using namespace std;

int main(int argc, char **argv)
{
	// Get command line arguments
	TString cat_name = argv[1];   // Name of sample category
	char *fileList = argv[2];  // List of names of input samples without '.root' suffix
	TString indir = argv[3];   // Path of input directory
	TString fout_name = argv[4];  // Path of output file
	TString fsummary_name = argv[5]; // Path of summary text file

	vector<TString> fins;
	ParseCStringList(fileList, fins);
	int Nfiles = fins.size();

	// Read input files
	vector<float*> yield_in(Nfiles);
	float yield_out[2] = {0};
	for (int i=0; i<Nfiles; ++i) {
		yield_in[i] = new float[2];
		TFile *fin = new TFile(indir + "/" + fins[i] + ".root");
		yield_in[i][0] = ((TH1D*)fin->Get("yield"))->GetBinContent(1);
		yield_in[i][1] = ((TH1D*)fin->Get("yield2"))->GetBinContent(1);
		fin->Close();
	}

	// Calculate category yields
	for (int i=0; i<Nfiles; ++i) {
		yield_out[0] += yield_in[i][0];
		yield_out[1] += yield_in[i][1];
	}

	// Print results
	FILE *fsummary = fopen(fsummary_name.Data(), "a");
	fprintf(fsummary, "%35s\t\tYield: %15f +- %15f\n", cat_name.Data(), yield_out[0], sqrt(yield_out[1]));
	fclose(fsummary);

	// Save results
	TFile *fout = new TFile(fout_name, "recreate");
	TH1D *h_yield = new TH1D("yield", "", 1, 0, 1);  h_yield->SetBinContent(1, yield_out[0]);
	TH1D *h_yield2 = new TH1D("yield2", "", 1, 0, 1);  h_yield2->SetBinContent(1, yield_out[1]);
	fout->Write();
	fout->Close();

	return 0;
}
