/*
 * Fit diphoton mass for non-resonant bkg.
 * Various functions are tested.
 * Hadronic & leptonic channels are fitted simultaneously.
 */

#include "include/Paths.h"
using namespace RooFit;


void FitExponential(TString outdir_name)
{
    // Set paths
    std::vector<TString> fin = {
        dir_res + "model_had/trees/data.root",
        dir_res + "model_lep/trees/data.root",
    };
    TString outdir = dir_res + "model_both/" + outdir_name + '/';
    for (int i=0; i<fin.size(); ++i)
        if (gSystem->AccessPathName(fin[i])) {
            std::cout << "[ERROR] Input file does not exist\n";
            std::exit(1);
        }
    if (gSystem->AccessPathName(outdir))  gSystem->mkdir(outdir, true);

    // Read samples
    TChain *T = new TChain("T");
    for (int i=0; i<fin.size(); ++i)
        T->Add( fin[i] );

    // Create variables
    RooRealVar mgg("dipho_mass", "M_{#gamma#gamma}", 100, 180);
    RooRealVar evtwei("evtwei_central", "event weight", -10, 10);

    // Set mgg ranges
    mgg.setRange("full", 100, 180);
    mgg.setRange("R1", 100, 120);
    mgg.setRange("R2", 130, 180);

    // Import samples
    RooDataSet data("data", "data", RooArgSet(mgg,evtwei), WeightVar(evtwei), Import(*T),
            Cut("(100<=dipho_mass && dipho_mass<=120) || (130<=dipho_mass && dipho_mass<=180)"));

    // Create plot
    RooPlot *mframe = mgg.frame( Title("Non-resonant bkg M_{#gamma#gamma}"), Bins(16) );
    data.plotOn(mframe);

    // Create model
    RooRealVar tau1("tau1", "tau 1", -0.02, -0.5, -1e-6);
    RooExponential model("model", "mgg pdf", mgg, tau1);
    // Fit model
    RooFitResult *res = model.fitTo(data, Range("R1,R2"), Save(true), SumW2Error(true));    // Correct error & cov matrix for weighted likelihood
    res->Print("v");
    model.plotOn(mframe, NormRange("R1,R2"));

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
    out << "tau 1 = " << tau1.getVal() << std::endl;
    out << "\n[PDF NLL]\n";
    out << "NLL = " << nll->getVal() << std::endl;
    out.close();
} // End of function FitExponential


void FitExponential2(TString outdir_name)
{
    // Set paths
    std::vector<TString> fin = {
        dir_res + "model_had/trees/data.root",
        dir_res + "model_lep/trees/data.root",
    };
    TString outdir = dir_res + "model_both/" + outdir_name + '/';
    for (int i=0; i<fin.size(); ++i)
        if (gSystem->AccessPathName(fin[i])) {
            std::cout << "[ERROR] Input file does not exist\n";
            std::exit(1);
        }
    if (gSystem->AccessPathName(outdir))  gSystem->mkdir(outdir, true);

    // Read samples
    TChain *T = new TChain("T");
    for (int i=0; i<fin.size(); ++i)
        T->Add( fin[i] );

    // Create variables
    RooRealVar mgg("dipho_mass", "M_{#gamma#gamma}", 100, 180);
    RooRealVar evtwei("evtwei_central", "event weight", -10, 10);

    // Set mgg ranges
    mgg.setRange("full", 100, 180);
    mgg.setRange("R1", 100, 120);
    mgg.setRange("R2", 130, 180);

    // Import samples
    RooDataSet data("data", "data", RooArgSet(mgg,evtwei), WeightVar(evtwei), Import(*T),
            Cut("(100<=dipho_mass && dipho_mass<=120) || (130<=dipho_mass && dipho_mass<=180)"));

    // Create plot
    RooPlot *mframe = mgg.frame( Title("Non-resonant bkg M_{#gamma#gamma}"), Bins(16) );
    data.plotOn(mframe);

    // Create model
    RooRealVar tau1("tau1", "tau 1", -0.05, -0.5, -1e-6);
    RooExponential exp1("exp1", "exponential 1", mgg, tau1);
    RooRealVar tau2("tau2", "tau 2", -0.005, -0.5, -1e-6);
    RooExponential exp2("exp2", "exponential 2", mgg, tau2);
    RooRealVar N1("N1", "N1", 15, 1e-6, 1e3);
    RooRealVar N2("N2", "N2", 15, 1e-6, 1e3);
    RooAddPdf model("model", "mgg pdf", RooArgList(exp1,exp2), RooArgList(N1,N2));
    // Fit model
    RooFitResult *res = model.fitTo(data, Range("R1,R2"), Save(true), SumW2Error(true));    // Correct error & cov matrix for weighted likelihood
    res->Print("v");
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("exp1"), LineStyle(kDashed), LineColor(kRed));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("exp2"), LineStyle(kDashed), LineColor(kViolet));

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
    out << "tau 1 = " << tau1.getVal() << std::endl;
    out << "tau 2 = " << tau2.getVal() << std::endl;
    out << "N1 = " << N1.getVal() << std::endl;
    out << "N2 = " << N2.getVal() << std::endl;
    out << "Fitted yield = " << N1.getVal()+N2.getVal() << std::endl;
    out << "\n[PDF NLL]\n";
    out << "NLL = " << nll->getVal() << std::endl;
    out.close();
} // End of function FitExponential2


