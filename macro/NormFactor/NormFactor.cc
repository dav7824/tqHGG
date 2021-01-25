/*
 * Calculate norm factors of MC samples.
 *
 * Cross section reference:
 * Signal: https://twiki.cern.ch/twiki/bin/viewauth/CMS/THFCNC#Data_and_simulation_samples_2017
 * Background: XS_17.txt (https://github.com/cms-analysis/flashgg/blob/dev_legacy_runII/MetaData/data/cross_sections.json)
 */

#include "include/Paths.h"

// norm factor dir
TString outdir = dir_res + "normfactor";
// sum of weight dir
TString weidir = dir_res + "sumweight";
// bash script processing multiple files
TString sh_multi = dir_tqHGG + "macro/NormFactor/SumWeight.sh";

// luminosity 2017
float Lumi = 41.53;

// branching fractions
float BR_tqH = 0.001;
float BR_Hgg = 0.0023;

// MC sample specification
struct MCSpec
{
    MCSpec() : filename(""), XS(0) {}    // std::map<TString,MCSpec> cannot be compiled without this constuctor
    MCSpec(TString filename_, float XS_) :
        filename(filename_), XS(XS_) {}

    TString filename;
    float XS;
};

// Functions
void ProcessSingle(TString ntdir, TString ntname);
void ProcessMulti(TString ntdir, TString ntname);

// MC sample info
map<TString, MCSpec> mc;

// Pointer of log file
FILE *flog = 0;


