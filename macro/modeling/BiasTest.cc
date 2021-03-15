#include "include/Paths.h"
using namespace RooFit;


void MakeNonExtended_exp(TString indir_name, TString outdir_name)
{
    // Read input file
    TString fin_name = dir_res + "model_both/" + indir_name + "/model.root";
    if (gSystem->AccessPathName(fin_name)) {
        std::cout << "[ERROR] Input file does not exist\n";
        std::exit(1);
    }
    TFile *fin = new TFile( fin_name );
    RooWorkspace *w = (RooWorkspace*)fin->Get("w");
    // Get input obj
    RooRealVar *tau1 = w->var("tau1");  tau1->setConstant(true);
    RooRealVar *tau2 = w->var("tau2");  tau2->setConstant(true);
    RooAbsPdf *exp1 = w->pdf("exp1");
    RooAbsPdf *exp2 = w->pdf("exp2");
    RooRealVar *N1 = w->var("N1");
    RooRealVar *N2 = w->var("N2");
    // Make non-extended pdf
    RooConstVar frac("frac", "frac", N1->getVal()/(N1->getVal()+N2->getVal()));
    RooAddPdf model("pdfnrbkg", "mgg pdf", RooArgList(*exp1,*exp2), frac);
    // Save result
    TString outdir = dir_res + "model_both/" + outdir_name +'/';
    if (gSystem->AccessPathName(outdir))  gSystem->mkdir(outdir);
    RooWorkspace *w_ = new RooWorkspace("w", "workspace");
    w_->import(model);
    w_->writeToFile( outdir+"model.root" );

    fin->Close();
} // End of function MakeNonExtended_exp


void MakeNonExtended_pow(TString indir_name, TString outdir_name)
{
    // Read input file
    TString fin_name = dir_res + "model_both/" + indir_name + "/model.root";
    if (gSystem->AccessPathName(fin_name)) {
        std::cout << "[ERROR] Input file does not exist\n";
        std::exit(1);
    }
    TFile *fin = new TFile( fin_name );
    RooWorkspace *w = (RooWorkspace*)fin->Get("w");
    // Get input obj
    RooRealVar *a1 = w->var("a1");  a1->setConstant(true);
    RooRealVar *a2 = w->var("a2");  a2->setConstant(true);
    RooAbsPdf *pow1 = w->pdf("pow1");
    RooAbsPdf *pow2 = w->pdf("pow2");
    RooRealVar *N1 = w->var("N1");
    RooRealVar *N2 = w->var("N2");
    // Make non-extended pdf
    RooConstVar frac("frac", "frac", N1->getVal()/(N1->getVal()+N2->getVal()));
    RooAddPdf model("pdfnrbkg", "mgg pdf", RooArgList(*pow1,*pow2), frac);
    // Save result
    TString outdir = dir_res + "model_both/" + outdir_name +'/';
    if (gSystem->AccessPathName(outdir))  gSystem->mkdir(outdir);
    RooWorkspace *w_ = new RooWorkspace("w", "workspace");
    w_->import(model);
    w_->writeToFile( outdir+"model.root" );

    fin->Close();
} // End of function MakeNonExtended_pow


void MakeNonExtended_gaus(TString indir_name, TString outdir_name, TString type, TString channel)
{
    // Read input file
    TString dir_ch;
    if (channel == "had")  dir_ch = "model_had/";
    else if (channel == "lep")  dir_ch = "model_lep/";
    else if (channel == "both")  dir_ch = "model_both/";
    else {
        std::cout << "[ERROR] Invalid channel\n";
        std::exit(1);
    }
    TString fin_name = dir_res + dir_ch + indir_name + "/model.root";
    if (gSystem->AccessPathName(fin_name)) {
        std::cout << "[ERROR] Input file does not exist\n";
        std::exit(1);
    }
    TFile *fin = new TFile( fin_name );
    RooWorkspace *w = (RooWorkspace*)fin->Get("w");
    // Get input obj
    RooRealVar *mu1 = w->var("mu1");  mu1->setConstant(true);
    RooRealVar *sigma1 = w->var("sigma1");  sigma1->setConstant(true);
    RooRealVar *mu2 = w->var("mu2");  mu2->setConstant(true);
    RooRealVar *sigma2 = w->var("sigma2");  sigma2->setConstant(true);
    RooAbsPdf *gaus1 = w->pdf("gaus1");
    RooAbsPdf *gaus2 = w->pdf("gaus2");
    RooRealVar *N1 = w->var("N1");
    RooRealVar *N2 = w->var("N2");
    // Make non-extended pdf
    RooConstVar frac("frac", "frac", N1->getVal()/(N1->getVal()+N2->getVal()));
    TString pdf_name = (type=="sig") ? "pdfsig" : "pdfresbkg";
    RooAddPdf model(pdf_name, "mgg pdf", RooArgList(*gaus1,*gaus2), frac);
    // Save result
    TString outdir = dir_res + dir_ch + outdir_name +'/';
    if (gSystem->AccessPathName(outdir))  gSystem->mkdir(outdir);
    RooWorkspace *w_ = new RooWorkspace("w", "workspace");
    w_->import(model);
    w_->writeToFile( outdir+"model.root" );

    // Make plot
    RooRealVar *mgg = w->var("dipho_mass");
    RooPlot *mframe = mgg->frame();
    model.plotOn(mframe);
    model.plotOn(mframe, Components("gaus1"), LineStyle(kDashed), LineColor(kRed));
    model.plotOn(mframe, Components("gaus2"), LineStyle(kDashed), LineColor(kViolet));
    TCanvas *c = new TCanvas();
    mframe->Draw();
    c->SaveAs( outdir+"model.png" );

    fin->Close();
} // End of function MakeNonExtended_gaus


