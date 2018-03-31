#ifndef _UTIL_H_
#define _UTIL_H_
// below are headers of C files;
#include <time.h>
#include <malloc.h>
// below are headers of C++ files
#include <bitset>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <deque>
#include <vector>
#include <list>
#include <iterator>
#include <algorithm>
#include <functional>
#include <utility>
#include <new>
using namespace std;
struct lpair
{
	unsigned s,t;
};

class adj_list
{
public:
	unsigned n,m,tl;
	unsigned *ind, *outd;// in-degree and out-degree
	adj_list(unsigned maxn,unsigned maxm);
	void add_edge(unsigned u,unsigned v);
	void sort_list();
	unsigned *get_child(unsigned u,unsigned &size);
	~adj_list();
private:
	unsigned *from, *to, *start; 
	bool sorted;
};


class Util
{
public:
	static const unsigned MAX_NODE = 100000U;
	static const unsigned MAX_EDGE = 300000U;	
	static const unsigned CHECK_PAIR = 100000U;
};
#endif