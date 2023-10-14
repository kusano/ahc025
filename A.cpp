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

    for (int q=0; q<Q; q++)
    {
        vector<int> l;
        vector<int> r;
        while (true)
        {
            l.clear();
            r.clear();
            for (int i=0; i<N; i++)
                if (xor64()%2==0)
                    (xor64()%2==0?l:r).push_back(i);
            if (!l.empty() && !r.empty())
                break;
        }

        cout<<l.size()<<" "<<r.size();
        for (int t: l)
            cout<<" "<<t;
        for (int t: r)
            cout<<" "<<t;
        cout<<endl;

        string res;
        cin>>res;
    }

    vector<int> ans;
    for (int i=0; i<N; i++)
        ans.push_back(i%D);

    for (int i=0; i<N; i++)
        cout<<(i==0?"":" ")<<ans[i];
    cout<<endl;
}
