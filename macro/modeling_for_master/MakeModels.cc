/*
 * Fit diphoton mass for discrete profiling method. (with master's magic codes)
 *
 * Models are saved as RooExtendPdf.
 * Signal models are fitted for each channel x coupling.
 * Resonant bkg models are fitted with 2 channels simultaneously, then normalize to each channel.
 * Data in each channel are saved as RooDataSet.
 *
 * The products of all routines are saved in seperate files at first, then merged into a single
 * workspace.
 */

#include "include/Paths.h"
using namespace RooFit;

TString outdir = dir_res + "for_master/files/";


void FitSignal(TString samples, TString channel, TString sample_name)
{
    // Set paths
    TString indir;
    int Nbin;
    if (channel == "had") {
        indir = dir_res + "model_had/trees/";
        Nbin = 32;
    } else if (channel == "lep") {
        indir = dir_res + "model_lep/trees/";
        Nbin = 32;
    } else {
        std::cout << "[ERROR] Invalid channel\n";
        std::exit(1);
    }
    if (gSystem->AccessPathName(indir)) {
        std::cout << "[ERROR] Input dir doen not exist\n";
        std::exit(1);
    }
    if (gSystem->AccessPathName(outdir))  gSystem->mkdir(outdir, true);

    // Read samples
    TChain *T = new TChain("T");
    T->Add( indir+samples+".root" );

    // Create variables
    RooRealVar mgg("dipho_mass", "M_{#gamma#gamma}", 100, 180);
    RooRealVar evtwei("evtwei_central", "event weight", -10, 10);

    // Import samples
    RooDataSet data("data", "data", RooArgSet(mgg,evtwei), WeightVar(evtwei), Import(*T),
            Cut("100<=dipho_mass && dipho_mass<=180"));

    // Create plot
    RooPlot *mframe = mgg.frame( Bins(Nbin) );
    data.plotOn(mframe);

    // Create model
    RooRealVar mu1("mu1_"+sample_name, "mean 1", 125, 105, 145);
    RooRealVar sigma1("sigma1_"+sample_name, "sigma 1", 2, 1e-3, 100);
    RooGaussian gaus1("gaus1_"+sample_name, "gaussian pdf 1", mgg, mu1, sigma1);
    RooRealVar mu2("mu2_"+sample_name, "mean 2", 124, 105, 145);
    RooRealVar sigma2("sigma2_"+sample_name, "sigma 2", 3, 1e-3, 100);
    RooGaussian gaus2("gaus2_"+sample_name, "gaussian pdf 2", mgg, mu2, sigma2);
    RooRealVar N1("N1", "N1", 1, 1e-6, 100);
    RooRealVar N2("N2", "N2", 1, 1e-6, 100);
    RooAddPdf model_tmp("model_tmp", "mgg pdf", RooArgList(gaus1,gaus2), RooArgList(N1,N2));
    // Fit model
    RooFitResult *res = model_tmp.fitTo(data, Save(true), SumW2Error(true));    // Correct error & cov matrix for weighted likelihood
    res->Print("v");
    model_tmp.plotOn(mframe, Normalization(1,RooAbsReal::Relative));
    model_tmp.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("gaus1_"+sample_name), LineStyle(kDashed), LineColor(kRed));
    model_tmp.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("gaus2_"+sample_name), LineStyle(kDashed), LineColor(kViolet));

    // Save plot
    TCanvas *c = new TCanvas();
    mframe->Draw();
    c->SaveAs( outdir+"pdf_"+sample_name+'_'+channel+".png" );

    // Make RooExtendPdf model
    RooConstVar frac("frac_"+sample_name, "frac", N1.getVal()/(N1.getVal()+N2.getVal()));
    RooRealVar Ntot("N_"+sample_name, "N", N1.getVal()+N2.getVal(), 0, 1000);
    RooAddPdf pdf_("pdf_"+sample_name+'_', "mgg pdf", RooArgList(gaus1,gaus2), frac);
    RooExtendPdf pdf("pdf_"+sample_name, "mgg pdf", pdf_, Ntot);

    // Set constants
    mu1.setConstant(true);
    sigma1.setConstant(true);
    mu2.setConstant(true);
    sigma2.setConstant(true);

    // Save model to workspace
    TString fout_name = outdir+"Pdf_"+sample_name+'_'+channel+".root";
    RooWorkspace *w = new RooWorkspace("w_"+sample_name, "workspace");
    w->import(pdf);
    w->writeToFile( fout_name );

    // Print summary
    std::ofstream out( (outdir+"fit_sig.log").Data(), ios::app );
    out << "[ pdf_" << sample_name << '_' << channel << " ]\n";
    out << "Event entries: " << data.numEntries() << std::endl;
    out << "Event yield: " << data.sumEntries() << std::endl;
    out << "mean 1 = " << mu1.getVal() << std::endl;
    out << "sigma 1 = " << sigma1.getVal() << std::endl;
    out << "mean 2 = " << mu2.getVal() << std::endl;
    out << "sigma 2 = " << sigma2.getVal() << std::endl;
    out << "fraction 1 = " << frac.getVal() << std::endl;
    out << "Fitted yield = " << Ntot.getVal() << std::endl << std::endl;
    out.close();
} // End of function FitSignal


