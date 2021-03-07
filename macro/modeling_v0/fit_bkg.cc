using namespace RooFit;

//TString model_name = "PowerLaw1";
TString model_name = "PowerLaw2";
//TString model_name = "Exponential2";
//TString model_name = "Bernstein3";

void fit_bkg(TString ch)
{
    // Set paths
    TString dir_had = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_had/output_BDT_v1/";
    TString dir_lep = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_lep/output_BDT_v1/";
    TString dir_both = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_both/output_BDT_v1/";
    TString outdir = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_both/output_BDT_v1/models/";
    TFile *fin_resbkg = 0;
    TFile *fin_nrbkg = 0;
    TFile *fin_dataset = 0;
    double BDT_cut = 0.;
    // Read files
    fin_nrbkg = new TFile(dir_both+"models/nrbkg/"+model_name+".root");
    if (ch == "had") {
        fin_resbkg = new TFile(dir_had+"models/resbkg/model.root");
        fin_dataset = new TFile(dir_had+"data.root");
        BDT_cut = 0.3231;
    } else if (ch == "lep") {
        fin_resbkg = new TFile(dir_lep+"models/resbkg/model.root");
        fin_dataset = new TFile(dir_lep+"data.root");
        BDT_cut = 0.2448;
    } else {
        cout << "[ERROR] Invalid channel\n";
        return;
    }

    // Create tree variables
    RooRealVar Mgg("Mgg", "M_{#gamma#gamma}", 100, 180);
    RooRealVar BDT_score("BDT_score", "BDT score", 0, -1, 1);
    RooRealVar evt_weight("evt_weight", "event weight", 1, -10, 10);

    // Get dataset
    TTree *T = (TTree*)fin_dataset->Get("T");
    RooDataSet data("data", "data", RooArgSet(Mgg,BDT_score,evt_weight), WeightVar(evt_weight), Import(*T),
            Cut("100<=Mgg && Mgg<=180"));
    const char *cut_expr = "BDT_score>=%f";
    RooDataSet *rdata = (RooDataSet*)data.reduce( Form(cut_expr,BDT_cut) );

    // Get resonant bkg pdf
    RooWorkspace *w_resbkg = (RooWorkspace*)fin_resbkg->Get("w");
    RooRealVar *N1 = w_resbkg->var("N1");
    RooRealVar *N2 = w_resbkg->var("N2");
    RooRealVar *N3 = w_resbkg->var("N3");
    RooRealVar *mean = w_resbkg->var("mean");  mean->setConstant(true);
    RooRealVar *mean2 = w_resbkg->var("mean2");  mean2->setConstant(true);
    RooRealVar *mean3 = w_resbkg->var("mean3");  mean3->setConstant(true);
    RooRealVar *sigma = w_resbkg->var("sigma");  sigma->setConstant(true);
    RooRealVar *sigma2 = w_resbkg->var("sigma2");  sigma2->setConstant(true);
    RooRealVar *sigma3 = w_resbkg->var("sigma3");  sigma3->setConstant(true);
    RooAbsPdf *gaus1 = w_resbkg->pdf("gaus1");
    RooAbsPdf *gaus2 = w_resbkg->pdf("gaus2");
    RooAbsPdf *gaus3 = w_resbkg->pdf("gaus3");
    double Ntot = N1->getVal() + N2->getVal() + N3->getVal();
    // Make pdf not extended
    RooAddPdf pdf_resbkg("resbkg", "resbkg", RooArgList(*gaus1,*gaus2,*gaus3),
            RooArgList( RooConst(N1->getVal()/Ntot), RooConst(N2->getVal()/Ntot) ));
    // Get non-res bkg pdf
    RooWorkspace *w_nrbkg = (RooWorkspace*)fin_nrbkg->Get("w");
    /* PowerLaw1 */
    /*RooRealVar *a1 = w_nrbkg->var("a1");  a1->setConstant(true);
    RooAbsPdf *pdf_nrbkg = w_nrbkg->pdf("nrbkg");*/
    /* PowerLaw2 */
    RooRealVar *a1 = w_nrbkg->var("a1");  a1->setConstant(true);
    RooRealVar *a2 = w_nrbkg->var("a2");  a2->setConstant(true);
    RooAbsPdf *pdf_nrbkg = w_nrbkg->pdf("nrbkg");
    /* Exponential2 */
    /*RooRealVar *tau1 = w_nrbkg->var("tau1");  tau1->setConstant(true);
    RooRealVar *tau2 = w_nrbkg->var("tau2");  tau2->setConstant(true);
    RooAbsPdf *pdf_nrbkg = w_nrbkg->pdf("nrbkg");*/
    /* Bernstein3 */
    //RooAbsPdf *pdf_nrbkg = w_nrbkg->pdf("nrbkg");

    // Create total (res+non-res) bkg model
    RooRealVar Nres("Nres", "Nres", 1, 0, 1000);
    RooRealVar Nnr("Nnr", "Nnr", 1, 0, 1000);
    RooAddPdf pdf_bkg("pdf_bkg", "total bkg", RooArgList(pdf_resbkg,*pdf_nrbkg), RooArgList(Nres,Nnr));
    // Fit to dataset
    RooFitResult *res = pdf_bkg.fitTo(*rdata, Save(true), SumW2Error(true));
    res->Print("v");

    // Make plot
    RooPlot *pmgg = Mgg.frame();
    rdata->plotOn(pmgg);
    pdf_bkg.plotOn(pmgg, Normalization(1,RooAbsReal::Relative));
    pdf_bkg.plotOn(pmgg, Normalization(1,RooAbsReal::Relative), Components("resbkg"), LineStyle(kDashed), LineColor(kRed));
    pdf_bkg.plotOn(pmgg, Normalization(1,RooAbsReal::Relative), Components("nrbkg"), LineStyle(kDashed), LineColor(kGreen));
    TCanvas *c = new TCanvas();
    pmgg->Draw();
    c->SaveAs(outdir+"fit/"+model_name+'_'+ch+".png");

    // Save model
    RooWorkspace *w = new RooWorkspace("w", "workspace");
    w->import(pdf_bkg);
    w->writeToFile(outdir+"fit/"+model_name+'_'+ch+".root");
}
