#include "TreeReader.h"
#include "TreeWriter.h"

#include <iostream>
using namespace std;

void PrepareAnalysis(TreeReader &inTree, TreeWriter &outTree)
{
}

void Analyze(TreeReader &inTree, TreeWriter &outTree)
{
    long nevt = inTree.GetEntries();
    for (long evt=0; evt<nevt; ++evt) {
	if (evt % 100000 == 0) cout << "Processing event " << evt << "...\n";
	inTree.GetEntry(evt);

	if (inTree.ElecInfo_Size + inTree.MuonInfo_Size > 0) continue;
	if (inTree.jets_size < 3) continue;

	outTree.EvtInfo_NPu = inTree.EvtInfo_NPu;
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
	outTree.jets_size = inTree.jets_size;
	outTree.JetInfo_Pt = inTree.JetInfo_Pt;
	outTree.JetInfo_Eta = inTree.JetInfo_Eta;
	outTree.JetInfo_Phi = inTree.JetInfo_Phi;
	outTree.JetInfo_Mass = inTree.JetInfo_Mass;
	outTree.JetInfo_Energy = inTree.JetInfo_Energy;
	outTree.JetInfo_PtRaw = inTree.JetInfo_PtRaw;
	outTree.JetInfo_QGL = inTree.JetInfo_QGL;
	outTree.JetInfo_RMS = inTree.JetInfo_RMS;
	outTree.JetInfo_puJetIdMVA = inTree.JetInfo_puJetIdMVA;
	outTree.JetInfo_GenJetMatch = inTree.JetInfo_GenJetMatch;
	outTree.JetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags = inTree.JetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags;
	outTree.JetInfo_pfCombinedMVAV2BJetTags = inTree.JetInfo_pfCombinedMVAV2BJetTags;
	outTree.JetInfo_pfDeepCSVJetTags_probb = inTree.JetInfo_pfDeepCSVJetTags_probb;
	outTree.JetInfo_pfDeepCSVJetTags_probbb = inTree.JetInfo_pfDeepCSVJetTags_probbb;
	outTree.JetInfo_pfDeepCSVJetTags_probc = inTree.JetInfo_pfDeepCSVJetTags_probc;
	outTree.JetInfo_pfDeepCSVJetTags_probudsg = inTree.JetInfo_pfDeepCSVJetTags_probudsg;
	outTree.JetInfo_JECScale = inTree.JetInfo_JECScale;
	outTree.JetInfo_JERScale = inTree.JetInfo_JERScale;
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
