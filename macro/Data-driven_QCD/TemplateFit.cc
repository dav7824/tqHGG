/*
 * Fill photon IDMVA histograms for each sample and perform template fit to decide scale factors
 * on GGJets sample and data-driven QCD sample.
 */

#include "include/Paths.h"
#include "include/utility.h"

TString histdir = dir_res + "Data-driven_QCD/hist/";
TString ntdir = dir_res + "Presel_had_phID_btag-L/";
TString normdir = dir_res + "normfactor/";


//////////////////////////////////////////////////
// Fill photon IDMVA histograms                 //
//////////////////////////////////////////////////

// Create empty histograms
void CreateHist(TH1D **hist, int nbin)
{
    hist[0] = new TH1D("hidmva_max", ";max #gamma IDMVA;events", nbin, -0.7, 1.);
    hist[1] = new TH1D("hidmva_min", ";min #gamma IDMVA;events", nbin, -0.7, 1.);
    hist[0]->Sumw2();
    hist[1]->Sumw2();
}

// Add histogram of given sample to main histogram
void FillSampleHist(TH1D **acc, TString nt, bool flag_norm=true)
{
    // Read sample
    TFile *fin = new TFile( ntdir+nt+".root" );
    TTree *T = (TTree*)fin->Get("T");
    float Evt_genweight = 0;
    float leadIDMVA = 0;
    float subleadIDMVA = 0;
    T->SetBranchAddress("EvtInfo.genweight", &Evt_genweight);
    T->SetBranchAddress("DiPhoInfo.leadIDMVA", &leadIDMVA);
    T->SetBranchAddress("DiPhoInfo.subleadIDMVA", &subleadIDMVA);

    // Create new histogram
    int nbin = acc[0]->GetNbinsX();
    TH1D *hist[2];
    CreateHist(hist, nbin);

    // Start event loop
    for (int evt=0; evt<T->GetEntries(); ++evt) {
        T->GetEntry(evt);

        float idmva_max, idmva_min;
        if (leadIDMVA > subleadIDMVA) {
            idmva_max = leadIDMVA;
            idmva_min = subleadIDMVA;
        } else {
            idmva_max = subleadIDMVA;
            idmva_min = leadIDMVA;
        }

        hist[0]->Fill(idmva_max, Evt_genweight);
        hist[1]->Fill(idmva_min, Evt_genweight);
    } // End event loop

    // Normalize histogram
    if (flag_norm) {
        float nf = GetNormFactor( normdir+nt+".root" );
        hist[0]->Scale(nf);
        hist[1]->Scale(nf);
    }

    // Add histogram
    acc[0]->Add( hist[0] );
    acc[1]->Add( hist[1] );

    fin->Close();
}

// Create histograms of all samples
void FillHist(int nbin)
{
    if (gSystem->AccessPathName(histdir))  gSystem->mkdir(histdir);
    TFile *fout = 0;

    // Create histogram
    // DiPhotonJetsBox
    fout = new TFile( histdir+"DiPhotonJetsBox.root", "recreate" );
    TH1D *hGGJets[2];  CreateHist(hGGJets, nbin);
    FillSampleHist(hGGJets, "DiPhotonJetsBox");
    fout->Write();
    fout->Close();
    // Data-driven QCD
    fout = new TFile( histdir+"Data-driven_QCD.root", "recreate" );
    TH1D *hDDqcd[2];  CreateHist(hDDqcd, nbin);
    FillSampleHist(hDDqcd, "Data-driven_QCD", false);
    fout->Write();
    fout->Close();
    // Other bkg
    fout = new TFile( histdir+"Others.root", "recreate" );
    TH1D *hOthers[2];  CreateHist(hOthers, nbin);
    FillSampleHist(hOthers, "TGJets");
    FillSampleHist(hOthers, "tW_antitop");
    FillSampleHist(hOthers, "tW_top");
    FillSampleHist(hOthers, "TTGG");
    FillSampleHist(hOthers, "TTGJets");
    FillSampleHist(hOthers, "TTJets");
    FillSampleHist(hOthers, "TTWJetsToLNu");
    FillSampleHist(hOthers, "TTZToLLNuNu");
    FillSampleHist(hOthers, "DYJetsToLL");
    FillSampleHist(hOthers, "WGToLNuG");
    FillSampleHist(hOthers, "ZGToLLG");
    FillSampleHist(hOthers, "WW");
    FillSampleHist(hOthers, "WZ");
    FillSampleHist(hOthers, "ZZ");
    FillSampleHist(hOthers, "GluGluHToGG");
    FillSampleHist(hOthers, "VBFHToGG");
    FillSampleHist(hOthers, "VHToGG");
    FillSampleHist(hOthers, "ttHJetToGG");
    fout->Write();
    fout->Close();
    // Data
    fout = new TFile( histdir+"data.root", "recreate" );
    TH1D *hdata[2];  CreateHist(hdata, nbin);
    FillSampleHist(hdata, "data", false);
    fout->Write();
    fout->Close();
}


