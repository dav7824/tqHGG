// Usage:
//   ./HistPlotter <bkg_file_list> <bkg_legend_list> <total_bkg_file_path> <sig_file_list> <sig_legend_list> \
//                 <data_file_path> <bkg_directory> <sig_directory> <output_dir> <channel> <plot_dom> <use_log_y>
//
//   <bkg_file_list>:           Names of bkg histogram files, without '.root' suffix. (separated by ',')
//   <bkg_legend_list>:         Names of bkg legend names. (separated by ',')
//   <total_bkg_file_path>:     Path of the file of total MC bkg.
//   <sig_file_list>:           Names of sig histogram files, without '.root' suffix. (separated by ',')
//                              Pass 'none' if you don't want to draw signal histograms.
//   <sig_legend_list>:         Names of sig legend names. (separated by ',')
//   <data_file_path>:          Path of the file of data.
//   <bkg_directory>:           Path of the directory containing all bkg files
//   <sig_directory>:           Path of the directory containing all sig files
//   <output_dir>:              Path of the directory of output files.
//   <channel>:                 Specify channel (for drawing Latex)
//   <plot_dom>:                Whether make data/MC plots or not.
//   <use_log_y>:               Whether use log scale for y-axis or not.

#include "include/utility.h"

#include "TString.h"
#include "TFile.h"
#include "TH1D.h"
#include "THStack.h"
#include "TKey.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TLine.h"
#include "TLatex.h"

#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

vector<int> bkg_color = {
    kCyan-10, kCyan+1, kBlue-9, kAzure+1, kBlue-7, kViolet+6, kMagenta-9, kMagenta-10, kRed-7, kOrange+1, kOrange-9,
    kYellow-7, kSpring+9, kGreen-7, kGreen-6,
};
vector<int> sig_color = {
    kRed, kBlack, kRed, kBlack,
};
vector<int> sig_style = {
    1, 1, 7, 7,
};