void FitExponential3(TString outdir_name)
{
    // Set paths
    std::vector<TString> fin = {
        dir_res + "model_had/trees/data.root",
        dir_res + "model_lep/trees/data.root",
    };
    TString outdir = dir_res + "model_both/" + outdir_name + '/';
    for (int i=0; i<fin.size(); ++i)
        if (gSystem->AccessPathName(fin[i])) {
            std::cout << "[ERROR] Input file does not exist\n";
            std::exit(1);
        }
    if (gSystem->AccessPathName(outdir))  gSystem->mkdir(outdir, true);

    // Read samples
    TChain *T = new TChain("T");
    for (int i=0; i<fin.size(); ++i)
        T->Add( fin[i] );

    // Create variables
    RooRealVar mgg("dipho_mass", "M_{#gamma#gamma}", 100, 180);
    RooRealVar evtwei("evtwei_central", "event weight", -10, 10);

    // Set mgg ranges
    mgg.setRange("full", 100, 180);
    mgg.setRange("R1", 100, 120);
    mgg.setRange("R2", 130, 180);

    // Import samples
    RooDataSet data("data", "data", RooArgSet(mgg,evtwei), WeightVar(evtwei), Import(*T),
            Cut("(100<=dipho_mass && dipho_mass<=120) || (130<=dipho_mass && dipho_mass<=180)"));

    // Create plot
    RooPlot *mframe = mgg.frame( Title("Non-resonant bkg M_{#gamma#gamma}"), Bins(16) );
    data.plotOn(mframe);

    // Create model
    RooRealVar tau1("tau1", "tau 1", -0.05, -0.5, -1e-6);
    RooExponential exp1("exp1", "exponential 1", mgg, tau1);
    RooRealVar tau2("tau2", "tau 2", -0.005, -0.5, -1e-6);
    RooExponential exp2("exp2", "exponential 2", mgg, tau2);
    RooRealVar tau3("tau3", "tau 3", -0.01, -0.5, -1e-6);
    RooExponential exp3("exp3", "exponential 3", mgg, tau3);
    RooRealVar N1("N1", "N1", 10, 1e-6, 1e3);
    RooRealVar N2("N2", "N2", 10, 1e-6, 1e3);
    RooRealVar N3("N3", "N3", 10, 1e-6, 1e3);
    RooAddPdf model("model", "mgg pdf", RooArgList(exp1,exp2,exp3), RooArgList(N1,N2,N3));
    // Fit model
    RooFitResult *res = model.fitTo(data, Range("R1,R2"), Save(true), SumW2Error(true));    // Correct error & cov matrix for weighted likelihood
    res->Print("v");
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("exp1"), LineStyle(kDashed), LineColor(kRed));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("exp2"), LineStyle(kDashed), LineColor(kViolet));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("exp3"), LineStyle(kDashed), LineColor(kGreen));

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
    out << "tau 1 = " << tau1.getVal() << std::endl;
    out << "tau 2 = " << tau2.getVal() << std::endl;
    out << "tau 3 = " << tau3.getVal() << std::endl;
    out << "N1 = " << N1.getVal() << std::endl;
    out << "N2 = " << N2.getVal() << std::endl;
    out << "N3 = " << N3.getVal() << std::endl;
    out << "Fitted yield = " << N1.getVal()+N2.getVal()+N3.getVal() << std::endl;
    out << "\n[PDF NLL]\n";
    out << "NLL = " << nll->getVal() << std::endl;
    out.close();
} // End of function FitExponential3


