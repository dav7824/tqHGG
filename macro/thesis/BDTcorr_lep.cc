#include "include/Paths.h"

vector<TString> nt_sig = {
    "TT_FCNC-TtoHJ_aTlep_hct",
    "TT_FCNC-aTtoHJ_Tlep_hct",
    "TT_FCNC-TtoHJ_aTlep_hut",
    "TT_FCNC-aTtoHJ_Tlep_hut",
    "ST_FCNC-TH_Tlep_hct",
    "ST_FCNC-TH_Tlep_hut",
};
vector<TString> nt_bkg = {
    "DiPhotonJetsBox",
    "GJet_Pt-20to40_MGG-80toInf",
    "GJet_Pt-20toInf_MGG-40to80",
    "GJet_Pt-40toInf_MGG-80toInf",
    //"QCD_Pt-30toInf_MGG-40to80",
    //"QCD_Pt-40toInf_MGG-80toInf",
    "TGJets",
    "tW_antitop",
    "tW_top",
    "TTGG",
    "TTGJets",
    "TTJets",
    "TTWJetsToLNu",
    "TTZToLLNuNu",
    "DYJetsToLL",
    "WGToLNuG",
    "ZGToLLG",
    "WW",
    "WZ",
    "ZZ",
    "GluGluHToGG",
    "VBFHToGG",
    "VHToGG",
    "ttHJetToGG",
};

const int Nvar = 46;
TString vars[Nvar] = {
    // photons
    "pho1_ptOverMgg",
    "pho1_eta",
    "pho1_PixelSeed",
    "pho2_ptOverMgg",
    "pho2_eta",
    "pho2_PixelSeed",
    "pho_maxIDMVA",
    "pho_minIDMVA",
    // diphoton
    "dipho_ptOverMgg",
    "dipho_eta",
    "dipho_dPhi",
    "dipho_dR",
    "dipho_heliAngle",
    // lepton
    "lepton_ID",
    "lepton_pt",
    "lepton_eta",
    "lepton_ntight",
    // jets
    "jet1_pt",
    "jet1_eta",
    "jet1_btag",
    "jet2_pt",
    "jet2_eta",
    "jet2_btag",
    "jet3_pt",
    "jet3_eta",
    "jet3_btag",
    "jet_btag1",
    "jet_btag2",
    "jet_N",
    "jet_Ht",
    // MET
    "met_Pt",
    // reco TT
    "TT_tqH_ptOverM",
    "TT_tqH_eta",
    "TT_dR_tqH_H",
    "TT_dR_qH",
    "TT_dR_tt",
    "TT_tbW_pt",
    "TT_tbW_eta",
    "TT_tbW_M",
    "TT_dR_tbW_H",
    "TT_score",
    // reco ST
    "ST_tbW_pt",
    "ST_tbW_eta",
    "ST_tbW_M",
    "ST_dR_tbW_H",
    "ST_score",
};


void MakeCorr(TTree *T, TString fout_name, TString hist_name, TString hist_title)
{
    // Tree variables
    float br[Nvar];
    float evtwei_central = 0;

    // Set tree branches
    T->SetBranchAddress("evtwei_central", &evtwei_central);
    for (int i=0; i<Nvar; ++i)
        T->SetBranchAddress( vars[i], &br[i] );

    // Compute average of each variable
    cout << "---Computing mean...\n";
    float mean[Nvar] = {0.};
    float sumweight = 0;
    for (int evt=0; evt<T->GetEntries(); ++evt) {
        T->GetEntry(evt);

        sumweight += evtwei_central;
        for (int i=0; i<Nvar; ++i)
            mean[i] += br[i] * evtwei_central;
    }
    for (int i=0; i<Nvar; ++i)  mean[i] /= sumweight;

    // Compute covariance between variables
    cout << "---Computing covariance...\n";
    float cov[Nvar][Nvar] = {0.};
    for (int evt=0; evt<T->GetEntries(); ++evt) {
        T->GetEntry(evt);

        for (int i=0; i<Nvar; ++i)
            for (int j=i; j<Nvar; ++j)
                cov[i][j] += (br[i]-mean[i]) * (br[j]-mean[j]) * evtwei_central;
    }
    for (int i=0; i<Nvar; ++i)
        for (int j=i; j<Nvar; ++j)
            cov[i][j] /= sumweight;

    // Std deviation of each variable
    float sigma[Nvar];
    for (int i=0; i<Nvar; ++i)  sigma[i] = sqrt(cov[i][i]);

    // Compute correlation matrix
    float corr[Nvar][Nvar] = {0.};
    for (int i=0; i<Nvar; ++i)
        for (int j=i; j<Nvar; ++j) {
            corr[i][j] = cov[i][j] / sigma[i] / sigma[j];

            if (i != j)  corr[j][i] = corr[i][j];
        }

    // Create output 2-D histogram for saving correlation matrix
    cout << "---Saving results...\n";
    TFile *fout = new TFile( fout_name, "update" );
    TH2F *hcorr = new TH2F(hist_name, hist_title, Nvar, 0, (double)Nvar, Nvar, 0, (double)Nvar);
    // Set bin labels
    for (int i=0; i<Nvar; ++i) {
        hcorr->GetXaxis()->SetBinLabel(i+1, vars[i]);
        hcorr->GetYaxis()->SetBinLabel(Nvar-i, vars[i]);  // y-axis is reversed
    }
    // Set bin contents
    for (int i=0; i<Nvar; ++i)
        for (int j=0; j<Nvar; ++j) {
            hcorr->SetBinContent( i+1, Nvar-j, corr[i][j]*100. );  // y-axis is reversed
        }
    hcorr->SetStats(false);
    hcorr->LabelsOption("v", "x");
    // Save result
    hcorr->Write();
    fout->Close();
}


void BDTcorr_lep()
{
    // Read input trees
    TString indir = dir_res + "BDT_input_lep/";
    TChain *Tsig = new TChain("T");
    TChain *Tbkg = new TChain("T");
    for (int i=0; i<nt_sig.size(); ++i)  Tsig->Add( indir+nt_sig[i]+".root" );
    for (int i=0; i<nt_bkg.size(); ++i)  Tbkg->Add( indir+nt_bkg[i]+".root" );

    // Process
    cout << "Processing signal\n";
    MakeCorr( Tsig, "BDTcorr_lep.root", "hcorr_sig", "Leptonic signal" );
    cout << "Processing background\n";
    MakeCorr( Tbkg, "BDTcorr_lep.root", "hcorr_bkg", "Leptonic bkg" );
}
