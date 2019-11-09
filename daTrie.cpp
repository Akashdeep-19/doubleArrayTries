#include <iostream>
#include <bits/stdc++.h>
#include <string> 
#include<fstream>
#include<time.h>

using namespace std;
class DoubleArrayTrie
{
    public:
    int *base, *check, size;    
    int pos;
    string tail;


    DoubleArrayTrie(int size_)
    {
        size = size_;
        base = new int[size];
        check = new int[size];
        for (int i = 0; i < size; i++)
        {
            base[i] = 0;
            check[i] = 0;
        }
        base[1] = 1;
        pos = 1;
        tail = "";
    }

    void a_insert(int r, string s)
    {
        //r-current node
        //s-remaing part of input string
        vector<int> a = string_to_vec(s);
        int i = 0;
        int t = base[r] + a[i];
        if (check[t] != 0)
        {
            int k = check[t];
            vector<int> r_list = set_list(r);
            vector<int> k_list = set_list(k);

            if (r_list.size() + 1 < k_list.size())
            {
                r=modify(r, r, a, r_list);
            }
            else
            {
                vector<int> null;
                r=modify(r, k, null, k_list);
            }
        }
        ins_str(r, a, pos);
    }

    void b_insert(int r, string a1,string ak1, string b1)
    {
        vector<int> a = string_to_vec(a1);
        vector<int> b = string_to_vec(b1);
        vector<int> ak = string_to_vec(ak1);
        int old_pos;
        old_pos = -base[r];
       
        for (int i = 0; i < a.size(); i++)
        {
            
            vector<int> temp = {a[i]};
            base[r] = x_check(temp);
            check[base[r] + a[i]] = r;
            r = base[r] + a[i];
        }
        vector<int> v = {ak[0], b[0]};
        base[r] = x_check(v);
       // b = vector<int>(b.begin(),b.end());
        ins_str(r, b, old_pos);
       // a = vector<int>(ak.begin()+1, ak.end());
        ins_str(r, ak, pos);
    }

    bool retrieval(string x)
    {
        int r = 1;
        int h = 0;
        int t;
        string s_temp, rem_input_string = "";
        while (base[r] > 0)
        {
            if(x[h] == '#')return true;
            t = base[r] + code(x[h]);

            if (t > size || check[t] != r)
            {
                return false; //if the next state is not present
            }
            else
            {
                r = t;
            }
            h++;
        }
        if (h == strlen(x.c_str()))
        {
            return true;
        }
        else
        {
            s_temp = fetch_str(-base[r] - 1);
        }
        for (int i = h; i < strlen(x.c_str()); i++)
        {
            rem_input_string = rem_input_string + x[i];
        }
        if (str_cmp(rem_input_string, s_temp) == -1)
        {
            return true;
        }
        else
        {
            return false; //if the multinode is not same as the remaining part of the string
        }
    }

    //helper functions here

    int code(char c)
    { //returns 'a' -> 1,'b' -> 2 etc
        if(c == '#')return 27;
        return c - 96;
    }

    char val(int c)
    {
        if(c == 27)return '#';
        return c + 96;
    }

    vector<int> string_to_vec(string s)
    {
        vector<int> v;
        for (int i = 0; i < s.length(); i++)
        {
            v.push_back(code(s[i]));
        }
        return v;
    }

    string vec_to_str(vector<int> vec)
    {
        string str;
        for (int i = 0; i < vec.size(); i++)
        {
            str += val(vec[i]);
        }
        return str;
    }

    vector<int> set_list(int r)//returns a set of symbols a such that check[base[r] + i] == r
    {
        vector<int> a;
        int k = 1;
        for (int i = 1; i < 27; i++)
        {

            if (base[r] + i >= size)
                break;

            if (check[base[r] + i] == r)
            {
                a.push_back(i);
            }
        }
        return a;
    }

    int modify(int current_s, int h, vector<int> add, vector<int> org)
    {
        int old_base = base[h];
        vector<int> comb = combine(add, org);
        base[h] = x_check(comb);
        
        for (int c : org)
        {
            int t = old_base + c;
            int t_new = base[h] + c;
            base[t_new] = base[t];
            check[t_new] = h;

            if (base[t] > 0)
            {
                for (int b : set_list(t))
                {
                    check[base[t] + b] = t_new;
                }
                if (t == current_s)
                    current_s = t_new;
            }
            base[t] = 0;
            check[t] = 0;
        }
        return current_s;
    }

    int x_check(vector<int> a)
    {
        int q = 1, j = 0;
        while (1)
        {
            j= 0;
            for (int i : a)
            { 
                if (check[q + i] == 0)
                    j++;
                
            }
            if (j == a.size())
                return q;
            q++;
        }
    }

    vector<int> combine(vector<int> add, vector<int> org)
    {
        vector<int> a;
        for (int i : add)
            a.push_back(i);
        for (int i : org)
            a.push_back(i);
        return a;
    }

    void ins_str(int h, vector<int> a, int d_pos)
    {
        int t;
        vector<int> rem_string;

        t = base[h] + a[0];
        base[t] = -d_pos;
        check[t] = h;

        for (int i = 1; i < a.size(); i++)
            rem_string.push_back(a[i]);

        pos = str_tail(d_pos, rem_string);
    }

