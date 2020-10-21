#include "include/BTagCalibrationStandalone.h"
#include "include/BTagCalibrationStandalone.cpp"

#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
using namespace std;

int main(int argc, char **argv)
{
    TString fin_name = argv[1];
    TString fin_csv = argv[2];

    cout << "[INFO] Openning csv: " << fin_csv << endl;
    BTagCalibration bc("csv", fin_csv.Data());
    cout << "[INFO] Set reader...\n";
    // Reshaping
    BTagCalibrationReader bcr(BTagEntry::OP_RESHAPING);
    cout << "[INFO] Load calib...\n";
    bcr.load(bc, BTagEntry::FLAV_B, "iterativefit");
    bcr.load(bc, BTagEntry::FLAV_C, "iterativefit");
    bcr.load(bc, BTagEntry::FLAV_UDSG, "iterativefit");

    cout << "[INFO] Openning input file: " << fin_name << endl;
    TFile *fin = new TFile(fin_name, "update");
    TTree *T = (TTree*)fin->Get("T");
    int jets_size = 0;
    vector<float> *Jet_Pt = 0;
    vector<float> *Jet_Eta = 0;
    vector<float> *Jet_probb = 0;
    vector<float> *Jet_probbb = 0;
    vector<int> *Jet_GenHadronFlavor = 0;
    T->SetBranchStatus("*", 0);
    T->SetBranchStatus("jets_size", 1);
    T->SetBranchStatus("JetInfo.Pt", 1);
    T->SetBranchStatus("JetInfo.Eta", 1);
    T->SetBranchStatus("JetInfo.pfDeepCSVJetTags_probb", 1);
    T->SetBranchStatus("JetInfo.pfDeepCSVJetTags_probbb", 1);
    T->SetBranchStatus("JetInfo.GenHadronFlavor", 1);
    T->SetBranchAddress("jets_size", &jets_size);
    T->SetBranchAddress("JetInfo.Pt", &Jet_Pt);
    T->SetBranchAddress("JetInfo.Eta", &Jet_Eta);
    T->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probb", &Jet_probb);
    T->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probbb", &Jet_probbb);
    T->SetBranchAddress("JetInfo.GenHadronFlavor", &Jet_GenHadronFlavor);

    TTree *Tbtag = new TTree("SF_btag", "");
    vector<float> *SF_btag_jets = 0;    // vector with SF of each jet
    float SF_btag = 0;    // SF of whole event
    Tbtag->Branch("SF_btag_jets", &SF_btag_jets);
    Tbtag->Branch("SF_btag", &SF_btag);

    // Start event loop
    for (int evt=0; evt<T->GetEntries(); ++evt)
    {
        T->GetEntry(evt);
        //if (evt % 100 == 0) cout << "Processing event " << evt << endl;

        SF_btag_jets->clear();
        SF_btag = 1;

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
            SF_btag_jets->push_back( sf );
            SF_btag *= sf;
        } // End of jet loop

        Tbtag->Fill();
    } // End of event loop

    fin->WriteTObject(Tbtag, "", "Overwrite");
    fin->Close();

    return 0;
}
