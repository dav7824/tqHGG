/*
 * Scale event weights of a sample. (after dividing the sample)
 * Usage:
 *   ./ScaleWeight <fin> <SF> <syst|nosyst>
 *
 * If "syst" is specified, all systematic weights are also scaled. Otherwise, only central
 * weight is scaled, and the syst weights are kept at the null values.
 */

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <cstdlib>
using namespace std;

int main(int argc, char **argv)
{
    // Get command line arguments
    TString fin_name(argv[1]);
    TString SF_(argv[2]);
    TString flag_syst_(argv[3]);
    float SF = atof(SF_.Data());
    bool flag_syst = false;
    if (flag_syst_ == "syst")  flag_syst = true;

    // Read file
    TFile *fin = new TFile(fin_name, "update");
    TTree *Tin = (TTree*)fin->Get("T");
    // Event weight variables
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
    Tin->SetBranchAddress("evtwei_central", &evtwei_central);
    // Photon syst
    Tin->SetBranchAddress("evtwei_DiPhoLooseMvaSFUp", &evtwei_DiPhoLooseMvaSFUp);
    Tin->SetBranchAddress("evtwei_DiPhoLooseMvaSFDown", &evtwei_DiPhoLooseMvaSFDown);
    Tin->SetBranchAddress("evtwei_DiPhoPreselSFUp", &evtwei_DiPhoPreselSFUp);
    Tin->SetBranchAddress("evtwei_DiPhoPreselSFDown", &evtwei_DiPhoPreselSFDown);
    Tin->SetBranchAddress("evtwei_DiPhoelectronVetoSFUp", &evtwei_DiPhoelectronVetoSFUp);
    Tin->SetBranchAddress("evtwei_DiPhoelectronVetoSFDown", &evtwei_DiPhoelectronVetoSFDown);
    Tin->SetBranchAddress("evtwei_DiPhoTriggerWeightUp", &evtwei_DiPhoTriggerWeightUp);
    Tin->SetBranchAddress("evtwei_DiPhoTriggerWeightDown", &evtwei_DiPhoTriggerWeightDown);
    // Electron syst
    Tin->SetBranchAddress("evtwei_ElecRecoUp", &evtwei_ElecRecoUp);
    Tin->SetBranchAddress("evtwei_ElecRecoDown", &evtwei_ElecRecoDown);
    Tin->SetBranchAddress("evtwei_ElecIdUp", &evtwei_ElecIdUp);
    Tin->SetBranchAddress("evtwei_ElecIdDown", &evtwei_ElecIdDown);
    // Muon syst
    Tin->SetBranchAddress("evtwei_MuonIdUp", &evtwei_MuonIdUp);
    Tin->SetBranchAddress("evtwei_MuonIdDown", &evtwei_MuonIdDown);
    Tin->SetBranchAddress("evtwei_MuonIsoUp", &evtwei_MuonIsoUp);
    Tin->SetBranchAddress("evtwei_MuonIsoDown", &evtwei_MuonIsoDown);
    // b-tag syst
    Tin->SetBranchAddress("evtwei_btagjesUp", &evtwei_btagjesUp);
    Tin->SetBranchAddress("evtwei_btagjesDown", &evtwei_btagjesDown);
    Tin->SetBranchAddress("evtwei_btaglfUp", &evtwei_btaglfUp);
    Tin->SetBranchAddress("evtwei_btaglfDown", &evtwei_btaglfDown);
    Tin->SetBranchAddress("evtwei_btaghfUp", &evtwei_btaghfUp);
    Tin->SetBranchAddress("evtwei_btaghfDown", &evtwei_btaghfDown);
    Tin->SetBranchAddress("evtwei_btaglfstats1Up", &evtwei_btaglfstats1Up);
    Tin->SetBranchAddress("evtwei_btaglfstats1Down", &evtwei_btaglfstats1Down);
    Tin->SetBranchAddress("evtwei_btaglfstats2Up", &evtwei_btaglfstats2Up);
    Tin->SetBranchAddress("evtwei_btaglfstats2Down", &evtwei_btaglfstats2Down);
    Tin->SetBranchAddress("evtwei_btaghfstats1Up", &evtwei_btaghfstats1Up);
    Tin->SetBranchAddress("evtwei_btaghfstats1Down", &evtwei_btaghfstats1Down);
    Tin->SetBranchAddress("evtwei_btaghfstats2Up", &evtwei_btaghfstats2Up);
    Tin->SetBranchAddress("evtwei_btaghfstats2Down", &evtwei_btaghfstats2Down);
    Tin->SetBranchAddress("evtwei_btagcferr1Up", &evtwei_btagcferr1Up);
    Tin->SetBranchAddress("evtwei_btagcferr1Down", &evtwei_btagcferr1Down);
    Tin->SetBranchAddress("evtwei_btagcferr2Up", &evtwei_btagcferr2Up);
    Tin->SetBranchAddress("evtwei_btagcferr2Down", &evtwei_btagcferr2Down);

    // Create new tree
    TTree *Tout = Tin->CloneTree(0);

    // Start event loop
    for (int evt=0; evt<Tin->GetEntries(); ++evt) {
        Tin->GetEntry(evt);

        // Scale event weight
        // Central
        evtwei_central *= SF;
        if (flag_syst) {
            // Photon syst
            evtwei_DiPhoLooseMvaSFUp *= SF;
            evtwei_DiPhoLooseMvaSFDown *= SF;
            evtwei_DiPhoPreselSFUp *= SF;
            evtwei_DiPhoPreselSFDown *= SF;
            evtwei_DiPhoelectronVetoSFUp *= SF;
            evtwei_DiPhoelectronVetoSFDown *= SF;
            evtwei_DiPhoTriggerWeightUp *= SF;
            evtwei_DiPhoTriggerWeightDown *= SF;
            // Electron syst
            evtwei_ElecRecoUp *= SF;
            evtwei_ElecRecoDown *= SF;
            evtwei_ElecIdUp *= SF;
            evtwei_ElecIdDown *= SF;
            // Muon syst
            evtwei_MuonIdUp *= SF;
            evtwei_MuonIdDown *= SF;
            evtwei_MuonIsoUp *= SF;
            evtwei_MuonIsoDown *= SF;
            // b-tag syst
            evtwei_btagjesUp *= SF;
            evtwei_btagjesDown *= SF;
            evtwei_btaglfUp *= SF;
            evtwei_btaglfDown *= SF;
            evtwei_btaghfUp *= SF;
            evtwei_btaghfDown *= SF;
            evtwei_btaglfstats1Up *= SF;
            evtwei_btaglfstats1Down *= SF;
            evtwei_btaglfstats2Up *= SF;
            evtwei_btaglfstats2Down *= SF;
            evtwei_btaghfstats1Up *= SF;
            evtwei_btaghfstats1Down *= SF;
            evtwei_btaghfstats2Up *= SF;
            evtwei_btaghfstats2Down *= SF;
            evtwei_btagcferr1Up *= SF;
            evtwei_btagcferr1Down *= SF;
            evtwei_btagcferr2Up *= SF;
            evtwei_btagcferr2Down *= SF;
        }

        Tout->Fill();
    } // End event loop

    fin->WriteTObject(Tout, "", "Overwrite");
    fin->Close();

    return 0;
}
