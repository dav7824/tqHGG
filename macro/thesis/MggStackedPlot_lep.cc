#include "include/Paths.h"
#include "include/utility.h"

// Set input dir name
TString indir_name = "Presel_lep_phID";
// Use log scale on y axis
bool use_logy = true;

vector<TString> nt
{
    "TT_FCNC-TtoHJ_aTlep_hct",
    "TT_FCNC-aTtoHJ_Tlep_hct",
    "TT_FCNC-TtoHJ_aTlep_hut",
    "TT_FCNC-aTtoHJ_Tlep_hut",
    "ST_FCNC-TH_Tlep_hct",
    "ST_FCNC-TH_Tlep_hut",
    "DiPhotonJetsBox",
    "GJet_Pt-20to40_MGG-80toInf",
    "GJet_Pt-20toInf_MGG-40to80",
    "GJet_Pt-40toInf_MGG-80toInf",
    "QCD_Pt-30toInf_MGG-40to80",
    "QCD_Pt-40toInf_MGG-80toInf",
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
    "GJet",
    "QCD",
    "TTGG",
    "TTGJets",
    "TTJets",
    "DY",
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
    "#gamma+jets",
    "QCD",
    "t#bar{t}#gamma#gamma",
    "t#bar{t}#gamma+jets",
    "t#bar{t}+jets",
    "DY",
    "VG",
    "SM Higgs",
    "Others",
};
vector<int> color_bkg
{
    kCyan-10, kCyan+1, kBlue-9, kAzure+1, kBlue-7, kViolet+6, kYellow, kOrange, kMagenta-10, kRed-7
};
vector<int> color_sig
{
    kRed, kBlack, kRed, kBlack,
};
vector<int> style_sig
{
    1, 1, 7, 7,
};

