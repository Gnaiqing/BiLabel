#ifndef _BILABEL_H_
#define _BILABEL_H_
#include "Util.h"

class Graph
{
public:
	Graph(unsigned maxn,unsigned maxm);
	~Graph();
	int LoadGraph(const char *FileName);
	void dfs(unsigned u);/* dfs for MakeSpanTree */
	void MakeSpanTree();
	void MakeLout();
	bool CanReach(unsigned u,unsigned v);
	bool CanReach_dfs(unsigned u,unsigned v);
	void dfs2(unsigned u,unsigned v);/* dfs for reachability */
	bool update(unsigned u,unsigned v);
	void display(unsigned max_item = 100);
	void CheckCorrectness(unsigned pairnum = 100);
	void CheckSpeed(unsigned pairnum = 1000);
private:
	adj_list * pAll;
	adj_list * pSpan;
	adj_list * pOther;
	unsigned * fa;
	bool *vis;
	unsigned *pre_order;
	lpair *lin, *lout;
	unsigned n,m,mTree,ptl,maxn_,maxm_;
	map<unsigned,unsigned> *mp;
	unsigned dfn;
};
#endif