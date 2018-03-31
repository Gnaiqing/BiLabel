#include "BiLabel.h"
using namespace std;
int main()
{
	Graph *gp = new Graph(Util::MAX_NODE,Util::MAX_EDGE);
	string FileName;
	cout << "Please enter File Name:";
	cin >> FileName;
	gp->LoadGraph(FileName.c_str());
	gp->MakeSpanTree();
	gp->MakeLout();
	gp->display();
	gp->CheckCorrectness(Util::CHECK_PAIR);
	gp->CheckSpeed(Util::CHECK_PAIR);
	delete gp;
	return 0;
}