#include "TString.h"
#include "TFile.h"
#include "TH1D.h"
using namespace std;

int main(int argc, char **argv)
{
    TString facc_name(argv[1]);
    TString fin_name(argv[2]);

    TFile *facc = new TFile(facc_name, "update");
    TH1D *hacc = (TH1D*)facc->Get("hsumwei");

    TFile *fin = new TFile(fin_name);
    TH1D *hin = (TH1D*)fin->Get("flashggNtuples/h1_weight");

    hacc->SetBinContent(1, hacc->GetBinContent(1)+hin->GetBinContent(1));

    facc->WriteTObject(hacc, "", "Overwrite");
    facc->Close();

    return 0;
}
