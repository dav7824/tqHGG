/*
 * Calculate and add b-tagging SF to the event tree.
 *
 * Usage:
 *   ./SFcalc_btag <fin> <file_factor> <csv> <flag_syst=syst|nosyst>
 *
 * <fin>:         MC sample to add b-tagging SF
 * <file_factor>: File with b-tag extrapolation factor
 * <csv>:         b-tagging SF CSV file
 * <flag_syst>:   syst=add b-tag systematic SF;  nosyst=do not add
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
#include <string>
using namespace std;

vector<string> systtype = {
    "up_jes",
    "down_jes",
    "up_lf",
    "down_lf",
    "up_hf",
    "down_hf",
    "up_lfstats1",
    "down_lfstats1",
    "up_lfstats2",
    "down_lfstats2",
    "up_hfstats1",
    "down_hfstats1",
    "up_hfstats2",
    "down_hfstats2",
    "up_cferr1",
    "down_cferr1",
    "up_cferr2",
    "down_cferr2",
};

int main(int argc, char **argv)
{
    // Command line arguments
    TString fin_name = argv[1];
    TString ffac_name = argv[2];
    TString fin_csv = argv[3];
    TString makeSyst_ = argv[4];
    bool makeSyst;
    if (makeSyst_ == "syst") makeSyst = true;
    else makeSyst = false;

    // Read SF CSV
    BTagCalibration bc("csv", fin_csv.Data());
    // Reshaping
    BTagCalibrationReader *bcr = 0;
    if (makeSyst)  bcr = new BTagCalibrationReader(BTagEntry::OP_RESHAPING, "central", systtype);
    else  bcr = new BTagCalibrationReader(BTagEntry::OP_RESHAPING);
    bcr->load(bc, BTagEntry::FLAV_B, "iterativefit");
    bcr->load(bc, BTagEntry::FLAV_C, "iterativefit");
    bcr->load(bc, BTagEntry::FLAV_UDSG, "iterativefit");

    // Get b-tagging extrapolation factor
    TFile *ffac = new TFile(ffac_name);
    float bfactor = ((TH1D*)ffac->Get("btag_factor"))->GetBinContent(1);
    ffac->Close();

    // Read input file
    TFile *fin = new TFile(fin_name, "update");
    TTree *Tin = (TTree*)fin->Get("T");
    int jets_size = 0;
    vector<float> *Jet_Pt = 0;
    vector<float> *Jet_Eta = 0;
    vector<float> *Jet_probb = 0;
    vector<float> *Jet_probbb = 0;
    vector<int> *Jet_GenHadronFlavor = 0;
    Tin->SetBranchAddress("jets_size", &jets_size);
    Tin->SetBranchAddress("JetInfo.Pt", &Jet_Pt);
    Tin->SetBranchAddress("JetInfo.Eta", &Jet_Eta);
    Tin->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probb", &Jet_probb);
    Tin->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probbb", &Jet_probbb);
    Tin->SetBranchAddress("JetInfo.GenHadronFlavor", &Jet_GenHadronFlavor);

    // Create new tree
    TTree *Tout = Tin->CloneTree(0);
    float SF_btag = -999;
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

    // Start event loop
    for (int evt=0; evt<Tin->GetEntries(); ++evt)
    {
        Tin->GetEntry(evt);

        SF_btag = 1;
        if (makeSyst) {
            SF_btag_jesUp = 1;
            SF_btag_jesDown = 1;
            SF_btag_lfUp = 1;
            SF_btag_lfDown = 1;
            SF_btag_hfUp = 1;
            SF_btag_hfDown = 1;
            SF_btag_lfstats1Up = 1;
            SF_btag_lfstats1Down = 1;
            SF_btag_lfstats2Up = 1;
            SF_btag_lfstats2Down = 1;
            SF_btag_hfstats1Up = 1;
            SF_btag_hfstats1Down = 1;
            SF_btag_hfstats2Up = 1;
            SF_btag_hfstats2Down = 1;
            SF_btag_cferr1Up = 1;
            SF_btag_cferr1Down = 1;
            SF_btag_cferr2Up = 1;
            SF_btag_cferr2Down = 1;
        }

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
            SF_btag *= bcr->eval_auto_bounds("central", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
            if (makeSyst) {
                // These systematics only affect udsg & b flavors
                // Evaluate c flavor SF at central, which is always 1 (see SF CSV file)
                if (flavor == BTagEntry::FLAV_UDSG || flavor == BTagEntry::FLAV_B) {
                    SF_btag_jesUp *= bcr->eval_auto_bounds("up_jes", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
                    SF_btag_jesDown *= bcr->eval_auto_bounds("down_jes", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
                    SF_btag_lfUp *= bcr->eval_auto_bounds("up_lf", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
                    SF_btag_lfDown *= bcr->eval_auto_bounds("down_lf", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
                    SF_btag_hfUp *= bcr->eval_auto_bounds("up_hf", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
                    SF_btag_hfDown *= bcr->eval_auto_bounds("down_hf", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
                    SF_btag_lfstats1Up *= bcr->eval_auto_bounds("up_lfstats1", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
                    SF_btag_lfstats1Down *= bcr->eval_auto_bounds("down_lfstats1", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
                    SF_btag_lfstats2Up *= bcr->eval_auto_bounds("up_lfstats2", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
                    SF_btag_lfstats2Down *= bcr->eval_auto_bounds("down_lfstats2", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
                    SF_btag_hfstats1Up *= bcr->eval_auto_bounds("up_hfstats1", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
                    SF_btag_hfstats1Down *= bcr->eval_auto_bounds("down_hfstats1", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
                    SF_btag_hfstats2Up *= bcr->eval_auto_bounds("up_hfstats2", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
                    SF_btag_hfstats2Down *= bcr->eval_auto_bounds("down_hfstats2", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
                }
                // These systematics only affect c flavor
                // Evaluate other flavor SF at central
                if (flavor == BTagEntry::FLAV_C) {
                    SF_btag_cferr1Up *= bcr->eval_auto_bounds("up_cferr1", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
                    SF_btag_cferr1Down *= bcr->eval_auto_bounds("down_cferr1", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
                    SF_btag_cferr2Up *= bcr->eval_auto_bounds("up_cferr2", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
                    SF_btag_cferr2Down *= bcr->eval_auto_bounds("down_cferr2", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
                } else {
                    SF_btag_cferr1Up *= bcr->eval_auto_bounds("central", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
                    SF_btag_cferr1Down *= bcr->eval_auto_bounds("central", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
                    SF_btag_cferr2Up *= bcr->eval_auto_bounds("central", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
                    SF_btag_cferr2Down *= bcr->eval_auto_bounds("central", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
                }
            }
        } // End of jet loop

        // Scale the result
        SF_btag *= bfactor;
        if (makeSyst) {
            SF_btag_jesUp *= bfactor;
            SF_btag_jesDown *= bfactor;
            SF_btag_lfUp *= bfactor;
            SF_btag_lfDown *= bfactor;
            SF_btag_hfUp *= bfactor;
            SF_btag_hfDown *= bfactor;
            SF_btag_lfstats1Up *= bfactor;
            SF_btag_lfstats1Down *= bfactor;
            SF_btag_lfstats2Up *= bfactor;
            SF_btag_lfstats2Down *= bfactor;
            SF_btag_hfstats1Up *= bfactor;
            SF_btag_hfstats1Down *= bfactor;
            SF_btag_hfstats2Up *= bfactor;
            SF_btag_hfstats2Down *= bfactor;
            SF_btag_cferr1Up *= bfactor;
            SF_btag_cferr1Down *= bfactor;
            SF_btag_cferr2Up *= bfactor;
            SF_btag_cferr2Down *= bfactor;
        }

        Tout->Fill();
    } // End of event loop

    fin->WriteTObject(Tout, "", "Overwrite");
    fin->Close();

    return 0;
}
