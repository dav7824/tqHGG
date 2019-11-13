#include "TreeReader.h"
#include "TreeWriter.h"

#include "TLorentzVector.h"

#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

void PrepareAnalysis(TreeReader &inTree, TreeWriter &outTree)
{
    outTree.ElecInfo_Charge = new vector<int>();
    outTree.ElecInfo_Pt = new vector<float>();
    outTree.ElecInfo_Eta = new vector<float>();
    outTree.ElecInfo_Phi = new vector<float>();
    outTree.ElecInfo_Energy = new vector<float>();
    outTree.ElecInfo_EtaSC = new vector<float>();
    outTree.ElecInfo_PhiSC = new vector<float>();
    outTree.ElecInfo_EGMCutBasedIDVeto = new vector<bool>();
    outTree.ElecInfo_EGMCutBasedIDLoose = new vector<bool>();
    outTree.ElecInfo_EGMCutBasedIDMedium = new vector<bool>();
    outTree.ElecInfo_EGMCutBasedIDTight = new vector<bool>();
    outTree.ElecInfo_fggPhoVeto = new vector<bool>();
    //////////
    outTree.MuonInfo_Charge = new vector<int>();
    outTree.MuonInfo_MuonType = new vector<float>();
    outTree.MuonInfo_Pt = new vector<float>();
    outTree.MuonInfo_Eta = new vector<float>();
    outTree.MuonInfo_Phi = new vector<float>();
    outTree.MuonInfo_Energy = new vector<float>();
    outTree.MuonInfo_BestTrackDz = new vector<float>();
    outTree.MuonInfo_BestTrackDxy = new vector<float>();
    outTree.MuonInfo_PFIsoDeltaBetaCorrR04 = new vector<float>();
    outTree.MuonInfo_TrackerBasedIsoR03 = new vector<float>();
    outTree.MuonInfo_CutBasedIdMedium = new vector<bool>();
    outTree.MuonInfo_CutBasedIdTight = new vector<bool>();
    //////////
    outTree.JetInfo_Pt = new vector<float>();
    outTree.JetInfo_Eta = new vector<float>();
    outTree.JetInfo_Phi = new vector<float>();
    outTree.JetInfo_Mass = new vector<float>();
    outTree.JetInfo_Energy = new vector<float>();
    outTree.JetInfo_PtRaw = new vector<float>();
    outTree.JetInfo_QGL = new vector<float>();
    outTree.JetInfo_RMS = new vector<float>();
    outTree.JetInfo_puJetIdMVA = new vector<float>();
    outTree.JetInfo_GenJetMatch = new vector<bool>();
    outTree.JetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags = new vector<float>();
    outTree.JetInfo_pfCombinedMVAV2BJetTags = new vector<float>();
    outTree.JetInfo_pfDeepCSVJetTags_probb = new vector<float>();
    outTree.JetInfo_pfDeepCSVJetTags_probbb = new vector<float>();
    outTree.JetInfo_pfDeepCSVJetTags_probc = new vector<float>();
    outTree.JetInfo_pfDeepCSVJetTags_probudsg = new vector<float>();
    outTree.JetInfo_JECScale = new vector<float>();
    outTree.JetInfo_JERScale = new vector<float>();
}