void NormFactor()
{
    if (gSystem->AccessPathName(outdir)) gSystem->mkdir(outdir);
    if (gSystem->AccessPathName(weidir)) gSystem->mkdir(weidir);
    flog = fopen( Form("%s/normfactor.log",outdir.Data()), "a" );

    // Note:
    // Putting these lines outside of NormFactor() will produce error for ROOT
    // interpreter
    // Literal numbers are double by default. If initializer list (curly
    // brakets) is used to create MCSpec, "literal * variable" expression
    // won't be converted to float automatically and will cause error in
    // compilation. So MCSpec constructors are used explicitly.
    // signal
    mc["TT_FCNC-TtoHJ_aThad_hct"] = MCSpec( "TT_FCNC-TtoHJ_aThadronic_HToaa_eta_hct-MadGraph5-pythia8", 0.23*BR_tqH );
    mc["TT_FCNC-aTtoHJ_Thad_hct"] = MCSpec( "TT_FCNC-aTtoHJ_Thadronic_HToaa_eta_hct-MadGraph5-pythia8", 0.23*BR_tqH );
    mc["TT_FCNC-TtoHJ_aThad_hut"] = MCSpec( "TT_FCNC-TtoHJ_aThadronic_HToaa_eta_hut-MadGraph5-pythia8", 0.23*BR_tqH );
    mc["TT_FCNC-aTtoHJ_Thad_hut"] = MCSpec( "TT_FCNC-aTtoHJ_Thadronic_HToaa_eta_hut-MadGraph5-pythia8", 0.23*BR_tqH );
    mc["TT_FCNC-TtoHJ_aTlep_hct"] = MCSpec( "TT_FCNC-T2HJ_aTleptonic_HToaa_eta_hct-MadGraph5-pythia8", 0.11*BR_tqH );
    mc["TT_FCNC-aTtoHJ_Tlep_hct"] = MCSpec( "TT_FCNC-aTtoHJ_Tleptonic_HToaa_eta_hct-MadGraph5-pythia8", 0.11*BR_tqH );
    mc["TT_FCNC-TtoHJ_aTlep_hut"] = MCSpec( "TT_FCNC-TtoHJ_aTleptonic_HToaa_eta_hut-MadGraph5-pythia8", 0.11*BR_tqH );
    mc["TT_FCNC-aTtoHJ_Tlep_hut"] = MCSpec( "TT_FCNC-aTtoHJ_Tleptonic_HToaa_eta_hut-MadGraph5-pythia8", 0.11*BR_tqH );
    mc["ST_FCNC-TH_Thad_hct"] = MCSpec( "ST_FCNC-TH_Thadronic_HToaa_eta_hct-MadGraph5-pythia8", 0.019*BR_tqH );
    mc["ST_FCNC-TH_Thad_hut"] = MCSpec( "ST_FCNC-TH_Thadronic_HToaa_eta_hut-MadGraph5-pythia8", 0.139*BR_tqH );
    mc["ST_FCNC-TH_Tlep_hct"] = MCSpec( "ST_FCNC-TH_Tleptonic_HToaa_eta_hct-MadGraph5-pythia8", 0.009*BR_tqH );
    mc["ST_FCNC-TH_Tlep_hut"] = MCSpec( "ST_FCNC-TH_Tleptonic_HToaa_eta_hut-MadGraph5-pythia8", 0.067*BR_tqH );
    // non-res bkg
    mc["DiPhotonJetsBox"] = MCSpec( "DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa", 84.4 );
    mc["GJet_Pt-20to40_MGG-80toInf"] = MCSpec( "GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8", 232.9 );
    mc["GJet_Pt-20toInf_MGG-40to80"] = MCSpec( "GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8", 3164. );
    mc["GJet_Pt-40toInf_MGG-80toInf"] = MCSpec( "GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8", 872.1 );
    mc["QCD_Pt-30to40_MGG-80toInf"] = MCSpec( "QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8", 24810. );
    mc["QCD_Pt-30toInf_MGG-40to80"] = MCSpec( "QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8", 241400. );
    mc["QCD_Pt-40toInf_MGG-80toInf"] = MCSpec( "QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8", 118100. );
    mc["TGJets"] = MCSpec( "TGJets_TuneCP5_13TeV_amcatnlo_madspin_pythia8", 3.055 );
    mc["tW_antitop"] = MCSpec( "ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8", 34.97 );
    mc["tW_top"] = MCSpec( "ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8", 34.91 );
    mc["TTGG"] = MCSpec( "TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8", 0.01687 );
    mc["TTGJets"] = MCSpec( "TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8", 4.078 );
    mc["TTJets"] = MCSpec( "TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8", 831.76 );
    mc["TTWJetsToLNu"] = MCSpec( "TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8", 0.2149 );
    mc["TTZToLLNuNu"] = MCSpec( "TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8", 0.2432 );
    mc["DYJetsToLL"] = MCSpec( "DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8", 6529.0 );
    mc["WGToLNuG"] = MCSpec( "WGToLNuG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8", 191.4 );
    mc["ZGToLLG"] = MCSpec( "ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8", 55.6 );
    mc["WW"] = MCSpec( "WW_TuneCP5_13TeV-pythia8", 75.8 );
    mc["WZ"] = MCSpec( "WZ_TuneCP5_13TeV-pythia8", 27.6 );
    mc["ZZ"] = MCSpec( "ZZ_TuneCP5_13TeV-pythia8", 12.14 );
    // resonant bkg
    mc["GluGluHToGG"] = MCSpec( "GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8", 48.58*BR_Hgg );
    mc["VBFHToGG"] = MCSpec( "VBFHToGG_M125_13TeV_amcatnlo_pythia8", 3.782*BR_Hgg );
    mc["VHToGG"] = MCSpec( "VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8", 2.257*BR_Hgg );
    mc["ttHJetToGG"] = MCSpec( "ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8", 0.5071*BR_Hgg );

    // Run
    ProcessMulti(dir_nt_2017_Higgs, "TT_FCNC-TtoHJ_aThad_hct");
    ProcessMulti(dir_nt_2017_Higgs, "TT_FCNC-aTtoHJ_Thad_hct");
    ProcessMulti(dir_nt_2017_Higgs, "TT_FCNC-TtoHJ_aThad_hut");
    ProcessMulti(dir_nt_2017_Higgs, "TT_FCNC-aTtoHJ_Thad_hut");
    ProcessMulti(dir_nt_2017_Higgs, "TT_FCNC-TtoHJ_aTlep_hct");
    ProcessMulti(dir_nt_2017_Higgs, "TT_FCNC-aTtoHJ_Tlep_hct");
    ProcessMulti(dir_nt_2017_Higgs, "TT_FCNC-TtoHJ_aTlep_hut");
    ProcessMulti(dir_nt_2017_Higgs, "TT_FCNC-aTtoHJ_Tlep_hut");
    ProcessMulti(dir_nt_2017_Higgs, "ST_FCNC-TH_Thad_hct");
    ProcessMulti(dir_nt_2017_Higgs, "ST_FCNC-TH_Thad_hut");
    ProcessMulti(dir_nt_2017_Higgs, "ST_FCNC-TH_Tlep_hct");
    ProcessMulti(dir_nt_2017_Higgs, "ST_FCNC-TH_Tlep_hut");
    ProcessMulti(dir_nt_2017, "DiPhotonJetsBox");
    ProcessSingle(dir_nt_2017, "GJet_Pt-20to40_MGG-80toInf");
    ProcessSingle(dir_nt_2017, "GJet_Pt-20toInf_MGG-40to80");
    ProcessSingle(dir_nt_2017, "GJet_Pt-40toInf_MGG-80toInf");
    ProcessSingle(dir_nt_2017, "QCD_Pt-30to40_MGG-80toInf");
    ProcessSingle(dir_nt_2017, "QCD_Pt-30toInf_MGG-40to80");
    ProcessSingle(dir_nt_2017, "QCD_Pt-40toInf_MGG-80toInf");
    ProcessSingle(dir_nt_2017, "TGJets");
    ProcessSingle(dir_nt_2017, "tW_antitop");
    ProcessSingle(dir_nt_2017, "tW_top");
    ProcessSingle(dir_nt_2017, "TTGG");
    ProcessSingle(dir_nt_2017, "TTGJets");
    ProcessSingle(dir_nt_2017, "TTJets");
    ProcessSingle(dir_nt_2017, "TTWJetsToLNu");
    ProcessSingle(dir_nt_2017, "TTZToLLNuNu");
    ProcessSingle(dir_nt_2017, "DYJetsToLL");
    ProcessSingle(dir_nt_2017, "WGToLNuG");
    ProcessSingle(dir_nt_2017, "ZGToLLG");
    ProcessSingle(dir_nt_2017, "WW");
    ProcessSingle(dir_nt_2017, "WZ");
    ProcessSingle(dir_nt_2017, "ZZ");
    ProcessMulti(dir_nt_2017_Higgs, "GluGluHToGG");
    ProcessMulti(dir_nt_2017_Higgs, "VBFHToGG");
    ProcessMulti(dir_nt_2017_Higgs, "VHToGG");
    ProcessMulti(dir_nt_2017_Higgs, "ttHJetToGG");

    fclose(flog);
}

