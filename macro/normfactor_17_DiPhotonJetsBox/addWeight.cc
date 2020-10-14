#include "TString.h"
#include "TFile.h"
#include "TH1D.h"

#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
    TString fin_name = argv[1];
    cout << "Processing: " << fin_name << endl;

    TFile *fin = new TFile(fin_name);
    TH1D *hin = (TH1D*)fin->Get("flashggNtuples/h1_weight");

    TFile *fout = new TFile("SumWeight_DiPhotonJetsBox.root", "update");
    TH1D *hout = (TH1D*)fout->Get("weight");

    double res = hout->GetBinContent(1) + hin->GetBinContent(1);
    hout->SetBinContent(1, res);

    fout->WriteTObject(hout, "", "Overwrite");
    fout->Close();
    fin->Close();

    return 0;
}
