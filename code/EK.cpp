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

int64_t ek(
  std::vector<std::set<int64_t>> const& adj,
  std::vector<std::vector<int64_t>> &cap,
  int64_t s,
  int64_t t,
  Timer &timer
) {
  int64_t flow = 0;
  std::vector<int64_t> pred(adj.size(), -1);

  while(true) {
    if (timer.finished()) return -1;

    int64_t new_flow = 0;

    std::queue<std::pair<int64_t, int64_t>> q;
    q.emplace(s, std::numeric_limits<int64_t>::max());
    pred[s] = -2;

    while(!q.empty() && new_flow == 0) {
      int64_t cur = q.front().first;
      int64_t cur_flow = q.front().second;
      q.pop();

      for (int64_t next : adj[cur]) {
        if (pred[next] == -1 && cap[cur][next] > 0) {
          pred[next] = cur;
          int64_t aux = std::min(cur_flow, cap[cur][next]);
          if (next == t) {
            new_flow = aux;
            break;
          }
          q.emplace(next, aux);
        }
      }
    }

    if (new_flow > 0) {
      flow += new_flow;
      for(int64_t cur = t; cur != s;) {
        int64_t prev = pred[cur];
        cap[prev][cur] -= new_flow;
        cap[cur][prev] += new_flow;
        cur = prev;
      }
      std::fill(pred.begin(), pred.end(), -1);
    } else {
      break;
    }
  }

  return flow;
}

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

  std::vector<std::set<int64_t>> adj(v);
  std::vector<std::vector<int64_t>> cap(v, std::vector<int64_t>(v, 0));
  for(int64_t i = 0; i < e; ++i) {
    int64_t s, t, c;
    fin >> s >> t >> c;
    s -= 1;
    t -= 1;
    adj[s].emplace(t);
    adj[t].emplace(s);
    cap[s][t] += c;
  }

  Timer timer(timeout);
  int64_t ans = ek(adj, cap, 0, v-1, timer);
  double elapsed = timer.elapsed();

  std::cout << (elapsed > timeout ? -1 : ans) << "\n";
  std::cout << elapsed << "\n";

  return 0;
}
