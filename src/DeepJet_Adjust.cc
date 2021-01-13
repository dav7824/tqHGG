/*
 * Change DeepJet b-tagging score unknown value from -1000 to -1.
 *
 * DeepJet_Adjust <fin> <tree>
 */

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fin_name(argv[1]);
    TString tree_name(argv[2]);

    // Get input file
    cout << "[INFO] Reading file: " << fin_name << endl;
    TFile *fin = new TFile(fin_name, "update");
    TTree *Tin = (TTree*)fin->Get(tree_name);
    // Set input tree
    Tin->SetBranchStatus("JetInfo.pfDeepFlavourJetTags_probb", 0);
    Tin->SetBranchStatus("JetInfo.pfDeepFlavourJetTags_probbb", 0);
    Tin->SetBranchStatus("JetInfo.pfDeepFlavourJetTags_probc", 0);
    Tin->SetBranchStatus("JetInfo.pfDeepFlavourJetTags_probuds", 0);
    Tin->SetBranchStatus("JetInfo.pfDeepFlavourJetTags_probg", 0);
    Tin->SetBranchStatus("JetInfo.pfDeepFlavourJetTags_problepb", 0);

    // Create output tree
    TTree *Tout = Tin->CloneTree(0); // Set output branch addresses to input branch addresses, except jet b-tag branches

    // Input b-tag score variables
    int jets_size = 0;
    vector<float> *Jet_probb = 0;
    vector<float> *Jet_probbb = 0;
    vector<float> *Jet_probc = 0;
    vector<float> *Jet_probuds = 0;
    vector<float> *Jet_probg = 0;
    vector<float> *Jet_problepb = 0;
    Tin->SetBranchStatus("JetInfo.pfDeepFlavourJetTags_probb", 1);
    Tin->SetBranchStatus("JetInfo.pfDeepFlavourJetTags_probbb", 1);
    Tin->SetBranchStatus("JetInfo.pfDeepFlavourJetTags_probc", 1);
    Tin->SetBranchStatus("JetInfo.pfDeepFlavourJetTags_probuds", 1);
    Tin->SetBranchStatus("JetInfo.pfDeepFlavourJetTags_probg", 1);
    Tin->SetBranchStatus("JetInfo.pfDeepFlavourJetTags_problepb", 1);
    Tin->SetBranchAddress("jets_size", &jets_size);
    Tin->SetBranchAddress("JetInfo.pfDeepFlavourJetTags_probb", &Jet_probb);
    Tin->SetBranchAddress("JetInfo.pfDeepFlavourJetTags_probbb", &Jet_probbb);
    Tin->SetBranchAddress("JetInfo.pfDeepFlavourJetTags_probc", &Jet_probc);
    Tin->SetBranchAddress("JetInfo.pfDeepFlavourJetTags_probuds", &Jet_probuds);
    Tin->SetBranchAddress("JetInfo.pfDeepFlavourJetTags_probg", &Jet_probg);
    Tin->SetBranchAddress("JetInfo.pfDeepFlavourJetTags_problepb", &Jet_problepb);
    // Output b-tag score variables
    vector<float> *Jet_probb_ = 0;
    vector<float> *Jet_probbb_ = 0;
    vector<float> *Jet_probc_ = 0;
    vector<float> *Jet_probuds_ = 0;
    vector<float> *Jet_probg_ = 0;
    vector<float> *Jet_problepb_ = 0;
    Tout->Branch("JetInfo.pfDeepFlavourJetTags_probb", &Jet_probb_);
    Tout->Branch("JetInfo.pfDeepFlavourJetTags_probbb", &Jet_probbb_);
    Tout->Branch("JetInfo.pfDeepFlavourJetTags_probc", &Jet_probc_);
    Tout->Branch("JetInfo.pfDeepFlavourJetTags_probuds", &Jet_probuds_);
    Tout->Branch("JetInfo.pfDeepFlavourJetTags_probg", &Jet_probg_);
    Tout->Branch("JetInfo.pfDeepFlavourJetTags_problepb", &Jet_problepb_);

    // Start event loop
    for (int evt=0; evt<Tin->GetEntries(); ++evt)
    {
        // Reset output b-tag branches
        Jet_probb_->clear();
        Jet_probbb_->clear();
        Jet_probc_->clear();
        Jet_probuds_->clear();
        Jet_probg_->clear();
        Jet_problepb_->clear();

        Tin->GetEntry(evt);

        // Start jet loop
        for (int i=0; i<jets_size; ++i) {
            // probb
            if (Jet_probb->at(i) < 0)
                Jet_probb_->push_back(-1);
            else
                Jet_probb_->push_back( Jet_probb->at(i) );
            // probbb
            if (Jet_probbb->at(i) < 0)
                Jet_probbb_->push_back(-1);
            else
                Jet_probbb_->push_back( Jet_probbb->at(i) );
            // probc
            if (Jet_probc->at(i) < 0)
                Jet_probc_->push_back(-1);
            else
                Jet_probc_->push_back( Jet_probc->at(i) );
            // probuds
            if (Jet_probuds->at(i) < 0)
                Jet_probuds_->push_back(-1);
            else
                Jet_probuds_->push_back( Jet_probuds->at(i) );
            // probg
            if (Jet_probg->at(i) < 0)
                Jet_probg_->push_back(-1);
            else
                Jet_probg_->push_back( Jet_probg->at(i) );
            // problepb
            if (Jet_problepb->at(i) < 0)
                Jet_problepb_->push_back(-1);
            else
                Jet_problepb_->push_back( Jet_problepb->at(i) );
        } // End jet loop

        Tout->Fill();
    }
    // End of event loop

    // Save result
    fin->WriteTObject(Tout, "", "Overwrite");
    fin->Close();

    cout << "[INFO] Complete\n";

    return 0;
}
