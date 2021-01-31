#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fin_name(argv[1]);

    // Read input
    TFile *fin = new TFile(fin_name, "update");
    TTree *Tin = (TTree*)fin->Get("T");

    // Create new tree
    TTree *Tout = Tin->CloneTree(0);
    // Pileup SF
    float SF_pileup = 1;
    // Electron SF
    float SF_Elec = 1;
    float SF_Elec_RecoUp = -999;
    float SF_Elec_RecoDown = -999;
    float SF_Elec_IdUp = -999;
    float SF_Elec_IdDown = -999;
    // Moun SF
    float SF_Muon = 1;
    float SF_Muon_IdUp = -999;
    float SF_Muon_IdDown = -999;
    float SF_Muon_IsoUp = -999;
    float SF_Muon_IsoDown = -999;
    // b-tagging SF
    float SF_btag = 1;
    float SF_btag_jesUp = -999;
    float SF_btag_jesDown = -999;
    float SF_btag_lfUp = -999;
    float SF_btag_lfDown = -999;
    float SF_btag_hfUp = -999;
    float SF_btag_hfDown = -999;
    float SF_btag_lfstats1Up = -999;
    float SF_btag_lfstats1Down = -999;
    float SF_btag_lfstats2Up = -999;
    float SF_btag_lfstats2Down = -999;
    float SF_btag_hfstats1Up = -999;
    float SF_btag_hfstats1Down = -999;
    float SF_btag_hfstats2Up = -999;
    float SF_btag_hfstats2Down = -999;
    float SF_btag_cferr1Up = -999;
    float SF_btag_cferr1Down = -999;
    float SF_btag_cferr2Up = -999;
    float SF_btag_cferr2Down = -999;
    // Event weight
    float evtwei_central = 1;
    float evtwei_DiPhoLooseMvaSFUp = -999;
    float evtwei_DiPhoLooseMvaSFDown = -999;
    float evtwei_DiPhoPreselSFUp = -999;
    float evtwei_DiPhoPreselSFDown = -999;
    float evtwei_DiPhoelectronVetoSFUp = -999;
    float evtwei_DiPhoelectronVetoSFDown = -999;
    float evtwei_DiPhoTriggerWeightUp = -999;
    float evtwei_DiPhoTriggerWeightDown = -999;
    float evtwei_ElecRecoUp = -999;
    float evtwei_ElecRecoDown = -999;
    float evtwei_ElecIdUp = -999;
    float evtwei_ElecIdDown = -999;
    float evtwei_MuonIdUp = -999;
    float evtwei_MuonIdDown = -999;
    float evtwei_MuonIsoUp = -999;
    float evtwei_MuonIsoDown = -999;
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
    // Pileup SF
    Tout->Branch("SF_pileup", &SF_pileup);
    // Electron SF
    Tout->Branch("SF_Elec", &SF_Elec);
    Tout->Branch("SF_Elec_RecoUp", &SF_Elec_RecoUp);
    Tout->Branch("SF_Elec_RecoDown", &SF_Elec_RecoDown);
    Tout->Branch("SF_Elec_IdUp", &SF_Elec_IdUp);
    Tout->Branch("SF_Elec_IdDown", &SF_Elec_IdDown);
    // Moun SF
    Tout->Branch("SF_Muon", &SF_Muon);
    Tout->Branch("SF_Muon_IdUp", &SF_Muon_IdUp);
    Tout->Branch("SF_Muon_IdDown", &SF_Muon_IdDown);
    Tout->Branch("SF_Muon_IsoUp", &SF_Muon_IsoUp);
    Tout->Branch("SF_Muon_IsoDown", &SF_Muon_IsoDown);
    // b-tagging SF
    Tout->Branch("SF_btag", &SF_btag);
    Tout->Branch("SF_btag_jesUp", &SF_btag_jesUp);
    Tout->Branch("SF_btag_jesDown", &SF_btag_jesDown);
    Tout->Branch("SF_btag_lfUp", &SF_btag_lfUp);
    Tout->Branch("SF_btag_lfDown", &SF_btag_lfDown);
    Tout->Branch("SF_btag_hfUp", &SF_btag_hfUp);
    Tout->Branch("SF_btag_hfDown", &SF_btag_hfDown);
    Tout->Branch("SF_btag_lfstats1Up", &SF_btag_lfstats1Up);
    Tout->Branch("SF_btag_lfstats1Down", &SF_btag_lfstats1Down);
    Tout->Branch("SF_btag_lfstats2Up", &SF_btag_lfstats2Up);
    Tout->Branch("SF_btag_lfstats2Down", &SF_btag_lfstats2Down);
    Tout->Branch("SF_btag_hfstats1Up", &SF_btag_hfstats1Up);
    Tout->Branch("SF_btag_hfstats1Down", &SF_btag_hfstats1Down);
    Tout->Branch("SF_btag_hfstats2Up", &SF_btag_hfstats2Up);
    Tout->Branch("SF_btag_hfstats2Down", &SF_btag_hfstats2Down);
    Tout->Branch("SF_btag_cferr1Up", &SF_btag_cferr1Up);
    Tout->Branch("SF_btag_cferr1Down", &SF_btag_cferr1Down);
    Tout->Branch("SF_btag_cferr2Up", &SF_btag_cferr2Up);
    Tout->Branch("SF_btag_cferr2Down", &SF_btag_cferr2Down);
    // Event weight
    Tout->Branch("evtwei_central", &evtwei_central);
    Tout->Branch("evtwei_DiPhoLooseMvaSFUp", &evtwei_DiPhoLooseMvaSFUp);
    Tout->Branch("evtwei_DiPhoLooseMvaSFDown", &evtwei_DiPhoLooseMvaSFDown);
    Tout->Branch("evtwei_DiPhoPreselSFUp", &evtwei_DiPhoPreselSFUp);
    Tout->Branch("evtwei_DiPhoPreselSFDown", &evtwei_DiPhoPreselSFDown);
    Tout->Branch("evtwei_DiPhoelectronVetoSFUp", &evtwei_DiPhoelectronVetoSFUp);
    Tout->Branch("evtwei_DiPhoelectronVetoSFDown", &evtwei_DiPhoelectronVetoSFDown);
    Tout->Branch("evtwei_DiPhoTriggerWeightUp", &evtwei_DiPhoTriggerWeightUp);
    Tout->Branch("evtwei_DiPhoTriggerWeightDown", &evtwei_DiPhoTriggerWeightDown);
    Tout->Branch("evtwei_ElecRecoUp", &evtwei_ElecRecoUp);
    Tout->Branch("evtwei_ElecRecoDown", &evtwei_ElecRecoDown);
    Tout->Branch("evtwei_ElecIdUp", &evtwei_ElecIdUp);
    Tout->Branch("evtwei_ElecIdDown", &evtwei_ElecIdDown);
    Tout->Branch("evtwei_MuonIdUp", &evtwei_MuonIdUp);
    Tout->Branch("evtwei_MuonIdDown", &evtwei_MuonIdDown);
    Tout->Branch("evtwei_MuonIsoUp", &evtwei_MuonIsoUp);
    Tout->Branch("evtwei_MuonIsoDown", &evtwei_MuonIsoDown);
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

    // Start event loop
    for (int evt=0; evt<Tin->GetEntries(); ++evt)
    {
        Tin->GetEntry(evt);
        Tout->Fill();
    } // End event loop

    fin->WriteTObject(Tout, "", "Overwrite");
    fin->Close();

    return 0;
}
