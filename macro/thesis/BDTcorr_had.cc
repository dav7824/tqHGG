#include "include/Paths.h"

vector<TString> nt_sig = {
    "TT_FCNC-TtoHJ_aThad_hct",
    "TT_FCNC-aTtoHJ_Thad_hct",
    "TT_FCNC-TtoHJ_aThad_hut",
    "TT_FCNC-aTtoHJ_Thad_hut",
    "ST_FCNC-TH_Thad_hct",
    "ST_FCNC-TH_Thad_hut",
};
vector<TString> nt_bkg = {
    "DiPhotonJetsBox_scaled",
    "Data-driven_QCD",
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

const int Nvar = 45;
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
    "jet4_pt",
    "jet4_eta",
    "jet4_btag",
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
    // photons
    T->SetBranchAddress("pho1_ptOverMgg", &br[0]);
    T->SetBranchAddress("pho1_eta", &br[1]);
    T->SetBranchAddress("pho1_PixelSeed", &br[2]);
    T->SetBranchAddress("pho2_ptOverMgg", &br[3]);
    T->SetBranchAddress("pho2_eta", &br[4]);
    T->SetBranchAddress("pho2_PixelSeed", &br[5]);
    T->SetBranchAddress("pho_maxIDMVA", &br[6]);
    T->SetBranchAddress("pho_minIDMVA", &br[7]);
    // diphoton
    T->SetBranchAddress("dipho_ptOverMgg", &br[8]);
    T->SetBranchAddress("dipho_eta", &br[9]);
    T->SetBranchAddress("dipho_dPhi", &br[10]);
    T->SetBranchAddress("dipho_dR", &br[11]);
    T->SetBranchAddress("dipho_heliAngle", &br[12]);
    // jets
    T->SetBranchAddress("jet1_pt", &br[13]);
    T->SetBranchAddress("jet1_eta", &br[14]);
    T->SetBranchAddress("jet1_btag", &br[15]);
    T->SetBranchAddress("jet2_pt", &br[16]);
    T->SetBranchAddress("jet2_eta", &br[17]);
    T->SetBranchAddress("jet2_btag", &br[18]);
    T->SetBranchAddress("jet3_pt", &br[19]);
    T->SetBranchAddress("jet3_eta", &br[20]);
    T->SetBranchAddress("jet3_btag", &br[21]);
    T->SetBranchAddress("jet4_pt", &br[22]);
    T->SetBranchAddress("jet4_eta", &br[23]);
    T->SetBranchAddress("jet4_btag", &br[24]);
    T->SetBranchAddress("jet_btag1", &br[25]);
    T->SetBranchAddress("jet_btag2", &br[26]);
    T->SetBranchAddress("jet_N", &br[27]);
    T->SetBranchAddress("jet_Ht", &br[28]);
    // MET
    T->SetBranchAddress("met_Pt", &br[29]);
    // reco TT
    T->SetBranchAddress("TT_tqH_ptOverM", &br[30]);
    T->SetBranchAddress("TT_tqH_eta", &br[31]);
    T->SetBranchAddress("TT_dR_tqH_H", &br[32]);
    T->SetBranchAddress("TT_dR_qH", &br[33]);
    T->SetBranchAddress("TT_dR_tt", &br[34]);
    T->SetBranchAddress("TT_tbW_pt", &br[35]);
    T->SetBranchAddress("TT_tbW_eta", &br[36]);
    T->SetBranchAddress("TT_tbW_M", &br[37]);
    T->SetBranchAddress("TT_dR_tbW_H", &br[38]);
    T->SetBranchAddress("TT_score", &br[39]);
    // reco ST
    T->SetBranchAddress("ST_tbW_pt", &br[40]);
    T->SetBranchAddress("ST_tbW_eta", &br[41]);
    T->SetBranchAddress("ST_tbW_M", &br[42]);
    T->SetBranchAddress("ST_dR_tbW_H", &br[43]);
    T->SetBranchAddress("ST_score", &br[44]);

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


void BDTcorr_had()
{
    // Read input trees
    TString indir = dir_res + "BDT_input_had/";
    TChain *Tsig = new TChain("T");
    TChain *Tbkg = new TChain("T");
    for (int i=0; i<nt_sig.size(); ++i)  Tsig->Add( indir+nt_sig[i]+".root" );
    for (int i=0; i<nt_bkg.size(); ++i)  Tbkg->Add( indir+nt_bkg[i]+".root" );

    // Process
    cout << "Processing signal\n";
    MakeCorr( Tsig, "BDTcorr_had.root", "hcorr_sig", "Hadronic signal" );
    cout << "Processing background\n";
    MakeCorr( Tbkg, "BDTcorr_had.root", "hcorr_bkg", "Hadronic bkg" );
}
