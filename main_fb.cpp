#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <algorithm>
#include <fstream>

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

int compareP (const void * a, const void * b) {
    double diff = (p[*(int*)a]) - (p[*(int*)b]);
    return  (0 < diff) - (diff < 0);
}

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
        std::cout << a[i] << " ";

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
bool compare(vector<int> &p, vector<int> &qi, int i, int j){
    return p[i] < p[j] && qi[i] > qi[j];
}

/**
 * Brute force algorithm to calculate LIS (longest Increasing Subsequence)
 * @param  c       [description]
 * @param  p       [description]
 * @param  qi      [description]
 * @param  n       [description]
 * @param  max_ref [description]
 * @param  cs      [description]
 * @param  size    [description]
 * @return         [description]
 */
int bf_lis(int c[], vector<int> &p, vector<int> &qi, int n, int &mi, int &max_ref, int *pr)
{

    if(n == 0){
        return 0;
    }
 
    int lis_length, max = 1;

    for(int i = 1; i < n; i++)
    {   
        lis_length = bf_lis(c, p, qi, i, mi, max_ref, pr);
        if (lis_length + 1 > max && compare(p, qi, c[i-1], c[n-1])){
            max = lis_length + 1;
            pr[n-1] = i-1;
        }   
    }
 
    if (max > max_ref){
        max_ref = max;
        mi = n-1;
    }
 
    return max;
}

/**
 * 
 * @param bestEnd [description]
 * @param prev    [description]
 * @param lis     [description]
 */
void get_lis(int c[], int prev[], int n, vector<int> &lis){
    for (int i = n; i != -1; i = prev[i]) {
        lis.push_back(c[i]+1);
    }
    reverse(lis.begin(), lis.end());
}

/**
 * The wrapper function for bf_lis()
 * @param  c  [description]
 * @param  p  [description]
 * @param  qi [description]
 * @param  n  [description]
 * @return    int
 */
int brute_force_lis(vector<int> &p, vector<int> &qi, vector<int> &lis)
{  
    int max_lis_length = 1;

    int *c = new int[p.size()];
    
    int * prev = new int[p.size()+1];
    
    for (int j = 0; j < p.size(); ++j)
    {
      c[j] = j;
      prev[j] = -1;
    }

    // sort the array in order of QI
    qsort(c, p.size(), sizeof(int), compareQI);
 
    int mi = p.size() - 1;
    bf_lis(c, p, qi, p.size(), mi, max_lis_length, prev);

    get_lis(c, prev, mi, lis);

    return max_lis_length;
}

bool validaLIS(const vector<int> &lis, vector<int> &p, vector<int> &qi){
    for (int i = 1; i < lis.size(); ++i)
    {   
        /*cout << compare(p, qi, lis[i-1]-1, lis[i]-1) << ":" << p[lis[i-1]-1] << " " <<  qi[lis[i-1]-1] << " "<< p[lis[i]-1] << " " <<  qi[lis[i]-1] << endl;*/
        if(!compare(p, qi, lis[i-1]-1, lis[i]-1)) return false;
    }

    return true;
}


int main(int argc, char* argv[])
{
    string filename = argv[1];
  
    // input
    load_input_file(p, qi, filename);
  
    if(p.size() == 0) return 0;

    // find LIS
    vector<int> lis;
    brute_force_lis(p, qi, lis);
    if(validaLIS(lis, p, qi))
        cout << "LIS valido!!!" << endl;
    else
        cout << "LIS invalido!!!" << endl;
    
    // output
    write_output_file(lis, "output.txt");    

    return 0;

}
