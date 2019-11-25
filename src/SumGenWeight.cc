#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"

#include <iostream>
using namespace std;

void SumGenWeight(const char *input, const char *treeName, const char *output)
{
    TFile *fin = new TFile(input);
    TTree *T = (TTree*)fin->Get(treeName);

    float genweight = 0.;
    T->SetBranchAddress("EvtInfo.genweight", &genweight);

    cout << "Start processing n-tuple: " << input << endl << endl;

    long nevt = T->GetEntries();
    double sum_genweight = 0.;
    double sum_genweight2 = 0.;
    for (long evt=0; evt<nevt; ++evt) {
	if (evt % 100000 == 0) cout << "Processing event " << evt << "...\n";
	T->GetEntry(evt);

	sum_genweight += genweight;
	sum_genweight2 += genweight * genweight;
    }
    cout << endl;

    TFile *fout = new TFile(output, "recreate");
    TH1D *hist = new TH1D("sum_genweight", "", 1, 0., 1.);
    TH1D *hist2 = new TH1D("sum_genweight2", "", 1, 0., 1.);
    hist->SetBinContent(1, sum_genweight);
    hist2->SetBinContent(1, sum_genweight2);
    hist->Write();
    hist2->Write();
    cout << "Output is saved at: " << output << endl;

    fout->Close();
    fin->Close();
}

int main(int argc, char **argv)
{
    SumGenWeight(argv[1], argv[2], argv[3]);
    return 0;
}