void FitResBkg(TString channel)
{
    if (channel != "had" && channel != "lep") {
        std::cout << "[ERROR] Invalid channel\n";
        std::exit(1);
    }

    // Read samples
    TChain *T = new TChain("T");
    T->Add( dir_res+"model_had/trees/*ToGG.root" );
    T->Add( dir_res+"model_lep/trees/*ToGG.root" );
    // Read channel samples
    TChain *Thad = new TChain("T");
    Thad->Add( dir_res+"model_had/trees/*ToGG.root" );
    TChain *Tlep = new TChain("T");
    Tlep->Add( dir_res+"model_lep/trees/*ToGG.root" );

    // Create variables
    RooRealVar mgg("dipho_mass", "M_{#gamma#gamma}", 100, 180);
    RooRealVar evtwei("evtwei_central", "event weight", -10, 10);

    // Import samples
    RooDataSet data("data", "data", RooArgSet(mgg,evtwei), WeightVar(evtwei), Import(*T),
            Cut("100<=dipho_mass && dipho_mass<=180"));
    // Import channel samples
    RooDataSet data_had("data_had", "data_had", RooArgSet(mgg,evtwei), WeightVar(evtwei), Import(*Thad),
            Cut("100<=dipho_mass && dipho_mass<=180"));
    RooDataSet data_lep("data_lep", "data_lep", RooArgSet(mgg,evtwei), WeightVar(evtwei), Import(*Tlep),
            Cut("100<=dipho_mass && dipho_mass<=180"));
    double frac_ch;
    if (channel == "had")
        frac_ch = data_had.sumEntries()/( data_had.sumEntries()+data_lep.sumEntries() );
    else
        frac_ch = data_lep.sumEntries()/( data_had.sumEntries()+data_lep.sumEntries() );

    // Create plot
    RooPlot *mframe = mgg.frame( Bins(32) );
    data.plotOn(mframe);

    // Create model
    TString sample_name = "resbkg";
    RooRealVar mu1("mu1_"+sample_name, "mean 1", 125, 105, 145);
    RooRealVar sigma1("sigma1_"+sample_name, "sigma 1", 2, 1e-3, 100);
    RooGaussian gaus1("gaus1_"+sample_name, "gaussian pdf 1", mgg, mu1, sigma1);
    RooRealVar mu2("mu2_"+sample_name, "mean 2", 124, 105, 145);
    RooRealVar sigma2("sigma2_"+sample_name, "sigma 2", 3, 1e-3, 100);
    RooGaussian gaus2("gaus2_"+sample_name, "gaussian pdf 2", mgg, mu2, sigma2);
    RooRealVar N1("N1", "N1", 1, 1e-6, 100);
    RooRealVar N2("N2", "N2", 1, 1e-6, 100);
    RooAddPdf model_tmp("model_tmp", "mgg pdf", RooArgList(gaus1,gaus2), RooArgList(N1,N2));
    // Fit model
    RooFitResult *res = model_tmp.fitTo(data, Save(true), SumW2Error(true));    // Correct error & cov matrix for weighted likelihood
    res->Print("v");
    model_tmp.plotOn(mframe, Normalization(1,RooAbsReal::Relative));
    model_tmp.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("gaus1_"+sample_name), LineStyle(kDashed), LineColor(kRed));
    model_tmp.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("gaus2_"+sample_name), LineStyle(kDashed), LineColor(kViolet));

    // Save plot
    TCanvas *c = new TCanvas();
    mframe->Draw();
    c->SaveAs( outdir+"pdf_"+sample_name+'_'+channel+".png" );

    // Make RooExtendPdf model
    RooConstVar frac("frac_"+sample_name, "frac", N1.getVal()/(N1.getVal()+N2.getVal()));
    RooRealVar Ntot("N_"+sample_name, "N", (N1.getVal()+N2.getVal())*frac_ch, 0, 1000);
    RooAddPdf pdf_("pdf_"+sample_name+'_', "mgg pdf", RooArgList(gaus1,gaus2), frac);
    RooExtendPdf pdf("pdf_"+sample_name, "mgg pdf", pdf_, Ntot);

    // Set constants
    mu1.setConstant(true);
    sigma1.setConstant(true);
    mu2.setConstant(true);
    sigma2.setConstant(true);

    // Save model to workspace
    TString fout_name = outdir+"Pdf_"+sample_name+'_'+channel+".root";
    RooWorkspace *w = new RooWorkspace("w_"+sample_name, "workspace");
    w->import(pdf);
    w->writeToFile( fout_name );

    // Print summary
    std::ofstream out( (outdir+"fit_resbkg.log").Data(), ios::app );
    out << "[ pdf_" << sample_name << '_' << channel << " ]\n";
    out << "mean 1 = " << mu1.getVal() << std::endl;
    out << "sigma 1 = " << sigma1.getVal() << std::endl;
    out << "mean 2 = " << mu2.getVal() << std::endl;
    out << "sigma 2 = " << sigma2.getVal() << std::endl;
    out << "fraction 1 = " << frac.getVal() << std::endl;
    out << "Fitted yield = " << Ntot.getVal() << std::endl << std::endl;
    out.close();
} // End of function FitResBkg


