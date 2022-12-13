#include <bits/stdc++.h>
// #include <Dark/inout>
using namespace std;
using ll = long long;
using ull = unsigned long long;
// #define int long long


signed main() {
    
    multiset <pair<string,string>> mp;
    int n;
    string x,y;
    freopen("test1.txt","r",stdin);
    freopen("ans1.txt","w",stdout);
    std::ios::sync_with_stdio(false);
    cin >> n;
    while(n--) {
        string str;
        cin >> str;
        if(str == "delete") {
            cin >> x >> y;
            mp.erase(make_pair(x,y));
        } else if(str == "insert") {
            cin >> x >> y;
            mp.emplace(x,y);
        } else {
            cin >> x;
            vector <string> ans;
            string last = "";
            for(auto i : mp) {
                if(i.first < x) continue;
                if(i.first > x) break; 
                if(last != i.second)
                    ans.push_back(last = i.second);
            }
            if(ans.empty()) {cout << "null\n";}
            else {
                for(auto &i : ans) cout << i << ' ';
                cout << '\n';
            }
        }
    }

    return 0;
}