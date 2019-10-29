#include "TreeReader.h"

TreeReader::TreeReader(const char *treeName) {
    nt = new TChain(treeName);

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

void TreeReader::Add(const char *fileName) {
    nt->Add(fileName);
}

void TreeReader::Init() {
    nt->SetBranchAddress("EvtInfo.NPu", &EvtInfo_NPu);
    nt->SetBranchAddress("EvtInfo.NVtx", &EvtInfo_NVtx);
    nt->SetBranchAddress("EvtInfo.passTrigger", &EvtInfo_passTrigger);
    nt->SetBranchAddress("EvtInfo.genweight", &EvtInfo_genweight);
    nt->SetBranchAddress("EvtInfo.Rho", &EvtInfo_Rho);
    nt->SetBranchAddress("EvtInfo.PVz", &EvtInfo_PVz);
    nt->SetBranchAddress("EvtInfo.BSsigmaz", &EvtInfo_BSsigmaz);
    //////////
    nt->SetBranchAddress("DiPhoInfo.mass", &DiPhoInfo_mass);
    nt->SetBranchAddress("DiPhoInfo.pt", &DiPhoInfo_pt);
    nt->SetBranchAddress("DiPhoInfo.leadPt", &DiPhoInfo_leadPt);
    nt->SetBranchAddress("DiPhoInfo.leadEta", &DiPhoInfo_leadEta);
    nt->SetBranchAddress("DiPhoInfo.leadPhi", &DiPhoInfo_leadPhi);
    nt->SetBranchAddress("DiPhoInfo.leadE", &DiPhoInfo_leadE);
    nt->SetBranchAddress("DiPhoInfo.leadsigEOverE", &DiPhoInfo_leadsigEOverE);
    nt->SetBranchAddress("DiPhoInfo.leadR9", &DiPhoInfo_leadR9);
    nt->SetBranchAddress("DiPhoInfo.leadsieie", &DiPhoInfo_leadsieie);
    nt->SetBranchAddress("DiPhoInfo.leadhoe", &DiPhoInfo_leadhoe);
    nt->SetBranchAddress("DiPhoInfo.leadIDMVA", &DiPhoInfo_leadIDMVA);
    nt->SetBranchAddress("DiPhoInfo.leadhasPixelSeed", &DiPhoInfo_leadhasPixelSeed);
    nt->SetBranchAddress("DiPhoInfo.leadGenMatch", &DiPhoInfo_leadGenMatch);
    nt->SetBranchAddress("DiPhoInfo.leadGenMatchType", &DiPhoInfo_leadGenMatchType);
    nt->SetBranchAddress("DiPhoInfo.subleadPt", &DiPhoInfo_subleadPt);
    nt->SetBranchAddress("DiPhoInfo.subleadEta", &DiPhoInfo_subleadEta);
    nt->SetBranchAddress("DiPhoInfo.subleadPhi", &DiPhoInfo_subleadPhi);
    nt->SetBranchAddress("DiPhoInfo.subleadE", &DiPhoInfo_subleadE);
    nt->SetBranchAddress("DiPhoInfo.subleadsigEOverE", &DiPhoInfo_subleadsigEOverE);
    nt->SetBranchAddress("DiPhoInfo.subleadR9", &DiPhoInfo_subleadR9);
    nt->SetBranchAddress("DiPhoInfo.subleadsieie", &DiPhoInfo_subleadsieie);
    nt->SetBranchAddress("DiPhoInfo.subleadhoe", &DiPhoInfo_subleadhoe);
    nt->SetBranchAddress("DiPhoInfo.subleadIDMVA", &DiPhoInfo_subleadIDMVA);
    nt->SetBranchAddress("DiPhoInfo.subleadhasPixelSeed", &DiPhoInfo_subleadhasPixelSeed);
    nt->SetBranchAddress("DiPhoInfo.subleadGenMatch", &DiPhoInfo_subleadGenMatch);
    nt->SetBranchAddress("DiPhoInfo.subleadGenMatchType", &DiPhoInfo_subleadGenMatchType);
    nt->SetBranchAddress("DiPhoInfo.SelectedVz", &DiPhoInfo_SelectedVz);
    nt->SetBranchAddress("DiPhoInfo.GenVz", &DiPhoInfo_GenVz);
    nt->SetBranchAddress("DiPhoInfo.centralWeight", &DiPhoInfo_centralWeight);
    //////////
    nt->SetBranchAddress("ElecInfo.Size", &ElecInfo_Size);
    nt->SetBranchAddress("ElecInfo.Charge", &ElecInfo_Charge);
    nt->SetBranchAddress("ElecInfo.Pt", &ElecInfo_Pt);
    nt->SetBranchAddress("ElecInfo.Eta", &ElecInfo_Eta);
    nt->SetBranchAddress("ElecInfo.Phi", &ElecInfo_Phi);
    nt->SetBranchAddress("ElecInfo.Energy", &ElecInfo_Energy);
    nt->SetBranchAddress("ElecInfo.EtaSC", &ElecInfo_EtaSC);
    nt->SetBranchAddress("ElecInfo.PhiSC", &ElecInfo_PhiSC);
    nt->SetBranchAddress("ElecInfo.EGMCutBasedIDVeto", &ElecInfo_EGMCutBasedIDVeto);
    nt->SetBranchAddress("ElecInfo.EGMCutBasedIDLoose", &ElecInfo_EGMCutBasedIDLoose);
    nt->SetBranchAddress("ElecInfo.EGMCutBasedIDMedium", &ElecInfo_EGMCutBasedIDMedium);
    nt->SetBranchAddress("ElecInfo.EGMCutBasedIDTight", &ElecInfo_EGMCutBasedIDTight);
    nt->SetBranchAddress("ElecInfo.fggPhoVeto", &ElecInfo_fggPhoVeto);
    //////////
    nt->SetBranchAddress("MuonInfo.Size", &MuonInfo_Size);
    nt->SetBranchAddress("MuonInfo.Charge", &MuonInfo_Charge);
    nt->SetBranchAddress("MuonInfo.MuonType", &MuonInfo_MuonType);
    nt->SetBranchAddress("MuonInfo.Pt", &MuonInfo_Pt);
    nt->SetBranchAddress("MuonInfo.Eta", &MuonInfo_Eta);
    nt->SetBranchAddress("MuonInfo.Phi", &MuonInfo_Phi);
    nt->SetBranchAddress("MuonInfo.Energy", &MuonInfo_Energy);
    nt->SetBranchAddress("MuonInfo.BestTrackDz", &MuonInfo_BestTrackDz);
    nt->SetBranchAddress("MuonInfo.BestTrackDxy", &MuonInfo_BestTrackDxy);
    nt->SetBranchAddress("MuonInfo.PFIsoDeltaBetaCorrR04", &MuonInfo_PFIsoDeltaBetaCorrR04);
    nt->SetBranchAddress("MuonInfo.TrackerBasedIsoR03", &MuonInfo_TrackerBasedIsoR03);
    nt->SetBranchAddress("MuonInfo.CutBasedIdMedium", &MuonInfo_CutBasedIdMedium);
    nt->SetBranchAddress("MuonInfo.CutBasedIdTight", &MuonInfo_CutBasedIdTight);
    //////////
    nt->SetBranchAddress("jets_size", &jets_size);
    nt->SetBranchAddress("JetInfo.Pt", &JetInfo_Pt);
    nt->SetBranchAddress("JetInfo.Eta", &JetInfo_Eta);
    nt->SetBranchAddress("JetInfo.Phi", &JetInfo_Phi);
    nt->SetBranchAddress("JetInfo.Mass", &JetInfo_Mass);
    nt->SetBranchAddress("JetInfo.Energy", &JetInfo_Energy);
    nt->SetBranchAddress("JetInfo.PtRaw", &JetInfo_PtRaw);
    nt->SetBranchAddress("JetInfo.QGL", &JetInfo_QGL);
    nt->SetBranchAddress("JetInfo.RMS", &JetInfo_RMS);
    nt->SetBranchAddress("JetInfo.puJetIdMVA", &JetInfo_puJetIdMVA);
    nt->SetBranchAddress("JetInfo.GenJetMatch", &JetInfo_GenJetMatch);
    nt->SetBranchAddress("JetInfo.pfCombinedInclusiveSecondaryVertexV2BJetTags", &JetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags);
    nt->SetBranchAddress("JetInfo.pfCombinedMVAV2BJetTags", &JetInfo_pfCombinedMVAV2BJetTags);
    nt->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probb", &JetInfo_pfDeepCSVJetTags_probb);
    nt->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probbb", &JetInfo_pfDeepCSVJetTags_probbb);
    nt->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probc", &JetInfo_pfDeepCSVJetTags_probc);
    nt->SetBranchAddress("JetInfo.pfDeepCSVJetTags_probudsg", &JetInfo_pfDeepCSVJetTags_probudsg);
    nt->SetBranchAddress("JetInfo.JECScale", &JetInfo_JECScale);
    nt->SetBranchAddress("JetInfo.JERScale", &JetInfo_JERScale);
    //////////
    nt->SetBranchAddress("MetInfo.Pt", &MetInfo_Pt);
    nt->SetBranchAddress("MetInfo.Phi", &MetInfo_Phi);
    nt->SetBranchAddress("MetInfo.Px", &MetInfo_Px);
    nt->SetBranchAddress("MetInfo.Py", &MetInfo_Py);
    nt->SetBranchAddress("MetInfo.SumET", &MetInfo_SumET);
    //////////
    nt->SetBranchAddress("GenPartInfo.size", &GenPartInfo_size);
    nt->SetBranchAddress("GenPartInfo.Pt", &GenPartInfo_Pt);
    nt->SetBranchAddress("GenPartInfo.Eta", &GenPartInfo_Eta);
    nt->SetBranchAddress("GenPartInfo.Phi", &GenPartInfo_Phi);
    nt->SetBranchAddress("GenPartInfo.Mass", &GenPartInfo_Mass);
    nt->SetBranchAddress("GenPartInfo.PdgID", &GenPartInfo_PdgID);
    nt->SetBranchAddress("GenPartInfo.Status", &GenPartInfo_Status);
    nt->SetBranchAddress("GenPartInfo.nMo", &GenPartInfo_nMo);
    nt->SetBranchAddress("GenPartInfo.nDa", &GenPartInfo_nDa);
}

void TreeReader::GetEntry(long entry) {
    nt->GetEntry(entry);
}

long TreeReader::GetEntries() {
    return nt->GetEntries();
}
