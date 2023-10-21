#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cstdio>
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

    vector<int> ans;
    for (int i=0; i<N; i++)
        ans.push_back(i%D);

    int q = 0;
    while (q<Q)
    {
        int l = xor64()%D;
        int r = xor64()%D;
        if (l==r)
            continue;

        vector<int> L, R;
        for (int i=0; i<N; i++)
        {
            if (ans[i]==l)
                L.push_back(i);
            if (ans[i]==r)
                R.push_back(i);
        }
        if (L.empty() && R.empty())
            continue;

        if (L.empty())
            ans[R[xor64()%R.size()]] = l;
        else if (R.empty())
            ans[L[xor64()%L.size()]] = r;
        else
        {
            string res = judge->query(L, R);
            q++;
            if (res=="<")
                ans[R[xor64()%R.size()]] = l;
            if (res==">")
                ans[L[xor64()%L.size()]] = r;
        }
    }

    judge->answer(ans);
}
