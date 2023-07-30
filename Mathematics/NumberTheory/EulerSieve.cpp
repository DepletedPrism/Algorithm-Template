// EulerSieve: https://depletedprism.github.io/笔记/euler-sieve/
// DeP

const int MAXN = 1e6 + 5;

int Prime[MAXN], notPrime[MAXN], tot;

int phi[MAXN], mu[MAXN];

void EulerSieve(void) {
  notPrime[1] = true;
  phi[1] = mu[1] = 1;
  for (int i = 2; i < MAXN; i++) {
    if (!notPrime[i]) {
      Prime[++tot] = i;
      phi[i] = i - 1, mu[i] = -1;
    }
    for (int j = 1; j <= tot && i * Prime[j] < MAXN; j++) {
      notPrime[i*Prime[j]] = true;
      if (!(i % Prime[j])) {
        phi[i*Prime[j]] = phi[i] * Prime[j];
        mu[i*Prime[j]] = 0;
        break;
      }
      phi[i*Prime[j]] = phi[i] * phi[Prime[j]];
      mu[i*Prime[j]] = -mu[i];
    }
  }
}
