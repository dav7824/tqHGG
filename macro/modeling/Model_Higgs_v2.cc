/*
 * Fit diphoton mass with sum of gaussians.
 * Hadronic & leptonic channels are fitted simultaneously.
 */

#include "include/Paths.h"
using namespace RooFit;


void FitGaussian(TString samples, TString outdir_name, TString title)
{
    // Set paths
    std::vector<TString> indir = {
        dir_res + "model_had/trees/",
        dir_res + "model_lep/trees/",
    };
    TString outdir = dir_res + "model_both/" + outdir_name +'/';
    for (int i=0; i<indir.size(); ++i)
        if (gSystem->AccessPathName(indir[i])) {
            std::cout << "[ERROR] Input dir doen not exist\n";
            std::exit(1);
        }
    if (gSystem->AccessPathName(outdir))  gSystem->mkdir(outdir, true);

    // Read samples
    TChain *T = new TChain("T");
    for (int i=0; i<indir.size(); ++i)
        T->Add( indir[i]+samples+".root" );

    // Create variables
    RooRealVar mgg("dipho_mass", "M_{#gamma#gamma}", 100, 180);
    RooRealVar evtwei("evtwei_central", "event weight", -10, 10);

    // Import samples
    RooDataSet data("data", "data", RooArgSet(mgg,evtwei), WeightVar(evtwei), Import(*T),
            Cut("100<=dipho_mass && dipho_mass<=180"));

    // Create plot
    RooPlot *mframe = mgg.frame( Title(title.Data())/*, Bins(xxx)*/ );
    data.plotOn(mframe);

    // Create model
    RooRealVar mu1("mu1", "mean 1", 125, 105, 145);
    RooRealVar sigma1("sigma1", "sigma 1", 2, 1e-3, 100);
    RooGaussian model("model", "mgg pdf", mgg, mu1, sigma1);
    // Fit model
    RooFitResult *res = model.fitTo(data, Save(true), SumW2Error(true));    // Correct error & cov matrix for weighted likelihood
    res->Print("v");
    model.plotOn(mframe);

    // Get fitted NLL value
    RooAbsReal *nll = model.createNLL(data);

    // Save plot
    TCanvas *c = new TCanvas();
    mframe->Draw();
    c->SaveAs( outdir+"model.png" );

    // Save model to workspace
    RooWorkspace *w = new RooWorkspace("w", "workspace");
    w->import(model);
    w->writeToFile( outdir+"model.root" );

    // Print summary
    std::ofstream out( (outdir+"fit.log").Data(), ios::out );
    out << "Event entries: " << data.numEntries() << std::endl;
    out << "Event yield: " << data.sumEntries() << std::endl;
    out << "\n[PDF parameters]\n";
    out << "mean 1 = " << mu1.getVal() << std::endl;
    out << "sigma 1 = " << sigma1.getVal() << std::endl;
    out << "\n[PDF NLL]\n";
    out << "NLL = " << nll->getVal() << std::endl;
    out.close();
} // End of function FitGaussian


