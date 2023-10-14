#include <iostream>
#include <vector>
#include <string>
using namespace std;

int xor64() {
    static uint64_t x = 88172645463345263ULL;
    x ^= x<<13;
    x ^= x>> 7;
    x ^= x<<17;
    return int(x&0x7fffffff);
}

int main()
{
    int N, D, Q;
    cin>>N>>D>>Q;

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
            cout<<L.size()<<" "<<R.size();
            for (int t: L)
                cout<<" "<<t;
            for (int t: R)
                cout<<" "<<t;
            cout<<endl;
            q++;

            string res;
            cin>>res;

            if (res=="<")
                ans[R[xor64()%R.size()]] = l;
            if (res==">")
                ans[L[xor64()%L.size()]] = r;
        }
    }

    for (int i=0; i<N; i++)
        cout<<(i==0?"":" ")<<ans[i];
    cout<<endl;
}
