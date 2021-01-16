/*
 * Producing permutation trees for leptonic ST signal reconstruction.
 * Usage:
 *   ./MVAreco_GenPerm_STlep <fin> <fout> <sig_train|sig_test|bkg>
 *
 *
 * For sig_train & sig_test modes, 2 trees are needed:
 *   1. `T`: Contain general info of events
 *   2. `TGenPart`: Contain info about gen particle indices of each gen final state
 * For bkg mode, only `T` tree is needed.
 *
 * A tree named "TPerm_ST" containing all generated permutations would be saved in the output file. Under
 * sig_train or sig_test modes, the match of permutations is saved, whereas under bkg mode the match
 * variable is set to null value.
 *
 * If number of jets in the event is not enough for reconstruction, a single entry with all branches set
 * to null values is saved to output, if not using sig_train mode.
 *
 * For each testing sig MC, bkg MC, and data sample in given channel, both TT & ST permutation-generating
 * codes should be run to get both `TPerm_TT` & `TPerm_ST`.
 */

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
using namespace std;

double dR_cut = 0.4; // Delta_R cut for truth matching

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fin_name = argv[1];
    TString fout_name = argv[2];
    TString nt_type = argv[3];

    if (nt_type!="sig_train" && nt_type!="sig_test" && nt_type!="bkg") {
        cout << "[ERROR] Invalid n-tuple type!\n";
        exit(1);
    }
    bool is_signal = nt_type=="sig_train" || nt_type=="sig_test";
    bool is_train = nt_type=="sig_train";

    // Read file
    cout << "[INFO] Openning file: " << fin_name << endl;
    TFile *fin = new TFile(fin_name);
    TTree *Tin = (TTree*)fin->Get("T");
    if (is_signal)  Tin->AddFriend("TGenPart");

    // Input tree variables
    int Evt_no = 0;
    float Evt_genweight = 0;
    // Photon
    float DiPho_leadPt = 0;
    float DiPho_leadEta = 0;
    float DiPho_leadPhi = 0;
    float DiPho_leadE = 0;
    int DiPho_leadGenMatchType = 0;
    float DiPho_subleadPt = 0;
    float DiPho_subleadEta = 0;
    float DiPho_subleadPhi = 0;
    float DiPho_subleadE = 0;
    int DiPho_subleadGenMatchType = 0;
    // Electron
    int Elec_Size = 0;
    vector<int> *Elec_Charge = 0;
    vector<float> *Elec_Pt = 0;
    vector<float> *Elec_Eta = 0;
    vector<float> *Elec_Phi = 0;
    vector<float> *Elec_Energy = 0;
    // Muon
    int Muon_Size = 0;
    vector<int> *Muon_Charge = 0;
    vector<float> *Muon_Pt = 0;
    vector<float> *Muon_Eta = 0;
    vector<float> *Muon_Phi = 0;
    vector<float> *Muon_Energy = 0;
    // Jet
    int jets_size = 0;
    vector<float> *Jet_Pt = 0;
    vector<float> *Jet_Eta = 0;
    vector<float> *Jet_Phi = 0;
    vector<float> *Jet_Energy = 0;
    vector<float> *Jet_probb = 0;
    vector<float> *Jet_probbb = 0;
    vector<float> *Jet_problepb = 0;
    // MET
    float Met_Pt = 0;
    float Met_Phi = 0;
    // Variables for signal samples only
    vector<float> *Gen_Pt = 0;
    vector<float> *Gen_Eta = 0;
    vector<float> *Gen_Phi = 0;
    vector<float> *Gen_Mass = 0;
    vector<int> *Gen_PdgID = 0;
    int Idx_bq = 0;
    int Idx_lq = 0;
    int Idx_Wq1 = 0;
    int Idx_Wq2 = 0;
    int Idx_lep = 0;

    // Set input tree branches
    Tin->SetBranchAddress("EvtInfo.EvtNo", &Evt_no);
    Tin->SetBranchAddress("EvtInfo.genweight", &Evt_genweight);
    // Photon
    Tin->SetBranchAddress("DiPhoInfo.leadPt", &DiPho_leadPt);
    Tin->SetBranchAddress("DiPhoInfo.leadEta", &DiPho_leadEta);
    Tin->SetBranchAddress("DiPhoInfo.leadPhi", &DiPho_leadPhi);
    Tin->SetBranchAddress("DiPhoInfo.leadE", &DiPho_leadE);
    Tin->SetBranchAddress("DiPhoInfo.leadGenMatchType", &DiPho_leadGenMatchType);
    Tin->SetBranchAddress("DiPhoInfo.subleadPt", &DiPho_subleadPt);
    Tin->SetBranchAddress("DiPhoInfo.subleadEta", &DiPho_subleadEta);
    Tin->SetBranchAddress("DiPhoInfo.subleadPhi", &DiPho_subleadPhi);
    Tin->SetBranchAddress("DiPhoInfo.subleadE", &DiPho_subleadE);
    Tin->SetBranchAddress("DiPhoInfo.subleadGenMatchType", &DiPho_subleadGenMatchType);
    // Electron
    Tin->SetBranchAddress("ElecInfo.Size", &Elec_Size);
    Tin->SetBranchAddress("ElecInfo.Charge", &Elec_Charge);
    Tin->SetBranchAddress("ElecInfo.Pt", &Elec_Pt);
    Tin->SetBranchAddress("ElecInfo.Eta", &Elec_Eta);
    Tin->SetBranchAddress("ElecInfo.Phi", &Elec_Phi);
    Tin->SetBranchAddress("ElecInfo.Energy", &Elec_Energy);
    // Muon
    Tin->SetBranchAddress("MuonInfo.Size", &Muon_Size);
    Tin->SetBranchAddress("MuonInfo.Charge", &Muon_Charge);
    Tin->SetBranchAddress("MuonInfo.Pt", &Muon_Pt);
    Tin->SetBranchAddress("MuonInfo.Eta", &Muon_Eta);
    Tin->SetBranchAddress("MuonInfo.Phi", &Muon_Phi);
    Tin->SetBranchAddress("MuonInfo.Energy", &Muon_Energy);
    // Jet
    Tin->SetBranchAddress("jets_size", &jets_size);
    Tin->SetBranchAddress("JetInfo.Pt", &Jet_Pt);
    Tin->SetBranchAddress("JetInfo.Eta", &Jet_Eta);
    Tin->SetBranchAddress("JetInfo.Phi", &Jet_Phi);
    Tin->SetBranchAddress("JetInfo.Energy", &Jet_Energy);
    Tin->SetBranchAddress("JetInfo.pfDeepFlavourJetTags_probb", &Jet_probb);
    Tin->SetBranchAddress("JetInfo.pfDeepFlavourJetTags_probbb", &Jet_probbb);
    Tin->SetBranchAddress("JetInfo.pfDeepFlavourJetTags_problepb", &Jet_problepb);
    // MET
    Tin->SetBranchAddress("MetInfo.Pt", &Met_Pt);
    Tin->SetBranchAddress("MetInfo.Phi", &Met_Phi);
    if (is_signal) {
        Tin->SetBranchAddress("GenPartInfo.Pt", &Gen_Pt);
        Tin->SetBranchAddress("GenPartInfo.Eta", &Gen_Eta);
        Tin->SetBranchAddress("GenPartInfo.Phi", &Gen_Phi);
        Tin->SetBranchAddress("GenPartInfo.Mass", &Gen_Mass);
        Tin->SetBranchAddress("GenPartInfo.PdgID", &Gen_PdgID);
        Tin->SetBranchAddress("Idx_bq", &Idx_bq);
        Tin->SetBranchAddress("Idx_lq", &Idx_lq);
        Tin->SetBranchAddress("Idx_Wq1", &Idx_Wq1);
        Tin->SetBranchAddress("Idx_Wq2", &Idx_Wq2);
        Tin->SetBranchAddress("Idx_lep", &Idx_lep);
    }

    // New variables for output tree
    int Perm_N = -1;
    int Perm_idx = -1;
    int Perm_match = -1;
    // b-jet
    int bJet_idx = -1;
    float bJet_Pt = -999;
    float bJet_Eta = -999;
    float bJet_btag = -999;
    // M1 jet (TT)
    int M1Jet_idx = -1;
    float M1Jet_Pt = -999;
    float M1Jet_Eta = -999;
    float M1Jet_btag = -999;
    // WJets (had)
    int WJet1_idx = -1;
    float WJet1_Pt = -999;
    float WJet1_Eta = -999;
    float WJet1_btag = -999;
    int WJet2_idx = -1;
    float WJet2_Pt = -999;
    float WJet2_Eta = -999;
    float WJet2_btag = -999;
    // Lepton (lep)
    int lep_idx = -1;
    float lep_ID = -999;
    float lep_Pt = -999;
    float lep_Eta = -999;
    // Invariance mass
    float M1 = -999; // TThad SThad TTlep
    float M2 = -999; // TT had
    float MW = -999; // had
    // Angle
    float dR_qH = -999; // TT
    float dR_bW = -999; // had
    float dR_tt = -999; // TT had
    float dR_tH = -999; // ST had
    float dR_qq = -999; // had
    float dR_lb = -999; // lep
    float dR_lt = -999; // TT lep
    float dR_lH = -999; // ST lep
    float dPhi_bMET = -999; // lep

    /* Attention: Create TFile BEFORE TTree (or there could be error in execution) */
    // Create output file
    TFile *fout = new TFile(fout_name, "update");

    // Create output tree
    TTree *Tout = new TTree("TPerm_ST", "");
    Tout->Branch("Evt_no", &Evt_no); // Directly read from input tree
    Tout->Branch("Evt_genweight", &Evt_genweight); // Directly read from input tree
    Tout->Branch("Perm_N", &Perm_N);
    Tout->Branch("Perm_idx", &Perm_idx);
    Tout->Branch("Perm_match", &Perm_match);
    // b-jet
    Tout->Branch("bJet_idx", &bJet_idx);
    Tout->Branch("bJet_Pt", &bJet_Pt);
    Tout->Branch("bJet_Eta", &bJet_Eta);
    Tout->Branch("bJet_btag", &bJet_btag);
    // M1 jet (TT)
    Tout->Branch("M1Jet_idx", &M1Jet_idx);
    Tout->Branch("M1Jet_Pt", &M1Jet_Pt);
    Tout->Branch("M1Jet_Eta", &M1Jet_Eta);
    Tout->Branch("M1Jet_btag", &M1Jet_btag);
    // WJets (had)
    Tout->Branch("WJet1_idx", &WJet1_idx);
    Tout->Branch("WJet1_Pt", &WJet1_Pt);
    Tout->Branch("WJet1_Eta", &WJet1_Eta);
    Tout->Branch("WJet1_btag", &WJet1_btag);
    Tout->Branch("WJet2_idx", &WJet2_idx);
    Tout->Branch("WJet2_Pt", &WJet2_Pt);
    Tout->Branch("WJet2_Eta", &WJet2_Eta);
    Tout->Branch("WJet2_btag", &WJet2_btag);
    // Lepton (lep)
    Tout->Branch("lep_idx", &lep_idx);
    Tout->Branch("lep_ID", &lep_ID);
    Tout->Branch("lep_Pt", &lep_Pt);
    Tout->Branch("lep_Eta", &lep_Eta);
    // Invariance mass
    Tout->Branch("M1", &M1);
    Tout->Branch("M2", &M2);
    Tout->Branch("MW", &MW);
    // Angle
    Tout->Branch("dR_qH", &dR_qH);
    Tout->Branch("dR_bW", &dR_bW);
    Tout->Branch("dR_tt", &dR_tt);
    Tout->Branch("dR_tH", &dR_tH);
    Tout->Branch("dR_qq", &dR_qq);
    Tout->Branch("dR_lb", &dR_lb);
    Tout->Branch("dR_lt", &dR_lt);
    Tout->Branch("dR_lH", &dR_lH);
    Tout->Branch("dPhi_bMET", &dPhi_bMET);
    // MET
    Tout->Branch("Met_Pt", &Met_Pt); // Directly read from input tree

    // Event / permutation counter
    int Nevt_tot = Tin->GetEntries();
    int Nperm = 0;

    // 4-momenta of gen-level particles
    TLorentzVector gen_bq, gen_lq, gen_Wq1, gen_Wq2, gen_lep;
    // 4-momenta of reconstructed objects
    TLorentzVector reco_bJet, reco_M1Jet, reco_WJet1, reco_WJet2, reco_lep;
    TLorentzVector reco_pho1, reco_pho2, reco_H, reco_W, reco_M1, reco_M2;

    // Start event loop
    cout << "Start processing...\n";
    for (int evt=0; evt<Nevt_tot; ++evt)
    {
        //if (evt % 10000 == 0) cout << "Processing event " << evt << endl;

        Tin->GetEntry(evt);

        // Initialize output variables
        Perm_N = -1;
        Perm_idx = -1;
        Perm_match = -1;
        // b-jet
        bJet_idx = -1;
        bJet_Pt = -999;
        bJet_Eta = -999;
        bJet_btag = -999;
        // M1 jet (TT)
        M1Jet_idx = -1;
        M1Jet_Pt = -999;
        M1Jet_Eta = -999;
        M1Jet_btag = -999;
        // WJets (had)
        WJet1_idx = -1;
        WJet1_Pt = -999;
        WJet1_Eta = -999;
        WJet1_btag = -999;
        WJet2_idx = -1;
        WJet2_Pt = -999;
        WJet2_Eta = -999;
        WJet2_btag = -999;
        // Lepton (lep)
        lep_idx = -1;
        lep_ID = -999;
        lep_Pt = -999;
        lep_Eta = -999;
        // Invariance mass
        M1 = -999;
        M2 = -999;
        MW = -999;
        // Angle
        dR_qH = -999;
        dR_bW = -999;
        dR_tt = -999;
        dR_tH = -999;
        dR_qq = -999;
        dR_lb = -999;
        dR_lt = -999;
        dR_lH = -999;
        dPhi_bMET = -999;

        // Set the 4-momenta of gen particles if processing signal sample
        if (is_signal) {
            gen_bq.SetPtEtaPhiM(Gen_Pt->at(Idx_bq), Gen_Eta->at(Idx_bq), Gen_Phi->at(Idx_bq), Gen_Mass->at(Idx_bq));
            gen_lep.SetPtEtaPhiM(Gen_Pt->at(Idx_lep), Gen_Eta->at(Idx_lep), Gen_Phi->at(Idx_lep), Gen_Mass->at(Idx_lep));
        }

        // Combine the input variables of electrons & muons, for later processing
        int Lep_Size = Elec_Size + Muon_Size;
        vector<int> Lep_ID(Lep_Size);
        vector<float> Lep_Pt(Lep_Size);
        vector<float> Lep_Eta(Lep_Size);
        vector<float> Lep_Phi(Lep_Size);
        vector<float> Lep_Energy(Lep_Size);
        for (int i=0; i<Lep_Size; ++i) {
            if (i < Elec_Size) {
                if (Elec_Charge->at(i) > 0) Lep_ID[i] = -11;
                else Lep_ID[i] = 11;
                Lep_Pt[i] = Elec_Pt->at(i);
                Lep_Eta[i] = Elec_Eta->at(i);
                Lep_Phi[i] = Elec_Phi->at(i);
                Lep_Energy[i] = Elec_Energy->at(i);
            } else {
                int j = i-Elec_Size;
                if (Muon_Charge->at(j) > 0) Lep_ID[i] = -13;
                else Lep_ID[i] = 13;
                Lep_Pt[i] = Muon_Pt->at(j);
                Lep_Eta[i] = Muon_Eta->at(j);
                Lep_Phi[i] = Muon_Phi->at(j);
                Lep_Energy[i] = Muon_Energy->at(j);
            }
        }

        // Calculate total number of perms for this event
        Perm_N = jets_size * Lep_Size;
        // Initalize permutation index
        Perm_idx = 0;

        // Set 4-momenta of photons & Higgs of this event
        reco_pho1.SetPtEtaPhiE(DiPho_leadPt, DiPho_leadEta, DiPho_leadPhi, DiPho_leadE);
        reco_pho2.SetPtEtaPhiE(DiPho_subleadPt, DiPho_subleadEta, DiPho_subleadPhi, DiPho_subleadE);
        reco_H = reco_pho1 + reco_pho2;

        // Start b-jet loop
        for (int i=0; i<jets_size; ++i) {
            // Start lepton loop
            for (int k=0; k<Lep_Size; ++k) {

                // Set 4-momenta of reco objects
                reco_bJet.SetPtEtaPhiE(Jet_Pt->at(i), Jet_Eta->at(i), Jet_Phi->at(i), Jet_Energy->at(i));
                reco_lep.SetPtEtaPhiE(Lep_Pt[k], Lep_Eta[k], Lep_Phi[k], Lep_Energy[k]);

                // Decide if reco & gen particles are matched if processing signal sample
                // When producing bkg tree, variable `Perm_match` is not modified and stay at -1
                if (is_signal) {
                    Perm_match = 0;
                    if (reco_bJet.DeltaR(gen_bq)<dR_cut &&
                            reco_lep.DeltaR(gen_lep)<dR_cut && Lep_ID[k]==Gen_PdgID->at(Idx_lep) &&
                            DiPho_leadGenMatchType==1 && DiPho_subleadGenMatchType==1)
                        Perm_match = 1;
                }

                bJet_idx = i;
                bJet_Pt = Jet_Pt->at(i);
                bJet_Eta = Jet_Eta->at(i);
                bJet_btag = Jet_probb->at(i)+Jet_probbb->at(i)+Jet_problepb->at(i);
                if (k >= Elec_Size) lep_idx = k-Elec_Size;
                else lep_idx = k;
                lep_ID = (float)Lep_ID[k];
                lep_Pt = Lep_Pt[k];
                lep_Eta = Lep_Eta[k];

                dR_lb = reco_lep.DeltaR(reco_bJet);
                dR_lH = reco_lep.DeltaR(reco_H);
                // Note: Phi of each object has range of [-pi, +pi]
                // Beware to use the "correct" delta_phi angle
                dPhi_bMET = fabs( reco_bJet.Phi() - Met_Phi );
                if (dPhi_bMET > TMath::Pi()) dPhi_bMET = 2 * TMath::Pi() - dPhi_bMET;

                Tout->Fill(); ++Nperm;
                ++Perm_idx;
            } // End lepton loop
        } // End b-jet loop
    } // End event loop

    // Save results
    cout << "[INFO] Saving file: " << fout_name << endl;
    fout->WriteTObject(Tout, "", "Overwrite");

    fout->Close();
    fin->Close();

    // Print summary
    cout << "\n[Summary]\n";
    cout << "# of input events: " << Nevt_tot << endl;
    cout << "# of permutations: " << Nperm << endl;

    return 0;
}
