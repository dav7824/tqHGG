#include "TreeWriter.h"

TreeWriter::TreeWriter(const char *treeName, const char *fileName) {
    fout = new TFile(fileName, "recreate");
    nt = new TTree(treeName, "");

    EvtInfo_NPu = 0;
    EvtInfo_NVtx = 0;
    EvtInfo_passTrigger = 0;
    EvtInfo_genweight = 0;
    EvtInfo_Rho = 0;
    EvtInfo_PVz = 0;
    EvtInfo_BSsigmaz = 0;
    //////////
    DiPhoInfo_mass = 0;
    DiPhoInfo_pt = 0;
    DiPhoInfo_leadPt = 0;
    DiPhoInfo_leadEta = 0;
    DiPhoInfo_leadPhi = 0;
    DiPhoInfo_leadE = 0;
    DiPhoInfo_leadsigEOverE = 0;
    DiPhoInfo_leadR9 = 0;
    DiPhoInfo_leadsieie = 0;
    DiPhoInfo_leadhoe = 0;
    DiPhoInfo_leadIDMVA = 0;
    DiPhoInfo_leadhasPixelSeed = 0;
    DiPhoInfo_leadGenMatch = 0;
    DiPhoInfo_leadGenMatchType = 0;
    DiPhoInfo_subleadPt = 0;
    DiPhoInfo_subleadEta = 0;
    DiPhoInfo_subleadPhi = 0;
    DiPhoInfo_subleadE = 0;
    DiPhoInfo_subleadsigEOverE = 0;
    DiPhoInfo_subleadR9 = 0;
    DiPhoInfo_subleadsieie = 0;
    DiPhoInfo_subleadhoe = 0;
    DiPhoInfo_subleadIDMVA = 0;
    DiPhoInfo_subleadhasPixelSeed = 0;
    DiPhoInfo_subleadGenMatch = 0;
    DiPhoInfo_subleadGenMatchType = 0;
    DiPhoInfo_SelectedVz = 0;
    DiPhoInfo_GenVz = 0;
    DiPhoInfo_centralWeight = 0;
    //////////
    ElecInfo_Size = 0;
    ElecInfo_Charge = 0;
    ElecInfo_Pt = 0;
    ElecInfo_Eta = 0;
    ElecInfo_Phi = 0;
    ElecInfo_Energy = 0;
    ElecInfo_EtaSC = 0;
    ElecInfo_PhiSC = 0;
    ElecInfo_EGMCutBasedIDVeto = 0;
    ElecInfo_EGMCutBasedIDLoose = 0;
    ElecInfo_EGMCutBasedIDMedium = 0;
    ElecInfo_EGMCutBasedIDTight = 0;
    ElecInfo_fggPhoVeto = 0;
    //////////
    MuonInfo_Size = 0;
    MuonInfo_Charge = 0;
    MuonInfo_MuonType = 0;
    MuonInfo_Pt = 0;
    MuonInfo_Eta = 0;
    MuonInfo_Phi = 0;
    MuonInfo_Energy = 0;
    MuonInfo_BestTrackDz = 0;
    MuonInfo_BestTrackDxy = 0;
    MuonInfo_PFIsoDeltaBetaCorrR04 = 0;
    MuonInfo_TrackerBasedIsoR03 = 0;
    MuonInfo_CutBasedIdMedium = 0;
    MuonInfo_CutBasedIdTight = 0;
    //////////
    jets_size = 0;
    JetInfo_Pt = 0;
    JetInfo_Eta = 0;
    JetInfo_Phi = 0;
    JetInfo_Mass = 0;
    JetInfo_Energy = 0;
    JetInfo_PtRaw = 0;
    JetInfo_QGL = 0;
    JetInfo_RMS = 0;
    JetInfo_puJetIdMVA = 0;
    JetInfo_GenJetMatch = 0;
    JetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags = 0;
    JetInfo_pfCombinedMVAV2BJetTags = 0;
    JetInfo_pfDeepCSVJetTags_probb = 0;
    JetInfo_pfDeepCSVJetTags_probbb = 0;
    JetInfo_pfDeepCSVJetTags_probc = 0;
    JetInfo_pfDeepCSVJetTags_probudsg = 0;
    JetInfo_JECScale = 0;
    JetInfo_JERScale = 0;
    //////////
    MetInfo_Pt = 0;
    MetInfo_Phi = 0;
    MetInfo_Px = 0;
    MetInfo_Py = 0;
    MetInfo_SumET = 0;
    //////////
    GenPartInfo_size = 0;
    GenPartInfo_Pt = 0;
    GenPartInfo_Eta = 0;
    GenPartInfo_Phi = 0;
    GenPartInfo_Mass = 0;
    GenPartInfo_PdgID = 0;
    GenPartInfo_Status = 0;
    GenPartInfo_nMo = 0;
    GenPartInfo_nDa = 0;
}

