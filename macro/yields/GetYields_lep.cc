#include "include/Paths.h"
#include "include/utility.h"

TString normdir = dir_res + "normfactor/";

FILE *flog = 0;


void GetYield(TString indir, TString nt, float *yield)
{
    TString fin_name = indir + nt + ".root";
    TString fnorm_name = normdir + nt + ".root";

    cout << "Processing sample: " << nt << endl;
    TFile *fin = new TFile(fin_name);
    TTree *T = (TTree*)fin->Get("T");
    float weight = 0;
    //T->SetBranchAddress("EvtInfo.genweight", &weight);
    T->SetBranchAddress("evtwei_central", &weight);

    yield[0] = 0;  yield[1] = 0;
    for (int evt=0; evt<T->GetEntries(); ++evt) {
        T->GetEntry(evt);
        yield[0] += weight;
        yield[1] += weight * weight;
    }

    float nf = GetNormFactor(fnorm_name.Data());
    yield[0] *= nf;  yield[1] *= nf * nf;

    fin->Close();
}


void GetYield_data(TString indir, float *yield)
{
    TString fin_name = indir + "data.root";

    cout << "Processing sample: data\n";
    TFile *fin = new TFile(fin_name);
    TTree *T = (TTree*)fin->Get("T");

    yield[0] = (float)T->GetEntries();
    yield[1] = yield[0];

    fin->Close();
}


void PrintYield(char *nt, float *yield)
{
    fprintf(flog, "%30s    %.2f +- %.2f\n", nt, yield[0], sqrt(yield[1]));
}


void AddYield(float *accu, float *added)
{
    accu[0] += added[0];
    accu[1] += added[1];
}


