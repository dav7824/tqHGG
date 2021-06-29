#include "include/Paths.h"

TH1D *GetHist(TString fin_name)
{
    TFile *fin = new TFile( dir_res+"BDT_model/"+fin_name );
    TH1D *hh = (TH1D*)fin->Get("dataset/Method_BDT/BDT/MVA_BDT_effBvsS");
    hh->SetStats(false);

    return hh;
}

void BDT_eff_v0()
{
    TH1D *hhad = GetHist("result_SRhad.root");
    TH1D *hlep = GetHist("result_SRlep.root");
    hhad->SetLineWidth(2);
    hhad->SetLineColor(kRed);
    hlep->SetLineWidth(2);
    hlep->SetLineColor(kBlue);

    TLegend *leg = new TLegend(0.6, 0.2, 0.85, 0.4);
    leg->AddEntry(hhad, "Hadronic", "l");
    leg->AddEntry(hlep, "Leptonic", "l");

    TCanvas *c = new TCanvas();
    c->SetLogy(true);
    c->SetGrid(true);
    hlep->Draw("hist");
    hhad->Draw("hist same");
    leg->Draw();
    c->SaveAs( dir_res+"thesis/BDT_eff.png" );
}
