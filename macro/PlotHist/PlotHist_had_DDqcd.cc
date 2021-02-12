#include "include/Paths.h"
#include "include/utility.h"

// Set input dir name
TString indir_name = "Presel_had_phID_btag-L";
// Use log scale on y axis
bool use_logy = true;

vector<TString> nt
{
    "TT_FCNC-TtoHJ_aThad_hct",
    "TT_FCNC-aTtoHJ_Thad_hct",
    "TT_FCNC-TtoHJ_aThad_hut",
    "TT_FCNC-aTtoHJ_Thad_hut",
    "ST_FCNC-TH_Thad_hct",
    "ST_FCNC-TH_Thad_hut",
    "DiPhotonJetsBox",
    "Data-driven_QCD",
    "TGJets",
    "tW_antitop",
    "tW_top",
    "TTGG",
    "TTGJets",
    "TTJets",
    "TTWJetsToLNu",
    "TTZToLLNuNu",
    "DYJetsToLL",
    "WGToLNuG",
    "ZGToLLG",
    "WW",
    "WZ",
    "ZZ",
    "GluGluHToGG",
    "VBFHToGG",
    "VHToGG",
    "ttHJetToGG",
    "data",
};
vector<TString> cat_sig
{
    "TT_hct",
    "TT_hut",
    "ST_hct",
    "ST_hut",
};
vector<TString> cat_bkg
{
    "DiPhotonJetsBox",
    "GJet_QCD",
    "TTGG",
    "TTGJets",
    "TTJets",
    "VG",
    "Higgs",
    "Others",
};
vector<TString> legname_sig
{
    "TT Hct",
    "TT Hut",
    "ST Hct",
    "ST Hut",
};
vector<TString> legname_bkg
{
    "#gamma#gamma+jets",
    "(#gamma)+jets",
    "t#bar{t}#gamma#gamma",
    "t#bar{t}#gamma+jets",
    "t#bar{t}+jets",
    "VG",
    "SM Higgs",
    "Others",
};
vector<int> color_bkg
{
    kCyan-10, kCyan, kAzure+1, kBlue-7, kViolet+6, kOrange, kMagenta-10, kRed-7
};
vector<int> color_sig
{
    kRed, kBlack, kRed, kBlack,
};
vector<int> style_sig
{
    1, 1, 7, 7,
};

void AddHisto(vector<TH1D*> &acc, vector<TH1D*> &added)
{
    for (int i=0; i<acc.size(); ++i)
        acc[i]->Add(added[i]);
}
void CloneHisto(vector<TH1D*> &copy, vector<TH1D*> &paste)
{
    paste.clear();
    for (int i=0; i<copy.size(); ++i)
        paste.push_back( (TH1D*)copy[i]->Clone() );
}


