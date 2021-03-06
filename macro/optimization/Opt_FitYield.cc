#include "include/Paths.h"
using namespace RooFit;

TString indir, outdir;
TCanvas *c = 0;
int plot_idx = 0;
FILE *flog = 0;
TFile *fout = 0;
TTree *Tout = 0;

vector<TString> nt_sig_had = {
    "TT_FCNC-TtoHJ_aThad_hct",
    "TT_FCNC-aTtoHJ_Thad_hct",
    "TT_FCNC-TtoHJ_aThad_hut",
    "TT_FCNC-aTtoHJ_Thad_hut",
    "ST_FCNC-TH_Thad_hct",
    "ST_FCNC-TH_Thad_hut",
};
vector<TString> nt_sig_lep = {
    "TT_FCNC-TtoHJ_aTlep_hct",
    "TT_FCNC-aTtoHJ_Tlep_hct",
    "TT_FCNC-TtoHJ_aTlep_hut",
    "TT_FCNC-aTtoHJ_Tlep_hut",
    "ST_FCNC-TH_Tlep_hct",
    "ST_FCNC-TH_Tlep_hut",
};
vector<TString> nt_resbkg = {
    "GluGluHToGG",
    "VBFHToGG",
    "VHToGG",
    "ttHJetToGG",
};
vector<TString> nt_nrbkg_had = {
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
vector<TString> nt_nrbkg_lep = {
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


void FitGaussian2(TH1 *hh, float BDT_cut)
{
    // Make RooDataHist
    RooRealVar mgg("mgg", "M_{#gamma#gamma}", 100, 180);
    RooDataHist data("hmgg", "M_{#gamma#gamma}", mgg, hh);

    mgg.setRange("full", 100, 180);
    mgg.setRange("window", 115, 135);

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

    // Calculate integral
    RooAbsReal *intw = model.createIntegral(mgg, NormSet(mgg), Range("window"));

    // Print log
    flog = fopen( (outdir+"param.log").Data(), "a" );
    fprintf( flog, "%f:\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", BDT_cut, mu1.getVal(), sigma1.getVal(), mu2.getVal(), sigma2.getVal(), N1.getVal(), N2.getVal(),
            (N1.getVal()+N2.getVal())*intw->getVal());
    fclose(flog);

    // Save plot
    mframe->Draw();
    c->SaveAs( outdir+Form("mgg_%02d.png",plot_idx) );

    delete mframe;
} // End of function FitGaussian2


void FitExponential(TH1 *hh, float BDT_cut)
{
    // Make RooDataHist
    RooRealVar mgg("mgg", "M_{#gamma#gamma}", 100, 180);
    RooDataHist data("hmgg", "M_{#gamma#gamma}", mgg, hh);

    mgg.setRange("full", 100, 180);
    mgg.setRange("window", 115, 135);

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

    // Calculate integral
    RooAbsReal *intw = exp.createIntegral(mgg, NormSet(mgg), Range("window"));

    // Print log
    flog = fopen( (outdir+"param.log").Data(), "a" );
    fprintf( flog, "%f:\t%f\t%f\n", BDT_cut, tau.getVal(), data.sumEntries()*intw->getVal() );
    fclose(flog);

    // Save plot
    mframe->Draw();
    c->SaveAs( outdir+Form("mgg_%02d.png",plot_idx) );

    delete mframe;
} // End of function FitExponential


void ScanBDTscore(vector<TString> &samples, TString outdir_name, TString ch)
{
    // Set paths
    if (ch == "had") {
        indir = dir_res + "BDT_input_had/";
        outdir = dir_res + "optimization_had/" + outdir_name + '/';
    } else if (ch == "lep") {
        indir = dir_res + "BDT_input_lep/";
        outdir = dir_res + "optimization_lep/" + outdir_name + '/';
    } else {
        std::cout << "[ERROR] Invalid channel\n";
        std::exit(1);
    }
    if (gSystem->AccessPathName(indir)) {
        std::cout << "[ERROR] Input dir not exist\n";
        std::exit(1);
    }
    if (gSystem->AccessPathName(outdir))
        gSystem->mkdir(outdir, true);

    // Read input files
    TChain *nt = new TChain("T");
    for (int i=0; i<samples.size(); ++i)  nt->Add(indir+samples.at(i)+".root");

    // Set BDT scanning range
    float BDT_cut, BDT_inc;
    if (ch == "had") {
        BDT_cut = -0.24;
        BDT_inc = (0.38-BDT_cut)/100.;
    } else {
        BDT_cut = -0.22;
        BDT_inc = (0.24-BDT_cut)/100.;
    }

    // Initialize loop variables
    plot_idx = 0;
    TH1F *h = 0;
    TString expr_cut = "(evtwei_central*evtwei_scale) * (%s && BDT_score>%%f)";

    // For non-resonant bkg samples, only use evt_no%2==1
    if (samples == nt_nrbkg_had || samples == nt_nrbkg_lep)
        expr_cut = Form(expr_cut,"evt_no%2==1");
    // For other bkg samples, only use evt_no%3==1
    else
        expr_cut = Form(expr_cut,"evt_no%3==1");

    c = new TCanvas();

    // Loop for BDT cut values
    for (int i=0; i<100; ++i) {
        /*if (i != 96) { // For fitting specific BDT cut
            BDT_cut += BDT_inc;
            plot_idx += 1;
            continue;
        }*/

        h = new TH1F("hmgg", Form("BDT cut at %f",BDT_cut), 32, 100, 180);

        // Make Mgg histogram
        nt->Draw("dipho_mass>>hmgg", Form(expr_cut,BDT_cut));
        // Perform fit
        FitGaussian2(h, BDT_cut);
        //FitExponential(h, BDT_cut);

        delete h;
        BDT_cut += BDT_inc;
        plot_idx += 1;
    } // End BDT cut loop

    delete c;
    delete nt;
} // End of function ScanBDTscore


void Opt_FitYield()
{
    //ScanBDTscore(nt_sig_had, "Fit_signal", "had");
    ScanBDTscore(nt_resbkg, "Fit_resbkg", "had");
    //ScanBDTscore(nt_nrbkg_had, "Fit_nrbkg", "had");
    //ScanBDTscore(nt_sig_lep, "Fit_signal", "lep");
    //ScanBDTscore(nt_resbkg, "Fit_resbkg", "lep");
    //ScanBDTscore(nt_nrbkg_lep, "Fit_nrbkg", "lep");
}
