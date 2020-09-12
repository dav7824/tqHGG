#include "TFile.h"
#include "TH2D.h"
#include "TTree.h"
#include "TString.h"
#include "TLorentzVector.h"

#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

int main(int argc, char **argv)
{
	// Get command line arguments
	TString fin_name = argv[1];
	TString fSF_ID_name = argv[2];
	TString fSF_ISO_name = argv[3];

	// SF file for muon ID
	TFile *fSF_ID = new TFile(fSF_ID_name);
	TH2D *hSF_ID = (TH2D*)fSF_ID->Get("NUM_MediumID_DEN_genTracks_pt_abseta");
	// SF file for muon ISO
	TFile *fSF_ISO = new TFile(fSF_ISO_name);
	TH2D *hSF_ISO = (TH2D*)fSF_ISO->Get("NUM_LooseRelIso_DEN_MediumID_pt_abseta");

	// Read input file
	cout << "[INFO] Input file: " << fin_name << endl;
	TFile *fin = new TFile(fin_name, "update");
	TTree *Tin = (TTree*)fin->Get("T");
	// Input variables
	int Muon_Size = 0;
	vector<float> *Muon_Pt = 0;
	vector<float> *Muon_Eta = 0;
	vector<float> *Muon_Phi = 0;
	vector<float> *Muon_Energy = 0;
	// Set input branches
	Tin->SetBranchStatus("*", 0);
	Tin->SetBranchStatus("MuonInfo.Size", 1);
	Tin->SetBranchStatus("MuonInfo.Pt", 1);
	Tin->SetBranchStatus("MuonInfo.Eta", 1);
	Tin->SetBranchStatus("MuonInfo.Phi", 1);
	Tin->SetBranchStatus("MuonInfo.Energy", 1);
	Tin->SetBranchAddress("MuonInfo.Size", &Muon_Size);
	Tin->SetBranchAddress("MuonInfo.Pt", &Muon_Pt);
	Tin->SetBranchAddress("MuonInfo.Eta", &Muon_Eta);
	Tin->SetBranchAddress("MuonInfo.Phi", &Muon_Phi);
	Tin->SetBranchAddress("MuonInfo.Energy", &Muon_Energy);

	TTree *Tout = new TTree("SF_Muon", "");
	// Output variables
	float SF_Muon_ID = 0;
	float SF_Muon_ISO = 0;
	// Set output branches
	Tout->Branch("ID", &SF_Muon_ID);
	Tout->Branch("ISO", &SF_Muon_ISO);

	TLorentzVector muon_vec;

	// Start event loop
	for (int evt=0; evt<Tin->GetEntries(); ++evt)
	{
		Tin->GetEntry(evt);

		SF_Muon_ID = 1.;
		SF_Muon_ISO = 1.;

		// Calculate SF for each muon
		// Start muon loop
		for (int i=0; i<Muon_Size; ++i) {
			// If the muon has Pt in these ranges, take its SF as 1.  (It's outside the SF histograms)
			if (Muon_Pt->at(i) <= 20 || Muon_Pt->at(i) >= 120) continue;
			muon_vec.SetPtEtaPhiE(Muon_Pt->at(i), Muon_Eta->at(i), Muon_Phi->at(i), Muon_Energy->at(i));

			SF_Muon_ID *= hSF_ID->GetBinContent( hSF_ID->FindBin(Muon_Pt->at(i), fabs(Muon_Eta->at(i))) );
			SF_Muon_ISO *= hSF_ISO->GetBinContent( hSF_ISO->FindBin(Muon_Pt->at(i), fabs(Muon_Eta->at(i))) );
		}
		// End muon loop

		Tout->Fill();
	}
	// End event loop

	fin->WriteTObject(Tout, "", "Overwrite");
	fin->Close();
	fSF_ID->Close();
	fSF_ISO->Close();

	cout << "[INFO] Calculated SF tree is saved in input file\n";

	return 0;
}