void FitPower1(TString outdir_name)
{
    // Set paths
    std::vector<TString> fin = {
        dir_res + "model_had/trees/data.root",
        dir_res + "model_lep/trees/data.root",
    };
    TString outdir = dir_res + "model_both/" + outdir_name + '/';
    for (int i=0; i<fin.size(); ++i)
        if (gSystem->AccessPathName(fin[i])) {
            std::cout << "[ERROR] Input file does not exist\n";
            std::exit(1);
        }
    if (gSystem->AccessPathName(outdir))  gSystem->mkdir(outdir, true);

    // Read samples
    TChain *T = new TChain("T");
    for (int i=0; i<fin.size(); ++i)
        T->Add( fin[i] );

    // Create variables
    RooRealVar mgg("dipho_mass", "M_{#gamma#gamma}", 100, 180);
    RooRealVar evtwei("evtwei_central", "event weight", -10, 10);

    // Set mgg ranges
    mgg.setRange("full", 100, 180);
    mgg.setRange("R1", 100, 120);
    mgg.setRange("R2", 130, 180);

    // Import samples
    RooDataSet data("data", "data", RooArgSet(mgg,evtwei), WeightVar(evtwei), Import(*T),
            Cut("(100<=dipho_mass && dipho_mass<=120) || (130<=dipho_mass && dipho_mass<=180)"));

    // Create plot
    RooPlot *mframe = mgg.frame( Title("Non-resonant bkg M_{#gamma#gamma}"), Bins(16) );
    data.plotOn(mframe);

    // Create model
    RooRealVar a1("a1", "a1", -1e-2, -100, -1e-6);
    RooGenericPdf model("model", "pow(dipho_mass,a1)", RooArgSet(mgg,a1));
    // Fit model
    RooFitResult *res = model.fitTo(data, Range("R1,R2"), Save(true), SumW2Error(true));    // Correct error & cov matrix for weighted likelihood
    res->Print("v");
    model.plotOn(mframe, NormRange("R1,R2"));

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
    out << "a1 = " << a1.getVal() << std::endl;
    out << "\n[PDF NLL]\n";
    out << "NLL = " << nll->getVal() << std::endl;
    out.close();
} // End of function FitPower1


void FitPower2(TString outdir_name)
{
    // Set paths
    std::vector<TString> fin = {
        dir_res + "model_had/trees/data.root",
        dir_res + "model_lep/trees/data.root",
    };
    TString outdir = dir_res + "model_both/" + outdir_name + '/';
    for (int i=0; i<fin.size(); ++i)
        if (gSystem->AccessPathName(fin[i])) {
            std::cout << "[ERROR] Input file does not exist\n";
            std::exit(1);
        }
    if (gSystem->AccessPathName(outdir))  gSystem->mkdir(outdir, true);

    // Read samples
    TChain *T = new TChain("T");
    for (int i=0; i<fin.size(); ++i)
        T->Add( fin[i] );

    // Create variables
    RooRealVar mgg("dipho_mass", "M_{#gamma#gamma}", 100, 180);
    RooRealVar evtwei("evtwei_central", "event weight", -10, 10);

    // Set mgg ranges
    mgg.setRange("full", 100, 180);
    mgg.setRange("R1", 100, 120);
    mgg.setRange("R2", 130, 180);

    // Import samples
    RooDataSet data("data", "data", RooArgSet(mgg,evtwei), WeightVar(evtwei), Import(*T),
            Cut("(100<=dipho_mass && dipho_mass<=120) || (130<=dipho_mass && dipho_mass<=180)"));

    // Create plot
    RooPlot *mframe = mgg.frame( Title("Non-resonant bkg M_{#gamma#gamma}"), Bins(16) );
    data.plotOn(mframe);

    // Create model
    RooRealVar a1("a1", "a1", -1e-2, -100, -1e-6);
    RooGenericPdf pow1("pow1", "pow(dipho_mass,a1)", RooArgSet(mgg,a1));
    RooRealVar a2("a2", "a2", -1, -100, -1e-6);
    RooGenericPdf pow2("pow2", "pow(dipho_mass,a2)", RooArgSet(mgg,a2));
    RooRealVar N1("N1", "N1", 15, 1e-6, 1e3);
    RooRealVar N2("N2", "N2", 15, 1e-6, 1e3);
    RooAddPdf model("model", "mgg pdf", RooArgList(pow1,pow2), RooArgList(N1,N2));
    // Fit model
    RooFitResult *res = model.fitTo(data, Range("R1,R2"), Save(true), SumW2Error(true));    // Correct error & cov matrix for weighted likelihood
    res->Print("v");
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("pow1"), LineStyle(kDashed), LineColor(kRed));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("pow2"), LineStyle(kDashed), LineColor(kViolet));

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
    out << "a1 = " << a1.getVal() << std::endl;
    out << "a2 = " << a2.getVal() << std::endl;
    out << "N1 = " << N1.getVal() << std::endl;
    out << "N2 = " << N2.getVal() << std::endl;
    out << "Fitted yield = " << N1.getVal()+N2.getVal() << std::endl;
    out << "\n[PDF NLL]\n";
    out << "NLL = " << nll->getVal() << std::endl;
    out.close();
} // End of function FitPower2


