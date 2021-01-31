/*
 * Calculate event weights given gen-weights & SFs.
 *
 * Usage:
 *   ./MakeWeight <fin> <makeSyst=syst|nosyst>
 */

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fin_name = argv[1];
    TString makeSyst_ = argv[2];
    bool makeSyst;
    if (makeSyst_ == "syst") makeSyst = true;
    else makeSyst = false;

    // Read file
    TFile *fin = new TFile(fin_name, "update");
    TTree *Tin = (TTree*)fin->Get("T");
    // Gen-weight
    float Evt_genweight = 0;
    // Photon SF
    float DiPho_centralWeight = 0;
    // Photon SF syst
    float DiPho_LooseMvaSFUp = 0;
    float DiPho_LooseMvaSFDown = 0;
    float DiPho_PreselSFUp = 0;
    float DiPho_PreselSFDown = 0;
    float DiPho_electronVetoSFUp = 0;
    float DiPho_electronVetoSFDown = 0;
    float DiPho_TriggerWeightUp = 0;
    float DiPho_TriggerWeightDown = 0;
    // Pileup SF
    float SF_pileup = 0;
    // Electron SF
    float SF_Elec = 0;
    // Electron SF syst
    float SF_Elec_RecoUp = 0;
    float SF_Elec_RecoDown = 0;
    float SF_Elec_IdUp = 0;
    float SF_Elec_IdDown = 0;
    // Muon SF
    float SF_Muon = 0;
    // Muon SF syst
    float SF_Muon_IdUp = 0;
    float SF_Muon_IdDown = 0;
    float SF_Muon_IsoUp = 0;
    float SF_Muon_IsoDown = 0;
    // b-tag SF
    float SF_btag = 0;
    // b-tag SF syst
    float SF_btag_jesUp = 0;
    float SF_btag_jesDown = 0;
    float SF_btag_lfUp = 0;
    float SF_btag_lfDown = 0;
    float SF_btag_hfUp = 0;
    float SF_btag_hfDown = 0;
    float SF_btag_lfstats1Up = 0;
    float SF_btag_lfstats1Down = 0;
    float SF_btag_lfstats2Up = 0;
    float SF_btag_lfstats2Down = 0;
    float SF_btag_hfstats1Up = 0;
    float SF_btag_hfstats1Down = 0;
    float SF_btag_hfstats2Up = 0;
    float SF_btag_hfstats2Down = 0;
    float SF_btag_cferr1Up = 0;
    float SF_btag_cferr1Down = 0;
    float SF_btag_cferr2Up = 0;
    float SF_btag_cferr2Down = 0;
    // Set branches
    // Gen-weight
    Tin->SetBranchAddress("EvtInfo.genweight", &Evt_genweight);
    // Photon SF
    Tin->SetBranchAddress("DiPhoInfo.centralWeight", &DiPho_centralWeight);
    // Photon SF syst
    Tin->SetBranchAddress("DiPhoInfo.LooseMvaSFUp", &DiPho_LooseMvaSFUp);
    Tin->SetBranchAddress("DiPhoInfo.LooseMvaSFDown", &DiPho_LooseMvaSFDown);
    Tin->SetBranchAddress("DiPhoInfo.PreselSFUp", &DiPho_PreselSFUp);
    Tin->SetBranchAddress("DiPhoInfo.PreselSFDown", &DiPho_PreselSFDown);
    Tin->SetBranchAddress("DiPhoInfo.electronVetoSFUp", &DiPho_electronVetoSFUp);
    Tin->SetBranchAddress("DiPhoInfo.electronVetoSFDown", &DiPho_electronVetoSFDown);
    Tin->SetBranchAddress("DiPhoInfo.TriggerWeightUp", &DiPho_TriggerWeightUp);
    Tin->SetBranchAddress("DiPhoInfo.TriggerWeightDown", &DiPho_TriggerWeightDown);
    // Pileup SF
    Tin->SetBranchAddress("SF_pileup", &SF_pileup);
    // Electron SF
    Tin->SetBranchAddress("SF_Elec", &SF_Elec);
    // Electron SF syst
    Tin->SetBranchAddress("SF_Elec_RecoUp", &SF_Elec_RecoUp);
    Tin->SetBranchAddress("SF_Elec_RecoDown", &SF_Elec_RecoDown);
    Tin->SetBranchAddress("SF_Elec_IdUp", &SF_Elec_IdUp);
    Tin->SetBranchAddress("SF_Elec_IdDown", &SF_Elec_IdDown);
    // Muon SF
    Tin->SetBranchAddress("SF_Muon", &SF_Muon);
    // Muon SF syst
    Tin->SetBranchAddress("SF_Muon_IdUp", &SF_Muon_IdUp);
    Tin->SetBranchAddress("SF_Muon_IdDown", &SF_Muon_IdDown);
    Tin->SetBranchAddress("SF_Muon_IsoUp", &SF_Muon_IsoUp);
    Tin->SetBranchAddress("SF_Muon_IsoDown", &SF_Muon_IsoDown);
    // b-tag SF
    Tin->SetBranchAddress("SF_btag", &SF_btag);
    // b-tag SF syst
    Tin->SetBranchAddress("SF_btag_jesUp", &SF_btag_jesUp);
    Tin->SetBranchAddress("SF_btag_jesDown", &SF_btag_jesDown);
    Tin->SetBranchAddress("SF_btag_lfUp", &SF_btag_lfUp);
    Tin->SetBranchAddress("SF_btag_lfDown", &SF_btag_lfDown);
    Tin->SetBranchAddress("SF_btag_hfUp", &SF_btag_hfUp);
    Tin->SetBranchAddress("SF_btag_hfDown", &SF_btag_hfDown);
    Tin->SetBranchAddress("SF_btag_lfstats1Up", &SF_btag_lfstats1Up);
    Tin->SetBranchAddress("SF_btag_lfstats1Down", &SF_btag_lfstats1Down);
    Tin->SetBranchAddress("SF_btag_lfstats2Up", &SF_btag_lfstats2Up);
    Tin->SetBranchAddress("SF_btag_lfstats2Down", &SF_btag_lfstats2Down);
    Tin->SetBranchAddress("SF_btag_hfstats1Up", &SF_btag_hfstats1Up);
    Tin->SetBranchAddress("SF_btag_hfstats1Down", &SF_btag_hfstats1Down);
    Tin->SetBranchAddress("SF_btag_hfstats2Up", &SF_btag_hfstats2Up);
    Tin->SetBranchAddress("SF_btag_hfstats2Down", &SF_btag_hfstats2Down);
    Tin->SetBranchAddress("SF_btag_cferr1Up", &SF_btag_cferr1Up);
    Tin->SetBranchAddress("SF_btag_cferr1Down", &SF_btag_cferr1Down);
    Tin->SetBranchAddress("SF_btag_cferr2Up", &SF_btag_cferr2Up);
    Tin->SetBranchAddress("SF_btag_cferr2Down", &SF_btag_cferr2Down);

    // New tree
    TTree *Tout = Tin->CloneTree(0);
    // Central
    float evtwei_central = -999;
    // Photon syst
    float evtwei_DiPhoLooseMvaSFUp = -999;
    float evtwei_DiPhoLooseMvaSFDown = -999;
    float evtwei_DiPhoPreselSFUp = -999;
    float evtwei_DiPhoPreselSFDown = -999;
    float evtwei_DiPhoelectronVetoSFUp = -999;
    float evtwei_DiPhoelectronVetoSFDown = -999;
    float evtwei_DiPhoTriggerWeightUp = -999;
    float evtwei_DiPhoTriggerWeightDown = -999;
    // Electron syst
    float evtwei_ElecRecoUp = -999;
    float evtwei_ElecRecoDown = -999;
    float evtwei_ElecIdUp = -999;
    float evtwei_ElecIdDown = -999;
    // Muon syst
    float evtwei_MuonIdUp = -999;
    float evtwei_MuonIdDown = -999;
    float evtwei_MuonIsoUp = -999;
    float evtwei_MuonIsoDown = -999;
    // b-tag syst
    float evtwei_btagjesUp = -999;
    float evtwei_btagjesDown = -999;
    float evtwei_btaglfUp = -999;
    float evtwei_btaglfDown = -999;
    float evtwei_btaghfUp = -999;
    float evtwei_btaghfDown = -999;
    float evtwei_btaglfstats1Up = -999;
    float evtwei_btaglfstats1Down = -999;
    float evtwei_btaglfstats2Up = -999;
    float evtwei_btaglfstats2Down = -999;
    float evtwei_btaghfstats1Up = -999;
    float evtwei_btaghfstats1Down = -999;
    float evtwei_btaghfstats2Up = -999;
    float evtwei_btaghfstats2Down = -999;
    float evtwei_btagcferr1Up = -999;
    float evtwei_btagcferr1Down = -999;
    float evtwei_btagcferr2Up = -999;
    float evtwei_btagcferr2Down = -999;
    // Set branches
    // Central
    Tout->Branch("evtwei_central", &evtwei_central);
    // Photon syst
    Tout->Branch("evtwei_DiPhoLooseMvaSFUp", &evtwei_DiPhoLooseMvaSFUp);
    Tout->Branch("evtwei_DiPhoLooseMvaSFDown", &evtwei_DiPhoLooseMvaSFDown);
    Tout->Branch("evtwei_DiPhoPreselSFUp", &evtwei_DiPhoPreselSFUp);
    Tout->Branch("evtwei_DiPhoPreselSFDown", &evtwei_DiPhoPreselSFDown);
    Tout->Branch("evtwei_DiPhoelectronVetoSFUp", &evtwei_DiPhoelectronVetoSFUp);
    Tout->Branch("evtwei_DiPhoelectronVetoSFDown", &evtwei_DiPhoelectronVetoSFDown);
    Tout->Branch("evtwei_DiPhoTriggerWeightUp", &evtwei_DiPhoTriggerWeightUp);
    Tout->Branch("evtwei_DiPhoTriggerWeightDown", &evtwei_DiPhoTriggerWeightDown);
    // Electron syst
    Tout->Branch("evtwei_ElecRecoUp", &evtwei_ElecRecoUp);
    Tout->Branch("evtwei_ElecRecoDown", &evtwei_ElecRecoDown);
    Tout->Branch("evtwei_ElecIdUp", &evtwei_ElecIdUp);
    Tout->Branch("evtwei_ElecIdDown", &evtwei_ElecIdDown);
    // Muon syst
    Tout->Branch("evtwei_MuonIdUp", &evtwei_MuonIdUp);
    Tout->Branch("evtwei_MuonIdDown", &evtwei_MuonIdDown);
    Tout->Branch("evtwei_MuonIsoUp", &evtwei_MuonIsoUp);
    Tout->Branch("evtwei_MuonIsoDown", &evtwei_MuonIsoDown);
    // b-tag syst
    Tout->Branch("evtwei_btagjesUp", &evtwei_btagjesUp);
    Tout->Branch("evtwei_btagjesDown", &evtwei_btagjesDown);
    Tout->Branch("evtwei_btaglfUp", &evtwei_btaglfUp);
    Tout->Branch("evtwei_btaglfDown", &evtwei_btaglfDown);
    Tout->Branch("evtwei_btaghfUp", &evtwei_btaghfUp);
    Tout->Branch("evtwei_btaghfDown", &evtwei_btaghfDown);
    Tout->Branch("evtwei_btaglfstats1Up", &evtwei_btaglfstats1Up);
    Tout->Branch("evtwei_btaglfstats1Down", &evtwei_btaglfstats1Down);
    Tout->Branch("evtwei_btaglfstats2Up", &evtwei_btaglfstats2Up);
    Tout->Branch("evtwei_btaglfstats2Down", &evtwei_btaglfstats2Down);
    Tout->Branch("evtwei_btaghfstats1Up", &evtwei_btaghfstats1Up);
    Tout->Branch("evtwei_btaghfstats1Down", &evtwei_btaghfstats1Down);
    Tout->Branch("evtwei_btaghfstats2Up", &evtwei_btaghfstats2Up);
    Tout->Branch("evtwei_btaghfstats2Down", &evtwei_btaghfstats2Down);
    Tout->Branch("evtwei_btagcferr1Up", &evtwei_btagcferr1Up);
    Tout->Branch("evtwei_btagcferr1Down", &evtwei_btagcferr1Down);
    Tout->Branch("evtwei_btagcferr2Up", &evtwei_btagcferr2Up);
    Tout->Branch("evtwei_btagcferr2Down", &evtwei_btagcferr2Down);

    // Event loop
    for (int evt=0; evt<Tin->GetEntries(); ++evt)
    {
        Tin->GetEntry(evt);

        // Central
        evtwei_central = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon * SF_btag;
        if (makeSyst) {
            // Photon syst
            evtwei_DiPhoLooseMvaSFUp = Evt_genweight * DiPho_LooseMvaSFUp * SF_pileup * SF_Elec * SF_Muon * SF_btag;
            evtwei_DiPhoLooseMvaSFDown = Evt_genweight * DiPho_LooseMvaSFDown * SF_pileup * SF_Elec * SF_Muon * SF_btag;
            evtwei_DiPhoPreselSFUp = Evt_genweight * DiPho_PreselSFUp * SF_pileup * SF_Elec * SF_Muon * SF_btag;
            evtwei_DiPhoPreselSFDown = Evt_genweight * DiPho_PreselSFDown * SF_pileup * SF_Elec * SF_Muon * SF_btag;
            evtwei_DiPhoelectronVetoSFUp = Evt_genweight * DiPho_electronVetoSFUp * SF_pileup * SF_Elec * SF_Muon * SF_btag;
            evtwei_DiPhoelectronVetoSFDown = Evt_genweight * DiPho_electronVetoSFDown * SF_pileup * SF_Elec * SF_Muon * SF_btag;
            evtwei_DiPhoTriggerWeightUp = Evt_genweight * DiPho_TriggerWeightUp * SF_pileup * SF_Elec * SF_Muon * SF_btag;
            evtwei_DiPhoTriggerWeightDown = Evt_genweight * DiPho_TriggerWeightDown * SF_pileup * SF_Elec * SF_Muon * SF_btag;
            // Electron syst
            evtwei_ElecRecoUp = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec_RecoUp * SF_Muon * SF_btag;
            evtwei_ElecRecoDown = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec_RecoDown * SF_Muon * SF_btag;
            evtwei_ElecIdUp = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec_IdUp * SF_Muon * SF_btag;
            evtwei_ElecIdDown = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec_IdDown * SF_Muon * SF_btag;
            // Muon syst
            evtwei_MuonIdUp = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon_IdUp * SF_btag;
            evtwei_MuonIdDown = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon_IdDown * SF_btag;
            evtwei_MuonIsoUp = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon_IsoUp * SF_btag;
            evtwei_MuonIsoDown = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon_IsoDown * SF_btag;
            // b-tag syst
            evtwei_btagjesUp = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon * SF_btag_jesUp;
            evtwei_btagjesDown = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon * SF_btag_jesDown;
            evtwei_btaglfUp = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon * SF_btag_lfUp;
            evtwei_btaglfDown = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon * SF_btag_lfDown;
            evtwei_btaghfUp = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon * SF_btag_hfUp;
            evtwei_btaghfDown = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon * SF_btag_hfDown;
            evtwei_btaglfstats1Up = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon * SF_btag_lfstats1Up;
            evtwei_btaglfstats1Down = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon * SF_btag_lfstats1Down;
            evtwei_btaglfstats2Up = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon * SF_btag_lfstats2Up;
            evtwei_btaglfstats2Down = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon * SF_btag_lfstats2Down;
            evtwei_btaghfstats1Up = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon * SF_btag_hfstats1Up;
            evtwei_btaghfstats1Down = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon * SF_btag_hfstats1Down;
            evtwei_btaghfstats2Up = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon * SF_btag_hfstats2Up;
            evtwei_btaghfstats2Down = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon * SF_btag_hfstats2Down;
            evtwei_btagcferr1Up = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon * SF_btag_cferr1Up;
            evtwei_btagcferr1Down = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon * SF_btag_cferr1Down;
            evtwei_btagcferr2Up = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon * SF_btag_cferr2Up;
            evtwei_btagcferr2Down = Evt_genweight * DiPho_centralWeight * SF_pileup * SF_Elec * SF_Muon * SF_btag_cferr2Down;
        }

        Tout->Fill();
    } // End event loop

    fin->WriteTObject(Tout, "", "Overwrite");
    fin->Close();

    return 0;
}
