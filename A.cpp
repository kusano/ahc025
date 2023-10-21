#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdio>
#include <functional>
using namespace std;

int xor64() {
    static uint64_t x = 88172645463345263ULL;
    x ^= x<<13;
    x ^= x>> 7;
    x ^= x<<17;
    return int(x&0x7fffffff);
}

class Judge
{
public:
    virtual string query(const vector<int> &L, const vector<int> &R) = 0;
    virtual void answer(const vector<int> ans) = 0;
};

class Server: public Judge
{
public:
    string query(const vector<int> &L, const vector<int> &R) override
    {
        cout<<L.size()<<" "<<R.size();
        for (int l: L)
            cout<<" "<<l;
        for (int r: R)
            cout<<" "<<r;
        cout<<endl;
        string res;
        cin>>res;
        return res;
    }

    void answer(const vector<int> ans) override
    {
        for (int i=0; i<(int)ans.size(); i++)
            cout<<(i==0?"":" ")<<ans[i];
        cout<<endl;
    }
};

class Local: public Judge
{
    int N, D, Q;
    vector<long long> W;

public:
    Local(int N, int D, int Q, vector<long long> W)
        : N(N), D(D), Q(Q), W(W)
    {
    }

    string query(const vector<int> &L, const vector<int> &R) override
    {
        cout<<L.size()<<" "<<R.size();
        for (int l: L)
            cout<<" "<<l;
        for (int r: R)
            cout<<" "<<r;
        cout<<endl;

        long long wl = 0;
        for (int l: L)
            wl += W[l];
        long long wr = 0;
        for (int r: R)
            wr += W[r];
        if (wl<wr)
            return "<";
        else if (wl>wr)
            return ">";
        else
            return "=";
    }

    void answer(const vector<int> ans) override
    {
        for (int i=0; i<(int)ans.size(); i++)
            cout<<(i==0?"":" ")<<ans[i];
        cout<<endl;

        vector<long long> T(D);
        for (int i=0; i<N; i++)
            T[ans[i]] += W[i];

        double avg = 0;
        for (long long t: T)
            avg += t;
        avg /= D;

        double V = 0;
        for (long long t: T)
            V += (t-avg)*(t-avg);
        V /= D;

        long long score = 1LL + (long long)(100*sqrt(V)+.5);
        fprintf(stderr, "%3d %2d %4d %8lld\n", N, D, Q, score);
    }
};

int main(int argc, char **argv)
{
    int N, D, Q;
    cin>>N>>D>>Q;

    Judge *judge;
    if (argc==2 && argv[1]==string("local"))
    {
        vector<long long> W(N);
        for (long long &w: W)
            cin>>w;
        judge = new Local(N, D, Q, W);
    }
    else
        judge = new Server();


    vector<vector<int>> T(D);
    for (int i=0; i<N; i++)
        T[i%D].push_back(i);

    int q = 0;
    function<void(int, int)> sort = [&](int l, int r)
    {
        if (r-l<=1)
            return;

        vector<int> p = T[l];
        vector<vector<int>> L, R;
        for (int i=l+1; i<r; i++)
        {
            bool less = false;
            if (q<Q)
            {
                q++;
                if (judge->query(T[i], p)=="<")
                    less = true;
            }
            if (less)
                L.push_back(T[i]);
            else
                R.push_back(T[i]);
        }
        for (int i=0; i<(int)L.size(); i++)
            T[l+i] = L[i];
        T[l+(int)L.size()] = p;
        for (int i=0; i<(int)R.size(); i++)
            T[l+(int)L.size()+1+i] = R[i];
        sort(l, l+(int)L.size());
        sort(l+(int)L.size()+1, r);
    };
    sort(0, D);

    while (q<Q)
    {
        int m = D-1;
        while (T[m].size()==1)
            m--;
        if (m==0)
            break;

        int r = xor64()%(int)T[m].size();
        T[0].push_back(T[m][r]);
        T[m].erase(T[m].begin()+r);

        int p = m;
        while (0<=p-1 && q<Q)
        {
            q++;
            if (judge->query(T[p], T[p-1])=="<")
            {
                swap(T[p], T[p-1]);
                p--;
            }
            else
                break;
        }
        if (p!=1)
        {
            if (p==0)
                p = 1;
            else
                p = 0;

            while (p+1<D && q<Q)
            {
                q++;
                if (judge->query(T[p], T[p+1])==">")
                {
                    swap(T[p], T[p+1]);
                    p++;
                }
                else
                    break;
            }
        }
    }

    while (q<Q)
    {
        q++;
        judge->query({0}, {1});
    }

    vector<int> ans(N);
    for (int i=0; i<D; i++)
        for (int t: T[i])
            ans[t] = i;

    judge->answer(ans);
}