void FitPower3(TString outdir_name)
{
    // Set paths
    std::vector<TString> fin = {
        dir_res + "model_had/trees/data.root",
        dir_res + "model_lep/trees/data.root",
    };
    TString outdir = dir_res + "model_both/" + outdir_name + '/';
    for (int i=0; i<fin.size(); ++i)
        if (gSystem->AccessPathName(fin[i])) {
            std::cout << "[ERROR] Input file does not exist\n";
            std::exit(1);
        }
    if (gSystem->AccessPathName(outdir))  gSystem->mkdir(outdir, true);

    // Read samples
    TChain *T = new TChain("T");
    for (int i=0; i<fin.size(); ++i)
        T->Add( fin[i] );

    // Create variables
    RooRealVar mgg("dipho_mass", "M_{#gamma#gamma}", 100, 180);
    RooRealVar evtwei("evtwei_central", "event weight", -10, 10);

    // Set mgg ranges
    mgg.setRange("full", 100, 180);
    mgg.setRange("R1", 100, 120);
    mgg.setRange("R2", 130, 180);

    // Import samples
    RooDataSet data("data", "data", RooArgSet(mgg,evtwei), WeightVar(evtwei), Import(*T),
            Cut("(100<=dipho_mass && dipho_mass<=120) || (130<=dipho_mass && dipho_mass<=180)"));

    // Create plot
    RooPlot *mframe = mgg.frame( Title("Non-resonant bkg M_{#gamma#gamma}"), Bins(16) );
    data.plotOn(mframe);

    // Create model
    RooRealVar a1("a1", "a1", -1e-2, -100, -1e-6);
    RooGenericPdf pow1("pow1", "pow(dipho_mass,a1)", RooArgSet(mgg,a1));
    RooRealVar a2("a2", "a2", -1, -100, -1e-6);
    RooGenericPdf pow2("pow2", "pow(dipho_mass,a2)", RooArgSet(mgg,a2));
    RooRealVar a3("a3", "a3", -0.1, -100, -1e-6);
    RooGenericPdf pow3("pow3", "pow(dipho_mass,a3)", RooArgSet(mgg,a3));
    RooRealVar N1("N1", "N1", 10, 1e-6, 1e3);
    RooRealVar N2("N2", "N2", 10, 1e-6, 1e3);
    RooRealVar N3("N3", "N3", 10, 1e-6, 1e3);
    RooAddPdf model("model", "mgg pdf", RooArgList(pow1,pow2,pow3), RooArgList(N1,N2,N3));
    // Fit model
    RooFitResult *res = model.fitTo(data, Range("R1,R2"), Save(true), SumW2Error(true));    // Correct error & cov matrix for weighted likelihood
    res->Print("v");
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("pow1"), LineStyle(kDashed), LineColor(kRed));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("pow2"), LineStyle(kDashed), LineColor(kViolet));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("pow3"), LineStyle(kDashed), LineColor(kGreen));

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
    out << "a1 = " << a1.getVal() << std::endl;
    out << "a2 = " << a2.getVal() << std::endl;
    out << "a3 = " << a3.getVal() << std::endl;
    out << "N1 = " << N1.getVal() << std::endl;
    out << "N2 = " << N2.getVal() << std::endl;
    out << "N3 = " << N3.getVal() << std::endl;
    out << "Fitted yield = " << N1.getVal()+N2.getVal()+N3.getVal() << std::endl;
    out << "\n[PDF NLL]\n";
    out << "NLL = " << nll->getVal() << std::endl;
    out.close();
} // End of function FitPower3


