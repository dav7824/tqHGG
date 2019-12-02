#include "HistVar.h"
#include "TreeReader.h"

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
using namespace std;

void FillHist(TreeReader &inTree, TH1D **hist, const string &type)
{
    TFile *f_egamma_RECO = new TFile("/wk_cms2/mc_cheng/public/tqHGG/2017/SF/egammaEffi.txt_EGM2D_runBCDEF_passingRECO.root");
    TFile *f_egamma_ID = new TFile("/wk_cms2/mc_cheng/public/tqHGG/2017/SF/2017_ElectronMedium.root");
    TFile *f_muon_ID = new TFile("/wk_cms2/mc_cheng/public/tqHGG/2017/SF/RunBCDEF_SF_ID.root");
    TFile *f_muon_ISO = new TFile("/wk_cms2/mc_cheng/public/tqHGG/2017/SF/RunBCDEF_SF_ISO.root");
    TH2F *h_egamma_RECO = (TH2F*)f_egamma_RECO->Get("EGamma_SF2D");
    TH2F *h_egamma_ID = (TH2F*)f_egamma_ID->Get("EGamma_SF2D");
    TH2D *h_muon_ID = (TH2D*)f_muon_ID->Get("NUM_TightID_DEN_genTracks_pt_abseta");
    TH2D *h_muon_ISO = (TH2D*)f_muon_ISO->Get("NUM_LooseRelIso_DEN_TightIDandIPCut_pt_abseta");

    long nevt = inTree.GetEntries();
    for (long evt=0; evt<nevt; ++evt) {
	if (evt % 100000 == 0) cout << "Processing event " << evt << "...\n";
	inTree.GetEntry(evt);

	float weight = 0.;
	if (type == "data") weight = 1.;
	else if (type == "mc") {
	    weight = inTree.EvtInfo_genweight;
	    // electron SFs
	    for (int i=0; i<inTree.ElecInfo_Size; ++i) {
		weight *= h_egamma_RECO->GetBinContent(h_egamma_RECO->FindBin(inTree.ElecInfo_Eta->at(i), inTree.ElecInfo_Pt->at(i)));
		weight *= h_egamma_ID->GetBinContent(h_egamma_ID->FindBin(inTree.ElecInfo_Eta->at(i), inTree.ElecInfo_Pt->at(i)));
	    }
	    // muon SFs
	    for (int i=0; i<inTree.MuonInfo_Size; ++i) {
		weight *= h_muon_ID->GetBinContent(h_muon_ID->FindBin(inTree.MuonInfo_Pt->at(i), fabs(inTree.MuonInfo_Eta->at(i))));
		weight *= h_muon_ISO->GetBinContent(h_muon_ISO->FindBin(inTree.MuonInfo_Pt->at(i), fabs(inTree.MuonInfo_Eta->at(i))));
	    }
	    // photon SFs
	    weight *= inTree.DiPhoInfo_centralWeight;
	}
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

    f_egamma_RECO->Close();
    f_egamma_ID->Close();
    f_muon_ID->Close();
    f_muon_ISO->Close();
}