void HistPlotter(TString fbkg_list_, TString legbkg_list_, TString ftotbkg_name, TString fsig_list_, TString legsig_list_,
        TString fdata_name, TString indir_bkg, TString indir_sig, TString outdir, TString ch, TString plot_dom_, TString use_logy_)
{
    bool plot_dom = atoi(plot_dom_.Data());
    bool use_logy = atoi(use_logy_.Data());
    bool draw_sig = false;

    vector<TString> fbkg_list, legbkg_list;
    ParseCStringList(fbkg_list_.Data(), fbkg_list);
    ParseCStringList(legbkg_list_.Data(), legbkg_list);
    int Nbkgs = fbkg_list.size();
    vector<TString> fsig_list, legsig_list;
    if (fsig_list_!="none") {
        draw_sig = true;
        ParseCStringList(fsig_list_.Data(), fsig_list);
        ParseCStringList(legsig_list_.Data(), legsig_list);
    }
    int Nsig = fsig_list.size();

    // Open files
    TFile *fdata = new TFile(fdata_name);
    TFile *ftotbkg = new TFile(ftotbkg_name);
    vector<TFile*> fbkgs(Nbkgs);
    for (int i=0; i<Nbkgs; ++i) fbkgs[i] = new TFile(indir_bkg+"/"+fbkg_list[i]+".root");
    vector<TFile*> fsig(Nsig);
    if (draw_sig)
        for (int i=0; i<Nsig; ++i)
            fsig[i] = new TFile(indir_sig+"/"+fsig_list[i]+".root");

    // Get histograms
    vector<TH1D*> hist_data;
    vector<TH1D*> hist_totbkg;
    vector<vector<TH1D*>> hist_bkgs(Nbkgs);
    vector<vector<TH1D*>> hist_sig(Nsig);
    TKey *key = 0;
    TIter nextkey(fdata->GetListOfKeys());
    while (( key=(TKey*)nextkey() )) {
        const char *histname = key->GetName();
        hist_data.push_back( (TH1D*)fdata->Get(histname) );
        hist_totbkg.push_back( (TH1D*)ftotbkg->Get(histname) );
        for (int i=0; i<Nbkgs; ++i) hist_bkgs[i].push_back( (TH1D*)fbkgs[i]->Get(histname) );
        if (draw_sig)
            for (int i=0; i<Nsig; ++i) hist_sig[i].push_back( (TH1D*)fsig[i]->Get(histname) );
    }
    int Nhist = hist_data.size();

    // Create bkg THStack
    vector<THStack*> hstack_bkgs(Nhist);
    for (int i=0; i<Nhist; ++i) {
        hstack_bkgs[i] = new THStack(hist_data[i]->GetName(), "");
        for (int j=0; j<Nbkgs; ++j) {
            if (j < bkg_color.size()) hist_bkgs[j][i]->SetFillColor(bkg_color[j]);    // Set hist color for each bkg MC
            hstack_bkgs[i]->Add(hist_bkgs[j][i]);
        }
    }

    // Create legend
    TLegend *leg = new TLegend(0.35, 0.65, 0.89, 0.89);
    leg->SetNColumns(3);
    leg->SetLineWidth(0);
    leg->SetFillColorAlpha(0, 0);
    for (int i=0; i<Nbkgs; ++i) leg->AddEntry(hist_bkgs[i][0], legbkg_list[i], "f");
    leg->AddEntry(hist_totbkg[0], "tot bkg", "f");
    leg->AddEntry(hist_data[0], "data", "p");
    if (draw_sig)
        for (int i=0; i<Nsig; ++i) leg->AddEntry(hist_sig[i][0], legsig_list[i], "l");

    // Create dom (data over MC) histograms
    vector<TH1D*> hist_dom(Nhist);
    vector<TH1D*> hist_dom_err(Nhist);
    if (plot_dom)
        for (int i=0; i<Nhist; ++i) {
            hist_dom[i] = (TH1D*)hist_data[i]->Clone();
            hist_dom[i]->Divide(hist_totbkg[i]);
            hist_dom_err[i] = (TH1D*)hist_data[i]->Clone();
            for (int j=1; j<=hist_dom[i]->GetNbinsX(); ++j) {
                hist_dom[i]->SetBinError(j, hist_data[i]->GetBinError(j)/hist_totbkg[i]->GetBinContent(j));
                hist_dom_err[i]->SetBinContent(j, 1);
                hist_dom_err[i]->SetBinError(j, hist_totbkg[i]->GetBinError(j)/hist_totbkg[i]->GetBinContent(j));
            }
        }

    // Set color & style of histograms
    for (int i=0; i<Nhist; ++i) {
        // Data
        hist_data[i]->SetMarkerStyle(20);
        hist_data[i]->SetMarkerSize(0.6);
        // Total bkg MC
        hist_totbkg[i]->SetFillColorAlpha(kBlack, 0.4);
        hist_totbkg[i]->SetMarkerStyle(0);
        // Sig MC
        if (draw_sig)
            for (int j=0; j<Nsig; ++j) {
                if (j < sig_color.size()) {
                    hist_sig[j][i]->SetLineColor(sig_color[j]);
                    hist_sig[j][i]->SetLineStyle(sig_style[j]);
                }
                hist_sig[j][i]->SetLineWidth(2);
            }
        // DOM
        if (plot_dom) {
            // dom
            hist_dom[i]->SetMarkerStyle(20);
            hist_dom[i]->SetMarkerSize(0.6);
            // error
            hist_dom_err[i]->SetFillColorAlpha(kBlack, 0.4);
            hist_dom_err[i]->SetMarkerStyle(0);
        }
    }

    // TLine for making plots
    TLine *lin = new TLine();
    lin->SetLineColor(kBlack);
    lin->SetLineWidth(1);

    // Start loop to draw stacked plots
    for (int i=0; i<Nhist; ++i)
    {
        // Create & set canvas / pads
        TCanvas *c = 0;
        TPad *pad1 = 0, *pad2 = 0;
        if (plot_dom) {
            c = new TCanvas("c", "c", 600, 700);
            pad1 = new TPad("pad1", "pad1", 0, 0.27, 1, 1);
            pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.27);
            pad1->Draw();
            pad2->Draw();
        }
        else {
            c = new TCanvas("c", "c", 600, 500);
            pad1 = c;
        }
        pad1->SetTicks();
        pad1->SetGrid();
        if (use_logy) pad1->SetLogy();
        if (plot_dom) {
            pad2->SetGrid();
            pad2->SetTicks();
        }

        // Set y-axis maximum
        vector<double> max;
        max.push_back(hist_data[i]->GetMaximum());
        max.push_back(hist_totbkg[i]->GetMaximum());
        if (draw_sig)
            for (int j=0; j<Nsig; ++j) max.push_back(hist_sig[j][i]->GetMaximum());
        double ymax = GetMaximum(max.data(), max.size());
        ymax *= use_logy ? 1000 : 1.4;
        hstack_bkgs[i]->SetMaximum(ymax);
        // Set y-axis minimum
        double ymin = use_logy ? 0.1 : 0;
        hstack_bkgs[i]->SetMinimum(ymin);

        // Set pad margins
        if (plot_dom) {
            pad1->SetRightMargin(0.05);
            pad1->SetBottomMargin(0);
            pad2->SetRightMargin(0.05);
            pad2->SetTopMargin(0);
            pad2->SetBottomMargin(0.2);
        }

        // Draw stacked plot
        pad1->cd();
        hstack_bkgs[i]->Draw("hist");
        hstack_bkgs[i]->GetYaxis()->SetTitle( hist_data[i]->GetYaxis()->GetTitle() );
        if (draw_sig)
            for (int j=0; j<Nsig; ++j)
                hist_sig[j][i]->Draw("hist same");
        hist_totbkg[i]->Draw("e2 same");
        hist_data[i]->Draw("p same");
        leg->Draw();
        // Latex
        double x_right = 0;
        if (plot_dom) x_right = 0.95;
        else x_right = 0.9;
        TLatex latex;
        latex.SetTextAlign(10);
        latex.DrawLatexNDC(.2, .91, "CMS #bf{Preliminary}");
        if (ch == "had")
            latex.DrawLatexNDC(.15, .75, "hadronic");
        else if (ch == "lep")
            latex.DrawLatexNDC(.15, .75, "leptonic");
        latex.SetTextAlign(30);
        latex.DrawLatexNDC(x_right, .91, "#bf{41.5 fb^{-1} (2017, 13TeV)}");

        // Draw dom plot
        if (plot_dom) {
            hist_dom_err[i]->GetYaxis()->SetRangeUser(0.1, 1.9);
            hist_dom_err[i]->SetYTitle("data/MC");
            hist_dom_err[i]->SetStats(false);
            pad2->cd();
            hist_dom_err[i]->Draw("e2");
            lin->DrawLine(hist_dom_err[i]->GetXaxis()->GetXmin(), 1, hist_dom_err[i]->GetXaxis()->GetXmax(), 1);
            hist_dom[i]->Draw("p same");
            // Addtional adjustments
            hist_dom_err[i]->GetXaxis()->SetLabelSize(0.07);
            hist_dom_err[i]->GetXaxis()->SetTitleSize(0.1);
            hist_dom_err[i]->GetXaxis()->SetTitleOffset(0.8);
            hist_dom_err[i]->GetYaxis()->SetLabelSize(0.07);
            hist_dom_err[i]->GetYaxis()->SetTitleSize(0.1);
            hist_dom_err[i]->GetYaxis()->SetTitleOffset(0.5);
            hstack_bkgs[i]->GetYaxis()->SetLabelSize(0.03);
            hstack_bkgs[i]->GetYaxis()->SetTitleSize(0.04);
            hstack_bkgs[i]->GetYaxis()->SetTitleOffset(1.2);
        }

        c->SaveAs(outdir+"/"+hist_data[i]->GetName()+".png");

        // Clean up
        /*
         * Small discovery:
         * If you delete pads after deleting the canvas that contains them, there will be segmentation fault.
         * So delete the pads first, then the canvas, as done here.
         * But it's probable that by deleting the canvas the pads are also cleaned? So the mentioned segmentation
         * fault is caused by repeated deletion of the pads.
         */
        if (plot_dom) {
            delete pad1;
            delete pad2;
        }
        delete c;
    } // End of plot-drawing loop

    fdata->Close();
    ftotbkg->Close();
    for (int i=0; i<Nbkgs; ++i) fbkgs[i]->Close();
    if (draw_sig)
        for (int i=0; i<Nsig; ++i) fsig[i]->Close();

    cout << "Completed!\n";
}

int main(int argc, char** argv)
{
    TString fbkg_list__ = argv[1];
    TString legbkg_list__ = argv[2];
    TString ftotbkg_name_ = argv[3];
    TString fsig_list__ = argv[4];
    TString legsig_list__ = argv[5];
    TString fdata_name_ = argv[6];
    TString indir_bkg_ = argv[7];
    TString indir_sig_ = argv[8];
    TString outdir_ = argv[9];
    TString ch_ = argv[10];
    TString plot_dom__ = argv[11];
    TString use_logy__ = argv[12];

    HistPlotter(fbkg_list__, legbkg_list__, ftotbkg_name_, fsig_list__, legsig_list__,
            fdata_name_, indir_bkg_, indir_sig_, outdir_, ch_, plot_dom__, use_logy__);

    return 0;
}
