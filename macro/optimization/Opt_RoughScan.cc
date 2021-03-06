#include "include/Paths.h"
using namespace RooFit;

TString indir, outdir;
TCanvas *c = 0;
int plot_idx = 0;

std::vector<TString> nt_sig_had = {
    "TT_FCNC-TtoHJ_aThad_hct",
    "TT_FCNC-aTtoHJ_Thad_hct",
    "TT_FCNC-TtoHJ_aThad_hut",
    "TT_FCNC-aTtoHJ_Thad_hut",
    "ST_FCNC-TH_Thad_hct",
    "ST_FCNC-TH_Thad_hut",
};
std::vector<TString> nt_sig_lep = {
    "TT_FCNC-TtoHJ_aTlep_hct",
    "TT_FCNC-aTtoHJ_Tlep_hct",
    "TT_FCNC-TtoHJ_aTlep_hut",
    "TT_FCNC-aTtoHJ_Tlep_hut",
    "ST_FCNC-TH_Tlep_hct",
    "ST_FCNC-TH_Tlep_hut",
};
std::vector<TString> nt_resbkg = {
    "GluGluHToGG",
    "VBFHToGG",
    "VHToGG",
    "ttHJetToGG",
};
std::vector<TString> nt_nrbkg_had = {
    "DiPhotonJetsBox_scaled",
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
};
std::vector<TString> nt_nrbkg_lep = {
    "DiPhotonJetsBox",
    "GJet_Pt-20to40_MGG-80toInf",
    "GJet_Pt-20toInf_MGG-40to80",
    "GJet_Pt-40toInf_MGG-80toInf",
    //"QCD_Pt-30toInf_MGG-40to80",
    //"QCD_Pt-40toInf_MGG-80toInf",
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
};


void FitGaussian(TH1 *hh, float BDT_cut)
{
    // Make RooDataHist
    RooRealVar mgg("mgg", "M_{#gamma#gamma}", 100, 180);
    RooDataHist data("hmgg", "M_{#gamma#gamma}", mgg, hh);

    // Create Mgg frame
    RooPlot *mframe = mgg.frame( Title(Form("BDT cut at %f",BDT_cut)) );
    // Plot hist
    data.plotOn(mframe);

    // Create model
    RooRealVar mu("mu", "mean", 125, 115, 135);
    RooRealVar sigma("sigma", "sigma", 2, 1e-3, 20);
    RooGaussian gaus("gaus", "gaussian pdf", mgg, mu, sigma);
    // Fit to histogram
    gaus.fitTo(data, SumW2Error(true));
    // Plot pdf
    gaus.plotOn(mframe);

    // Save plot
    mframe->Draw();
    c->SaveAs( outdir+Form("mgg_%02d.png",plot_idx) );

    delete mframe;
} // End of function FitGaussian


void FitGaussian2(TH1 *hh, float BDT_cut)
{
    // Make RooDataHist
    RooRealVar mgg("mgg", "M_{#gamma#gamma}", 100, 180);
    RooDataHist data("hmgg", "M_{#gamma#gamma}", mgg, hh);

    // Create Mgg frame
    RooPlot *mframe = mgg.frame( Title(Form("BDT cut at %f",BDT_cut)) );
    // Plot hist
    data.plotOn(mframe);

    // Create model
    RooRealVar mu1("mu1", "mean1", 127, 105, 145);
    RooRealVar sigma1("sigma1", "sigma1", 3, 1e-3, 20);
    RooGaussian gaus1("gaus1", "gaussian pdf 1", mgg, mu1, sigma1);
    RooRealVar mu2("mu2", "mean2", 125, 105, 145);
    RooRealVar sigma2("sigma2", "sigma2", 2, 1e-3, 20);
    RooGaussian gaus2("gaus2", "gaussian pdf 2", mgg, mu2, sigma2);
    RooRealVar N1("N1", "N1", 5, 1e-6, 50);
    RooRealVar N2("N2", "N2", 5, 1e-6, 50);
    RooAddPdf model("model", "composite model", RooArgList(gaus1,gaus2), RooArgList(N1,N2));
    // Fit to histogram
    model.fitTo(data, SumW2Error(true));
    // Plot pdf
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("gaus1"), LineStyle(kDashed), LineColor(kRed));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("gaus2"), LineStyle(kDashed), LineColor(kViolet));

    // Save plot
    mframe->Draw();
    c->SaveAs( outdir+Form("mgg_%02d.png",plot_idx) );

    delete mframe;
} // End of function FitGaussian2


