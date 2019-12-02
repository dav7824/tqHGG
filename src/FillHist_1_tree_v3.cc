#include "HistVar.h"
#include "TreeReader.h"

#include "TH1D.h"

#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

void FillHist(TreeReader &inTree, TH1D **hist, const string &type)
{
    long nevt = inTree.GetEntries();
    for (long evt=0; evt<nevt; ++evt) {
	if (evt % 100000 == 0) cout << "Processing event " << evt << "...\n";
	inTree.GetEntry(evt);

	float weight = 0.;
	if (type == "data") weight = 1.;
	else if (type == "mc") weight = inTree.EvtInfo_genweight;
	else {
	    cout << "[ERROR] Unknown dataset type\n";
	    exit(1);
	}

	hist[dipho_mass]->Fill(inTree.DiPhoInfo_mass, weight);
	hist[dipho_pt]->Fill(inTree.DiPhoInfo_pt, weight);

	hist[pho1_pt]->Fill(inTree.DiPhoInfo_leadPt, weight);
	hist[pho1_eta]->Fill(inTree.DiPhoInfo_leadEta, weight);
	hist[pho1_phi]->Fill(inTree.DiPhoInfo_leadPhi, weight);
	hist[pho1_E]->Fill(inTree.DiPhoInfo_leadE, weight);
	hist[pho1_idmva]->Fill(inTree.DiPhoInfo_leadIDMVA, weight);

	hist[pho2_pt]->Fill(inTree.DiPhoInfo_subleadPt, weight);
	hist[pho2_eta]->Fill(inTree.DiPhoInfo_subleadEta, weight);
	hist[pho2_phi]->Fill(inTree.DiPhoInfo_subleadPhi, weight);
	hist[pho2_E]->Fill(inTree.DiPhoInfo_subleadE, weight);
	hist[pho2_idmva]->Fill(inTree.DiPhoInfo_subleadIDMVA, weight);

	hist[jet_N]->Fill(inTree.nbjets_size + inTree.bjets_size, weight);
	for (int j=0; j<inTree.nbjets_size; ++j) {
	    hist[jet_pt]->Fill(inTree.nbJetInfo_Pt->at(j), weight);
	    hist[jet_eta]->Fill(inTree.nbJetInfo_Eta->at(j), weight);
	    hist[jet_phi]->Fill(inTree.nbJetInfo_Phi->at(j), weight);
	    hist[jet_E]->Fill(inTree.nbJetInfo_Energy->at(j), weight);
	    hist[jet_M]->Fill(inTree.nbJetInfo_Mass->at(j), weight);
	}
	for (int j=0; j<inTree.bjets_size; ++j) {
	    hist[jet_pt]->Fill(inTree.bJetInfo_Pt->at(j), weight);
	    hist[jet_eta]->Fill(inTree.bJetInfo_Eta->at(j), weight);
	    hist[jet_phi]->Fill(inTree.bJetInfo_Phi->at(j), weight);
	    hist[jet_E]->Fill(inTree.bJetInfo_Energy->at(j), weight);
	    hist[jet_M]->Fill(inTree.bJetInfo_Mass->at(j), weight);
	}

	hist[met_pt]->Fill(inTree.MetInfo_Pt, weight);
	hist[met_phi]->Fill(inTree.MetInfo_Phi, weight);
	hist[met_px]->Fill(inTree.MetInfo_Px, weight);
	hist[met_py]->Fill(inTree.MetInfo_Py, weight);
	hist[met_SumET]->Fill(inTree.MetInfo_SumET, weight);
    }
}
