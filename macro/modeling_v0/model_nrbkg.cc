// Hadronic and leptonic channels are combined into a single dataset for fitting

using namespace RooFit;

// Cut on BDT score of each channel
double BDT_cut_had = 0.3231;
double BDT_cut_lep = 0.2448;

void model_nrbkg()
{
    // Get input trees
    TChain *T_had = new TChain("T");
    TChain *T_lep = new TChain("T");
    TString fin_had = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_had/output_BDT_v1/data.root";
    TString fin_lep = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_lep/output_BDT_v1/data.root";
    T_had->Add(fin_had);
    T_lep->Add(fin_lep);

    // Create tree variables
    RooRealVar Mgg("Mgg", "M_{#gamma#gamma}", 100, 180);
    RooRealVar BDT_score("BDT_score", "BDT score", 0, -1, 1);
    RooRealVar evt_weight("evt_weight", "event weight", 1, -10, 10);

    // Set RooDataSet
    RooDataSet data_had("data_had", "data_had", RooArgSet(Mgg,BDT_score,evt_weight), WeightVar(evt_weight), Import(*T_had),
            Cut("(100<=Mgg && Mgg<=120) || (130<=Mgg && Mgg<=180)"));
    RooDataSet data_lep("data_lep", "data_lep", RooArgSet(Mgg,BDT_score,evt_weight), WeightVar(evt_weight), Import(*T_lep),
            Cut("(100<=Mgg && Mgg<=120) || (130<=Mgg && Mgg<=180)"));
    const char *cut_expr = "BDT_score>=%f";
    RooDataSet *rdata_had = (RooDataSet*)data_had.reduce( Form(cut_expr,BDT_cut_had) );
    RooDataSet *rdata_lep = (RooDataSet*)data_lep.reduce( Form(cut_expr,BDT_cut_lep) );
    // Join 2 channels
    RooCategory ch("ch", "channel");
    ch.defineType("hadronic", 0);
    ch.defineType("leptonic", 1);
    // Dataset from both channels
    RooDataSet *dsall = new RooDataSet("dsall", "dataset of both channels", RooArgSet(Mgg,BDT_score,evt_weight), WeightVar(evt_weight), Index(ch),
            Import("hadronic",*rdata_had), Import("leptonic",*rdata_lep));

    cout << "\n\n[Dataset summary]\n";
    cout << "Entries: " << dsall->numEntries() << endl;
    cout << "Yield: " << dsall->sumEntries() << endl;

    // Mgg ranges
    Mgg.setRange("full", 100, 180);
    Mgg.setRange("R1", 100, 120);
    Mgg.setRange("R2", 130, 180);
    Mgg.setRange("window", 115, 135);
    Mgg.setRange("blind", 120, 130);

    // Make plots
    RooPlot *pmgg = Mgg.frame();
    // Plot dataset
    dsall->plotOn(pmgg);

    /* Test models */
    /* 2 exponentials */
    /*TString model_name = "Exponential2";
    RooRealVar tau1("tau1", "tau1", -0.02, -0.5, -1e-6);
    RooRealVar tau2("tau2", "tau2", -0.01, -0.5, -1e-6);
    RooRealVar N1("N1", "N1", 2500, 0., 50000);
    RooRealVar N2("N2", "N2", 1500, 0., 50000);
    RooExponential exp1("exp1", "exp1", Mgg, tau1);
    RooExponential exp2("exp2", "exp2", Mgg, tau2);
    RooAddPdf model("model", "composite model", RooArgList(exp1,exp2), RooArgList(N1,N2));
    // Fit model to dataset
    RooFitResult *res = model.fitTo(*dsall, Range("R1,R2"), Save(true), SumW2Error(true));    // Correct error & cov matrix for weighted likelihood
    res->Print("v");
    // Calculate integrals
    RooAbsReal *intw = model.createIntegral(Mgg, NormSet(Mgg), Range("window"));
    RooAbsReal *intb = model.createIntegral(Mgg, NormSet(Mgg), Range("blind"));
    double Ntot = N1.getVal()+N2.getVal();
    // Print fit summary
    cout << "\n\n[Fit summary]\n";
    cout << "Total yield: " << Ntot << endl;
    cout << "Sideband yield: " << Ntot*(1-intb->getVal()) << endl;
    cout << "Window yield: " << Ntot*intw->getVal() << endl;

    model.plotOn(pmgg, Normalization(1.,RooAbsReal::Relative));

    // Create non-extended version of the model for saving
    RooAddPdf nrbkg("nrbkg", "non-resonant bkg", RooArgList(exp1,exp2), RooConst(N1.getVal()/Ntot));
    nrbkg.plotOn(pmgg, NormRange("R1,R2"), LineStyle(kDashed), LineColor(kRed));*/

    RooConstVar rc0("rc0", "const 0", 0);
    RooConstVar rc1("rc1", "const 1", 1);

    /* 1st order power law */
    /*TString model_name = "PowerLaw1";
    RooRealVar a1("a1", "a1", -1e-2, -100, -1e-6);
    RooGenericPdf nrbkg("nrbkg", "pow(Mgg,a1)", RooArgSet(Mgg,a1));
    // Fit model to dataset
    RooFitResult *res = nrbkg.fitTo(*dsall, Range("R1,R2"), Save(true), SumW2Error(true));
    res->Print("v");
    // Calculate integrals
    RooAbsReal *intw = nrbkg.createIntegral(Mgg, NormSet(Mgg), Range("window"));
    RooAbsReal *intb = nrbkg.createIntegral(Mgg, NormSet(Mgg), Range("blind"));
    // Print fit summary
    cout << "\n\n[Fit summary]\n";
    cout << "Total yield: " << dsall->sumEntries() << endl;
    cout << "Sideband yield: " << dsall->sumEntries()*(1-intb->getVal()) << endl;
    cout << "Window yield: " << dsall->sumEntries()*intw->getVal() << endl;

    nrbkg.plotOn(pmgg, NormRange("R1,R2"));*/

    /* 2nd order power law */
    TString model_name = "PowerLaw2";
    RooRealVar N1("N1", "N1", 10, 0, 1000);
    RooRealVar N2("N2", "N2", 10, 0, 1000);
    RooRealVar a1("a1", "a1", -1e-2, -100, -1e-6);
    RooRealVar a2("a2", "a2", -1, -100, -1e-6);
    RooGenericPdf pow1("pow1", "pow(Mgg,a1)", RooArgSet(Mgg,a1));
    RooGenericPdf pow2("pow2", "pow(Mgg,a2)", RooArgSet(Mgg,a2));
    RooAddPdf model("model", "composite model", RooArgList(pow1,pow2), RooArgList(N1,N2));
    // Fit model to dataset
    RooFitResult *res = model.fitTo(*dsall, Range("R1,R2"), Save(true), SumW2Error(true));
    res->Print("v");
    // Calculate integrals
    RooAbsReal *intw = model.createIntegral(Mgg, NormSet(Mgg), Range("window"));
    RooAbsReal *intb = model.createIntegral(Mgg, NormSet(Mgg), Range("blind"));
    double Ntot = N1.getVal()+N2.getVal();
    // Print fit summary
    cout << "\n\n[Fit summary]\n";
    cout << "Total yield: " << Ntot << endl;
    cout << "Sideband yield: " << Ntot*(1-intb->getVal()) << endl;
    cout << "Window yield: " << Ntot*intw->getVal() << endl;

    model.plotOn(pmgg, Normalization(1.,RooAbsReal::Relative));

    // Create non-extended version of the model for saving
    RooAddPdf nrbkg("nrbkg", "non-resonant bkg", RooArgList(pow1,pow2), RooConst(N1.getVal()/Ntot));
    nrbkg.plotOn(pmgg, NormRange("R1,R2"), LineStyle(kDashed), LineColor(kRed));

    /* 2nd order Bernstein polynomial */
    /*TString model_name = "Bernstein2";
    RooRealVar N0("N0", "N0", 30, 0, 1000);
    RooRealVar N1("N1", "N1", 20, 0, 1000);
    RooRealVar N2("N2", "N2", 1, 0, 1000);
    RooBernstein B0_2("B0_2", "Bernstein0_2", Mgg, RooArgList(rc1,rc0,rc0));
    RooBernstein B1_2("B1_2", "Bernstein1_2", Mgg, RooArgList(rc0,rc1,rc0));
    RooBernstein B2_2("B2_2", "Bernstein2_2", Mgg, RooArgList(rc0,rc0,rc1));
    RooAddPdf model("model", "composite model", RooArgList(B0_2,B1_2,B2_2), RooArgList(N0,N1,N2));
    // Fit model to dataset
    RooFitResult *res = model.fitTo(*dsall, Range("R1,R2"), Save(true), SumW2Error(true));
    res->Print("v");
    // Calculate integrals
    RooAbsReal *intw = model.createIntegral(Mgg, NormSet(Mgg), Range("window"));
    RooAbsReal *intb = model.createIntegral(Mgg, NormSet(Mgg), Range("blind"));
    double Ntot = N0.getVal()+N1.getVal()+N2.getVal();
    // Print fit summary
    cout << "\n\n[Fit summary]\n";
    cout << "Total yield: " << Ntot << endl;
    cout << "Sideband yield: " << Ntot*(1-intb->getVal()) << endl;
    cout << "Window yield: " << Ntot*intw->getVal() << endl;

    model.plotOn(pmgg, Normalization(1.,RooAbsReal::Relative));

    // Create non-extended version of the model for saving
    RooConstVar frac0("frac0", "frac0", N0.getVal()/Ntot);
    RooConstVar frac1("frac1", "frac1", N1.getVal()/Ntot);
    RooAddPdf nrbkg("nrbkg", "non-resonant bkg", RooArgList(B0_2,B1_2,B2_2), RooArgList(frac0,frac1));
    nrbkg.plotOn(pmgg, NormRange("R1,R2"), LineStyle(kDashed), LineColor(kRed));*/

    /* 3rd order Bernstein polynomial */
    /*TString model_name = "Bernstein3";
    RooRealVar N0("N0", "N0", 30, 0, 1000);
    RooRealVar N1("N1", "N1", 20, 0, 1000);
    RooRealVar N2("N2", "N2", 1, 0, 1000);
    RooRealVar N3("N3", "N3", 1, 0, 1000);
    RooBernstein B0_3("B0_3", "Bernstein0_3", Mgg, RooArgList(rc1,rc0,rc0,rc0));
    RooBernstein B1_3("B1_3", "Bernstein1_3", Mgg, RooArgList(rc0,rc1,rc0,rc0));
    RooBernstein B2_3("B2_3", "Bernstein2_3", Mgg, RooArgList(rc0,rc0,rc1,rc0));
    RooBernstein B3_3("B3_3", "Bernstein3_3", Mgg, RooArgList(rc0,rc0,rc0,rc1));
    RooAddPdf model("model", "composite model", RooArgList(B0_3,B1_3,B2_3,B3_3), RooArgList(N0,N1,N2,N3));
    // Fit model to dataset
    RooFitResult *res = model.fitTo(*dsall, Range("R1,R2"), Save(true), SumW2Error(true));
    res->Print("v");
    // Calculate integrals
    RooAbsReal *intw = model.createIntegral(Mgg, NormSet(Mgg), Range("window"));
    RooAbsReal *intb = model.createIntegral(Mgg, NormSet(Mgg), Range("blind"));
    double Ntot = N0.getVal()+N1.getVal()+N2.getVal()+N3.getVal();
    // Print fit summary
    cout << "\n\n[Fit summary]\n";
    cout << "Total yield: " << Ntot << endl;
    cout << "Sideband yield: " << Ntot*(1-intb->getVal()) << endl;
    cout << "Window yield: " << Ntot*intw->getVal() << endl;

    model.plotOn(pmgg, Normalization(1.,RooAbsReal::Relative));

    // Create non-extended version of the model for saving
    RooConstVar frac0("frac0", "frac0", N0.getVal()/Ntot);
    RooConstVar frac1("frac1", "frac1", N1.getVal()/Ntot);
    RooConstVar frac2("frac2", "frac2", N2.getVal()/Ntot);
    RooAddPdf nrbkg("nrbkg", "non-resonant bkg", RooArgList(B0_3,B1_3,B2_3,B3_3), RooArgList(frac0,frac1,frac2));
    nrbkg.plotOn(pmgg, NormRange("R1,R2"), LineStyle(kDashed), LineColor(kRed));*/

    /* Save results */
    TString outdir = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_both/output_BDT_v1/models/nrbkg/";
    // Save plot
    TCanvas *c = new TCanvas();
    pmgg->Draw();
    c->SaveAs(outdir+model_name+".png");
    // Save model
    RooWorkspace *w = new RooWorkspace("w","workspace");
    w->import(nrbkg);
    w->writeToFile(outdir+model_name+".root");
}