void FitGaussian2(TString samples, TString outdir_name, TString title)
{
    // Set paths
    std::vector<TString> indir = {
        dir_res + "model_had/trees/",
        dir_res + "model_lep/trees/",
    };
    TString outdir = dir_res + "model_both/" + outdir_name +'/';
    for (int i=0; i<indir.size(); ++i)
        if (gSystem->AccessPathName(indir[i])) {
            std::cout << "[ERROR] Input dir doen not exist\n";
            std::exit(1);
        }
    if (gSystem->AccessPathName(outdir))  gSystem->mkdir(outdir, true);

    // Read samples
    TChain *T = new TChain("T");
    for (int i=0; i<indir.size(); ++i)
        T->Add( indir[i]+samples+".root" );

    // Create variables
    RooRealVar mgg("dipho_mass", "M_{#gamma#gamma}", 100, 180);
    RooRealVar evtwei("evtwei_central", "event weight", -10, 10);

    // Import samples
    RooDataSet data("data", "data", RooArgSet(mgg,evtwei), WeightVar(evtwei), Import(*T),
            Cut("100<=dipho_mass && dipho_mass<=180"));

    // Create plot
    RooPlot *mframe = mgg.frame( Title(title.Data()), Bins(32) );
    data.plotOn(mframe);

    // Create model
    RooRealVar mu1("mu1", "mean 1", 125, 105, 145);
    RooRealVar sigma1("sigma1", "sigma 1", 2, 1e-3, 100);
    RooGaussian gaus1("gaus1", "gaussian pdf 1", mgg, mu1, sigma1);
    RooRealVar mu2("mu2", "mean 2", 124, 105, 145);
    RooRealVar sigma2("sigma2", "sigma 2", 3, 1e-3, 100);
    RooGaussian gaus2("gaus2", "gaussian pdf 2", mgg, mu2, sigma2);
    RooRealVar N1("N1", "N1", 1, 1e-6, 100);
    RooRealVar N2("N2", "N2", 1, 1e-6, 100);
    RooAddPdf model("model", "mgg pdf", RooArgList(gaus1,gaus2), RooArgList(N1,N2));
    // Fit model
    RooFitResult *res = model.fitTo(data, Save(true), SumW2Error(true));    // Correct error & cov matrix for weighted likelihood
    res->Print("v");
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("gaus1"), LineStyle(kDashed), LineColor(kRed));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("gaus2"), LineStyle(kDashed), LineColor(kViolet));

    // Get fitted NLL value
    RooAbsReal *nll = model.createNLL(data, Extended(true));

    // Save plot
    TCanvas *c = new TCanvas();
    mframe->Draw();
    c->SaveAs( outdir+"model.png" );

    // Save model to workspace
    RooWorkspace *w = new RooWorkspace("w", "workspace");
    w->import(model);
    w->writeToFile( outdir+"model.root" );

    // Print summary
    std::ofstream out( (outdir+"fit.log").Data(), ios::out );
    out << "Event entries: " << data.numEntries() << std::endl;
    out << "Event yield: " << data.sumEntries() << std::endl;
    out << "\n[PDF parameters]\n";
    out << "mean 1 = " << mu1.getVal() << std::endl;
    out << "sigma 1 = " << sigma1.getVal() << std::endl;
    out << "N1 = " << N1.getVal() << std::endl;
    out << "mean 2 = " << mu2.getVal() << std::endl;
    out << "sigma 2 = " << sigma2.getVal() << std::endl;
    out << "N2 = " << N2.getVal() << std::endl;
    out << "Fitted yield = " << N1.getVal()+N2.getVal() << std::endl;
    out << "\n[PDF NLL]\n";
    out << "NLL = " << nll->getVal() << std::endl;
    out.close();
} // End of function FitGaussian2


