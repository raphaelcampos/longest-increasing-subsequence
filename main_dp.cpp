#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <algorithm>
#include <iomanip>
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
 * Comparison function for qsort, uses the array P
 * as reference in order to sort array C
 * @param  a 
 * @param  b 
 * @return int
 */
int compareP (const void * a, const void * b) {
    double diff = (p[*(int*)a]) - (p[*(int*)b]);
    return  (0 < diff) - (diff < 0);
}

/**
 * Comparison function for qsort, uses the array QI
 * as reference in order to sort array C
 * @param  a 
 * @param  b 
 * @return int
 */
int compareQI (const void * a, const void * b) {
    double diff = qi[*(int*)a] - qi[*(int*)b];
    return  (0 > diff) - (diff > 0);
}

void printVector(const vector<int> &v){
    for (int i=0;i<v.size();i++)
        std::cout << v[i] << " ";

    std::cout << std::endl;
}

void printArray(int a[],int size){
    int i;

    for (i=0;i<size;i++)
        std::cout << setw(4) << a[i] << " ";

    std::cout << std::endl;

    return;
}

/**
 * Compares the pairs in the array p and qi following the
 * contraints
 * @param  p  Brain weights array
 * @param  qi QIs array
 * @param  i  first element
 * @param  j  second element
 * @return    bool
 */
bool compare(vector<int> & p, vector<int> & qi, int i, int j){
    return p[i] < p[j] && qi[i] > qi[j];
}

/**
 * Dynamic programming algorithm to calculate LIS (longest Increasing Subsequence)
 * @param  c       [description]
 * @param  p       [description]
 * @param  qi      [description]
 * @param  n       [description]
 * @param  max_ref [description]
 * @param  cs      [description]
 * @param  size    [description]
 * @return         [description]
 */
int dp_lis(int c[], vector<int> &p, vector<int> &qi, int m[], int n, int &max_ref, int &mi, int *pr)
{
 
    if(n==0)
        return 0;
    else if(m[n-1] != 0)
        return m[n-1];

    int lis_length, max = 1;

    for(int i = 1; i < n; i++)
    {
        lis_length = dp_lis(c, p, qi, m, i, max_ref, mi, pr);
        if (lis_length + 1 > max && compare(p, qi, c[i-1], c[n-1]) ){
            max = lis_length + 1;
            pr[n-1] = i-1;
        }
    }

    if(max > max_ref){
        max_ref = max;
        mi = n-1;
    }
 
    return m[n-1] = max;
}

/**
 * Get lis using prev array
 * @param c    [description]
 * @param prev [description]
 * @param n    [description]
 * @param lis  [description]
 */
void get_lis(int c[], int prev[], int n, vector<int> &lis){
    
    for (int i = n; i != -1; i = prev[i]) {
        lis.push_back(c[i]+1);
    }

    //reverse(lis.begin(), lis.end());
}

/**
 * find LIS using the memoization array
 * @param c   brain index array
 * @param m   memoization array
 * @param n   start point to look at m
 * @param lis 
 */
void get_lis_by_m_quad(int c[], int m[], int n, vector<int> &lis){
    lis.push_back(c[n]+1);
    int j = n;
    int max = m[n];
    while(j > 0 && max > 1)    
    {
        for (int i = j-1; i >=0 ; i--) {
            if (m[i] + 1 == max && compare(p, qi, c[i], c[j]) ){
                lis.push_back(c[i]+1);
                j = i;
                max = m[i];
            }
        }
    }
    
    reverse(lis.begin(), lis.end());
}

void get_lis_by_m(int c[], int m[], int n, vector<int> &lis){
    lis.push_back(c[n]+1);
    int j = n;
    int max = m[n];
    int i = j-1;
    while(i >= 0 && j > 0 && max > 1)    
    {   
        if (m[i] + 1 == max && compare(p, qi, c[i], c[j]) ){
            lis.push_back(c[i]+1);
            j = i;
            max = m[i];
        }

        i--;
    }
    
    reverse(lis.begin(), lis.end());
}

/**
 * The wrapper function for dp_lis()
 * @param  p    weights vector
 * @param  qi   QI vector
 * @param  m    memoization array
 * @param  lis  vector representing LIS
 * @return int  LIS max length
 */
int dynamic_programming_lis(vector<int> &p, vector<int> &qi, int m[], vector<int> &lis)
{   

    int max_lis_length = 1;

    int *c = new int[p.size()];
    
    int *prev = new int[p.size()];
    
    for (int j = 0; j < p.size(); ++j)
    {
        c[j] = j;
        prev[j] = -1;
        m[j] = 0;
    }

    // sort the array in order of QI
    qsort(c, p.size(), sizeof(int), compareQI);
 
    int mi = 0;   
    dp_lis(c, p, qi, m, p.size(), max_lis_length, mi, prev);

    //get_lis(c, prev, mi, lis);
    //cout << "Finding solution..." << endl;
    get_lis_by_m(c, m, mi, lis);

    printArray(m, p.size());

    return max_lis_length;
}

bool validaLIS(const vector<int> &lis, vector<int> &p, vector<int> &qi){
    for (int i = 1; i < lis.size(); ++i)
    {   
    //    cout << compare(p, qi, lis[i-1]-1, lis[i]-1) << ":" << p[lis[i-1]-1] << " " <<  qi[lis[i-1]-1] << " "<< p[lis[i]-1] << " " <<  qi[lis[i]-1] << endl;
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
    load_input_file(p, qi, filename);

    int *m = new int[p.size()];  

    if(p.size() == 0) return 0;

    // find LIS
    vector<int> lis;

    timestamp_t t0 = get_timestamp();
    dynamic_programming_lis(p, qi, m, lis);
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
