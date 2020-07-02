/*
 *  Compute sum of weight, sum of weight squared and number of entries of a dataset, then save the results.
 *
 *  Usage:
 *    ./SampleSummary <input.root> <sample_name> <tree_name> <norm.root> <output.root>
 */

#include "include/utility.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TString.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
using namespace std;

int main(int argc, char **argv)
{
	// Get command line arguments
	TString fin_name = argv[1];
	TString nt_name = argv[2];
	TString tree_name = argv[3];
	TString fnorm_name = argv[4];
	TString fout_name = argv[5];

	// Read input file
	TFile *fin = new TFile(fin_name);
	if (!fin) {
		cout << "[ERROR] Cannot open file: " << fin_name << endl;
		exit(1);
	}
	TTree *Tin = (TTree*)fin->Get(tree_name);

	// Set branches
	float genweight = 0;
	Tin->SetBranchStatus("*", 0);
	Tin->SetBranchStatus("EvtInfo.genweight", 1);
	Tin->SetBranchAddress("EvtInfo.genweight", &genweight);

	long long Nevt = Tin->GetEntries();
	float sumweight = 0;
	float sumweight2 = 0;

	// Loop throught events
	for (long long evt=0; evt<Nevt; ++evt)
	{
		Tin->GetEntry(evt);
		sumweight += genweight;
		sumweight2 += genweight * genweight;
	}

	// Yield normalization
	float nf = GetNormFactor(fnorm_name.Data());
	float yield = sumweight * nf;
	float yield2 = sumweight2 * nf * nf;

	// Print result
	printf("Sample: %35s\t\tYield: %15f +- %15f\t\tEntries: %15lld\n", nt_name.Data(), yield, sqrt(yield2), Nevt);

	// Save results
	TFile *fout = new TFile(fout_name, "recreate");
	TH1D *h_yield = new TH1D("yield", "", 1, 0, 1);  h_yield->SetBinContent(1, yield);
	TH1D *h_yield2 = new TH1D("yield2", "", 1, 0, 1);  h_yield2->SetBinContent(1, yield2);
	fout->Write();
	fout->Close();

	fin->Close();

	return 0;
}
