void normfactor_GGJets()
{
    TFile *fin = new TFile("SumWeight_DiPhotonJetsBox.root");
    TH1D *hin = (TH1D*)fin->Get("weight");
    float sumwei = hin->GetBinContent(1);

    TFile *fout = new TFile("DiPhotonJetsBox.root", "recreate");
    TH1D *hout = new TH1D("normfactor", "", 1, 0, 1);

    float lumi = 41.53;
    float xs = 88.36;

    float nf = xs * lumi * 1000 / sumwei;
    hout->SetBinContent(1, nf);

    fout->cd();
    hout->Write();
    fout->Close();
    fin->Close();

    ofstream output("normfactor_GGJets.log", ios::out);
    output << "DiPhotonJetsBox\t" << nf << endl;
    output.close();
}