void FitBernstein2(TString outdir_name)
{
    // Set paths
    std::vector<TString> fin = {
        dir_res + "model_had/trees/data.root",
        dir_res + "model_lep/trees/data.root",
    };
    TString outdir = dir_res + "model_both/" + outdir_name + '/';
    for (int i=0; i<fin.size(); ++i)
        if (gSystem->AccessPathName(fin[i])) {
            std::cout << "[ERROR] Input file does not exist\n";
            std::exit(1);
        }
    if (gSystem->AccessPathName(outdir))  gSystem->mkdir(outdir, true);

    // Read samples
    TChain *T = new TChain("T");
    for (int i=0; i<fin.size(); ++i)
        T->Add( fin[i] );

    // Create variables
    RooRealVar mgg("dipho_mass", "M_{#gamma#gamma}", 100, 180);
    RooRealVar evtwei("evtwei_central", "event weight", -10, 10);

    // Set mgg ranges
    mgg.setRange("full", 100, 180);
    mgg.setRange("R1", 100, 120);
    mgg.setRange("R2", 130, 180);

    // Import samples
    RooDataSet data("data", "data", RooArgSet(mgg,evtwei), WeightVar(evtwei), Import(*T),
            Cut("(100<=dipho_mass && dipho_mass<=120) || (130<=dipho_mass && dipho_mass<=180)"));

    // Create plot
    RooPlot *mframe = mgg.frame( Title("Non-resonant bkg M_{#gamma#gamma}"), Bins(16) );
    data.plotOn(mframe);

    // Create model
    RooConstVar rc0("rc0", "const 0", 0);
    RooConstVar rc1("rc1", "const 1", 1);
    RooRealVar N0("N0", "N0", 5, 1e-6, 1e3);
    RooRealVar N1("N1", "N1", 10, 1e-6, 1e3);
    RooRealVar N2("N2", "N2", 15, 1e-6, 1e3);
    RooBernstein B0_2("B0_2", "Bernstein0_2", mgg, RooArgList(rc1,rc0,rc0));
    RooBernstein B1_2("B1_2", "Bernstein1_2", mgg, RooArgList(rc0,rc1,rc0));
    RooBernstein B2_2("B2_2", "Bernstein2_2", mgg, RooArgList(rc0,rc0,rc1));
    RooAddPdf model("model", "mgg pdf", RooArgList(B0_2,B1_2,B2_2), RooArgList(N0,N1,N2));
    // Fit model
    RooFitResult *res = model.fitTo(data, Range("R1,R2"), Save(true), SumW2Error(true));    // Correct error & cov matrix for weighted likelihood
    res->Print("v");
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("B0_2"), LineStyle(kDashed), LineColor(kRed));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("B1_2"), LineStyle(kDashed), LineColor(kViolet));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("B2_2"), LineStyle(kDashed), LineColor(kGreen));

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
    out << "N0 = " << N0.getVal() << std::endl;
    out << "N1 = " << N1.getVal() << std::endl;
    out << "N2 = " << N2.getVal() << std::endl;
    out << "Fitted yield = " << N0.getVal()+N1.getVal()+N2.getVal() << std::endl;
    out << "\n[PDF NLL]\n";
    out << "NLL = " << nll->getVal() << std::endl;
    out.close();
} // End of function FitBernstein2


