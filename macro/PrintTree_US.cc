void PrintTree_US(const char *filename)
{
	TChain nt("tthLeptonicTagDumper/trees/tth_13TeV_all");
	nt.Add(filename);

	nt.Print();
}