void ProcessSingle(TString ntdir, TString ntname)
{
    // Get weight of sample
    TString fname = ntdir + '/' + mc[ntname].filename + ".root";
    TFile *fin = new TFile(fname);
    TH1D *hist = (TH1D*)fin->Get("flashggNtuples/h1_weight");
    // Calculate sum of weight
    float nf = mc[ntname].XS * 1000. * Lumi / hist->GetBinContent(1);
    fprintf(flog, "%20s    %g\n", ntname.Data(), nf);
    fin->Close();

    // Save result
    TFile *fout = new TFile(outdir+'/'+ntname+".root", "recreate");
    TH1D *hnf = new TH1D("normfactor", "", 1, 0, 1);
    hnf->SetBinContent(1, nf);
    fout->Write();
    fout->Close();
}

void ProcessMulti(TString ntdir, TString ntname)
{
    TString indir = ntdir + '/' + mc[ntname].filename;
    TString fsumwei = weidir + '/' + ntname + ".root";
    // Add weight in each root file
    gSystem->Exec( Form("%s %s %s",sh_multi.Data(),indir.Data(),fsumwei.Data()) );

    // Read file of sum of weight
    TFile *fin = new TFile(fsumwei);
    TH1D *hist = (TH1D*)fin->Get("hsumwei");
    // Calculate norm factor
    float nf = mc[ntname].XS * 1000. * Lumi / hist->GetBinContent(1);
    fprintf(flog, "%20s    %g\n", ntname.Data(), nf);
    fin->Close();

    // Save result
    TFile* fout = new TFile(outdir+'/'+ntname+".root", "recreate");
    TH1D *hnf = new TH1D("normfactor", "", 1, 0, 1);
    hnf->SetBinContent(1, nf);
    fout->Write();
    fout->Close();
}
