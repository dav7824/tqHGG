// Preselection:
//   Trigger:
//     Diphoton triggers
//   Photon:
//     (1) pt > 35(25) GeV
//     (2) |eta| < 2.5, excluding [1.4442, 1.566] (EE-EB gap)
//   Diphoton:
//     Mgg > 100 GeV
//   Electron:
//     (1) Cut-based medium ID
//     (2) pt > 10 GeV
//     (3) |eta| < 2.4, excluding [1.4442, 1.566] (EE-EB gap)
//     (4) delta_R(electron, photon) > 0.2
//   Muon:
//     (1) Cut-based medium ID
//     (2) pt > 5 GeV
//     (3) |eta| < 2.4
//     (4) Relative PF isolation (R=0.4) < 0.25, (loose WP)
//     (5) delta_R(muon, photon) > 0.2
//   Jet:
//     (1) pt > 25 GeV
//     (2) |eta| < 2.4
//     (3) delta_R(lepton/photon, jet) > 0.4

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TString.h"

#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

int main(int argc, char **argv)
{
	// Get input arguments
	TString fin_name = argv[1];
	TString inTree_name = argv[2];
	TString fout_name = argv[3];

	// Create TTree for input n-tuple
	cout << "Openning input file: " << fin_name << endl;
	TChain *inTree = new TChain(inTree_name);
	inTree->Add(fin_name);

	// Create TTree for output n-tuple
	cout << "Creating output file: " << fout_name << endl;
	TFile *fout = new TFile(fout_name, "recreate");
	TTree *outTree = new TTree("flashggStdTree", "");

	/* BEGIN: Common variables for input & output TTree */
	float EvtInfo_NPu = 0;
	int EvtInfo_NVtx = 0;
	bool EvtInfo_passTrigger = 0;
	float EvtInfo_genweight = 0;
	float EvtInfo_Rho = 0;
	float DiPhoInfo_mass = 0;
	float DiPhoInfo_pt = 0;
	float DiPhoInfo_leadPt = 0;
	float DiPhoInfo_leadEta = 0;
	float DiPhoInfo_leadPhi = 0;
	float DiPhoInfo_leadE = 0;
	float DiPhoInfo_leadIDMVA = 0;
	bool DiPhoInfo_leadhasPixelSeed = 0;
	float DiPhoInfo_subleadPt = 0;
	float DiPhoInfo_subleadEta = 0;
	float DiPhoInfo_subleadPhi = 0;
	float DiPhoInfo_subleadE = 0;
	float DiPhoInfo_subleadIDMVA = 0;
	bool DiPhoInfo_subleadhasPixelSeed = 0;
	float DiPhoInfo_diphotonMVA = 0;
	float DiPhoInfo_centralWeight = 0;
	float MetInfo_Pt = 0;
	float MetInfo_Phi = 0;
	float MetInfo_Px = 0;
	float MetInfo_Py = 0;
	float MetInfo_SumET = 0;
	int GenPartInfo_size = 0;
	vector<float>* GenPartInfo_Pt = 0;
	vector<float>* GenPartInfo_Eta = 0;
	vector<float>* GenPartInfo_Phi = 0;
	vector<float>* GenPartInfo_Mass = 0;
	vector<int>* GenPartInfo_PdgID = 0;
	vector<int>* GenPartInfo_Status = 0;
	vector<int>* GenPartInfo_MomPdgID = 0;
	vector<int>* GenPartInfo_MomStatus = 0;
	vector<float>* GenPartInfo_MomPt = 0;
	vector<float>* GenPartInfo_MomEta = 0;
	vector<float>* GenPartInfo_MomPhi = 0;
	vector<float>* GenPartInfo_MomMass = 0;
	/* END: Common variables for input & output TTree */

	/* BEGIN: Variables for input TTree */
	int ElecInfo_Size_in = 0;
	vector<int>* ElecInfo_Charge_in = 0;
	vector<float>* ElecInfo_Pt_in = 0;
	vector<float>* ElecInfo_Eta_in = 0;
	vector<float>* ElecInfo_Phi_in = 0;
	vector<float>* ElecInfo_Energy_in = 0;
	vector<bool>* ElecInfo_EGMCutBasedIDMedium_in = 0;
	int MuonInfo_Size_in = 0;
	vector<int>* MuonInfo_Charge_in = 0;
	vector<float>* MuonInfo_Pt_in = 0;
	vector<float>* MuonInfo_Eta_in = 0;
	vector<float>* MuonInfo_Phi_in = 0;
	vector<float>* MuonInfo_Energy_in = 0;
	vector<float>* MuonInfo_PFIsoDeltaBetaCorrR04_in = 0;
	vector<bool>* MuonInfo_CutBasedIdMedium_in = 0;
	vector<bool>* MuonInfo_CutBasedIdTight_in = 0;
	vector<bool>* MuonInfo_CutBasedIdTight_bestVtx_in = 0;
	int jets_size_in = 0;
	vector<float>* JetInfo_Pt_in = 0;
	vector<float>* JetInfo_Eta_in = 0;
	vector<float>* JetInfo_Phi_in = 0;
	vector<float>* JetInfo_Mass_in = 0;
	vector<float>* JetInfo_Energy_in = 0;
	vector<float>* JetInfo_pfDeepCSVJetTags_probb_in = 0;
	vector<float>* JetInfo_pfDeepCSVJetTags_probbb_in = 0;
	vector<float>* JetInfo_pfDeepCSVJetTags_probc_in = 0;
	vector<float>* JetInfo_pfDeepCSVJetTags_probudsg_in = 0;
	vector<float>* JetInfo_JECScale_in = 0;
	vector<float>* JetInfo_JERScale_in = 0;
	/* END: Variables for input TTree */

	/* BEGIN: Variables for output TTree */
	int ElecInfo_Size_out = 0;
	vector<int>* ElecInfo_Charge_out = 0;
	vector<float>* ElecInfo_Pt_out = 0;
	vector<float>* ElecInfo_Eta_out = 0;
	vector<float>* ElecInfo_Phi_out = 0;
	vector<float>* ElecInfo_Energy_out = 0;
	vector<bool>* ElecInfo_EGMCutBasedIDMedium_out = 0;
	int MuonInfo_Size_out = 0;
	vector<int>* MuonInfo_Charge_out = 0;
	vector<float>* MuonInfo_Pt_out = 0;
	vector<float>* MuonInfo_Eta_out = 0;
	vector<float>* MuonInfo_Phi_out = 0;
	vector<float>* MuonInfo_Energy_out = 0;
	vector<float>* MuonInfo_PFIsoDeltaBetaCorrR04_out = 0;
	vector<bool>* MuonInfo_CutBasedIdMedium_out = 0;
	vector<bool>* MuonInfo_CutBasedIdTight_out = 0;
	vector<bool>* MuonInfo_CutBasedIdTight_bestVtx_out = 0;
	int jets_size_out = 0;
	vector<float>* JetInfo_Pt_out = 0;
	vector<float>* JetInfo_Eta_out = 0;
	vector<float>* JetInfo_Phi_out = 0;
	vector<float>* JetInfo_Mass_out = 0;
	vector<float>* JetInfo_Energy_out = 0;
	vector<float>* JetInfo_pfDeepCSVJetTags_probb_out = 0;
	vector<float>* JetInfo_pfDeepCSVJetTags_probbb_out = 0;
	vector<float>* JetInfo_pfDeepCSVJetTags_probc_out = 0;
	vector<float>* JetInfo_pfDeepCSVJetTags_probudsg_out = 0;
	vector<float>* JetInfo_JECScale_out = 0;
	vector<float>* JetInfo_JERScale_out = 0;
	/* END: Variables for output TTree */

	cout << "Setting input TTree...\n";
	/* BEGIN: Set branches for input TTree */
	inTree->SetBranchAddress("EvtInfo.NPu", &EvtInfo_NPu);
	inTree->SetBranchAddress("EvtInfo.NVtx", &EvtInfo_NVtx);
	inTree->SetBranchAddress("EvtInfo.passTrigger", &EvtInfo_passTrigger);
	inTree->SetBranchAddress("EvtInfo.genweight", &EvtInfo_genweight);
	inTree->SetBranchAddress("EvtInfo.Rho", &EvtInfo_Rho);
	inTree->SetBranchAddress("DiPhoInfo.mass", &DiPhoInfo_mass);
	inTree->SetBranchAddress("DiPhoInfo.pt", &DiPhoInfo_pt);
	inTree->SetBranchAddress("DiPhoInfo.leadPt", &DiPhoInfo_leadPt);
	inTree->SetBranchAddress("DiPhoInfo.leadEta", &DiPhoInfo_leadEta);
	inTree->SetBranchAddress("DiPhoInfo.leadPhi", &DiPhoInfo_leadPhi);
	inTree->SetBranchAddress("DiPhoInfo.leadE", &DiPhoInfo_leadE);
	inTree->SetBranchAddress("DiPhoInfo.leadIDMVA", &DiPhoInfo_leadIDMVA);
	inTree->SetBranchAddress("DiPhoInfo.leadhasPixelSeed", &DiPhoInfo_leadhasPixelSeed);
	inTree->SetBranchAddress("DiPhoInfo.subleadPt", &DiPhoInfo_subleadPt);
	inTree->SetBranchAddress("DiPhoInfo.subleadEta", &DiPhoInfo_subleadEta);
	inTree->SetBranchAddress("DiPhoInfo.subleadPhi", &DiPhoInfo_subleadPhi);
	inTree->SetBranchAddress("DiPhoInfo.subleadE", &DiPhoInfo_subleadE);
	inTree->SetBranchAddress("DiPhoInfo.subleadIDMVA", &DiPhoInfo_subleadIDMVA);
	inTree->SetBranchAddress("DiPhoInfo.subleadhasPixelSeed", &DiPhoInfo_subleadhasPixelSeed);
	inTree->SetBranchAddress("DiPhoInfo.diphotonMVA", &DiPhoInfo_diphotonMVA);
	inTree->SetBranchAddress("DiPhoInfo.centralWeight", &DiPhoInfo_centralWeight);
	inTree->SetBranchAddress("ElecInfo.Size", &ElecInfo_Size_in);
	inTree->SetBranchAddress("ElecInfo.Charge", &ElecInfo_Charge_in);
	inTree->SetBranchAddress("ElecInfo.Pt", &ElecInfo_Pt_in);
	inTree->SetBranchAddress("ElecInfo.Eta", &ElecInfo_Eta_in);
	inTree->SetBranchAddress("ElecInfo.Phi", &ElecInfo_Phi_in);
	inTree->SetBranchAddress("ElecInfo.Energy", &ElecInfo_Energy_in);
	inTree->SetBranchAddress("ElecInfo.EGMCutBasedIDMedium", &ElecInfo_EGMCutBasedIDMedium_in);
	inTree->SetBranchAddress("MuonInfo.Size", &MuonInfo_Size_in);
	inTree->SetBranchAddress("MuonInfo.Charge", &MuonInfo_Charge_in);
	inTree->SetBranchAddress("MuonInfo.Pt", &MuonInfo_Pt_in);
	inTree->SetBranchAddress("MuonInfo.Eta", &MuonInfo_Eta_in);
	inTree->SetBranchAddress("MuonInfo.Phi", &MuonInfo_Phi_in);
	inTree->SetBranchAddress("MuonInfo.Energy", &MuonInfo_Energy_in);
	inTree->SetBranchAddress("MuonInfo.PFIsoDeltaBetaCorrR04", &MuonInfo_PFIsoDeltaBetaCorrR04_in);
	inTree->SetBranchAddress("MuonInfo.CutBasedIdMedium", &MuonInfo_CutBasedIdMedium_in);
	inTree->SetBranchAddress("MuonInfo.CutBasedIdTight", &MuonInfo_CutBasedIdTight_in);
	inTree->SetBranchAddress("MuonInfo.CutBasedIdTight_bestVtx", &MuonInfo_CutBasedIdTight_bestVtx_in);
	inTree->SetBranchAddress("jets_size", &jets_size_in);
	inTree->SetBranchAddress("JetInfo.Pt", &JetInfo_Pt_in);
	inTree->SetBranchAddress("JetInfo.Eta", &JetInfo_Eta_in);
	inTree->SetBranchAddress("JetInfo.Phi", &JetInfo_Phi_in);
	inTree->SetBranchAddress("JetInfo.Mass", &JetInfo_Mass_in);
	inTree->SetBranchAddress("JetInfo.Energy", &JetInfo_Energy_in);
	inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probb", &JetInfo_pfDeepCSVJetTags_probb_in);
	inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probbb", &JetInfo_pfDeepCSVJetTags_probbb_in);
	inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probc", &JetInfo_pfDeepCSVJetTags_probc_in);
	inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probudsg", &JetInfo_pfDeepCSVJetTags_probudsg_in);
	inTree->SetBranchAddress("JetInfo.JECScale", &JetInfo_JECScale_in);
	inTree->SetBranchAddress("JetInfo.JERScale", &JetInfo_JERScale_in);
	inTree->SetBranchAddress("MetInfo.Pt", &MetInfo_Pt);
	inTree->SetBranchAddress("MetInfo.Phi", &MetInfo_Phi);
	inTree->SetBranchAddress("MetInfo.Px", &MetInfo_Px);
	inTree->SetBranchAddress("MetInfo.Py", &MetInfo_Py);
	inTree->SetBranchAddress("MetInfo.SumET", &MetInfo_SumET);
	inTree->SetBranchAddress("GenPartInfo.size", &GenPartInfo_size);
	inTree->SetBranchAddress("GenPartInfo.Pt", &GenPartInfo_Pt);
	inTree->SetBranchAddress("GenPartInfo.Eta", &GenPartInfo_Eta);
	inTree->SetBranchAddress("GenPartInfo.Phi", &GenPartInfo_Phi);
	inTree->SetBranchAddress("GenPartInfo.Mass", &GenPartInfo_Mass);
	inTree->SetBranchAddress("GenPartInfo.PdgID", &GenPartInfo_PdgID);
	inTree->SetBranchAddress("GenPartInfo.Status", &GenPartInfo_Status);
	inTree->SetBranchAddress("GenPartInfo.MomPdgID", &GenPartInfo_MomPdgID);
	inTree->SetBranchAddress("GenPartInfo.MomStatus", &GenPartInfo_MomStatus);
	inTree->SetBranchAddress("GenPartInfo.MomPt", &GenPartInfo_MomPt);
	inTree->SetBranchAddress("GenPartInfo.MomEta", &GenPartInfo_MomEta);
	inTree->SetBranchAddress("GenPartInfo.MomPhi", &GenPartInfo_MomPhi);
	inTree->SetBranchAddress("GenPartInfo.MomMass", &GenPartInfo_MomMass);
	/* END: Set branches for input TTree */

	cout << "Setting output TTree...\n";
	/* BEGIN: Set branches for output TTree */
	outTree->Branch("EvtInfo.NPu", &EvtInfo_NPu);
	outTree->Branch("EvtInfo.NVtx", &EvtInfo_NVtx);
	outTree->Branch("EvtInfo.passTrigger", &EvtInfo_passTrigger);
	outTree->Branch("EvtInfo.genweight", &EvtInfo_genweight);
	outTree->Branch("EvtInfo.Rho", &EvtInfo_Rho);
	outTree->Branch("DiPhoInfo.mass", &DiPhoInfo_mass);
	outTree->Branch("DiPhoInfo.pt", &DiPhoInfo_pt);
	outTree->Branch("DiPhoInfo.leadPt", &DiPhoInfo_leadPt);
	outTree->Branch("DiPhoInfo.leadEta", &DiPhoInfo_leadEta);
	outTree->Branch("DiPhoInfo.leadPhi", &DiPhoInfo_leadPhi);
	outTree->Branch("DiPhoInfo.leadE", &DiPhoInfo_leadE);
	outTree->Branch("DiPhoInfo.leadIDMVA", &DiPhoInfo_leadIDMVA);
	outTree->Branch("DiPhoInfo.leadhasPixelSeed", &DiPhoInfo_leadhasPixelSeed);
	outTree->Branch("DiPhoInfo.subleadPt", &DiPhoInfo_subleadPt);
	outTree->Branch("DiPhoInfo.subleadEta", &DiPhoInfo_subleadEta);
	outTree->Branch("DiPhoInfo.subleadPhi", &DiPhoInfo_subleadPhi);
	outTree->Branch("DiPhoInfo.subleadE", &DiPhoInfo_subleadE);
	outTree->Branch("DiPhoInfo.subleadIDMVA", &DiPhoInfo_subleadIDMVA);
	outTree->Branch("DiPhoInfo.subleadhasPixelSeed", &DiPhoInfo_subleadhasPixelSeed);
	outTree->Branch("DiPhoInfo.diphotonMVA", &DiPhoInfo_diphotonMVA);
	outTree->Branch("DiPhoInfo.centralWeight", &DiPhoInfo_centralWeight);
	outTree->Branch("ElecInfo.Size", &ElecInfo_Size_out);
	outTree->Branch("ElecInfo.Charge", &ElecInfo_Charge_out);
	outTree->Branch("ElecInfo.Pt", &ElecInfo_Pt_out);
	outTree->Branch("ElecInfo.Eta", &ElecInfo_Eta_out);
	outTree->Branch("ElecInfo.Phi", &ElecInfo_Phi_out);
	outTree->Branch("ElecInfo.Energy", &ElecInfo_Energy_out);
	outTree->Branch("ElecInfo.EGMCutBasedIDMedium", &ElecInfo_EGMCutBasedIDMedium_out);
	outTree->Branch("MuonInfo.Size", &MuonInfo_Size_out);
	outTree->Branch("MuonInfo.Charge", &MuonInfo_Charge_out);
	outTree->Branch("MuonInfo.Pt", &MuonInfo_Pt_out);
	outTree->Branch("MuonInfo.Eta", &MuonInfo_Eta_out);
	outTree->Branch("MuonInfo.Phi", &MuonInfo_Phi_out);
	outTree->Branch("MuonInfo.Energy", &MuonInfo_Energy_out);
	outTree->Branch("MuonInfo.PFIsoDeltaBetaCorrR04", &MuonInfo_PFIsoDeltaBetaCorrR04_out);
	outTree->Branch("MuonInfo.CutBasedIdMedium", &MuonInfo_CutBasedIdMedium_out);
	outTree->Branch("MuonInfo.CutBasedIdTight", &MuonInfo_CutBasedIdTight_out);
	outTree->Branch("MuonInfo.CutBasedIdTight_bestVtx", &MuonInfo_CutBasedIdTight_bestVtx_out);
	outTree->Branch("jets_size", &jets_size_out);
	outTree->Branch("JetInfo.Pt", &JetInfo_Pt_out);
	outTree->Branch("JetInfo.Eta", &JetInfo_Eta_out);
	outTree->Branch("JetInfo.Phi", &JetInfo_Phi_out);
	outTree->Branch("JetInfo.Mass", &JetInfo_Mass_out);
	outTree->Branch("JetInfo.Energy", &JetInfo_Energy_out);
	outTree->Branch("JetInfo.pfDeepCSVJetTags_probb", &JetInfo_pfDeepCSVJetTags_probb_out);
	outTree->Branch("JetInfo.pfDeepCSVJetTags_probbb", &JetInfo_pfDeepCSVJetTags_probbb_out);
	outTree->Branch("JetInfo.pfDeepCSVJetTags_probc", &JetInfo_pfDeepCSVJetTags_probc_out);
	outTree->Branch("JetInfo.pfDeepCSVJetTags_probudsg", &JetInfo_pfDeepCSVJetTags_probudsg_out);
	outTree->Branch("JetInfo.JECScale", &JetInfo_JECScale_out);
	outTree->Branch("JetInfo.JERScale", &JetInfo_JERScale_out);
	outTree->Branch("MetInfo.Pt", &MetInfo_Pt);
	outTree->Branch("MetInfo.Phi", &MetInfo_Phi);
	outTree->Branch("MetInfo.Px", &MetInfo_Px);
	outTree->Branch("MetInfo.Py", &MetInfo_Py);
	outTree->Branch("MetInfo.SumET", &MetInfo_SumET);
	outTree->Branch("GenPartInfo.size", &GenPartInfo_size);
	outTree->Branch("GenPartInfo.Pt", &GenPartInfo_Pt);
	outTree->Branch("GenPartInfo.Eta", &GenPartInfo_Eta);
	outTree->Branch("GenPartInfo.Phi", &GenPartInfo_Phi);
	outTree->Branch("GenPartInfo.Mass", &GenPartInfo_Mass);
	outTree->Branch("GenPartInfo.PdgID", &GenPartInfo_PdgID);
	outTree->Branch("GenPartInfo.Status", &GenPartInfo_Status);
	outTree->Branch("GenPartInfo.MomPdgID", &GenPartInfo_MomPdgID);
	outTree->Branch("GenPartInfo.MomStatus", &GenPartInfo_MomStatus);
	outTree->Branch("GenPartInfo.MomPt", &GenPartInfo_MomPt);
	outTree->Branch("GenPartInfo.MomEta", &GenPartInfo_MomEta);
	outTree->Branch("GenPartInfo.MomPhi", &GenPartInfo_MomPhi);
	outTree->Branch("GenPartInfo.MomMass", &GenPartInfo_MomMass);
	/* END: Set branches for output TTree */

	// Loop over input TTree to do preselection
	cout << "Start processing events...\n";
	for (int evt=0; evt<inTree->GetEntries(); ++evt)
	{
		inTree->GetEntry(evt);

		/* BEGIN: Prepare area A */
		/* END: Prepare area A */

		/* BEGIN: Selection cuts on whole events */
		if (!EvtInfo_passTrigger) continue;

		if (DiPhoInfo_mass < 100) continue;

		if (DiPhoInfo_leadPt < 35) continue;
		if (DiPhoInfo_subleadPt < 25) continue;
		if (fabs(DiPhoInfo_leadEta) > 2.5) continue;
		if (fabs(DiPhoInfo_subleadEta) > 2.5) continue;
		if (fabs(DiPhoInfo_leadEta) > 1.4442 && fabs(DiPhoInfo_leadEta) < 1.566) continue;
		if (fabs(DiPhoInfo_subleadEta) > 1.4442 && fabs(DiPhoInfo_subleadEta) < 1.566) continue;
		/* END: Selection cuts on whole events */

		// If the event passes the whole-event cuts, do selection on its physical objects and fill it to output TTree

		/* BEGIN: Reset output branches */
		ElecInfo_Size_out = 0;
		ElecInfo_Charge_out->clear();
		ElecInfo_Pt_out->clear();
		ElecInfo_Eta_out->clear();
		ElecInfo_Phi_out->clear();
		ElecInfo_Energy_out->clear();
		ElecInfo_EGMCutBasedIDMedium_out->clear();
		MuonInfo_Size_out = 0;
		MuonInfo_Charge_out->clear();
		MuonInfo_Pt_out->clear();
		MuonInfo_Eta_out->clear();
		MuonInfo_Phi_out->clear();
		MuonInfo_Energy_out->clear();
		MuonInfo_PFIsoDeltaBetaCorrR04_out->clear();
		MuonInfo_CutBasedIdMedium_out->clear();
		MuonInfo_CutBasedIdTight_out->clear();
		MuonInfo_CutBasedIdTight_bestVtx_out->clear();
		jets_size_out = 0;
		JetInfo_Pt_out->clear();
		JetInfo_Eta_out->clear();
		JetInfo_Phi_out->clear();
		JetInfo_Mass_out->clear();
		JetInfo_Energy_out->clear();
		JetInfo_pfDeepCSVJetTags_probb_out->clear();
		JetInfo_pfDeepCSVJetTags_probbb_out->clear();
		JetInfo_pfDeepCSVJetTags_probc_out->clear();
		JetInfo_pfDeepCSVJetTags_probudsg_out->clear();
		JetInfo_JECScale_out->clear();
		JetInfo_JERScale_out->clear();
		/* END: Reset output branches */

		/* BEGIN: Prepare area B */
		// Create 4-momenta of leading photon and subleading photon (used when calculating deltaR with other objects)
		TLorentzVector leadPho, subleadPho;
		leadPho.SetPtEtaPhiE(DiPhoInfo_leadPt, DiPhoInfo_leadEta, DiPhoInfo_leadPhi, DiPhoInfo_leadE);
		subleadPho.SetPtEtaPhiE(DiPhoInfo_subleadPt, DiPhoInfo_subleadEta, DiPhoInfo_subleadPhi, DiPhoInfo_subleadE);

		// Create vectors of 4-momenta of selected leptons (used when calculating deltaR with jets)
		vector<TLorentzVector*> electrons;
		vector<TLorentzVector*> muons;
		/* END: Prepare area B */

		/* BEGIN: Select electrons */
		for (int i=0; i<ElecInfo_Size_in; ++i)
		{
			// Selection of electrons
			if (!ElecInfo_EGMCutBasedIDMedium_in->at(i)) continue;
			if (ElecInfo_Pt_in->at(i) < 10) continue;
			if (fabs(ElecInfo_Eta_in->at(i)) > 2.4) continue;
			if (fabs(ElecInfo_Eta_in->at(i)) > 1.4442 && fabs(ElecInfo_Eta_in->at(i)) < 1.566) continue;
			TLorentzVector *electron_tmp = new TLorentzVector();
			electron_tmp->SetPtEtaPhiE(ElecInfo_Pt_in->at(i), ElecInfo_Eta_in->at(i), ElecInfo_Phi_in->at(i), ElecInfo_Energy_in->at(i));
			// Cut on the angles between the electrons and photons
			if (electron_tmp->DeltaR(leadPho) < 0.2 || electron_tmp->DeltaR(subleadPho) < 0.2) { delete electron_tmp; continue; }
			electrons.push_back(electron_tmp);

			// Fill selected electrons to output vectors
			ElecInfo_Size_out += 1;
			ElecInfo_Charge_out->push_back( ElecInfo_Charge_in->at(i) );
			ElecInfo_Pt_out->push_back( ElecInfo_Pt_in->at(i) );
			ElecInfo_Eta_out->push_back( ElecInfo_Eta_in->at(i) );
			ElecInfo_Phi_out->push_back( ElecInfo_Phi_in->at(i) );
			ElecInfo_Energy_out->push_back( ElecInfo_Energy_in->at(i) );
			ElecInfo_EGMCutBasedIDMedium_out->push_back( ElecInfo_EGMCutBasedIDMedium_in->at(i) );
		}
		/* END: Select electrons */

		/* BEGIN: Select muons */
		for (int i=0; i<MuonInfo_Size_in; ++i)
		{
			// Selection of muons
			if (!MuonInfo_CutBasedIdMedium_in->at(i)) continue;
			if (MuonInfo_Pt_in->at(i) < 5) continue;
			if (fabs(MuonInfo_Eta_in->at(i)) > 2.4) continue;
			if (MuonInfo_PFIsoDeltaBetaCorrR04_in->at(i) > 0.25) continue;
			TLorentzVector *muon_tmp = new TLorentzVector();
			muon_tmp->SetPtEtaPhiE(MuonInfo_Pt_in->at(i), MuonInfo_Eta_in->at(i), MuonInfo_Phi_in->at(i), MuonInfo_Energy_in->at(i));
			// Cut on the angles between the muons and photons
			if (muon_tmp->DeltaR(leadPho) < 0.2 || muon_tmp->DeltaR(subleadPho) < 0.2) { delete muon_tmp; continue; }
			muons.push_back(muon_tmp);

			// Fill selected muons to output vectors
			MuonInfo_Size_out += 1;
			MuonInfo_Charge_out->push_back( MuonInfo_Charge_in->at(i) );
			MuonInfo_Pt_out->push_back( MuonInfo_Pt_in->at(i) );
			MuonInfo_Eta_out->push_back( MuonInfo_Eta_in->at(i) );
			MuonInfo_Phi_out->push_back( MuonInfo_Phi_in->at(i) );
			MuonInfo_Energy_out->push_back( MuonInfo_Energy_in->at(i) );
			MuonInfo_PFIsoDeltaBetaCorrR04_out->push_back( MuonInfo_PFIsoDeltaBetaCorrR04_in->at(i) );
			MuonInfo_CutBasedIdMedium_out->push_back( MuonInfo_CutBasedIdMedium_in->at(i) );
			MuonInfo_CutBasedIdTight_out->push_back( MuonInfo_CutBasedIdTight_in->at(i) );
			MuonInfo_CutBasedIdTight_bestVtx_out->push_back( MuonInfo_CutBasedIdTight_bestVtx_in->at(i) );
		}
		/* END: Select muons */

		/* BEGIN: Select jets */
		for (int i=0; i<jets_size_in; ++i)
		{
			// Selection of jets
			if (JetInfo_Pt_in->at(i) < 25) continue;
			if (fabs(JetInfo_Eta_in->at(i)) > 2.4) continue;
			TLorentzVector jet_tmp;
			jet_tmp.SetPtEtaPhiE(JetInfo_Pt_in->at(i), JetInfo_Eta_in->at(i), JetInfo_Phi_in->at(i), JetInfo_Energy_in->at(i));
			// Cut on the angles between the jets and photons
			if (jet_tmp.DeltaR(leadPho) < 0.4 || jet_tmp.DeltaR(subleadPho) < 0.4) continue;
			// Cut on the angles between the jets and selected leptons
			bool jet_is_iso = true;
			for (int j=0; j<electrons.size(); ++j)
			if (jet_tmp.DeltaR(*electrons.at(j)) < 0.4) { jet_is_iso = false; break; }
			if (!jet_is_iso) continue;
			for (int j=0; j<muons.size(); ++j)
			if (jet_tmp.DeltaR(*muons.at(j)) < 0.4) { jet_is_iso = false; break; }
			if (!jet_is_iso) continue;

			// Fill selected jets to output vector
			jets_size_out += 1;
			JetInfo_Pt_out->push_back( JetInfo_Pt_in->at(i) );
			JetInfo_Eta_out->push_back( JetInfo_Eta_in->at(i) );
			JetInfo_Phi_out->push_back( JetInfo_Phi_in->at(i) );
			JetInfo_Mass_out->push_back( JetInfo_Mass_in->at(i) );
			JetInfo_Energy_out->push_back( JetInfo_Energy_in->at(i) );
			JetInfo_pfDeepCSVJetTags_probb_out->push_back( JetInfo_pfDeepCSVJetTags_probb_in->at(i) );
			JetInfo_pfDeepCSVJetTags_probbb_out->push_back( JetInfo_pfDeepCSVJetTags_probbb_in->at(i) );
			JetInfo_pfDeepCSVJetTags_probc_out->push_back( JetInfo_pfDeepCSVJetTags_probc_in->at(i) );
			JetInfo_pfDeepCSVJetTags_probudsg_out->push_back( JetInfo_pfDeepCSVJetTags_probudsg_in->at(i) );
			JetInfo_JECScale_out->push_back( JetInfo_JECScale_in->at(i) );
			JetInfo_JERScale_out->push_back( JetInfo_JERScale_in->at(i) );
		}
		/* END: Select jets */

		/* BEGIN: Cleanup */
		for (int i=0; i<electrons.size(); ++i) delete electrons[i];
		for (int i=0; i<muons.size(); ++i) delete muons[i];
		/* END: Cleanup */

		// If the event passes the preselection, fill it to output TTree
		outTree->Fill();
		// Process the next event
	} // End of preselection loop over input TTree
	cout << "Completed processing all events.\n";

	// Write output TTree to output TFile
	fout->cd();
	outTree->Write();
	// Close output TFile
	fout->Close();

	cout << "Output is saved at: " << fout_name << endl;

	return 0;
}
