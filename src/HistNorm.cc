/*
 * Normalize histograms of a sample.
 * Usage:
 *   ./HistNorm <fin> <fnorm> <fout>
 */

#include "include/utility.h"

#include "TString.h"
#include "TH1D.h"
#include "TKey.h"
#include "TFile.h"

#include <vector>
using namespace std;

int main(int argc, char **argv)
{
	// Get command line arguments
	TString fin_name = argv[1];
	TString fnorm_name = argv[2];
	TString fout_name = argv[3];

	// Read input histograms
	TFile *fin = new TFile(fin_name);
	vector<TH1D*> hist;
	TKey *key = 0;
	TIter nextkey(fin->GetListOfKeys());
	while (( key=(TKey*)nextkey() )) {
		hist.push_back((TH1D*)key->ReadObj());
	}
	int Nhist = hist.size();

	// Normalize histograms
	float nf = GetNormFactor( fnorm_name.Data() );
	for (int j=0; j<Nhist; ++j) hist[j]->Scale(nf);

	// Save results
	TFile *fout = new TFile(fout_name, "recreate");
	for (int j=0; j<Nhist; ++j) fout->WriteTObject(hist[j]);

	fout->Close();
	fin->Close();

	return 0;
}