void PackModel(TString channel)
{
    if (channel != "had" && channel != "lep") {
        std::cout << "[ERROR] Invalid channel\n";
        std::exit(1);
    }

    TString indir = outdir;
    TString fout_name = dir_res+"for_master/mgg_pdf_"+channel+".root";

    // Create variables
    RooRealVar mgg("dipho_mass", "M_{#gamma#gamma}", 100, 180);
    RooRealVar evtwei("evtwei_central", "event weight", -10, 10);

    // Create workspace
    RooWorkspace *w = new RooWorkspace("w", "workspace");

    // Get data TTree & save them as RooDataSet
    TChain *T = new TChain("T");
    T->Add( dir_res+"model_"+channel+"/trees/data.root" );
    RooDataSet data("data", "data", RooArgSet(mgg,evtwei), WeightVar(evtwei), Import(*T),
            Cut("100<=dipho_mass && dipho_mass<=180"));

    // Import data
    w->import( data );
    // Import Hut signal pdf
    TFile *fin_sigHut = new TFile( indir+"Pdf_sigHut_"+channel+".root" );
    RooWorkspace *w_sigHut = (RooWorkspace*)fin_sigHut->Get("w_sigHut");
    w->import( *w_sigHut->pdf("pdf_sigHut") );
    // Import Hct signal pdf
    TFile *fin_sigHct = new TFile( indir+"Pdf_sigHct_"+channel+".root" );
    RooWorkspace *w_sigHct = (RooWorkspace*)fin_sigHct->Get("w_sigHct");
    w->import( *w_sigHct->pdf("pdf_sigHct") );
    // Import resonant bkg pdf
    TFile *fin_resbkg = new TFile( indir+"Pdf_resbkg_"+channel+".root" );
    RooWorkspace *w_resbkg = (RooWorkspace*)fin_resbkg->Get("w_resbkg");
    w->import( *w_resbkg->pdf("pdf_resbkg") );

    // Save workspace
    w->writeToFile( fout_name );
} // End of function PackModel


void MakeModels()
{
    //FitSignal("*hut", "had", "sigHut");
    //FitSignal("*hct", "had", "sigHct");
    //FitSignal("*hut", "lep", "sigHut");
    //FitSignal("*hct", "lep", "sigHct");

    //FitResBkg("had");
    //FitResBkg("lep");

    //PackModel("had");
    PackModel("lep");
}
