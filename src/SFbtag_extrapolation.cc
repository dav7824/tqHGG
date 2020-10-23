/*
 * Calculate extrapolation factor of b-tagging SF.
 * More info: https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagShapeCalibration
 *
 * Usage:
 *   ./SFbtag_extrapolation <fin> <csv> <fout>
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
    // Command line arguments
    TString fin_name = argv[1];
    TString csv_name = argv[2];
    TString fout_name = argv[3];

    // Read & set input tree
    cout << "[INFO] Openning file: " << fin_name << endl;
    TFile *fin = new TFile(fin_name);
    TTree *T = (TTree*)fin->Get("T");
    float genweight = 0;
    int jets_size = 0;
    vector<float> *Jet_Pt = 0;
    vector<float> *Jet_Eta = 0;
    vector<float> *Jet_probb = 0;
    vector<float> *Jet_probbb = 0;
    vector<int> *Jet_GenHadronFlavor = 0;
    T->SetBranchStatus("*", 0);
    T->SetBranchStatus("EvtInfo.genweight", 1);
    T->SetBranchStatus("jets_size", 1);
    T->SetBranchStatus("JetInfo.Pt", 1);
    T->SetBranchStatus("JetInfo.Eta", 1);
    T->SetBranchStatus("JetInfo.pfDeepCSVJetTags_probb", 1);
    T->SetBranchStatus("JetInfo.pfDeepCSVJetTags_probbb", 1);
    T->SetBranchStatus("JetInfo.GenHadronFlavor", 1);
    T->SetBranchAddress("EvtInfo.genweight", &genweight);
    T->SetBranchAddress("jets_size", &jets_size);
    T->SetBranchAddress("JetInfo.Pt", &Jet_Pt);
    T->SetBranchAddress("JetInfo.Eta", &Jet_Eta);
    T->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probb", &Jet_probb);
    T->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probbb", &Jet_probbb);
    T->SetBranchAddress("JetInfo.GenHadronFlavor", &Jet_GenHadronFlavor);

    // If the tree has 0 entries, save extrapolation factor as 1
    if (T->GetEntries() == 0) {
        cout << "The tree has 0 entries.  Save extrapolation factor as 1.\n";
        // Save result
        TFile *fout = new TFile(fout_name, "recreate");
        TH1D *hh = new TH1D("btag_factor", "", 1, 0, 1);
        hh->SetBinContent(1, 1);
        fout->Write();
        fout->Close();

        fin->Close();

        cout << "[INFO] Output saved: " << fout_name << endl;

        return 0;
    }

    // Read b-tagging SF csv
    cout << "[INFO] Openning csv: " << csv_name << endl;
    BTagCalibration bc("csv", csv_name.Data());
    // Reshaping
    BTagCalibrationReader bcr(BTagEntry::OP_RESHAPING);
    bcr.load(bc, BTagEntry::FLAV_B, "iterativefit");
    bcr.load(bc, BTagEntry::FLAV_C, "iterativefit");
    bcr.load(bc, BTagEntry::FLAV_UDSG, "iterativefit");

    float sumwei_before = 0;  // Sum of weight before applying b-tagging SF
    float sumwei_after = 0;  // Sum of weight after applying b-tagging SF

    // Start event loop
    for (int evt=0; evt<T->GetEntries(); ++evt)
    {
        T->GetEntry(evt);
        float SF_btag = 1;

        sumwei_before += genweight;

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

        sumwei_after += genweight * SF_btag;
    } // End event loop

    // Save result
    TFile *fout = new TFile(fout_name, "recreate");
    TH1D *hh = new TH1D("btag_factor", "", 1, 0, 1);
    hh->SetBinContent(1, sumwei_before/sumwei_after);
    fout->Write();
    fout->Close();

    fin->Close();

    cout << "[INFO] Output saved: " << fout_name << endl;

    return 0;
}
