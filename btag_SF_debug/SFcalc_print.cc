#include "BTagCalibrationStandalone.h"
#include "BTagCalibrationStandalone.cpp"

#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include <iostream>
#include <cstdio>
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
    // Loose wp
    //BTagCalibrationReader bcr(BTagEntry::OP_LOOSE);
    cout << "[INFO] Load calib...\n";
    // Reshaping
    bcr.load(bc, BTagEntry::FLAV_B, "iterativefit");
    // Loose wp
    //bcr.load(bc, BTagEntry::FLAV_B, "comb");

    cout << "[INFO] Openning input file: " << fin_name << endl;
    TFile *fin = new TFile(fin_name);
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

    vector<float> *SF_jet = new vector<float>();
    float SF_evt = 0;

    // Start event loop
    for (int evt=0; evt<100; ++evt)
    {
        T->GetEntry(evt);

        SF_jet->clear();
        SF_evt = 1;

        cout << "\n[ Evt " << evt << " ]\n";

        // Start jet loop
        for (int i=0; i<jets_size; ++i) {
            BTagEntry::JetFlavor flavor = BTagEntry::FLAV_UDSG;
            int flavor_ = 0;
            switch (Jet_GenHadronFlavor->at(i)) {
                case 0:
                    flavor = BTagEntry::FLAV_UDSG;
                    flavor_ = 2;
                    break;
                case 4:
                    flavor = BTagEntry::FLAV_C;
                    flavor_ = 1;
                    break;
                case 5:
                    flavor = BTagEntry::FLAV_B;
                    flavor_ = 0;
                    break;
                default:
                    cout << "[ERROR] Invalid hadron flavor!\n";
                    exit(1);
            }
            // Reshaping
            float sf = bcr.eval_auto_bounds("central", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i));
            // Loose wp
            //float sf = bcr.eval_auto_bounds("central", flavor, fabs(Jet_Eta->at(i)), Jet_Pt->at(i));
            SF_jet->push_back( sf );
            SF_evt *= sf;

            printf("Jet %2d    %d   %f   %f   %f      %f\n", i, flavor_, fabs(Jet_Eta->at(i)), Jet_Pt->at(i), Jet_probb->at(i)+Jet_probbb->at(i), sf);
        } // End of jet loop

        cout << "< SF_evt = " << SF_evt << " >\n";
    } // End of event loop

    fin->Close();

    return 0;
}
