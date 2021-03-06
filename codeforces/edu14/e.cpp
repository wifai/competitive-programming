#include <bits/stdc++.h>

using namespace std;

const int N = 105;
const long long mod = 1e9 + 7;
struct matrix {
  int n, m;
  long long _mat[N][N];
  matrix(int n, int m) : n(n), m(m) {
  }
  void reset() {
    memset(_mat, 0, sizeof _mat);
    for(int i = 0; i<n; i++)
      _mat[i][i] = 1;
  }
  const long long * operator[](const int id) const {
    return _mat[id];
  }
  long long * operator[](const int id) {
    return _mat[id];
  }
  matrix operator*(const matrix& mat) const {
    matrix ret(this->n, mat.m);
    for(int i = 0; i<this->n; i++) {
      for(int j = 0; j<mat.m; j++) {
        long long &now = ret[i][j];
        now = 0;
        for(int k = 0; k<this->m; k++) {
          now = (now + this->_mat[i][k] * mat[k][j]) % mod;
        }
      }
    }
    return ret;
  }
  void print() {
    for(int i = 0; i<n; i++) {
      for(int j = 0; j<m; j++)
        cout << _mat[i][j] << " ";
      cout << endl;
    }
  }
  matrix operator^(long long pw) const {
    matrix ret(n, m), base = *this;
    ret.reset();
    while(pw) {
      if(pw & 1)
        ret = ret*base;
      base = base*base;
      pw >>= 1;
    }
    return ret;
  }
};
long long a[N];
int main() {
  int n;
  long long k;
  scanf("%d%I64d", &n, &k);
  for(int i = 0; i<n; i++)
    scanf("%I64d", a+i);
  matrix m(n, n);
  for(int i = 0; i<n; i++)
    for(int j = 0; j<n; j++) {
      m[i][j] = (__builtin_popcountll(a[i]^a[j]) %3) == 0;
    }
  matrix a(n, 1);
  for(int i = 0; i<n; i++) {
    a[i][0] = 1;
  }
  matrix res = (m^(k-1)) * a;
  long long ans = 0;
  for(int i = 0; i<n; i++)
    ans = (ans + res[i][0]) % mod;
  printf("%I64d\n", ans);
  return 0;
}