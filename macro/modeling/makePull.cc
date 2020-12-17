using namespace RooFit;

TRandom3 rnd;

double gen_times = 10;

TString model_name = "PowerLaw1";
//TString model_name = "PowerLaw2";
//TString model_name = "Exponential2";
//TString model_name = "Bernstein3";

void makePull(TString ch)
{
    TString fin_sig_ = Form("/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_%s/output_BDT_v1/models/Hut/model.root", ch.Data());
    TString fin_bkg_ = Form("/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_both/output_BDT_v1/models/fit/%s_%s.root", model_name.Data(), ch.Data());
    TString outdir = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_both/output_BDT_v1/models/pull_v2/";

    TFile *fin_sig = new TFile(fin_sig_);
    TFile *fin_bkg = new TFile(fin_bkg_);
    RooWorkspace *w_sig = (RooWorkspace*)fin_sig->Get("w");
    RooWorkspace *w_bkg = (RooWorkspace*)fin_bkg->Get("w");
    // Get signal pdf
    RooRealVar *Mgg = w_sig->var("Mgg");
    RooRealVar *N1 = w_sig->var("N1");
    RooRealVar *N2 = w_sig->var("N2");
    RooRealVar *N3 = w_sig->var("N3");
    RooRealVar *mean = w_sig->var("mean");  mean->setConstant(true);
    RooRealVar *mean2 = w_sig->var("mean2");  mean2->setConstant(true);
    RooRealVar *mean3 = w_sig->var("mean3");  mean3->setConstant(true);
    RooRealVar *sigma = w_sig->var("sigma");  sigma->setConstant(true);
    RooRealVar *sigma2 = w_sig->var("sigma2");  sigma2->setConstant(true);
    RooRealVar *sigma3 = w_sig->var("sigma3");  sigma3->setConstant(true);
    RooAbsPdf *gaus1 = w_sig->pdf("gaus1");
    RooAbsPdf *gaus2 = w_sig->pdf("gaus2");
    RooAbsPdf *gaus3 = w_sig->pdf("gaus3");
    double Ntot = N1->getVal()+N2->getVal()+N3->getVal();
    // Make unextended signal pdf
    RooAddPdf pdf_sig("pdf_sig", "signal", RooArgList(*gaus1,*gaus2,*gaus3),
            RooArgList( RooConst(N1->getVal()/Ntot), RooConst(N2->getVal()/Ntot) ));
    // Get bkg pdf
    RooRealVar *Nres = w_bkg->var("Nres");  Nres->setConstant(true);
    RooRealVar *Nnr = w_bkg->var("Nnr");  Nnr->setConstant(true);
    double nbkg = Nres->getVal() + Nnr->getVal();
    RooAbsPdf *resbkg = w_bkg->pdf("resbkg");
    RooAbsPdf *nrbkg = w_bkg->pdf("nrbkg");
    // Make unextended bkg pdf
    RooAddPdf pdf_bkg("pdf_bkg", "background", RooArgList(*resbkg,*nrbkg), RooConst(Nres->getVal()/nbkg));

    // Set sig+bkg model
    RooRealVar Nsig("Nsig", "Nsig", 1, -100, 100);
    RooRealVar Nbkg("Nbkg", "Nbkg", 1, 0, 10000);
    RooAddPdf pdf_tot("pdf_tot", "total pdf", RooArgList(pdf_sig,pdf_bkg), RooArgList(Nsig,Nbkg));

    // Pull variable
    RooRealVar pull("pull", "pull Nsig", 0, -3, 3);
    RooDataSet dspull("dspull", "dspull", pull);

    /* Original setup */
    /*if (ch == "had") gen_times = 1.5;
    else gen_times = 100;*/

    // Make directory for pseudo fit results
    TString fitdir = outdir+Form("toyfit_%s_pull-%.2f/",ch.Data(),gen_times);
    gSystem->mkdir(fitdir);

    // Run pseudo-experiments
    for (int i=0; i<1000; ++i) {
        int ntoy = rnd.Poisson(nbkg*gen_times);
        RooDataSet *toy = pdf_bkg.generate(*Mgg, ntoy);
        pdf_tot.fitTo(*toy, SumW2Error(true));
        pull = Nsig.getVal() / Nsig.getError();
        dspull.add(pull);

        if (i<50) {
            TCanvas c;
            RooPlot *pl = Mgg->frame();
            toy->plotOn(pl, Binning(20));
            pdf_tot.plotOn(pl);
            pdf_tot.plotOn(pl, Components(pdf_bkg), LineStyle(kDashed), LineColor(kGreen));
            pdf_tot.plotOn(pl, Components(pdf_sig), LineStyle(kDashed), LineColor(kRed));
            pl->Draw();
            c.SaveAs(fitdir+Form("toy_%d.png",i));
            delete pl;
        }

        /*RooPlot *pmgg = Mgg->frame();
        toy->plotOn(pmgg);
        pdf_tot.plotOn(pmgg, Normalization(1,RooAbsReal::Relative));
        pdf_tot.plotOn(pmgg, Normalization(1,RooAbsReal::Relative), Components("pdf_sig"), LineStyle(kDashed), LineColor(kRed));
        pdf_tot.plotOn(pmgg, Normalization(1,RooAbsReal::Relative), Components("pdf_bkg"), LineStyle(kDashed), LineColor(kGreen));
        pmgg->Draw();*/
        delete toy;
    } // End of sample loop

    // Fit pull distribution
    RooRealVar mupu("mupu", "pull pdf mean", 0.5, -3, 3);
    RooRealVar sigmapu("sigmapu", "pull pdf sigma", 1, 1e-6, 5);
    RooGaussian pdf_pull("pdf_pull", "pull PDF", pull, mupu, sigmapu);
    RooFitResult *res_pull = pdf_pull.fitTo(dspull, Save(true));
    // Draw pull result
    RooPlot *ppull = pull.frame();
    dspull.plotOn(ppull, Binning(24));
    pdf_pull.plotOn(ppull);
    TCanvas *c = new TCanvas();
    ppull->Draw();
    c->SaveAs(outdir+model_name+'_'+ch+Form("_pull-%.2f",gen_times)+".png");
    res_pull->Print("v");
    // Print summary
    ofstream output((outdir+model_name+'_'+ch+Form("_pull-%.2f",gen_times)+".txt").Data(), ios::out);
    output << "mean: " << mupu.getVal() << " +- " << mupu.getError() << endl;
    output << "sigma: " << sigmapu.getVal() << " +- " << sigmapu.getError() << endl;
    output.close();
}