//////////////////////////////////////////////////
// Template fit to decide scale factors         //
//////////////////////////////////////////////////

// Histogram pointers
// MC (0:GGJets, 1:Data-driven_QCD, 2:Others)
TH1D *hmax_mc[3];  TH1D *hmin_mc[3];
// Data
TH1D *hmax_data, *hmin_data;

// Function calculating the estimator to be minimized
void fcn(int &npar, double *gin, double &f, double *par, int iflag)
{
    double SF0 = par[0], SF1 = par[1];
    int nbin = hmax_data->GetNbinsX();

    f = 0;  // Reset estimator
    // Start bin loop
    for (int i=1; i<=nbin; ++i) {
        double diff_hmax = SF0*hmax_mc[0]->GetBinContent(i) + SF1*hmax_mc[1]->GetBinContent(i)
            + hmax_mc[2]->GetBinContent(i) - hmax_data->GetBinContent(i);
        double diff_hmin = SF0*hmin_mc[0]->GetBinContent(i) + SF1*hmin_mc[1]->GetBinContent(i)
            + hmin_mc[2]->GetBinContent(i) - hmin_data->GetBinContent(i);
        f += pow(diff_hmax/hmax_data->GetBinError(i),2) + pow(diff_hmin/hmin_data->GetBinError(i),2);
    } // End bin loop
}

void TemplateFit()
{
    // Read files
    TFile *fin_mc[3];
    fin_mc[0] = new TFile( histdir+"DiPhotonJetsBox.root" );
    fin_mc[1] = new TFile( histdir+"Data-driven_QCD.root" );
    fin_mc[2] = new TFile( histdir+"Others.root" );
    TFile *fin_data = new TFile( histdir+"data.root" );
    // Get histograms
    for (int i=0; i<3; ++i) {
        hmax_mc[i] = (TH1D*)fin_mc[i]->Get("hidmva_max");
        hmin_mc[i] = (TH1D*)fin_mc[i]->Get("hidmva_min");
    }
    hmax_data = (TH1D*)fin_data->Get("hidmva_max");
    hmin_data = (TH1D*)fin_data->Get("hidmva_min");

    // Minuit for optimization
    TMinuit *gMinuit = new TMinuit(2);
    gMinuit->SetFCN(fcn);
    // param no. | param name | init val | init err | lower limit | upper limit
    gMinuit->DefineParameter(0, "SF_GGJet", 1, 0.5, 0., 100.);
    gMinuit->DefineParameter(1, "SF_DDqcd", 1, 0.5, 0., 100.);
    // Perform minimization
    gMinuit->Command("MIGRAD");
    gMinuit->Command("MIGRAD");

    // Print result
    double SF0, SF1, err0, err1;
    gMinuit->GetParameter(0, SF0, err0);
    gMinuit->GetParameter(1, SF1, err1);
    printf("[INFO] Fitted scale factors:\n");
    printf("---%10s = %.3f +- %.3f\n", "GGJets", SF0, err0);
    printf("---%10s = %.3f +- %.3f\n", "DDqcd", SF1, err1);
    /* result:
     * GGJets = 1.190 +- 0.009
     * DDqcd = 0.822 +- 0.002
     */

    for (int i=0; i<3; ++i)  fin_mc[i]->Close();
    fin_data->Close();
}


////////////////////////////////////////////////////
// Make data-driven QCD tree have standard format //
////////////////////////////////////////////////////

