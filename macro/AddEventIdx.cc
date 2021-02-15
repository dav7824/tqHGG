/*
 * Set event index for samples having strange index.
*/

#include "include/Paths.h"

void AddEvtIdx_sample(TString indir_name, TString nt)
{
    TString indir = dir_res + '/' + indir_name + '/';
    TString fin_name = indir+nt+".root";
    cout << "Processing: " << fin_name << endl;

    // Open input file
    TFile *fin = new TFile( fin_name, "update" );
    TTree *Tin = (TTree*)fin->Get("T");
    int EvtInfo_EvtNo = 0;
    Tin->SetBranchAddress("EvtInfo.EvtNo", &EvtInfo_EvtNo);
    // Create output tree
    TTree *Tout = Tin->CloneTree(0);

    // Start event loop
    for (int evt=0; evt<Tin->GetEntries(); ++evt) {
        Tin->GetEntry(evt);
        EvtInfo_EvtNo = evt;
        Tout->Fill();
    } // End event loop

    fin->WriteTObject(Tout, "", "Overwrite");
    fin->Close();
}

void AddEvtIdx_had(TString nt) {
    AddEvtIdx_sample("Presel_had_phID_btag-L", nt);
}

void AddEvtIdx_lep(TString nt) {
    AddEvtIdx_sample("Presel_lep_phID", nt);
}

void AddEventIdx()
{
    // hadronic channel
    AddEvtIdx_had("GJet_Pt-20to40_MGG-80toInf");
    AddEvtIdx_had("QCD_Pt-30to40_MGG-80toInf");
    AddEvtIdx_had("QCD_Pt-30toInf_MGG-40to80");
    AddEvtIdx_had("QCD_Pt-40toInf_MGG-80toInf");
    AddEvtIdx_had("Data-driven_QCD");
    // leptonic channel
    AddEvtIdx_lep("GJet_Pt-20to40_MGG-80toInf");
    AddEvtIdx_lep("QCD_Pt-30toInf_MGG-40to80");
    AddEvtIdx_lep("QCD_Pt-40toInf_MGG-80toInf");
}