/*
 * MakePull():
 * Make pull distribution. The bkg model is unextended.
 */
void MakePull(TString sig_name, TString resbkg_name, TString nrbkg_name, TString outdir_name, TString channel)
{
    // Set paths
    TString indir1, outdir;
    if (channel == "had") {
        indir1 = dir_res + "model_had/";
        outdir = dir_res + "model_had/" + outdir_name + '/';
    } else if (channel == "lep") {
        indir1 = dir_res + "model_lep/";
        outdir = dir_res + "model_lep/" + outdir_name + '/';
    } else {
        std::cout << "[ERROR] Invalid channel\n";
        std::exit(1);
    }
    TString indir2 = dir_res + "model_both/";

    TString fsig_name = indir1 + sig_name + "/model.root";
    TString fresbkg_name = indir2 + resbkg_name + "/model.root";
    TString fnrbkg_name = indir2 + nrbkg_name + "/model.root";
    TString fdata_name = indir1 + "trees/data.root";
    if (gSystem->AccessPathName(fsig_name) || gSystem->AccessPathName(fresbkg_name) ||
            gSystem->AccessPathName(fnrbkg_name) || gSystem->AccessPathName(fdata_name)) {
        std::cout << "[ERROR] Input file not exist\n";
        std::exit(1);
    }
    if (gSystem->AccessPathName(outdir))  gSystem->mkdir(outdir);

    // RooWorkspace files
    TFile *fsig = new TFile(fsig_name);
    TFile *fresbkg = new TFile(fresbkg_name);
    TFile *fnrbkg = new TFile(fnrbkg_name);
    TFile *fdata = new TFile(fdata_name);

    // Get pdfs
    RooAbsPdf *pdfsig = ((RooWorkspace*)fsig->Get("w"))->pdf("pdfsig");  // signal
    RooAbsPdf *pdfresbkg = ((RooWorkspace*)fresbkg->Get("w"))->pdf("pdfresbkg");  // resonant bkg
    RooAbsPdf *pdfnrbkg = ((RooWorkspace*)fnrbkg->Get("w"))->pdf("pdfnrbkg");  // non-resonant bkg

    // Create variables
    RooRealVar mgg("dipho_mass", "M_{#gamma#gamma}", 100, 180);
    RooRealVar evtwei("evtwei_central", "event weight", -10, 10);

    // Get data
    TTree *Tdata = (TTree*)fdata->Get("T");
    RooDataSet data("data", "data", RooArgSet(mgg,evtwei), WeightVar(evtwei), Import(*Tdata),
            Cut("100<=dipho_mass && dipho_mass<=180"));

    // Make mgg plot
    RooPlot *mframe = mgg.frame( Bins(16) );
    data.plotOn(mframe);

    // Create total bkg model
    RooRealVar frac("frac", "res-bkg fraction", 0.01, 0, 1);
    RooAddPdf pdfbkg("pdfbkg", "mgg pdf", RooArgList(*pdfresbkg,*pdfnrbkg), frac);
    // Fit bkg model to data
    RooFitResult *res = pdfbkg.fitTo( data, Save(true), SumW2Error(true) );
    res->Print("v");
    // Plot model
    pdfbkg.plotOn(mframe);
    pdfresbkg->plotOn(mframe, Normalization(frac.getVal(),RooAbsReal::NumEvent), LineStyle(kDashed), LineColor(kRed));
    pdfnrbkg->plotOn(mframe, Normalization(1.-frac.getVal(),RooAbsReal::NumEvent), LineStyle(kDashed), LineColor(kViolet));

    // Save plot
    TCanvas *c = new TCanvas();
    mframe->Draw();
    c->SaveAs( outdir+"model_bkg.png" );

    fdata->Close();
    fnrbkg->Close();
    fresbkg->Close();
    fsig->Close();
} // End of function MakePull