void PlotHist_had_DDqcd()
{
    // Set paths
    TString indir = dir_res + indir_name + "/hist/";
    TString outdir = dir_res + indir_name + "/plots_DDqcd/";
    TString normdir = dir_res + "normfactor/";
    if (gSystem->AccessPathName(indir) || gSystem->AccessPathName(normdir)) {
        cout << "[ERROR] Input dirs do not exist!\n";
        exit(1);
    }
    if (gSystem->AccessPathName(outdir))
        gSystem->mkdir(outdir);

    int Nsig = cat_sig.size();
    int Nbkg = cat_bkg.size();

    // Get input files
    map<TString, TFile*> fins;
    for (int i=0; i<nt.size(); ++i)
        fins[nt.at(i)] = new TFile(indir + nt.at(i) + ".root");
    // Get histograms
    map<TString, vector<TH1D*>> hsample;
    TKey *key;
    TIter nextkey( fins[nt.at(0)]->GetListOfKeys() );
    while (( key=(TKey*)nextkey() )) {
        const char *hname = key->GetName();
        for (int i=0; i<nt.size(); ++i)
            hsample[nt.at(i)].push_back( (TH1D*)fins[nt.at(i)]->Get(hname) );
    }
    vector<TH1D*> &hdata = hsample["data"];

    // Normalize histograms to lumiosity
    for (int i=0; i<nt.size(); ++i) {
        if (nt.at(i) == "data" || nt.at(i) == "Data-driven_QCD")
            continue;
        float nf = GetNormFactor( (normdir+nt.at(i)+".root").Data() );
        for (int j=0; j<hsample[nt.at(i)].size(); ++j)
            hsample[nt.at(i)][j]->Scale(nf);
    }

    // Combine bkg categories
    map<TString, vector<TH1D*>> hsig;
    map<TString, vector<TH1D*>> hbkg;
    // TT_hct
    CloneHisto( hsample["TT_FCNC-TtoHJ_aThad_hct"], hsig["TT_hct"] );
    AddHisto( hsig["TT_hct"], hsample["TT_FCNC-aTtoHJ_Thad_hct"] );
    // TT_hut
    CloneHisto( hsample["TT_FCNC-TtoHJ_aThad_hut"], hsig["TT_hut"] );
    AddHisto( hsig["TT_hut"], hsample["TT_FCNC-aTtoHJ_Thad_hut"] );
    // ST_hct
    CloneHisto( hsample["ST_FCNC-TH_Thad_hct"], hsig["ST_hct"] );
    // ST_hut
    CloneHisto( hsample["ST_FCNC-TH_Thad_hut"], hsig["ST_hut"] );
    // DiPhotonJetsBox
    CloneHisto( hsample["DiPhotonJetsBox"], hbkg["DiPhotonJetsBox"] );
    // Data-driven GJet_QCD
    CloneHisto( hsample["Data-driven_QCD"], hbkg["GJet_QCD"] );
    // TTGG
    CloneHisto( hsample["TTGG"], hbkg["TTGG"] );
    // TTGJets
    CloneHisto( hsample["TTGJets"], hbkg["TTGJets"] );
    // TTJets
    CloneHisto( hsample["TTJets"], hbkg["TTJets"] );
    // VG
    CloneHisto( hsample["WGToLNuG"], hbkg["VG"] );
    AddHisto( hbkg["VG"], hsample["ZGToLLG"] );
    // SM Higgs
    CloneHisto( hsample["GluGluHToGG"], hbkg["Higgs"] );
    AddHisto( hbkg["Higgs"], hsample["VBFHToGG"] );
    AddHisto( hbkg["Higgs"], hsample["VHToGG"] );
    AddHisto( hbkg["Higgs"], hsample["ttHJetToGG"] );
    // Others
    CloneHisto( hsample["TGJets"], hbkg["Others"] );
    AddHisto( hbkg["Others"], hsample["tW_antitop"] );
    AddHisto( hbkg["Others"], hsample["tW_top"] );
    AddHisto( hbkg["Others"], hsample["TTWJetsToLNu"] );
    AddHisto( hbkg["Others"], hsample["TTZToLLNuNu"] );
    AddHisto( hbkg["Others"], hsample["DYJetsToLL"] );
    AddHisto( hbkg["Others"], hsample["WW"] );
    AddHisto( hbkg["Others"], hsample["WZ"] );
    AddHisto( hbkg["Others"], hsample["ZZ"] );
    // Total bkg
    vector<TH1D*> htotbkg;
    CloneHisto( hsample["DiPhotonJetsBox"], htotbkg );
    AddHisto( htotbkg, hsample["Data-driven_QCD"] );
    AddHisto( htotbkg, hsample["TGJets"] );
    AddHisto( htotbkg, hsample["tW_antitop"] );
    AddHisto( htotbkg, hsample["tW_top"] );
    AddHisto( htotbkg, hsample["TTGG"] );
    AddHisto( htotbkg, hsample["TTGJets"] );
    AddHisto( htotbkg, hsample["TTJets"] );
    AddHisto( htotbkg, hsample["TTWJetsToLNu"] );
    AddHisto( htotbkg, hsample["TTZToLLNuNu"] );
    AddHisto( htotbkg, hsample["DYJetsToLL"] );
    AddHisto( htotbkg, hsample["WGToLNuG"] );
    AddHisto( htotbkg, hsample["ZGToLLG"] );
    AddHisto( htotbkg, hsample["WW"] );
    AddHisto( htotbkg, hsample["WZ"] );
    AddHisto( htotbkg, hsample["ZZ"] );
    AddHisto( htotbkg, hsample["GluGluHToGG"] );
    AddHisto( htotbkg, hsample["VBFHToGG"] );
    AddHisto( htotbkg, hsample["VHToGG"] );
    AddHisto( htotbkg, hsample["ttHJetToGG"] );

    int Nhist = htotbkg.size();

    // Set histogram stack
    vector<THStack*> hs(Nhist);
    for (int i=0; i<Nhist; ++i) {
        hs[i] = new THStack(htotbkg[i]->GetName(), "");
        hs[i]->Add( hbkg["Higgs"][i] );
        hs[i]->Add( hbkg["TTGG"][i] );
        hs[i]->Add( hbkg["TTGJets"][i] );
        hs[i]->Add( hbkg["TTJets"][i] );
        hs[i]->Add( hbkg["VG"][i] );
        hs[i]->Add( hbkg["Others"][i] );
        hs[i]->Add( hbkg["DiPhotonJetsBox"][i] );
        hs[i]->Add( hbkg["GJet_QCD"][i] );
    }

    // Make data over MC hist
    vector<TH1D*> hdom(Nhist);
    vector<TH1D*> hdom_err(Nhist);
    for (int i=0; i<Nhist; ++i) {
        hdom[i] = (TH1D*)hdata[i]->Clone();
        hdom[i]->Divide(htotbkg[i]);
        hdom_err[i] = (TH1D*)hdata[i]->Clone();
        for (int j=1; j<=hdom[i]->GetNbinsX(); ++j) {
            hdom[i]->SetBinError(j, hdata[i]->GetBinError(j)/htotbkg[i]->GetBinContent(j));
            hdom_err[i]->SetBinContent(j, 1);
            hdom_err[i]->SetBinError(j, htotbkg[i]->GetBinError(j)/htotbkg[i]->GetBinContent(j));
        }
    }

    // Set legend
    TLegend *leg = new TLegend(0.4, 0.62, 0.89, 0.86);
    leg->SetNColumns(3);
    leg->SetLineWidth(0);
    leg->SetFillColorAlpha(0, 0);
    for (int i=0; i<Nsig; ++i)
        leg->AddEntry( hsig[cat_sig.at(i)][0], legname_sig.at(i), "l" );
    for (int i=0; i<Nbkg; ++i)
        leg->AddEntry( hbkg[cat_bkg.at(i)][0], legname_bkg.at(i), "f" );
    leg->AddEntry(htotbkg[0], "Stat. err", "f");
    leg->AddEntry(hdata[0], "data", "p");

    // Set histogram style
    for (int i=0; i<Nhist; ++i) {
        // data
        hdata[i]->SetMarkerStyle(20);
        hdata[i]->SetMarkerSize(0.6);
        // total bkg
        htotbkg[i]->SetFillColor(kBlack);
        htotbkg[i]->SetFillStyle(3344);
        htotbkg[i]->SetMarkerStyle(0);
        // sig MC
        for (int j=0; j<Nsig; ++j) {
            hsig[cat_sig.at(j)][i]->SetLineColor(color_sig[j]);
            hsig[cat_sig.at(j)][i]->SetLineStyle(style_sig[j]);
            hsig[cat_sig.at(j)][i]->SetLineWidth(2);
        }
        // bkg MC
        for (int j=0; j<Nbkg; ++j)
            hbkg[cat_bkg.at(j)][i]->SetFillColor(color_bkg[j]);
        // DOM
        hdom[i]->SetMarkerStyle(20);
        hdom[i]->SetMarkerSize(0.6);
        // DOM err
        hdom_err[i]->SetFillColorAlpha(kBlack, 0.4);
        hdom_err[i]->SetMarkerStyle(0);
    }

    // TLine for making plots
    TLine *lin = new TLine();
    lin->SetLineColor(kBlack);
    lin->SetLineWidth(1);

    // Start loop to draw stacked plots
    for (int i=0; i<Nhist; ++i)
    {
        // Create & set canvas / pads
        TCanvas *c = new TCanvas("c", "c", 600, 700);
        TPad *pad1 = new TPad("pad1", "pad1", 0, 0.27, 1, 1);
        TPad *pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.27);
        pad1->Draw();
        pad2->Draw();
        pad1->SetTicks();
        pad1->SetGrid();
        if (use_logy) pad1->SetLogy();
        pad2->SetGrid();
        pad2->SetTicks();

        // Set y-axis maximum
        vector<double> max;
        max.push_back(hdata[i]->GetMaximum());
        max.push_back(htotbkg[i]->GetMaximum());
        for (int j=0; j<Nsig; ++j)
            max.push_back( hsig[cat_sig.at(j)][i]->GetMaximum() );
        double ymax = GetMaximum(max.data(), max.size());
        ymax *= use_logy ? 1000 : 1.4;
        hs[i]->SetMaximum(ymax);
        // Set y-axis minimum
        double ymin = use_logy ? 0.1 : 0;
        hs[i]->SetMinimum(ymin);

        // Set pad margins
        pad1->SetRightMargin(0.05);
        pad1->SetBottomMargin(0);
        pad2->SetRightMargin(0.05);
        pad2->SetTopMargin(0);
        pad2->SetBottomMargin(0.2);

        // Draw stacked plot
        pad1->cd();
        hs[i]->Draw("hist");
        hs[i]->GetYaxis()->SetTitle( hdata[i]->GetYaxis()->GetTitle() );
        for (int j=0; j<Nsig; ++j)
            hsig[cat_sig.at(j)][i]->Draw("hist same");
        htotbkg[i]->Draw("e2 same");
        hdata[i]->Draw("p same");
        leg->Draw();
        // Latex
        TLatex latex;
        latex.SetTextAlign(10);
        latex.DrawLatexNDC(.2, .91, "CMS #bf{Preliminary}");
        latex.DrawLatexNDC(.15, .75, "hadronic");
        latex.SetTextAlign(30);
        latex.DrawLatexNDC(0.95, .91, "#bf{41.5 fb^{-1} (2017, 13TeV)}");

        // Draw dom plot
        hdom_err[i]->GetYaxis()->SetRangeUser(0.1, 1.9);
        hdom_err[i]->SetYTitle("data/MC");
        hdom_err[i]->SetStats(false);
        pad2->cd();
        hdom_err[i]->Draw("e2");
        lin->DrawLine(hdom_err[i]->GetXaxis()->GetXmin(), 1, hdom_err[i]->GetXaxis()->GetXmax(), 1);
        hdom[i]->Draw("p same");
        // Addtional adjustments
        hdom_err[i]->GetXaxis()->SetLabelSize(0.07);
        hdom_err[i]->GetXaxis()->SetTitleSize(0.1);
        hdom_err[i]->GetXaxis()->SetTitleOffset(0.8);
        hdom_err[i]->GetYaxis()->SetLabelSize(0.07);
        hdom_err[i]->GetYaxis()->SetTitleSize(0.1);
        hdom_err[i]->GetYaxis()->SetTitleOffset(0.5);
        hs[i]->GetYaxis()->SetLabelSize(0.03);
        hs[i]->GetYaxis()->SetTitleSize(0.04);
        hs[i]->GetYaxis()->SetTitleOffset(1.2);

        c->SaveAs(outdir+hdata[i]->GetName()+".png");

        delete pad1;
        delete pad2;
        delete c;
    } // End of plot-drawing loop

    for (map<TString,TFile*>::iterator it=fins.begin(); it!=fins.end(); ++it)
        it->second->Close();
}
