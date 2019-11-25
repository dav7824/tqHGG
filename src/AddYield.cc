#include "utility.h"

#include "TFile.h"
#include "TH1D.h"

#include <iostream>
#include <vector>
#include <string>
using namespace std;

void AddYield(const char *input, const char *output)
{
    cout << "Openning input file: " << input << endl;

    vector<string> input_;
    ParseCStringList(input, input_);

    double yield = 0.;
    double yield_err2 = 0.;
    for (int i=0; i<input_.size(); ++i) {
	TFile *fin = new TFile(input_[i].c_str());
	TH1D *hyield = (TH1D*)fin->Get("yield");
	TH1D *herr2 = (TH1D*)fin->Get("yield_err2");
	yield += hyield->GetBinContent(1);
	yield_err2 += herr2->GetBinContent(1);
	fin->Close();
    }

    TFile *fout = new TFile(output, "recreate");
    TH1D *h1 = new TH1D("yield", "", 1, 0., 1.);
    TH1D *h2 = new TH1D("yield_err2", "", 1, 0., 1.);
    h1->SetBinContent(1, yield);
    h2->SetBinContent(1, yield_err2);
    h1->Write();
    h2->Write();
    fout->Close();

    cout << "Output is saved at: " << output << endl;
}

int main(int argc, char **argv)
{
    AddYield(argv[1], argv[2]);
    return 0;
}