/*
 * MakePull_v2():
 * Make pull distribution. The bkg model is extended.
 */
void MakePull_v2(TString sig_name, TString resbkg_name, TString nrbkg_name, TString outdir_name, TString channel)
{
    // Set paths
    TString indir1, outdir;
    if (channel == "had") {
        indir1 = dir_res + "model_had/";
        outdir = dir_res + "model_had/" + outdir_name + '/';
    } else if (channel == "lep") {
        indir1 = dir_res + "model_lep/";
        outdir = dir_res + "model_lep/" + outdir_name + '/';
    } else {
        std::cout << "[ERROR] Invalid channel\n";
        std::exit(1);
    }
    TString indir2 = dir_res + "model_both/";

    TString fsig_name = indir1 + sig_name + "/model.root";
    TString fresbkg_name = indir2 + resbkg_name + "/model.root";
    TString fnrbkg_name = indir2 + nrbkg_name + "/model.root";
    TString fdata_name = indir1 + "trees/data.root";
    if (gSystem->AccessPathName(fsig_name) || gSystem->AccessPathName(fresbkg_name) ||
            gSystem->AccessPathName(fnrbkg_name) || gSystem->AccessPathName(fdata_name)) {
        std::cout << "[ERROR] Input file not exist\n";
        std::exit(1);
    }
    if (gSystem->AccessPathName(outdir))  gSystem->mkdir(outdir);

    // RooWorkspace files
    TFile *fsig = new TFile(fsig_name);
    TFile *fresbkg = new TFile(fresbkg_name);
    TFile *fnrbkg = new TFile(fnrbkg_name);
    TFile *fdata = new TFile(fdata_name);

    // Get pdfs
    RooAbsPdf *pdfsig = ((RooWorkspace*)fsig->Get("w"))->pdf("pdfsig");  // signal
    RooAbsPdf *pdfresbkg = ((RooWorkspace*)fresbkg->Get("w"))->pdf("pdfresbkg");  // resonant bkg
    RooAbsPdf *pdfnrbkg = ((RooWorkspace*)fnrbkg->Get("w"))->pdf("pdfnrbkg");  // non-resonant bkg

    // Create variables
    RooRealVar mgg("dipho_mass", "M_{#gamma#gamma}", 100, 180);
    RooRealVar evtwei("evtwei_central", "event weight", -10, 10);

    // Get data
    TTree *Tdata = (TTree*)fdata->Get("T");
    RooDataSet data("data", "data", RooArgSet(mgg,evtwei), WeightVar(evtwei), Import(*Tdata),
            Cut("100<=dipho_mass && dipho_mass<=180"));

    // Make mgg plot
    RooPlot *mframe = mgg.frame( Bins(16) );
    data.plotOn(mframe);

    // Create total bkg model
    RooRealVar Nres("Nres", "Nres", 1, 0, 1000);
    RooRealVar Nnr("Nnr", "Nnr", 20, 0, 1000);
    RooAddPdf pdfbkg("pdfbkg", "mgg pdf", RooArgList(*pdfresbkg,*pdfnrbkg), RooArgList(Nres,Nnr));
    // Fit bkg model to data
    RooFitResult *res = pdfbkg.fitTo( data, Save(true), SumW2Error(true) );
    res->Print("v");
    // Plot model
    pdfbkg.plotOn(mframe, Normalization(1,RooAbsReal::Relative));
    pdfbkg.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("pdfresbkg"), LineStyle(kDashed), LineColor(kRed));
    pdfbkg.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("pdfnrbkg"), LineStyle(kDashed), LineColor(kViolet));

    // Save plot
    TCanvas *c = new TCanvas();
    mframe->Draw();
    c->SaveAs( outdir+"model_bkg.png" );

    fdata->Close();
    fnrbkg->Close();
    fresbkg->Close();
    fsig->Close();
} // End of function MakePull_v2


/*
 * MakePull_v3():
 * Make pull distribution. The bkg model is extended and both channels are fitted simultaneously.
 */
