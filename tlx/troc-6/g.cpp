#include <bits/stdc++.h>

using namespace std;

const int N = 1e5 + 5, LG = 20;

vector<pair<int, int>> g[N];

int preorder[N], en[N], ex[N], cntr = 0;

int par[LG][N], col[LG][N], lv[N];

void dfs(int v, int p = -1, int c = -1) {
  lv[v] = p == -1 ? 0 : lv[p] + 1;
  par[0][v] = p == -1 ? v : p;
  col[0][v] = p == -1 ? 0 : 1 << c;
  for (int i = 0; i + 1 < LG; ++i) {
    par[i+1][v] = par[i][par[i][v]];
    col[i+1][v] = col[i][v] | col[i][par[i][v]];
  }
  preorder[cntr] = v;
  en[v] = cntr++;
  for (auto it : g[v]) {
    int u = it.first, c = it.second;
    if (u == p) continue;
    dfs(u, v, c);
  }
  ex[v] = cntr;
}

int lca(int u, int v) {
  if (lv[u] > lv[v]) {
    swap(u, v);
  }
  int d = lv[v] - lv[u];
  for (int i = 0; i < LG; ++i) {
    if ((d >> i) & 1) {
      v = par[i][v];
    }
  }
  if (u == v)
    return v;
  for (int i = LG-1; i >= 0; --i) {
    if (par[i][u] != par[i][v])
      u = par[i][u], v = par[i][v];
  }
  return par[0][v];
}

int ancestor(int u, int naik) {
  for (int i = 0; i < LG; ++i) {
    if ((naik >> i) & 1) {
      u = par[i][u];
    }
  }
  return u;
}

int color(int u, int naik) {
  int ret = 0;
  for (int i = 0; i < LG; ++i) {
    if ((naik >> i) & 1) {
      ret |= col[i][u];
      u = par[i][u];
    }
  }
  return ret;
}

struct node {
  long long c, m;
  node() {
    c = m = 0;
  }
  long long get(long long x) {
    return x * m + c;
  }
  bool ada() {
    return c != 0 || m != 0;
  }
};

node seg[N << 2];
long long ans[N];
vector<int> muncul[N]; 
vector<pair<int, int>> ask[N];
int n, k;

vector<pair<int, int>> get_mask(int w, int u) {
  int cur = color(u, lv[u] - lv[w]);
  vector<pair<int, int> > ret;
  while (cur) {
    int lo = 0, ri = lv[u] - lv[w];
    while (lo < ri) {
      int mid = (lo + ri) >> 1;
      int x = ancestor(u, mid);
      if (color(x, lv[x] - lv[w]) == cur)
        lo = mid+1;
      else
        ri = mid;
    }
    int v = ancestor(u, lo);
    ret.emplace_back(cur, lv[u] - lv[v]);
    u = v;
    cur = color(u, lv[u] - lv[w]);
  }
  sort(ret.begin(), ret.end());
  return ret;
}

void upd(int id, long long m, long long c) {
  seg[id].m += m;
  seg[id].c += c;
}

void propagate(int id, int il, int ir) {
  if (!seg[id].ada())
    return;
  upd(il, seg[id].m, seg[id].c);
  upd(ir, seg[id].m, seg[id].c);
  seg[id].m = seg[id].c = 0;
}

void add(int x, int y, int m, int c, int id = 1, int l = 0, int r = n) {
  if (x >= r || l >= y) return;
  if (x <= l && r <= y) {
    upd(id, m, c);
    return;
  }
  int mid = (l + r) >> 1, il = id << 1, ir = il | 1;
  propagate(id, il, ir);
  add(x, y, m, c, il, l, mid);
  add(x, y, m, c, ir, mid, r);
}

long long get(int x, long long v, int id = 1, int l = 0, int r = n) {
  if (r-l < 2) {
    return seg[id].get(v);
  }
  int mid = (l + r) >> 1, il = id << 1, ir = il | 1;
  propagate(id, il, ir);
  return x < mid ? get(x, v, il, l, mid) : get(x, v, ir, mid, r);
}

void dfs2(int v, int p = -1) {
  for (auto it : g[v]) {
    if (it.first == p) continue;
    dfs2(it.first, v);
  }
  for (int x : muncul[v]) {
    add(en[x], ex[x], -1, lv[v] + 1);
  }
  for (auto it : ask[v]) {
    int u = it.first, id = it.second;
    ans[id] += get(en[u], lv[v]);
  }
}

int main() {
  scanf("%d %d", &n, &k);
  int all = (1<< k) - 1;
  for (int i = 1; i < n; ++i) {
    int u, v;
    char c;
    scanf("%d %d %c", &u, &v, &c);
    --u, --v;
    g[u].emplace_back(v, c-'a');
    g[v].emplace_back(u, c-'a');
  }
  dfs(0);
  for (int i = 0; i < n; ++i) {
    if (col[LG-1][i] != all) continue;
    int lo = 0, ri = lv[i];
    while (lo < ri) {
      int mid = (lo + ri) >> 1;
      if (color(i, mid) != all) {
        lo = mid + 1;
      }
      else
        ri = mid;
    }
    muncul[ancestor(i, lo)].push_back(i);
  }
  int q;
  scanf("%d", &q);
  for (int x = 0; x < q; ++x) {
    int u, v;
    scanf("%d %d", &u, &v);
    --u, --v;
    int w = lca(u, v);
    if (u != w)
      ask[w].emplace_back(u, x);
    if (v != w)
      ask[w].emplace_back(v, x);
    ans[x] = 0;
    if (u != w && v != w) {
      vector<pair<int, int>> lef = get_mask(w, u);
      vector<pair<int, int>> rig = get_mask(w, v);
      for (int i = 0, sum = 0, j = (int)rig.size() - 1; i < (int)lef.size(); ++i) {
        while (j >= 0 && (lef[i].first | rig[j].first) == all) {
          sum += rig[j].second;
          --j;
        }
        ans[x] += sum;
      }
    }
  }

  dfs2(0);

  for (int i = 0; i < q; ++i)
    printf("%lld\n", ans[i]);
  return 0;
}
