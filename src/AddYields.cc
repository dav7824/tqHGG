/*
 * Calculate yield of a sample category. Take a list of files as input, sum their yields and save the result.
 * Usage:
 *   ./AddYields <file_list> <in_dir> <output.root> <category_name>
 *
 * <file_list>: a string of comma-separated list of input sample names
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
	char *fileList = argv[1];  // List of names of input samples
	TString indir = argv[2];   // Path of input directory
	TString fout_name = argv[3];  // Path of output file
	TString out_name = argv[4];   // Name of sample category

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
	printf("Category: %35s\t\tYield: %20f +- %15f\n", out_name.Data(), yield_out[0], sqrt(yield_out[1]));

	// Save results
	TFile *fout = new TFile(fout_name, "recreate");
	TH1D *h_yield = new TH1D("yield", "", 1, 0, 1);  h_yield->SetBinContent(1, yield_out[0]);
	TH1D *h_yield2 = new TH1D("yield2", "", 1, 0, 1);  h_yield2->SetBinContent(1, yield_out[1]);
	fout->Write();
	fout->Close();

	return 0;
}