void MakePull_v3(TString sig_name, TString resbkg_name, TString nrbkg_name, TString outdir_name)
{
    // Set paths
    TString indir_had = dir_res + "model_had/";
    TString indir_lep = dir_res + "model_lep/";
    TString indir_both = dir_res + "model_both/";
    TString outdir = dir_res + "model_both/" + outdir_name + '/';

    TString fsig_name = indir_lep + sig_name + "/model.root";  // try leptonic signal model
    TString fresbkg_name = indir_both + resbkg_name + "/model.root";
    TString fnrbkg_name = indir_both + nrbkg_name + "/model.root";
    if (gSystem->AccessPathName(fsig_name) || gSystem->AccessPathName(fresbkg_name) ||
            gSystem->AccessPathName(fnrbkg_name)) {
        std::cout << "[ERROR] Input file not exist\n";
        std::exit(1);
    }
    if (gSystem->AccessPathName(outdir))  gSystem->mkdir(outdir, true);

    // Get data TTree (data is for fitting bkg model)
    TChain *Tdata = new TChain("T");
    Tdata->Add( indir_had + "trees/data.root" );
    Tdata->Add( indir_lep + "trees/data.root" );

    // Get pdf files
    TFile *fsig = new TFile(fsig_name);
    TFile *fresbkg = new TFile(fresbkg_name);
    TFile *fnrbkg = new TFile(fnrbkg_name);

    // Get pdfs
    RooAbsPdf *pdfsig = ((RooWorkspace*)fsig->Get("w"))->pdf("pdfsig");  // signal
    RooAbsPdf *pdfresbkg = ((RooWorkspace*)fresbkg->Get("w"))->pdf("pdfresbkg");  // resonant bkg
    RooAbsPdf *pdfnrbkg = ((RooWorkspace*)fnrbkg->Get("w"))->pdf("pdfnrbkg");  // non-resonant bkg

    // Create variables
    RooRealVar mgg("dipho_mass", "M_{#gamma#gamma}", 100, 180);
    RooRealVar evtwei("evtwei_central", "event weight", -10, 10);

    // Get data
    RooDataSet data("data", "data", RooArgSet(mgg,evtwei), WeightVar(evtwei), Import(*Tdata),
            Cut("100<=dipho_mass && dipho_mass<=180"));

    // Make mgg plot
    RooPlot *mframe = mgg.frame( Bins(16) );
    data.plotOn(mframe);

    // Create total bkg model
    RooRealVar Nres("Nres", "Nres", 5, 0, 1000);
    RooRealVar Nnr("Nnr", "Nnr", 30, 0, 1000);
    RooAddPdf pdfbkg("pdfbkg", "mgg pdf", RooArgList(*pdfresbkg,*pdfnrbkg), RooArgList(Nres,Nnr));
    // Fit bkg model to data
    RooFitResult *res = pdfbkg.fitTo( data, Save(true), SumW2Error(true) );
    res->Print("v");
    // Plot model
    pdfbkg.plotOn(mframe, Normalization(1,RooAbsReal::Relative));
    pdfbkg.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("pdfresbkg"), LineStyle(kDashed), LineColor(kRed));
    pdfbkg.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("pdfnrbkg"), LineStyle(kDashed), LineColor(kViolet));

    // Save plot
    TCanvas *c = new TCanvas();
    mframe->Draw();
    c->SaveAs( outdir+"model_bkg.png" );

    fnrbkg->Close();
    fresbkg->Close();
    fsig->Close();
} // End of function MakePull_v3


void BiasTest()
{
    /* Make non-extended bkg models */
    //MakeNonExtended_exp("Fit_nrbkg_exp2", "Model_nrbkg_exp2");
    //MakeNonExtended_pow("Fit_nrbkg_pow2", "Model_nrbkg_pow2");
    //MakeNonExtended_gaus("Fit_sig_hut_gaus2", "Model_sig_hut_gaus2", "sig", "had");
    //MakeNonExtended_gaus("Fit_sig_hct_gaus2", "Model_sig_hct_gaus2", "sig", "had");
    //MakeNonExtended_gaus("Fit_sig_hut_gaus2", "Model_sig_hut_gaus2", "sig", "lep");
    //MakeNonExtended_gaus("Fit_sig_hct_gaus2", "Model_sig_hct_gaus2", "sig", "lep");
    //MakeNonExtended_gaus("Fit_resbkg_gaus2", "Model_resbkg_gaus2", "resbkg", "both");

    /* Make pull distribution */
    // MakePull() doesn't fit well
    //MakePull_v2("Model_sig_hut_gaus2", "Model_resbkg_gaus2", "Model_nrbkg_exp2", "Pull_exp2", "had");
    //MakePull_v2("Model_sig_hut_gaus2", "Model_resbkg_gaus2", "Model_nrbkg_exp2", "Pull_exp2", "lep");
    //MakePull_v2("Model_sig_hut_gaus2", "Model_resbkg_gaus2", "Model_nrbkg_pow2", "Pull_pow2", "had");
    //MakePull_v2("Model_sig_hut_gaus2", "Model_resbkg_gaus2", "Model_nrbkg_pow2", "Pull_pow2", "lep");
    //MakePull_v3("Model_sig_hut_gaus2", "Model_resbkg_gaus2", "Model_nrbkg_exp2", "Pull_exp2");
    MakePull_v3("Model_sig_hut_gaus2", "Model_resbkg_gaus2", "Model_nrbkg_pow2", "Pull_pow2");
}
