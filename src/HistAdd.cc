/*
 * Add histograms of the same category.
 * Usage:
 *   ./HistAdd <fin_list> <indir> <fout>
 */

#include "include/utility.h"

#include "TString.h"
#include "TH1D.h"
#include "TKey.h"
#include "TFile.h"

#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char **argv)
{
	// Get command line arguments
	vector<TString> fin_list;
	ParseCStringList(argv[1], fin_list);
	TString indir = argv[2];
	TString fout_name = argv[3];
	int Nfins = fin_list.size();

	// Read input histograms
	vector<TFile*> fins(Nfins);
	for (int i=0; i<Nfins; ++i) fins[i] = new TFile(indir+"/"+fin_list[i]+".root");
	vector<vector<TH1D*>> hist_in(Nfins);
	TKey *key = 0;
	TIter nextkey(fins[0]->GetListOfKeys());
	while (( key=(TKey*)nextkey() )) {
		//cout << "Found hist: " << key->GetName() << endl;
		for (int i=0; i<Nfins; ++i) hist_in[i].push_back((TH1D*)fins[i]->Get(key->GetName()));
	}
	int Nhist = hist_in[0].size();

	// Add histograms
	vector<TH1D*> hist_out(Nhist);
	for (int j=0; j<Nhist; ++j) {
		hist_out[j] = (TH1D*)hist_in[0][j]->Clone();
		for (int i=1; i<Nfins; ++i) hist_out[j]->Add(hist_in[i][j]);
	}

	// Save results
	TFile *fout = new TFile(fout_name, "recreate");
	for (int j=0; j<Nhist; ++j) fout->WriteTObject(hist_out[j]);
	fout->Close();

	for (int i=0; i<Nfins; ++i) fins[i]->Close();

	return 0;
}