void FitGaussian3(TString samples, TString outdir_name, TString title)
{
    // Set paths
    std::vector<TString> indir = {
        dir_res + "model_had/trees/",
        dir_res + "model_lep/trees/",
    };
    TString outdir = dir_res + "model_both/" + outdir_name +'/';
    for (int i=0; i<indir.size(); ++i)
        if (gSystem->AccessPathName(indir[i])) {
            std::cout << "[ERROR] Input dir doen not exist\n";
            std::exit(1);
        }
    if (gSystem->AccessPathName(outdir))  gSystem->mkdir(outdir, true);

    // Read samples
    TChain *T = new TChain("T");
    for (int i=0; i<indir.size(); ++i)
        T->Add( indir[i]+samples+".root" );

    // Create variables
    RooRealVar mgg("dipho_mass", "M_{#gamma#gamma}", 100, 180);
    RooRealVar evtwei("evtwei_central", "event weight", -10, 10);

    // Import samples
    RooDataSet data("data", "data", RooArgSet(mgg,evtwei), WeightVar(evtwei), Import(*T),
            Cut("100<=dipho_mass && dipho_mass<=180"));

    // Create plot
    RooPlot *mframe = mgg.frame( Title(title.Data())/*, Bins(xxx)*/ );
    data.plotOn(mframe);

    // Create model
    RooRealVar mu1("mu1", "mean 1", 125, 105, 145);
    RooRealVar sigma1("sigma1", "sigma 1", 2, 1e-3, 100);
    RooGaussian gaus1("gaus1", "gaussian pdf 1", mgg, mu1, sigma1);
    RooRealVar mu2("mu2", "mean 2", 124, 105, 145);
    RooRealVar sigma2("sigma2", "sigma 2", 3, 1e-3, 100);
    RooGaussian gaus2("gaus2", "gaussian pdf 2", mgg, mu2, sigma2);
    RooRealVar mu3("mu3", "mean 3", 127, 105, 145);
    RooRealVar sigma3("sigma3", "sigma 3", 5, 1e-3, 100);
    RooGaussian gaus3("gaus3", "gaussian pdf 3", mgg, mu3, sigma3);
    RooRealVar N1("N1", "N1", 1, 1e-6, 100);
    RooRealVar N2("N2", "N2", 0.5, 1e-6, 100);
    RooRealVar N3("N3", "N3", 0.1, 1e-6, 100);
    RooAddPdf model("model", "mgg pdf", RooArgList(gaus1,gaus2,gaus3), RooArgList(N1,N2,N3));
    // Fit model
    RooFitResult *res = model.fitTo(data, Save(true), SumW2Error(true));    // Correct error & cov matrix for weighted likelihood
    res->Print("v");
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("gaus1"), LineStyle(kDashed), LineColor(kRed));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("gaus2"), LineStyle(kDashed), LineColor(kViolet));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("gaus3"), LineStyle(kDashed), LineColor(kGreen));

    // Get fitted NLL value
    RooAbsReal *nll = model.createNLL(data, Extended(true));

    // Save plot
    TCanvas *c = new TCanvas();
    mframe->Draw();
    c->SaveAs( outdir+"model.png" );

    // Save model to workspace
    RooWorkspace *w = new RooWorkspace("w", "workspace");
    w->import(model);
    w->writeToFile( outdir+"model.root" );

    // Print summary
    std::ofstream out( (outdir+"fit.log").Data(), ios::out );
    out << "Event entries: " << data.numEntries() << std::endl;
    out << "Event yield: " << data.sumEntries() << std::endl;
    out << "\n[PDF parameters]\n";
    out << "mean 1 = " << mu1.getVal() << std::endl;
    out << "sigma 1 = " << sigma1.getVal() << std::endl;
    out << "N1 = " << N1.getVal() << std::endl;
    out << "mean 2 = " << mu2.getVal() << std::endl;
    out << "sigma 2 = " << sigma2.getVal() << std::endl;
    out << "N2 = " << N2.getVal() << std::endl;
    out << "mean 3 = " << mu3.getVal() << std::endl;
    out << "sigma 3 = " << sigma3.getVal() << std::endl;
    out << "N3 = " << N3.getVal() << std::endl;
    out << "Fitted yield = " << N1.getVal()+N2.getVal()+N3.getVal() << std::endl;
    out << "\n[PDF NLL]\n";
    out << "NLL = " << nll->getVal() << std::endl;
    out.close();
} // End of function FitGaussian3


