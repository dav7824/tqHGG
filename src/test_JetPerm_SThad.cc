#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TH1D.h"

#include <iostream>
#include <cstdio>
#include <vector>
using namespace std;

int main(int argc, char **argv)
{
	TString dir = "/wk_cms2/mc_cheng/public/tqHGG/2017/MVAreco_GenPerm/";
	TFile *fin = new TFile(dir+"ST_FCNC-TH_Thad_hct.root");
	TTree *T = (TTree*)fin->Get("T");
	T->AddFriend("TGenPart");

	TFile *fout = new TFile(dir+"test_important_jet_SThad/ST_FCNC-TH_Thad_hct.root", "recreate");
	TH1D *h_bjet = new TH1D("h_bjet", ";b-jet idx;", 10, -0.5, 9.5);
	//TH1D *h_ljet = new TH1D("h_ljet", ";fcnc jet idx;", 10, -0.5, 9.5);
	TH1D *h_wjet1 = new TH1D("h_wjet1", ";W jet1 idx;", 10, -0.5, 9.5);
	TH1D *h_wjet2 = new TH1D("h_wjet2", ";W jet2 idx;", 10, -0.5, 9.5);

	int DiPho_leadGenMatchType = 0;
	int DiPho_subleadGenMatchType = 0;
	int jets_size = 0;
	vector<float> *Jet_Pt = 0;
	vector<float> *Jet_Eta = 0;
	vector<float> *Jet_Phi = 0;
	vector<float> *Jet_Energy = 0;
	int GenPart_size = 0;
	vector<float> *GenPart_Pt = 0;
	vector<float> *GenPart_Eta = 0;
	vector<float> *GenPart_Phi = 0;
	vector<float> *GenPart_Mass = 0;
	int Idx_bq = 0;
	//int Idx_lq = 0;
	int Idx_Wq1 = 0;
	int Idx_Wq2 = 0;

	T->SetBranchStatus("*", 0);
	T->SetBranchStatus("DiPhoInfo.leadGenMatchType", 1);
	T->SetBranchStatus("DiPhoInfo.subleadGenMatchType", 1);
	T->SetBranchStatus("jets_size", 1);
	T->SetBranchStatus("JetInfo.Pt", 1);
	T->SetBranchStatus("JetInfo.Eta", 1);
	T->SetBranchStatus("JetInfo.Phi", 1);
	T->SetBranchStatus("JetInfo.Energy", 1);
	T->SetBranchStatus("GenPartInfo.size", 1);
	T->SetBranchStatus("GenPartInfo.Pt", 1);
	T->SetBranchStatus("GenPartInfo.Eta", 1);
	T->SetBranchStatus("GenPartInfo.Phi", 1);
	T->SetBranchStatus("GenPartInfo.Mass", 1);
	T->SetBranchStatus("Idx_bq", 1);
	//T->SetBranchStatus("Idx_lq", 1);
	T->SetBranchStatus("Idx_Wq1", 1);
	T->SetBranchStatus("Idx_Wq2", 1);
	T->SetBranchAddress("DiPhoInfo.leadGenMatchType", &DiPho_leadGenMatchType);
	T->SetBranchAddress("DiPhoInfo.subleadGenMatchType", &DiPho_subleadGenMatchType);
	T->SetBranchAddress("jets_size", &jets_size);
	T->SetBranchAddress("JetInfo.Pt", &Jet_Pt);
	T->SetBranchAddress("JetInfo.Eta", &Jet_Eta);
	T->SetBranchAddress("JetInfo.Phi", &Jet_Phi);
	T->SetBranchAddress("JetInfo.Energy", &Jet_Energy);
	T->SetBranchAddress("GenPartInfo.size", &GenPart_size);
	T->SetBranchAddress("GenPartInfo.Pt", &GenPart_Pt);
	T->SetBranchAddress("GenPartInfo.Eta", &GenPart_Eta);
	T->SetBranchAddress("GenPartInfo.Phi", &GenPart_Phi);
	T->SetBranchAddress("GenPartInfo.Mass", &GenPart_Mass);
	T->SetBranchAddress("Idx_bq", &Idx_bq);
	//T->SetBranchAddress("Idx_lq", &Idx_lq);
	T->SetBranchAddress("Idx_Wq1", &Idx_Wq1);
	T->SetBranchAddress("Idx_Wq2", &Idx_Wq2);

	TLorentzVector bq, Wq1, Wq2;
	TLorentzVector bjet, Wjet1, Wjet2;

	double dR_cut = 0.4;

	int Nevt_recoable = 0;
	int Nevt_normal = 0;
	int Nevt_highmulti = 0;
	int Nevt_match = 0;
	int Nevt_manymatch = 0;
	int Nevt_unmatch = 0;

	int Nsamples = 100000;

	// Event loop
	for (int evt=0; evt<Nsamples; ++evt)
	{
		if (evt % 10000 == 0) cout << "Processing event " << evt << endl;

		T->GetEntry(evt);
		if (jets_size<3) continue;
		Nevt_recoable += 1;

		int njet_ = 0;
		if (jets_size<=15) { Nevt_normal += 1;  njet_ = jets_size; }
		else { Nevt_highmulti += 1;  njet_ = 15; }

		bq.SetPtEtaPhiM(GenPart_Pt->at(Idx_bq), GenPart_Eta->at(Idx_bq), GenPart_Phi->at(Idx_bq), GenPart_Mass->at(Idx_bq));
		//lq.SetPtEtaPhiM(GenPart_Pt->at(Idx_lq), GenPart_Eta->at(Idx_lq), GenPart_Phi->at(Idx_lq), GenPart_Mass->at(Idx_lq));
		Wq1.SetPtEtaPhiM(GenPart_Pt->at(Idx_Wq1), GenPart_Eta->at(Idx_Wq1), GenPart_Phi->at(Idx_Wq1), GenPart_Mass->at(Idx_Wq1));
		Wq2.SetPtEtaPhiM(GenPart_Pt->at(Idx_Wq2), GenPart_Eta->at(Idx_Wq2), GenPart_Phi->at(Idx_Wq2), GenPart_Mass->at(Idx_Wq2));

		int Nperm_matched = 0;

		// b-jet loop
		for (int i=0; i<njet_; ++i) {
			// W jet 1 loop
			for (int k=0; k<njet_-1; ++k) {
				if (k == i) continue;
				// W jet 2 loop
				for (int l=k+1; l<njet_; ++l) {
					if (l == i) continue;

					bjet.SetPtEtaPhiE(Jet_Pt->at(i), Jet_Eta->at(i), Jet_Phi->at(i), Jet_Energy->at(i));
					//ljet.SetPtEtaPhiE(Jet_Pt->at(j), Jet_Eta->at(j), Jet_Phi->at(j), Jet_Energy->at(j));
					Wjet1.SetPtEtaPhiE(Jet_Pt->at(k), Jet_Eta->at(k), Jet_Phi->at(k), Jet_Energy->at(k));
					Wjet2.SetPtEtaPhiE(Jet_Pt->at(l), Jet_Eta->at(l), Jet_Phi->at(l), Jet_Energy->at(l));

					bool matched = false;
					bool W_matched = (Wjet1.DeltaR(Wq1)<dR_cut && Wjet2.DeltaR(Wq2)<dR_cut) || (Wjet1.DeltaR(Wq2)<dR_cut && Wjet2.DeltaR(Wq1)<dR_cut);
					if (bjet.DeltaR(bq)<dR_cut && W_matched
							&& DiPho_leadGenMatchType==1 && DiPho_subleadGenMatchType==1)
						matched = true;

					if (matched) {
						h_bjet->Fill(i);
						//h_ljet->Fill(j);
						h_wjet1->Fill(k);
						h_wjet2->Fill(l);
						Nperm_matched += 1;
					} else {
						h_bjet->Fill(-1);
						//h_ljet->Fill(-1);
						h_wjet1->Fill(-1);
						h_wjet2->Fill(-1);
					}
				} // End W jet 2 loop
			} // End W jet 1 loop
		} // End b-jet loop

		if (Nperm_matched == 1) Nevt_match += 1;
		else if (Nperm_matched == 0) Nevt_unmatch += 1;
		else Nevt_manymatch += 1;
	} // End event loop

	fout->Write();
	fout->Close();
	fin->Close();

	FILE *output = fopen((dir+"test_important_jet_SThad/ST_FCNC-TH_Thad_hct.txt").Data(), "w");
	fprintf(output, "Samples\t%d\n", Nsamples);
	fprintf(output, "Can be reco          %d  ->  %f\n", Nevt_recoable, (float)Nevt_recoable/Nsamples);
	fprintf(output, "Njet = 3~15          %d  ->  %f\n", Nevt_normal, (float)Nevt_normal/Nsamples);
	fprintf(output, "Njet > 15            %d  ->  %f\n", Nevt_highmulti, (float)Nevt_highmulti/Nsamples);
	fprintf(output, "1 matched perm       %d  ->  %f\n", Nevt_match, (float)Nevt_match/Nsamples);
	fprintf(output, "no matched perm      %d  ->  %f\n", Nevt_unmatch, (float)Nevt_unmatch/Nsamples);
	fprintf(output, "many matched perms   %d  ->  %f\n", Nevt_manymatch, (float)Nevt_manymatch/Nsamples);
	fclose(output);

	cout << "Completed!\n";

	return 0;
}