void GetYields_lep()
{
    TString indir_name = "Presel_lep_phID";

    TString indir = dir_res + indir_name + '/';
    flog = fopen( (indir+"yields.log").Data(), "w" );

    // Calculate yields
    float yTT_FCNC_TtoHJ_aTlep_hct[2];    GetYield(indir, "TT_FCNC-TtoHJ_aTlep_hct", yTT_FCNC_TtoHJ_aTlep_hct);
    float yTT_FCNC_aTtoHJ_Tlep_hct[2];    GetYield(indir, "TT_FCNC-aTtoHJ_Tlep_hct", yTT_FCNC_aTtoHJ_Tlep_hct);
    float yTT_FCNC_TtoHJ_aTlep_hut[2];    GetYield(indir, "TT_FCNC-TtoHJ_aTlep_hut", yTT_FCNC_TtoHJ_aTlep_hut);
    float yTT_FCNC_aTtoHJ_Tlep_hut[2];    GetYield(indir, "TT_FCNC-aTtoHJ_Tlep_hut", yTT_FCNC_aTtoHJ_Tlep_hut);
    float yST_FCNC_TH_Tlep_hct[2];    GetYield(indir, "ST_FCNC-TH_Tlep_hct", yST_FCNC_TH_Tlep_hct);
    float yST_FCNC_TH_Tlep_hut[2];    GetYield(indir, "ST_FCNC-TH_Tlep_hut", yST_FCNC_TH_Tlep_hut);
    float yDiPhotonJetsBox[2];    GetYield(indir, "DiPhotonJetsBox", yDiPhotonJetsBox);
    float yGJet_Pt_20to40_MGG_80toInf[2];    GetYield(indir, "GJet_Pt-20to40_MGG-80toInf", yGJet_Pt_20to40_MGG_80toInf);
    float yGJet_Pt_20toInf_MGG_40to80[2];    GetYield(indir, "GJet_Pt-20toInf_MGG-40to80", yGJet_Pt_20toInf_MGG_40to80);
    float yGJet_Pt_40toInf_MGG_80toInf[2];    GetYield(indir, "GJet_Pt-40toInf_MGG-80toInf", yGJet_Pt_40toInf_MGG_80toInf);
    float yQCD_Pt_30toInf_MGG_40to80[2];    GetYield(indir, "QCD_Pt-30toInf_MGG-40to80", yQCD_Pt_30toInf_MGG_40to80);
    float yQCD_Pt_40toInf_MGG_80toInf[2];    GetYield(indir, "QCD_Pt-40toInf_MGG-80toInf", yQCD_Pt_40toInf_MGG_80toInf);
    float yTGJets[2];    GetYield(indir, "TGJets", yTGJets);
    float ytW_antitop[2];    GetYield(indir, "tW_antitop", ytW_antitop);
    float ytW_top[2];    GetYield(indir, "tW_top", ytW_top);
    float yTTGG[2];    GetYield(indir, "TTGG", yTTGG);
    float yTTGJets[2];    GetYield(indir, "TTGJets", yTTGJets);
    float yTTJets[2];    GetYield(indir, "TTJets", yTTJets);
    float yTTWJetsToLNu[2];    GetYield(indir, "TTWJetsToLNu", yTTWJetsToLNu);
    float yTTZToLLNuNu[2];    GetYield(indir, "TTZToLLNuNu", yTTZToLLNuNu);
    float yDYJetsToLL[2];    GetYield(indir, "DYJetsToLL", yDYJetsToLL);
    float yWGToLNuG[2];    GetYield(indir, "WGToLNuG", yWGToLNuG);
    float yZGToLLG[2];    GetYield(indir, "ZGToLLG", yZGToLLG);
    float yWW[2];    GetYield(indir, "WW", yWW);
    float yWZ[2];    GetYield(indir, "WZ", yWZ);
    float yZZ[2];    GetYield(indir, "ZZ", yZZ);
    float yGluGluHToGG[2];    GetYield(indir, "GluGluHToGG", yGluGluHToGG);
    float yVBFHToGG[2];    GetYield(indir, "VBFHToGG", yVBFHToGG);
    float yVHToGG[2];    GetYield(indir, "VHToGG", yVHToGG);
    float yttHJetToGG[2];    GetYield(indir, "ttHJetToGG", yttHJetToGG);
    float ydata[2];    GetYield_data(indir, ydata);

    // Category yields
    float yTT_hct_[2] = {0, 0};
    float yTT_hut_[2] = {0, 0};
    float yST_hct_[2] = {0, 0};
    float yST_hut_[2] = {0, 0};
    float yDiPhotonJetsBox_[2] = {0, 0};
    float yGJet_[2] = {0, 0};
    float yQCD_[2] = {0, 0};
    float yTGJets_[2] = {0, 0};
    float ytW_[2] = {0, 0};
    float yTTGG_[2] = {0, 0};
    float yTTGJets_[2] = {0, 0};
    float yTTJets_[2] = {0, 0};
    float yTTV_[2] = {0, 0};
    float yDY_[2] = {0, 0};
    float yVG_[2] = {0, 0};
    float yVV_[2] = {0, 0};
    float yHiggs_[2] = {0, 0};
    float ybkg[2] = {0, 0};
    AddYield(yTT_hct_, yTT_FCNC_TtoHJ_aTlep_hct);
    AddYield(yTT_hct_, yTT_FCNC_aTtoHJ_Tlep_hct);
    AddYield(yTT_hut_, yTT_FCNC_TtoHJ_aTlep_hut);
    AddYield(yTT_hut_, yTT_FCNC_aTtoHJ_Tlep_hut);
    AddYield(yST_hct_, yST_FCNC_TH_Tlep_hct);
    AddYield(yST_hut_, yST_FCNC_TH_Tlep_hut);
    AddYield(yDiPhotonJetsBox_, yDiPhotonJetsBox);
    AddYield(yGJet_, yGJet_Pt_20to40_MGG_80toInf);
    AddYield(yGJet_, yGJet_Pt_20toInf_MGG_40to80);
    AddYield(yGJet_, yGJet_Pt_40toInf_MGG_80toInf);
    AddYield(yQCD_, yQCD_Pt_30toInf_MGG_40to80);
    AddYield(yQCD_, yQCD_Pt_40toInf_MGG_80toInf);
    AddYield(yTGJets_, yTGJets);
    AddYield(ytW_, ytW_antitop);
    AddYield(ytW_, ytW_top);
    AddYield(yTTGG_, yTTGG);
    AddYield(yTTGJets_, yTTGJets);
    AddYield(yTTJets_, yTTJets);
    AddYield(yTTV_, yTTWJetsToLNu);
    AddYield(yTTV_, yTTZToLLNuNu);
    AddYield(yDY_, yDYJetsToLL);
    AddYield(yVG_, yWGToLNuG);
    AddYield(yVG_, yZGToLLG);
    AddYield(yVV_, yWW);
    AddYield(yVV_, yWZ);
    AddYield(yVV_, yZZ);
    AddYield(yHiggs_, yGluGluHToGG);
    AddYield(yHiggs_, yVBFHToGG);
    AddYield(yHiggs_, yVHToGG);
    AddYield(yHiggs_, yttHJetToGG);
    AddYield(ybkg, yDiPhotonJetsBox_);
    AddYield(ybkg, yGJet_);
    AddYield(ybkg, yQCD_);
    AddYield(ybkg, yTGJets_);
    AddYield(ybkg, ytW_);
    AddYield(ybkg, yTTGG_);
    AddYield(ybkg, yTTGJets_);
    AddYield(ybkg, yTTJets_);
    AddYield(ybkg, yTTV_);
    AddYield(ybkg, yDY_);
    AddYield(ybkg, yVG_);
    AddYield(ybkg, yVV_);
    AddYield(ybkg, yHiggs_);

    // Print yields
    PrintYield("TT_FCNC-TtoHJ_aTlep_hct", yTT_FCNC_TtoHJ_aTlep_hct);
    PrintYield("TT_FCNC-aTtoHJ_Tlep_hct", yTT_FCNC_aTtoHJ_Tlep_hct);
    PrintYield("TT_FCNC-TtoHJ_aTlep_hut", yTT_FCNC_TtoHJ_aTlep_hut);
    PrintYield("TT_FCNC-aTtoHJ_Tlep_hut", yTT_FCNC_aTtoHJ_Tlep_hut);
    PrintYield("ST_FCNC-TH_Tlep_hct", yST_FCNC_TH_Tlep_hct);
    PrintYield("ST_FCNC-TH_Tlep_hut", yST_FCNC_TH_Tlep_hut);
    PrintYield("DiPhotonJetsBox", yDiPhotonJetsBox);
    PrintYield("GJet_Pt-20to40_MGG-80toInf", yGJet_Pt_20to40_MGG_80toInf);
    PrintYield("GJet_Pt-20toInf_MGG-40to80", yGJet_Pt_20toInf_MGG_40to80);
    PrintYield("GJet_Pt-40toInf_MGG-80toInf", yGJet_Pt_40toInf_MGG_80toInf);
    PrintYield("QCD_Pt-30toInf_MGG-40to80", yQCD_Pt_30toInf_MGG_40to80);
    PrintYield("QCD_Pt-40toInf_MGG-80toInf", yQCD_Pt_40toInf_MGG_80toInf);
    PrintYield("TGJets", yTGJets);
    PrintYield("tW_antitop", ytW_antitop);
    PrintYield("tW_top", ytW_top);
    PrintYield("TTGG", yTTGG);
    PrintYield("TTGJets", yTTGJets);
    PrintYield("TTJets", yTTJets);
    PrintYield("TTWJetsToLNu", yTTWJetsToLNu);
    PrintYield("TTZToLLNuNu", yTTZToLLNuNu);
    PrintYield("DYJetsToLL", yDYJetsToLL);
    PrintYield("WGToLNuG", yWGToLNuG);
    PrintYield("ZGToLLG", yZGToLLG);
    PrintYield("WW", yWW);
    PrintYield("WZ", yWZ);
    PrintYield("ZZ", yZZ);
    PrintYield("GluGluHToGG", yGluGluHToGG);
    PrintYield("VBFHToGG", yVBFHToGG);
    PrintYield("VHToGG", yVHToGG);
    PrintYield("ttHJetToGG", yttHJetToGG);
    fprintf(flog, "\n\n");
    // Print category yields
    PrintYield("TT Hct", yTT_hct_);
    PrintYield("TT Hut", yTT_hut_);
    PrintYield("ST Hct", yST_hct_);
    PrintYield("ST Hut", yST_hut_);
    PrintYield("DiPhotonJetsBox", yDiPhotonJetsBox_);
    PrintYield("GJet", yGJet_);
    PrintYield("QCD", yQCD_);
    PrintYield("TGJets", yTGJets_);
    PrintYield("tW", ytW_);
    PrintYield("TTGG", yTTGG_);
    PrintYield("TTGJets", yTTGJets_);
    PrintYield("TTJets", yTTJets_);
    PrintYield("TTV", yTTV_);
    PrintYield("DY", yDY_);
    PrintYield("VG", yVG_);
    PrintYield("VV", yVV_);
    PrintYield("SM_Higgs", yHiggs_);
    fprintf(flog, "\n");
    PrintYield("Total bkg", ybkg);
    PrintYield("Data", ydata);

    // Calculate error ratio
    float ratio[2];
    ratio[0] = ybkg[0]/ydata[0] - 1.;  // Define data-MC diff ratio: (MC-data)/data = MC/data - 1
    // Err of (MC/data) - 1 = err of MC/data
    // Error propagation of multiplication: c = a*b -> (dc / c)^2 = (da / a)^2 + (db / b)^2    (assuming a and b are independent of each other)
    ratio[1] = ( ydata[1]/pow(ydata[0],2) + ybkg[1]/pow(ybkg[0],2) ) * pow(ybkg[0]/ydata[0],2);
    fprintf(flog, "%30s    %.1f +- %.1f %%\n", "(MC-data)/data", ratio[0]*100., 100.*sqrt(ratio[1]));

    // Calculate bkg fractions
    fprintf(flog, "\n\n");
    fprintf(flog, "%30s    %.2f %%\n", "DiPhotonJetsBox", yDiPhotonJetsBox_[0]/ybkg[0]*100.);
    fprintf(flog, "%30s    %.2f %%\n", "GJet", yGJet_[0]/ybkg[0]*100.);
    fprintf(flog, "%30s    %.2f %%\n", "QCD", yQCD_[0]/ybkg[0]*100.);
    fprintf(flog, "%30s    %.2f %%\n", "TGJets", yTGJets_[0]/ybkg[0]*100.);
    fprintf(flog, "%30s    %.2f %%\n", "tW", ytW_[0]/ybkg[0]*100.);
    fprintf(flog, "%30s    %.2f %%\n", "TTGG", yTTGG_[0]/ybkg[0]*100.);
    fprintf(flog, "%30s    %.2f %%\n", "TTGJets", yTTGJets_[0]/ybkg[0]*100.);
    fprintf(flog, "%30s    %.2f %%\n", "TTJets", yTTJets_[0]/ybkg[0]*100.);
    fprintf(flog, "%30s    %.2f %%\n", "TTV", yTTV_[0]/ybkg[0]*100.);
    fprintf(flog, "%30s    %.2f %%\n", "DY", yDY_[0]/ybkg[0]*100.);
    fprintf(flog, "%30s    %.2f %%\n", "VG", yVG_[0]/ybkg[0]*100.);
    fprintf(flog, "%30s    %.2f %%\n", "VV", yVV_[0]/ybkg[0]*100.);
    fprintf(flog, "%30s    %.2f %%\n", "SM_Higgs", yHiggs_[0]/ybkg[0]*100.);

    fclose(flog);
}
