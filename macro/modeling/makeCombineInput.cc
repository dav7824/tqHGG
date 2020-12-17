/*
 * (a)
 * Produce pdf for parametric analysis using HiggsCombineLimit package.
 * Parameters are set constant.
 *
 * (b)
 * Prepare observed data for HiggsCombineLimit.
 */

using namespace RooFit;

double BDT_cut_had = 0.3231;
double BDT_cut_lep = 0.2448;

TString dir_sig_had_Hct = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_had/output_BDT_v1/models/Hct/";
TString dir_sig_had_Hut = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_had/output_BDT_v1/models/Hut/";
TString dir_resbkg_had = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_had/output_BDT_v1/models/resbkg/";
TString dir_sig_lep_Hct = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_lep/output_BDT_v1/models/Hct/";
TString dir_sig_lep_Hut = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_lep/output_BDT_v1/models/Hut/";
TString dir_resbkg_lep = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_lep/output_BDT_v1/models/resbkg/";
TString dir_nrbkg = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_both/output_BDT_v1/models/nrbkg/";

TString dir_nt_had = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_had/output_BDT_v1/";
TString dir_nt_lep = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_lep/output_BDT_v1/";

void make_nrbkg_PowerLaw1(TString fin_, TString fout_)
{
    cout << "[INFO] Processing non-resonant background: PowerLaw1\n";
    cout << "[INFO] Input: " << fin_ << endl;
    cout << "[INFO] Output: " << fout_ << endl;

    TFile *fin = new TFile(fin_);
    RooWorkspace *w = (RooWorkspace*)fin->Get("w");
    RooRealVar *a1 = (RooRealVar*)w->var("a1");    a1->setConstant(true);
    RooAbsPdf *nrbkg = (RooAbsPdf*)w->pdf("nrbkg");

    RooWorkspace *w_combine = new RooWorkspace("w_combine", "combine input");
    w_combine->import(*nrbkg);
    w_combine->writeToFile(fout_);
    fin->Close();

    cout << "[INFO] Complete!\n";
}

void make_Gaus3(TString fin_, TString fout_, TString name="model", TString title="composite model")
{
    cout << "[INFO] Processing 3 gaussian pdf\n";
    cout << "[INFO] Input: " << fin_ << endl;
    cout << "[INFO] Output: " << fout_ << endl;

    TFile *fin = new TFile(fin_);
    RooWorkspace *w = (RooWorkspace*)fin->Get("w");
    RooRealVar *N1 = (RooRealVar*)w->var("N1");
    RooRealVar *N2 = (RooRealVar*)w->var("N2");
    RooRealVar *N3 = (RooRealVar*)w->var("N3");
    RooRealVar *mean = (RooRealVar*)w->var("mean");    mean->setConstant(true);
    RooRealVar *mean2 = (RooRealVar*)w->var("mean2");    mean2->setConstant(true);
    RooRealVar *mean3 = (RooRealVar*)w->var("mean3");    mean3->setConstant(true);
    RooRealVar *sigma = (RooRealVar*)w->var("sigma");    sigma->setConstant(true);
    RooRealVar *sigma2 = (RooRealVar*)w->var("sigma2");    sigma2->setConstant(true);
    RooRealVar *sigma3 = (RooRealVar*)w->var("sigma3");    sigma3->setConstant(true);
    RooAbsPdf *gaus1 = (RooAbsPdf*)w->pdf("gaus1");
    RooAbsPdf *gaus2 = (RooAbsPdf*)w->pdf("gaus2");
    RooAbsPdf *gaus3 = (RooAbsPdf*)w->pdf("gaus3");
    double Ntot = N1->getVal() + N2->getVal() + N3->getVal();
    RooAddPdf model(name, title, RooArgList(*gaus1,*gaus2,*gaus3),
            RooArgList(RooConst(N1->getVal()/Ntot),RooConst(N2->getVal()/Ntot)));

    RooWorkspace *w_combine = new RooWorkspace("w_combine", "combine input");
    w_combine->import(model);
    w_combine->writeToFile(fout_);

    fin->Close();

    cout << "[INFO] Complete\n";
}

void makeObsData(TString fin_, TString fout_, TString ch)
{
    TFile *fin = new TFile(fin_);
    TTree *T = (TTree*)fin->Get("T");

    double BDT_cut = 0;
    if (ch == "had") BDT_cut = BDT_cut_had;
    else BDT_cut = BDT_cut_lep;

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

    // Save dataset
    RooWorkspace *w_combine = new RooWorkspace("w_combine", "combine input");
    w_combine->import(*rdata);
    w_combine->writeToFile(fout_);

    fin->Close();
}

void makeCombineInput()
{
    /* Hadronic */
    // Process Hct signal
    //make_Gaus3(dir_sig_had_Hct+"model.root", dir_sig_had_Hct+"model_combine.root");
    // Process Hut signal
    //make_Gaus3(dir_sig_had_Hut+"model.root", dir_sig_had_Hut+"model_combine.root");
    // Process res-bkg
    //make_Gaus3(dir_resbkg_had+"model.root", dir_resbkg_had+"model_combine.root");
    // Obs data
    makeObsData(dir_nt_had+"data.root", dir_nt_had+"models/data_obs.root", "had");

    /* Leptonic */
    // Process Hct signal
    //make_Gaus3(dir_sig_lep_Hct+"model.root", dir_sig_lep_Hct+"model_combine.root");
    // Process Hut signal
    //make_Gaus3(dir_sig_lep_Hut+"model.root", dir_sig_lep_Hut+"model_combine.root");
    // Process res-bkg
    //make_Gaus3(dir_resbkg_lep+"model.root", dir_resbkg_lep+"model_combine.root");
    // Obs data
    makeObsData(dir_nt_lep+"data.root", dir_nt_lep+"models/data_obs.root", "lep");

    // Process non-res bkg (power law 1)
    //make_nrbkg_PowerLaw1(dir_nrbkg+"PowerLaw1.root", dir_nrbkg+"PowerLaw1_combine.root");
}
