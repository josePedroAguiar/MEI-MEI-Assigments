#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <list>
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

// MPM struct adapted from https://cp-algorithms.com/graph/mpm.html
struct MPM {
  struct FlowEdge{
    int v, u;
    long long cap, flow;
    FlowEdge(){}
    FlowEdge(int _v, int _u, long long _cap, long long _flow)
      : v(_v), u(_u), cap(_cap), flow(_flow){}
    FlowEdge(int _v, int _u, long long _cap)
      : v(_v), u(_u), cap(_cap), flow(0ll){}
  };
  const long long flow_inf = 1e18;
  std::vector<FlowEdge> edges;
  std::vector<char> alive;
  std::vector<long long> pin, pout;
  std::vector<std::list<int> > in, out;
  std::vector<std::vector<int> > adj;
  std::vector<long long> ex;
  int n, m = 0;
  int s, t;
  std::vector<int> level;
  std::vector<int> q;
  int qh, qt;
  void resize(int _n){
    n = _n;
    ex.resize(n);
    q.resize(n);
    pin.resize(n);
    pout.resize(n);
    adj.resize(n);
    level.resize(n);
    in.resize(n);
    out.resize(n);
  }
  MPM(){}
  MPM(int _n, int _s, int _t){resize(_n); s = _s; t = _t;}
  void add_edge(int v, int u, long long cap){
    edges.push_back(FlowEdge(v, u, cap));
    edges.push_back(FlowEdge(u, v, 0));
    adj[v].push_back(m);
    adj[u].push_back(m + 1);
    m += 2;
  }
  bool bfs(){
    while(qh < qt){
      int v = q[qh++];
      for(int id : adj[v]){
        if(edges[id].cap - edges[id].flow < 1)continue;
        if(level[edges[id].u] != -1)continue;
        level[edges[id].u] = level[v] + 1;
        q[qt++] = edges[id].u;
      }
    }
    return level[t] != -1;
  }
  long long pot(int v){
    return std::min(pin[v], pout[v]);
  }
  void remove_node(int v){
    for(int i : in[v]){
      int u = edges[i].v;
      auto it = find(out[u].begin(), out[u].end(), i);
      out[u].erase(it);
      pout[u] -= edges[i].cap - edges[i].flow;
    }
    for(int i : out[v]){
      int u = edges[i].u;
      auto it = find(in[u].begin(), in[u].end(), i);
      in[u].erase(it);
      pin[u] -= edges[i].cap - edges[i].flow;
    }
  }
  void push(int from, int to, long long f, bool forw){
    qh = qt = 0;
    ex.assign(n, 0);
    ex[from] = f;
    q[qt++] = from;
    while(qh < qt){
      int v = q[qh++];
      if(v == to)
        break;
      long long must = ex[v];
      auto it = forw ? out[v].begin() : in[v].begin();
      while(true){
        int u = forw ? edges[*it].u : edges[*it].v;
        long long pushed = std::min(must, edges[*it].cap - edges[*it].flow);
        if(pushed == 0)break;
        if(forw){
          pout[v] -= pushed;
          pin[u] -= pushed;
        }
        else{
          pin[v] -= pushed;
          pout[u] -= pushed;
        }
        if(ex[u] == 0)
          q[qt++] = u;
        ex[u] += pushed;
        edges[*it].flow += pushed;
        edges[(*it)^1].flow -= pushed;
        must -= pushed;
        if(edges[*it].cap - edges[*it].flow == 0){
          auto jt = it;
          ++jt;
          if(forw){
            in[u].erase(find(in[u].begin(), in[u].end(), *it));
            out[v].erase(it);
          }
          else{
            out[u].erase(find(out[u].begin(), out[u].end(), *it));
            in[v].erase(it);
          }
          it = jt;
        }
        else break;
        if(!must)break;
      }
    }
  }
  long long flow(Timer const& timer){
    long long ans = 0;
    while(!timer.finished()){
      pin.assign(n, 0);
      pout.assign(n, 0);
      level.assign(n, -1);
      alive.assign(n, true);
      level[s] = 0;
      qh = 0; qt = 1;
      q[0] = s;
      if(!bfs())
        break;
      for(int i = 0; i < n; i++){
        out[i].clear();
        in[i].clear();
      }
      for(int i = 0; i < m; i++){
        if(edges[i].cap - edges[i].flow == 0)
          continue;
        int v = edges[i].v, u = edges[i].u;
        if(level[v] + 1 == level[u] && (level[u] < level[t] || u == t)){
          in[u].push_back(i);
          out[v].push_back(i);
          pin[u] += edges[i].cap - edges[i].flow;
          pout[v] += edges[i].cap - edges[i].flow;
        }
      }
      pin[s] = pout[t] = flow_inf;
      while(true){
        int v = -1;
        for(int i = 0; i < n; i++){
          if(!alive[i])continue;
          if(v == -1 || pot(i) < pot(v))
            v = i;
        }
        if(v == -1)
          break;
        if(pot(v) == 0){
          alive[v] = false;
          remove_node(v);
          continue;
        }
        long long f = pot(v);
        ans += f;
        push(v, s, f, false);
        push(v, t, f, true);
        alive[v] = false;
        remove_node(v);
      }
    }
    return ans;
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

  int64_t v, e;
  fin >> v >> e;

  MPM m(v, 0, v-1);
  for(int64_t i = 0; i < e; ++i) {
    int64_t s, t, c;
    fin >> s >> t >> c;
    m.add_edge(s-1, t-1, c);
  }

  Timer timer(timeout);
  int64_t ans = m.flow(timer);
  double elapsed = timer.elapsed();

  std::cout << (elapsed > timeout ? -1 : ans) << "\n";
  std::cout << elapsed << "\n";

  return 0;
}
