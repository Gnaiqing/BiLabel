#include "Util.h"
using namespace std;

adj_list::adj_list(unsigned maxn,unsigned maxm)
{
	n = maxn;
	m = maxm;
	tl = 0;
	from = to = start = NULL;
	sorted = true;
	/* from and to records edges */
	from = (unsigned *)calloc(maxm+1,sizeof(unsigned));
	to = (unsigned *)calloc(maxm+1,sizeof(unsigned));
	/* start and finish records where the edges of a
	 * specific node start and finish in edge list
	 */
	start = (unsigned *)calloc(maxn+1,sizeof(unsigned));
	ind =  (unsigned *)calloc(maxn+1,sizeof(unsigned));
	outd = (unsigned *)calloc(maxn+1,sizeof(unsigned));
	if (from == NULL || to == NULL || start == NULL || 
		 ind == NULL || outd == NULL)
	{
		cerr << "adj_list memory allocation failed!\n";
		system("pause");
		exit(0);
	}

}
void adj_list::add_edge(unsigned u,unsigned v)
{
	if(u > n || v > n)
	{
		cerr << "node exceed boundary!\n";
		system("pause");
		exit(0);
	}
	sorted = false;
	from[tl] = u;
	to[tl] = v;
	outd[u]++;
	ind[v]++;
	tl++;
	if(tl > m)
	{
		cerr << "adj_list is full!\n";
		system("pause");
		exit(0);
	}
}
void adj_list::sort_list()
{
	if(sorted) return;
	unsigned *tf = (unsigned *)calloc(m+1,sizeof(unsigned));
	unsigned *tt = (unsigned *)calloc(m+1,sizeof(unsigned));
	unsigned *wl = (unsigned *)calloc(n+1,sizeof(unsigned));
	if(tf == NULL || tt == NULL || wl == NULL)
	{
		cerr << "memory allocation failed in sort" << endl;
		system("pause");
		exit(0);
	}
	wl[0] = outd[0]-1;
	for(unsigned i = 1; i <= n;i++)
		wl[i] = wl[i-1] + outd[i];
	for(unsigned i = 0; i < tl;i++)
	{
		tf[wl[from[i]]] = from[i];
		tt[wl[from[i]]] = to[i];
		wl[from[i]]--;
	}
	memcpy(from,tf,sizeof(unsigned)*tl);
	memcpy(to,tt,sizeof(unsigned)*tl);
	start[0] = 0;
	for(unsigned i = 1;i <= n;i++)
		start[i] = start[i-1] + outd[i-1];
	free(tf);
	free(tt);
	free(wl);
	sorted = true;
}
unsigned *adj_list::get_child(unsigned u,unsigned &size)
{
	size = outd[u];
	return to + start[u];
}
adj_list::~adj_list()
{
	free(from);
	free(to);
	free(start);
	free(ind);
	free(outd);
}