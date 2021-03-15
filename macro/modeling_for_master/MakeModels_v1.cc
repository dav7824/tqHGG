/*
 * Fit diphoton mass for discrete profiling method.
 *
 * Models are saved as extended form. (with event yield)
 * Signal models are fitted for each channel x coupling.
 * Resonant bkg models are fitted with 2 channels simultaneously, then normalize to each channel.
 * Data in each channel are saved as RooDataSet.
 */

#include "include/Paths.h"
using namespace RooFit;

TString outdir = dir_res + "for_master/";


void FitSignal(TString samples, TString channel, TString sample_name)
{
    // Set paths
    TString indir, fout_name;
    int Nbin;
    if (channel == "had") {
        indir = dir_res + "model_had/trees/";
        fout_name = outdir + "mgg_pdf_had.root";
        Nbin = 32;
    } else if (channel == "lep") {
        indir = dir_res + "model_lep/trees/";
        fout_name = outdir + "mgg_pdf_lep.root";
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
    RooRealVar N1("N1_"+sample_name, "N1", 1, 1e-6, 100);
    RooRealVar N2("N2_"+sample_name, "N2", 1, 1e-6, 100);
    RooAddPdf model("pdf_"+sample_name, "mgg pdf", RooArgList(gaus1,gaus2), RooArgList(N1,N2));
    // Fit model
    RooFitResult *res = model.fitTo(data, Save(true), SumW2Error(true));    // Correct error & cov matrix for weighted likelihood
    res->Print("v");
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("gaus1_"+sample_name), LineStyle(kDashed), LineColor(kRed));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("gaus2_"+sample_name), LineStyle(kDashed), LineColor(kViolet));

    // Get fitted NLL value
    RooAbsReal *nll = model.createNLL(data, Extended(true));

    // Save plot
    TCanvas *c = new TCanvas();
    mframe->Draw();
    c->SaveAs( outdir+"pdf_"+sample_name+'_'+channel+".png" );

    // Save model to workspace
    TFile *fout;
    RooWorkspace *w;
    if (gSystem->AccessPathName(fout_name)) {
        w = new RooWorkspace("w", "workspace");
    } else {
        fout = new TFile(fout_name, "update");
        w = (RooWorkspace*)fout->Get("w");
    }
    w->import(model);
    w->writeToFile( fout_name );

    // Print summary
    std::ofstream out( (outdir+"fit_sig.log").Data(), ios::app );
    out << "[ pdf_" << sample_name << '_' << channel << " ]\n";
    out << "Event entries: " << data.numEntries() << std::endl;
    out << "Event yield: " << data.sumEntries() << std::endl;
    out << "mean 1 = " << mu1.getVal() << std::endl;
    out << "sigma 1 = " << sigma1.getVal() << std::endl;
    out << "N1 = " << N1.getVal() << std::endl;
    out << "mean 2 = " << mu2.getVal() << std::endl;
    out << "sigma 2 = " << sigma2.getVal() << std::endl;
    out << "N2 = " << N2.getVal() << std::endl;
    out << "Fitted yield = " << N1.getVal()+N2.getVal() << std::endl;
    out << "NLL = " << nll->getVal() << std::endl << std::endl;
    out.close();
} // End of function FitSignal


void MakeModels_v1()
{
    //FitSignal("*hut", "had", "sigHut");
    //FitSignal("*hct", "had", "sigHct");
    //FitSignal("*hut", "lep", "sigHut");
    //FitSignal("*hct", "lep", "sigHct");
}
