#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <chrono>
#include <tuple>
#include <algorithm>
#include <sstream>
#include <unordered_set>
#include <iomanip>

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

bool isValidCycle(const vector<P>& path) {
    if (path.empty()) return false;
    unordered_set<int> ids;
    for (const auto& p : path) {
        if (ids.count(p.id)) return false;
        ids.insert(p.id);
    }
    return true;
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
        for (int i = 0; i < n; ++i) {
            for (int j = i + 2; j < n; ++j) {
                int next_i = (i + 1) % n;
                int next_j = (j + 1) % n;
                if (next_i == j || next_j == i) continue;
                float delta = -d[i][next_i] - d[j][next_j] + d[i][j] + d[next_i][next_j];
                if (delta < -1e-6) {
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
                    vector<tuple<int, int, int>> cases = {
                        {i + 1, j, k},
                        {i + 1, k, j},
                        {j, i + 1, k},
                        {j, k, i + 1},
                        {k, i + 1, j},
                        {k, j, i + 1}
                    };

                    for (const auto& case_item : cases) {
                        int a = get<0>(case_item);
                        int b = get<1>(case_item);
                        int c = get<2>(case_item);
                        if (a >= b || b >= c || c > n) continue;

                        vector<P> tmp = v;
                        reverse(tmp.begin() + a, tmp.begin() + b);
                        reverse(tmp.begin() + b, tmp.begin() + c);

                        float len = pathLen(tmp);
                        if (len < res - 1e-6) {
                            v = tmp;
                            res = len;
                            chg = true;
                            goto next_segment;
                        }
                    }
                }
                next_segment:;
            }
        }
    }
    return res;
}

int main() {
    ifstream fin("tsp_100_1.txt");
    if (!fin) {
        cerr << "Failed to open input file.\n";
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
    vector<P> pts2 = pts;
    vector<P> pts3 = pts;

    auto t1 = chrono::high_resolution_clock::now();
    float len2 = opt2(pts2, maxIt);
    auto t2 = chrono::high_resolution_clock::now();
    double tOpt2 = chrono::duration<double>(t2 - t1).count();

    auto t3 = chrono::high_resolution_clock::now();
    float len3 = opt3(pts3, maxIt);
    auto t4 = chrono::high_resolution_clock::now();
    double tOpt3 = chrono::duration<double>(t4 - t3).count();

    cout << fixed << setprecision(2);
    cout << len2 << " " << (isValidCycle(pts2) ? 1 : 0) << "\n";
    for (auto& p : pts2) cout << p.id << " ";
    cout << "\n" << tOpt2 << "\n";

    cout << len3 << " " << (isValidCycle(pts3) ? 1 : 0) << "\n";
    for (auto& p : pts3) cout << p.id << " ";
    cout << "\n" << tOpt3 << "\n";

    return 0;
}