void FitGaussian4(TString samples, TString outdir_name, TString title)
{
    // Set paths
    std::vector<TString> indir = {
        dir_res + "model_had/trees/",
        dir_res + "model_lep/trees/",
    };
    TString outdir = dir_res + "model_both/" + outdir_name +'/';
    for (int i=0; i<indir.size(); ++i)
        if (gSystem->AccessPathName(indir[i])) {
            std::cout << "[ERROR] Input dir doen not exist\n";
            std::exit(1);
        }
    if (gSystem->AccessPathName(outdir))  gSystem->mkdir(outdir, true);

    // Read samples
    TChain *T = new TChain("T");
    for (int i=0; i<indir.size(); ++i)
        T->Add( indir[i]+samples+".root" );

    // Create variables
    RooRealVar mgg("dipho_mass", "M_{#gamma#gamma}", 100, 180);
    RooRealVar evtwei("evtwei_central", "event weight", -10, 10);

    // Import samples
    RooDataSet data("data", "data", RooArgSet(mgg,evtwei), WeightVar(evtwei), Import(*T),
            Cut("100<=dipho_mass && dipho_mass<=180"));

    // Create plot
    RooPlot *mframe = mgg.frame( Title(title.Data())/*, Bins(xxx)*/ );
    data.plotOn(mframe);

    // Create model
    RooRealVar mu1("mu1", "mean 1", 125, 105, 145);
    RooRealVar sigma1("sigma1", "sigma 1", 2, 1e-3, 100);
    RooGaussian gaus1("gaus1", "gaussian pdf 1", mgg, mu1, sigma1);
    RooRealVar mu2("mu2", "mean 2", 124, 105, 145);
    RooRealVar sigma2("sigma2", "sigma 2", 4, 1e-3, 100);
    RooGaussian gaus2("gaus2", "gaussian pdf 2", mgg, mu2, sigma2);
    RooRealVar mu3("mu3", "mean 3", 126, 105, 145);
    RooRealVar sigma3("sigma3", "sigma 3", 3, 1e-3, 100);
    RooGaussian gaus3("gaus3", "gaussian pdf 3", mgg, mu3, sigma3);
    RooRealVar mu4("mu4", "mean 4", 124.5, 105, 145);
    RooRealVar sigma4("sigma4", "sigma 4", 2.5, 1e-3, 100);
    RooGaussian gaus4("gaus4", "gaussian pdf 4", mgg, mu4, sigma4);
    RooRealVar N1("N1", "N1", 1, 1e-6, 100);
    RooRealVar N2("N2", "N2", 1, 1e-6, 100);
    RooRealVar N3("N3", "N3", 1, 1e-6, 100);
    RooRealVar N4("N4", "N4", 1, 1e-6, 100);
    RooAddPdf model("model", "mgg pdf", RooArgList(gaus1,gaus2,gaus3,gaus4), RooArgList(N1,N2,N3,N4));
    // Fit model
    RooFitResult *res = model.fitTo(data, Save(true), SumW2Error(true));    // Correct error & cov matrix for weighted likelihood
    res->Print("v");
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("gaus1"), LineStyle(kDashed), LineColor(kRed));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("gaus2"), LineStyle(kDashed), LineColor(kViolet));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("gaus3"), LineStyle(kDashed), LineColor(kGreen));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("gaus4"), LineStyle(kDashed), LineColor(kOrange));

    // Get fitted NLL value
    RooAbsReal *nll = model.createNLL(data, Extended(true));

    // Save plot
    TCanvas *c = new TCanvas();
    mframe->Draw();
    c->SaveAs( outdir+"model.png" );

    // Save model to workspace
    RooWorkspace *w = new RooWorkspace("w", "workspace");
    w->import(model);
    w->writeToFile( outdir+"model.root" );

    // Print summary
    std::ofstream out( (outdir+"fit.log").Data(), ios::out );
    out << "Event entries: " << data.numEntries() << std::endl;
    out << "Event yield: " << data.sumEntries() << std::endl;
    out << "\n[PDF parameters]\n";
    out << "mean 1 = " << mu1.getVal() << std::endl;
    out << "sigma 1 = " << sigma1.getVal() << std::endl;
    out << "N1 = " << N1.getVal() << std::endl;
    out << "mean 2 = " << mu2.getVal() << std::endl;
    out << "sigma 2 = " << sigma2.getVal() << std::endl;
    out << "N2 = " << N2.getVal() << std::endl;
    out << "mean 3 = " << mu3.getVal() << std::endl;
    out << "sigma 3 = " << sigma3.getVal() << std::endl;
    out << "N3 = " << N3.getVal() << std::endl;
    out << "mean 4 = " << mu4.getVal() << std::endl;
    out << "sigma 4 = " << sigma4.getVal() << std::endl;
    out << "N4 = " << N4.getVal() << std::endl;
    out << "Fitted yield = " << N1.getVal()+N2.getVal()+N3.getVal()+N4.getVal() << std::endl;
    out << "\n[PDF NLL]\n";
    out << "NLL = " << nll->getVal() << std::endl;
    out.close();
} // End of function FitGaussian4


void Model_Higgs_v2()
{
    //FitGaussian("*ToGG", "Fit_resbkg_gaus1", "Resonant bkg M_{#gamma#gamma}");
    FitGaussian2("*ToGG", "Fit_resbkg_gaus2", "Resonant bkg M_{#gamma#gamma}");  // chosen
    //FitGaussian3("*ToGG", "Fit_resbkg_gaus3", "Resonant bkg M_{#gamma#gamma}");
    //FitGaussian4("*ToGG", "Fit_resbkg_gaus4", "Resonant bkg M_{#gamma#gamma}");
}
