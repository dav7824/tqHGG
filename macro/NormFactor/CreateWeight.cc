void CreateWeight(TString fout_name)
{
    TFile *fout = new TFile(fout_name, "recreate");
    TH1D *h = new TH1D("hsumwei", "", 1, 0, 1);
    fout->Write();
    fout->Close();
}