void FitGaussian3(TH1 *hh, float BDT_cut)
{
    // Make RooDataHist
    RooRealVar mgg("mgg", "M_{#gamma#gamma}", 100, 180);
    RooDataHist data("hmgg", "M_{#gamma#gamma}", mgg, hh);

    // Create Mgg frame
    RooPlot *mframe = mgg.frame( Title(Form("BDT cut at %f",BDT_cut)) );
    // Plot hist
    data.plotOn(mframe);

    // Create model
    RooRealVar mu1("mu1", "mean1", 127, 105, 145);
    RooRealVar sigma1("sigma1", "sigma1", 4, 1e-3, 20);
    RooGaussian gaus1("gaus1", "gaussian pdf 1", mgg, mu1, sigma1);
    RooRealVar mu2("mu2", "mean2", 125, 105, 145);
    RooRealVar sigma2("sigma2", "sigma2", 2, 1e-3, 20);
    RooGaussian gaus2("gaus2", "gaussian pdf 2", mgg, mu2, sigma2);
    RooRealVar mu3("mu3", "mean3", 124, 105, 145);
    RooRealVar sigma3("sigma3", "sigma3", 3, 1e-3, 20);
    RooGaussian gaus3("gaus3", "gaussian pdf 3", mgg, mu3, sigma3);
    RooRealVar N1("N1", "N1", 5, 1e-6, 50);
    RooRealVar N2("N2", "N2", 5, 1e-6, 50);
    RooRealVar N3("N3", "N3", 5, 1e-6, 50);
    RooAddPdf model("model", "composite model", RooArgList(gaus1,gaus2,gaus3), RooArgList(N1,N2,N3));
    // Fit to histogram
    model.fitTo(data, SumW2Error(true));
    // Plot pdf
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("gaus1"), LineStyle(kDashed), LineColor(kRed));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("gaus2"), LineStyle(kDashed), LineColor(kViolet));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("gaus3"), LineStyle(kDashed), LineColor(kGreen));

    // Save plot
    mframe->Draw();
    c->SaveAs( outdir+Form("mgg_%02d.png",plot_idx) );

    delete mframe;
} // End of function FitGaussian3


void FitExponential(TH1 *hh, float BDT_cut)
{
    // Make RooDataHist
    RooRealVar mgg("mgg", "M_{#gamma#gamma}", 100, 180);
    RooDataHist data("hmgg", "M_{#gamma#gamma}", mgg, hh);

    // Create Mgg frame
    RooPlot *mframe = mgg.frame( Title(Form("BDT cut at %f",BDT_cut)) );
    // Plot hist
    data.plotOn(mframe);

    // Create model
    RooRealVar tau("tau", "tau", -0.02, -0.5, -1e-6);
    RooExponential exp("exp", "exp", mgg, tau);
    // Fit to histogram
    exp.fitTo(data, SumW2Error(true));
    // Plot pdf
    exp.plotOn(mframe);

    // Save plot
    mframe->Draw();
    c->SaveAs( outdir+Form("mgg_%02d.png",plot_idx) );

    delete mframe;
} // End of function FitExponential


void FitExponential2(TH1 *hh, float BDT_cut)
{
    // Make RooDataHist
    RooRealVar mgg("mgg", "M_{#gamma#gamma}", 100, 180);
    RooDataHist data("hmgg", "M_{#gamma#gamma}", mgg, hh);

    // Create Mgg frame
    RooPlot *mframe = mgg.frame( Title(Form("BDT cut at %f",BDT_cut)) );
    // Plot hist
    data.plotOn(mframe);

    // Create model
    RooRealVar tau1("tau1", "tau1", -0.02, -0.5, -1e-6);
    RooExponential exp1("exp1", "exp1", mgg, tau1);
    RooRealVar tau2("tau2", "tau2", -0.01, -0.5, -1e-6);
    RooExponential exp2("exp2", "exp2", mgg, tau2);
    RooRealVar N1("N1", "N1", 2500, 0.01, 50000);
    RooRealVar N2("N2", "N2", 1500, 0.01, 50000);
    RooAddPdf model("model", "composite model", RooArgList(exp1,exp2), RooArgList(N1,N2));
    // Fit to histogram
    model.fitTo(data, SumW2Error(true));
    // Plot pdf
    model.plotOn(mframe);
    model.plotOn(mframe, Normalization(RooAbsReal::Relative), Components("exp1"), LineStyle(kDashed), LineColor(kRed));
    model.plotOn(mframe, Normalization(RooAbsReal::Relative), Components("exp2"), LineStyle(kDashed), LineColor(kViolet));

    // Save plot
    mframe->Draw();
    c->SaveAs( outdir+Form("mgg_%02d.png",plot_idx) );

    delete mframe;
} // End of function FitExponential2


