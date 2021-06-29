#include "include/Paths.h"

vector<TString> nt_sig_had = {
    "TT_FCNC-TtoHJ_aThad_hct",
    "TT_FCNC-aTtoHJ_Thad_hct",
    "TT_FCNC-TtoHJ_aThad_hut",
    "TT_FCNC-aTtoHJ_Thad_hut",
    "ST_FCNC-TH_Thad_hct",
    "ST_FCNC-TH_Thad_hut",
};
vector<TString> nt_sig_lep = {
    "TT_FCNC-TtoHJ_aTlep_hct",
    "TT_FCNC-aTtoHJ_Tlep_hct",
    "TT_FCNC-TtoHJ_aTlep_hut",
    "TT_FCNC-aTtoHJ_Tlep_hut",
    "ST_FCNC-TH_Tlep_hct",
    "ST_FCNC-TH_Tlep_hut",
};
vector<TString> nt_bkg_had = {
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
vector<TString> nt_bkg_lep = {
    "DiPhotonJetsBox",
    "GJet_Pt-20to40_MGG-80toInf",
    "GJet_Pt-20toInf_MGG-40to80",
    "GJet_Pt-40toInf_MGG-80toInf",
    "QCD_Pt-30toInf_MGG-40to80",
    "QCD_Pt-40toInf_MGG-80toInf",
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

struct EffvsScore
{
public:
    EffvsScore(float score_, float weight_) : score(score_), weight(weight_) {}
    float score;
    float weight;
};

vector<EffvsScore> *ReadTree(TString indir, vector<TString> &nt)
{
    // Read input tree
    TChain *T = new TChain("T");
    for (int i=0; i<nt.size(); ++i)  T->Add( indir+nt.at(i)+".root" );
    // Set branches
    float evtwei_central = 0;
    float BDT_score = 0;
    T->SetBranchAddress("evtwei_central", &evtwei_central);
    T->SetBranchAddress("BDT_score", &BDT_score);
    // Calculate sum of weights
    float sumwei = 0.;
    for (int evt=0; evt<T->GetEntries(); ++evt) {
        T->GetEntry(evt);
        sumwei += evtwei_central;
    }
    // Create EffvsScore vector
    vector<EffvsScore> *evs = new vector<EffvsScore>();
    for (int evt=0; evt<T->GetEntries(); ++evt) {
        T->GetEntry(evt);
        evs->push_back( EffvsScore(BDT_score, evtwei_central/sumwei) );
    }
    delete T;
    return evs;
}

void SortEvents(vector<EffvsScore> *evs)
{
    int Nevt = evs->size();

    // Sort events according to BDT score from low to high
    for (int i=1; i<Nevt; ++i) {
        EffvsScore tmp = evs->at(i);
        int j = i-1;
        while (j >=0 && evs->at(j).score > tmp.score) {
            evs->at(j+1) = evs->at(j);
            --j;
        }
        evs->at(j+1) = tmp;
    }
}

void MakeEffGraph(TString ch, TString graph_name, float BDT_lo, float BDT_hi)
{
    TString indir = dir_res;
    vector<TString> *nt_sig, *nt_bkg;
    if (ch == "had") {
        indir += "BDT_input_had/";
        nt_sig = &nt_sig_had;
        nt_bkg = &nt_bkg_had;
    } else if (ch == "lep") {
        indir += "BDT_input_lep/";
        nt_sig = &nt_sig_lep;
        nt_bkg = &nt_bkg_lep;
    } else {
        cerr << "[ERROR] Invalid channel\n";
        exit(1);
    }
    // Get events from trees
    cout << "Reading files...\n";
    vector<EffvsScore> *evs_sig = ReadTree(indir, *nt_sig);
    vector<EffvsScore> *evs_bkg = ReadTree(indir, *nt_bkg);
    cout << "Sorting events...\n";
    SortEvents(evs_sig);
    SortEvents(evs_bkg);

    // Make graph of signal eff vs bkg eff
    float BDT_cut = BDT_hi;
    int Nscan = 300;
    float BDT_inc = (BDT_hi - BDT_lo) / Nscan;
    int idx_sig = evs_sig->size()-1, idx_bkg = evs_bkg->size()-1;
    float eff_sig = 0, eff_bkg = 0;
    // Create output file
    TFile *fout = new TFile("BDT_eff.root", "update");
    TGraph *gr = new TGraph(Nscan);
    gr->SetName(graph_name);
    gr->GetXaxis()->SetTitle("signal eff");
    gr->GetYaxis()->SetTitle("background eff");
    cout << "Making graph...\n";
    for (int i=0; i<Nscan; ++i) {
        while ( idx_sig >=0 && evs_sig->at(idx_sig).score >= BDT_cut ) {
            eff_sig += evs_sig->at(idx_sig).weight;
            --idx_sig;
        }
        while ( idx_bkg >=0 && evs_bkg->at(idx_bkg).score >= BDT_cut ) {
            eff_bkg += evs_bkg->at(idx_bkg).weight;
            --idx_bkg;
        }
        gr->SetPoint(i, eff_sig, eff_bkg);
        BDT_cut -= BDT_inc;
    }
    gr->Write();
    fout->Close();
    cout << "Complete!\n";

    delete evs_sig;
    delete evs_bkg;
}

void BDT_eff()
{
    MakeEffGraph("had", "eff_had", -0.6, 0.6);
    //MakeEffGraph("lep", "eff_lep", -0.5, 0.5);
}
