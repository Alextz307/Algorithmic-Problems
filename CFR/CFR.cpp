#include <bits/stdc++.h>

using namespace std;

ifstream fin("cfr.in");
ofstream fout("cfr.out");

const int kN = 1e5;
const int kM = 2e5;
const int kLog = 17;
const int mod = 1e9 + 7;
int m, f[1 + kN], invf[1 + kN], sol[1 + kN], first[1 + kN], tour[1 + kM], dep[1 + kM], lg2[1 + kM], rmq[1 + kLog][1 + kM];
vector<int> g[1 + kN];

struct edge_t {
  int u, v, w;

  void read() {
    fin >> u >> v >> w;
  }

  void addEdge() {
    g[u].emplace_back(v);
    g[v].emplace_back(u);
  }

  bool operator < (const edge_t &rhs) const {
    return w < rhs.w;
  }
} edges[kN];

struct query_t {
  int m, k, index;

  void read() {
    fin >> m >> k;
  }

  bool operator < (const query_t &rhs) const {
    return m < rhs.m;
  }
} q[kN];

struct node {
  int sum, cnt;

  node operator + (const node &rhs) const {
    return {sum + rhs.sum, cnt + rhs.cnt};
  }
} NIL{0, 0};

struct ST {
  int n;
  vector<node> t;

  void init(int N) {
    n = N;
    int dim = 1;
    while (dim < n) {
      dim *= 2;
    }
    t.assign(dim * 2, NIL);
  }

  void update(int x, int lx, int rx, int pos, int v) {
    if (lx == rx) {
      t[x].sum += v * lx;
      t[x].cnt += v;
      return;
    }
    int mid = (lx + rx) / 2;
    if (pos <= mid) {
      update(x * 2, lx, mid, pos, v);
    } else {
      update(x * 2 + 1, mid + 1, rx, pos, v);
    }
    t[x] = t[x * 2] + t[x * 2 + 1];
  }

  void update(int pos, int v) {
    update(1, 0, n - 1, pos, v);
  }

  int findPos(int x, int lx, int rx, int req) {
    if (lx == rx) {
      if (t[x].cnt < req) {
        return lx;
      }
      return lx + 1;
    }
    int mid = (lx + rx) / 2;
    if (t[x * 2 + 1].cnt < req) {
      return findPos(x * 2, lx, mid, req - t[x * 2 + 1].cnt);
    }
    return findPos(x * 2 + 1, mid + 1, rx, req);
  }
    
  int findPos(int req) {
  	return findPos(1, 0, n - 1, req);
  }

  node query(int x, int lx, int rx, int st, int dr) {
    if (st <= lx && rx <= dr) {
      return t[x];
    }
    int mid = (lx + rx) / 2;
    node left = NIL, right = NIL;
    if (st <= mid) {
      left = query(x * 2, lx, mid, st, dr);
    }
    if (mid < dr) {
      right = query(x * 2 + 1, mid + 1, rx, st, dr);
    }
    return left + right;
  }

  node query(int st, int dr) {
    return query(1, 0, n - 1, st, dr);
  }
} t;

void multSelf(int &x, const int &y) {
  x = (int64_t)x * y % mod;
}

int mult(int x, const int &y) {
  multSelf(x, y);
  return x;
}

int Pow(int x, int n) {
  int ans = 1;
  while (n != 0) {
    if (n % 2 == 1) {
      multSelf(ans, x);
    }
    multSelf(x, x);
    n /= 2;
  }
  return ans;
}

int invers(int x) {
  return Pow(x, mod - 2);
}

void precalc() {
  f[0] = f[1] = 1;
  for (int i = 2; i <= kN; ++i) {
    f[i] = mult(f[i - 1], i);
  }
  invf[kN] = invers(f[kN]);
  for (int i = kN - 1; i >= 0; --i) {
    invf[i] = mult(invf[i + 1], i + 1);
  }
  for (int i = 2; i <= kM; ++i) {
    lg2[i] = lg2[i / 2] + 1;
  }
  for (int i = 1; i <= kM; ++i) {
    rmq[0][i] = i;
  }
}

int nck(int n, int k) {
  return mult(f[n], mult(invf[k], invf[n - k]));
}

