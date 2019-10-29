#include "TreeReader.h"
#include "TreeWriter.h"
#include "utility.h"

#include <iostream>
#include <vector>
#include <string>
using namespace std;

void PrepareAnalysis(TreeReader&, TreeWriter&);
void Analyze(TreeReader&, TreeWriter&);

int main(int argc, char **argv)
{
    // get the command line arguments
    char *name = argv[1];		// n-tuple name
    vector<string> path;    ParseCStringList(argv[2], path);	// paths to the n-tuple root files
    char *inTreeName = argv[3];		// TTree Name of the input n-tuple
    char *outTreeName = argv[4];	// TTree Name of the output n-tuple
    char *outFile = argv[5];		// path & name of the output root file

    cout << "Start processing n-tuple: " << name << endl << endl;

    // reading input n-tuple files
    TreeReader inTree(inTreeName);
    for (int i=0; i<path.size(); ++i) {
	inTree.Add(path.at(i).c_str());
	cout << "Successfully opened root files: " << path.at(i) << endl;
    }
    cout << endl;
    inTree.Init();

    // create TreeWriter to output selected n-tuple
    TreeWriter outTree(outTreeName, outFile);
    outTree.Init();

    PrepareAnalysis(inTree, outTree);

    Analyze(inTree, outTree);

    outTree.Save();
    cout << "\nSelected n-tuple is saved at: " << outFile << endl;

    return 0;
}
