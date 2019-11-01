#include "utility.h"

#include "TFile.h"
#include "TH1D.h"
#include "THStack.h"
#include "TKey.h"
#include "TCanvas.h"

#include <vector>
#include <string>
using namespace std;

// color of each MC category
vector<int> color = {
    kCyan-10, kCyan+1, kBlue-9, kAzure+1, kBlue-7, kViolet+6, kMagenta-9, kMagenta-10,
    kRed-7, kOrange+1, kOrange-9, kYellow-7, kSpring+9, kGreen-7, kGreen-6,
};

int main(int argc, char **argv)
{
    // get command line arguments
    vector<string> input;    ParseCStringList(argv[1], input);
    char *output = argv[2];

    // get input histograms
    vector<TFile*> fin;
    fin.resize(input.size());
    for (int i=0; i<fin.size(); ++i) {
	fin[i] = new TFile(input[i].c_str());
    }

    // create output file
    TFile *fout = new TFile(output, "recreate");

    // stack histograms
    TKey *key = 0;
    TIter nextkey(fin[0]->GetListOfKeys());
    TCanvas *c = new TCanvas();
    while ((key = (TKey*)nextkey())) {
	const char *hname = key->GetName();
	THStack *hs = new THStack(hname, "");
	for (int i=0; i<fin.size(); ++i) {
	    TH1D *hist = (TH1D*)fin[i]->Get(hname);
	    if (i < color.size()) hist->SetFillColor(color[i]);
	    hs->Add(hist);
	}
	hs->Draw();
	hs->GetXaxis()->SetTitle( ( (TH1D*)fin[0]->Get(hname) )->GetXaxis()->GetTitle() );
	hs->GetYaxis()->SetTitle( ( (TH1D*)fin[0]->Get(hname) )->GetYaxis()->GetTitle() );
	hs->Write();
    }

    fout->Close();
    for (int i=0; i<fin.size(); ++i) fin[i]->Close();

    return 0;
}
