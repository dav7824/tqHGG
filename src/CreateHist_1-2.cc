#include "HistVar.h"

#include "TH1D.h"

void CreateHist(TH1D **hist)
{
    hist[dipho_mass] = new TH1D("dipho_mass", ";diphoton mass (GeV);", 16, 100., 180.);
    hist[dipho_pt] = new TH1D("dipho_pt", ";diphoton p_{T} (GeV);", 16, 0., 400.);

    hist[pho1_pt] = new TH1D("pho1_pt", ";leading photon p_{T} (GeV);", 16, 0., 300.);
    hist[pho1_eta] = new TH1D("pho1_eta", ";leading photon #eta;", 16, -3., 3.);
    hist[pho1_phi] = new TH1D("pho1_phi", ";leading photon #phi;", 16, -3.5, 3.5);
    hist[pho1_E] = new TH1D("pho1_E", ";leading photon E (GeV);", 16, 0., 600.);
    hist[pho1_idmva] = new TH1D("pho1_idmva", ";leading photon IDMVA;", 16, -1., 1.);

    hist[pho2_pt] = new TH1D("pho2_pt", ";subleading photon p_{T} (GeV);", 16, 0., 150.);
    hist[pho2_eta] = new TH1D("pho2_eta", ";subleading photon #eta;", 16, -3., 3.);
    hist[pho2_phi] = new TH1D("pho2_phi", ";subleading photon #phi;", 16, -3.5, 3.5);
    hist[pho2_E] = new TH1D("pho2_E", ";subleading photon E (GeV);", 16, 0., 300.);
    hist[pho2_idmva] = new TH1D("pho2_idmva", ";subleading photon IDMVA;", 16, -1., 1.);

    hist[jet_N] = new TH1D("jet_N", ";number of jets;", 12, -0.5, 11.5);
    hist[jet_pt] = new TH1D("jet_pt", ";jet p_{T} (GeV);", 16, 0., 300.);
    hist[jet_eta] = new TH1D("jet_eta", ";jet #eta;", 16, -3., 3.);
    hist[jet_phi] = new TH1D("jet_phi", ";jet #phi;", 16, -3.5, 3.5);
    hist[jet_E] = new TH1D("jet_E", ";jet E (GeV);", 16, 0., 600.);
    hist[jet_M] = new TH1D("jet_M", ";jet mass (GeV);", 16, 0., 50.);

    hist[met_pt] = new TH1D("met_pt", ";MET p_{T} (GeV);", 16, 0., 200.);
    hist[met_phi] = new TH1D("met_phi", ";MET #phi;", 16, -3.5, 3.5);
    hist[met_px] = new TH1D("met_px", ";MET p_{x} (GeV);", 16, -200., 200.);
    hist[met_py] = new TH1D("met_py", ";MET p_{y} (GeV);", 16, -200., 200.);
    hist[met_SumET] = new TH1D("met_SumET", ";MET sum E_{T} (GeV);", 16, 0., 4000.);

    char unit[nhist][32] = {
	"GeV",
	"GeV",
	//////////
	"GeV",
	"",
	"",
	"GeV",
	"",
	"GeV",
	"",
	"",
	"GeV",
	"",
	//////////
	"",
	"GeV",
	"",
	"",
	"GeV",
	"GeV",
	//////////
	"GeV",
	"",
	"GeV",
	"GeV",
	"GeV"
    };

    for (int i=0; i<nhist; ++i) {
	double width = (hist[i]->GetXaxis()->GetXmax() - hist[i]->GetXaxis()->GetXmin()) / hist[i]->GetNbinsX();
	hist[i]->SetYTitle( Form("events / %.2g %s", width, unit[i]) );
	hist[i]->Sumw2();
    }
}
