/*
 * Given reconstructed leptonic signal, reconstruct Pz of neutrino.
 * Usage:
 *   ./NuPz <event_file> <reco_file> <reco_type=TT|ST>
 *
 * The new tree containing nu Pz info will overwrite original tree in <reco_file>.
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
	TString fevt_name = argv[1];
	TString freco_name = argv[2];
	TString recotype = argv[3];
	if (recotype!="TT" && recotype!="ST") {
		cout << "[ERROR] Invalid reconstruction type\n";
		exit(1);
	}

	// Read event tree
	cout << "[INFO] Reading event file: " << fevt_name << endl;
	TFile *fevt = new TFile( fevt_name );
	TTree *Tevt = (TTree*)fevt->Get("T");
	// Read reconstruction tree
	cout << "[INFO] Reading reco file: " << freco_name << endl;
	TFile *freco = new TFile( freco_name, "update" );
	TString Treco_name;
	if (recotype=="TT") Treco_name = "Treco_TT";
	else Treco_name = "Treco_ST";
	TTree *Treco = (TTree*)freco->Get(Treco_name);
	TTree *Treco_ = Treco->CloneTree(0); // New tree with additional vars

	// Tree variables
	// Event tree
	vector<float> *Elec_Phi = 0;
	vector<float> *Elec_Energy = 0;
	vector<float> *Muon_Phi = 0;
	vector<float> *Muon_Energy = 0;
	float met_Px = 0;
	float met_Py = 0;
	// Reconstruction tree
	int NPerm = 0;
	int lep_idx = 0;
	float lep_ID = 0;
	float lep_Pt = 0;
	float lep_Eta = 0;
	// Reconstruction tree additional
	/* nu_Pz_real: -1: cannot be reconstructed, 0: D<0, 1: D>=0 */
	int nu_Pz_real = 0;
	/* nu_Pz_arr: [0]=real large solution, [1]=real small solution, [2]=real part of complex solution */
	float nu_Pz_arr[3] = {0.};

	// Set tree branches
	// Event tree
	Tevt->SetBranchStatus("*", 0);
	Tevt->SetBranchStatus("ElecInfo.Phi", 1);
	Tevt->SetBranchStatus("ElecInfo.Energy", 1);
	Tevt->SetBranchStatus("MuonInfo.Phi", 1);
	Tevt->SetBranchStatus("MuonInfo.Energy", 1);
	Tevt->SetBranchStatus("MetInfo.Px", 1);
	Tevt->SetBranchStatus("MetInfo.Py", 1);
	Tevt->SetBranchAddress("ElecInfo.Phi", &Elec_Phi);
	Tevt->SetBranchAddress("ElecInfo.Energy", &Elec_Energy);
	Tevt->SetBranchAddress("MuonInfo.Phi", &Muon_Phi);
	Tevt->SetBranchAddress("MuonInfo.Energy", &Muon_Energy);
	Tevt->SetBranchAddress("MetInfo.Px", &met_Px);
	Tevt->SetBranchAddress("MetInfo.Py", &met_Py);
	// Reconstruction tree
	Treco->SetBranchAddress("NPerm", &NPerm);
	Treco->SetBranchAddress("lep_idx", &lep_idx);
	Treco->SetBranchAddress("lep_ID", &lep_ID);
	Treco->SetBranchAddress("lep_Pt", &lep_Pt);
	Treco->SetBranchAddress("lep_Eta", &lep_Eta);
	// Reconstruction tree additional
	Treco_->Branch("nu_Pz_real", &nu_Pz_real);
	Treco_->Branch("nu_Pz_L", &nu_Pz_arr[0]);
	Treco_->Branch("nu_Pz_S", &nu_Pz_arr[1]);
	Treco_->Branch("nu_Pz_M", &nu_Pz_arr[2]);

	TLorentzVector lep;
	float lep_Phi = 0;
	float lep_E = 0;

	// Start event loop
	for (int evt=0; evt<Tevt->GetEntries(); ++evt)
	{
		Tevt->GetEntry(evt);
		Treco->GetEntry(evt);

		// Initialize output variables
		nu_Pz_real = -1;
		for (int i=0; i<3; ++i) nu_Pz_arr[i] = -99999;

		// For events that can't be reconstructed, fill null values to output
		if (NPerm < 0) { Treco_->Fill();  continue; }

		if ( fabs(lep_ID)==11 ) {
			lep_Phi = Elec_Phi->at( lep_idx );
			lep_E = Elec_Energy->at( lep_idx );
		}
		else {
			lep_Phi = Muon_Phi->at( lep_idx );
			lep_E = Muon_Energy->at( lep_idx );
		}
		lep.SetPtEtaPhiE( lep_Pt, lep_Eta, lep_Phi, lep_E );

		// Calculate neutrino Pz
		nu_Pz_real = CalcNuPz( lep, met_Px, met_Py, nu_Pz_arr );

		Treco_->Fill();
	} // End of event loop

	fevt->Close();
	freco->WriteTObject(Treco_, "", "Overwrite");
	freco->Close();
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
		nu_Pz[2] = ( 2*A*lep_Pz ) / ( 2*pow(lep_Pt,2) );
		return 0;
	}
}
