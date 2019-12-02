#include "HistVar.h"

#include "TH1D.h"

void CreateHist(TH1D **hist)
{
    hist[elec_N] = new TH1D("elec_N", ";number of electrons;", 7, -0.5, 6.5);
    hist[elec_charge] = new TH1D("elec_charge", ";electron charge;", 5, -2.5, 2.5);
    hist[elec_pt] = new TH1D("elec_pt", ";electron p_{T} (GeV);", 12, 0., 250.);
    hist[elec_eta] = new TH1D("elec_eta", ";electron #eta;", 12, -3., 3.);
    hist[elec_phi] = new TH1D("elec_phi", ";electron #phi;", 12, -3.5, 3.5);
    hist[elec_E] = new TH1D("elec_E", ";electron E (GeV);", 12, 0., 500.);

    hist[muon_N] = new TH1D("muon_N", ";number of muons;", 7, -0.5, 6.5);
    hist[muon_charge] = new TH1D("muon_charge", ";muon charge;", 5, -2.5, 2.5);
    hist[muon_pt] = new TH1D("muon_pt", ";muon p_{T} (GeV);", 12, 0., 250.);
    hist[muon_eta] = new TH1D("muon_eta", ";muon #eta;", 12, -3., 3.);
    hist[muon_phi] = new TH1D("muon_phi", ";muon #phi;", 12, -3.5, 3.5);
    hist[muon_E] = new TH1D("muon_E", ";muon E (GeV);", 12, 0., 500.);

    hist[lep_N] = new TH1D("lep_N", ";number of leptons;", 7, -0.5, 6.5);
    hist[lep_charge] = new TH1D("lep_charge", ";lepton charge;", 5, -2.5, 2.5);
    hist[lep_pt] = new TH1D("lep_pt", ";lepton p_{T} (GeV);", 12, 0., 250.);
    hist[lep_eta] = new TH1D("lep_eta", ";lepton #eta;", 12, -3., 3.);
    hist[lep_phi] = new TH1D("lep_phi", ";lepton #phi;", 12, -3.5, 3.5);
    hist[lep_E] = new TH1D("lep_E", ";lepton E (GeV);", 12, 0., 500.);

    char unit[nhist][32] = {
	"",
	"",
	"GeV",
	"",
	"",
	"GeV",
	//////////
	"",
	"",
	"GeV",
	"",
	"",
	"GeV",
	//////////
	"",
	"",
	"GeV",
	"",
	"",
	"GeV"
    };

    for (int i=0; i<nhist; ++i) {
	double width = (hist[i]->GetXaxis()->GetXmax() - hist[i]->GetXaxis()->GetXmin()) / hist[i]->GetNbinsX();
	hist[i]->SetYTitle( Form("events / %.2g %s", width, unit[i]) );
	hist[i]->Sumw2();
    }
}
