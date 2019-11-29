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
    nbjets_size = 0;
    nbJetInfo_Pt = 0;
    nbJetInfo_Eta = 0;
    nbJetInfo_Phi = 0;
    nbJetInfo_Mass = 0;
    nbJetInfo_Energy = 0;
    nbJetInfo_PtRaw = 0;
    nbJetInfo_QGL = 0;
    nbJetInfo_RMS = 0;
    nbJetInfo_puJetIdMVA = 0;
    nbJetInfo_GenJetMatch = 0;
    nbJetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags = 0;
    nbJetInfo_pfCombinedMVAV2BJetTags = 0;
    nbJetInfo_pfDeepCSVJetTags_probb = 0;
    nbJetInfo_pfDeepCSVJetTags_probbb = 0;
    nbJetInfo_pfDeepCSVJetTags_probc = 0;
    nbJetInfo_pfDeepCSVJetTags_probudsg = 0;
    nbJetInfo_JECScale = 0;
    nbJetInfo_JERScale = 0;
    //////////
    bjets_size = 0;
    bJetInfo_Pt = 0;
    bJetInfo_Eta = 0;
    bJetInfo_Phi = 0;
    bJetInfo_Mass = 0;
    bJetInfo_Energy = 0;
    bJetInfo_PtRaw = 0;
    bJetInfo_QGL = 0;
    bJetInfo_RMS = 0;
    bJetInfo_puJetIdMVA = 0;
    bJetInfo_GenJetMatch = 0;
    bJetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags = 0;
    bJetInfo_pfCombinedMVAV2BJetTags = 0;
    bJetInfo_pfDeepCSVJetTags_probb = 0;
    bJetInfo_pfDeepCSVJetTags_probbb = 0;
    bJetInfo_pfDeepCSVJetTags_probc = 0;
    bJetInfo_pfDeepCSVJetTags_probudsg = 0;
    bJetInfo_JECScale = 0;
    bJetInfo_JERScale = 0;
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
    nt->SetBranchAddress("nbjets_size", &nbjets_size);
    nt->SetBranchAddress("nbJetInfo.Pt", &nbJetInfo_Pt);
    nt->SetBranchAddress("nbJetInfo.Eta", &nbJetInfo_Eta);
    nt->SetBranchAddress("nbJetInfo.Phi", &nbJetInfo_Phi);
    nt->SetBranchAddress("nbJetInfo.Mass", &nbJetInfo_Mass);
    nt->SetBranchAddress("nbJetInfo.Energy", &nbJetInfo_Energy);
    nt->SetBranchAddress("nbJetInfo.PtRaw", &nbJetInfo_PtRaw);
    nt->SetBranchAddress("nbJetInfo.QGL", &nbJetInfo_QGL);
    nt->SetBranchAddress("nbJetInfo.RMS", &nbJetInfo_RMS);
    nt->SetBranchAddress("nbJetInfo.puJetIdMVA", &nbJetInfo_puJetIdMVA);
    nt->SetBranchAddress("nbJetInfo.GenJetMatch", &nbJetInfo_GenJetMatch);
    nt->SetBranchAddress("nbJetInfo.pfCombinedInclusiveSecondaryVertexV2BJetTags", &nbJetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags);
    nt->SetBranchAddress("nbJetInfo.pfCombinedMVAV2BJetTags", &nbJetInfo_pfCombinedMVAV2BJetTags);
    nt->SetBranchAddress("nbJetInfo.pfDeepCSVJetTags_probb", &nbJetInfo_pfDeepCSVJetTags_probb);
    nt->SetBranchAddress("nbJetInfo.pfDeepCSVJetTags_probbb", &nbJetInfo_pfDeepCSVJetTags_probbb);
    nt->SetBranchAddress("nbJetInfo.pfDeepCSVJetTags_probc", &nbJetInfo_pfDeepCSVJetTags_probc);
    nt->SetBranchAddress("nbJetInfo.pfDeepCSVJetTags_probudsg", &nbJetInfo_pfDeepCSVJetTags_probudsg);
    nt->SetBranchAddress("nbJetInfo.JECScale", &nbJetInfo_JECScale);
    nt->SetBranchAddress("nbJetInfo.JERScale", &nbJetInfo_JERScale);
    //////////
    nt->SetBranchAddress("bjets_size", &bjets_size);
    nt->SetBranchAddress("bJetInfo.Pt", &bJetInfo_Pt);
    nt->SetBranchAddress("bJetInfo.Eta", &bJetInfo_Eta);
    nt->SetBranchAddress("bJetInfo.Phi", &bJetInfo_Phi);
    nt->SetBranchAddress("bJetInfo.Mass", &bJetInfo_Mass);
    nt->SetBranchAddress("bJetInfo.Energy", &bJetInfo_Energy);
    nt->SetBranchAddress("bJetInfo.PtRaw", &bJetInfo_PtRaw);
    nt->SetBranchAddress("bJetInfo.QGL", &bJetInfo_QGL);
    nt->SetBranchAddress("bJetInfo.RMS", &bJetInfo_RMS);
    nt->SetBranchAddress("bJetInfo.puJetIdMVA", &bJetInfo_puJetIdMVA);
    nt->SetBranchAddress("bJetInfo.GenJetMatch", &bJetInfo_GenJetMatch);
    nt->SetBranchAddress("bJetInfo.pfCombinedInclusiveSecondaryVertexV2BJetTags", &bJetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags);
    nt->SetBranchAddress("bJetInfo.pfCombinedMVAV2BJetTags", &bJetInfo_pfCombinedMVAV2BJetTags);
    nt->SetBranchAddress("bJetInfo.pfDeepCSVJetTags_probb", &bJetInfo_pfDeepCSVJetTags_probb);
    nt->SetBranchAddress("bJetInfo.pfDeepCSVJetTags_probbb", &bJetInfo_pfDeepCSVJetTags_probbb);
    nt->SetBranchAddress("bJetInfo.pfDeepCSVJetTags_probc", &bJetInfo_pfDeepCSVJetTags_probc);
    nt->SetBranchAddress("bJetInfo.pfDeepCSVJetTags_probudsg", &bJetInfo_pfDeepCSVJetTags_probudsg);
    nt->SetBranchAddress("bJetInfo.JECScale", &bJetInfo_JECScale);
    nt->SetBranchAddress("bJetInfo.JERScale", &bJetInfo_JERScale);
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
