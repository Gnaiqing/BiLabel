#include "BiLabel.h"
using namespace std;

Graph::Graph(unsigned maxn,unsigned maxm)
{
	maxn_ = maxn;
	maxm_ = maxm;
	pAll = new adj_list(maxn,maxm+maxn);
	pSpan = new adj_list(maxn,maxn);
	pOther = new adj_list(maxn,maxm);
	mp = new map<unsigned,unsigned>();
	fa = (unsigned *)calloc(maxn,sizeof(unsigned));
	vis = (bool *)calloc(maxn,sizeof(bool));
	pre_order = (unsigned *)calloc(maxn,sizeof(unsigned));
	lin = (lpair *)calloc(maxn,sizeof(lpair));
	lout = (lpair *)calloc(maxn,sizeof(lpair));
	dfn = ptl = 0;
}
Graph::~Graph()
{
	delete pAll;
	delete pSpan;
	delete pOther;
	delete mp;
	free(fa);
	free(vis);
	free(pre_order);
	free(lin);
	free(lout);
}
int Graph::LoadGraph(const char *FileName)
{
	ifstream fin;
	fin.open(FileName);
	if(fin.fail())
	{
		cerr << "Open File Failed"<<endl;
		system("pause");
		exit(0);
	}
	char op,tmp;
	unsigned u,v,l;
	bool firstE = false;
	while(1)
	{
		fin >> op;
		if(op == 't')
		{
			fin >> tmp >> l;
			if(l == -1)
			{
				/* add a virture root '0', and connect it to every other node */
				for(unsigned i = 1;i <= n;i++)
					pAll->add_edge(0,i);
				pAll->sort_list();
				fin.close();
				return 0;
			}
		}
		else if(op == 'v')
		{
			fin >> u >> l;
			(*mp)[u] = 0;
		}
		else if(op == 'e')
		{
			if(!firstE)
			{
				/* map all nodes into label 1 to node_num */
				firstE = true;
				unsigned cnt = 1;
				for(map<unsigned,unsigned>::iterator iter = (*mp).begin();
					iter != (*mp).end();iter++)
				{
					iter->second = cnt++;
				}
				n = cnt-1;
				if(n+1 > maxn_)
				{
					cerr << "graph node exceed boundary!\n";
					system("pause");
					exit(0);
				}
				m = 0;
			}
			fin >> u >> v >> l;
			u = (*mp)[u];
			v = (*mp)[v];
			m++;
			pAll->add_edge(u,v);
		}
	}

	return 0;
}
void Graph::dfs(unsigned u)
{
	lin[u].s = n;
	pre_order[ptl++] = u;
	unsigned len = 0;
	unsigned *a = NULL;
	vis[u] = true;
	a = pAll->get_child(u,len);
	for(unsigned i = 0;i < len;i++)
	{
		if(!vis[a[i]])
		{
			pSpan->add_edge(u,a[i]);
			fa[a[i]] = u;
			dfs(a[i]);
			lin[u].s = min(lin[u].s,lin[a[i]].s);
		}
		else
		{
			/* Can we add less edges to pOther? */
			pOther->add_edge(u,a[i]);
		}
	}
	dfn++;
	lin[u].s = min(lin[u].s,dfn);
	lin[u].t = dfn;
	lout[u] = lin[u];
}
void Graph::MakeSpanTree()
{
	memset(vis,0,sizeof(bool)*(n+1));
	dfs(0);
	pOther->sort_list();
}
void Graph::MakeLout()
{
	for(unsigned i = 0;i < ptl;i++)
	{
		unsigned u = pre_order[i],len = 0;
		unsigned *a = NULL;
		a = pOther->get_child(u,len);
		for(unsigned j = 0;j < len;j++)
		{
			unsigned v = a[j];
			unsigned tmp_u = u;
			/* update Lout of u and u's tree ancestors using v */
			while(tmp_u!= 0&& update(tmp_u,v)) tmp_u = fa[tmp_u];
		}
	}
}
bool Graph::CanReach(unsigned u,unsigned v)
{
	/* to acceletate query, we suppose the mapping of nodes has
	 * already been done, so 1 <= u,v <= n. 
	 */
	if(u > n || v > n)
	{
		cerr << "Reachability query exceed bound!\n";
		system("pause");
		exit(0);
	}
	/* condition 1, reachable through tree edges */
	if(lin[v].t >= lin[u].s && lin[v].t <= lin[u].t)
		return true;
	/* condition 2 & 3, unreachable through all edges */ 
	if(lin[v].t < lout[u].s || lin[v].t > lout[u].t)
		return false;
	/* condition 4,call dfs */
	return CanReach_dfs(u,v);
}
/* simple reachability check using dfs */
bool Graph::CanReach_dfs(unsigned u,unsigned v)
{
	memset(vis,0,sizeof(bool)*(n+1));
	dfs2(u,v);
	return vis[v];
}
/* dfs for reachability check */
void Graph::dfs2(unsigned u,unsigned v)
{
	vis[u] = true;
	unsigned len = 0;
	unsigned *a = NULL;	
	a = pAll->get_child(u,len);
	for(unsigned i = 0;i < len;i++)
	{
		if(vis[v]) break;
		if(!vis[a[i]])
		{
			dfs2(a[i],v);
		}
	}
}

