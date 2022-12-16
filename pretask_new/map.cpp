#include <bits/stdc++.h>
// #include <Dark/inout>
using namespace std;
using ll = long long;
using ull = unsigned long long;


signed main() {
    unordered_map <string,set <int> > mp;
    int n,x;
    string opt;
    freopen("cache","r",stdin);
    cin >> n;
    mp.reserve(n);
    while(n--) {
        cin >> x >> opt;
        set <int> &s = mp[opt];
        while(x--) {
            int t;
            cin >> t; 
            s.insert(t);
        }
    }
    cin.clear();
    freopen("1.txt","r",stdin);
    freopen("ans.txt","w",stdout);
    cin >> n;
    while(n--) {
        cin >> opt;
        if(opt == "find") {
            cin >> opt;
            set <int> &s = mp[opt];
            if(s.empty()) {
                cout << "null\n";
            } else {
                for(auto it : s) {
                    cout << it << ' ';
                }
                cout << '\n';
            }
        } else if(opt == "insert") {
            cin >> opt >> x;
            mp[opt].insert(x);
        } else {
            cin >> opt >> x;
            mp[opt].erase(x);            
        }

    }
    freopen("cache","w",stdout);
    cout << mp.size() << ' ';
    for(auto it : mp) {
        cout << it.second.size() << ' ' << it.first << ' ';
        for(auto i : it.second) {
            cout << i << ' ';
        }
    }


    return 0;
}