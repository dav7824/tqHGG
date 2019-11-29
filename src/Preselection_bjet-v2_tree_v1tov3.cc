#include "TreeReader.h"
#include "TreeWriter.h"

#include "TLorentzVector.h"

#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

float deepCSV_wp = 0.8001;    // deepCSV tight working point

void PrepareAnalysis(TreeReader &inTree, TreeWriter &outTree)
{
    outTree.nbJetInfo_Pt = new vector<float>();
    outTree.nbJetInfo_Eta = new vector<float>();
    outTree.nbJetInfo_Phi = new vector<float>();
    outTree.nbJetInfo_Mass = new vector<float>();
    outTree.nbJetInfo_Energy = new vector<float>();
    outTree.nbJetInfo_PtRaw = new vector<float>();
    outTree.nbJetInfo_QGL = new vector<float>();
    outTree.nbJetInfo_RMS = new vector<float>();
    outTree.nbJetInfo_puJetIdMVA = new vector<float>();
    outTree.nbJetInfo_GenJetMatch = new vector<bool>();
    outTree.nbJetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags = new vector<float>();
    outTree.nbJetInfo_pfCombinedMVAV2BJetTags = new vector<float>();
    outTree.nbJetInfo_pfDeepCSVJetTags_probb = new vector<float>();
    outTree.nbJetInfo_pfDeepCSVJetTags_probbb = new vector<float>();
    outTree.nbJetInfo_pfDeepCSVJetTags_probc = new vector<float>();
    outTree.nbJetInfo_pfDeepCSVJetTags_probudsg = new vector<float>();
    outTree.nbJetInfo_JECScale = new vector<float>();
    outTree.nbJetInfo_JERScale = new vector<float>();
    //////////
    outTree.bJetInfo_Pt = new vector<float>();
    outTree.bJetInfo_Eta = new vector<float>();
    outTree.bJetInfo_Phi = new vector<float>();
    outTree.bJetInfo_Mass = new vector<float>();
    outTree.bJetInfo_Energy = new vector<float>();
    outTree.bJetInfo_PtRaw = new vector<float>();
    outTree.bJetInfo_QGL = new vector<float>();
    outTree.bJetInfo_RMS = new vector<float>();
    outTree.bJetInfo_puJetIdMVA = new vector<float>();
    outTree.bJetInfo_GenJetMatch = new vector<bool>();
    outTree.bJetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags = new vector<float>();
    outTree.bJetInfo_pfCombinedMVAV2BJetTags = new vector<float>();
    outTree.bJetInfo_pfDeepCSVJetTags_probb = new vector<float>();
    outTree.bJetInfo_pfDeepCSVJetTags_probbb = new vector<float>();
    outTree.bJetInfo_pfDeepCSVJetTags_probc = new vector<float>();
    outTree.bJetInfo_pfDeepCSVJetTags_probudsg = new vector<float>();
    outTree.bJetInfo_JECScale = new vector<float>();
    outTree.bJetInfo_JERScale = new vector<float>();
}

