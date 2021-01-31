/*
 * Calculate b-tagging SF extrapolation.
 * Do it before apply b-tag score cut.
 *
 * Usage:
 *   ./SFcalc_btag_factor <fin> <fout> <csv>
 */

#include "include/BTagCalibrationStandalone.h"
#include "include/BTagCalibrationStandalone.cpp"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fin_name(argv[1]);
    TString fout_name(argv[2]);
    TString fin_csv(argv[3]);

    // Read SF CSV
    BTagCalibration bc("csv", fin_csv.Data());
    // Reshaping
    BTagCalibrationReader bcr(BTagEntry::OP_RESHAPING);
    bcr.load(bc, BTagEntry::FLAV_B, "iterativefit");
    bcr.load(bc, BTagEntry::FLAV_C, "iterativefit");
    bcr.load(bc, BTagEntry::FLAV_UDSG, "iterativefit");

    // Read input tree
    cout << "[INFO] Reading file: " << fin_name << endl;
    TFile *fin = new TFile(fin_name);
    TTree *T = (TTree*)fin->Get("T");
    // Set branches
    float Evt_genweight = 0;
    float DiPho_centralWeight = 0;
    int jets_size = 0;
    vector<float> *Jet_Pt = 0;
    vector<float> *Jet_Eta = 0;
    vector<float> *Jet_probb = 0;
    vector<float> *Jet_probbb = 0;
    vector<int> *Jet_GenHadronFlavor = 0;
    float SF_pileup = 0;
    float SF_Elec = 0;
    float SF_Muon = 0;
    T->SetBranchAddress("EvtInfo.genweight", &Evt_genweight);
    T->SetBranchAddress("DiPhoInfo.centralWeight", &DiPho_centralWeight);
    T->SetBranchAddress("jets_size", &jets_size);
    T->SetBranchAddress("JetInfo.Pt", &Jet_Pt);
    T->SetBranchAddress("JetInfo.Eta", &Jet_Eta);
    T->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probb", &Jet_probb);
    T->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probbb", &Jet_probbb);
    T->SetBranchAddress("JetInfo.GenHadronFlavor", &Jet_GenHadronFlavor);
    T->SetBranchAddress("SF_pileup", &SF_pileup);
    T->SetBranchAddress("SF_Elec", &SF_Elec);
    T->SetBranchAddress("SF_Muon", &SF_Muon);

    float sumwei_before = 0;  // Sum of weight before applying b-tagging SF
    float sumwei_after = 0;  // Sum of weight after applying b-tagging SF

    // Start event loop
    for (int evt=0; evt<T->GetEntries(); ++evt)
    {
        T->GetEntry(evt);
        float SF_btag = 1;
        float wei_before = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon;

        sumwei_before += wei_before;

        // Start jet loop
        for (int i=0; i<jets_size; ++i) {
            BTagEntry::JetFlavor flavor = BTagEntry::FLAV_UDSG;
            switch (Jet_GenHadronFlavor->at(i)) {
                case 0:
                    flavor = BTagEntry::FLAV_UDSG;
                    break;
                case 4:
                    flavor = BTagEntry::FLAV_C;
                    break;
                case 5:
                    flavor = BTagEntry::FLAV_B;
                    break;
                default:
                    cout << "[ERROR] Invalid hadron flavor!\n";
                    exit(1);
            }
            float sf = bcr.eval_auto_bounds("central", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
            SF_btag *= sf;
        } // End of jet loop

        sumwei_after += wei_before * SF_btag;
    } // End event loop

    fin->Close();

    // b-tagging SF factor
    float bfactor = sumwei_before / sumwei_after;
    cout << "[INFO] b-tag factor = " << bfactor << endl;

    // Save result
    TFile *fout = new TFile(fout_name, "recreate");
    TH1D *hh = new TH1D("btag_factor", "", 1, 0, 1);
    hh->SetBinContent(1, bfactor);
    hh->Write();
    fout->Close();

    return 0;
}