void Analyze(TreeReader &inTree, TreeWriter &outTree)
{
    long nevt = inTree.GetEntries();
    for (long evt=0; evt<nevt; ++evt) {
	if (evt % 100000 == 0) cout << "Processing event " << evt << "...\n";
	inTree.GetEntry(evt);

	if (inTree.DiPhoInfo_mass < 100 || inTree.DiPhoInfo_mass > 180) continue;
	if (inTree.DiPhoInfo_mass > 120 && inTree.DiPhoInfo_mass < 130) continue;

	if (!inTree.EvtInfo_passTrigger) continue;
	if (inTree.DiPhoInfo_leadPt < 35 || inTree.DiPhoInfo_subleadPt < 25) continue;
	if (fabs(inTree.DiPhoInfo_leadEta) > 2.5) continue;
	if (fabs(inTree.DiPhoInfo_subleadEta) > 2.5) continue;
	if (fabs(inTree.DiPhoInfo_leadEta) > 1.4442 && fabs(inTree.DiPhoInfo_leadEta) < 1.566) continue;
	if (fabs(inTree.DiPhoInfo_subleadEta) > 1.4442 && fabs(inTree.DiPhoInfo_subleadEta) < 1.566) continue;

	outTree.ElecInfo_Size = 0;
	outTree.ElecInfo_Charge->clear();
	outTree.ElecInfo_Pt->clear();
	outTree.ElecInfo_Eta->clear();
	outTree.ElecInfo_Phi->clear();
	outTree.ElecInfo_Energy->clear();
	outTree.ElecInfo_EtaSC->clear();
	outTree.ElecInfo_PhiSC->clear();
	outTree.ElecInfo_EGMCutBasedIDVeto->clear();
	outTree.ElecInfo_EGMCutBasedIDLoose->clear();
	outTree.ElecInfo_EGMCutBasedIDMedium->clear();
	outTree.ElecInfo_EGMCutBasedIDTight->clear();
	outTree.ElecInfo_fggPhoVeto->clear();
	//////////
	outTree.MuonInfo_Size = 0;
	outTree.MuonInfo_Charge->clear();
	outTree.MuonInfo_MuonType->clear();
	outTree.MuonInfo_Pt->clear();
	outTree.MuonInfo_Eta->clear();
	outTree.MuonInfo_Phi->clear();
	outTree.MuonInfo_Energy->clear();
	outTree.MuonInfo_BestTrackDz->clear();
	outTree.MuonInfo_BestTrackDxy->clear();
	outTree.MuonInfo_PFIsoDeltaBetaCorrR04->clear();
	outTree.MuonInfo_TrackerBasedIsoR03->clear();
	outTree.MuonInfo_CutBasedIdMedium->clear();
	outTree.MuonInfo_CutBasedIdTight->clear();
	//////////
	outTree.jets_size = 0;
	outTree.JetInfo_Pt->clear();
	outTree.JetInfo_Eta->clear();
	outTree.JetInfo_Phi->clear();
	outTree.JetInfo_Mass->clear();
	outTree.JetInfo_Energy->clear();
	outTree.JetInfo_PtRaw->clear();
	outTree.JetInfo_QGL->clear();
	outTree.JetInfo_RMS->clear();
	outTree.JetInfo_puJetIdMVA->clear();
	outTree.JetInfo_GenJetMatch->clear();
	outTree.JetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags->clear();
	outTree.JetInfo_pfCombinedMVAV2BJetTags->clear();
	outTree.JetInfo_pfDeepCSVJetTags_probb->clear();
	outTree.JetInfo_pfDeepCSVJetTags_probbb->clear();
	outTree.JetInfo_pfDeepCSVJetTags_probc->clear();
	outTree.JetInfo_pfDeepCSVJetTags_probudsg->clear();
	outTree.JetInfo_JECScale->clear();
	outTree.JetInfo_JERScale->clear();

	// create 4-vector of leading photon and subleading photon
	TLorentzVector leadPho, subleadPho;
	leadPho.SetPtEtaPhiE(inTree.DiPhoInfo_leadPt, inTree.DiPhoInfo_leadEta, inTree.DiPhoInfo_leadPhi, inTree.DiPhoInfo_leadE);
	subleadPho.SetPtEtaPhiE(inTree.DiPhoInfo_subleadPt, inTree.DiPhoInfo_subleadEta, inTree.DiPhoInfo_subleadPhi, inTree.DiPhoInfo_subleadE);

	vector<TLorentzVector*> electrons;
	vector<TLorentzVector*> muons;

	// electron preselection
	for (int i=0; i<inTree.ElecInfo_Size; ++i) {
	    if (!inTree.ElecInfo_EGMCutBasedIDMedium->at(i)) continue;
	    if (inTree.ElecInfo_Pt->at(i) < 20) continue;
	    if (fabs(inTree.ElecInfo_Eta->at(i)) > 2.4) continue;
	    if (fabs(inTree.ElecInfo_Eta->at(i)) > 1.4442 && fabs(inTree.ElecInfo_Eta->at(i)) < 1.566) continue;

	    TLorentzVector *electron_tmp = new TLorentzVector();
	    electron_tmp->SetPtEtaPhiE(inTree.ElecInfo_Pt->at(i), inTree.ElecInfo_Eta->at(i), inTree.ElecInfo_Phi->at(i), inTree.ElecInfo_Energy->at(i));
	    if (electron_tmp->DeltaR(leadPho) < 0.3 || electron_tmp->DeltaR(subleadPho) < 0.3) { delete electron_tmp; continue; }
	    // save the electron data which pass preselection

	    outTree.ElecInfo_Size += 1;
	    outTree.ElecInfo_Charge->push_back(inTree.ElecInfo_Charge->at(i));
	    outTree.ElecInfo_Pt->push_back(inTree.ElecInfo_Pt->at(i));
	    outTree.ElecInfo_Eta->push_back(inTree.ElecInfo_Eta->at(i));
	    outTree.ElecInfo_Phi->push_back(inTree.ElecInfo_Phi->at(i));
	    outTree.ElecInfo_Energy->push_back(inTree.ElecInfo_Energy->at(i));
	    outTree.ElecInfo_EtaSC->push_back(inTree.ElecInfo_EtaSC->at(i));
	    outTree.ElecInfo_PhiSC->push_back(inTree.ElecInfo_PhiSC->at(i));
	    outTree.ElecInfo_EGMCutBasedIDVeto->push_back(inTree.ElecInfo_EGMCutBasedIDVeto->at(i));
	    outTree.ElecInfo_EGMCutBasedIDLoose->push_back(inTree.ElecInfo_EGMCutBasedIDLoose->at(i));
	    outTree.ElecInfo_EGMCutBasedIDMedium->push_back(inTree.ElecInfo_EGMCutBasedIDMedium->at(i));
	    outTree.ElecInfo_EGMCutBasedIDTight->push_back(inTree.ElecInfo_EGMCutBasedIDTight->at(i));
	    outTree.ElecInfo_fggPhoVeto->push_back(inTree.ElecInfo_fggPhoVeto->at(i));

	    electrons.push_back(electron_tmp);
	} // end of electron loop

	// muon preselection
	for (int i=0; i<inTree.MuonInfo_Size; ++i) {
	    if (!inTree.MuonInfo_CutBasedIdTight->at(i)) continue;
	    if (inTree.MuonInfo_Pt->at(i) < 20) continue;
	    if (fabs(inTree.MuonInfo_Eta->at(i)) > 2.4) continue;
	    if (inTree.MuonInfo_PFIsoDeltaBetaCorrR04->at(i) > 0.25) continue;

	    TLorentzVector *muon_tmp = new TLorentzVector();
	    muon_tmp->SetPtEtaPhiE(inTree.MuonInfo_Pt->at(i), inTree.MuonInfo_Eta->at(i), inTree.MuonInfo_Phi->at(i), inTree.MuonInfo_Energy->at(i));
	    if (muon_tmp->DeltaR(leadPho) < 0.3 || muon_tmp->DeltaR(subleadPho) < 0.3) { delete muon_tmp; continue; }
	    // save the muon data which pass preselection

	    outTree.MuonInfo_Size += 1;
	    outTree.MuonInfo_Charge->push_back(inTree.MuonInfo_Charge->at(i));
	    outTree.MuonInfo_MuonType->push_back(inTree.MuonInfo_MuonType->at(i));
	    outTree.MuonInfo_Pt->push_back(inTree.MuonInfo_Pt->at(i));
	    outTree.MuonInfo_Eta->push_back(inTree.MuonInfo_Eta->at(i));
	    outTree.MuonInfo_Phi->push_back(inTree.MuonInfo_Phi->at(i));
	    outTree.MuonInfo_Energy->push_back(inTree.MuonInfo_Energy->at(i));
	    outTree.MuonInfo_BestTrackDz->push_back(inTree.MuonInfo_BestTrackDz->at(i));
	    outTree.MuonInfo_BestTrackDxy->push_back(inTree.MuonInfo_BestTrackDxy->at(i));
	    outTree.MuonInfo_PFIsoDeltaBetaCorrR04->push_back(inTree.MuonInfo_PFIsoDeltaBetaCorrR04->at(i));
	    outTree.MuonInfo_TrackerBasedIsoR03->push_back(inTree.MuonInfo_TrackerBasedIsoR03->at(i));
	    outTree.MuonInfo_CutBasedIdMedium->push_back(inTree.MuonInfo_CutBasedIdMedium->at(i));
	    outTree.MuonInfo_CutBasedIdTight->push_back(inTree.MuonInfo_CutBasedIdTight->at(i));

	    muons.push_back(muon_tmp);
	} // end of muon loop

	// jet preselection
	for (int i=0; i<inTree.jets_size; ++i) {
	    if (inTree.JetInfo_Pt->at(i) < 25) continue;
	    if (fabs(inTree.JetInfo_Eta->at(i)) > 2.4) continue;

	    TLorentzVector jet_tmp;
	    jet_tmp.SetPtEtaPhiE(inTree.JetInfo_Pt->at(i), inTree.JetInfo_Eta->at(i), inTree.JetInfo_Phi->at(i), inTree.JetInfo_Energy->at(i));

	    if (jet_tmp.DeltaR(leadPho) < 0.4 || jet_tmp.DeltaR(subleadPho) < 0.4) continue;

	    bool jet_is_iso = true;
	    for (int j=0; j<electrons.size(); ++j)
		if (jet_tmp.DeltaR(*electrons.at(j)) < 0.4) {
		    jet_is_iso = false;
		    break;
		}
	    if (!jet_is_iso) continue;
	    for (int j=0; j<muons.size(); ++j)
		if (jet_tmp.DeltaR(*muons.at(j)) < 0.4) {
		    jet_is_iso = false;
		    break;
		}
	    if (!jet_is_iso) continue;

	    outTree.jets_size += 1;
	    outTree.JetInfo_Pt->push_back(inTree.JetInfo_Pt->at(i));
	    outTree.JetInfo_Eta->push_back(inTree.JetInfo_Eta->at(i));
	    outTree.JetInfo_Phi->push_back(inTree.JetInfo_Phi->at(i));
	    outTree.JetInfo_Mass->push_back(inTree.JetInfo_Mass->at(i));
	    outTree.JetInfo_Energy->push_back(inTree.JetInfo_Energy->at(i));
	    outTree.JetInfo_PtRaw->push_back(inTree.JetInfo_PtRaw->at(i));
	    outTree.JetInfo_QGL->push_back(inTree.JetInfo_QGL->at(i));
	    outTree.JetInfo_RMS->push_back(inTree.JetInfo_RMS->at(i));
	    outTree.JetInfo_puJetIdMVA->push_back(inTree.JetInfo_puJetIdMVA->at(i));
	    outTree.JetInfo_GenJetMatch->push_back(inTree.JetInfo_GenJetMatch->at(i));
	    outTree.JetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags->push_back(inTree.JetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags->at(i));
	    outTree.JetInfo_pfCombinedMVAV2BJetTags->push_back(inTree.JetInfo_pfCombinedMVAV2BJetTags->at(i));
	    outTree.JetInfo_pfDeepCSVJetTags_probb->push_back(inTree.JetInfo_pfDeepCSVJetTags_probb->at(i));
	    outTree.JetInfo_pfDeepCSVJetTags_probbb->push_back(inTree.JetInfo_pfDeepCSVJetTags_probbb->at(i));
	    outTree.JetInfo_pfDeepCSVJetTags_probc->push_back(inTree.JetInfo_pfDeepCSVJetTags_probc->at(i));
	    outTree.JetInfo_pfDeepCSVJetTags_probudsg->push_back(inTree.JetInfo_pfDeepCSVJetTags_probudsg->at(i));
	    outTree.JetInfo_JECScale->push_back(inTree.JetInfo_JECScale->at(i));
	    outTree.JetInfo_JERScale->push_back(inTree.JetInfo_JERScale->at(i));
	} // end of jet loop

	for (int i=0; i<electrons.size(); ++i) delete electrons[i];
	for (int i=0; i<muons.size(); ++i) delete muons[i];

	// set the rest of variables
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
