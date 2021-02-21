/*
 * Apply the trained MVA to get the reconstruction result.
 * Usage:
 *   ./MVAreco_application <fin> <fweight> <fout> <tree_type=sig|bkg> <reco_type=TT|ST> <channel=had|lep>
 *
 * <fin>:       Input root file with permutation tree
 * <fweight>:   Weight file containing the training result for given (TT|ST)x(had|lep) reconstruction
 * <fout>:      Output file where the best permutation of each event is saved
 * <tree_type>: Specify input tree type
 * <reco_type>: Specify signal type to be reconstructed
 * <channel>:   Specify channel type
 *
 * The matching rate will be calculated for signal tree.
 *
 * For each sample, run this code twice with the two weight files for TT and ST reconstruction.  The best
 * permutation for TT & ST reconstruction are respectively saved in trees "Treco_TT" & "Treco_ST" in output.
 */

#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "TString.h"
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <cstdlib>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fin_name        = argv[1];
    TString fweight_name    = argv[2];
    TString fout_name       = argv[3];
    TString tree_type       = argv[4];
    TString reco_type       = argv[5];
    TString channel         = argv[6];
    if (tree_type!="sig" && tree_type!="bkg") {
        cout << "[ERROR] Invalid tree type\n";
        exit(1);
    }
    if (reco_type!="TT" && reco_type!="ST") {
        cout << "[ERROR] Invalid reconstruction type\n";
        exit(1);
    }
    if (channel!="had" && channel!="lep") {
        cout << "[ERROR] Invalid channel\n";
        exit(1);
    }
    bool is_signal = false;
    if (tree_type == "sig")  is_signal = true;

    // Read permutation tree
    cout << "[INFO] Reading input file: " << fin_name << endl;
    TChain *inTree = 0;
    if (reco_type == "TT")
        inTree = new TChain("TPerm_TT");
    else
        inTree = new TChain("TPerm_ST");
    inTree->Add( fin_name );

    /* Input tree variables */
    int Evt_no = 0;
    int Perm_N = 0;
    int Perm_idx = 0;
    int Perm_match = 0;
    // b-jet
    int bJet_idx = 0;
    float bJet_Pt = 0;
    float bJet_Eta = 0;
    float bJet_btag = 0;
    // M1 jet (TT)
    int M1Jet_idx = 0;
    float M1Jet_Pt = 0;
    float M1Jet_Eta = 0;
    float M1Jet_btag = 0;
    // WJets (had)
    int WJet1_idx = 0;
    float WJet1_Pt = 0;
    float WJet1_Eta = 0;
    float WJet1_btag = 0;
    int WJet2_idx = 0;
    float WJet2_Pt = 0;
    float WJet2_Eta = 0;
    float WJet2_btag = 0;
    // Lepton (lep)
    int lep_idx = 0;
    float lep_ID = 0;
    float lep_Pt = 0;
    float lep_Eta = 0;
    // Invariance mass
    float M1 = 0; // TThad SThad TTlep
    float M2 = 0; // TT had
    float MW = 0; // had
    // Angle
    float dR_qH = 0; // TT
    float dR_bW = 0; // had
    float dR_tt = 0; // TT had
    float dR_tH = 0; // ST had
    float dR_qq = 0; // had
    float dR_lb = 0; // lep
    float dR_lt = 0; // TT lep
    float dR_lH = 0; // ST lep
    float dPhi_bMET = 0; // lep
    float Met_Pt = 0; // lep

    // Set input tree branches
    inTree->SetBranchAddress("Evt_no", &Evt_no);
    inTree->SetBranchAddress("Perm_N", &Perm_N);
    inTree->SetBranchAddress("Perm_idx", &Perm_idx);
    inTree->SetBranchAddress("Perm_match", &Perm_match);
    inTree->SetBranchAddress("bJet_idx", &bJet_idx);
    inTree->SetBranchAddress("bJet_Pt", &bJet_Pt);
    inTree->SetBranchAddress("bJet_Eta", &bJet_Eta);
    inTree->SetBranchAddress("bJet_btag", &bJet_btag);
    inTree->SetBranchAddress("M1Jet_idx", &M1Jet_idx);
    inTree->SetBranchAddress("M1Jet_Pt", &M1Jet_Pt);
    inTree->SetBranchAddress("M1Jet_Eta", &M1Jet_Eta);
    inTree->SetBranchAddress("M1Jet_btag", &M1Jet_btag);
    inTree->SetBranchAddress("WJet1_idx", &WJet1_idx);
    inTree->SetBranchAddress("WJet1_Pt", &WJet1_Pt);
    inTree->SetBranchAddress("WJet1_Eta", &WJet1_Eta);
    inTree->SetBranchAddress("WJet1_btag", &WJet1_btag);
    inTree->SetBranchAddress("WJet2_idx", &WJet2_idx);
    inTree->SetBranchAddress("WJet2_Pt", &WJet2_Pt);
    inTree->SetBranchAddress("WJet2_Eta", &WJet2_Eta);
    inTree->SetBranchAddress("WJet2_btag", &WJet2_btag);
    inTree->SetBranchAddress("lep_idx", &lep_idx);
    inTree->SetBranchAddress("lep_ID", &lep_ID);
    inTree->SetBranchAddress("lep_Pt", &lep_Pt);
    inTree->SetBranchAddress("lep_Eta", &lep_Eta);
    inTree->SetBranchAddress("M1", &M1);
    inTree->SetBranchAddress("M2", &M2);
    inTree->SetBranchAddress("MW", &MW);
    inTree->SetBranchAddress("dR_qH", &dR_qH);
    inTree->SetBranchAddress("dR_bW", &dR_bW);
    inTree->SetBranchAddress("dR_tt", &dR_tt);
    inTree->SetBranchAddress("dR_tH", &dR_tH);
    inTree->SetBranchAddress("dR_qq", &dR_qq);
    inTree->SetBranchAddress("dR_lb", &dR_lb);
    inTree->SetBranchAddress("dR_lt", &dR_lt);
    inTree->SetBranchAddress("dR_lH", &dR_lH);
    inTree->SetBranchAddress("dPhi_bMET", &dPhi_bMET);
    inTree->SetBranchAddress("Met_Pt", &Met_Pt);

    // Create output tree
    TFile *fout = new TFile( fout_name, "update" );
    TTree *outTree = inTree->CloneTree(0);
    if (reco_type == "TT") outTree->SetName("Treco_TT");
    else outTree->SetName("Treco_ST");

    /* Additional output tree variables */
    float MVA_score = 0;
    outTree->Branch("MVA_score", &MVA_score);

    // Load TMVA library
    TMVA::Tools::Instance();

    // Create TMVA reader
    TMVA::Reader *reader = new TMVA::Reader("V");

    // Set discriminating variables
    // Obj variables
    reader->AddVariable("bJet_Pt", &bJet_Pt);
    reader->AddVariable("bJet_Eta", &bJet_Eta);
    reader->AddVariable("bJet_btag", &bJet_btag);
    if (reco_type == "TT") {
        reader->AddVariable("M1Jet_Pt", &M1Jet_Pt);
        reader->AddVariable("M1Jet_Eta", &M1Jet_Eta);
        reader->AddVariable("M1Jet_btag", &M1Jet_btag);
    }
    if (channel == "had") {
        reader->AddVariable("WJet1_Pt", &WJet1_Pt);
        reader->AddVariable("WJet1_Eta", &WJet1_Eta);
        reader->AddVariable("WJet1_btag", &WJet1_btag);
        reader->AddVariable("WJet2_Pt", &WJet2_Pt);
        reader->AddVariable("WJet2_Eta", &WJet2_Eta);
        reader->AddVariable("WJet2_btag", &WJet2_btag);
    }
    else if (channel == "lep") {
        reader->AddVariable("lep_ID", &lep_ID);
        reader->AddVariable("lep_Pt", &lep_Pt);
        reader->AddVariable("lep_Eta", &lep_Eta);
    }
    // Inv. mass & relative angles
    if (reco_type=="TT" && channel=="had") {
        reader->AddVariable("M1", &M1);
        reader->AddVariable("M2", &M2);
        reader->AddVariable("MW", &MW);
        reader->AddVariable("dR_qH", &dR_qH);
        reader->AddVariable("dR_bW", &dR_bW);
        reader->AddVariable("dR_tt", &dR_tt);
        reader->AddVariable("dR_qq", &dR_qq);
    } else if (reco_type=="ST" && channel=="had") {
        reader->AddVariable("M1", &M1);
        reader->AddVariable("MW", &MW);
        reader->AddVariable("dR_bW", &dR_bW);
        reader->AddVariable("dR_tH", &dR_tH);
        reader->AddVariable("dR_qq", &dR_qq);
    } else if (reco_type=="TT" && channel=="lep") {
        reader->AddVariable("M1", &M1);
        reader->AddVariable("dR_qH", &dR_qH);
        reader->AddVariable("dR_lb", &dR_lb);
        reader->AddVariable("dR_lt", &dR_lt);
    } else if (reco_type=="ST" && channel=="lep") {
        reader->AddVariable("dR_lb", &dR_lb);
        reader->AddVariable("dR_lH", &dR_lH);
    }
    if (channel=="lep") {
        reader->AddVariable("dPhi_bMET", &dPhi_bMET);
        reader->AddVariable("Met_Pt", &Met_Pt);
    }

    // Book MVA methods
    TString mva_name = reco_type+channel;
    reader->BookMVA(mva_name, fweight_name);

    /* Calculating matching rates (for signal tree) */
    // Number of events that can be reconstructed (only count non-training events (Evt_no%3!=0))
    int Nevt_reco = 0;
    // Number of reconstructed events that are truth matched (only count non-training events (Evt_no%3!=0))
    int Nevt_reco_match = 0;

    int best_perm = -1;
    float best_score = -999;

    // Start permutation loop
    for (int perm=0; perm<inTree->GetEntries(); ++perm) {
        inTree->GetEntry(perm);

        // Initialize output branch
        MVA_score = -999;

        // For events that can't be reconstructed (Perm_N = -1), fill null values
        if (Perm_N < 0) {
            outTree->Fill();
            continue;
        }

        // For reconstructable events, there will be "Perm_N" continuous entries of permutations.
        // For each of them, calculate the corresponding MVA score, and record the highest score so far.
        float score = reader->EvaluateMVA( mva_name );
        if (score > best_score) {
            best_score = score;
            best_perm = perm;
        }
        // In the last iteration, fill the highest score and the corresponding permutation to output.
        if (Perm_idx == Perm_N-1) {
            inTree->GetEntry(best_perm);
            MVA_score = best_score;
            outTree->Fill();

            // Reset for next event
            best_perm = -1;
            best_score = -999;

            // Update counter
            if (is_signal && (Evt_no%3 != 0)) {
                ++Nevt_reco;
                if (Perm_match == 1)  ++Nevt_reco_match;
            }
        }
    } // End of permutation loop

    fout->Write();
    fout->Close();
    delete reader;

    cout << "[INFO] Saved output: " << fout_name << endl;

    // If processing signal tree, print matching rate
    if (is_signal) {
        cout << "[INFO] Reconstruction matching rate: " << (float)Nevt_reco_match / Nevt_reco
            << "  (" << Nevt_reco_match << '/' << Nevt_reco << ")\n";
    }

    return 0;
}
