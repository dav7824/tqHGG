#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"
#include "TString.h"

using namespace std;

int main(int argc, char **argv)
{
	// Get input arguments
	TString fin_name = argv[1];
	TString fpu_name = argv[2];

	// Open event file
	TFile *fin = new TFile(fin_name, "update");
	TTree *T = (TTree*)fin->Get("T");
	float NPu = 0;
	T->SetBranchStatus("*", 0);
	T->SetBranchStatus("EvtInfo.NPu", 1);
	T->SetBranchAddress("EvtInfo.NPu", &NPu);

	// Create SF tree
	float SF_pileup = 0;
	TTree *Tpu = new TTree("SF_pileup", "");
	Tpu->Branch("SF_pileup", &SF_pileup);

	// Open pileup SF file
	TFile *fpu = new TFile(fpu_name);
	TH1D *hpu = (TH1D*)fpu->Get("puhist");

	// Start event loop
	for (int evt=0; evt<T->GetEntries(); ++evt)
	{
		T->GetEntry(evt);

		SF_pileup = hpu->GetBinContent((int)NPu);
		Tpu->Fill();
	}
	// End evnet loop

	fpu->Close();
	fin->WriteTObject(Tpu, "", "Overwrite");
	fin->Close();

	return 0;
}
