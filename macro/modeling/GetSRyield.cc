/*
 * This macro calculates event yields to be used in CombineLimit datacards
 * For signal (Hut & Hct) and resonant bkg, the yields are estimated by summing event weights from MC TTree.
 * For non-resonant bkg, the yields are estimated by summing event weights in Mgg sideband (from data TTree), and scale
 * the result by the assumed non-res bkg model.
 */

using namespace RooFit;

const int Nchannel = 2;
enum eChannel {eHad, eLep};
enum eType {eSigHut, eSigHct, eResBkg, eNrBkg};

// BDT score cut
double BDT_cut[Nchannel] = {0.3231, 0.2448};

// TTree dir
TString indir[Nchannel] = {
    "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_had/output_BDT_v1/",
    "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_lep/output_BDT_v1/"
};
// Non-res bkg model dir
TString moddir = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/model_both/output_BDT_v1/models/nrbkg/";

// Channels
TString channel[Nchannel] = {"hadronic", "leptonic"};
// Sample type
TString samType[4] = {
    "signal_Hut",
    "signal_Hct",
    "res_bkg",
    "non-res_bkg",
};

// Input file names
// Signals
std::vector<TString> fins_sig[Nchannel][2] = {
    // hadronic
    {
        // Hut signal
        {
            "TT_FCNC-TtoHJ_aThad_hut",
            "TT_FCNC-aTtoHJ_Thad_hut",
            "ST_FCNC-TH_Thad_hut",
        },
        // Hct signal
        {
            "TT_FCNC-TtoHJ_aThad_hct",
            "TT_FCNC-aTtoHJ_Thad_hct",
            "ST_FCNC-TH_Thad_hct",
        },
    },
    // leptonic
    {
        // Hut signal
        {
            "TT_FCNC-TtoHJ_aTlep_hut",
            "TT_FCNC-aTtoHJ_Tlep_hut",
            "ST_FCNC-TH_Tlep_hut",
        },
        // Hct signal
        {
            "TT_FCNC-TtoHJ_aTlep_hct",
            "TT_FCNC-aTtoHJ_Tlep_hct",
            "ST_FCNC-TH_Tlep_hct",
        },
    },
};
// Resonant bkg
std::vector<TString> fins_resbkg = {
    "GluGluHToGG",
    "VBFHToGG",
    "VHToGG",
    "ttHJetToGG",
};
// Non-res bkg
std::vector<TString> fins_nrbkg = {
    "data"
};

void Yield_sumEntries(eChannel ch, eType type, TTree *T)
{
    // Create tree variables
    RooRealVar Mgg("Mgg", "M_{#gamma#gamma}", 100, 180);
    RooRealVar BDT_score("BDT_score", "BDT score", 0, -1, 1);
    RooRealVar evt_weight("evt_weight", "event weight", 1, -10, 10);

    // Set RooDataSet
    RooDataSet data("data", "data", RooArgSet(Mgg,BDT_score,evt_weight), WeightVar(evt_weight), Import(*T),
            Cut("100<=Mgg && Mgg<=180"));
    const char *cut_expr = Form("BDT_score>=%f", BDT_cut[ch]);
    // New dataset with BDT cut
    RooDataSet *rdata = (RooDataSet*)data.reduce(Cut(cut_expr));

    // Print yield to log
    FILE *out = fopen( (indir[ch]+"models/combine_yield.txt").Data(), "a" );
    fprintf(out, "%s_%s\t%.3f\n", samType[type].Data(), ch==0 ? "had":"lep", rdata->sumEntries());
    fclose(out);

    delete rdata;
}

void Yield_nrbkg_v1(eChannel ch, TTree *T, TString model_name)
{
    // Open model file
    TFile *fmodel = TFile::Open(moddir+model_name+"_combine.root");
    if (!fmodel) {
        cout << "[ERROR] Function model not found: " << model_name << endl;
        return;
    }

    // Create tree variables
    RooRealVar Mgg("Mgg", "M_{#gamma#gamma}", 100, 180);
    RooRealVar BDT_score("BDT_score", "BDT score", 0, -1, 1);
    RooRealVar evt_weight("evt_weight", "event weight", 1, -10, 10);
    // Get function model
    Mgg.setRange("blind", 120, 130);
    RooWorkspace *w = (RooWorkspace*)fmodel->Get("w_combine");
    RooAbsPdf *model = w->pdf("nrbkg");
    RooAbsReal *intb = model->createIntegral(Mgg, NormSet(Mgg), Range("blind"));

    // Set RooDataSet
    RooDataSet data("data", "data", RooArgSet(Mgg,BDT_score,evt_weight), WeightVar(evt_weight), Import(*T),
            Cut("(100<=Mgg && Mgg<=120) || (130<=Mgg && Mgg<=180)"));
    const char *cut_expr = Form("BDT_score>=%f", BDT_cut[ch]);
    // New dataset with BDT cut
    RooDataSet *rdata = (RooDataSet*)data.reduce(Cut(cut_expr));
    double yield = rdata->sumEntries() / (1 - intb->getVal());

    // Print yield to log
    FILE *out = fopen( (indir[ch]+"models/combine_yield.txt").Data(), "a" );
    fprintf(out, "Non-res_bkg_%s\t%.3f\n", ch==0 ? "had":"lep", yield);
    fclose(out);

    delete rdata;
    delete intb;
    delete fmodel;
}

void CalcYield(eChannel ch, eType type, TString model = "")
{
    cout << "Processing: " << channel[ch] << "    " << samType[type] << endl;

    std::vector<TString> *fins_ = 0;
    switch (type) {
        case eSigHut:
        case eSigHct:
            fins_ = &fins_sig[ch][type];
            break;
        case eResBkg:
            fins_ = &fins_resbkg;
            break;
        case eNrBkg:
            fins_ = &fins_nrbkg;
            break;
    }

    // Get input tree
    TChain *T = new TChain("T");
    for (int i=0; i<fins_->size(); ++i)
        T->Add( indir[ch]+fins_->at(i)+".root" );

    // Calculate yields
    if (type<3)  Yield_sumEntries(ch, type, T);
    else Yield_nrbkg_v1(ch, T, model);

    delete T;
}

void GetSRyield()
{
    CalcYield(eHad, eSigHut);
    CalcYield(eHad, eSigHct);
    CalcYield(eHad, eResBkg);
    CalcYield(eHad, eNrBkg, "PowerLaw1");
    CalcYield(eLep, eSigHut);
    CalcYield(eLep, eSigHct);
    CalcYield(eLep, eResBkg);
    CalcYield(eLep, eNrBkg, "PowerLaw1");
}
