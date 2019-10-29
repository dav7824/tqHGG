#ifndef _TREEREADER_V1_2_H_
#define _TREEREADER_V1_2_H_

#include "TChain.h"

#include <vector>
using namespace std;

class TreeReader
{
public:
    TreeReader(const char*);
    void Add(const char*);
    void Init();
    void GetEntry(long);
    long GetEntries();

    TChain *nt;

    // tree variables
    float EvtInfo_NPu;
    int EvtInfo_NVtx;
    bool EvtInfo_passTrigger;
    float EvtInfo_genweight;
    float EvtInfo_Rho;
    float EvtInfo_PVz;
    float EvtInfo_BSsigmaz;
    //////////
    float DiPhoInfo_mass;
    float DiPhoInfo_pt;
    float DiPhoInfo_leadPt;
    float DiPhoInfo_leadEta;
    float DiPhoInfo_leadPhi;
    float DiPhoInfo_leadE;
    float DiPhoInfo_leadsigEOverE;
    float DiPhoInfo_leadR9;
    float DiPhoInfo_leadsieie;
    float DiPhoInfo_leadhoe;
    float DiPhoInfo_leadIDMVA;
    bool DiPhoInfo_leadhasPixelSeed;
    bool DiPhoInfo_leadGenMatch;
    int DiPhoInfo_leadGenMatchType;
    float DiPhoInfo_subleadPt;
    float DiPhoInfo_subleadEta;
    float DiPhoInfo_subleadPhi;
    float DiPhoInfo_subleadE;
    float DiPhoInfo_subleadsigEOverE;
    float DiPhoInfo_subleadR9;
    float DiPhoInfo_subleadsieie;
    float DiPhoInfo_subleadhoe;
    float DiPhoInfo_subleadIDMVA;
    bool DiPhoInfo_subleadhasPixelSeed;
    bool DiPhoInfo_subleadGenMatch;
    int DiPhoInfo_subleadGenMatchType;
    float DiPhoInfo_SelectedVz;
    float DiPhoInfo_GenVz;
    float DiPhoInfo_centralWeight;
    //////////
    int ElecInfo_Size;
    vector<int> *ElecInfo_Charge;
    vector<float> *ElecInfo_Pt;
    vector<float> *ElecInfo_Eta;
    vector<float> *ElecInfo_Phi;
    vector<float> *ElecInfo_Energy;
    vector<float> *ElecInfo_EtaSC;
    vector<float> *ElecInfo_PhiSC;
    vector<bool> *ElecInfo_EGMCutBasedIDVeto;
    vector<bool> *ElecInfo_EGMCutBasedIDLoose;
    vector<bool> *ElecInfo_EGMCutBasedIDMedium;
    vector<bool> *ElecInfo_EGMCutBasedIDTight;
    vector<bool> *ElecInfo_fggPhoVeto;
    //////////
    int MuonInfo_Size;
    vector<int> *MuonInfo_Charge;
    vector<float> *MuonInfo_MuonType;
    vector<float> *MuonInfo_Pt;
    vector<float> *MuonInfo_Eta;
    vector<float> *MuonInfo_Phi;
    vector<float> *MuonInfo_Energy;
    vector<float> *MuonInfo_BestTrackDz;
    vector<float> *MuonInfo_BestTrackDxy;
    vector<float> *MuonInfo_PFIsoDeltaBetaCorrR04;
    vector<float> *MuonInfo_TrackerBasedIsoR03;
    vector<bool> *MuonInfo_CutBasedIdMedium;
    vector<bool> *MuonInfo_CutBasedIdTight;
    //////////
    int jets_size;
    vector<float> *JetInfo_Pt;
    vector<float> *JetInfo_Eta;
    vector<float> *JetInfo_Phi;
    vector<float> *JetInfo_Mass;
    vector<float> *JetInfo_Energy;
    vector<float> *JetInfo_PtRaw;
    vector<float> *JetInfo_QGL;
    vector<float> *JetInfo_RMS;
    vector<float> *JetInfo_puJetIdMVA;
    vector<bool> *JetInfo_GenJetMatch;
    vector<float> *JetInfo_pfCombinedInclusiveSecondaryVertexV2BJetTags;
    vector<float> *JetInfo_pfCombinedMVAV2BJetTags;
    vector<float> *JetInfo_pfDeepCSVJetTags_probb;
    vector<float> *JetInfo_pfDeepCSVJetTags_probbb;
    vector<float> *JetInfo_pfDeepCSVJetTags_probc;
    vector<float> *JetInfo_pfDeepCSVJetTags_probudsg;
    vector<float> *JetInfo_JECScale;
    vector<float> *JetInfo_JERScale;
    //////////
    float MetInfo_Pt;
    float MetInfo_Phi;
    float MetInfo_Px;
    float MetInfo_Py;
    float MetInfo_SumET;
    //////////
    int GenPartInfo_size;
    vector<float> *GenPartInfo_Pt;
    vector<float> *GenPartInfo_Eta;
    vector<float> *GenPartInfo_Phi;
    vector<float> *GenPartInfo_Mass;
    vector<int> *GenPartInfo_PdgID;
    vector<int> *GenPartInfo_Status;
    vector<int> *GenPartInfo_nMo;
    vector<int> *GenPartInfo_nDa;
};

#endif