void FitBernstein3(TString outdir_name)
{
    // Set paths
    std::vector<TString> fin = {
        dir_res + "model_had/trees/data.root",
        dir_res + "model_lep/trees/data.root",
    };
    TString outdir = dir_res + "model_both/" + outdir_name + '/';
    for (int i=0; i<fin.size(); ++i)
        if (gSystem->AccessPathName(fin[i])) {
            std::cout << "[ERROR] Input file does not exist\n";
            std::exit(1);
        }
    if (gSystem->AccessPathName(outdir))  gSystem->mkdir(outdir, true);

    // Read samples
    TChain *T = new TChain("T");
    for (int i=0; i<fin.size(); ++i)
        T->Add( fin[i] );

    // Create variables
    RooRealVar mgg("dipho_mass", "M_{#gamma#gamma}", 100, 180);
    RooRealVar evtwei("evtwei_central", "event weight", -10, 10);

    // Set mgg ranges
    mgg.setRange("full", 100, 180);
    mgg.setRange("R1", 100, 120);
    mgg.setRange("R2", 130, 180);

    // Import samples
    RooDataSet data("data", "data", RooArgSet(mgg,evtwei), WeightVar(evtwei), Import(*T),
            Cut("(100<=dipho_mass && dipho_mass<=120) || (130<=dipho_mass && dipho_mass<=180)"));

    // Create plot
    RooPlot *mframe = mgg.frame( Title("Non-resonant bkg M_{#gamma#gamma}"), Bins(16) );
    data.plotOn(mframe);

    // Create model
    RooConstVar rc0("rc0", "const 0", 0);
    RooConstVar rc1("rc1", "const 1", 1);
    RooRealVar N0("N0", "N0", 20, 1e-6, 1e3);
    RooRealVar N1("N1", "N1", 10, 1e-6, 1e3);
    RooRealVar N2("N2", "N2", 2, 1e-6, 1e3);
    RooRealVar N3("N3", "N3", 1, 1e-6, 1e3);
    RooBernstein B0_3("B0_3", "Bernstein0_3", mgg, RooArgList(rc1,rc0,rc0,rc0));
    RooBernstein B1_3("B1_3", "Bernstein1_3", mgg, RooArgList(rc0,rc1,rc0,rc0));
    RooBernstein B2_3("B2_3", "Bernstein2_3", mgg, RooArgList(rc0,rc0,rc1,rc0));
    RooBernstein B3_3("B3_3", "Bernstein3_3", mgg, RooArgList(rc0,rc0,rc0,rc1));
    RooAddPdf model("model", "mgg pdf", RooArgList(B0_3,B1_3,B2_3,B3_3), RooArgList(N0,N1,N2,N3));
    // Fit model
    RooFitResult *res = model.fitTo(data, Range("R1,R2"), Save(true), SumW2Error(true));    // Correct error & cov matrix for weighted likelihood
    res->Print("v");
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("B0_3"), LineStyle(kDashed), LineColor(kRed));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("B1_3"), LineStyle(kDashed), LineColor(kViolet));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("B2_3"), LineStyle(kDashed), LineColor(kGreen));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("B3_3"), LineStyle(kDashed), LineColor(kOrange));

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
    out << "N0 = " << N0.getVal() << std::endl;
    out << "N1 = " << N1.getVal() << std::endl;
    out << "N2 = " << N2.getVal() << std::endl;
    out << "N3 = " << N3.getVal() << std::endl;
    out << "Fitted yield = " << N0.getVal()+N1.getVal()+N2.getVal()+N3.getVal() << std::endl;
    out << "\n[PDF NLL]\n";
    out << "NLL = " << nll->getVal() << std::endl;
    out.close();
} // End of function FitBernstein3


