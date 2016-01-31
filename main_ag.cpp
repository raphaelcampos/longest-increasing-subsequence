#include <stdio.h>
#include <iomanip>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <algorithm>
#include <sys/time.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

using namespace std;

vector<int> p;
vector<int> qi;

void load_input_file(vector<int> &p, vector<int> &qi, string filename){
    ifstream file;
    file.open (filename.c_str());
    
    if(!file) return;
    
    int tmp;
    int i = 0;
	file >> tmp;    
    do{
      p.push_back(tmp);
      
      file >> tmp;
      qi.push_back(tmp);
      file >> tmp;
      i++;
    }while(!file.eof());

	file.close();
    
}

void write_output_file(const vector<int> &lis, string filename){
    ofstream file;
    file.open (filename.c_str());
    
    if(!file) return;
    
    file << lis.size() << endl;

    for(int i = 0; i < lis.size(); i++)
	file << lis[i] << endl;
    
	file.close();
}

/**
 * Print a vector
 * @param v   vector
 * @param sep elements separator
 */
void printVector(const vector<int> &v, char sep){
    for (int i=0;i<v.size();i++)
        std::cout << v[i] << sep;
}

/**
 * Print an array
 * @param a    array
 * @param size size
 */
void printArray(int a[],int size){
    int i;

    for (i=0;i<size;i++)
        std::cout << a[i] << " ";

    std::cout << std::endl;

    return;
}

/**
 * Comparison function for qsort, uses the array P
 * as reference in order to sort array C
 * @param  a 
 * @param  b 
 * @return int
 */
int compareP (const void * a, const void * b) {
    double diff = (p[*(int*)a]) - (p[*(int*)b]);
    if(diff == 0){
        diff = qi[*(int*)a] - qi[*(int*)b];    
    }

    return  (0 < diff) - (diff < 0) ;
}

/**
 * Comparison function for qsort, uses the array QI
 * as reference in order to sort array C
 * @param  a 
 * @param  b 
 * @return int
 */
int compareQI (const void * a, const void * b) {
    
    double diff = (qi[*(int*)a]) - (qi[*(int*)b]);
    
    if(diff == 0){
        diff = p[*(int*)a] - p[*(int*)b];
    }

    return (0 > diff) - (diff > 0);
}

/**
 * Compares the pairs in the array p and qi following the
 * constraint
 * @param  p  Brain weights array
 * @param  qi QIs array
 * @param  i  first element
 * @param  j  second element
 * @return    bool
 */
inline bool compare(vector<int> &p, vector<int> &qi, int i, int j){
    return p[i] < p[j] && qi[i] > qi[j];
}

/**
 * Print stacks side by side
 * @param stacks list of "stacks"
 * @param p      brain weights
 * @param qi     brain QI
 * @param n      total number of brains
 */
void printStacks(vector< vector<int> > stacks, vector<int> &p, vector<int> &qi){
    for(int i = 0; i <= p.size()-stacks.size(); i++){
        for(int j = 0; j< stacks.size(); j++){
            if(stacks[j].empty()) {cout << "     " << setw(4) << " " << "   " <<
setw(5) << " " << " - " << setw(5) << " " << " " ;continue;}

            cout << "id : " << setw(4) << *stacks[j].begin() << " | " << setw(5)
<< p[*stacks[j].begin()] << " - " << setw(5) << qi[*stacks[j].begin()] << " " ;
            stacks[j].erase(stacks[j].begin());
        }
        cout << endl;
    }
}

/**
 * Insert a brain in its correspondent pile
 * @param stacks [description]
 * @param p      [description]
 * @param qi     [description]
 * @param last   [description]
 */
void insertBrain(vector< vector<int> > &stacks, vector<int> &p, vector<int> &qi, int prev[],
int last){
	int i;
    int first;
	for(i = 0; i < stacks.size(); i++){
		first = stacks[i][stacks[i].size()-1];
		
        if(compare(p, qi, first, last)) continue;

        int top_card = ((i-1>=0)? stacks[i-1][stacks[i-1].size()-1] : -1);
        prev[last] = (top_card!=-1 && compare(p, qi, top_card, last))? top_card
: -1;
		stacks[i].push_back(last);
		
		return;
		
	}

    vector<int> v;
	v.push_back(last);
	stacks.push_back(v);

    int top_card = ((i-1>=0)? stacks[i-1][stacks[i-1].size()-1] : -1);
    prev[last] = (top_card!=-1 && compare(p, qi, top_card, last))? top_card :
-1;
}

/**
 * Greedy algorithm to calculate LIS (longest Increasing Subsequence)
 * based on pacience sort method
 * @param  c       
 * @param  p       
 * @param  qi      
 * @param  n       
 * @param  max_ref 
 * @param  cs      
 * @param  size
 * @return
 */
vector< vector<int> > g_lis(int c[], vector<int> &p, vector<int> &qi, int prev[])
{
    vector< vector<int> > stacks;

    for(int i = 0; i < p.size(); i++){
	   insertBrain(stacks, p, qi, prev, c[i]);
    }

    return stacks;
}

/**
 * Get a lis from the stacks
 * @param stacks
 * @param lis
 */
void get_lis(const vector< vector<int> > &stacks, int prev[], vector<int> &lis){
    
    int top_card = 0;
    int j = 0;
    do{
        top_card = stacks[stacks.size()-1][j];
        j++;
    }while(prev[top_card] == -1 && j < stacks[stacks.size()-1].size());

    while(top_card != -1 ){   
        lis.push_back(top_card+1);
        top_card = prev[top_card];
    }

    reverse(lis.begin(), lis.end());
}

/**
 * The wrapper function for bf_lis()
 * @param  c  Brain indices array
 * @param  p  Brain weights
 * @param  qi QI
 * @param  n  size
 * @return    max length of the longest subsequence
 */
int greedy_lis(vector<int> &p, vector<int> &qi, vector<int> &lis)
{   
    int *c = new int[p.size()];
    
    int *prev = new int[p.size()];
    
    for (int j = 0; j < p.size(); ++j)
    {
      c[j] = j;
      prev[j] = -1;
    }
  
    // order by QI
    qsort(c, p.size(), sizeof(int), compareQI);
  
    vector< vector<int> > stacks = g_lis(c, p, qi, prev);

    get_lis(stacks, prev, lis);

    //printStacks(stacks, p, qi);

    delete [] prev;
    delete [] c;

    return stacks.size();
}

bool validaLIS(const vector<int> &lis, vector<int> &p, vector<int> &qi){
    for (int i = 1; i < lis.size(); ++i)
    {   
        if(!compare(p, qi, lis[i-1]-1, lis[i]-1)) return false;
    }

    return true;
}

typedef unsigned long long timestamp_t;

static timestamp_t get_timestamp ()
    {
      struct timeval now;
      gettimeofday (&now, NULL);
      return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
    }

int main(int argc, char* argv[])
{
    string filename = argv[1];
  
    // input
    load_input_file( p, qi, filename);
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
    if(p.size() == 0) return 0;

    // find LIS
    vector<int> lis;
    timestamp_t t0 = get_timestamp();

    greedy_lis(p, qi, lis);    

    // Process
    timestamp_t t1 = get_timestamp();

    double secs = (t1 - t0) / 1000000.0L;

    cout << "Exec time: " << secs << endl;

    if(validaLIS(lis, p, qi))
        cout << "LIS valido!!!" << endl;
    else
        cout << "LIS invalido!!!" << endl;

    // output
    write_output_file(lis, "output.txt");    

    return 0;
}
