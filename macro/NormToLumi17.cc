void NormToLumi17()
{
    vector<TString> nt_name = {
	"GJet_Pt-20to40_MGG-80toInf", "GJet_Pt-20toInf_MGG-40to80", "GJet_Pt-40toInf_MGG-80toInf",
	"QCD_Pt-30to40_MGG-80toInf", "QCD_Pt-30toInf_MGG-40to80", "QCD_Pt-40toInf_MGG-80toInf",
	"TGJets", "tW_antitop", "tW_top",
	"TTGG", "TTGJets", "TTJets", "TTWJetsToLNu", "TTZToLLNuNu",
	"DYJetsToLL", "WGToLNuG", "ZGToLLG",
	"WW", "WZ", "ZZ",
	"GluGluHToGG", "VBFHToGG", "VHToGG", "ttHJetToGG",
	"THQ_HToGG", "THW_HToGG"
    };
    vector<TString> file_name = {
	"GJet_Pt-20to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root",
	"GJet_Pt-20toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8.root",
	"GJet_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root",
	"QCD_Pt-30to40_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root",
	"QCD_Pt-30toInf_DoubleEMEnriched_MGG-40to80_TuneCP5_13TeV_Pythia8.root",
	"QCD_Pt-40toInf_DoubleEMEnriched_MGG-80toInf_TuneCP5_13TeV_Pythia8.root",
	"TGJets_TuneCP5_13TeV_amcatnlo_madspin_pythia8.root",
	"ST_tW_antitop_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8.root",
	"ST_tW_top_5f_inclusiveDecays_TuneCP5_13TeV-powheg-pythia8.root",
	"TTGG_0Jets_TuneCP5_13TeV_amcatnlo_madspin_pythia8.root",
	"TTGJets_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8.root",
	"TTJets_TuneCP5_13TeV-amcatnloFXFX-pythia8.root",
	"TTWJetsToLNu_TuneCP5_13TeV-amcatnloFXFX-madspin-pythia8.root",
	"TTZToLLNuNu_M-10_TuneCP5_13TeV-amcatnlo-pythia8.root",
	"DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8.root",
	"WGToLNuG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8.root",
	"ZGToLLG_01J_5f_TuneCP5_13TeV-amcatnloFXFX-pythia8.root",
	"WW_TuneCP5_13TeV-pythia8.root",
	"WZ_TuneCP5_13TeV-pythia8.root",
	"ZZ_TuneCP5_13TeV-pythia8.root",
	"GluGluHToGG_M125_13TeV_amcatnloFXFX_pythia8.root",
	"VBFHToGG_M125_13TeV_amcatnlo_pythia8.root",
	"VHToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8.root",
	"ttHJetToGG_M125_13TeV_amcatnloFXFX_madspin_pythia8.root",
	"THQ_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCP5.root",
	"THW_ctcvcp_HToGG_M125_13TeV-madgraph-pythia8_TuneCP5.root"
    };
    vector<double> xs = { // unit: pb
	232.8, 3164.0, 872.8,
	24750.0, 242700.0, 117400.0,
	3.055, 34.97, 34.91,
	0.01687, 4.078, 722.8, 0.2149, 0.2432,
	6529.0, 489.0, 117.864,
	75.8, 27.6, 12.14,
	33.4*0.0023, 3.992*0.0023, 2.449*0.0023, 0.5101*0.0023, // multiply resonant bkg x-sec by BR(H->GG)=0.23%
	0.7301*0.0023, 0.1464*0.0023
    };
    double lumi = 41.529; // unit: 1/fb

    int n_nt = nt_name.size();
    vector<double> sum_genweight, normfactor;
    sum_genweight.resize(n_nt);
    normfactor.resize(n_nt);

    for (int i=0; i<n_nt; ++i) {
	cout << "Processing n-tuple: " << nt_name[i] << endl;
	TFile *fin = new TFile(TString("/wk_cms2/youying/public/thFCNC/flashgg_105X/2017/") + file_name[i]);
	TH1D *hist = (TH1D*)fin->Get("flashggNtuples/h1_weight");
	sum_genweight[i] = hist->GetBinContent(1);
	normfactor[i] = lumi * xs[i] * 1000 / sum_genweight[i];
	fin->Close();
    }

    // process DiPhotonJetsBox n-tuple
    cout << "Processing n-tuple: DiPhotonJetsBox\n";
    double sum_genweight2 = 0;
    double normfactor2 = 0;
    char subfile_name[256];
    for (int i=0; i<=517; ++i) {
	sprintf(subfile_name, "/wk_cms2/youying/public/thFCNC/flashgg_105X/2017/DiPhotonJetsBox_MGG-80toInf_13TeV-Sherpa/output%d.root", i);
	TFile *fin = new TFile(subfile_name);
	TH1D *hist = (TH1D*)fin->Get("flashggNtuples/h1_weight");
	sum_genweight2 += hist->GetBinContent(1);
	fin->Close();
    }
    normfactor2 = 88.36 * lumi * 1000 / sum_genweight2;

    // print the result
    ofstream output("/wk_cms2/mc_cheng/public/tqHGG/2017/normfactor17.txt", ios::out);
    output << "DiPhotonJetsBox\t\t" << normfactor2 << endl;
    for (int i=0; i<n_nt; ++i) {
	output << nt_name[i] << "\t\t" << normfactor[i] << endl;
    }
    output.close();

    // save the result to root files
    TFile *fout = 0;
    TH1D *h_nf = 0;
    TString outdir = "/wk_cms2/mc_cheng/public/tqHGG/2017/normfactor/";

    fout = new TFile(outdir + "DiPhotonJetsBox.root", "recreate");
    h_nf = new TH1D("normfactor", "", 1, 0, 1);
    h_nf->SetBinContent(1, normfactor2); h_nf->Write();
    fout->Close();
    for (int i=0; i<n_nt; ++i) {
	fout = new TFile(outdir + nt_name[i] + ".root", "recreate");
	h_nf = new TH1D("normfactor", "", 1, 0, 1);
	h_nf->SetBinContent(1, normfactor[i]); h_nf->Write();
	fout->Close();
    }

    cout << "Complete!!!\n";
}