void MggStackedPlot_lep()
{
    // Set paths
    TString indir = dir_res + indir_name + "/hist/";
    TString normdir = dir_res + "normfactor/";
    if (gSystem->AccessPathName(indir) || gSystem->AccessPathName(normdir)) {
        cout << "[ERROR] Input dirs do not exist!\n";
        exit(1);
    }

    int Nsig = cat_sig.size();
    int Nbkg = cat_bkg.size();

    // Get input files
    cout << "---Reading files\n";
    map<TString, TFile*> fins;
    for (int i=0; i<nt.size(); ++i)
        fins[nt.at(i)] = new TFile(indir + nt.at(i) + ".root");
    // Get histograms
    map<TString, TH1D*> hsample;
    for (int i=0; i<nt.size(); ++i)
        hsample[nt.at(i)] = (TH1D*)fins[nt.at(i)]->Get("dipho_mass");
    TH1D *hdata = hsample["data"];

    // Blind data histogram
    hdata->SetBinContent(3, 0);

    // Normalize histograms to lumiosity
    for (int i=0; i<nt.size(); ++i) {
        if (nt.at(i) == "data")
            continue;
        float nf = GetNormFactor( (normdir+nt.at(i)+".root").Data() );
        hsample[nt.at(i)]->Scale(nf);
    }

    // Combine bkg categories
    cout << "---Combining histograms\n";
    map<TString, TH1D*> hsig;
    map<TString, TH1D*> hbkg;
    // TT_hct
    hsig["TT_hct"] = (TH1D*)hsample["TT_FCNC-TtoHJ_aTlep_hct"]->Clone();
    hsig["TT_hct"]->Add( hsample["TT_FCNC-aTtoHJ_Tlep_hct"] );
    // TT_hut
    hsig["TT_hut"] = (TH1D*)hsample["TT_FCNC-TtoHJ_aTlep_hut"]->Clone();
    hsig["TT_hut"]->Add( hsample["TT_FCNC-aTtoHJ_Tlep_hut"] );
    // ST_hct
    hsig["ST_hct"] = (TH1D*)hsample["ST_FCNC-TH_Tlep_hct"]->Clone();
    // ST_hut
    hsig["ST_hut"] = (TH1D*)hsample["ST_FCNC-TH_Tlep_hut"]->Clone();
    // DiPhotonJetsBox
    hbkg["DiPhotonJetsBox"] = (TH1D*)hsample["DiPhotonJetsBox"]->Clone();
    // GJet
    hbkg["GJet"] = (TH1D*)hsample["GJet_Pt-20to40_MGG-80toInf"]->Clone();
    hbkg["GJet"]->Add( hsample["GJet_Pt-20toInf_MGG-40to80"] );
    hbkg["GJet"]->Add( hsample["GJet_Pt-40toInf_MGG-80toInf"] );
    // QCD
    hbkg["QCD"] = (TH1D*)hsample["QCD_Pt-30toInf_MGG-40to80"]->Clone();
    hbkg["QCD"]->Add( hsample["QCD_Pt-40toInf_MGG-80toInf"] );
    // TTGG
    hbkg["TTGG"] = (TH1D*)hsample["TTGG"]->Clone();
    // TTGJets
    hbkg["TTGJets"] = (TH1D*)hsample["TTGJets"]->Clone();
    // TTJets
    hbkg["TTJets"] = (TH1D*)hsample["TTJets"]->Clone();
    // DY
    hbkg["DY"] = (TH1D*)hsample["DYJetsToLL"]->Clone();
    // VG
    hbkg["VG"] = (TH1D*)hsample["WGToLNuG"]->Clone();
    hbkg["VG"]->Add( hsample["ZGToLLG"] );
    // SM Higgs
    hbkg["Higgs"] = (TH1D*)hsample["GluGluHToGG"]->Clone();
    hbkg["Higgs"]->Add( hsample["VBFHToGG"] );
    hbkg["Higgs"]->Add( hsample["VHToGG"] );
    hbkg["Higgs"]->Add( hsample["ttHJetToGG"] );
    // Others
    hbkg["Others"] = (TH1D*)hsample["TGJets"]->Clone();
    hbkg["Others"]->Add( hsample["tW_antitop"] );
    hbkg["Others"]->Add( hsample["tW_top"] );
    hbkg["Others"]->Add( hsample["TTWJetsToLNu"] );
    hbkg["Others"]->Add( hsample["TTZToLLNuNu"] );
    hbkg["Others"]->Add( hsample["WW"] );
    hbkg["Others"]->Add( hsample["WZ"] );
    hbkg["Others"]->Add( hsample["ZZ"] );
    // Total bkg
    TH1D *htotbkg = (TH1D*)hsample["DiPhotonJetsBox"]->Clone();
    htotbkg->Add( hsample["GJet_Pt-20to40_MGG-80toInf"] );
    htotbkg->Add( hsample["GJet_Pt-20toInf_MGG-40to80"] );
    htotbkg->Add( hsample["GJet_Pt-40toInf_MGG-80toInf"] );
    htotbkg->Add( hsample["QCD_Pt-30toInf_MGG-40to80"] );
    htotbkg->Add( hsample["QCD_Pt-40toInf_MGG-80toInf"] );
    htotbkg->Add( hsample["TGJets"] );
    htotbkg->Add( hsample["tW_antitop"] );
    htotbkg->Add( hsample["tW_top"] );
    htotbkg->Add( hsample["TTGG"] );
    htotbkg->Add( hsample["TTGJets"] );
    htotbkg->Add( hsample["TTJets"] );
    htotbkg->Add( hsample["TTWJetsToLNu"] );
    htotbkg->Add( hsample["TTZToLLNuNu"] );
    htotbkg->Add( hsample["DYJetsToLL"] );
    htotbkg->Add( hsample["WGToLNuG"] );
    htotbkg->Add( hsample["ZGToLLG"] );
    htotbkg->Add( hsample["WW"] );
    htotbkg->Add( hsample["WZ"] );
    htotbkg->Add( hsample["ZZ"] );
    htotbkg->Add( hsample["GluGluHToGG"] );
    htotbkg->Add( hsample["VBFHToGG"] );
    htotbkg->Add( hsample["VHToGG"] );
    htotbkg->Add( hsample["ttHJetToGG"] );

    // Set histogram stack
    THStack *hs;
    hs = new THStack(htotbkg->GetName(), "");
    hs->Add( hbkg["Higgs"] );
    hs->Add( hbkg["TTGG"] );
    hs->Add( hbkg["TTGJets"] );
    hs->Add( hbkg["TTJets"] );
    hs->Add( hbkg["VG"] );
    hs->Add( hbkg["DY"] );
    hs->Add( hbkg["Others"] );
    hs->Add( hbkg["DiPhotonJetsBox"] );
    hs->Add( hbkg["GJet"] );
    hs->Add( hbkg["QCD"] );

    // Make data over MC hist
    TH1D *hdom;
    TH1D *hdom_err;
    hdom = (TH1D*)hdata->Clone();
    hdom->Divide(htotbkg);
    hdom_err = (TH1D*)hdata->Clone();
    for (int j=1; j<=hdom->GetNbinsX(); ++j) {
        hdom->SetBinError(j, hdata->GetBinError(j)/htotbkg->GetBinContent(j));
        hdom_err->SetBinContent(j, 1);
        hdom_err->SetBinError(j, htotbkg->GetBinError(j)/htotbkg->GetBinContent(j));
    }

    // Set legend
    TLegend *leg = new TLegend(0.4, 0.62, 0.89, 0.86);
    leg->SetNColumns(3);
    leg->SetLineWidth(0);
    leg->SetFillColorAlpha(0, 0);
    for (int i=0; i<Nsig; ++i)
        leg->AddEntry( hsig[cat_sig.at(i)], legname_sig.at(i), "l" );
    for (int i=0; i<Nbkg; ++i)
        leg->AddEntry( hbkg[cat_bkg.at(i)], legname_bkg.at(i), "f" );
    leg->AddEntry(htotbkg, "Stat. err", "f");
    leg->AddEntry(hdata, "data", "p");

    // Set histogram style
    // data
    hdata->SetMarkerStyle(20);
    hdata->SetMarkerSize(0.6);
    // total bkg
    htotbkg->SetFillColor(kBlack);
    htotbkg->SetFillStyle(3344);
    htotbkg->SetMarkerStyle(0);
    // sig MC
    for (int j=0; j<Nsig; ++j) {
        hsig[cat_sig.at(j)]->SetLineColor(color_sig[j]);
        hsig[cat_sig.at(j)]->SetLineStyle(style_sig[j]);
        hsig[cat_sig.at(j)]->SetLineWidth(2);
    }
    // bkg MC
    for (int j=0; j<Nbkg; ++j)
        hbkg[cat_bkg.at(j)]->SetFillColor(color_bkg[j]);
    // DOM
    hdom->SetMarkerStyle(20);
    hdom->SetMarkerSize(0.6);
    // DOM err
    hdom_err->SetFillColorAlpha(kBlack, 0.4);
    hdom_err->SetMarkerStyle(0);

    // TLine for making plots
    TLine *lin = new TLine();
    lin->SetLineColor(kBlack);
    lin->SetLineWidth(1);

    // Draw histogram
    // Create & set canvas / pads
    cout << "---Making plots\n";
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
    max.push_back(hdata->GetMaximum());
    max.push_back(htotbkg->GetMaximum());
    for (int j=0; j<Nsig; ++j)
        max.push_back( hsig[cat_sig.at(j)]->GetMaximum() );
    double ymax = GetMaximum(max.data(), max.size());
    ymax *= use_logy ? 1000 : 1.4;
    hs->SetMaximum(ymax);
    // Set y-axis minimum
    double ymin = use_logy ? 0.1 : 0;
    hs->SetMinimum(ymin);

    // Set pad margins
    pad1->SetRightMargin(0.05);
    pad1->SetBottomMargin(0);
    pad2->SetRightMargin(0.05);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.2);

    // Draw stacked plot
    pad1->cd();
    hs->Draw("hist");
    hs->GetYaxis()->SetTitle( hdata->GetYaxis()->GetTitle() );
    for (int j=0; j<Nsig; ++j)
        hsig[cat_sig.at(j)]->Draw("hist same");
    htotbkg->Draw("e2 same");
    hdata->Draw("p same");
    leg->Draw();
    // Latex
    TLatex latex;
    latex.SetTextAlign(10);
    latex.DrawLatexNDC(.2, .91, "CMS #bf{Preliminary}");
    latex.DrawLatexNDC(.15, .75, "leptonic");
    latex.SetTextAlign(30);
    latex.DrawLatexNDC(0.95, .91, "#bf{41.5 fb^{-1} (2017, 13TeV)}");

    // Draw dom plot
    hdom_err->GetYaxis()->SetRangeUser(0.1, 1.9);
    hdom_err->SetYTitle("data/MC");
    hdom_err->SetStats(false);
    pad2->cd();
    hdom_err->Draw("e2");
    lin->DrawLine(hdom_err->GetXaxis()->GetXmin(), 1, hdom_err->GetXaxis()->GetXmax(), 1);
    hdom->Draw("p same");
    // Addtional adjustments
    hdom_err->GetXaxis()->SetLabelSize(0.07);
    hdom_err->GetXaxis()->SetTitleSize(0.1);
    hdom_err->GetXaxis()->SetTitleOffset(0.8);
    hdom_err->GetYaxis()->SetLabelSize(0.07);
    hdom_err->GetYaxis()->SetTitleSize(0.1);
    hdom_err->GetYaxis()->SetTitleOffset(0.5);
    hs->GetYaxis()->SetLabelSize(0.03);
    hs->GetYaxis()->SetTitleSize(0.04);
    hs->GetYaxis()->SetTitleOffset(1.2);

    c->SaveAs("dipho_mass_blind_lep.png");

    delete pad1;
    delete pad2;
    delete c;

    for (map<TString,TFile*>::iterator it=fins.begin(); it!=fins.end(); ++it)
        it->second->Close();
}
