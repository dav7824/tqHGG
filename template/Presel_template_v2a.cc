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

	/* BEGIN: Variables for input & output TTree */
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
	int ElecInfo_Size = 0;
	vector<int>* ElecInfo_Charge = 0;
	vector<float>* ElecInfo_Pt = 0;
	vector<float>* ElecInfo_Eta = 0;
	vector<float>* ElecInfo_Phi = 0;
	vector<float>* ElecInfo_Energy = 0;
	vector<bool>* ElecInfo_EGMCutBasedIDMedium = 0;
	int MuonInfo_Size = 0;
	vector<int>* MuonInfo_Charge = 0;
	vector<float>* MuonInfo_Pt = 0;
	vector<float>* MuonInfo_Eta = 0;
	vector<float>* MuonInfo_Phi = 0;
	vector<float>* MuonInfo_Energy = 0;
	vector<float>* MuonInfo_PFIsoDeltaBetaCorrR04 = 0;
	vector<bool>* MuonInfo_CutBasedIdMedium = 0;
	vector<bool>* MuonInfo_CutBasedIdTight = 0;
	vector<bool>* MuonInfo_CutBasedIdTight_bestVtx = 0;
	int jets_size = 0;
	vector<float>* JetInfo_Pt = 0;
	vector<float>* JetInfo_Eta = 0;
	vector<float>* JetInfo_Phi = 0;
	vector<float>* JetInfo_Mass = 0;
	vector<float>* JetInfo_Energy = 0;
	vector<float>* JetInfo_pfDeepCSVJetTags_probb = 0;
	vector<float>* JetInfo_pfDeepCSVJetTags_probbb = 0;
	vector<float>* JetInfo_pfDeepCSVJetTags_probc = 0;
	vector<float>* JetInfo_pfDeepCSVJetTags_probudsg = 0;
	vector<float>* JetInfo_JECScale = 0;
	vector<float>* JetInfo_JERScale = 0;
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
	/* END: Variables for input & output TTree */

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
	inTree->SetBranchAddress("ElecInfo.Size", &ElecInfo_Size);
	inTree->SetBranchAddress("ElecInfo.Charge", &ElecInfo_Charge);
	inTree->SetBranchAddress("ElecInfo.Pt", &ElecInfo_Pt);
	inTree->SetBranchAddress("ElecInfo.Eta", &ElecInfo_Eta);
	inTree->SetBranchAddress("ElecInfo.Phi", &ElecInfo_Phi);
	inTree->SetBranchAddress("ElecInfo.Energy", &ElecInfo_Energy);
	inTree->SetBranchAddress("ElecInfo.EGMCutBasedIDMedium", &ElecInfo_EGMCutBasedIDMedium);
	inTree->SetBranchAddress("MuonInfo.Size", &MuonInfo_Size);
	inTree->SetBranchAddress("MuonInfo.Charge", &MuonInfo_Charge);
	inTree->SetBranchAddress("MuonInfo.Pt", &MuonInfo_Pt);
	inTree->SetBranchAddress("MuonInfo.Eta", &MuonInfo_Eta);
	inTree->SetBranchAddress("MuonInfo.Phi", &MuonInfo_Phi);
	inTree->SetBranchAddress("MuonInfo.Energy", &MuonInfo_Energy);
	inTree->SetBranchAddress("MuonInfo.PFIsoDeltaBetaCorrR04", &MuonInfo_PFIsoDeltaBetaCorrR04);
	inTree->SetBranchAddress("MuonInfo.CutBasedIdMedium", &MuonInfo_CutBasedIdMedium);
	inTree->SetBranchAddress("MuonInfo.CutBasedIdTight", &MuonInfo_CutBasedIdTight);
	inTree->SetBranchAddress("MuonInfo.CutBasedIdTight_bestVtx", &MuonInfo_CutBasedIdTight_bestVtx);
	inTree->SetBranchAddress("jets_size", &jets_size);
	inTree->SetBranchAddress("JetInfo.Pt", &JetInfo_Pt);
	inTree->SetBranchAddress("JetInfo.Eta", &JetInfo_Eta);
	inTree->SetBranchAddress("JetInfo.Phi", &JetInfo_Phi);
	inTree->SetBranchAddress("JetInfo.Mass", &JetInfo_Mass);
	inTree->SetBranchAddress("JetInfo.Energy", &JetInfo_Energy);
	inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probb", &JetInfo_pfDeepCSVJetTags_probb);
	inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probbb", &JetInfo_pfDeepCSVJetTags_probbb);
	inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probc", &JetInfo_pfDeepCSVJetTags_probc);
	inTree->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probudsg", &JetInfo_pfDeepCSVJetTags_probudsg);
	inTree->SetBranchAddress("JetInfo.JECScale", &JetInfo_JECScale);
	inTree->SetBranchAddress("JetInfo.JERScale", &JetInfo_JERScale);
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
	outTree->Branch("ElecInfo.Size", &ElecInfo_Size);
	outTree->Branch("ElecInfo.Charge", &ElecInfo_Charge);
	outTree->Branch("ElecInfo.Pt", &ElecInfo_Pt);
	outTree->Branch("ElecInfo.Eta", &ElecInfo_Eta);
	outTree->Branch("ElecInfo.Phi", &ElecInfo_Phi);
	outTree->Branch("ElecInfo.Energy", &ElecInfo_Energy);
	outTree->Branch("ElecInfo.EGMCutBasedIDMedium", &ElecInfo_EGMCutBasedIDMedium);
	outTree->Branch("MuonInfo.Size", &MuonInfo_Size);
	outTree->Branch("MuonInfo.Charge", &MuonInfo_Charge);
	outTree->Branch("MuonInfo.Pt", &MuonInfo_Pt);
	outTree->Branch("MuonInfo.Eta", &MuonInfo_Eta);
	outTree->Branch("MuonInfo.Phi", &MuonInfo_Phi);
	outTree->Branch("MuonInfo.Energy", &MuonInfo_Energy);
	outTree->Branch("MuonInfo.PFIsoDeltaBetaCorrR04", &MuonInfo_PFIsoDeltaBetaCorrR04);
	outTree->Branch("MuonInfo.CutBasedIdMedium", &MuonInfo_CutBasedIdMedium);
	outTree->Branch("MuonInfo.CutBasedIdTight", &MuonInfo_CutBasedIdTight);
	outTree->Branch("MuonInfo.CutBasedIdTight_bestVtx", &MuonInfo_CutBasedIdTight_bestVtx);
	outTree->Branch("jets_size", &jets_size);
	outTree->Branch("JetInfo.Pt", &JetInfo_Pt);
	outTree->Branch("JetInfo.Eta", &JetInfo_Eta);
	outTree->Branch("JetInfo.Phi", &JetInfo_Phi);
	outTree->Branch("JetInfo.Mass", &JetInfo_Mass);
	outTree->Branch("JetInfo.Energy", &JetInfo_Energy);
	outTree->Branch("JetInfo.pfDeepCSVJetTags_probb", &JetInfo_pfDeepCSVJetTags_probb);
	outTree->Branch("JetInfo.pfDeepCSVJetTags_probbb", &JetInfo_pfDeepCSVJetTags_probbb);
	outTree->Branch("JetInfo.pfDeepCSVJetTags_probc", &JetInfo_pfDeepCSVJetTags_probc);
	outTree->Branch("JetInfo.pfDeepCSVJetTags_probudsg", &JetInfo_pfDeepCSVJetTags_probudsg);
	outTree->Branch("JetInfo.JECScale", &JetInfo_JECScale);
	outTree->Branch("JetInfo.JERScale", &JetInfo_JERScale);
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
		/* END: Selection cuts on whole events */

		/* BEGIN: Cleanup */
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
