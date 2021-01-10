// Find the gen particle that corresponds to hard interaction final states.
// Usage:
//   ./FindGenPart <fin_name> <inTree_name> <TT|ST> <had|lep>

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
    // Get command line arguments
    TString fin_name = argv[1];
    TString inTree_name = argv[2];
    TString type = argv[3];
    TString ch = argv[4];
    if (type!="TT" && type!="ST") {
        cout << "[ERROR] Invalid signal type!\n";
        exit(1);
    }
    if (ch!="had" && ch!="lep") {
        cout << "[ERROR] Invalid channel!\n";
        exit(1);
    }

    // Get input tree
    cout << "Openning input file: " << fin_name << endl;
    TFile *fin = new TFile(fin_name, "update");
    TTree *Tin = (TTree*)fin->Get(inTree_name);
    Tin->SetBranchStatus("*", 0);
    Tin->SetBranchStatus("GenPartInfo.size", 1);
    Tin->SetBranchStatus("GenPartInfo.PdgID", 1);
    Tin->SetBranchStatus("GenPartInfo.fromHardProcessFinalState", 1);
    Tin->SetBranchStatus("GenPartInfo.MomPdgID", 1);

    // Input variables
    int Gen_size = 0;
    vector<int> *Gen_PdgID = 0;
    vector<bool> *Gen_fromHardProcessFinalState = 0;
    vector<int> *Gen_MomPdgID = 0;
    // Set input branches
    Tin->SetBranchAddress("GenPartInfo.size", &Gen_size);
    Tin->SetBranchAddress("GenPartInfo.PdgID", &Gen_PdgID);
    Tin->SetBranchAddress("GenPartInfo.fromHardProcessFinalState", &Gen_fromHardProcessFinalState);
    Tin->SetBranchAddress("GenPartInfo.MomPdgID", &Gen_MomPdgID);

    // Output vairables
    int Idx_bq = 0;    // b-quark
    int Idx_lq = 0;    // light quark from top FCNH decay (TT only)
    int Idx_Wq1 = 0;   // light quark 1 from W decay (had channel only)
    int Idx_Wq2 = 0;   // light quark 2 from W decay (had channel only)
    int Idx_lep = 0;   // lepton from W decay (lep channel only)

    // Create output tree
    TTree *Tout = new TTree("TGenPart", "");
    Tout->Branch("Idx_bq", &Idx_bq);
    Tout->Branch("Idx_lq", &Idx_lq);
    Tout->Branch("Idx_Wq1", &Idx_Wq1);
    Tout->Branch("Idx_Wq2", &Idx_Wq2);
    Tout->Branch("Idx_lep", &Idx_lep);

    // Start event loop
    cout << "Start processing...\n";
    for (long long evt=0; evt<Tin->GetEntries(); ++evt)
    {
        Tin->GetEntry(evt);

        Idx_bq = -1;
        Idx_lq = -1;
        Idx_Wq1 = -1;
        Idx_Wq2 = -1;
        Idx_lep = -1;

        if (type=="TT" && ch=="had") {
            bool after_W = false;
            int i_WJet = 0;
            for (int i=0; i<Gen_size; ++i) { // start gen particle loop
                // The W from top
                if ( fabs(Gen_PdgID->at(i))==24 && fabs(Gen_MomPdgID->at(i))==6 ) after_W = true;
                // The b-jet from M2
                else if ( fabs(Gen_PdgID->at(i))==5 && fabs(Gen_MomPdgID->at(i))==6 )
                    Idx_bq = i;
                // The jet from M1
                else if ( (fabs(Gen_PdgID->at(i))==2 || fabs(Gen_PdgID->at(i))==4)
                        && fabs(Gen_MomPdgID->at(i))==6 )
                    Idx_lq = i;
                // The 2 jets from W
                else if ( (fabs(Gen_PdgID->at(i))>=1 && fabs(Gen_PdgID->at(i))<=4)
                        && fabs(Gen_MomPdgID->at(i))==24 && after_W ) {
                    if (i_WJet == 0) { Idx_Wq1 = i;  ++i_WJet; }
                    else Idx_Wq2 = i;
                }
            } // end gen particle loop
        }
        else if (type=="ST" && ch=="had") {
            bool after_W = false;
            int i_WJet = 0;
            for (int i=0; i<Gen_size; ++i) { // start gen particle loop
                // The W from top
                if ( fabs(Gen_PdgID->at(i))==24 && fabs(Gen_MomPdgID->at(i))==6 ) after_W = true;
                // The b-jet from M1
                else if ( fabs(Gen_PdgID->at(i))==5 && fabs(Gen_MomPdgID->at(i))==6 )
                    Idx_bq = i;
                // The 2 jets from W
                else if ( (fabs(Gen_PdgID->at(i))>=1 && fabs(Gen_PdgID->at(i))<=4)
                        && fabs(Gen_MomPdgID->at(i))==24 && after_W) {
                    if (i_WJet == 0) { Idx_Wq1 = i;  ++i_WJet; }
                    else Idx_Wq2 = i;
                }
            } // end gen particle loop
        }
        else if (type=="TT" && ch=="lep") {
            for (int i=0; i<Gen_size; ++i) { // start gen particle loop
                // The b-jet from M2
                if ( fabs(Gen_PdgID->at(i))==5 && fabs(Gen_MomPdgID->at(i))==6 )
                    Idx_bq = i;
                // The jet from M1
                else if ( (fabs(Gen_PdgID->at(i))==2 || fabs(Gen_PdgID->at(i))==4)
                        && fabs(Gen_MomPdgID->at(i))==6 )
                    Idx_lq = i;
                // The lepton from W
                else if ( (fabs(Gen_PdgID->at(i))==11 || fabs(Gen_PdgID->at(i))==13)
                        && fabs(Gen_MomPdgID->at(i))==24 && Gen_fromHardProcessFinalState->at(i) )
                    Idx_lep = i;
            } // end gen particle loop
        }
        else { // type=="ST" && ch=="lep"
            for (int i=0; i<Gen_size; ++i) { // start gen particle loop
                // The b-jet from M1
                if ( fabs(Gen_PdgID->at(i))==5 && fabs(Gen_MomPdgID->at(i))==6 )
                    Idx_bq = i;
                // The lepton from W
                else if ( (fabs(Gen_PdgID->at(i))==11 || fabs(Gen_PdgID->at(i))==13)
                        && fabs(Gen_MomPdgID->at(i))==24 && Gen_fromHardProcessFinalState->at(i) )
                    Idx_lep = i;
            } // end gen particle loop
        }

        Tout->Fill();
    } // End of event loop

    Tout->Write();
    fin->Close();
    cout << "Completed!\n";

    return 0;
}
