#include "TFile.h"
#include "TH1D.h"

#include <iostream>
using namespace std;

void CalcYield(const char *input, const char *normfile, const char *output)
{
    cout << "Processing n-tuple: " << input << endl;

    TFile *fin = new TFile(input);
    TH1D *hsumweight = (TH1D*)fin->Get("sum_genweight");
    TH1D *hsumweight2 = (TH1D*)fin->Get("sum_genweight2");

    TFile *fnorm = new TFile(normfile);
    TH1D *hnorm = (TH1D*)fnorm->Get("normfactor");

    double sum_genweight = hsumweight->GetBinContent(1);
    double sum_genweight2 = hsumweight2->GetBinContent(1);
    double normfactor = hnorm->GetBinContent(1);

    sum_genweight *= normfactor;
    sum_genweight2 *= normfactor * normfactor;

    TFile *fout = new TFile(output, "recreate");
    TH1D *hyield = new TH1D("yield", "", 1, 0., 1.);
    TH1D *herr2 = new TH1D("yield_err2", "", 1, 0., 1.);
    hyield->SetBinContent(1, sum_genweight);
    herr2->SetBinContent(1, sum_genweight2);
    hyield->Write();
    herr2->Write();

    fout->Close();
    fnorm->Close();
    fin->Close();

    cout << "Output saved at: " << output << endl;
}

int main(int argc, char **argv)
{
    CalcYield(argv[1], argv[2], argv[3]);
    return 0;
}