void MakeWeight()
{
    // Read file
    TFile *fin = new TFile( ntdir+"Data-driven_QCD.root", "update" );
    TTree *Tin = (TTree*)fin->Get("T");
    float Evt_genweight = 0;
    Tin->SetBranchAddress("EvtInfo.genweight", &Evt_genweight);

    TTree *Tout = Tin->CloneTree(0);
    // Pileup SF
    float SF_pileup = 1;
    // Electron SF
    float SF_Elec = 1;
    float SF_Elec_RecoUp = -999;
    float SF_Elec_RecoDown = -999;
    float SF_Elec_IdUp = -999;
    float SF_Elec_IdDown = -999;
    // Moun SF
    float SF_Muon = 1;
    float SF_Muon_IdUp = -999;
    float SF_Muon_IdDown = -999;
    float SF_Muon_IsoUp = -999;
    float SF_Muon_IsoDown = -999;
    // b-tagging SF
    float SF_btag = 1;
    float SF_btag_jesUp = -999;
    float SF_btag_jesDown = -999;
    float SF_btag_lfUp = -999;
    float SF_btag_lfDown = -999;
    float SF_btag_hfUp = -999;
    float SF_btag_hfDown = -999;
    float SF_btag_lfstats1Up = -999;
    float SF_btag_lfstats1Down = -999;
    float SF_btag_lfstats2Up = -999;
    float SF_btag_lfstats2Down = -999;
    float SF_btag_hfstats1Up = -999;
    float SF_btag_hfstats1Down = -999;
    float SF_btag_hfstats2Up = -999;
    float SF_btag_hfstats2Down = -999;
    float SF_btag_cferr1Up = -999;
    float SF_btag_cferr1Down = -999;
    float SF_btag_cferr2Up = -999;
    float SF_btag_cferr2Down = -999;
    // Event weight
    float evtwei_central = 1;
    float evtwei_DiPhoLooseMvaSFUp = -999;
    float evtwei_DiPhoLooseMvaSFDown = -999;
    float evtwei_DiPhoPreselSFUp = -999;
    float evtwei_DiPhoPreselSFDown = -999;
    float evtwei_DiPhoelectronVetoSFUp = -999;
    float evtwei_DiPhoelectronVetoSFDown = -999;
    float evtwei_DiPhoTriggerWeightUp = -999;
    float evtwei_DiPhoTriggerWeightDown = -999;
    float evtwei_ElecRecoUp = -999;
    float evtwei_ElecRecoDown = -999;
    float evtwei_ElecIdUp = -999;
    float evtwei_ElecIdDown = -999;
    float evtwei_MuonIdUp = -999;
    float evtwei_MuonIdDown = -999;
    float evtwei_MuonIsoUp = -999;
    float evtwei_MuonIsoDown = -999;
    float evtwei_btagjesUp = -999;
    float evtwei_btagjesDown = -999;
    float evtwei_btaglfUp = -999;
    float evtwei_btaglfDown = -999;
    float evtwei_btaghfUp = -999;
    float evtwei_btaghfDown = -999;
    float evtwei_btaglfstats1Up = -999;
    float evtwei_btaglfstats1Down = -999;
    float evtwei_btaglfstats2Up = -999;
    float evtwei_btaglfstats2Down = -999;
    float evtwei_btaghfstats1Up = -999;
    float evtwei_btaghfstats1Down = -999;
    float evtwei_btaghfstats2Up = -999;
    float evtwei_btaghfstats2Down = -999;
    float evtwei_btagcferr1Up = -999;
    float evtwei_btagcferr1Down = -999;
    float evtwei_btagcferr2Up = -999;
    float evtwei_btagcferr2Down = -999;
    // Set branches
    // Pileup SF
    Tout->Branch("SF_pileup", &SF_pileup);
    // Electron SF
    Tout->Branch("SF_Elec", &SF_Elec);
    Tout->Branch("SF_Elec_RecoUp", &SF_Elec_RecoUp);
    Tout->Branch("SF_Elec_RecoDown", &SF_Elec_RecoDown);
    Tout->Branch("SF_Elec_IdUp", &SF_Elec_IdUp);
    Tout->Branch("SF_Elec_IdDown", &SF_Elec_IdDown);
    // Moun SF
    Tout->Branch("SF_Muon", &SF_Muon);
    Tout->Branch("SF_Muon_IdUp", &SF_Muon_IdUp);
    Tout->Branch("SF_Muon_IdDown", &SF_Muon_IdDown);
    Tout->Branch("SF_Muon_IsoUp", &SF_Muon_IsoUp);
    Tout->Branch("SF_Muon_IsoDown", &SF_Muon_IsoDown);
    // b-tagging SF
    Tout->Branch("SF_btag", &SF_btag);
    Tout->Branch("SF_btag_jesUp", &SF_btag_jesUp);
    Tout->Branch("SF_btag_jesDown", &SF_btag_jesDown);
    Tout->Branch("SF_btag_lfUp", &SF_btag_lfUp);
    Tout->Branch("SF_btag_lfDown", &SF_btag_lfDown);
    Tout->Branch("SF_btag_hfUp", &SF_btag_hfUp);
    Tout->Branch("SF_btag_hfDown", &SF_btag_hfDown);
    Tout->Branch("SF_btag_lfstats1Up", &SF_btag_lfstats1Up);
    Tout->Branch("SF_btag_lfstats1Down", &SF_btag_lfstats1Down);
    Tout->Branch("SF_btag_lfstats2Up", &SF_btag_lfstats2Up);
    Tout->Branch("SF_btag_lfstats2Down", &SF_btag_lfstats2Down);
    Tout->Branch("SF_btag_hfstats1Up", &SF_btag_hfstats1Up);
    Tout->Branch("SF_btag_hfstats1Down", &SF_btag_hfstats1Down);
    Tout->Branch("SF_btag_hfstats2Up", &SF_btag_hfstats2Up);
    Tout->Branch("SF_btag_hfstats2Down", &SF_btag_hfstats2Down);
    Tout->Branch("SF_btag_cferr1Up", &SF_btag_cferr1Up);
    Tout->Branch("SF_btag_cferr1Down", &SF_btag_cferr1Down);
    Tout->Branch("SF_btag_cferr2Up", &SF_btag_cferr2Up);
    Tout->Branch("SF_btag_cferr2Down", &SF_btag_cferr2Down);
    // Event weight
    Tout->Branch("evtwei_central", &evtwei_central);
    Tout->Branch("evtwei_DiPhoLooseMvaSFUp", &evtwei_DiPhoLooseMvaSFUp);
    Tout->Branch("evtwei_DiPhoLooseMvaSFDown", &evtwei_DiPhoLooseMvaSFDown);
    Tout->Branch("evtwei_DiPhoPreselSFUp", &evtwei_DiPhoPreselSFUp);
    Tout->Branch("evtwei_DiPhoPreselSFDown", &evtwei_DiPhoPreselSFDown);
    Tout->Branch("evtwei_DiPhoelectronVetoSFUp", &evtwei_DiPhoelectronVetoSFUp);
    Tout->Branch("evtwei_DiPhoelectronVetoSFDown", &evtwei_DiPhoelectronVetoSFDown);
    Tout->Branch("evtwei_DiPhoTriggerWeightUp", &evtwei_DiPhoTriggerWeightUp);
    Tout->Branch("evtwei_DiPhoTriggerWeightDown", &evtwei_DiPhoTriggerWeightDown);
    Tout->Branch("evtwei_ElecRecoUp", &evtwei_ElecRecoUp);
    Tout->Branch("evtwei_ElecRecoDown", &evtwei_ElecRecoDown);
    Tout->Branch("evtwei_ElecIdUp", &evtwei_ElecIdUp);
    Tout->Branch("evtwei_ElecIdDown", &evtwei_ElecIdDown);
    Tout->Branch("evtwei_MuonIdUp", &evtwei_MuonIdUp);
    Tout->Branch("evtwei_MuonIdDown", &evtwei_MuonIdDown);
    Tout->Branch("evtwei_MuonIsoUp", &evtwei_MuonIsoUp);
    Tout->Branch("evtwei_MuonIsoDown", &evtwei_MuonIsoDown);
    Tout->Branch("evtwei_btagjesUp", &evtwei_btagjesUp);
    Tout->Branch("evtwei_btagjesDown", &evtwei_btagjesDown);
    Tout->Branch("evtwei_btaglfUp", &evtwei_btaglfUp);
    Tout->Branch("evtwei_btaglfDown", &evtwei_btaglfDown);
    Tout->Branch("evtwei_btaghfUp", &evtwei_btaghfUp);
    Tout->Branch("evtwei_btaghfDown", &evtwei_btaghfDown);
    Tout->Branch("evtwei_btaglfstats1Up", &evtwei_btaglfstats1Up);
    Tout->Branch("evtwei_btaglfstats1Down", &evtwei_btaglfstats1Down);
    Tout->Branch("evtwei_btaglfstats2Up", &evtwei_btaglfstats2Up);
    Tout->Branch("evtwei_btaglfstats2Down", &evtwei_btaglfstats2Down);
    Tout->Branch("evtwei_btaghfstats1Up", &evtwei_btaghfstats1Up);
    Tout->Branch("evtwei_btaghfstats1Down", &evtwei_btaghfstats1Down);
    Tout->Branch("evtwei_btaghfstats2Up", &evtwei_btaghfstats2Up);
    Tout->Branch("evtwei_btaghfstats2Down", &evtwei_btaghfstats2Down);
    Tout->Branch("evtwei_btagcferr1Up", &evtwei_btagcferr1Up);
    Tout->Branch("evtwei_btagcferr1Down", &evtwei_btagcferr1Down);
    Tout->Branch("evtwei_btagcferr2Up", &evtwei_btagcferr2Up);
    Tout->Branch("evtwei_btagcferr2Down", &evtwei_btagcferr2Down);

    // Start event loop
    for (int evt=0; evt<Tin->GetEntries(); ++evt) {
        Tin->GetEntry(evt);
        evtwei_central = Evt_genweight;
        Tout->Fill();
    } // End event loop

    fin->WriteTObject(Tout, "", "Overwrite");
    fin->Close();
}


