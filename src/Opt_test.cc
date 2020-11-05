/*
 * Make test plots for SR optimization.
 *
 * Usage:
 *   ./Opt_test <indir> <samples_sig> <samples_resbkg> <data> <outdir>
 *
 * Data is used as non-res bkg.
 */

#include "include/utility.h"
#include "TSystem.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    TString indir = argv[1];
    vector<TString> fsig_;
    vector<TString> fresbkg_;
    vector<TString> fnrbkg_;
    ParseCStringList(argv[2], fsig_); int Nsig = fsig_.size();
    ParseCStringList(argv[3], fresbkg_); int Nresbkg = fresbkg_.size();
    ParseCStringList(argv[4], fnrbkg_); int Nnrbkg = fnrbkg_.size();
    TString outdir = argv[5];
    // Read input samples
    vector<TFile*> fsig(Nsig);
    vector<TFile*> fresbkg(Nresbkg);
    vector<TFile*> fnrbkg(Nnrbkg);
    vector<TTree*> Tsig(Nsig);
    vector<TTree*> Tresbkg(Nresbkg);
    vector<TTree*> Tnrbkg(Nnrbkg);
    // Tree variables
    float genweight = 0;
    float Nscale = 0;
    float norm = 0;
    float SF_pileup = 0;
    float SF_btag = 0;
    float SF_photon = 0;
    float SF_ElecReco = 0;
    float SF_ElecID = 0;
    float SF_MuonID = 0;
    float SF_MuonIso = 0;
    float Mgg = 0;
    float BDT_score = 0;
    // Set trees
    cout << "[INFO] Input dir: " << indir << endl;
    cout << "[INFO] Signal samples: ";
    for (int i=0; i<Nsig; ++i) {
        cout << fsig_[i] << "  ";
        fsig[i] = new TFile(indir+"/"+fsig_[i]+".root");
        Tsig[i] = (TTree*)fsig[i]->Get("T");
    }
    cout << "\n[INFO] Resonant bkg samples: ";
    for (int i=0; i<Nresbkg; ++i) {
        cout << fresbkg_[i] << "  ";
        fresbkg[i] = new TFile(indir+"/"+fresbkg_[i]+".root");
        Tresbkg[i] = (TTree*)fresbkg[i]->Get("T");
    }
    cout << "\n[INFO] Non-resonant bkg samples: ";
    for (int i=0; i<Nnrbkg; ++i) {
        cout << fnrbkg_[i] << "  ";
        fnrbkg[i] = new TFile(indir+"/"+fnrbkg_[i]+".root");
        Tnrbkg[i] = (TTree*)fnrbkg[i]->Get("T");
    }
    cout << endl;

    const int Ntype = 3;
    vector<TTree*> *trees[Ntype] = {&Tsig, &Tresbkg, &Tnrbkg};

    // BDT score cut values
    float BDT_cut = -0.6;
    float increment = 0.2;

    // Output plot dir
    TString plotdir = outdir + "/plots_test";
    gSystem->mkdir(plotdir.Data());

    TCanvas *c = new TCanvas();

    // Start BDT cut loop
    while (BDT_cut < 0.5) {
        cout << "Processing BDT score cut: " << BDT_cut << endl;
        // Output file for this cut value
        TFile *fout = new TFile(outdir+Form("/hmgg_cut-%f.root",BDT_cut), "recreate");

        TH1D *hmgg[Ntype];
        // Start type loop
        for (int itype=0; itype<Ntype; ++itype) {
            hmgg[itype] = new TH1D(Form("hmgg%d",itype), ";diphoton mass;", 16, 100, 180);
            hmgg[itype]->Sumw2();

            // Start sample loop
            vector<TTree*> &samples = *(trees[itype]);
            for (int i=0; i<samples.size(); ++i) {
                TTree *T = samples[i];
                T->SetBranchAddress("genweight", &genweight);
                T->SetBranchAddress("Nscale", &Nscale);
                T->SetBranchAddress("norm", &norm);
                T->SetBranchAddress("SF_pileup", &SF_pileup);
                T->SetBranchAddress("SF_btag", &SF_btag);
                T->SetBranchAddress("SF_photon", &SF_photon);
                T->SetBranchAddress("SF_ElecReco", &SF_ElecReco);
                T->SetBranchAddress("SF_ElecID", &SF_ElecID);
                T->SetBranchAddress("SF_MuonID", &SF_MuonID);
                T->SetBranchAddress("SF_MuonIso", &SF_MuonIso);
                T->SetBranchAddress("Mgg", &Mgg);
                T->SetBranchAddress("BDT_score", &BDT_score);

                // Start tree loop
                for (int evt=0; evt<T->GetEntries(); ++evt) {
                    T->GetEntry(evt);
                    if (BDT_score < BDT_cut) continue;
                    if (Mgg < 100 || Mgg > 180) continue;
                    hmgg[itype]->Fill(Mgg, genweight*Nscale*norm*SF_pileup*SF_btag*SF_photon*SF_ElecReco*SF_ElecID*
                            SF_MuonID*SF_MuonIso);
                } // End tree loop
            } // End sample loop
            hmgg[itype]->Draw();
            c->SaveAs(plotdir + Form("/hmgg%d_cut-%f.png",itype,BDT_cut));
            hmgg[itype]->Write();
        } // End type loop
        fout->Close();

        BDT_cut += increment;
    } // End BDT cut loop

    // Close files
    for (int i=0; i<Nsig; ++i) fsig[i]->Close();
    for (int i=0; i<Nresbkg; ++i) fresbkg[i]->Close();
    for (int i=0; i<Nnrbkg; ++i) fnrbkg[i]->Close();

    cout << "[INFO] Results saved: " << outdir << endl;

    return 0;
}