/* check if lout(v) can update lout(u) */
bool Graph::update(unsigned u,unsigned v)
{
	bool up = false;
	if(lout[v].s < lout[u].s)
	{
		up = true;
		lout[u].s = lout[v].s;
	}
	if(lout[v].t > lout[u].t)
	{
		up = true;
		lout[u].t = lout[v].t;
	}
	return up;
}
void Graph::display(unsigned max_item)
{
	max_item = min(max_item,n+1);
	cout <<"Node num = "<<n<<" Edge num = "<<m<<endl;
	cout<<"Map for nodes:\n";
	int count = 0;
	for(map<unsigned,unsigned>::iterator iter = (*mp).begin();
					iter != (*mp).end();iter++)
	{
		cout <<iter->first<<"-->"<<iter->second<<endl;
		count++;
		if(count == max_item)
			break;
	}
	cout<<"Span Tree:\n";
	pSpan->sort_list();
	for(unsigned i = 0;i < max_item;i++)
	{
		unsigned len = 0;
		unsigned *a = pSpan->get_child(i,len);
		cout<<i<<": ";
		for(unsigned j = 0;j < len;j++)
		{
			cout<<a[j]<<' ';
			if(j > 10) 
			{
				cout <<"...";
				break;
			}
		}
		cout<<endl;
	}
	cout<<"Node\tLin.s\tLin.t\tLout.s\tLout.t\t\n";
	for(unsigned i = 0;i < max_item;i++)
	{
		cout<<i<<'\t'<<lin[i].s<<'\t'<<lin[i].t<<'\t'
			<< lout[i].s<<'\t'<<lout[i].t<<"\t\n";
	}
	system("pause");
}
void Graph::CheckCorrectness(unsigned pairnum)
{
	srand(time(NULL));
	for(unsigned i = 0;i < pairnum;i++)
	{
		unsigned u = (rand() % n)+1;
		unsigned v = (rand() % n)+1;
		bool b1 = CanReach(u,v);
		bool b2 = CanReach_dfs(u,v);
		if(b1 != b2)
		{
			cout <<"Diff:\t"<<u<<"->"<<v
				<<"\tBiL:"<<b1<<"\tdfs:"<<b2<<endl;
		}
	}
	cout << "Check correctness done. "
		<<pairnum<<" pairs checked.\n";
	system("pause");
}
void Graph::CheckSpeed(unsigned pairnum)
{
	srand(time(NULL));
	clock_t start,finish;
	start = clock();
	for(unsigned i = 0;i < pairnum;i++)
	{
		unsigned u = (rand() % n)+1;
		unsigned v = (rand() % n)+1;
		bool b1 = CanReach(u,v);
	}
	finish = clock();
	double time_tot = (double)(finish - start)/(CLOCKS_PER_SEC);
	double time_single = 1000*time_tot/pairnum;/* ms */
	cout << "Check speed done for "<<pairnum<<" pairs\n";
	cout << setiosflags(ios::fixed)<<setprecision(2)<<time_tot
		<<" s in total"<<endl;
	cout << setiosflags(ios::fixed)<<setprecision(4)<<time_single
		<<" ms per pair"<<endl;
	system("pause");
}