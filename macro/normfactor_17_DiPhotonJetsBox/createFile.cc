void createFile()
{
    TFile *fout = new TFile("SumWeight_DiPhotonJetsBox.root", "recreate");
    TH1D *h = new TH1D("weight", "", 1, 0, 1);
    fout->Write();
    fout->Close();
}