void ScanBDTscore(std::vector<TString> &samples, TString outdir_name, TString channel)
{
    // Set paths
    indir = dir_res;
    outdir = dir_res;
    if (channel=="had") {
        indir += "BDT_input_had/";
        outdir += "optimization_had/" + outdir_name + '/';
    } else if (channel=="lep") {
        indir += "BDT_input_lep/";
        outdir += "optimization_lep/" + outdir_name + '/';
    } else {
        std::cout << "[ERROR] Invalid channel!\n";
        std::exit(1);
    }
    if (gSystem->AccessPathName(indir)) {
        std::cout << "[ERROR] Input dir not exist!\n";
        std::exit(1);
    }
    if (gSystem->AccessPathName(outdir)) {
        gSystem->mkdir(outdir, true);  // "true" allows recursive mkdir
    }

    // Read input tree
    TChain *nt = new TChain("T");
    for (int i=0; i<samples.size(); ++i)
        nt->Add( indir + samples.at(i) + ".root" );
    // Set branches
    /*int evt_no = 0;
      int evtwei_scale = 0;
      float evtwei_central = 0;
      float BDT_score = 0;
      float dipho_mass = 0;
      nt->SetBranchAddress("evt_no", &evt_no);
      nt->SetBranchAddress("evtwei_scale", &evtwei_scale);
      nt->SetBranchAddress("evtwei_central", &evtwei_central);
      nt->SetBranchAddress("BDT_score", &BDT_score);
      nt->SetBranchAddress("dipho_mass", &dipho_mass);*/

    // Initialize loop variables
    float BDT_cut = -0.7;
    plot_idx = 0;
    TH1F *h;
    TString expr_cut = "(evtwei_central*evtwei_scale) * (%s && BDT_score>%%f)";
    // For non-resonant bkg samples, only use evt_no%2==1
    if (samples == nt_nrbkg_had || samples == nt_nrbkg_lep)
        expr_cut = Form(expr_cut,"evt_no%2==1");
    // For other bkg samples, only use evt_no%3==1
    else
        expr_cut = Form(expr_cut,"evt_no%3==1");

    c = new TCanvas();

    // Loop for BDT cut
    while (BDT_cut < 0.6) {
        h = new TH1F("hmgg", Form("BDT cut at %f",BDT_cut), 32, 100, 180);

        // Make Mgg histogram
        nt->Draw("dipho_mass>>hmgg", Form(expr_cut,BDT_cut));
        //FitGaussian( h, BDT_cut );
        //FitGaussian2( h, BDT_cut );
        //FitGaussian3( h, BDT_cut );
        //FitExponential( h, BDT_cut );
        FitExponential2( h, BDT_cut );
        //c->SaveAs(outdir+Form("BDTcut_%02d.png",plot_idx));

        delete h;
        BDT_cut += 0.02;
        plot_idx += 1;
    }

    delete c;
    delete nt;
} // End of function ScanBDTscore


void Opt_RoughScan()
{
    //ScanBDTscore(nt_sig_had, "TestPlot_signal", "had");
    //ScanBDTscore(nt_resbkg, "TestPlot_resbkg", "had");
    //ScanBDTscore(nt_nrbkg_had, "TestPlot_nrbkg", "had");
    //ScanBDTscore(nt_sig_lep, "TestPlot_signal", "lep");
    //ScanBDTscore(nt_resbkg, "TestPlot_resbkg", "lep");
    //ScanBDTscore(nt_nrbkg_lep, "TestPlot_nrbkg", "lep");

    //ScanBDTscore(nt_sig_had, "TestFit_signal_gaus1", "had");
    //ScanBDTscore(nt_resbkg, "TestFit_resbkg_gaus1", "had");
    //ScanBDTscore(nt_sig_lep, "TestFit_signal_gaus1", "lep");
    //ScanBDTscore(nt_resbkg, "TestFit_resbkg_gaus1", "lep");

    //ScanBDTscore(nt_sig_had, "TestFit_signal_gaus2", "had");
    //ScanBDTscore(nt_resbkg, "TestFit_resbkg_gaus2", "had");
    //ScanBDTscore(nt_sig_lep, "TestFit_signal_gaus2", "lep");
    //ScanBDTscore(nt_resbkg, "TestFit_resbkg_gaus2", "lep");

    //ScanBDTscore(nt_sig_had, "TestFit_signal_gaus3", "had");
    //ScanBDTscore(nt_resbkg, "TestFit_resbkg_gaus3", "had");
    //ScanBDTscore(nt_sig_lep, "TestFit_signal_gaus3", "lep");
    //ScanBDTscore(nt_resbkg, "TestFit_resbkg_gaus3", "lep");

    //ScanBDTscore(nt_nrbkg_had, "TestFit_nrbkg_exp", "had");
    //ScanBDTscore(nt_nrbkg_lep, "TestFit_nrbkg_exp", "lep");

    ScanBDTscore(nt_nrbkg_had, "TestFit_nrbkg_exp2", "had");
    ScanBDTscore(nt_nrbkg_lep, "TestFit_nrbkg_exp2", "lep");
}