    /**r-node no
     * t-subsequent governing node no
     * a-current input symbol
     * K-set of keys(strings)
    */
    string fetch_str(int p)
    {
        string temp = "";
        while (tail[p] != '#')
        {
            temp = temp + tail[p];
            p++;
        }
        temp = temp + "#";
        return temp;
    }
    int str_cmp(string x, string y)
    {
        if (x == y)
        {
            return -1;
        }
        else
        {
            int n = x.length() < y.length() ? x.length() : y.length();
            for (int i = 0; i < n; i++)
            {
                if (x[i] != y[i])
                    return i;
            }
            return n;
        }
    }

    //undefined str_tail function
    int str_tail(int p, vector<int> rem_string)
    {
        //no clue what this does, please check
        for(int i = 0;i < rem_string.size();i++){
            if(tail.length() <= i+p-1)tail += '?';
            tail[i+p-1] = val(rem_string[i]);
        }
        if (p == pos)
            return (pos + rem_string.size());
        else
            return pos;
    }
    //x=a1a2a3...anan+1


    int insert(string x/*, int *base, int *check, string tail*/)
    {
        int r = 1;
        int h = 0;
        int t;
        string s_temp, rem_input_string = "";
        while (base[r] > 0)
        {
            if(x[h] == '#')return 0;
            t = base[r] + code(x[h]);

            if (t > size || check[t] != r)
            {
                string rem;
                for(int i=h;i<x.length();i++)
                    rem.push_back(x[i]);
                a_insert(r,rem); //if the next state is not present
                return 1;
            }
            else
            {
                r = t;
            }
            h++;
        }
        if (h == x.length())
        {
            return 0;
        }
        else
        {
            s_temp = fetch_str(-base[r] - 1);
        }
        for (int i = h; i < x.length(); i++)
        {
            rem_input_string += x[i];
        }
        if (str_cmp(rem_input_string, s_temp) == -1)
        {
            return 0;
        }
        else
        {
            int k;
            string prefix,rem_suff,s_temp_suff;
            for (k = 0; (rem_input_string[k] == s_temp[k]); k++)prefix+=s_temp[k];
            for(int i = 0;i<rem_input_string.length();i++)rem_suff+= rem_input_string[k+i];
            for(int i = 0;i<s_temp.length();i++)s_temp_suff+= s_temp[k+i];
            b_insert(r,prefix,rem_suff,s_temp_suff); //if the multinode is not same as the remaining part of the string
            return 2;
        }
    }
};
int main(){
    int size = 30000;
    DoubleArrayTrie *dat=new DoubleArrayTrie(size);
    DoubleArrayTrie *dat2=new DoubleArrayTrie(2000);
    ifstream file;
    string line;
    long tot_chars = 0;
    long stored_chars = 0;
    int n_correct = 0;
    vector<string> words,missed;
    file.open("google-10000-english.txt");

    clock_t start = clock(); 
    if(file.is_open()){
        while (getline(file, line) && words.size() < 10000) {
            int count = 0;
            line += "#";
            int m = dat->insert(line);
            
            words.push_back(line);
            if(words.size()%100 == 0)
                cout<<"Words inserted : "<<words.size()<<endl;
        }   
    }
    
    start = clock() - start;
    double time_taken = ((double)start/CLOCKS_PER_SEC);
    
    clock_t end = clock();

    for(string s : words){
        int r = dat->retrieval(s);
        if(r == 0)dat->insert(s);
    }

    for(string s : words){
        int r = dat->retrieval(s);
        if(r == 0)missed.push_back(s);
        tot_chars += s.length();
        n_correct += r;
    }
    
    end = clock() - end;
    double time_ret = ((double)end/CLOCKS_PER_SEC);
    double acc = n_correct/(double)words.size()*100;
    
    cout<<"\naccuracy : "<<acc<<endl;
    cout<<"Time taken to insert : "<<time_taken<<"s"<<endl;
    cout<<"Time taken to retrieval : "<<time_ret<<"s"<<endl;
    cout<<"max base  : "<<*max_element(dat->base,dat->base+size)<<endl;
    cout<<"min base  : "<<*min_element(dat->base,dat->base+size)<<endl;
    cout<<"max check  : "<<*max_element(dat->check,dat->check+size)<<endl;
    cout<<"min check  : "<<*min_element(dat->check+1,dat->check+size)<<endl;

    cout<<"base : ";
    for(int i = 1;i < 30;i++)cout<<dat->base[i]<<" ";
    cout<<endl;
    cout<<"check : ";
    for(int i = 1;i < 45;i++)cout<<dat->check[i]<<" ";
    cout<<endl;
    //cout<<"tail : "<<dat->tail<<endl;
    cout<<"Total chars : "<<tot_chars<<endl;
    stored_chars += 2*size + dat->tail.length();
    cout<<"Stored chars : "<<stored_chars<<endl;
    cout<<"missed :"<<endl;
    for(int i = 0;i < 30;i++)cout<<missed[i]<<" "<<endl;
}