void dfs(int u, int par, int level) {
  tour[++m] = u;
  dep[m] = level;
  first[u] = m;
  for (int v : g[u]) {
    if (v != par) {
      dfs(v, u, level + 1);
      tour[++m] = u;
      dep[m] = level;
    }
  }
}

void computeRmq() {
  for (int i = 1; (1 << i) <= m; ++i) {
    for (int j = 1; j <= m - (1 << i) + 1; ++j) {
      int l = (1 << (i - 1));
      rmq[i][j] = rmq[i - 1][j];
      if (dep[rmq[i - 1][j + l]] < dep[rmq[i][j]]) {
        rmq[i][j] = rmq[i - 1][j + l];
      }
    }
  }
}

int lca(int u, int v, bool flag = false) {
  int x = first[u], y = first[v];
  if (y < x) {
    swap(x, y);
  }
  int len = y - x + 1;
  int l = lg2[len];
  int index = rmq[l][x];
  int shift = len - (1 << l);
  if (dep[rmq[l][x + shift]] < dep[index]) {
    index = rmq[l][x + shift];
  }
  return tour[index];
}

int dist(int u, int v) {
  return dep[first[u]] + dep[first[v]] - 2 * dep[first[lca(u, v)]];
}

struct DSU {
  vector<int> p, sz;
  vector<array<int, 2>> ends;

  DSU(int n) {
    p.resize(n + 1);
    iota(p.begin(), p.end(), 0);
    sz.assign(n + 1, 1);
    ends.resize(n + 1);
    for (int i = 1; i <= n; ++i) {
      ends[i] = {i, i};
    }
  }

  int root(int x) {
    if (x == p[x]) {
      return x;
    }
    return p[x] = root(p[x]);
  }

  bool unite(int u, int v) {
    int x = root(u), y = root(v);
    if (x == y) {
      return false;
    }
    if (sz[y] < sz[x]) {
      swap(x, y);
    }
    array<int, 2> nodes;
    int dX = dist(ends[x][0], ends[x][1]), dY = dist(ends[y][0], ends[y][1]);
    if (dX < dY) {
      nodes = ends[y];
    } else {
      nodes = ends[x];
    }
    int best = dist(nodes[0], nodes[1]);
    for (const int &st : ends[x]) {
      for (const int &dr : ends[y]) {
        int ret = dist(st, dr);
        if (best < ret) {
          best = ret;
          nodes = {st, dr};
        }
      }
    }
    t.update(dX, -1);
    t.update(dY, -1);
    t.update(best, 1);
    ends[y] = nodes;
    p[x] = y;
    sz[y] += sz[x];
    return true;
  }
};

void testCase() {
  int task, n;
  fin >> task >> n;
  for (int i = 1; i <= n; ++i) {
    g[i].clear();
  }
  for (int i = 0; i < n - 1; ++i) {
    edges[i].read();
    edges[i].addEdge();
  }
  sort(edges, edges + n - 1);
  int Q;
  fin >> Q;
  for (int i = 0; i < Q; ++i) {
    q[i].read();
    q[i].k += 1;
    q[i].index = i;
  }
  m = 0;
  dfs(1, 0, 0);
  computeRmq();
  sort(q, q + Q);
  DSU dsu(n);
  t.init(n);
  t.update(0, n);
  int ptr = 0;
  for (int i = 0; i < Q; ++i) {
    while (ptr < n - 1 && edges[ptr].w <= q[i].m) {
      dsu.unite(edges[ptr].u, edges[ptr].v);
      ptr += 1;
    }
    int minD = t.findPos(q[i].k);
    if (minD) {
      minD -= 1;
    }
    node ret = t.query(minD, n - 1);
    int ways = 1;
    if (ret.cnt <= q[i].k) {
      q[i].k = ret.cnt;
    } else {
      node last = t.query(minD, minD);
      int rem = ret.cnt - q[i].k;
      ret.sum -= rem * minD;
      ways = nck(last.cnt, rem);
    }
    if (task == 1) {
      sol[q[i].index] = ret.sum + q[i].k - 1;
    } else {
      sol[q[i].index] = ways;
    }
  }
  for (int i = 0; i < Q; ++i) {
    fout << sol[i] << '\n';
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
