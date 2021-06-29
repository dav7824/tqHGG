#include "include/Paths.h"

void MVAreco_eff()
{
    int Nfile = 4;
    TString indir = dir_res + "MVAreco_model/";
    TFile *fin[Nfile];
    TH1F *hh[Nfile];
    fin[0] = new TFile( indir+"result_TThad.root" );
    fin[1] = new TFile( indir+"result_SThad.root" );
    fin[2] = new TFile( indir+"result_TTlep.root" );
    fin[3] = new TFile( indir+"result_STlep.root" );
    for (int i=0; i<Nfile; ++i)  hh[i] = (TH1F*)fin[i]->Get("dataset/Method_ANN/ANN/MVA_ANN_effBvsS");

    hh[0]->SetLineColor(kRed);
    hh[1]->SetLineColor(kOrange);
    hh[2]->SetLineColor(kCyan);
    hh[3]->SetLineColor(kBlue);

    TLegend *leg = new TLegend(0.16, 0.65, 0.4, 0.85);
    leg->AddEntry(hh[0], "TT hadronic", "l");
    leg->AddEntry(hh[1], "ST hadronic", "l");
    leg->AddEntry(hh[2], "TT leptonic", "l");
    leg->AddEntry(hh[3], "ST leptonic", "l");

    TCanvas *c = new TCanvas();
    c->SetLogy(true);
    hh[3]->SetStats(false);
    hh[3]->Draw("l");
    hh[2]->Draw("lsame");
    hh[1]->Draw("lsame");
    hh[0]->Draw("lsame");
    leg->Draw();

    c->SaveAs(indir+"MVAreco_eff.png");
}
