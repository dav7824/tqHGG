using namespace RooFit;

// Cut on BDT score of each channel
double BDT_cut_had = 0.3231;
double BDT_cut_lep = 0.2448;

void makeModel_gaus(TString ch, TString type)
{
    if (ch!="had" && ch!="lep") {
        cout << "[ERROR] Invalid channel!\n";
        return;
    }
    if (type!="Hut" && type!="Hct" && type!="bkg") {
        cout << "[ERROR] Invalid sample type!\n";
        return;
    }

    // Load input trees
    double BDT_cut = 0;
    TString indir;
    TString outdir;
    TChain *T = new TChain("T");
    // Input dir
    if (ch == "had") {
        indir = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_had/output_BDT_v1/";
        BDT_cut = BDT_cut_had;
    } else {
        indir = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_lep/output_BDT_v1/";
        BDT_cut = BDT_cut_lep;
    }
    // Load trees
    if (type == "Hut") {
        outdir = indir + "models/Hut/";
        if (ch == "had") {
            T->Add(indir+"TT_FCNC-TtoHJ_aThad_hut.root");
            T->Add(indir+"TT_FCNC-aTtoHJ_Thad_hut.root");
            T->Add(indir+"ST_FCNC-TH_Thad_hut.root");
        } else {
            T->Add(indir+"TT_FCNC-TtoHJ_aTlep_hut.root");
            T->Add(indir+"TT_FCNC-aTtoHJ_Tlep_hut.root");
            T->Add(indir+"ST_FCNC-TH_Tlep_hut.root");
        }
    } else if (type == "Hct") {
        outdir = indir + "models/Hct/";
        if (ch == "had") {
            T->Add(indir+"TT_FCNC-TtoHJ_aThad_hct.root");
            T->Add(indir+"TT_FCNC-aTtoHJ_Thad_hct.root");
            T->Add(indir+"ST_FCNC-TH_Thad_hct.root");
        } else {
            T->Add(indir+"TT_FCNC-TtoHJ_aTlep_hct.root");
            T->Add(indir+"TT_FCNC-aTtoHJ_Tlep_hct.root");
            T->Add(indir+"ST_FCNC-TH_Tlep_hct.root");
        }
    } else { // type == bkg
        outdir = indir + "models/resbkg/";
        T->Add(indir+"GluGluHToGG.root");
        T->Add(indir+"VBFHToGG.root");
        T->Add(indir+"VHToGG.root");
        T->Add(indir+"ttHJetToGG.root");
    }
    //TString plotdir = outdir + "plots/";
    //TString wsdir = outdir + "wspace/";

    // Create tree variables
    RooRealVar Mgg("Mgg", "M_{#gamma#gamma}", 100, 180);
    RooRealVar BDT_score("BDT_score", "BDT score", 0, -1, 1);
    RooRealVar evt_weight("evt_weight", "event weight", 1, -10, 10);

    // Set RooDataSet
    RooDataSet data("data", "data", RooArgSet(Mgg,BDT_score,evt_weight), WeightVar(evt_weight), Import(*T),
            Cut("100<=Mgg && Mgg<=180"));
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
    Mgg.setRange("window", 115, 135);

    // Create model
    RooRealVar N1("N1", "N1", 5, 1e-6, 50);
    RooRealVar mean("mean", "mean", 127, 105, 145);
    RooRealVar sigma("sigma", "sigma", 1.5, 0.01, 10);
    RooRealVar N2("N2", "N2", 5, 1e-6, 50);
    RooRealVar mean2("mean2", "mean2", 120, 105, 145);
    RooRealVar sigma2("sigma2", "sigma2", 3, 0.01, 10);
    RooRealVar N3("N3", "N3", 5, 1e-6, 50);
    RooRealVar mean3("mean3", "mean3", 124, 105, 145);
    RooRealVar sigma3("sigma3", "sigma3", 1, 0.01, 10);
    RooGaussian gaus1("gaus1", "gaussian 1", Mgg, mean, sigma);
    RooGaussian gaus2("gaus2", "gaussian 2", Mgg, mean2, sigma2);
    RooGaussian gaus3("gaus3", "gaussian 3", Mgg, mean3, sigma3);
    RooAddPdf model("model", "composite model", RooArgList(gaus1,gaus2,gaus3), RooArgList(N1,N2,N3));
    // Fit model to dataset
    RooFitResult *res = model.fitTo(*rdata, Save(true), SumW2Error(true));    // Correct error & cov matrix for weighted likelihood
    res->Print("v");
    double Ntot = N1.getVal()+N2.getVal()+N3.getVal();
    // Plot model
    model.plotOn(pmgg, Normalization(1,RooAbsReal::Relative), NormRange("full"));
    model.plotOn(pmgg, Normalization(1,RooAbsReal::Relative), NormRange("full"), Components("gaus1"), LineStyle(kDashed), LineColor(kRed));
    model.plotOn(pmgg, Normalization(1,RooAbsReal::Relative), NormRange("full"), Components("gaus2"), LineStyle(kDashed), LineColor(kViolet));
    model.plotOn(pmgg, Normalization(1,RooAbsReal::Relative), NormRange("full"), Components("gaus3"), LineStyle(kDashed), LineColor(kGreen));

    // Calculate integrals
    RooAbsReal *intw = model.createIntegral(Mgg, NormSet(Mgg), Range("window"));

    // Print summary to log
    /*FILE *log = fopen((outdir+"param.log").Data(), "a");
    fprintf(log, "%3d    %10f      %10f    %10f    %10f    %10f    %10f    %10f    %10f    %10f    %10f    %10f    %10f\n", cut_idx, BDT_cut,
            N1.getVal(), mean.getVal(), sigma.getVal(), N2.getVal(), mean2.getVal(), sigma2.getVal(),
            N3.getVal(), mean3.getVal(), sigma3.getVal(), Ntot, Ntot*intw->getVal());
    fclose(log);*/

    // Print summary to stdout
    cout << "\n\n[ Summary <cut = " << BDT_cut << "> ]\n";
    cout << "Total yield: " << Ntot << endl;
    cout << "Window yield: " << Ntot * intw->getVal() << endl;

    // Draw model
    TCanvas *c = new TCanvas();
    pmgg->Draw();
    //c->SaveAs(plotdir+Form("cut_%.6f.png",BDT_cut));
    c->SaveAs(outdir+"model.png");

    // Save model to workspace
    RooWorkspace *w = new RooWorkspace("w","workspace");
    w->import(model);
    //w->writeToFile(wsdir+Form("model_%03d.root",cut_idx));
    w->writeToFile(outdir+"model.root");
}
