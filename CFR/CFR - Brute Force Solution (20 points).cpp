//solutie brut force de 20p

#include <bits/stdc++.h>

using namespace std;

ifstream fin("cfr.in");
ofstream fout("cfr.out");

const int kN = 1e3;
const int mod = 1e9 + 7;
int node, dp[1 + kN], C[1 + kN][1 + kN];
vector<pair<int, int>> g[1 + kN];
bitset<1 + kN> vis;

int add(int x, const int &y) {
  x += y;
  if (x >= mod) {
    x -= mod;
  }
  return x;
}

void precalc() {
  C[0][0] = 1;
  for (int i = 1; i <= kN; ++i) {
    C[i][0] = 1;
    for (int j = 1; j <= i; ++j) {
      C[i][j] = add(C[i - 1][j - 1], C[i - 1][j]);
    }
  }
}

void dfs(int u, int par, int m) {
  vis[u] = true;
  if (dp[node] <= dp[u]) {
    node = u;
  }
  for (auto it : g[u]) {
    int v, w;
    tie(v, w) = it;
    if (v != par && w <= m) {
      dp[v] = dp[u] + 1;
      dfs(v, u, m);
    }
  }
}

void testCase() {
  int task, n;
  fin >> task >> n;
  for (int i = 1; i <= n; ++i) {
    g[i].clear();
  }
  for (int i = 1; i < n; ++i) {
    int u, v, w;
    fin >> u >> v >> w;
    g[u].emplace_back(v, w);
    g[v].emplace_back(u, w);
  }
  int Q;
  fin >> Q;
  for (int qq = 0; qq < Q; ++qq) {
    int m, k;
    fin >> m >> k;
    vis.reset();
    vector<int> d;
    for (int v = 1; v <= n; ++v) {
      if (!vis[v]) {
        node = 0;
        dp[v] = 0;
        dfs(v, 0, m);
        int start = node;
        node = 0;
        dp[start] = 0;
        dfs(start, 0, m);
        d.emplace_back(dp[node]);
      }
    }
    sort(d.rbegin(), d.rend());
    k += 1;
    if ((int)d.size() < k) {
      k = d.size();
    }
    if (task == 1) {
      int sum = k - 1;
      for (int i = 0; i < k; ++i) {
        sum += d[i];
      }
      fout << sum << '\n';
    } else {
      int l = k - 1, r = k - 1;
      while (l > 0 && d[l - 1] == d[k - 1]) {
        l -= 1;
      }
      while (r < (int)d.size() - 1 && d[r + 1] == d[k - 1]) {
        r += 1;
      }
      fout << C[r - l + 1][k - l] << '\n';
    }
  }
}

int main() {
  precalc();
  int tests;
  fin >> tests;
  for (int tc = 0; tc < tests; ++tc) {
    testCase();
  }
  fin.close();
  fout.close();
  return 0;
}