void Analyze(TreeReader &inTree, TreeWriter &outTree)
{
    long nevt = inTree.GetEntries();
    for (long evt=0; evt<nevt; ++evt) {
	if (evt % 100000 == 0) cout << "Processing event " << evt << "...\n";
	inTree.GetEntry(evt);

	outTree.nbjets_size = 0;
	outTree.nbJetInfo_Pt->clear();
	outTree.nbJetInfo_Eta->clear();
	outTree.nbJetInfo_Phi->clear();
	outTree.nbJetInfo_Mass->clear();
	outTree.nbJetInfo_Energy->clear();
	outTree.nbJetInfo_PtRaw->clear();
	outTree.nbJetInfo_QGL->clear();
	outTree.nbJetInfo_RMS->clear();
	outTree.nbJetInfo_puJetIdMVA->clear();
	outTree.nbJetInfo_GenJetMatch->clear();
	outTree.nbJetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags->clear();
	outTree.nbJetInfo_pfCombinedMVAV2BJetTags->clear();
	outTree.nbJetInfo_pfDeepCSVJetTags_probb->clear();
	outTree.nbJetInfo_pfDeepCSVJetTags_probbb->clear();
	outTree.nbJetInfo_pfDeepCSVJetTags_probc->clear();
	outTree.nbJetInfo_pfDeepCSVJetTags_probudsg->clear();
	outTree.nbJetInfo_JECScale->clear();
	outTree.nbJetInfo_JERScale->clear();
	//////////
	outTree.bjets_size = 0;
	outTree.bJetInfo_Pt->clear();
	outTree.bJetInfo_Eta->clear();
	outTree.bJetInfo_Phi->clear();
	outTree.bJetInfo_Mass->clear();
	outTree.bJetInfo_Energy->clear();
	outTree.bJetInfo_PtRaw->clear();
	outTree.bJetInfo_QGL->clear();
	outTree.bJetInfo_RMS->clear();
	outTree.bJetInfo_puJetIdMVA->clear();
	outTree.bJetInfo_GenJetMatch->clear();
	outTree.bJetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags->clear();
	outTree.bJetInfo_pfCombinedMVAV2BJetTags->clear();
	outTree.bJetInfo_pfDeepCSVJetTags_probb->clear();
	outTree.bJetInfo_pfDeepCSVJetTags_probbb->clear();
	outTree.bJetInfo_pfDeepCSVJetTags_probc->clear();
	outTree.bJetInfo_pfDeepCSVJetTags_probudsg->clear();
	outTree.bJetInfo_JECScale->clear();
	outTree.bJetInfo_JERScale->clear();

	for (int i=0; i<inTree.jets_size; ++i) {
	    if (inTree.JetInfo_pfDeepCSVJetTags_probb->at(i) + inTree.JetInfo_pfDeepCSVJetTags_probbb->at(i) > deepCSV_wp) {
		outTree.bjets_size += 1;
		outTree.bJetInfo_Pt->push_back(inTree.JetInfo_Pt->at(i));
		outTree.bJetInfo_Eta->push_back(inTree.JetInfo_Eta->at(i));
		outTree.bJetInfo_Phi->push_back(inTree.JetInfo_Phi->at(i));
		outTree.bJetInfo_Mass->push_back(inTree.JetInfo_Mass->at(i));
		outTree.bJetInfo_Energy->push_back(inTree.JetInfo_Energy->at(i));
		outTree.bJetInfo_PtRaw->push_back(inTree.JetInfo_PtRaw->at(i));
		outTree.bJetInfo_QGL->push_back(inTree.JetInfo_QGL->at(i));
		outTree.bJetInfo_RMS->push_back(inTree.JetInfo_RMS->at(i));
		outTree.bJetInfo_puJetIdMVA->push_back(inTree.JetInfo_puJetIdMVA->at(i));
		outTree.bJetInfo_GenJetMatch->push_back(inTree.JetInfo_GenJetMatch->at(i));
		outTree.bJetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags->push_back(inTree.JetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags->at(i));
		outTree.bJetInfo_pfCombinedMVAV2BJetTags->push_back(inTree.JetInfo_pfCombinedMVAV2BJetTags->at(i));
		outTree.bJetInfo_pfDeepCSVJetTags_probb->push_back(inTree.JetInfo_pfDeepCSVJetTags_probb->at(i));
		outTree.bJetInfo_pfDeepCSVJetTags_probbb->push_back(inTree.JetInfo_pfDeepCSVJetTags_probbb->at(i));
		outTree.bJetInfo_pfDeepCSVJetTags_probc->push_back(inTree.JetInfo_pfDeepCSVJetTags_probc->at(i));
		outTree.bJetInfo_pfDeepCSVJetTags_probudsg->push_back(inTree.JetInfo_pfDeepCSVJetTags_probudsg->at(i));
		outTree.bJetInfo_JECScale->push_back(inTree.JetInfo_JECScale->at(i));
		outTree.bJetInfo_JERScale->push_back(inTree.JetInfo_JERScale->at(i));
	    } else {
		outTree.nbjets_size += 1;
		outTree.nbJetInfo_Pt->push_back(inTree.JetInfo_Pt->at(i));
		outTree.nbJetInfo_Eta->push_back(inTree.JetInfo_Eta->at(i));
		outTree.nbJetInfo_Phi->push_back(inTree.JetInfo_Phi->at(i));
		outTree.nbJetInfo_Mass->push_back(inTree.JetInfo_Mass->at(i));
		outTree.nbJetInfo_Energy->push_back(inTree.JetInfo_Energy->at(i));
		outTree.nbJetInfo_PtRaw->push_back(inTree.JetInfo_PtRaw->at(i));
		outTree.nbJetInfo_QGL->push_back(inTree.JetInfo_QGL->at(i));
		outTree.nbJetInfo_RMS->push_back(inTree.JetInfo_RMS->at(i));
		outTree.nbJetInfo_puJetIdMVA->push_back(inTree.JetInfo_puJetIdMVA->at(i));
		outTree.nbJetInfo_GenJetMatch->push_back(inTree.JetInfo_GenJetMatch->at(i));
		outTree.nbJetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags->push_back(inTree.JetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags->at(i));
		outTree.nbJetInfo_pfCombinedMVAV2BJetTags->push_back(inTree.JetInfo_pfCombinedMVAV2BJetTags->at(i));
		outTree.nbJetInfo_pfDeepCSVJetTags_probb->push_back(inTree.JetInfo_pfDeepCSVJetTags_probb->at(i));
		outTree.nbJetInfo_pfDeepCSVJetTags_probbb->push_back(inTree.JetInfo_pfDeepCSVJetTags_probbb->at(i));
		outTree.nbJetInfo_pfDeepCSVJetTags_probc->push_back(inTree.JetInfo_pfDeepCSVJetTags_probc->at(i));
		outTree.nbJetInfo_pfDeepCSVJetTags_probudsg->push_back(inTree.JetInfo_pfDeepCSVJetTags_probudsg->at(i));
		outTree.nbJetInfo_JECScale->push_back(inTree.JetInfo_JECScale->at(i));
		outTree.nbJetInfo_JERScale->push_back(inTree.JetInfo_JERScale->at(i));
	    }
	} // end of input jet loop

	// require at least 1 b-tagged jet
	if (outTree.bjets_size < 1) continue;

	// set the rest of variables
	outTree.EvtInfo_NPu = (float)inTree.EvtInfo_NPu;    // convert tree_v1-1 to tree_v1-2
	outTree.EvtInfo_NVtx = inTree.EvtInfo_NVtx;
	outTree.EvtInfo_passTrigger = inTree.EvtInfo_passTrigger;
	outTree.EvtInfo_genweight = inTree.EvtInfo_genweight;
	outTree.EvtInfo_Rho = inTree.EvtInfo_Rho;
	outTree.EvtInfo_PVz = inTree.EvtInfo_PVz;
	outTree.EvtInfo_BSsigmaz = inTree.EvtInfo_BSsigmaz;
	//////////
	outTree.DiPhoInfo_mass = inTree.DiPhoInfo_mass;
	outTree.DiPhoInfo_pt = inTree.DiPhoInfo_pt;
	outTree.DiPhoInfo_leadPt = inTree.DiPhoInfo_leadPt;
	outTree.DiPhoInfo_leadEta = inTree.DiPhoInfo_leadEta;
	outTree.DiPhoInfo_leadPhi = inTree.DiPhoInfo_leadPhi;
	outTree.DiPhoInfo_leadE = inTree.DiPhoInfo_leadE;
	outTree.DiPhoInfo_leadsigEOverE = inTree.DiPhoInfo_leadsigEOverE;
	outTree.DiPhoInfo_leadR9 = inTree.DiPhoInfo_leadR9;
	outTree.DiPhoInfo_leadsieie = inTree.DiPhoInfo_leadsieie;
	outTree.DiPhoInfo_leadhoe = inTree.DiPhoInfo_leadhoe;
	outTree.DiPhoInfo_leadIDMVA = inTree.DiPhoInfo_leadIDMVA;
	outTree.DiPhoInfo_leadhasPixelSeed = inTree.DiPhoInfo_leadhasPixelSeed;
	outTree.DiPhoInfo_leadGenMatch = inTree.DiPhoInfo_leadGenMatch;
	outTree.DiPhoInfo_leadGenMatchType = inTree.DiPhoInfo_leadGenMatchType;
	outTree.DiPhoInfo_subleadPt = inTree.DiPhoInfo_subleadPt;
	outTree.DiPhoInfo_subleadEta = inTree.DiPhoInfo_subleadEta;
	outTree.DiPhoInfo_subleadPhi = inTree.DiPhoInfo_subleadPhi;
	outTree.DiPhoInfo_subleadE = inTree.DiPhoInfo_subleadE;
	outTree.DiPhoInfo_subleadsigEOverE = inTree.DiPhoInfo_subleadsigEOverE;
	outTree.DiPhoInfo_subleadR9 = inTree.DiPhoInfo_subleadR9;
	outTree.DiPhoInfo_subleadsieie = inTree.DiPhoInfo_subleadsieie;
	outTree.DiPhoInfo_subleadhoe = inTree.DiPhoInfo_subleadhoe;
	outTree.DiPhoInfo_subleadIDMVA = inTree.DiPhoInfo_subleadIDMVA;
	outTree.DiPhoInfo_subleadhasPixelSeed = inTree.DiPhoInfo_subleadhasPixelSeed;
	outTree.DiPhoInfo_subleadGenMatch = inTree.DiPhoInfo_subleadGenMatch;
	outTree.DiPhoInfo_subleadGenMatchType = inTree.DiPhoInfo_subleadGenMatchType;
	outTree.DiPhoInfo_SelectedVz = inTree.DiPhoInfo_SelectedVz;
	outTree.DiPhoInfo_GenVz = inTree.DiPhoInfo_GenVz;
	outTree.DiPhoInfo_centralWeight = inTree.DiPhoInfo_centralWeight;
	//////////
	outTree.ElecInfo_Size = inTree.ElecInfo_Size;
	outTree.ElecInfo_Charge = inTree.ElecInfo_Charge;
	outTree.ElecInfo_Pt = inTree.ElecInfo_Pt;
	outTree.ElecInfo_Eta = inTree.ElecInfo_Eta;
	outTree.ElecInfo_Phi = inTree.ElecInfo_Phi;
	outTree.ElecInfo_Energy = inTree.ElecInfo_Energy;
	outTree.ElecInfo_EtaSC = inTree.ElecInfo_EtaSC;
	outTree.ElecInfo_PhiSC = inTree.ElecInfo_PhiSC;
	outTree.ElecInfo_EGMCutBasedIDVeto = inTree.ElecInfo_EGMCutBasedIDVeto;
	outTree.ElecInfo_EGMCutBasedIDLoose = inTree.ElecInfo_EGMCutBasedIDLoose;
	outTree.ElecInfo_EGMCutBasedIDMedium = inTree.ElecInfo_EGMCutBasedIDMedium;
	outTree.ElecInfo_EGMCutBasedIDTight = inTree.ElecInfo_EGMCutBasedIDTight;
	outTree.ElecInfo_fggPhoVeto = inTree.ElecInfo_fggPhoVeto;
	//////////
	outTree.MuonInfo_Size = inTree.MuonInfo_Size;
	outTree.MuonInfo_Charge = inTree.MuonInfo_Charge;
	outTree.MuonInfo_MuonType = inTree.MuonInfo_MuonType;
	outTree.MuonInfo_Pt = inTree.MuonInfo_Pt;
	outTree.MuonInfo_Eta = inTree.MuonInfo_Eta;
	outTree.MuonInfo_Phi = inTree.MuonInfo_Phi;
	outTree.MuonInfo_Energy = inTree.MuonInfo_Energy;
	outTree.MuonInfo_BestTrackDz = inTree.MuonInfo_BestTrackDz;
	outTree.MuonInfo_BestTrackDxy = inTree.MuonInfo_BestTrackDxy;
	outTree.MuonInfo_PFIsoDeltaBetaCorrR04 = inTree.MuonInfo_PFIsoDeltaBetaCorrR04;
	outTree.MuonInfo_TrackerBasedIsoR03 = inTree.MuonInfo_TrackerBasedIsoR03;
	outTree.MuonInfo_CutBasedIdMedium = inTree.MuonInfo_CutBasedIdMedium;
	outTree.MuonInfo_CutBasedIdTight = inTree.MuonInfo_CutBasedIdTight;
	//////////
	outTree.MetInfo_Pt = inTree.MetInfo_Pt;
	outTree.MetInfo_Phi = inTree.MetInfo_Phi;
	outTree.MetInfo_Px = inTree.MetInfo_Px;
	outTree.MetInfo_Py = inTree.MetInfo_Py;
	outTree.MetInfo_SumET = inTree.MetInfo_SumET;
	//////////
	outTree.GenPartInfo_size = inTree.GenPartInfo_size;
	outTree.GenPartInfo_Pt = inTree.GenPartInfo_Pt;
	outTree.GenPartInfo_Eta = inTree.GenPartInfo_Eta;
	outTree.GenPartInfo_Phi = inTree.GenPartInfo_Phi;
	outTree.GenPartInfo_Mass = inTree.GenPartInfo_Mass;
	outTree.GenPartInfo_PdgID = inTree.GenPartInfo_PdgID;
	outTree.GenPartInfo_Status = inTree.GenPartInfo_Status;
	outTree.GenPartInfo_nMo = inTree.GenPartInfo_nMo;
	outTree.GenPartInfo_nDa = inTree.GenPartInfo_nDa;

	outTree.Fill();
    }
}