void TreeWriter::Init() {
    nt->Branch("EvtInfo.NPu", &EvtInfo_NPu);
    nt->Branch("EvtInfo.NVtx", &EvtInfo_NVtx);
    nt->Branch("EvtInfo.passTrigger", &EvtInfo_passTrigger);
    nt->Branch("EvtInfo.genweight", &EvtInfo_genweight);
    nt->Branch("EvtInfo.Rho", &EvtInfo_Rho);
    nt->Branch("EvtInfo.PVz", &EvtInfo_PVz);
    nt->Branch("EvtInfo.BSsigmaz", &EvtInfo_BSsigmaz);
    //////////
    nt->Branch("DiPhoInfo.mass", &DiPhoInfo_mass);
    nt->Branch("DiPhoInfo.pt", &DiPhoInfo_pt);
    nt->Branch("DiPhoInfo.leadPt", &DiPhoInfo_leadPt);
    nt->Branch("DiPhoInfo.leadEta", &DiPhoInfo_leadEta);
    nt->Branch("DiPhoInfo.leadPhi", &DiPhoInfo_leadPhi);
    nt->Branch("DiPhoInfo.leadE", &DiPhoInfo_leadE);
    nt->Branch("DiPhoInfo.leadsigEOverE", &DiPhoInfo_leadsigEOverE);
    nt->Branch("DiPhoInfo.leadR9", &DiPhoInfo_leadR9);
    nt->Branch("DiPhoInfo.leadsieie", &DiPhoInfo_leadsieie);
    nt->Branch("DiPhoInfo.leadhoe", &DiPhoInfo_leadhoe);
    nt->Branch("DiPhoInfo.leadIDMVA", &DiPhoInfo_leadIDMVA);
    nt->Branch("DiPhoInfo.leadhasPixelSeed", &DiPhoInfo_leadhasPixelSeed);
    nt->Branch("DiPhoInfo.leadGenMatch", &DiPhoInfo_leadGenMatch);
    nt->Branch("DiPhoInfo.leadGenMatchType", &DiPhoInfo_leadGenMatchType);
    nt->Branch("DiPhoInfo.subleadPt", &DiPhoInfo_subleadPt);
    nt->Branch("DiPhoInfo.subleadEta", &DiPhoInfo_subleadEta);
    nt->Branch("DiPhoInfo.subleadPhi", &DiPhoInfo_subleadPhi);
    nt->Branch("DiPhoInfo.subleadE", &DiPhoInfo_subleadE);
    nt->Branch("DiPhoInfo.subleadsigEOverE", &DiPhoInfo_subleadsigEOverE);
    nt->Branch("DiPhoInfo.subleadR9", &DiPhoInfo_subleadR9);
    nt->Branch("DiPhoInfo.subleadsieie", &DiPhoInfo_subleadsieie);
    nt->Branch("DiPhoInfo.subleadhoe", &DiPhoInfo_subleadhoe);
    nt->Branch("DiPhoInfo.subleadIDMVA", &DiPhoInfo_subleadIDMVA);
    nt->Branch("DiPhoInfo.subleadhasPixelSeed", &DiPhoInfo_subleadhasPixelSeed);
    nt->Branch("DiPhoInfo.subleadGenMatch", &DiPhoInfo_subleadGenMatch);
    nt->Branch("DiPhoInfo.subleadGenMatchType", &DiPhoInfo_subleadGenMatchType);
    nt->Branch("DiPhoInfo.SelectedVz", &DiPhoInfo_SelectedVz);
    nt->Branch("DiPhoInfo.GenVz", &DiPhoInfo_GenVz);
    nt->Branch("DiPhoInfo.centralWeight", &DiPhoInfo_centralWeight);
    //////////
    nt->Branch("ElecInfo.Size", &ElecInfo_Size);
    nt->Branch("ElecInfo.Charge", &ElecInfo_Charge);
    nt->Branch("ElecInfo.Pt", &ElecInfo_Pt);
    nt->Branch("ElecInfo.Eta", &ElecInfo_Eta);
    nt->Branch("ElecInfo.Phi", &ElecInfo_Phi);
    nt->Branch("ElecInfo.Energy", &ElecInfo_Energy);
    nt->Branch("ElecInfo.EtaSC", &ElecInfo_EtaSC);
    nt->Branch("ElecInfo.PhiSC", &ElecInfo_PhiSC);
    nt->Branch("ElecInfo.EGMCutBasedIDVeto", &ElecInfo_EGMCutBasedIDVeto);
    nt->Branch("ElecInfo.EGMCutBasedIDLoose", &ElecInfo_EGMCutBasedIDLoose);
    nt->Branch("ElecInfo.EGMCutBasedIDMedium", &ElecInfo_EGMCutBasedIDMedium);
    nt->Branch("ElecInfo.EGMCutBasedIDTight", &ElecInfo_EGMCutBasedIDTight);
    nt->Branch("ElecInfo.fggPhoVeto", &ElecInfo_fggPhoVeto);
    //////////
    nt->Branch("MuonInfo.Size", &MuonInfo_Size);
    nt->Branch("MuonInfo.Charge", &MuonInfo_Charge);
    nt->Branch("MuonInfo.MuonType", &MuonInfo_MuonType);
    nt->Branch("MuonInfo.Pt", &MuonInfo_Pt);
    nt->Branch("MuonInfo.Eta", &MuonInfo_Eta);
    nt->Branch("MuonInfo.Phi", &MuonInfo_Phi);
    nt->Branch("MuonInfo.Energy", &MuonInfo_Energy);
    nt->Branch("MuonInfo.BestTrackDz", &MuonInfo_BestTrackDz);
    nt->Branch("MuonInfo.BestTrackDxy", &MuonInfo_BestTrackDxy);
    nt->Branch("MuonInfo.PFIsoDeltaBetaCorrR04", &MuonInfo_PFIsoDeltaBetaCorrR04);
    nt->Branch("MuonInfo.TrackerBasedIsoR03", &MuonInfo_TrackerBasedIsoR03);
    nt->Branch("MuonInfo.CutBasedIdMedium", &MuonInfo_CutBasedIdMedium);
    nt->Branch("MuonInfo.CutBasedIdTight", &MuonInfo_CutBasedIdTight);
    //////////
    nt->Branch("jets_size", &jets_size);
    nt->Branch("JetInfo.Pt", &JetInfo_Pt);
    nt->Branch("JetInfo.Eta", &JetInfo_Eta);
    nt->Branch("JetInfo.Phi", &JetInfo_Phi);
    nt->Branch("JetInfo.Mass", &JetInfo_Mass);
    nt->Branch("JetInfo.Energy", &JetInfo_Energy);
    nt->Branch("JetInfo.PtRaw", &JetInfo_PtRaw);
    nt->Branch("JetInfo.QGL", &JetInfo_QGL);
    nt->Branch("JetInfo.RMS", &JetInfo_RMS);
    nt->Branch("JetInfo.puJetIdMVA", &JetInfo_puJetIdMVA);
    nt->Branch("JetInfo.GenJetMatch", &JetInfo_GenJetMatch);
    nt->Branch("JetInfo.pfCombinedInclusiveSecondaryVertexV2BJetTags", &JetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags);
    nt->Branch("JetInfo.pfCombinedMVAV2BJetTags", &JetInfo_pfCombinedMVAV2BJetTags);
    nt->Branch("JetInfo.pfDeepCSVJetTags_probb", &JetInfo_pfDeepCSVJetTags_probb);
    nt->Branch("JetInfo.pfDeepCSVJetTags_probbb", &JetInfo_pfDeepCSVJetTags_probbb);
    nt->Branch("JetInfo.pfDeepCSVJetTags_probc", &JetInfo_pfDeepCSVJetTags_probc);
    nt->Branch("JetInfo.pfDeepCSVJetTags_probudsg", &JetInfo_pfDeepCSVJetTags_probudsg);
    nt->Branch("JetInfo.JECScale", &JetInfo_JECScale);
    nt->Branch("JetInfo.JERScale", &JetInfo_JERScale);
    //////////
    nt->Branch("MetInfo.Pt", &MetInfo_Pt);
    nt->Branch("MetInfo.Phi", &MetInfo_Phi);
    nt->Branch("MetInfo.Px", &MetInfo_Px);
    nt->Branch("MetInfo.Py", &MetInfo_Py);
    nt->Branch("MetInfo.SumET", &MetInfo_SumET);
    //////////
    nt->Branch("GenPartInfo.size", &GenPartInfo_size);
    nt->Branch("GenPartInfo.Pt", &GenPartInfo_Pt);
    nt->Branch("GenPartInfo.Eta", &GenPartInfo_Eta);
    nt->Branch("GenPartInfo.Phi", &GenPartInfo_Phi);
    nt->Branch("GenPartInfo.Mass", &GenPartInfo_Mass);
    nt->Branch("GenPartInfo.PdgID", &GenPartInfo_PdgID);
    nt->Branch("GenPartInfo.Status", &GenPartInfo_Status);
    nt->Branch("GenPartInfo.nMo", &GenPartInfo_nMo);
    nt->Branch("GenPartInfo.nDa", &GenPartInfo_nDa);
}

void TreeWriter::Fill() {
    nt->Fill();
}

void TreeWriter::Save() {
    fout->cd();
    nt->Write();
    fout->Close();
}
