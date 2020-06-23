/*
 * Given reconstructed leptonic signal, reconstruct Pz of neutrino.
 * Usage:
 *   ./NuPz <event file> <event tree> <reco file> <reco tree> <n-tuple type>
 *
 * The new tree containing nu Pz info will overwrite original <reco tree> in <reco file>.
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

float W_mass = 80.379;
/* mu_mass = 0.1 GeV */
// Mass of muon and electron can be neglected in computation compared to W mass

int CalcNuPz( TLorentzVector &lep, float met_Px, float met_Py, float *nu_Pz );

int main(int argc, char **argv)
{
	// Get command line arguments
	TString fin1_name = argv[1];
	TString Tin1_name = argv[2];
	TString fin2_name = argv[3];
	TString Tin2_name = argv[4];
	TString nttype = argv[5];
	if (nttype!="TT" && nttype!="ST" && nttype!="bkg") {
		cout << "[ERROR] Invalid n-tuple type\n";
		exit(1);
	}

	// Set flags
	bool run_TT = false;
	bool run_ST = false;
	if (nttype == "TT" || nttype == "bkg") run_TT = true;
	if (nttype == "ST" || nttype == "bkg") run_ST = true;

	// Read input tree
	cout << "[INFO] Reading event file: " << fin1_name << endl;
	TFile *fin1 = new TFile( fin1_name );
	TTree *Tin1 = (TTree*)fin1->Get( Tin1_name );
	cout << "[INFO] Reading reco file: " << fin2_name << endl;
	TFile *fin2 = new TFile( fin2_name, "update" );
	TTree *Tin2 = (TTree*)fin2->Get( Tin2_name );
	TTree *Tin2_ = Tin2->CloneTree(0);

	// Tree variables
	// Tree 1
	vector<float> *Elec_E = 0;
	vector<float> *Muon_E = 0;
	float met_Px = 0;
	float met_Py = 0;
	// Tree 2
	int TT_lep_idx = 0;
	int TT_lep_ID = 0;
	float TT_lep_Pt = 0;
	float TT_lep_Eta = 0;
	float TT_lep_Phi = 0;
	float TT_lep_E = 0;
	int ST_lep_idx = 0;
	int ST_lep_ID = 0;
	float ST_lep_Pt = 0;
	float ST_lep_Eta = 0;
	float ST_lep_Phi = 0;
	float ST_lep_E = 0;
	// Tree 2 additional
	float TT_Nu_Pz[2] = {0};
	int TT_Nu_Pz_real = 0;
	float ST_Nu_Pz[2] = {0};
	int ST_Nu_Pz_real = 0;

	// Set tree branches
	// Tree 1
	Tin1->SetBranchStatus("*", 0);
	Tin1->SetBranchStatus("ElecInfo.Energy", 1);
	Tin1->SetBranchStatus("MuonInfo.Energy", 1);
	Tin1->SetBranchStatus("MetInfo.Px", 1);
	Tin1->SetBranchStatus("MetInfo.Py", 1);
	Tin1->SetBranchAddress("ElecInfo.Energy", &Elec_E);
	Tin1->SetBranchAddress("MuonInfo.Energy", &Muon_E);
	Tin1->SetBranchAddress("MetInfo.Px", &met_Px);
	Tin1->SetBranchAddress("MetInfo.Py", &met_Py);
	// Tree 2
	if (run_TT) {
		Tin2->SetBranchAddress("TT_lep_idx", &TT_lep_idx);
		Tin2->SetBranchAddress("TT_lep_ID", &TT_lep_ID);
		Tin2->SetBranchAddress("TT_lep_Pt", &TT_lep_Pt);
		Tin2->SetBranchAddress("TT_lep_Eta", &TT_lep_Eta);
		Tin2->SetBranchAddress("TT_lep_Phi", &TT_lep_Phi);
	}
	if (run_ST) {
		Tin2->SetBranchAddress("ST_lep_idx", &ST_lep_idx);
		Tin2->SetBranchAddress("ST_lep_ID", &ST_lep_ID);
		Tin2->SetBranchAddress("ST_lep_Pt", &ST_lep_Pt);
		Tin2->SetBranchAddress("ST_lep_Eta", &ST_lep_Eta);
		Tin2->SetBranchAddress("ST_lep_Phi", &ST_lep_Phi);
	}
	// Tree 2 additional
	Tin2_->Branch("TT_Nu_Pz_0", &TT_Nu_Pz[0]);
	Tin2_->Branch("TT_Nu_Pz_1", &TT_Nu_Pz[1]);
	Tin2_->Branch("TT_Nu_Pz_real", &TT_Nu_Pz_real);
	Tin2_->Branch("ST_Nu_Pz_0", &ST_Nu_Pz[0]);
	Tin2_->Branch("ST_Nu_Pz_1", &ST_Nu_Pz[1]);
	Tin2_->Branch("ST_Nu_Pz_real", &ST_Nu_Pz_real);

	TLorentzVector lep;

	// Start event loop
	for (int evt=0; evt<Tin1->GetEntries(); ++evt)
	{
		Tin1->GetEntry(evt);
		Tin2->GetEntry(evt);

		TT_Nu_Pz[0] = -99999;
		TT_Nu_Pz[1] = -99999;
		TT_Nu_Pz_real = -1;
		ST_Nu_Pz[0] = -99999;
		ST_Nu_Pz[1] = -99999;
		ST_Nu_Pz_real = -1;

		if (run_TT && TT_lep_idx>=0) {
			if ( fabs(TT_lep_ID)==11 ) TT_lep_E = Elec_E->at( TT_lep_idx );
			else TT_lep_E = Muon_E->at( TT_lep_idx );
			lep.SetPtEtaPhiE( TT_lep_Pt, TT_lep_Eta, TT_lep_Phi, TT_lep_E );
			TT_Nu_Pz_real = CalcNuPz( lep, met_Px, met_Py, TT_Nu_Pz );
		}
		if (run_ST && ST_lep_idx>=0) {
			if ( fabs(ST_lep_ID)==11 ) ST_lep_E = Elec_E->at( ST_lep_idx );
			else ST_lep_E = Muon_E->at( ST_lep_idx );
			lep.SetPtEtaPhiE( ST_lep_Pt, ST_lep_Eta, ST_lep_Phi, ST_lep_E );
			ST_Nu_Pz_real = CalcNuPz( lep, met_Px, met_Py, ST_Nu_Pz );
		}

		Tin2_->Fill();
	} // End of event loop

	fin1->Close();
	fin2->WriteTObject(Tin2_);
	fin2->Close();
	cout << "[INFO] Nu Pz info is saved in original reco file\n";

	return 0;
}

int CalcNuPz(TLorentzVector &lep, float met_Px, float met_Py, float *nu_Pz)
{
	float lep_Px = lep.Px();
	float lep_Py = lep.Py();
	float lep_Pz = lep.Pz();
	float lep_Pt = lep.Pt();
	float lep_E = lep.Energy();

	float A = pow(W_mass, 2)/2 + met_Px * lep_Px + met_Py * lep_Py;
	float D = 4 * ( pow(A,2) * (pow(lep_Pt,2) + pow(lep_Pz,2)) - pow(lep_E,2) * pow(lep_Pt,2) * (pow(met_Px,2) + pow(met_Py,2)) );

	if (D >= 0) {
		nu_Pz[0] = ( 2*A*lep_Pz + sqrt(D) ) / ( 2*pow(lep_Pt,2) );
		nu_Pz[1] = ( 2*A*lep_Pz - sqrt(D) ) / ( 2*pow(lep_Pt,2) );
		return 1;
	} else {
		nu_Pz[0] = ( 2*A*lep_Pz ) / ( 2*pow(lep_Pt,2) );
		nu_Pz[1] = nu_Pz[0];
		return 0;
	}
}
