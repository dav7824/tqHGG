void CollectTree_had()
{
    TString dir17 = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/";
    TTree *T[5], *Tout[5];
    TFile *fin = new TFile(dir17+"Presel_had_phID_btag-L/data.root");
    TFile *fin2 = new TFile(dir17+"MVArecoV3_result_had/ANN_opt/data.root");
    T[0] = (TTree*)fin->Get("T");
    T[1] = (TTree*)fin->Get("SF_pileup");
    T[2] = (TTree*)fin->Get("SF_btag");
    T[3] = (TTree*)fin2->Get("Treco_TT");
    T[4] = (TTree*)fin2->Get("Treco_ST");

    TFile *fout = new TFile(dir17+"data.root", "recreate");
    for (int i=0; i<5; ++i) {
        Tout[i] = T[i]->CopyTree("");
        fout->WriteTObject(Tout[i]);
    }

    fout->Close();
    fin->Close();
    fin2->Close();
}

void CollectTree_lep()
{
    TString dir17 = "/wk_cms2/mc_cheng/public/tqHGG/2017_v2/";
    TTree *T[7], *Tout[7];
    TFile *fin = new TFile(dir17+"Presel_lep_phID/data.root");
    TFile *fin2 = new TFile(dir17+"MVArecoV3_result_lep/ANN_opt/data.root");
    T[0] = (TTree*)fin->Get("T");
    T[1] = (TTree*)fin->Get("SF_pileup");
    T[2] = (TTree*)fin->Get("SF_btag");
    T[3] = (TTree*)fin->Get("SF_Elec");
    T[4] = (TTree*)fin->Get("SF_Muon");
    T[5] = (TTree*)fin2->Get("Treco_TT");
    T[6] = (TTree*)fin2->Get("Treco_ST");

    TFile *fout = new TFile(dir17+"data.root", "recreate");
    for (int i=0; i<7; ++i) {
        Tout[i] = T[i]->CopyTree("");
        fout->WriteTObject(Tout[i]);
    }

    fout->Close();
    fin->Close();
    fin2->Close();
}

void CollectTree()
{
    //CollectTree_had();
    CollectTree_lep();
}
