/*
 * Generate event permutations.
 *
 * Usage:
 *   MVAreco_GenPerm <input.root> <tree name> <output.root> <n-tuple type> <channel> <year of dataset> <b-tagging WP>
 *     n-tuple type: TT, ST or bkg
 *     channel     : had or lep
 *     year of dataset: 2016, 2017 or 2018
 *     b-tagging WP: loose, medium or tight
 *
 * Input file contains a TTree of events passing the hadronic or leptonic preselection.
 * To run signal MC, select the options consistent with the sample. E.g., for TT signal sample with hadronic decay,
 * select <n-tuple type>=TT and <channel>=had.
 * To run bkg MC or data, select <n-tuple type>=bkg and the channel corresponds to the sample.
 *
 * If n-tuple type is TT or ST, the output generated contains 2 trees, T_train and T_test, for MVA training and testing
 * purposes. The truth matching result of each permutaion is saved as an int (1: matched; 0: not matched; -1: null).
 * If n-tuple type is bkg, the output generated contains 2 trees, T_TT and T_ST, with permutations of TT and ST signal
 * reconstruction, respectively.
 */

#include "include/utility.h"

#include "TString.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TLorentzVector.h"

#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;

int main(int argc, char **argv)
{
	// Get command line arguments
	TString fin_name = argv[1];
	TString inTree_name = argv[2];
	TString fout_name = argv[3];
	TString nt_type = argv[4];
	TString channel = argv[5];
	TString year = argv[6];
	TString wp = argv[7];
	if (nt_type!="TT" && nt_type!="ST" && nt_type!="bkg") {
		cout << "[ERROR] Invalid n-tuple type\n" << endl;
		exit(1);
	}
	if (channel!="had" && channel!="lep") {
		cout << "[ERROR] Invalid channel\n" << endl;
		exit(1);
	}
	// Get b-tag cut value for the specified year & WP
	float cut_btag = GetBtagCut(year, wp);
	// Set delta_R cut value (used in gen-reco matching)
	float cut_dR = 0.4;

	// Flags that determine the workflow
	bool run_TThad = false;
	bool run_SThad = false;
	bool run_TTlep = false;
	bool run_STlep = false;
	bool is_signal = false;
	// Set flags
	if (channel == "had") {
		if (nt_type == "TT") { run_TThad = true; is_signal = true; }
		else if (nt_type == "ST") { run_SThad = true; is_signal = true; }
		else if (nt_type == "bkg") { run_TThad = true;  run_SThad = true; }
	} else if (channel == "lep") {
		if (nt_type == "TT") { run_TTlep = true; is_signal = true; }
		else if (nt_type == "ST") { run_STlep = true; is_signal = true; }
		else if (nt_type == "bkg") { run_TTlep = true;  run_STlep = true; }
	}

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
	int Elec_Size = 0;
	vector<int>   *Elec_Charge = 0;
	vector<float> *Elec_Pt = 0;
	vector<float> *Elec_Eta = 0;
	vector<float> *Elec_Phi = 0;
	vector<float> *Elec_Energy = 0;
	int Muon_Size = 0;
	vector<int>   *Muon_Charge = 0;
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
	int Gen_size = 0;
	vector<float> *Gen_Pt = 0;
	vector<float> *Gen_Eta = 0;
	vector<float> *Gen_Phi = 0;
	vector<float> *Gen_Mass = 0;
	vector<int>   *Gen_PdgID = 0;
	vector<bool>  *Gen_fromHardProcessFinalState = 0;
	vector<int>   *Gen_MomPdgID = 0;

	// Read input file
	cout << "[INFO] Start processing: " << fin_name << endl;
	TChain *inTree = new TChain(inTree_name);
	inTree->Add(fin_name);
	// Set branch status
	inTree->SetBranchStatus("*", 0);
	inTree->SetBranchStatus("EvtInfo.genweight", 1);
	inTree->SetBranchStatus("DiPhoInfo.leadPt", 1);
	inTree->SetBranchStatus("DiPhoInfo.leadEta", 1);
	inTree->SetBranchStatus("DiPhoInfo.leadPhi", 1);
	inTree->SetBranchStatus("DiPhoInfo.leadIDMVA", 1);
	inTree->SetBranchStatus("DiPhoInfo.leadGenMatchType", 1);
	inTree->SetBranchStatus("DiPhoInfo.subleadPt", 1);
	inTree->SetBranchStatus("DiPhoInfo.subleadEta", 1);
	inTree->SetBranchStatus("DiPhoInfo.subleadPhi", 1);
	inTree->SetBranchStatus("DiPhoInfo.subleadIDMVA", 1);
	inTree->SetBranchStatus("DiPhoInfo.subleadGenMatchType", 1);
	inTree->SetBranchStatus("ElecInfo.Size", 1);
	inTree->SetBranchStatus("ElecInfo.Charge", 1);
	inTree->SetBranchStatus("ElecInfo.Pt", 1);
	inTree->SetBranchStatus("ElecInfo.Eta", 1);
	inTree->SetBranchStatus("ElecInfo.Phi", 1);
	inTree->SetBranchStatus("ElecInfo.Energy", 1);
	inTree->SetBranchStatus("MuonInfo.Size", 1);
	inTree->SetBranchStatus("MuonInfo.Charge", 1);
	inTree->SetBranchStatus("MuonInfo.Pt", 1);
	inTree->SetBranchStatus("MuonInfo.Eta", 1);
	inTree->SetBranchStatus("MuonInfo.Phi", 1);
	inTree->SetBranchStatus("MuonInfo.Energy", 1);
	inTree->SetBranchStatus("jets_size", 1);
	inTree->SetBranchStatus("JetInfo.Pt", 1);
	inTree->SetBranchStatus("JetInfo.Eta", 1);
	inTree->SetBranchStatus("JetInfo.Phi", 1);
	inTree->SetBranchStatus("JetInfo.Energy", 1);
	inTree->SetBranchStatus("JetInfo.pfDeepCSVJetTags_probb", 1);
	inTree->SetBranchStatus("JetInfo.pfDeepCSVJetTags_probbb", 1);
	if (is_signal) {
		inTree->SetBranchStatus("GenPartInfo.size", 1);
		inTree->SetBranchStatus("GenPartInfo.Pt", 1);
		inTree->SetBranchStatus("GenPartInfo.Eta", 1);
		inTree->SetBranchStatus("GenPartInfo.Phi", 1);
		inTree->SetBranchStatus("GenPartInfo.Mass", 1);
		inTree->SetBranchStatus("GenPartInfo.PdgID", 1);
		inTree->SetBranchStatus("GenPartInfo.fromHardProcessFinalState", 1);
		inTree->SetBranchStatus("GenPartInfo.MomPdgID", 1);
	}
	// Set branch address
	inTree->SetBranchAddress("EvtInfo.genweight", &Evt_genweight);
	inTree->SetBranchAddress("DiPhoInfo.leadPt", &DiPho_leadPt);
	inTree->SetBranchAddress("DiPhoInfo.leadEta", &DiPho_leadEta);
	inTree->SetBranchAddress("DiPhoInfo.leadPhi", &DiPho_leadPhi);
	inTree->SetBranchAddress("DiPhoInfo.leadIDMVA", &DiPho_leadIDMVA);
	inTree->SetBranchAddress("DiPhoInfo.leadGenMatchType", &DiPho_leadGenMatchType);
	inTree->SetBranchAddress("DiPhoInfo.subleadPt", &DiPho_subleadPt);
	inTree->SetBranchAddress("DiPhoInfo.subleadEta", &DiPho_subleadEta);
	inTree->SetBranchAddress("DiPhoInfo.subleadPhi", &DiPho_subleadPhi);
	inTree->SetBranchAddress("DiPhoInfo.subleadIDMVA", &DiPho_subleadIDMVA);
	inTree->SetBranchAddress("DiPhoInfo.subleadGenMatchType", &DiPho_subleadGenMatchType);
	inTree->SetBranchAddress("ElecInfo.Size", &Elec_Size);
	inTree->SetBranchAddress("ElecInfo.Charge", &Elec_Charge);
	inTree->SetBranchAddress("ElecInfo.Pt", &Elec_Pt);
	inTree->SetBranchAddress("ElecInfo.Eta", &Elec_Eta);
	inTree->SetBranchAddress("ElecInfo.Phi", &Elec_Phi);
	inTree->SetBranchAddress("ElecInfo.Energy", &Elec_Energy);
	inTree->SetBranchAddress("MuonInfo.Size", &Muon_Size);
	inTree->SetBranchAddress("MuonInfo.Charge", &Muon_Charge);
	inTree->SetBranchAddress("MuonInfo.Pt", &Muon_Pt);
	inTree->SetBranchAddress("MuonInfo.Eta", &Muon_Eta);
	inTree->SetBranchAddress("MuonInfo.Phi", &Muon_Phi);
	inTree->SetBranchAddress("MuonInfo.Energy", &Muon_Energy);
	inTree->SetBranchAddress("jets_size", &jets_size);
	inTree->SetBranchAddress("JetInfo.Pt", &Jet_Pt);
	inTree->SetBranchAddress("JetInfo.Eta", &Jet_Eta);
	inTree->SetBranchAddress("JetInfo.Phi", &Jet_Phi);
	inTree->SetBranchAddress("JetInfo.Energy", &Jet_Energy);
	inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probb", &Jet_probb);
	inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probbb", &Jet_probbb);
	if (is_signal) {
		inTree->SetBranchAddress("GenPartInfo.size", &Gen_size);
		inTree->SetBranchAddress("GenPartInfo.Pt", &Gen_Pt);
		inTree->SetBranchAddress("GenPartInfo.Eta", &Gen_Eta);
		inTree->SetBranchAddress("GenPartInfo.Phi", &Gen_Phi);
		inTree->SetBranchAddress("GenPartInfo.Mass", &Gen_Mass);
		inTree->SetBranchAddress("GenPartInfo.PdgID", &Gen_PdgID);
		inTree->SetBranchAddress("GenPartInfo.fromHardProcessFinalState", &Gen_fromHardProcessFinalState);
		inTree->SetBranchAddress("GenPartInfo.MomPdgID", &Gen_MomPdgID);
	}

	// Output tree variables
	int Evt_idx = 0;
	int NPerm = 0;
	int idxPerm = 0;
	int match = 0;
	// b-jet
	int bJet_idx = 0;
	float bJet_Pt = 0;
	float bJet_Eta = 0;
	float bJet_Phi = 0;
	float bJet_btag = 0;
	// M1 jet (TT)
	int M1Jet_idx = 0;
	float M1Jet_Pt = 0;
	float M1Jet_Eta = 0;
	float M1Jet_Phi = 0;
	float M1Jet_btag = 0;
	// WJets (had)
	int WJet1_idx = 0;
	float WJet1_Pt = 0;
	float WJet1_Eta = 0;
	float WJet1_Phi = 0;
	float WJet1_btag = 0;
	int WJet2_idx = 0;
	float WJet2_Pt = 0;
	float WJet2_Eta = 0;
	float WJet2_Phi = 0;
	float WJet2_btag = 0;
	// Lepton (lep)
	int lep_idx = 0;
	int lep_ID = 0;
	float lep_Pt = 0;
	float lep_Eta = 0;
	float lep_Phi = 0;

	// Create output file
	TFile *fout = new TFile(fout_name, "recreate");
	TTree *T_train = 0;
	TTree *T_test = 0;
	TTree *T_TT = 0;
	TTree *T_ST = 0;
	if (is_signal) T_train = new TTree("T_train", "");
	else T_TT = new TTree("T_TT", "");

	// Set output branches
	TTree *T_ptr = 0;
	if (is_signal) T_ptr = T_train;
	else T_ptr = T_TT;
	if (channel == "had") {
		T_ptr->Branch("Evt_idx", &Evt_idx);
		T_ptr->Branch("Evt_genweight", &Evt_genweight);
		T_ptr->Branch("NPerm", &NPerm);
		T_ptr->Branch("idxPerm", &idxPerm);
		T_ptr->Branch("match", &match);
		T_ptr->Branch("LeadPho_Pt", &DiPho_leadPt);
		T_ptr->Branch("LeadPho_Eta", &DiPho_leadEta);
		T_ptr->Branch("LeadPho_Phi", &DiPho_leadPhi);
		T_ptr->Branch("LeadPho_IDMVA", &DiPho_leadIDMVA);
		T_ptr->Branch("SubleadPho_Pt", &DiPho_subleadPt);
		T_ptr->Branch("SubleadPho_Eta", &DiPho_subleadEta);
		T_ptr->Branch("SubleadPho_Phi", &DiPho_subleadPhi);
		T_ptr->Branch("SubleadPho_IDMVA", &DiPho_subleadIDMVA);
		T_ptr->Branch("bJet_idx", &bJet_idx);
		T_ptr->Branch("bJet_Pt", &bJet_Pt);
		T_ptr->Branch("bJet_Eta", &bJet_Eta);
		T_ptr->Branch("bJet_Phi", &bJet_Phi);
		T_ptr->Branch("bJet_btag", &bJet_btag);
		T_ptr->Branch("M1Jet_idx", &M1Jet_idx);
		T_ptr->Branch("M1Jet_Pt", &M1Jet_Pt);
		T_ptr->Branch("M1Jet_Eta", &M1Jet_Eta);
		T_ptr->Branch("M1Jet_Phi", &M1Jet_Phi);
		T_ptr->Branch("M1Jet_btag", &M1Jet_btag);
		T_ptr->Branch("WJet1_idx", &WJet1_idx);
		T_ptr->Branch("WJet1_Pt", &WJet1_Pt);
		T_ptr->Branch("WJet1_Eta", &WJet1_Eta);
		T_ptr->Branch("WJet1_Phi", &WJet1_Phi);
		T_ptr->Branch("WJet1_btag", &WJet1_btag);
		T_ptr->Branch("WJet2_idx", &WJet2_idx);
		T_ptr->Branch("WJet2_Pt", &WJet2_Pt);
		T_ptr->Branch("WJet2_Eta", &WJet2_Eta);
		T_ptr->Branch("WJet2_Phi", &WJet2_Phi);
		T_ptr->Branch("WJet2_btag", &WJet2_btag);
	}
	else if (channel == "lep") {
		T_ptr->Branch("Evt_idx", &Evt_idx);
		T_ptr->Branch("Evt_genweight", &Evt_genweight);
		T_ptr->Branch("NPerm", &NPerm);
		T_ptr->Branch("idxPerm", &idxPerm);
		T_ptr->Branch("match", &match);
		T_ptr->Branch("LeadPho_Pt", &DiPho_leadPt);
		T_ptr->Branch("LeadPho_Eta", &DiPho_leadEta);
		T_ptr->Branch("LeadPho_Phi", &DiPho_leadPhi);
		T_ptr->Branch("LeadPho_IDMVA", &DiPho_leadIDMVA);
		T_ptr->Branch("SubleadPho_Pt", &DiPho_subleadPt);
		T_ptr->Branch("SubleadPho_Eta", &DiPho_subleadEta);
		T_ptr->Branch("SubleadPho_Phi", &DiPho_subleadPhi);
		T_ptr->Branch("SubleadPho_IDMVA", &DiPho_subleadIDMVA);
		T_ptr->Branch("bJet_idx", &bJet_idx);
		T_ptr->Branch("bJet_Pt", &bJet_Pt);
		T_ptr->Branch("bJet_Eta", &bJet_Eta);
		T_ptr->Branch("bJet_Phi", &bJet_Phi);
		T_ptr->Branch("bJet_btag", &bJet_btag);
		T_ptr->Branch("M1Jet_idx", &M1Jet_idx);
		T_ptr->Branch("M1Jet_Pt", &M1Jet_Pt);
		T_ptr->Branch("M1Jet_Eta", &M1Jet_Eta);
		T_ptr->Branch("M1Jet_Phi", &M1Jet_Phi);
		T_ptr->Branch("M1Jet_btag", &M1Jet_btag);
		T_ptr->Branch("lep_idx", &lep_idx);
		T_ptr->Branch("lep_ID", &lep_ID);
		T_ptr->Branch("lep_Pt", &lep_Pt);
		T_ptr->Branch("lep_Eta", &lep_Eta);
		T_ptr->Branch("lep_Phi", &lep_Phi);
	}
	if (is_signal) {
		T_test = T_train->CloneTree(0);
		T_test->SetName("T_test");
	} else {
		T_ST = T_TT->CloneTree(0);
		T_ST->SetName("T_ST");
	}

	// 4-momenta of reco objects
	TLorentzVector reco_bJet, reco_M1Jet, reco_WJet1, reco_WJet2, reco_lep;
	// 4-momenta of gen objects
	TLorentzVector gen_bJet, gen_M1Jet, gen_WJets[2], gen_lep;
	int gen_lep_ID = 0;

	// Vectors for b-jets and non-b-jets
	int NbJets = 0;
	int NnbJets = 0;
	vector<int> idx_bJets;
	vector<int> idx_nbJets;
	// Vectors for leptons
	int Nleps = 0;
	vector<int>   Lep_Type;
	vector<int>   Lep_Charge;
	vector<float> Lep_Pt;
	vector<float> Lep_Eta;
	vector<float> Lep_Phi;
	vector<float> Lep_Energy;

	match = -1;

	// start input tree loop
	int Nevt = inTree->GetEntries();
	int Nevt_TT = 0;
	int Nevt_ST = 0;
	int Nperm_TT = 0;
	int Nperm_ST = 0;
	for (int evt=0; evt<Nevt; ++evt)
	{
		inTree->GetEntry(evt);

		// Record the indices of reco b-jets/non-b-jets
		idx_bJets.clear();
		idx_nbJets.clear();
		for (int i=0; i<jets_size; ++i)
			if (Jet_probb->at(i)+Jet_probbb->at(i) > cut_btag) idx_bJets.push_back(i);
			else idx_nbJets.push_back(i);
		NbJets = idx_bJets.size();
		NnbJets = idx_nbJets.size();

		// To make the codes for leptonic channel easier, joint the vectors of electrons and muons
		if (channel == "lep") {
			Nleps = Elec_Size + Muon_Size;
			Lep_Type.resize(Nleps);
			Lep_Charge.resize(Nleps);
			Lep_Pt.resize(Nleps);
			Lep_Eta.resize(Nleps);
			Lep_Phi.resize(Nleps);
			Lep_Energy.resize(Nleps);
			for (int i=0; i<Nleps; ++i) {
				if (i<Elec_Size) {
					Lep_Type[i] = 0;
					Lep_Charge[i] = Elec_Charge->at(i);
					Lep_Pt[i] = Elec_Pt->at(i);
					Lep_Eta[i] = Elec_Eta->at(i);
					Lep_Phi[i] = Elec_Phi->at(i);
					Lep_Energy[i] = Elec_Energy->at(i);
				} else {
					int j = i - Elec_Size;
					Lep_Type[i] = 1;
					Lep_Charge[i] = Muon_Charge->at(j);
					Lep_Pt[i] = Muon_Pt->at(j);
					Lep_Eta[i] = Muon_Eta->at(j);
					Lep_Phi[i] = Muon_Phi->at(j);
					Lep_Energy[i] = Muon_Energy->at(j);
				}
			}
		}

		Evt_idx = evt;

		// If run_TThad
		if (run_TThad && NbJets>=1 && NnbJets>=3)
		{
			if (is_signal) { // start if is_signal
				bool after_W = false;
				int i_WJet = 0;
				for (int i=0; i<Gen_size; ++i) { // start gen particle loop
					// The W from top
					if ( fabs(Gen_PdgID->at(i))==24 && fabs(Gen_MomPdgID->at(i))==6 ) after_W = true;
					// The b-jet from M2
					else if ( fabs(Gen_PdgID->at(i))==5 && fabs(Gen_MomPdgID->at(i))==6 )
						gen_bJet.SetPtEtaPhiM(Gen_Pt->at(i), Gen_Eta->at(i), Gen_Phi->at(i), Gen_Mass->at(i));
					// The jet from M1
					else if ( (fabs(Gen_PdgID->at(i))==2 || fabs(Gen_PdgID->at(i))==4)
							&& fabs(Gen_MomPdgID->at(i))==6 )
						gen_M1Jet.SetPtEtaPhiM(Gen_Pt->at(i), Gen_Eta->at(i), Gen_Phi->at(i), Gen_Mass->at(i));
					// The 2 jets from W
					else if ( (fabs(Gen_PdgID->at(i))>=1 && fabs(Gen_PdgID->at(i))<=4)
							&& fabs(Gen_MomPdgID->at(i))==24 && after_W) {
						gen_WJets[i_WJet].SetPtEtaPhiM(Gen_Pt->at(i), Gen_Eta->at(i), Gen_Phi->at(i), Gen_Mass->at(i));
						++i_WJet;
					}
				} // end gen particle loop
			} // end if is_signal

			++Nevt_TT;
			NPerm = NbJets * NnbJets * (NnbJets-1)*(NnbJets-2)/2;
			idxPerm = 0;
			for (int i=0; i<NbJets; ++i) { // start b-jet loop
				for (int j=0; j<NnbJets; ++j) { // start M1 jet loop
					for (int k=0; k<NnbJets-1; ++k) { // start W jet1 loop
						if (k == j) continue;
						for (int l=k+1; l<NnbJets; ++l) { // start W jet2 loop
							if (l == j) continue;

							bJet_idx = idx_bJets[i];
							bJet_Pt = Jet_Pt->at(bJet_idx);
							bJet_Eta = Jet_Eta->at(bJet_idx);
							bJet_Phi = Jet_Phi->at(bJet_idx);
							bJet_btag = Jet_probb->at(bJet_idx)+Jet_probbb->at(bJet_idx);
							M1Jet_idx = idx_nbJets[j];
							M1Jet_Pt = Jet_Pt->at(M1Jet_idx);
							M1Jet_Eta = Jet_Eta->at(M1Jet_idx);
							M1Jet_Phi = Jet_Phi->at(M1Jet_idx);
							M1Jet_btag = Jet_probb->at(M1Jet_idx)+Jet_probbb->at(M1Jet_idx);
							WJet1_idx = idx_nbJets[k];
							WJet1_Pt = Jet_Pt->at(WJet1_idx);
							WJet1_Eta = Jet_Eta->at(WJet1_idx);
							WJet1_Phi = Jet_Phi->at(WJet1_idx);
							WJet1_btag = Jet_probb->at(WJet1_idx)+Jet_probbb->at(WJet1_idx);
							WJet2_idx = idx_nbJets[l];
							WJet2_Pt = Jet_Pt->at(WJet2_idx);
							WJet2_Eta = Jet_Eta->at(WJet2_idx);
							WJet2_Phi = Jet_Phi->at(WJet2_idx);
							WJet2_btag = Jet_probb->at(WJet2_idx)+Jet_probbb->at(WJet2_idx);

							if (is_signal) { // if is_signal
								reco_bJet.SetPtEtaPhiE(bJet_Pt, bJet_Eta, bJet_Phi, Jet_Energy->at(bJet_idx));
								reco_M1Jet.SetPtEtaPhiE(M1Jet_Pt, M1Jet_Eta, M1Jet_Phi, Jet_Energy->at(M1Jet_idx));
								reco_WJet1.SetPtEtaPhiE(WJet1_Pt, WJet1_Eta, WJet1_Phi, Jet_Energy->at(WJet1_idx));
								reco_WJet2.SetPtEtaPhiE(WJet2_Pt, WJet2_Eta, WJet2_Phi, Jet_Energy->at(WJet2_idx));
								// Test if the event is truch matched
								bool WJets_match = (reco_WJet1.DeltaR(gen_WJets[0])<cut_dR && reco_WJet2.DeltaR(gen_WJets[1])<cut_dR)
										|| (reco_WJet1.DeltaR(gen_WJets[1])<cut_dR && reco_WJet2.DeltaR(gen_WJets[0])<cut_dR);
								if (DiPho_leadGenMatchType==1 && DiPho_subleadGenMatchType==1 &&
										reco_bJet.DeltaR(gen_bJet)<cut_dR && reco_M1Jet.DeltaR(gen_M1Jet)<cut_dR && WJets_match)
									match = 1;
								else match = 0;
								// Fill output tree
								if (evt < Nevt/2) T_train->Fill();
								else T_test->Fill();
							} else
								T_TT->Fill(); // end if is_signal

							++idxPerm;
							++Nperm_TT;
						} // end W jet2 loop
					} // end W jet1 loop
				} // end M1 jet loop
			} // end b-jet loop
		} // end if run_TThad

		// If run_SThad
		if (run_SThad && NbJets>=1 && NnbJets>=2)
		{
			if (is_signal) { // start if is_signal
				bool after_W = false;
				int i_WJet = 0;
				for (int i=0; i<Gen_size; ++i) { // start gen particle loop
					// The W from top
					if ( fabs(Gen_PdgID->at(i))==24 && fabs(Gen_MomPdgID->at(i))==6 ) after_W = true;
					// The b-jet from M1
					else if ( fabs(Gen_PdgID->at(i))==5 && fabs(Gen_MomPdgID->at(i))==6 )
						gen_bJet.SetPtEtaPhiM(Gen_Pt->at(i), Gen_Eta->at(i), Gen_Phi->at(i), Gen_Mass->at(i));
					// The 2 jets from W
					else if ( (fabs(Gen_PdgID->at(i))>=1 && fabs(Gen_PdgID->at(i))<=4)
							&& fabs(Gen_MomPdgID->at(i))==24 && after_W) {
						gen_WJets[i_WJet].SetPtEtaPhiM(Gen_Pt->at(i), Gen_Eta->at(i), Gen_Phi->at(i), Gen_Mass->at(i));
						++i_WJet;
					}
				} // end gen particle loop
			} // end if is_signal

			++Nevt_ST;
			NPerm = NbJets * NnbJets*(NnbJets-1)/2;
			idxPerm = 0;
			M1Jet_idx = -1;
			M1Jet_Pt = -999;
			M1Jet_Eta = -999;
			M1Jet_Phi = -999;
			M1Jet_btag = -999;
			for (int i=0; i<NbJets; ++i) { // start b-jet loop
				for (int j=0; j<NnbJets-1; ++j) { // start W jet1 loop
					for (int k=j+1; k<NnbJets; ++k) { // start W jet2 loop

						bJet_idx = idx_bJets[i];
						bJet_Pt = Jet_Pt->at(bJet_idx);
						bJet_Eta = Jet_Eta->at(bJet_idx);
						bJet_Phi = Jet_Phi->at(bJet_idx);
						bJet_btag = Jet_probb->at(bJet_idx)+Jet_probbb->at(bJet_idx);
						WJet1_idx = idx_nbJets[j];
						WJet1_Pt = Jet_Pt->at(WJet1_idx);
						WJet1_Eta = Jet_Eta->at(WJet1_idx);
						WJet1_Phi = Jet_Phi->at(WJet1_idx);
						WJet1_btag = Jet_probb->at(WJet1_idx)+Jet_probbb->at(WJet1_idx);
						WJet2_idx = idx_nbJets[k];
						WJet2_Pt = Jet_Pt->at(WJet2_idx);
						WJet2_Eta = Jet_Eta->at(WJet2_idx);
						WJet2_Phi = Jet_Phi->at(WJet2_idx);
						WJet2_btag = Jet_probb->at(WJet2_idx)+Jet_probbb->at(WJet2_idx);

						if (is_signal) { // if is_signal
							reco_bJet.SetPtEtaPhiE(bJet_Pt, bJet_Eta, bJet_Phi, Jet_Energy->at(bJet_idx));
							reco_WJet1.SetPtEtaPhiE(WJet1_Pt, WJet1_Eta, WJet1_Phi, Jet_Energy->at(WJet1_idx));
							reco_WJet2.SetPtEtaPhiE(WJet2_Pt, WJet2_Eta, WJet2_Phi, Jet_Energy->at(WJet2_idx));
							// Test if the event is truch matched
							bool WJets_match = (reco_WJet1.DeltaR(gen_WJets[0])<cut_dR && reco_WJet2.DeltaR(gen_WJets[1])<cut_dR)
									|| (reco_WJet1.DeltaR(gen_WJets[1])<cut_dR && reco_WJet2.DeltaR(gen_WJets[0])<cut_dR);
							if (DiPho_leadGenMatchType==1 && DiPho_subleadGenMatchType==1 &&
									reco_bJet.DeltaR(gen_bJet)<cut_dR && WJets_match)
								match = 1;
							else match = 0;
							// Fill output tree
							if (evt < Nevt/2) T_train->Fill();
							else T_test->Fill();
						} else
							T_ST->Fill(); // end if is_signal

						++idxPerm;
						++Nperm_ST;
					} // end W jet2 loop
				} // end W jet1 loop
			} // end b-jet loop
		} // End if run_SThad

		// If run_TTlep
		if (run_TTlep && NbJets>=1 && NnbJets>=1 && Nleps>=1)
		{
			if (is_signal) { // start if is_signal
				for (int i=0; i<Gen_size; ++i) { // start gen particle loop
					// The b-jet from M2
					if ( fabs(Gen_PdgID->at(i))==5 && fabs(Gen_MomPdgID->at(i))==6 )
						gen_bJet.SetPtEtaPhiM(Gen_Pt->at(i), Gen_Eta->at(i), Gen_Phi->at(i), Gen_Mass->at(i));
					// The jet from M1
					else if ( (fabs(Gen_PdgID->at(i))==2 || fabs(Gen_PdgID->at(i))==4)
							&& fabs(Gen_MomPdgID->at(i))==6 )
						gen_M1Jet.SetPtEtaPhiM(Gen_Pt->at(i), Gen_Eta->at(i), Gen_Phi->at(i), Gen_Mass->at(i));
					// The lepton from W
					else if ( (fabs(Gen_PdgID->at(i))==11 || fabs(Gen_PdgID->at(i))==13)
							&& fabs(Gen_MomPdgID->at(i))==24 && Gen_fromHardProcessFinalState->at(i) ) {
						gen_lep.SetPtEtaPhiM(Gen_Pt->at(i), Gen_Eta->at(i), Gen_Phi->at(i), Gen_Mass->at(i));
						gen_lep_ID = Gen_PdgID->at(i);
					}
				} // end gen particle loop
			} // end if is_signal

			++Nevt_TT;
			NPerm = NbJets * NnbJets * Nleps;
			idxPerm = 0;
			for (int i=0; i<NbJets; ++i) { // start b-jet loop
				for (int j=0; j<NnbJets; ++j) { // start M1 jet loop
					for (int k=0; k<Nleps; ++k) { // start lepton loop

						bJet_idx = idx_bJets[i];
						bJet_Pt = Jet_Pt->at(bJet_idx);
						bJet_Eta = Jet_Eta->at(bJet_idx);
						bJet_Phi = Jet_Phi->at(bJet_idx);
						bJet_btag = Jet_probb->at(bJet_idx)+Jet_probbb->at(bJet_idx);
						M1Jet_idx = idx_nbJets[j];
						M1Jet_Pt = Jet_Pt->at(M1Jet_idx);
						M1Jet_Eta = Jet_Eta->at(M1Jet_idx);
						M1Jet_Phi = Jet_Phi->at(M1Jet_idx);
						M1Jet_btag = Jet_probb->at(M1Jet_idx)+Jet_probbb->at(M1Jet_idx);
						// Set lep_idx, lep_ID
						lep_idx = k;
						if (Lep_Type[k] == 0) {
							if (Lep_Charge[k] > 0) lep_ID = -11;
							else lep_ID = 11;
						} else {
							lep_idx -= Elec_Size;
							if (Lep_Charge[k] > 0) lep_ID = -13;
							else lep_ID = 13;
						}
						lep_Pt = Lep_Pt[k];
						lep_Eta = Lep_Eta[k];
						lep_Phi = Lep_Phi[k];

						if (is_signal) { // if is_signal
							reco_bJet.SetPtEtaPhiE(bJet_Pt, bJet_Eta, bJet_Phi, Jet_Energy->at(bJet_idx));
							reco_M1Jet.SetPtEtaPhiE(M1Jet_Pt, M1Jet_Eta, M1Jet_Phi, Jet_Energy->at(M1Jet_idx));
							reco_lep.SetPtEtaPhiE(lep_Pt, lep_Eta, lep_Phi, Lep_Energy[k]);
							// Test if the event is truch matched
							if (DiPho_leadGenMatchType==1 && DiPho_subleadGenMatchType==1 &&
									reco_bJet.DeltaR(gen_bJet)<cut_dR && reco_M1Jet.DeltaR(gen_M1Jet)<cut_dR && reco_lep.DeltaR(gen_lep)<cut_dR && lep_ID==gen_lep_ID)
								match = 1;
							else match = 0;
							// Fill output tree
							if (evt < Nevt/2) T_train->Fill();
							else T_test->Fill();
						} else
							T_TT->Fill(); // end if is_signal

						++idxPerm;
						++Nperm_TT;
					} // end lepton loop
				} // end M1 jet loop
			} // end b-jet loop
		} // End if run_TTlep

		// If run_STlep
		if (run_STlep && NbJets>=1 && Nleps>=1)
		{
			if (is_signal) { // start if is_signal
				for (int i=0; i<Gen_size; ++i) { // start gen particle loop
					// The b-jet from M1
					if ( fabs(Gen_PdgID->at(i))==5 && fabs(Gen_MomPdgID->at(i))==6 )
						gen_bJet.SetPtEtaPhiM(Gen_Pt->at(i), Gen_Eta->at(i), Gen_Phi->at(i), Gen_Mass->at(i));
					// The lepton from W
					else if ( (fabs(Gen_PdgID->at(i))==11 || fabs(Gen_PdgID->at(i))==13)
							&& fabs(Gen_MomPdgID->at(i))==24 && Gen_fromHardProcessFinalState->at(i) ) {
						gen_lep.SetPtEtaPhiM(Gen_Pt->at(i), Gen_Eta->at(i), Gen_Phi->at(i), Gen_Mass->at(i));
						gen_lep_ID = Gen_PdgID->at(i);
					}
				} // end gen particle loop
			} // end if is_signal

			++Nevt_ST;
			NPerm = NbJets * Nleps;
			idxPerm = 0;
			M1Jet_idx = -1;
			M1Jet_Pt = -999;
			M1Jet_Eta = -999;
			M1Jet_Phi = -999;
			M1Jet_btag = -999;
			for (int i=0; i<NbJets; ++i) { // start b-jet loop
				for (int j=0; j<Nleps; ++j) { // start lepton loop

					bJet_idx = idx_bJets[i];
					bJet_Pt = Jet_Pt->at(bJet_idx);
					bJet_Eta = Jet_Eta->at(bJet_idx);
					bJet_Phi = Jet_Phi->at(bJet_idx);
					bJet_btag = Jet_probb->at(bJet_idx)+Jet_probbb->at(bJet_idx);
					// Set lep_idx, lep_ID
					lep_idx = j;
					if (Lep_Type[j] == 0) {
						if (Lep_Charge[j] > 0) lep_ID = -11;
						else lep_ID = 11;
					} else {
						lep_idx -= Elec_Size;
						if (Lep_Charge[j] > 0) lep_ID = -13;
						else lep_ID = 13;
					}
					lep_Pt = Lep_Pt[j];
					lep_Eta = Lep_Eta[j];
					lep_Phi = Lep_Phi[j];

					if (is_signal) { // if is_signal
						reco_bJet.SetPtEtaPhiE(bJet_Pt, bJet_Eta, bJet_Phi, Jet_Energy->at(bJet_idx));
						reco_lep.SetPtEtaPhiE(lep_Pt, lep_Eta, lep_Phi, Lep_Energy[j]);
						// Test if the event is truch matched
						if (DiPho_leadGenMatchType==1 && DiPho_subleadGenMatchType==1 &&
								reco_bJet.DeltaR(gen_bJet)<cut_dR && reco_lep.DeltaR(gen_lep)<cut_dR && lep_ID==gen_lep_ID)
							match = 1;
						else match = 0;
						// Fill output tree
						if (evt < Nevt/2) T_train->Fill();
						else T_test->Fill();
					} else
						T_ST->Fill(); // end if is_signal

					++idxPerm;
					++Nperm_ST;
				} // end lepton loop
			} // end b-jet loop
		} // End if run_STlep
	} // end input tree loop

	fout->Write(); fout->Close();
	cout << "[INFO] Results saved at: " << fout_name << endl;
	cout << "\n[Summary]\n";
	cout << "Input events: " << Nevt << endl;
	cout << "TT events: " << Nevt_TT << endl;
	cout << "ST events: " << Nevt_ST << endl;
	cout << "TT permutations: " << Nperm_TT << endl;
	cout << "ST permutations: " << Nperm_ST << endl;

	return 0;
}
