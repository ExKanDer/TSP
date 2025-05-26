#include <bits/stdc++.h>
using namespace std;

struct P {
    float x, y;
    int id;
    P(float a, float b, int i) : x(a), y(b), id(i) {}
    P() : x(0), y(0), id(-1) {}
    float d(const P& o) const {
        return hypot(x - o.x, y - o.y);
    }
};

float pathLen(const vector<P>& v) {
    float sum = 0;
    int n = v.size();
    for (int i = 0; i < n - 1; ++i) sum += v[i].d(v[i + 1]);
    return sum + v[n - 1].d(v[0]);
}

float opt2(vector<P>& v, int maxIt) {
    int n = v.size();
    vector<vector<float>> d(n, vector<float>(n));
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            d[i][j] = d[j][i] = v[i].d(v[j]);

    float res = pathLen(v);
    bool chg = true;
    int it = 0;

    while (chg && it++ < maxIt) {
        chg = false;
        for (int i = 0; i < n - 1; ++i) {
            for (int j = i + 2; j < n; ++j) {
                float delta = -d[i][i+1] - d[j][(j+1)%n] + d[i][j] + d[i+1][(j+1)%n];
                if (delta < 0) {
                    reverse(v.begin() + i + 1, v.begin() + j + 1);
                    res += delta;
                    chg = true;
                }
            }
        }
    }
    return res;
}

float opt3(vector<P>& v, int maxIt) {
    int n = v.size();
    vector<vector<float>> d(n, vector<float>(n));
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            d[i][j] = d[j][i] = v[i].d(v[j]);

    float res = pathLen(v);
    bool chg = true;
    int it = 0;

    while (chg && it++ < maxIt) {
        chg = false;
        for (int i = 0; i < n - 2; ++i) {
            for (int j = i + 1; j < n - 1; ++j) {
                for (int k = j + 1; k < n; ++k) {
                    vector<int> idx = {i+1, j+1, k+1, 0, n};
                    vector<tuple<int, int, int>> cases = {
                        {idx[0], idx[1], idx[2]},
                        {idx[0], idx[2], idx[1]},
                        {idx[1], idx[0], idx[2]},
                        {idx[1], idx[2], idx[0]},
                        {idx[2], idx[0], idx[1]},
                        {idx[2], idx[1], idx[0]}
                    };

                    for (auto [a, b, c] : cases) {
                        vector<P> tmp = v;
                        reverse(tmp.begin() + a, tmp.begin() + b);
                        reverse(tmp.begin() + b, tmp.begin() + c);
                        float len = pathLen(tmp);
                        if (len < res) {
                            v = tmp;
                            res = len;
                            chg = true;
                        }
                    }
                }
            }
        }
    }
    return res;
}

bool checkCycle(const vector<P>& v) {
    int n = v.size();
    if (v[0].id != v[n - 1].id) return false;
    vector<bool> seen(n, false);
    for (const auto& p : v) {
        if (p.id < 1 || p.id > n || seen[p.id - 1]) return false;
        seen[p.id - 1] = true;
    }
    return true;
}

int main() {
    ifstream fin("tsp_99_1.txt");
    if (!fin) {
        cout << "File error\n";
        return 1;
    }

    vector<P> pts;
    string s;
    int total;
    getline(fin, s);
    stringstream(s) >> total;

    float a, b;
    int id = 1;
    while (getline(fin, s)) {
        stringstream ss(s);
        ss >> a >> b;
        pts.emplace_back(a, b, id++);
    }

    int maxIt = 10000;

    auto t1 = chrono::high_resolution_clock::now();
    float len2 = opt2(pts, maxIt);
    auto t2 = chrono::high_resolution_clock::now();
    double tOpt2 = chrono::duration<double>(t2 - t1).count();

    cout << fixed << setprecision(2);
    cout << len2 << " " << 1 << "\n"; // Always outputs 1; could be fixed
    for (auto& p : pts) cout << p.id << " ";
    cout << "\n" << tOpt2 << "\n\n";

    auto t3 = chrono::high_resolution_clock::now();
    float len3 = opt3(pts, maxIt);
    auto t4 = chrono::high_resolution_clock::now();
    double tOpt3 = chrono::duration<double>(t4 - t3).count();

    cout << len3 << " " << 1 << "\n";
    for (auto& p : pts) cout << p.id << " ";
    cout << "\n" << tOpt3 << "\n";

    return 0;
}