void FitBernstein4(TString outdir_name)
{
    // Set paths
    std::vector<TString> fin = {
        dir_res + "model_had/trees/data.root",
        dir_res + "model_lep/trees/data.root",
    };
    TString outdir = dir_res + "model_both/" + outdir_name + '/';
    for (int i=0; i<fin.size(); ++i)
        if (gSystem->AccessPathName(fin[i])) {
            std::cout << "[ERROR] Input file does not exist\n";
            std::exit(1);
        }
    if (gSystem->AccessPathName(outdir))  gSystem->mkdir(outdir, true);

    // Read samples
    TChain *T = new TChain("T");
    for (int i=0; i<fin.size(); ++i)
        T->Add( fin[i] );

    // Create variables
    RooRealVar mgg("dipho_mass", "M_{#gamma#gamma}", 100, 180);
    RooRealVar evtwei("evtwei_central", "event weight", -10, 10);

    // Set mgg ranges
    mgg.setRange("full", 100, 180);
    mgg.setRange("R1", 100, 120);
    mgg.setRange("R2", 130, 180);

    // Import samples
    RooDataSet data("data", "data", RooArgSet(mgg,evtwei), WeightVar(evtwei), Import(*T),
            Cut("(100<=dipho_mass && dipho_mass<=120) || (130<=dipho_mass && dipho_mass<=180)"));

    // Create plot
    RooPlot *mframe = mgg.frame( Title("Non-resonant bkg M_{#gamma#gamma}"), Bins(16) );
    data.plotOn(mframe);

    // Create model
    RooConstVar rc0("rc0", "const 0", 0);
    RooConstVar rc1("rc1", "const 1", 1);
    RooRealVar N0("N0", "N0", 20, 1e-6, 1e3);
    RooRealVar N1("N1", "N1", 10, 1e-6, 1e3);
    RooRealVar N2("N2", "N2", 2, 1e-6, 1e3);
    RooRealVar N3("N3", "N3", 1, 1e-6, 1e3);
    RooRealVar N4("N4", "N4", 1, 1e-6, 1e3);
    RooBernstein B0_4("B0_4", "Bernstein0_4", mgg, RooArgList(rc1,rc0,rc0,rc0,rc0));
    RooBernstein B1_4("B1_4", "Bernstein1_4", mgg, RooArgList(rc0,rc1,rc0,rc0,rc0));
    RooBernstein B2_4("B2_4", "Bernstein2_4", mgg, RooArgList(rc0,rc0,rc1,rc0,rc0));
    RooBernstein B3_4("B3_4", "Bernstein3_4", mgg, RooArgList(rc0,rc0,rc0,rc1,rc0));
    RooBernstein B4_4("B4_4", "Bernstein4_4", mgg, RooArgList(rc0,rc0,rc0,rc0,rc1));
    RooAddPdf model("model", "mgg pdf", RooArgList(B0_4,B1_4,B2_4,B3_4,B4_4), RooArgList(N0,N1,N2,N3,N4));
    // Fit model
    RooFitResult *res = model.fitTo(data, Range("R1,R2"), Save(true), SumW2Error(true));    // Correct error & cov matrix for weighted likelihood
    res->Print("v");
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("B0_4"), LineStyle(kDashed), LineColor(kRed));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("B1_4"), LineStyle(kDashed), LineColor(kViolet));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("B2_4"), LineStyle(kDashed), LineColor(kGreen));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("B3_4"), LineStyle(kDashed), LineColor(kOrange));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("B4_4"), LineStyle(kDashed), LineColor(kCyan));

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
    out << "N0 = " << N0.getVal() << std::endl;
    out << "N1 = " << N1.getVal() << std::endl;
    out << "N2 = " << N2.getVal() << std::endl;
    out << "N3 = " << N3.getVal() << std::endl;
    out << "N4 = " << N4.getVal() << std::endl;
    out << "Fitted yield = " << N0.getVal()+N1.getVal()+N2.getVal()+N3.getVal()+N4.getVal() << std::endl;
    out << "\n[PDF NLL]\n";
    out << "NLL = " << nll->getVal() << std::endl;
    out.close();
} // End of function FitBernstein4


