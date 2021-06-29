#include "include/Paths.h"
#include "include/utility.h"

// Make reconstructed and gen-level neutrino Pz histograms (using ST lep Hut signal)
void plot_nuPz()
{
    // Paths
    TString fin_name = "ST_FCNC-TH_Tlep_hut";
    TString ntdir = dir_res + "Presel_lep_phID/";
    TString recodir = dir_res + "MVAreco_result_lep/";
    TString normdir = dir_res + "normfactor/";
    // Get files
    TFile *fnt = new TFile(ntdir+fin_name+".root");
    TFile *freco = new TFile(recodir+fin_name+".root");
    // Get trees
    TTree *Tnt = (TTree*)fnt->Get("T");
    TTree *Treco = (TTree*)freco->Get("Treco_ST");
    // Get norm
    float norm = GetNormFactor((normdir+fin_name+".root").Data());

    // Set n-tuple tree branches
    float evtwei_central = 0;
    int GenPartInfo_size = 0;
    vector<float> *GenPartInfo_Pt = 0;
    vector<float> *GenPartInfo_Eta = 0;
    vector<float> *GenPartInfo_Phi = 0;
    vector<int> *GenPartInfo_PdgID = 0;
    vector<int> *GenPartInfo_MomPdgID = 0;
    Tnt->SetBranchAddress("evtwei_central", &evtwei_central);
    Tnt->SetBranchAddress("GenPartInfo.size", &GenPartInfo_size);
    Tnt->SetBranchAddress("GenPartInfo.Pt", &GenPartInfo_Pt);
    Tnt->SetBranchAddress("GenPartInfo.Eta", &GenPartInfo_Eta);
    Tnt->SetBranchAddress("GenPartInfo.Phi", &GenPartInfo_Phi);
    Tnt->SetBranchAddress("GenPartInfo.PdgID", &GenPartInfo_PdgID);
    Tnt->SetBranchAddress("GenPartInfo.MomPdgID", &GenPartInfo_MomPdgID);
    // Set reco tree branches
    float nu_Pz = 0;
    Treco->SetBranchAddress("nu_Pz", &nu_Pz);

    // Make neutrino Pz histogram
    TFile *fout = new TFile(dir_res+"thesis/plot_nuPz.root", "recreate");
    TH1F *hreco = new TH1F("hreco", ";p_{z} (GeV);entries / 15 GeV", 40, -300, 300);
    TH1F *hgen = new TH1F("hgen", ";p_{z} (GeV);entries / 15 GeV", 40, -300, 300);
    TH2F *hcorr = new TH2F("hcorr", ";p_{z}^{gen} (GeV);p_{z}^{reco} (GeV)", 40, -200, 200, 40, -200, 200);

    // Start event loop
    for (int evt=0; evt<Tnt->GetEntries(); ++evt) {
        Tnt->GetEntry(evt);
        Treco->GetEntry(evt);

        // Fill reconstructed Pz
        hreco->Fill(nu_Pz, evtwei_central*norm);

        for (int i=0; i<GenPartInfo_size; ++i) {
            // Find gen-level neutrino
            if ( ((fabs(GenPartInfo_PdgID->at(i)) == 12) || (fabs(GenPartInfo_PdgID->at(i)) == 14)) &&
                    (fabs(GenPartInfo_MomPdgID->at(i)) == 24) ) {
                TLorentzVector nu_gen;
                nu_gen.SetPtEtaPhiM( GenPartInfo_Pt->at(i), GenPartInfo_Eta->at(i), GenPartInfo_Phi->at(i), 0. );
                // Fill gen-leval Pz
                hgen->Fill(nu_gen.Pz(), evtwei_central*norm);
                // Fill 2-D hist
                hcorr->Fill(nu_gen.Pz(), nu_Pz, evtwei_central*norm);
                break;
            }
        }
    } // End event loop

    hreco->Write();
    hgen->Write();
    hcorr->Write();
    fout->Close();
}


void draw_nuPz()
{
    // Get histograms
    TString indir = dir_res + "thesis/";
    TFile *fin = new TFile( indir+"plot_nuPz.root" );
    TH1F *hreco = (TH1F*)fin->Get("hreco");
    TH1F *hgen = (TH1F*)fin->Get("hgen");
    TH2F *hcorr = (TH2F*)fin->Get("hcorr");

    hreco->SetLineColor(kBlue);
    hgen->SetLineColor(kOrange);
    hreco->SetLineWidth(2);
    hgen->SetLineWidth(2);
    TLegend *leg = new TLegend(0.15, 0.65, 0.4, 0.85);
    leg->AddEntry(hreco, "Reconstructed", "l");
    leg->AddEntry(hgen, "Generator", "l");

    TLatex *tex = new TLatex();
    tex->SetTextAlign(10);

    // Draw 1-D reco-gen comparison
    TCanvas *c = new TCanvas();
    hreco->SetStats(false);
    hreco->Draw("hist");
    hgen->Draw("histsame");
    leg->Draw();
    tex->DrawLatexNDC(0.65, 0.75, "#sigma_{reco}=71.18");
    tex->DrawLatexNDC(0.65, 0.65, "#sigma_{gen}=92.46");
    hreco->SetTitleSize(0.04, "x");
    hreco->SetTitleSize(0.04, "y");
    hreco->SetTitleOffset(1.2, "y");
    c->SaveAs(indir+"nuPz_1D.png");
    // Draw 2-D correlation plot
    c->SetRightMargin(0.15);
    hcorr->SetStats(false);
    hcorr->SetTitleSize(0.04, "x");
    hcorr->SetTitleSize(0.04, "y");
    hcorr->SetTitleOffset(1., "y");
    hcorr->Draw("colz");
    c->SaveAs(indir+"nuPz_2D.png");

    fin->Close();
}
