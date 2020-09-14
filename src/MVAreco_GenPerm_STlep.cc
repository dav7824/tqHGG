/*
 * Producing permutation trees for leptonic ST signal reconstruction.
 * Usage:
 *   ./MVAreco_GenPerm_STlep <fin> <fout> <sig|bkg>
 *
 *
 * For signal MC samples, 2 trees are needed:
 *   1. `T`: Contain general info of events
 *   2. `TGenPart`: Contain info about gen particle indices of each gen final state
 * For other kinds of samples, only `T` tree is needed.
 *
 * For signal MC samples, 2 trees would be produced:
 *   1. `TPerm_train`: Contain permutations for training MVA
 *   2. `TPerm_test`: Contain permutations for testing MVA and further use
 * For other kinds of samples, a tree would be produced:
 *   1. `TPerm_ST`
 *
 * For each bkg MC or data sample in given channel, both TT & ST permutation-generating codes
 * should be run to get both `TPerm_TT` & `TPerm_ST`.
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

float frac_train = 0.25;
double dR_cut = 0.4;

int main(int argc, char **argv)
{
	// Get command line arguments
	TString fin_name = argv[1];
	TString fout_name = argv[2];
	TString nt_type = argv[3];

	if (nt_type!="sig" && nt_type!="bkg") {
		cout << "[ERROR] Invalid n-tuple type!\n";
		exit(1);
	}
	bool is_signal = false;
	if (nt_type == "sig")  is_signal = true;

	// Read file
	cout << "[INFO] Openning file: " << fin_name << endl;
	TFile *fin = new TFile(fin_name);
	TTree *T = (TTree*)fin->Get("T");
	if (is_signal)  T->AddFriend("TGenPart");

	// Input tree variables
	float Evt_genweight = 0;
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
	int Elec_Size = 0;
	vector<int> *Elec_Charge = 0;
	vector<float> *Elec_Pt = 0;
	vector<float> *Elec_Eta = 0;
	vector<float> *Elec_Phi = 0;
	vector<float> *Elec_Energy = 0;
	int Muon_Size = 0;
	vector<int> *Muon_Charge = 0;
	vector<float> *Muon_Pt = 0;
	vector<float> *Muon_Eta = 0;
	vector<float> *Muon_Phi = 0;
	vector<float> *Muon_Energy = 0;
	int jets_size = 0;
	vector<float> *Jet_Pt = 0;
	vector<float> *Jet_Eta = 0;
	vector<float> *Jet_Phi = 0;
	vector<float> *Jet_Energy = 0;
	vector<float> *Jet_probb = 0;
	vector<float> *Jet_probbb = 0;
	float Met_Pt = 0;
	float Met_Phi = 0;
	// Variables for signal samples only
	vector<float> *Gen_Pt = 0;
	vector<float> *Gen_Eta = 0;
	vector<float> *Gen_Phi = 0;
	vector<float> *Gen_Mass = 0;
	vector<int> *Gen_PdgID = 0;
	int Idx_bq = 0;
	int Idx_lep = 0;

	// Set input tree branches
	T->SetBranchStatus("*", 0);
	T->SetBranchStatus("EvtInfo.genweight", 1);
	T->SetBranchStatus("DiPhoInfo.leadPt", 1);
	T->SetBranchStatus("DiPhoInfo.leadEta", 1);
	T->SetBranchStatus("DiPhoInfo.leadPhi", 1);
	T->SetBranchStatus("DiPhoInfo.leadE", 1);
	T->SetBranchStatus("DiPhoInfo.leadGenMatchType", 1);
	T->SetBranchStatus("DiPhoInfo.subleadPt", 1);
	T->SetBranchStatus("DiPhoInfo.subleadEta", 1);
	T->SetBranchStatus("DiPhoInfo.subleadPhi", 1);
	T->SetBranchStatus("DiPhoInfo.subleadE", 1);
	T->SetBranchStatus("DiPhoInfo.subleadGenMatchType", 1);
	T->SetBranchStatus("ElecInfo.Size", 1);
	T->SetBranchStatus("ElecInfo.Charge", 1);
	T->SetBranchStatus("ElecInfo.Pt", 1);
	T->SetBranchStatus("ElecInfo.Eta", 1);
	T->SetBranchStatus("ElecInfo.Phi", 1);
	T->SetBranchStatus("ElecInfo.Energy", 1);
	T->SetBranchStatus("MuonInfo.Size", 1);
	T->SetBranchStatus("MuonInfo.Charge", 1);
	T->SetBranchStatus("MuonInfo.Pt", 1);
	T->SetBranchStatus("MuonInfo.Eta", 1);
	T->SetBranchStatus("MuonInfo.Phi", 1);
	T->SetBranchStatus("MuonInfo.Energy", 1);
	T->SetBranchStatus("jets_size", 1);
	T->SetBranchStatus("JetInfo.Pt", 1);
	T->SetBranchStatus("JetInfo.Eta", 1);
	T->SetBranchStatus("JetInfo.Phi", 1);
	T->SetBranchStatus("JetInfo.Energy", 1);
	T->SetBranchStatus("JetInfo.pfDeepCSVJetTags_probb", 1);
	T->SetBranchStatus("JetInfo.pfDeepCSVJetTags_probbb", 1);
	T->SetBranchStatus("MetInfo.Pt", 1);
	T->SetBranchStatus("MetInfo.Phi", 1);
	if (is_signal) {
		T->SetBranchStatus("GenPartInfo.Pt", 1);
		T->SetBranchStatus("GenPartInfo.Eta", 1);
		T->SetBranchStatus("GenPartInfo.Phi", 1);
		T->SetBranchStatus("GenPartInfo.Mass", 1);
		T->SetBranchStatus("GenPartInfo.PdgID", 1);
		T->SetBranchStatus("Idx_bq", 1);
		T->SetBranchStatus("Idx_lep", 1);
	}
	T->SetBranchAddress("EvtInfo.genweight", &Evt_genweight);
	T->SetBranchAddress("DiPhoInfo.leadPt", &DiPho_leadPt);
	T->SetBranchAddress("DiPhoInfo.leadEta", &DiPho_leadEta);
	T->SetBranchAddress("DiPhoInfo.leadPhi", &DiPho_leadPhi);
	T->SetBranchAddress("DiPhoInfo.leadE", &DiPho_leadE);
	T->SetBranchAddress("DiPhoInfo.leadGenMatchType", &DiPho_leadGenMatchType);
	T->SetBranchAddress("DiPhoInfo.subleadPt", &DiPho_subleadPt);
	T->SetBranchAddress("DiPhoInfo.subleadEta", &DiPho_subleadEta);
	T->SetBranchAddress("DiPhoInfo.subleadPhi", &DiPho_subleadPhi);
	T->SetBranchAddress("DiPhoInfo.subleadE", &DiPho_subleadE);
	T->SetBranchAddress("DiPhoInfo.subleadGenMatchType", &DiPho_subleadGenMatchType);
	T->SetBranchAddress("ElecInfo.Size", &Elec_Size);
	T->SetBranchAddress("ElecInfo.Charge", &Elec_Charge);
	T->SetBranchAddress("ElecInfo.Pt", &Elec_Pt);
	T->SetBranchAddress("ElecInfo.Eta", &Elec_Eta);
	T->SetBranchAddress("ElecInfo.Phi", &Elec_Phi);
	T->SetBranchAddress("ElecInfo.Energy", &Elec_Energy);
	T->SetBranchAddress("MuonInfo.Size", &Muon_Size);
	T->SetBranchAddress("MuonInfo.Charge", &Muon_Charge);
	T->SetBranchAddress("MuonInfo.Pt", &Muon_Pt);
	T->SetBranchAddress("MuonInfo.Eta", &Muon_Eta);
	T->SetBranchAddress("MuonInfo.Phi", &Muon_Phi);
	T->SetBranchAddress("MuonInfo.Energy", &Muon_Energy);
	T->SetBranchAddress("jets_size", &jets_size);
	T->SetBranchAddress("JetInfo.Pt", &Jet_Pt);
	T->SetBranchAddress("JetInfo.Eta", &Jet_Eta);
	T->SetBranchAddress("JetInfo.Phi", &Jet_Phi);
	T->SetBranchAddress("JetInfo.Energy", &Jet_Energy);
	T->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probb", &Jet_probb);
	T->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probbb", &Jet_probbb);
	T->SetBranchAddress("MetInfo.Pt", &Met_Pt);
	T->SetBranchAddress("MetInfo.Phi", &Met_Phi);
	if (is_signal) {
		T->SetBranchAddress("GenPartInfo.Pt", &Gen_Pt);
		T->SetBranchAddress("GenPartInfo.Eta", &Gen_Eta);
		T->SetBranchAddress("GenPartInfo.Phi", &Gen_Phi);
		T->SetBranchAddress("GenPartInfo.Mass", &Gen_Mass);
		T->SetBranchAddress("GenPartInfo.PdgID", &Gen_PdgID);
		T->SetBranchAddress("Idx_bq", &Idx_bq);
		T->SetBranchAddress("Idx_lep", &Idx_lep);
	}

	// New variables for output tree
	int Evt_idx = 0;
	int NPerm = -1;
	int idxPerm = -1;
	int match = -1;
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

	// Create output file
	TFile *fout = new TFile(fout_name, "update");

	// Create output tree
	TTree *T_tmp = new TTree("T_tmp", "");
	T_tmp->Branch("Evt_idx", &Evt_idx);
	T_tmp->Branch("Evt_genweight", &Evt_genweight);
	T_tmp->Branch("NPerm", &NPerm);
	T_tmp->Branch("idxPerm", &idxPerm);
	T_tmp->Branch("match", &match);
	T_tmp->Branch("bJet_idx", &bJet_idx);
	T_tmp->Branch("bJet_Pt", &bJet_Pt);
	T_tmp->Branch("bJet_Eta", &bJet_Eta);
	T_tmp->Branch("bJet_btag", &bJet_btag);
	T_tmp->Branch("M1Jet_idx", &M1Jet_idx);
	T_tmp->Branch("M1Jet_Pt", &M1Jet_Pt);
	T_tmp->Branch("M1Jet_Eta", &M1Jet_Eta);
	T_tmp->Branch("M1Jet_btag", &M1Jet_btag);
	T_tmp->Branch("WJet1_idx", &WJet1_idx);
	T_tmp->Branch("WJet1_Pt", &WJet1_Pt);
	T_tmp->Branch("WJet1_Eta", &WJet1_Eta);
	T_tmp->Branch("WJet1_btag", &WJet1_btag);
	T_tmp->Branch("WJet2_idx", &WJet2_idx);
	T_tmp->Branch("WJet2_Pt", &WJet2_Pt);
	T_tmp->Branch("WJet2_Eta", &WJet2_Eta);
	T_tmp->Branch("WJet2_btag", &WJet2_btag);
	T_tmp->Branch("lep_idx", &lep_idx);
	T_tmp->Branch("lep_ID", &lep_ID);
	T_tmp->Branch("lep_Pt", &lep_Pt);
	T_tmp->Branch("lep_Eta", &lep_Eta);
	T_tmp->Branch("M1", &M1);
	T_tmp->Branch("M2", &M2);
	T_tmp->Branch("MW", &MW);
	T_tmp->Branch("dR_qH", &dR_qH);
	T_tmp->Branch("dR_bW", &dR_bW);
	T_tmp->Branch("dR_tt", &dR_tt);
	T_tmp->Branch("dR_tH", &dR_tH);
	T_tmp->Branch("dR_qq", &dR_qq);
	T_tmp->Branch("dR_lb", &dR_lb);
	T_tmp->Branch("dR_lt", &dR_lt);
	T_tmp->Branch("dR_lH", &dR_lH);
	T_tmp->Branch("dPhi_bMET", &dPhi_bMET);
	T_tmp->Branch("Met_Pt", &Met_Pt);
	TTree *TPerm_ST = 0;
	TTree *TPerm_train = 0, *TPerm_test = 0;
	if (is_signal) {
		TPerm_train = T_tmp;
		TPerm_test = T_tmp->CloneTree(0);
		TPerm_train->SetName("TPerm_train");
		TPerm_test->SetName("TPerm_test");
	} else {
		TPerm_ST = T_tmp;
		TPerm_ST->SetName("TPerm_ST");
	}

	int Nevt_tot = T->GetEntries();
	int Nevt_train = (int)(Nevt_tot * frac_train);

	int Nperm_train = 0;
	int Nperm_test = 0;
	int Nperm_bkg = 0;
	int Nperm_match = 0;
	int Nperm_unmatch = 0;

	TLorentzVector gen_bq, gen_lq, gen_Wq1, gen_Wq2, gen_lep;
	TLorentzVector reco_bJet, reco_M1Jet, reco_WJet1, reco_WJet2, reco_lep;
	TLorentzVector reco_pho1, reco_pho2, reco_H, reco_W, reco_M1, reco_M2;

	// Start event loop
	cout << "Start processing...\n";
	for (int evt=0; evt<Nevt_tot; ++evt)
	{
		//if (evt % 10000 == 0) cout << "Processing event " << evt << endl;

		T->GetEntry(evt);

		// Initialize output variables
		Evt_idx = evt;
		NPerm = -1;
		idxPerm = -1;
		match = -1;
		bJet_idx = -1;
		bJet_Pt = -999;
		bJet_Eta = -999;
		bJet_btag = -999;
		M1Jet_idx = -1;
		M1Jet_Pt = -999;
		M1Jet_Eta = -999;
		M1Jet_btag = -999;
		WJet1_idx = -1;
		WJet1_Pt = -999;
		WJet1_Eta = -999;
		WJet1_btag = -999;
		WJet2_idx = -1;
		WJet2_Pt = -999;
		WJet2_Eta = -999;
		WJet2_btag = -999;
		lep_idx = -1;
		lep_ID = -999;
		lep_Pt = -999;
		lep_Eta = -999;
		M1 = -999;
		M2 = -999;
		MW = -999;
		dR_qH = -999;
		dR_bW = -999;
		dR_tt = -999;
		dR_tH = -999;
		dR_qq = -999;
		dR_lb = -999;
		dR_lt = -999;
		dR_lH = -999;
		dPhi_bMET = -999;

		bool is_train = false;
		if (is_signal && evt<Nevt_train) is_train = true;

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

		NPerm = jets_size * Lep_Size;
		idxPerm = 0;

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
				// When producing bkg tree, variable `match` is not modified and stay at -1
				if (is_signal) {
					match = 0;
					if (reco_bJet.DeltaR(gen_bq)<dR_cut &&
							reco_lep.DeltaR(gen_lep)<dR_cut && Lep_ID[k]==Gen_PdgID->at(Idx_lep) &&
							DiPho_leadGenMatchType==1 && DiPho_subleadGenMatchType==1)
						match = 1;

					if (match) ++Nperm_match;
					else ++Nperm_unmatch;
				}

				// Fill output trees
				bJet_idx = i;
				bJet_Pt = Jet_Pt->at(i);
				bJet_Eta = Jet_Eta->at(i);
				bJet_btag = Jet_probb->at(i)+Jet_probbb->at(i);
				if (k >= Elec_Size) lep_idx = k-Elec_Size;
				else lep_idx = k;
				lep_ID = (float)(Lep_ID[k]);
				lep_Pt = Lep_Pt[k];
				lep_Eta = Lep_Eta[k];

				dR_lb = reco_lep.DeltaR(reco_bJet);
				dR_lH = reco_lep.DeltaR(reco_H);
				// Note: Phi of each object has range of [-pi, +pi]
				// Beware to use the "correct" delta_phi angle
				dPhi_bMET = fabs( reco_bJet.Phi() - Met_Phi );
				if (dPhi_bMET > TMath::Pi()) dPhi_bMET = 2 * TMath::Pi() - dPhi_bMET;

				if (is_signal) {
					if (is_train) {
						TPerm_train->Fill();
						++Nperm_train;
					} else {
						TPerm_test->Fill();
						++Nperm_test;
					}
				} else {
					TPerm_ST->Fill();
					++Nperm_bkg;
				}

				++idxPerm;
			} // End lepton loop
		} // End b-jet loop
	} // End event loop

	// Save results
	cout << "[INFO] Saving file: " << fout_name << endl;
	if (is_signal) {
		fout->WriteTObject(TPerm_train);
		fout->WriteTObject(TPerm_test);
	} else fout->WriteTObject(TPerm_ST);

	fout->Close();
	fin->Close();

	// Print summary
	cout << "\n[Summary]\n";
	cout << "# of input events: " << Nevt_tot << endl;
	if (is_signal) {
		cout << "# of training permutations: " << Nperm_train << endl;
		cout << "# of test permutations: " << Nperm_test << endl;
		cout << "# of matched permutations: " << Nperm_match << endl;
		cout << "# of unmatched permutations: " << Nperm_unmatch << endl;
	} else cout << "# of permutations: " << Nperm_bkg << endl;

	return 0;
}
