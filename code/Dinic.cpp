#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <set>
#include <utility>
#include <vector>

class Timer {
public:
  Timer(double timelimit) :
    m_timelimit(timelimit),
    m_start(std::chrono::high_resolution_clock::now())
  {}

  double elapsed() const {
    auto now = std::chrono::high_resolution_clock::now();
    auto dur = std::chrono::duration<double>(now - m_start);
    return dur.count();
  }

  bool finished() const {
    return elapsed() > m_timelimit;
  }

private:
  double m_timelimit;
  std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

// Dinic struct adapted from https://cp-algorithms.com/graph/dinic.html
struct Dinic {
  struct FlowEdge {
    int v, u;
    long long cap, flow = 0;
    FlowEdge(int v, int u, long long cap) : v(v), u(u), cap(cap) {}
  };
  const long long flow_inf = 1e18;
  std::vector<FlowEdge> edges;
  std::vector<std::vector<int>> adj;
  int n, m = 0;
  int s, t;
  std::vector<int> level, ptr;
  std::queue<int> q;

  Dinic(int n, int s, int t) : n(n), s(s), t(t) {
    adj.resize(n);
    level.resize(n);
    ptr.resize(n);
  }

  void add_edge(int v, int u, long long cap) {
    edges.emplace_back(v, u, cap);
    edges.emplace_back(u, v, 0);
    adj[v].push_back(m);
    adj[u].push_back(m + 1);
    m += 2;
  }

  bool bfs() {
    while (!q.empty()) {
      int v = q.front();
      q.pop();
      for (int id : adj[v]) {
        if (edges[id].cap - edges[id].flow < 1)
          continue;
        if (level[edges[id].u] != -1)
          continue;
        level[edges[id].u] = level[v] + 1;
        q.push(edges[id].u);
      }
    }
    return level[t] != -1;
  }

  long long dfs(int v, long long pushed) {
    if (pushed == 0)
      return 0;
    if (v == t)
      return pushed;
    for (int& cid = ptr[v]; cid < (int)adj[v].size(); cid++) {
      int id = adj[v][cid];
      int u = edges[id].u;
      if (level[v] + 1 != level[u] || edges[id].cap - edges[id].flow < 1)
        continue;
      long long tr = dfs(u, std::min(pushed, edges[id].cap - edges[id].flow));
      if (tr == 0)
        continue;
      edges[id].flow += tr;
      edges[id ^ 1].flow -= tr;
      return tr;
    }
    return 0;
  }

  long long flow(Timer const& timer) {
    long long f = 0;
    while (!timer.finished()) {
      fill(level.begin(), level.end(), -1);
      level[s] = 0;
      q.push(s);
      if (!bfs())
        break;
      fill(ptr.begin(), ptr.end(), 0);
      while (long long pushed = dfs(s, flow_inf)) {
        f += pushed;
      }
    }
    return f;
  }
};

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Error: wrong number of arguments\n";
    std::cerr << "Usage: " << argv[0] << " TIMEOUT INPUTFILE\n";
    return 1;
  }

  double timeout = std::stod(argv[1]);
  std::ifstream fin(argv[2]);

  int v, e;
  fin >> v >> e;

  Dinic d(v, 0, v-1);
  for(int i = 0; i < e; ++i) {
    int s, t;
    long long c;
    fin >> s >> t >> c;
    d.add_edge(s-1, t-1, c);
  }

  Timer timer(timeout);
  long long ans = d.flow(timer);
  double elapsed = timer.elapsed();

  std::cout << (elapsed > timeout ? -1 : ans) << "\n";
  std::cout << elapsed << "\n";

  return 0;
}
