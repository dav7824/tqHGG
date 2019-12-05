#include "HistVar.h"
#include "TreeReader.h"
#include "ScaleFactor.h"

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
using namespace std;

void FillHist(TreeReader &inTree, TH1D **hist, const string &type)
{
    vector<TFile*> f_SF;
    vector<TH2*> h_SF;
    PrepareSF(f_SF, h_SF);

    long nevt = inTree.GetEntries();
    for (long evt=0; evt<nevt; ++evt) {
	if (evt % 100000 == 0) cout << "Processing event " << evt << "...\n";
	inTree.GetEntry(evt);

	float weight = 0.;
	if (type == "data") weight = 1.;
	else if (type == "mc") {
	    weight = inTree.EvtInfo_genweight;
	    ApplySF(inTree, h_SF, weight);
	}
	else {
	    cout << "[ERROR] Unknown dataset type\n";
	    exit(1);
	}

	hist[elec_N]->Fill(inTree.ElecInfo_Size, weight);
	hist[muon_N]->Fill(inTree.MuonInfo_Size, weight);
	hist[lep_N]->Fill(inTree.ElecInfo_Size + inTree.MuonInfo_Size, weight);

	for (int i=0; i<inTree.ElecInfo_Size; ++i) {
	    hist[elec_charge]->Fill(inTree.ElecInfo_Charge->at(i), weight);
	    hist[elec_pt]->Fill(inTree.ElecInfo_Pt->at(i), weight);
	    hist[elec_eta]->Fill(inTree.ElecInfo_Eta->at(i), weight);
	    hist[elec_phi]->Fill(inTree.ElecInfo_Phi->at(i), weight);
	    hist[elec_E]->Fill(inTree.ElecInfo_Energy->at(i), weight);

	    hist[lep_charge]->Fill(inTree.ElecInfo_Charge->at(i), weight);
	    hist[lep_pt]->Fill(inTree.ElecInfo_Pt->at(i), weight);
	    hist[lep_eta]->Fill(inTree.ElecInfo_Eta->at(i), weight);
	    hist[lep_phi]->Fill(inTree.ElecInfo_Phi->at(i), weight);
	    hist[lep_E]->Fill(inTree.ElecInfo_Energy->at(i), weight);
	}

	for (int i=0; i<inTree.MuonInfo_Size; ++i) {
	    hist[muon_charge]->Fill(inTree.MuonInfo_Charge->at(i), weight);
	    hist[muon_pt]->Fill(inTree.MuonInfo_Pt->at(i), weight);
	    hist[muon_eta]->Fill(inTree.MuonInfo_Eta->at(i), weight);
	    hist[muon_phi]->Fill(inTree.MuonInfo_Phi->at(i), weight);
	    hist[muon_E]->Fill(inTree.MuonInfo_Energy->at(i), weight);

	    hist[lep_charge]->Fill(inTree.MuonInfo_Charge->at(i), weight);
	    hist[lep_pt]->Fill(inTree.MuonInfo_Pt->at(i), weight);
	    hist[lep_eta]->Fill(inTree.MuonInfo_Eta->at(i), weight);
	    hist[lep_phi]->Fill(inTree.MuonInfo_Phi->at(i), weight);
	    hist[lep_E]->Fill(inTree.MuonInfo_Energy->at(i), weight);
	}
    }

    CloseSF(f_SF);
}