//////////////////////////////////////////////////
// Multiply SF to event tree                    //
//////////////////////////////////////////////////

void ApplySF_sample(TString nt, float SF)
{
    TFile *fin = new TFile( ntdir+nt+".root", "update" );
    TTree *Tin = (TTree*)fin->Get("T");
    float evtwei_central = 0;
    Tin->SetBranchAddress("evtwei_central", &evtwei_central);

    TTree *Tout = Tin->CloneTree(0);

    for (int evt=0; evt<Tin->GetEntries(); ++evt)
    {
        Tin->GetEntry(evt);
        evtwei_central *= SF;
        Tout->Fill();
    }

    fin->WriteTObject(Tout, "", "Overwrite");
    fin->Close();
}

void ApplySF()
{
    ApplySF_sample("DiPhotonJetsBox", 1.19);
    ApplySF_sample("Data-driven_QCD", 0.822);
}


//////////////////////////////////////////////////
// Make histograms                              //
//////////////////////////////////////////////////

void ScaleHistFile(TString nt, float SF)
{
    // Read file
    TFile *fin = new TFile(ntdir+"hist/"+nt+".root", "update");
    vector<TH1D*> hist;

    // Read histograms
    TKey *key = 0;
    TIter nextkey( fin->GetListOfKeys() );
    while (( key=(TKey*)nextkey() )) {
        hist.push_back( (TH1D*)key->ReadObj() );
    }

    // Scale & save results
    for (int i=0; i<hist.size(); ++i) {
        hist[i]->Scale(SF);
        fin->WriteTObject(hist[i], "", "Overwrite");
    }

    fin->Close();
}

void MakeDataDrivenHist()
{
    // Make DiPhotonJetsBox histograms (by scaling the original histograms)
    ScaleHistFile( "DiPhotonJetsBox", 1.19 );

    // Make data-driven QCD histograms (run hist-filling code on data-driven QCD tree)
    const char *cmd_hist = Form("%sbin/FillHist_had %s/%s.root %s/%s.root", dir_tqHGG.Data(),
            (dir_res+"Presel_had_phID_btag-L").Data(), "Data-driven_QCD",
            (dir_res+"Presel_had_phID_btag-L/hist").Data(), "Data-driven_QCD");
    gSystem->Exec( cmd_hist );
}
