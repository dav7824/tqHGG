#include "include/Paths.h"
#include "include/utility.h"

TString indir = dir_res + "BDT_model/";
TString outdir = dir_res + "BDT_model/plots/";

void PlotBDThad(TString fin_name)
{
    // Read training result
    TFile *fin = new TFile(indir+fin_name);
    TTree *TrainTree = (TTree*)fin->Get("dataset/TrainTree");
    TTree *TestTree = (TTree*)fin->Get("dataset/TestTree");
    // Histograms
    TH1F *hsig_train = new TH1F("hsig_train", "", 40, -0.6, 0.5);
    hsig_train->SetLineColor(kBlue);  hsig_train->SetMarkerStyle(20);  hsig_train->SetMarkerColor(kBlue);  hsig_train->SetMarkerSize(0.9);
    TH1F *hsig_test = new TH1F("hsig_test", "", 40, -0.6, 0.5);  hsig_test->SetFillColor(kBlue-9);
    TH1F *hbkg_train = new TH1F("hbkg_train", "", 40, -0.6, 0.5);
    hbkg_train->SetLineColor(kRed-4);  hbkg_train->SetMarkerStyle(20);  hbkg_train->SetMarkerColor(kRed-4);  hbkg_train->SetMarkerSize(0.9);
    TH1F *hbkg_test = new TH1F("hbkg_test", "", 40, -0.6, 0.5);  hbkg_test->SetFillColor(kRed-4);  hbkg_test->SetFillStyle(3345);

    // Fill histograms
    TrainTree->Draw( "BDT>>hsig_train", "weight*(classID==0)" );
    TestTree->Draw( "BDT>>hsig_test", "weight*(classID==0)" );
    TrainTree->Draw( "BDT>>hbkg_train", "weight*(classID==1)" );
    TestTree->Draw( "BDT>>hbkg_test", "weight*(classID==1)" );

    // Norm hist to 1
    hsig_train->Scale( 1/hsig_train->Integral() );
    hsig_test->Scale( 1/hsig_test->Integral() );
    hbkg_train->Scale( 1/hbkg_train->Integral() );
    hbkg_test->Scale( 1/hbkg_test->Integral() );

    // Set legend
    TLegend *leg = new TLegend(0.11, 0.6, 0.45, 0.89);
    leg->SetLineWidth(0);
    leg->SetFillColorAlpha(0, 0);
    leg->AddEntry(hsig_train, "signal (training)", "ple");
    leg->AddEntry(hbkg_train, "background (training)", "ple");
    leg->AddEntry(hsig_test, "signal (test)", "f");
    leg->AddEntry(hbkg_test, "background (test)", "f");

    // Find hist maxima
    double hist_max[] = {
        hsig_train->GetMaximum(),
        hsig_test->GetMaximum(),
        hbkg_train->GetMaximum(),
        hbkg_test->GetMaximum(),
    };
    double hmax = GetMaximum(hist_max, 4);

    // Draw
    TCanvas *c = new TCanvas();
    hsig_test->SetStats(false);
    hsig_test->SetMaximum(hmax*1.5);
    hsig_test->SetTitle("Hadronic BDT;BDT score;");
    hsig_test->Draw("hist");
    hbkg_test->Draw("histsame");
    hsig_train->Draw("psame");
    hbkg_train->Draw("psame");
    leg->Draw();
    c->SaveAs(outdir+"BDT_score_had.png");

    fin->Close();
}

void PlotBDTlep(TString fin_name)
{
    // Read training result
    TFile *fin = new TFile(indir+fin_name);
    TTree *TrainTree = (TTree*)fin->Get("dataset/TrainTree");
    TTree *TestTree = (TTree*)fin->Get("dataset/TestTree");
    // Histograms
    TH1F *hsig_train = new TH1F("hsig_train", "", 40, -0.5, 0.5);
    hsig_train->SetLineColor(kBlue);  hsig_train->SetMarkerStyle(20);  hsig_train->SetMarkerColor(kBlue);  hsig_train->SetMarkerSize(0.9);
    TH1F *hsig_test = new TH1F("hsig_test", "", 40, -0.5, 0.5);  hsig_test->SetFillColor(kBlue-9);
    TH1F *hbkg_train = new TH1F("hbkg_train", "", 40, -0.5, 0.5);
    hbkg_train->SetLineColor(kRed-4);  hbkg_train->SetMarkerStyle(20);  hbkg_train->SetMarkerColor(kRed-4);  hbkg_train->SetMarkerSize(0.9);
    TH1F *hbkg_test = new TH1F("hbkg_test", "", 40, -0.5, 0.5);  hbkg_test->SetFillColor(kRed-4);  hbkg_test->SetFillStyle(3345);

    // Fill histograms
    TrainTree->Draw( "BDT>>hsig_train", "weight*(classID==0)" );
    TestTree->Draw( "BDT>>hsig_test", "weight*(classID==0)" );
    TrainTree->Draw( "BDT>>hbkg_train", "weight*(classID==1)" );
    TestTree->Draw( "BDT>>hbkg_test", "weight*(classID==1)" );

    // Norm hist to 1
    hsig_train->Scale( 1/hsig_train->Integral() );
    hsig_test->Scale( 1/hsig_test->Integral() );
    hbkg_train->Scale( 1/hbkg_train->Integral() );
    hbkg_test->Scale( 1/hbkg_test->Integral() );

    // Set legend
    TLegend *leg = new TLegend(0.55, 0.6, 0.89, 0.89);
    leg->SetLineWidth(0);
    leg->SetFillColorAlpha(0, 0);
    leg->AddEntry(hsig_train, "signal (training)", "ple");
    leg->AddEntry(hbkg_train, "background (training)", "ple");
    leg->AddEntry(hsig_test, "signal (test)", "f");
    leg->AddEntry(hbkg_test, "background (test)", "f");

    // Find hist maxima
    double hist_max[] = {
        hsig_train->GetMaximum(),
        hsig_test->GetMaximum(),
        hbkg_train->GetMaximum(),
        hbkg_test->GetMaximum(),
    };
    double hmax = GetMaximum(hist_max, 4);

    // Draw
    TCanvas *c = new TCanvas();
    hsig_test->SetStats(false);
    hsig_test->SetMaximum(hmax*1.5);
    hsig_test->SetTitle("Leptonic BDT;BDT score;");
    hsig_test->Draw("hist");
    hbkg_test->Draw("histsame");
    hsig_train->Draw("psame");
    hbkg_train->Draw("psame");
    leg->Draw();
    c->SaveAs(outdir+"BDT_score_lep.png");

    fin->Close();
}

void PlotBDTscore()
{
    PlotBDThad("result_SRhad.root");
    //PlotBDTlep("result_SRlep.root");
}
