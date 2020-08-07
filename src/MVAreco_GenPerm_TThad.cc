/*
 * Producing permutation trees for hadronic TT signal reconstruction.
 * Usage:
 *   ./MVAreco_GenPerm_TThad <fin> <fout> <sig|bkg>
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
 *   1. `TPerm_TT`
 *
 * Only jets of the highest Pt (lowest indices) are considered when generating permutations,
 * since the other jets rarely have any match to gen particles and thus unimportant. By this
 * way we can reduce the number of considered permutations.
 *
 * For training samples, permutation number is further reduced by random sampling from all
 * possible permutations of the significant jets.
 *
 * For each bkg MC or data sample in given channel, both TT & ST permutation-generating codes
 * should be run to get both `TPerm_TT` & `TPerm_ST`.
 */

#include "include/MVAreco_GenPerm.h"

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"

#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

float frac_train = 0.25;
int NJet_sig = 7;
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
	float DiPho_leadIDMVA = 0;
	int DiPho_leadGenMatchType = 0;
	float DiPho_subleadPt = 0;
	float DiPho_subleadEta = 0;
	float DiPho_subleadPhi = 0;
	float DiPho_subleadIDMVA = 0;
	int DiPho_subleadGenMatchType = 0;
	int jets_size = 0;
	vector<float> *Jet_Pt = 0;
	vector<float> *Jet_Eta = 0;
	vector<float> *Jet_Phi = 0;
	vector<float> *Jet_Energy = 0;
	vector<float> *Jet_probb = 0;
	vector<float> *Jet_probbb = 0;
	// Variables for signal samples only
	vector<float> *Gen_Pt = 0;
	vector<float> *Gen_Eta = 0;
	vector<float> *Gen_Phi = 0;
	vector<float> *Gen_Mass = 0;
	int Idx_bq = 0;
	int Idx_lq = 0;
	int Idx_Wq1 = 0;
	int Idx_Wq2 = 0;

	// Set input tree branches
	T->SetBranchStatus("*", 0);
	T->SetBranchStatus("EvtInfo.genweight", 1);
	T->SetBranchStatus("DiPhoInfo.leadPt", 1);
	T->SetBranchStatus("DiPhoInfo.leadEta", 1);
	T->SetBranchStatus("DiPhoInfo.leadPhi", 1);
	T->SetBranchStatus("DiPhoInfo.leadIDMVA", 1);
	T->SetBranchStatus("DiPhoInfo.leadGenMatchType", 1);
	T->SetBranchStatus("DiPhoInfo.subleadPt", 1);
	T->SetBranchStatus("DiPhoInfo.subleadEta", 1);
	T->SetBranchStatus("DiPhoInfo.subleadPhi", 1);
	T->SetBranchStatus("DiPhoInfo.subleadIDMVA", 1);
	T->SetBranchStatus("DiPhoInfo.subleadGenMatchType", 1);
	T->SetBranchStatus("jets_size", 1);
	T->SetBranchStatus("JetInfo.Pt", 1);
	T->SetBranchStatus("JetInfo.Eta", 1);
	T->SetBranchStatus("JetInfo.Phi", 1);
	T->SetBranchStatus("JetInfo.Energy", 1);
	T->SetBranchStatus("JetInfo.pfDeepCSVJetTags_probb", 1);
	T->SetBranchStatus("JetInfo.pfDeepCSVJetTags_probbb", 1);
	if (is_signal) {
		T->SetBranchStatus("GenPartInfo.Pt", 1);
		T->SetBranchStatus("GenPartInfo.Eta", 1);
		T->SetBranchStatus("GenPartInfo.Phi", 1);
		T->SetBranchStatus("GenPartInfo.Mass", 1);
		T->SetBranchStatus("Idx_bq", 1);
		T->SetBranchStatus("Idx_lq", 1);
		T->SetBranchStatus("Idx_Wq1", 1);
		T->SetBranchStatus("Idx_Wq2", 1);
	}
	T->SetBranchAddress("EvtInfo.genweight", &Evt_genweight);
	T->SetBranchAddress("DiPhoInfo.leadPt", &DiPho_leadPt);
	T->SetBranchAddress("DiPhoInfo.leadEta", &DiPho_leadEta);
	T->SetBranchAddress("DiPhoInfo.leadPhi", &DiPho_leadPhi);
	T->SetBranchAddress("DiPhoInfo.leadIDMVA", &DiPho_leadIDMVA);
	T->SetBranchAddress("DiPhoInfo.leadGenMatchType", &DiPho_leadGenMatchType);
	T->SetBranchAddress("DiPhoInfo.subleadPt", &DiPho_subleadPt);
	T->SetBranchAddress("DiPhoInfo.subleadEta", &DiPho_subleadEta);
	T->SetBranchAddress("DiPhoInfo.subleadPhi", &DiPho_subleadPhi);
	T->SetBranchAddress("DiPhoInfo.subleadIDMVA", &DiPho_subleadIDMVA);
	T->SetBranchAddress("DiPhoInfo.subleadGenMatchType", &DiPho_subleadGenMatchType);
	T->SetBranchAddress("jets_size", &jets_size);
	T->SetBranchAddress("JetInfo.Pt", &Jet_Pt);
	T->SetBranchAddress("JetInfo.Eta", &Jet_Eta);
	T->SetBranchAddress("JetInfo.Phi", &Jet_Phi);
	T->SetBranchAddress("JetInfo.Energy", &Jet_Energy);
	T->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probb", &Jet_probb);
	T->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probbb", &Jet_probbb);
	if (is_signal) {
		T->SetBranchAddress("GenPartInfo.Pt", &Gen_Pt);
		T->SetBranchAddress("GenPartInfo.Eta", &Gen_Eta);
		T->SetBranchAddress("GenPartInfo.Phi", &Gen_Phi);
		T->SetBranchAddress("GenPartInfo.Mass", &Gen_Mass);
		T->SetBranchAddress("Idx_bq", &Idx_bq);
		T->SetBranchAddress("Idx_lq", &Idx_lq);
		T->SetBranchAddress("Idx_Wq1", &Idx_Wq1);
		T->SetBranchAddress("Idx_Wq2", &Idx_Wq2);
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
	float bJet_Phi = -999;
	float bJet_btag = -999;
	// M1 jet (TT)
	int M1Jet_idx = -1;
	float M1Jet_Pt = -999;
	float M1Jet_Eta = -999;
	float M1Jet_Phi = -999;
	float M1Jet_btag = -999;
	// WJets (had)
	int WJet1_idx = -1;
	float WJet1_Pt = -999;
	float WJet1_Eta = -999;
	float WJet1_Phi = -999;
	float WJet1_btag = -999;
	int WJet2_idx = -1;
	float WJet2_Pt = -999;
	float WJet2_Eta = -999;
	float WJet2_Phi = -999;
	float WJet2_btag = -999;
	// Lepton (lep)
	int lep_idx = -1;
	float lep_ID = -999;
	float lep_Pt = -999;
	float lep_Eta = -999;
	float lep_Phi = -999;

	/* LESSON: Create TFile BEFORE TTree (See near the end of code) */
	// Create output file
	TFile *fout = new TFile(fout_name, "update");

	// Create output tree
	TTree *T_tmp = new TTree("T_tmp", "");
	T_tmp->Branch("Evt_idx", &Evt_idx);
	T_tmp->Branch("Evt_genweight", &Evt_genweight);
	T_tmp->Branch("NPerm", &NPerm);
	T_tmp->Branch("idxPerm", &idxPerm);
	T_tmp->Branch("match", &match);
	T_tmp->Branch("LeadPho_Pt", &DiPho_leadPt);
	T_tmp->Branch("LeadPho_Eta", &DiPho_leadEta);
	T_tmp->Branch("LeadPho_Phi", &DiPho_leadPhi);
	T_tmp->Branch("LeadPho_IDMVA", &DiPho_leadIDMVA);
	T_tmp->Branch("SubleadPho_Pt", &DiPho_subleadPt);
	T_tmp->Branch("SubleadPho_Eta", &DiPho_subleadEta);
	T_tmp->Branch("SubleadPho_Phi", &DiPho_subleadPhi);
	T_tmp->Branch("SubleadPho_IDMVA", &DiPho_subleadIDMVA);
	T_tmp->Branch("bJet_idx", &bJet_idx);
	T_tmp->Branch("bJet_Pt", &bJet_Pt);
	T_tmp->Branch("bJet_Eta", &bJet_Eta);
	T_tmp->Branch("bJet_Phi", &bJet_Phi);
	T_tmp->Branch("bJet_btag", &bJet_btag);
	T_tmp->Branch("M1Jet_idx", &M1Jet_idx);
	T_tmp->Branch("M1Jet_Pt", &M1Jet_Pt);
	T_tmp->Branch("M1Jet_Eta", &M1Jet_Eta);
	T_tmp->Branch("M1Jet_Phi", &M1Jet_Phi);
	T_tmp->Branch("M1Jet_btag", &M1Jet_btag);
	T_tmp->Branch("WJet1_idx", &WJet1_idx);
	T_tmp->Branch("WJet1_Pt", &WJet1_Pt);
	T_tmp->Branch("WJet1_Eta", &WJet1_Eta);
	T_tmp->Branch("WJet1_Phi", &WJet1_Phi);
	T_tmp->Branch("WJet1_btag", &WJet1_btag);
	T_tmp->Branch("WJet2_idx", &WJet2_idx);
	T_tmp->Branch("WJet2_Pt", &WJet2_Pt);
	T_tmp->Branch("WJet2_Eta", &WJet2_Eta);
	T_tmp->Branch("WJet2_Phi", &WJet2_Phi);
	T_tmp->Branch("WJet2_btag", &WJet2_btag);
	T_tmp->Branch("lep_idx", &lep_idx);
	T_tmp->Branch("lep_ID", &lep_ID);
	T_tmp->Branch("lep_Pt", &lep_Pt);
	T_tmp->Branch("lep_Eta", &lep_Eta);
	T_tmp->Branch("lep_Phi", &lep_Phi);
	TTree *TPerm_TT = 0;
	TTree *TPerm_train = 0, *TPerm_test = 0;
	if (is_signal) {
		TPerm_train = T_tmp;
		TPerm_test = T_tmp->CloneTree(0);
		TPerm_train->SetName("TPerm_train");
		TPerm_test->SetName("TPerm_test");
	} else {
		TPerm_TT = T_tmp;
		TPerm_TT->SetName("TPerm_TT");
	}

	int Nevt_tot = T->GetEntries();
	int Nevt_train = (int)(Nevt_tot * frac_train);

	int Nevt_recoable = 0;

	int Nperm_train = 0;
	int Nperm_test = 0;
	int Nperm_bkg = 0;

	TLorentzVector gen_bq, gen_lq, gen_Wq1, gen_Wq2, gen_lep;
	TLorentzVector reco_bJet, reco_M1Jet, reco_WJet1, reco_WJet2, reco_lep;

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
		bJet_Phi = -999;
		bJet_btag = -999;
		M1Jet_idx = -1;
		M1Jet_Pt = -999;
		M1Jet_Eta = -999;
		M1Jet_Phi = -999;
		M1Jet_btag = -999;
		WJet1_idx = -1;
		WJet1_Pt = -999;
		WJet1_Eta = -999;
		WJet1_Phi = -999;
		WJet1_btag = -999;
		WJet2_idx = -1;
		WJet2_Pt = -999;
		WJet2_Eta = -999;
		WJet2_Phi = -999;
		WJet2_btag = -999;
		lep_idx = -1;
		lep_ID = -999;
		lep_Pt = -999;
		lep_Eta = -999;
		lep_Phi = -999;

		bool is_train = false;
		if (is_signal && evt<Nevt_train) is_train = true;

		// Events that can't be recostructed are not saved in training tree, but are saved in testing tree and bkg tree with variables
		// set to null.
		if (jets_size < 4) {
			if (!is_train) {
				if (is_signal)  TPerm_test->Fill();
				else TPerm_TT->Fill();
			}
			continue;
		}

		Nevt_recoable += 1;

		// Set the 4-momenta of gen particles if processing signal sample
		if (is_signal) {
			gen_bq.SetPtEtaPhiM(Gen_Pt->at(Idx_bq), Gen_Eta->at(Idx_bq), Gen_Phi->at(Idx_bq), Gen_Mass->at(Idx_bq));
			gen_lq.SetPtEtaPhiM(Gen_Pt->at(Idx_lq), Gen_Eta->at(Idx_lq), Gen_Phi->at(Idx_lq), Gen_Mass->at(Idx_lq));
			gen_Wq1.SetPtEtaPhiM(Gen_Pt->at(Idx_Wq1), Gen_Eta->at(Idx_Wq1), Gen_Phi->at(Idx_Wq1), Gen_Mass->at(Idx_Wq1));
			gen_Wq2.SetPtEtaPhiM(Gen_Pt->at(Idx_Wq2), Gen_Eta->at(Idx_Wq2), Gen_Phi->at(Idx_Wq2), Gen_Mass->at(Idx_Wq2));
		}

		// Determine number of jets used to generate permutations
		int njet_ = 0;  // Number of significant jets
		if (jets_size <= NJet_sig) { njet_ = jets_size; }
		else { njet_ = NJet_sig; }

		NPerm = njet_ * (njet_-1) * (njet_-2)*(njet_-3)/2;
		idxPerm = 0;
		vector<IdxPerm_TThad> match_perms;  // Vector of truth matched permutations

		// Start b-jet loop
		for (int i=0; i<njet_; ++i) {
			// Start fcnc jet loop
			for (int j=0; j<njet_; ++j) {
				if (j == i) continue;
				// Start W jet 1 loop
				for (int k=0; k<njet_-1; ++k) {
					if (k == i || k == j) continue;
					// Start W jet 2 loop
					for (int l=k+1; l<njet_; ++l) {
						if (l == i || l == j) continue;

						// Decide if reco & gen particles are matched if processing signal sample
						// When producing bkg tree, variable `match` is not modified and stay at -1
						if (is_signal) {
							// Set 4-momenta of reco objects
							reco_bJet.SetPtEtaPhiE(Jet_Pt->at(i), Jet_Eta->at(i), Jet_Phi->at(i), Jet_Energy->at(i));
							reco_M1Jet.SetPtEtaPhiE(Jet_Pt->at(j), Jet_Eta->at(j), Jet_Phi->at(j), Jet_Energy->at(j));
							reco_WJet1.SetPtEtaPhiE(Jet_Pt->at(k), Jet_Eta->at(k), Jet_Phi->at(k), Jet_Energy->at(k));
							reco_WJet2.SetPtEtaPhiE(Jet_Pt->at(l), Jet_Eta->at(l), Jet_Phi->at(l), Jet_Energy->at(l));

							match = 0;
							bool W_matched = (reco_WJet1.DeltaR(gen_Wq1)<dR_cut && reco_WJet2.DeltaR(gen_Wq2)<dR_cut) ||
									(reco_WJet1.DeltaR(gen_Wq2)<dR_cut && reco_WJet2.DeltaR(gen_Wq1)<dR_cut);
							if (reco_bJet.DeltaR(gen_bq)<dR_cut && reco_M1Jet.DeltaR(gen_lq)<dR_cut && W_matched
									&& DiPho_leadGenMatchType==1 && DiPho_subleadGenMatchType==1)
								match = 1;
						}

						// If not producing training tree, fill the output tree
						// Training tree is not filled in this loop. For training tree, this loop only bookmarks the matched permutations
						// for later permutation sampling.
						if (!is_train) {
							bJet_idx = i;
							bJet_Pt = Jet_Pt->at(i);
							bJet_Eta = Jet_Eta->at(i);
							bJet_Phi = Jet_Phi->at(i);
							bJet_btag = Jet_probb->at(i)+Jet_probbb->at(i);
							M1Jet_idx = j;
							M1Jet_Pt = Jet_Pt->at(j);
							M1Jet_Eta = Jet_Eta->at(j);
							M1Jet_Phi = Jet_Phi->at(j);
							M1Jet_btag = Jet_probb->at(j)+Jet_probbb->at(j);
							WJet1_idx = k;
							WJet1_Pt = Jet_Pt->at(k);
							WJet1_Eta = Jet_Eta->at(k);
							WJet1_Phi = Jet_Phi->at(k);
							WJet1_btag = Jet_probb->at(k)+Jet_probbb->at(k);
							WJet2_idx = l;
							WJet2_Pt = Jet_Pt->at(l);
							WJet2_Eta = Jet_Eta->at(l);
							WJet2_Phi = Jet_Phi->at(l);
							WJet2_btag = Jet_probb->at(l)+Jet_probbb->at(l);

							if (is_signal) {
								TPerm_test->Fill();
								++Nperm_test;
							}
							else {
								TPerm_TT->Fill();
								++Nperm_bkg;
							}
						}

						// If produing training tree, bookmark the matched permutations.
						if (is_train && match)  match_perms.push_back(IdxPerm_TThad(i, j, k, l));

						++idxPerm;
					} // End W jet 2 loop
				} // End W jet 1 loop
			} // End fcnc jet loop
		} // End b-jet loop

		// If producing testing tree or bkg tree, the processing of this event is already completed. Go to next event.
		if (!is_train) continue;

		/* The remaining part of event loop is for training tree only */

		// If there is any matched permutations, randomly choose one of them to save
		vector<IdxPerm_TThad> train_perms;  // Vector of permutations to be saved in training tree
		if (match_perms.size() > 0) {
			int idxrnd_match = (int)rnd.Uniform(match_perms.size());
			train_perms.push_back( match_perms.at(idxrnd_match) );
		}

		/* Random sampling of permutations */
		// For different jet numbers, different number of permutations are sampled
		int NPerm = 0;
		switch (njet_) {
			case 4:
				NPerm = 6;
				break;
			case 5:
				NPerm = 12;
				break;
			case 6:
				NPerm = 18;
				break;
			case 7:
				NPerm = 21;
				break;
		}

		// Randomly choose `NPerm` permutations to save
		int idxPerm_sample = train_perms.size();
		while (idxPerm_sample < NPerm) {  // Leave the loop only after enough permutations are saved
			// Randomly generate a permutation
			IdxPerm_TThad rndPerm = IdxPerm_TThad::GetRandom(njet_);
			// If the permutation is already saved, generate a new one
			if (rndPerm.IncludedBy(train_perms))  continue;
			// If the permutation is new, save it
			train_perms.push_back(rndPerm);
			++idxPerm_sample;
		}

		// After choosing which permutations to save, fill them to training tree
		idxPerm = 0;
		// Start train perm loop
		for (int p=0; p<train_perms.size(); ++p) {
			int i = train_perms.at(p).bJet;
			int j = train_perms.at(p).M1Jet;
			int k = train_perms.at(p).WJet1;
			int l = train_perms.at(p).WJet2;
			reco_bJet.SetPtEtaPhiE(Jet_Pt->at(i), Jet_Eta->at(i), Jet_Phi->at(i), Jet_Energy->at(i));
			reco_M1Jet.SetPtEtaPhiE(Jet_Pt->at(j), Jet_Eta->at(j), Jet_Phi->at(j), Jet_Energy->at(j));
			reco_WJet1.SetPtEtaPhiE(Jet_Pt->at(k), Jet_Eta->at(k), Jet_Phi->at(k), Jet_Energy->at(k));
			reco_WJet2.SetPtEtaPhiE(Jet_Pt->at(l), Jet_Eta->at(l), Jet_Phi->at(l), Jet_Energy->at(l));

			match = 0;
			bool W_matched = (reco_WJet1.DeltaR(gen_Wq1)<dR_cut && reco_WJet2.DeltaR(gen_Wq2)<dR_cut) ||
					(reco_WJet1.DeltaR(gen_Wq2)<dR_cut && reco_WJet2.DeltaR(gen_Wq1)<dR_cut);
			if (reco_bJet.DeltaR(gen_bq)<dR_cut && reco_M1Jet.DeltaR(gen_lq)<dR_cut && W_matched
					&& DiPho_leadGenMatchType==1 && DiPho_subleadGenMatchType==1)
				match = 1;

			bJet_idx = i;
			bJet_Pt = Jet_Pt->at(i);
			bJet_Eta = Jet_Eta->at(i);
			bJet_Phi = Jet_Phi->at(i);
			bJet_btag = Jet_probb->at(i)+Jet_probbb->at(i);
			M1Jet_idx = j;
			M1Jet_Pt = Jet_Pt->at(j);
			M1Jet_Eta = Jet_Eta->at(j);
			M1Jet_Phi = Jet_Phi->at(j);
			M1Jet_btag = Jet_probb->at(j)+Jet_probbb->at(j);
			WJet1_idx = k;
			WJet1_Pt = Jet_Pt->at(k);
			WJet1_Eta = Jet_Eta->at(k);
			WJet1_Phi = Jet_Phi->at(k);
			WJet1_btag = Jet_probb->at(k)+Jet_probbb->at(k);
			WJet2_idx = l;
			WJet2_Pt = Jet_Pt->at(l);
			WJet2_Eta = Jet_Eta->at(l);
			WJet2_Phi = Jet_Phi->at(l);
			WJet2_btag = Jet_probb->at(l)+Jet_probbb->at(l);

			TPerm_train->Fill();  ++Nperm_train;
			++idxPerm;
		} // End of train perm loop
	} // End event loop

	/* LESSON: Don't create output TFile after creating the TTree that you want to save to that file! Somehow it creates error during execution.
	 * Create TFile BEFORE create TTree.
	// Create output file
	TFile *fout = new TFile(fout_name, "update");
	*/

	// Save results
	cout << "[INFO] Saving file: " << fout_name << endl;
	if (is_signal) {
		fout->WriteTObject(TPerm_train);
		fout->WriteTObject(TPerm_test);
	} else fout->WriteTObject(TPerm_TT);

	fout->Close();
	fin->Close();

	// Print summary
	cout << "\n[Summary]\n";
	cout << "# of input events: " << Nevt_tot << endl;
	cout << "# of recoable events: " << Nevt_recoable << endl;
	if (is_signal) {
		cout << "# of training permutations: " << Nperm_train << endl;
		cout << "# of test permutations: " << Nperm_test << endl;
	} else cout << "# of permutations: " << Nperm_bkg << endl;

	return 0;
}
