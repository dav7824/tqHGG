#include "TFile.h"
#include "TChain.h"
#include "TH1D.h"
#include "TH2.h"
#include "TLorentzVector.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
using namespace std;

enum HistVar {
	dipho_mass,
	dipho_pt,
	dipho_eta,
	dipho_phi,
	dipho_E,
	//////////
	pho1_pt,
	pho1_eta,
	pho1_phi,
	pho1_E,
	pho1_idmva,
	pho2_pt,
	pho2_eta,
	pho2_phi,
	pho2_E,
	pho2_idmva,
	//////////
	elec_N,
	elec_charge,
	elec_pt,
	elec_eta,
	elec_phi,
	elec_E,
	//////////
	muon_N,
	muon_charge,
	muon_pt,
	muon_eta,
	muon_phi,
	muon_E,
	//////////
	lep_N,
	lep_charge,
	lep_pt,
	lep_eta,
	lep_phi,
	lep_E,
	//////////
	jet_N,
	jet_pt,
	jet_eta,
	jet_phi,
	jet_E,
	jet_M,
	//////////
	met_pt,
	met_phi,
	met_px,
	met_py,
	met_SumET,
	//////////
	nhist
};

int main(int argc, char **argv)
{
	// get input arguments
	TString fin_name = argv[1];
	TString inTree_name = argv[2];
	TString fout_name = argv[3];

	// read input root files
	cout << "[INFO] Reading input file: " << fin_name << endl;
	TChain *inTree = new TChain(inTree_name);
	inTree->Add(fin_name);

	// read scale factor root files
	/*bool use_SF_elec_RECO = 1;
	bool use_SF_elec_ID = 1;
	bool use_SF_muon_ID = 1;
	bool use_SF_muon_ISO = 1;
	TFile *fSF_elec_RECO = 0;
	TFile *fSF_elec_ID = 0;
	TFile *fSF_muon_ID = 0;
	TFile *fSF_muon_ISO = 0;
	TH2F *hSF_elec_RECO = 0;
	TH2F *hSF_elec_ID = 0;
	TH2D *hSF_muon_ID = 0;
	TH2D *hSF_muon_ISO = 0;
	if (use_SF_elec_RECO) {
		fSF_elec_RECO = new TFile("/wk_cms2/mc_cheng/public/tqHGG/2017/SF/egammaEffi.txt_EGM2D_runBCDEF_passingRECO.root");
		hSF_elec_RECO = (TH2F*)fSF_elec_RECO->Get("EGamma_SF2D");
	}
	if (use_SF_elec_ID) {
		fSF_elec_ID = new TFile("/wk_cms2/mc_cheng/public/tqHGG/2017/SF/2017_ElectronMedium.root");
		hSF_elec_ID = (TH2F*)fSF_elec_ID->Get("EGamma_SF2D");
	}
	if (use_SF_muon_ID) {
		fSF_muon_ID = new TFile("/wk_cms2/mc_cheng/public/tqHGG/2017/SF/RunBCDEF_SF_ID.root");
		hSF_muon_ID = (TH2D*)fSF_muon_ID->Get("NUM_TightID_DEN_genTracks_pt_abseta");
	}
	if (use_SF_muon_ISO) {
		fSF_muon_ISO = new TFile("/wk_cms2/mc_cheng/public/tqHGG/2017/SF/RunBCDEF_SF_ISO.root");
		hSF_muon_ISO = (TH2D*)fSF_muon_ISO->Get("NUM_LooseRelIso_DEN_TightIDandIPCut_pt_abseta");
	}*/

	// create output file
	TFile *fout = new TFile(fout_name, "recreate");

	// create histograms
	TH1D *hists[nhist];
	hists[dipho_mass] =  new TH1D("dipho_mass", ";diphoton mass (GeV);",   16,  100,  180);
	hists[dipho_pt] =    new TH1D("dipho_pt",   ";diphoton p_{T} (GeV);",  20,    0,  400);
	hists[dipho_eta] =   new TH1D("dipho_eta",  ";diphoton #eta;",         20,   -3,    3);
	hists[dipho_phi] =   new TH1D("dipho_phi",  ";diphoton #phi;",         20, -3.5,  3.5);
	hists[dipho_E] =     new TH1D("dipho_E",    ";diphoton energy (GeV);", 20,    0,  800);
	hists[pho1_pt] =     new TH1D("pho1_pt",    ";leading photon p_{T} (GeV);",     20,    0,  300);
	hists[pho1_eta] =    new TH1D("pho1_eta",   ";leading photon #eta;",            20,   -3,    3);
	hists[pho1_phi] =    new TH1D("pho1_phi",   ";leading photon #phi;",            20, -3.5,  3.5);
	hists[pho1_E] =      new TH1D("pho1_E",     ";leading photon energy (GeV);",    20,    0,  600);
	hists[pho1_idmva] =  new TH1D("pho1_idmva", ";leading photon IDMVA;",           20,   -1,    1);
	hists[pho2_pt] =     new TH1D("pho2_pt",    ";subleading photon p_{T} (GeV);",  20,    0,  150);
	hists[pho2_eta] =    new TH1D("pho2_eta",   ";subleading photon #eta;",         20,   -3,    3);
	hists[pho2_phi] =    new TH1D("pho2_phi",   ";subleading photon #phi;",         20, -3.5,  3.5);
	hists[pho2_E] =      new TH1D("pho2_E",     ";subleading photon energy (GeV);", 20,    0,  300);
	hists[pho2_idmva] =  new TH1D("pho2_idmva", ";subleading photon IDMVA;",        20,   -1,    1);
	hists[elec_N] =      new TH1D("elec_N",      ";number of electrons;",    7, -0.5, 6.5);
	hists[elec_charge] = new TH1D("elec_charge", ";electron charge;",        5, -2.5, 2.5);
	hists[elec_pt] =     new TH1D("elec_pt",     ";electron p_{T} (GeV);",  20,    0, 250);
	hists[elec_eta] =    new TH1D("elec_eta",    ";electron #eta;",         20,   -3,   3);
	hists[elec_phi] =    new TH1D("elec_phi",    ";electron #phi;",         20, -3.5, 3.5);
	hists[elec_E] =      new TH1D("elec_E",      ";electron energy (GeV);", 20,    0, 500);
	hists[muon_N] =      new TH1D("muon_N",      ";number of muons;",        7, -0.5, 6.5);
	hists[muon_charge] = new TH1D("muon_charge", ";muon charge;",            5, -2.5, 2.5);
	hists[muon_pt] =     new TH1D("muon_pt",     ";muon p_{T} (GeV);",      20,    0, 250);
	hists[muon_eta] =    new TH1D("muon_eta",    ";muon #eta;",             20,   -3,   3);
	hists[muon_phi] =    new TH1D("muon_phi",    ";muon #phi;",             20, -3.5, 3.5);
	hists[muon_E] =      new TH1D("muon_E",      ";muon energy (GeV);",     20,    0, 500);
	hists[lep_N] =       new TH1D("lep_N",       ";number of leptons;",      7, -0.5, 6.5);
	hists[lep_charge] =  new TH1D("lep_charge",  ";lepton charge;",          5, -2.5, 2.5);
	hists[lep_pt] =      new TH1D("lep_pt",      ";lepton p_{T} (GeV);",    20,    0, 250);
	hists[lep_eta] =     new TH1D("lep_eta",     ";lepton #eta;",           20,   -3,   3);
	hists[lep_phi] =     new TH1D("lep_phi",     ";lepton #phi;",           20, -3.5, 3.5);
	hists[lep_E] =       new TH1D("lep_E",       ";lepton energy (GeV);",   20,    0, 500);
	hists[jet_N] =       new TH1D("jet_N",       ";number of jets;",   12, -0.5, 11.5);
	hists[jet_pt] =      new TH1D("jet_pt",      ";jet p_{T} (GeV);",  20,    0,  300);
	hists[jet_eta] =     new TH1D("jet_eta",     ";jet #eta;",         20,   -3,    3);
	hists[jet_phi] =     new TH1D("jet_phi",     ";jet #phi;",         20, -3.5,  3.5);
	hists[jet_E] =       new TH1D("jet_E",       ";jet energy (GeV);", 20,    0,  600);
	hists[jet_M] =       new TH1D("jet_M",       ";jet mass (GeV);",   20,    0,   50);
	hists[met_pt] =      new TH1D("met_pt",      ";MET p_{T} (GeV);",       20,    0,  200);
	hists[met_phi] =     new TH1D("met_phi",     ";MET #phi;",              20, -3.5,  3.5);
	hists[met_px] =      new TH1D("met_px",      ";MET p_{x} (GeV);",       20, -200,  200);
	hists[met_py] =      new TH1D("met_py",      ";MET p_{y} (GeV);",       20, -200,  200);
	hists[met_SumET] =   new TH1D("met_SumET",   ";MET E^{sum}_{T} (GeV);", 20,    0, 4000);

	char *unit[nhist];
	unit[dipho_mass]  = "GeV";
	unit[dipho_pt]    = "GeV";
	unit[dipho_eta]   = "";
	unit[dipho_phi]   = "";
	unit[dipho_E]     = "GeV";
	unit[pho1_pt]    = "GeV";
	unit[pho1_eta]   = "";
	unit[pho1_phi]   = "";
	unit[pho1_E]     = "GeV";
	unit[pho1_idmva] = "";
	unit[pho2_pt]    = "GeV";
	unit[pho2_eta]   = "";
	unit[pho2_phi]   = "";
	unit[pho2_E]     = "GeV";
	unit[pho2_idmva] = "";
	unit[elec_N]      = "";
	unit[elec_charge] = "";
	unit[elec_pt]     = "GeV";
	unit[elec_eta]    = "";
	unit[elec_phi]    = "";
	unit[elec_E]      = "GeV";
	unit[muon_N]      = "";
	unit[muon_charge] = "";
	unit[muon_pt]     = "GeV";
	unit[muon_eta]    = "";
	unit[muon_phi]    = "";
	unit[muon_E]      = "GeV";
	unit[lep_N]       = "";
	unit[lep_charge]  = "";
	unit[lep_pt]      = "GeV";
	unit[lep_eta]     = "";
	unit[lep_phi]     = "";
	unit[lep_E]       = "GeV";
	unit[jet_N]   = "";
	unit[jet_pt]  = "GeV";
	unit[jet_eta] = "";
	unit[jet_phi] = "";
	unit[jet_E]   = "GeV";
	unit[jet_M]   = "GeV";
	unit[met_pt]    = "GeV";
	unit[met_phi]   = "";
	unit[met_px]    = "GeV";
	unit[met_py]    = "GeV";
	unit[met_SumET] = "GeV";

	for (int i=0; i<nhist; ++i) {
		double bin_width = (hists[i]->GetXaxis()->GetXmax() - hists[i]->GetXaxis()->GetXmin()) / hists[i]->GetNbinsX();
		hists[i]->SetYTitle( Form("events / %.2g %s", bin_width, unit[i]) );
		hists[i]->Sumw2();
	}

	cout << "Histograms are initialized\n";

	// declare input tree variables
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

	// set input tree branches
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

	cout << "Input tree is successfully set\n";

	// loop through input tree events
	for (long evt=0; evt<inTree->GetEntries(); ++evt) {
		inTree->GetEntry(evt);

		float weight = EvtInfo_genweight;

		TLorentzVector leadPho, subleadPho, diphoton;
		leadPho.SetPtEtaPhiE(DiPhoInfo_leadPt, DiPhoInfo_leadEta, DiPhoInfo_leadPhi, DiPhoInfo_leadE);
		subleadPho.SetPtEtaPhiE(DiPhoInfo_subleadPt, DiPhoInfo_subleadEta, DiPhoInfo_subleadPhi, DiPhoInfo_subleadE);
		diphoton = leadPho + subleadPho;

		hists[dipho_mass]->Fill(DiPhoInfo_mass, weight);
		hists[dipho_pt]->Fill(DiPhoInfo_pt, weight);
		hists[dipho_eta]->Fill(diphoton.Eta(), weight);
		hists[dipho_phi]->Fill(diphoton.Phi(), weight);
		hists[dipho_E]->Fill(diphoton.Energy(), weight);

		hists[pho1_pt]->Fill(DiPhoInfo_leadPt, weight);
		hists[pho1_eta]->Fill(DiPhoInfo_leadEta, weight);
		hists[pho1_phi]->Fill(DiPhoInfo_leadPhi, weight);
		hists[pho1_E]->Fill(DiPhoInfo_leadE, weight);
		hists[pho1_idmva]->Fill(DiPhoInfo_leadIDMVA, weight);
		hists[pho2_pt]->Fill(DiPhoInfo_subleadPt, weight);
		hists[pho2_eta]->Fill(DiPhoInfo_subleadEta, weight);
		hists[pho2_phi]->Fill(DiPhoInfo_subleadPhi, weight);
		hists[pho2_E]->Fill(DiPhoInfo_subleadE, weight);
		hists[pho2_idmva]->Fill(DiPhoInfo_subleadIDMVA, weight);

		hists[elec_N]->Fill(ElecInfo_Size, weight);
		for (int i=0; i<ElecInfo_Size; ++i) {
			hists[elec_charge]->Fill(ElecInfo_Charge->at(i), weight);
			hists[elec_pt]->Fill(ElecInfo_Pt->at(i), weight);
			hists[elec_eta]->Fill(ElecInfo_Eta->at(i), weight);
			hists[elec_phi]->Fill(ElecInfo_Phi->at(i), weight);
			hists[elec_E]->Fill(ElecInfo_Energy->at(i), weight);

			hists[lep_charge]->Fill(ElecInfo_Charge->at(i), weight);
			hists[lep_pt]->Fill(ElecInfo_Pt->at(i), weight);
			hists[lep_eta]->Fill(ElecInfo_Eta->at(i), weight);
			hists[lep_phi]->Fill(ElecInfo_Phi->at(i), weight);
			hists[lep_E]->Fill(ElecInfo_Energy->at(i), weight);
		}
		hists[muon_N]->Fill(MuonInfo_Size, weight);
		for (int i=0; i<MuonInfo_Size; ++i) {
			hists[muon_charge]->Fill(MuonInfo_Charge->at(i), weight);
			hists[muon_pt]->Fill(MuonInfo_Pt->at(i), weight);
			hists[muon_eta]->Fill(MuonInfo_Eta->at(i), weight);
			hists[muon_phi]->Fill(MuonInfo_Phi->at(i), weight);
			hists[muon_E]->Fill(MuonInfo_Energy->at(i), weight);

			hists[lep_charge]->Fill(MuonInfo_Charge->at(i), weight);
			hists[lep_pt]->Fill(MuonInfo_Pt->at(i), weight);
			hists[lep_eta]->Fill(MuonInfo_Eta->at(i), weight);
			hists[lep_phi]->Fill(MuonInfo_Phi->at(i), weight);
			hists[lep_E]->Fill(MuonInfo_Energy->at(i), weight);
		}
		hists[lep_N]->Fill(ElecInfo_Size+MuonInfo_Size, weight);

		hists[jet_N]->Fill(jets_size, weight);
		for (int i=0; i<jets_size; ++i) {
			hists[jet_pt]->Fill(JetInfo_Pt->at(i), weight);
			hists[jet_eta]->Fill(JetInfo_Eta->at(i), weight);
			hists[jet_phi]->Fill(JetInfo_Phi->at(i), weight);
			hists[jet_E]->Fill(JetInfo_Energy->at(i), weight);
			hists[jet_M]->Fill(JetInfo_Mass->at(i), weight);
		}

		hists[met_pt]->Fill(MetInfo_Pt, weight);
		hists[met_phi]->Fill(MetInfo_Phi, weight);
		hists[met_px]->Fill(MetInfo_Px, weight);
		hists[met_py]->Fill(MetInfo_Py, weight);
		hists[met_SumET]->Fill(MetInfo_SumET, weight);
	} // end of input tree event loop

	// save the histograms
	fout->cd();
	for (int i=0; i<nhist; ++i) hists[i]->Write();
	fout->Close();
	cout << "[INFO] Output is saved at: " << fout_name << endl;

	return 0;
}
