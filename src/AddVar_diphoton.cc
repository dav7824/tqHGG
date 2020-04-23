// Create a new file that contains a tree with new variables derived from input tree.
// The output tree can be used as friend of the original tree in other applications.

#include "TString.h"
#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"

#include <iostream>
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
	TTree *outTree = new TTree("flashggStdTree_diphoton", "");

	// Set input tree branches
	float DiPhoInfo_leadPt = 0;
	float DiPhoInfo_leadEta = 0;
	float DiPhoInfo_leadPhi = 0;
	float DiPhoInfo_leadE = 0;
	float DiPhoInfo_subleadPt = 0;
	float DiPhoInfo_subleadEta = 0;
	float DiPhoInfo_subleadPhi = 0;
	float DiPhoInfo_subleadE = 0;
	inTree->SetBranchAddress("DiPhoInfo.leadPt", &DiPhoInfo_leadPt);
	inTree->SetBranchAddress("DiPhoInfo.leadEta", &DiPhoInfo_leadEta);
	inTree->SetBranchAddress("DiPhoInfo.leadPhi", &DiPhoInfo_leadPhi);
	inTree->SetBranchAddress("DiPhoInfo.leadE", &DiPhoInfo_leadE);
	inTree->SetBranchAddress("DiPhoInfo.subleadPt", &DiPhoInfo_subleadPt);
	inTree->SetBranchAddress("DiPhoInfo.subleadEta", &DiPhoInfo_subleadEta);
	inTree->SetBranchAddress("DiPhoInfo.subleadPhi", &DiPhoInfo_subleadPhi);
	inTree->SetBranchAddress("DiPhoInfo.subleadE", &DiPhoInfo_subleadE);
	// Set output tree branches
	float DiPhoInfo_Eta = 0;
	float DiPhoInfo_Phi = 0;
	float DiPhoInfo_Energy = 0;
	float DiPhoInfo_Theta = 0;
	outTree->Branch("DiPhoInfo.Eta", &DiPhoInfo_Eta);
	outTree->Branch("DiPhoInfo.Phi", &DiPhoInfo_Phi);
	outTree->Branch("DiPhoInfo.Energy", &DiPhoInfo_Energy);
	outTree->Branch("DiPhoInfo.Theta", &DiPhoInfo_Theta);

	// Process all events
	TLorentzVector leadPho, subleadPho, diphoton;
	cout << "Processing events...\n";
	for (long long evt=0; evt<inTree->GetEntries(); ++evt)
	{
		inTree->GetEntry(evt);

		leadPho.SetPtEtaPhiE(DiPhoInfo_leadPt, DiPhoInfo_leadEta, DiPhoInfo_leadPhi, DiPhoInfo_leadE);
		subleadPho.SetPtEtaPhiE(DiPhoInfo_subleadPt, DiPhoInfo_subleadEta, DiPhoInfo_subleadPhi, DiPhoInfo_subleadE);
		diphoton = leadPho + subleadPho;

		DiPhoInfo_Eta = diphoton.Eta();
		DiPhoInfo_Phi = diphoton.Phi();
		DiPhoInfo_Energy = diphoton.Energy();
		DiPhoInfo_Theta = leadPho.DeltaR(subleadPho);

		outTree->Fill();
	}

	// Write output TTree to output TFile
	outTree->Write();
	// Close output TFile
	fout->Close();

	cout << "Completed!\n";

	return 0;
}
