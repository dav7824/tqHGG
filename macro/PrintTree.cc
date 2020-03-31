void PrintTree(const char *filename)
{
	TChain nt("flashggNtuples/flashggStdTree");
	nt.Add(filename);

	nt.Print();
}
