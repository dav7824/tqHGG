// event
MakeFillHist(inTree, "EvtInfo.NVtx", weight, "evt_Nvtx", "number of vertices",  40, 0,  80);
MakeFillHist(inTree,  "EvtInfo.Rho", weight,  "evt_rho",               "#rho",  40, 0,  60);
// diphoton
MakeFillHist(inTree,        "DiPhoInfo.mass", weight, "dipho_mass", "diphoton mass", 40, 100, 360, "GeV");
MakeFillHist(inTree,          "DiPhoInfo.pt", weight,   "dipho_pt",   "diphoton Pt", 40,   0, 360, "GeV");
MakeFillHist(inTree, "DiPhoInfo.diphotonMVA", weight,  "dipho_mva",  "diphoton MVA", 40,  -1,   1);
// leading photon
MakeFillHist(inTree,           "DiPhoInfo.leadPt", weight,        "pho1_pt",              "leading #gamma Pt", 40,   35,  275, "GeV");
MakeFillHist(inTree,          "DiPhoInfo.leadEta", weight,       "pho1_eta",            "leading #gamma #eta", 40,   -3,    3);
MakeFillHist(inTree,          "DiPhoInfo.leadPhi", weight,       "pho1_phi",            "leading #gamma #phi", 40, -3.5,  3.5);
MakeFillHist(inTree,            "DiPhoInfo.leadE", weight,         "pho1_E",          "leading #gamma energy", 40,    0, 800, "GeV");
MakeFillHist(inTree,        "DiPhoInfo.leadIDMVA", weight,     "pho1_idmva",           "leading #gamma IDMVA", 40,   -1,    1);
MakeFillHist(inTree, "DiPhoInfo.leadhasPixelSeed", weight, "pho1_PixelSeed",  "leading #gamma has pixel seed",  2,    0,    2);
// subleading photon
MakeFillHist(inTree,           "DiPhoInfo.subleadPt", weight,        "pho2_pt",              "subleading #gamma Pt", 40,   25,  145, "GeV");
MakeFillHist(inTree,          "DiPhoInfo.subleadEta", weight,       "pho2_eta",            "subleading #gamma #eta", 40,   -3,    3);
MakeFillHist(inTree,          "DiPhoInfo.subleadPhi", weight,       "pho2_phi",            "subleading #gamma #phi", 40, -3.5,  3.5);
MakeFillHist(inTree,            "DiPhoInfo.subleadE", weight,         "pho2_E",          "subleading #gamma energy", 40,    0, 320, "GeV");
MakeFillHist(inTree,        "DiPhoInfo.subleadIDMVA", weight,     "pho2_idmva",           "subleading #gamma IDMVA", 40,   -1,    1);
MakeFillHist(inTree, "DiPhoInfo.subleadhasPixelSeed", weight, "pho2_PixelSeed",  "subleading #gamma has pixel seed",  2,    0,    2);
// jet
MakeFillHist(inTree,                       "jets_size", weight,      "jet_N",       "jet N", 13, -0.5, 12.5);
MakeFillHist(inTree,                      "JetInfo.Pt", weight,     "jet_pt",      "jet Pt", 40,   25,  225, "GeV");
MakeFillHist(inTree,                     "JetInfo.Eta", weight,    "jet_eta",    "jet #eta", 40,   -3,    3);
MakeFillHist(inTree,                     "JetInfo.Phi", weight,    "jet_phi",    "jet #phi", 40, -3.5,  3.5);
MakeFillHist(inTree,                    "JetInfo.Mass", weight,      "jet_M",    "jet mass", 40,    0,   40, "GeV");
MakeFillHist(inTree,                  "JetInfo.Energy", weight,      "jet_E",  "jet energy", 40,    0,  500, "GeV");
MakeFillHist(inTree,  "JetInfo.pfDeepCSVJetTags_probb", weight,  "jet_probb",  "jet prob_b", 40,    0,    1);
MakeFillHist(inTree, "JetInfo.pfDeepCSVJetTags_probbb", weight, "jet_probbb", "jet prob_bb", 40,    0,    1);
// MET
MakeFillHist(inTree,    "MetInfo.Pt", weight,    "met_pt",          "MET Pt", 40,    0,  140, "GeV");
MakeFillHist(inTree,   "MetInfo.Phi", weight,   "met_phi",        "MET #phi", 40, -3.5,  3.5);
MakeFillHist(inTree,    "MetInfo.Px", weight,    "met_px",          "MET Px", 40, -150,  150, "GeV");
MakeFillHist(inTree,    "MetInfo.Py", weight,    "met_py",          "MET Py", 40, -150,  150, "GeV");
MakeFillHist(inTree, "MetInfo.SumET", weight, "met_sumET", "MET E_{T}^{sum}", 40,    0, 4000, "GeV");
/*
if (channel == "lep" || channel == "all") {
	// electron
	MakeFillHist(inTree,   "ElecInfo.Size", weight,   "elec_N",      "electron N",  7, -0.5,  6.5);
	MakeFillHist(inTree, "ElecInfo.Charge", weight,   "elec_Q", "electron charge",  5, -2.5,  2.5);
	MakeFillHist(inTree,     "ElecInfo.Pt", weight,  "elec_pt",     "electron Pt", 40,    0,  120, "GeV");
	MakeFillHist(inTree,    "ElecInfo.Eta", weight, "elec_eta",   "electron #eta", 40,   -3,    3);
	MakeFillHist(inTree,    "ElecInfo.Phi", weight, "elec_phi",   "electron #phi", 40, -3.5,  3.5);
	MakeFillHist(inTree, "ElecInfo.Energy", weight,   "elec_E", "electron energy", 40,    0,  320, "GeV");
	// muon
	MakeFillHist(inTree,                  "MuonInfo.Size", weight,     "muon_N",         "muon N",  7, -0.5,  6.5);
	MakeFillHist(inTree,                "MuonInfo.Charge", weight,     "muon_Q",    "muon charge",  5, -2.5,  2.5);
	MakeFillHist(inTree,                    "MuonInfo.Pt", weight,    "muon_pt",        "muon Pt", 40,    0,  120, "GeV");
	MakeFillHist(inTree,                   "MuonInfo.Eta", weight,   "muon_eta",      "muon #eta", 40,   -3,    3);
	MakeFillHist(inTree,                   "MuonInfo.Phi", weight,   "muon_phi",      "muon #phi", 40, -3.5,  3.5);
	MakeFillHist(inTree,                "MuonInfo.Energy", weight,     "muon_E",    "muon energy", 40,    0,  320, "GeV");
	MakeFillHist(inTree, "MuonInfo.PFIsoDeltaBetaCorrR04", weight,   "muon_iso", "muon isolation", 40,    0, 0.28);
	// lepton
	CreateHist("lep_N", "lepton N", 7, -0.5, 6.5);
	inTree->Draw("ElecInfo.Size+MuonInfo.Size>>lep_N", weight);
	CopyHist("elec_Q", "lep_Q", "lepton charge") -> Add( (TH1D*)gDirectory->Get("muon_Q") );
	CopyHist("elec_pt", "lep_pt", "lepton Pt") -> Add( (TH1D*)gDirectory->Get("muon_pt") );
	CopyHist("elec_eta", "lep_eta", "lepton #eta") -> Add( (TH1D*)gDirectory->Get("muon_eta") );
	CopyHist("elec_phi", "lep_phi", "lepton #phi") -> Add( (TH1D*)gDirectory->Get("muon_phi") );
	CopyHist("elec_E", "lep_E", "lepton energy") -> Add( (TH1D*)gDirectory->Get("muon_E") );
}
*/