void FitBernstein5(TString outdir_name)
{
    // Set paths
    std::vector<TString> fin = {
        dir_res + "model_had/trees/data.root",
        dir_res + "model_lep/trees/data.root",
    };
    TString outdir = dir_res + "model_both/" + outdir_name + '/';
    for (int i=0; i<fin.size(); ++i)
        if (gSystem->AccessPathName(fin[i])) {
            std::cout << "[ERROR] Input file does not exist\n";
            std::exit(1);
        }
    if (gSystem->AccessPathName(outdir))  gSystem->mkdir(outdir, true);

    // Read samples
    TChain *T = new TChain("T");
    for (int i=0; i<fin.size(); ++i)
        T->Add( fin[i] );

    // Create variables
    RooRealVar mgg("dipho_mass", "M_{#gamma#gamma}", 100, 180);
    RooRealVar evtwei("evtwei_central", "event weight", -10, 10);

    // Set mgg ranges
    mgg.setRange("full", 100, 180);
    mgg.setRange("R1", 100, 120);
    mgg.setRange("R2", 130, 180);

    // Import samples
    RooDataSet data("data", "data", RooArgSet(mgg,evtwei), WeightVar(evtwei), Import(*T),
            Cut("(100<=dipho_mass && dipho_mass<=120) || (130<=dipho_mass && dipho_mass<=180)"));

    // Create plot
    RooPlot *mframe = mgg.frame( Title("Non-resonant bkg M_{#gamma#gamma}"), Bins(16) );
    data.plotOn(mframe);

    // Create model
    RooConstVar rc0("rc0", "const 0", 0);
    RooConstVar rc1("rc1", "const 1", 1);
    RooRealVar N0("N0", "N0", 20, 1e-6, 1e3);
    RooRealVar N1("N1", "N1", 10, 1e-6, 1e3);
    RooRealVar N2("N2", "N2", 2, 1e-6, 1e3);
    RooRealVar N3("N3", "N3", 1, 1e-6, 1e3);
    RooRealVar N4("N4", "N4", 1, 1e-6, 1e3);
    RooRealVar N5("N5", "N5", 1, 1e-6, 1e3);
    RooBernstein B0("B0", "Bernstein0", mgg, RooArgList(rc1,rc0,rc0,rc0,rc0,rc0));
    RooBernstein B1("B1", "Bernstein1", mgg, RooArgList(rc0,rc1,rc0,rc0,rc0,rc0));
    RooBernstein B2("B2", "Bernstein2", mgg, RooArgList(rc0,rc0,rc1,rc0,rc0,rc0));
    RooBernstein B3("B3", "Bernstein3", mgg, RooArgList(rc0,rc0,rc0,rc1,rc0,rc0));
    RooBernstein B4("B4", "Bernstein4", mgg, RooArgList(rc0,rc0,rc0,rc0,rc1,rc0));
    RooBernstein B5("B5", "Bernstein5", mgg, RooArgList(rc0,rc0,rc0,rc0,rc0,rc1));
    RooAddPdf model("model", "mgg pdf", RooArgList(B0,B1,B2,B3,B4,B5), RooArgList(N0,N1,N2,N3,N4,N5));
    // Fit model
    RooFitResult *res = model.fitTo(data, Range("R1,R2"), Save(true), SumW2Error(true));    // Correct error & cov matrix for weighted likelihood
    res->Print("v");
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("B0"), LineStyle(kDashed), LineColor(kRed));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("B1"), LineStyle(kDashed), LineColor(kViolet));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("B2"), LineStyle(kDashed), LineColor(kGreen));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("B3"), LineStyle(kDashed), LineColor(kOrange));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("B4"), LineStyle(kDashed), LineColor(kCyan));
    model.plotOn(mframe, Normalization(1,RooAbsReal::Relative), Components("B5"), LineStyle(kDashed), LineColor(kSpring));

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
    out << "N0 = " << N0.getVal() << std::endl;
    out << "N1 = " << N1.getVal() << std::endl;
    out << "N2 = " << N2.getVal() << std::endl;
    out << "N3 = " << N3.getVal() << std::endl;
    out << "N4 = " << N4.getVal() << std::endl;
    out << "N5 = " << N5.getVal() << std::endl;
    out << "Fitted yield = " << N0.getVal()+N1.getVal()+N2.getVal()+N3.getVal()+N4.getVal()+N5.getVal() << std::endl;
    out << "\n[PDF NLL]\n";
    out << "NLL = " << nll->getVal() << std::endl;
    out.close();
} // End of function FitBernstein5


void Model_nonres()
{
    //FitExponential("Fit_nrbkg_exp1");
    //FitExponential2("Fit_nrbkg_exp2");  // chosen exponential
    //FitExponential3("Fit_nrbkg_exp3");

    //FitPower1("Fit_nrbkg_pow1");
    //FitPower2("Fit_nrbkg_pow2");  // chosen power law
    //FitPower3("Fit_nrbkg_pow3");

    // Note: Berstein polynomials do not fit well up to 5th order
    //FitBernstein2("Fit_nrbkg_bern2");
    //FitBernstein3("Fit_nrbkg_bern3");
    //FitBernstein4("Fit_nrbkg_bern4");
    //FitBernstein5("Fit_nrbkg_bern5");
}
