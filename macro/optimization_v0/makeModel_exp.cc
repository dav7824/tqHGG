using namespace RooFit;

void makeModel_exp(TString ch, int cut_idx, double BDT_cut)
{
    if (ch!="had" && ch!="lep") {
        cout << "[ERROR] Invalid channel!\n";
        return;
    }

    // Load input trees
    TString indir;
    TString outdir;
    TChain *T = new TChain("T");
    // Input dir
    if (ch == "had") {
        indir = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/optimization_had/output_BDT_v1/";
    } else {
        indir = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/optimization_lep/output_BDT_v1/";
    }
    // Load trees
    T->Add(indir+"data.root");
    // Output dir
    outdir = indir + "models/nrbkg/";
    TString plotdir = outdir + "plots/";
    TString wsdir = outdir + "wspace/";

    // Create tree variables
    RooRealVar Mgg("Mgg", "M_{#gamma#gamma}", 100, 180);
    RooRealVar BDT_score("BDT_score", "BDT score", 0, -1, 1);
    RooRealVar evt_weight("evt_weight", "event weight", 1, -10, 10);

    // Set RooDataSet
    RooDataSet data("data", "data", RooArgSet(Mgg,BDT_score,evt_weight), WeightVar(evt_weight), Import(*T),
            Cut("(100<=Mgg && Mgg<=120) || (130<=Mgg && Mgg<=180)"));
    const char *cut_expr = Form("BDT_score>=%f", BDT_cut);
    // New dataset with BDT cut
    RooDataSet *rdata = (RooDataSet*)data.reduce(Cut(cut_expr));
    cout << "\n\n[Dataset summary]\n";
    cout << "# dataset entries: " << rdata->numEntries() << endl;
    cout << "Dataset yield: " << rdata->sumEntries() << endl;

    // Make plots
    RooPlot *pmgg = Mgg.frame();
    // Plot dataset
    rdata->plotOn(pmgg);

    // Mgg ranges
    Mgg.setRange("full", 100, 180);
    Mgg.setRange("R1", 100, 120);
    Mgg.setRange("R2", 130, 180);
    Mgg.setRange("window", 115, 135);
    Mgg.setRange("blind", 120, 130);

    // Create model
    RooRealVar tau1("tau1", "tau1", -0.02, -0.5, -1e-6);
    RooRealVar tau2("tau2", "tau2", -0.01, -0.5, -1e-6);
    RooRealVar N1("N1", "N1", 2500, 0.01, 50000);
    RooRealVar N2("N2", "N2", 1500, 0.01, 50000);
    RooExponential exp1("exp1", "exp1", Mgg, tau1);
    RooExponential exp2("exp2", "exp2", Mgg, tau2);
    RooAddPdf model("model", "composite model", RooArgList(exp1,exp2), RooArgList(N1,N2));
    // Fit model to dataset
    RooFitResult *res = model.fitTo(*rdata, Range("R1,R2"), Save(true), SumW2Error(true));    // Correct error & cov matrix for weighted likelihood
    res->Print("v");
    // Plot model
    // Components are not plotted since there are some normalization problems due to the presence of blind region
    model.plotOn(pmgg, /*NormRange("R1,R2"),*/ Normalization(1.,RooAbsReal::Relative));

    // Calculate integrals
    RooAbsReal *intw = model.createIntegral(Mgg, NormSet(Mgg), Range("window"));
    RooAbsReal *intb = model.createIntegral(Mgg, NormSet(Mgg), Range("blind"));

    double Ntot = N1.getVal()+N2.getVal();

    // Print summary to log
    FILE *log = fopen((outdir+"param.log").Data(), "a");
    fprintf(log, "%3d    %10f      %10f    %10f    %10f    %10f    %10f    %10f    %10f\n", cut_idx, BDT_cut,
            N1.getVal(), tau1.getVal(), N2.getVal(), tau2.getVal(), Ntot, Ntot*(1-intb->getVal()), Ntot*intw->getVal());
    fclose(log);

    // Draw model
    TCanvas *c = new TCanvas();
    pmgg->Draw();
    c->SaveAs(plotdir+Form("cut_%.6f.png",BDT_cut));

    // Save model to workspace
    RooWorkspace *w = new RooWorkspace("w","workspace");
    w->import(model);
    w->writeToFile(wsdir+Form("model_%03d.root",cut_idx));
}
