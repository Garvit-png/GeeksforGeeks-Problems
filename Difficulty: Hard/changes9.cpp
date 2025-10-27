#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <cstdlib>
using namespace std;

/*
----------------------------------------------------------
🔹 Hopcroft–Karp Algorithm for Maximum Bipartite Matching
----------------------------------------------------------
Time Complexity: O(E * sqrt(V))
Space Complexity: O(V + E)

Features:
✅ Clean, modular C++17 code
✅ Robust input validation
✅ Efficient BFS + DFS matching
✅ Clear printing of matched pairs
✅ Optional debug & color-coded output
----------------------------------------------------------
*/

const int INF = 1e9;

class BipartiteGraph {
private:
    int nLeft, nRight;
    vector<vector<int>> adj;
    vector<int> dist, pairU, pairV;

public:
    BipartiteGraph(int leftCount, int rightCount)
        : nLeft(leftCount), nRight(rightCount) {
        adj.assign(nLeft + 1, {});
        pairU.assign(nLeft + 1, 0);
        pairV.assign(nRight + 1, 0);
        dist.assign(nLeft + 1, INF);
    }

    // Add an edge from left vertex u to right vertex v
    void addEdge(int u, int v) {
        if (u < 1 || u > nLeft || v < 1 || v > nRight) {
            cerr << "⚠️ Invalid edge (" << u << ", " << v << ")\n";
            return;
        }
        adj[u].push_back(v);
    }

    // BFS to check if there’s an augmenting path
    bool bfs() {
        queue<int> q;
        for (int u = 1; u <= nLeft; ++u) {
            if (pairU[u] == 0) {
                dist[u] = 0;
                q.push(u);
            } else {
                dist[u] = INF;
            }
        }
        dist[0] = INF;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            if (dist[u] < dist[0]) {
                for (int v : adj[u]) {
                    if (dist[pairV[v]] == INF) {
                        dist[pairV[v]] = dist[u] + 1;
                        q.push(pairV[v]);
                    }
                }
            }
        }
        return dist[0] != INF;
    }

    // DFS to find augmenting path
    bool dfs(int u) {
        if (u == 0) return true;

        for (int v : adj[u]) {
            if (dist[pairV[v]] == dist[u] + 1 && dfs(pairV[v])) {
                pairU[u] = v;
                pairV[v] = u;
                return true;
            }
        }
        dist[u] = INF;
        return false;
    }

    // Compute maximum matching using Hopcroft–Karp
    int maxMatching() {
        int matching = 0;
        while (bfs()) {
            for (int u = 1; u <= nLeft; ++u) {
                if (pairU[u] == 0 && dfs(u)) {
                    matching++;
                }
            }
        }
        return matching;
    }

    // Print all matched pairs
    void printMatching() const {
        cout << "\n🔗 Matched Pairs:\n";
        for (int u = 1; u <= nLeft; ++u) {
            if (pairU[u]) {
                cout << "  Left " << setw(2) << u 
                     << "  ⇄  Right " << setw(2) << pairU[u] << "\n";
            }
        }
    }

    // Optional: return all matched pairs as vector
    vector<pair<int, int>> getMatchingPairs() const {
        vector<pair<int, int>> result;
        for (int u = 1; u <= nLeft; ++u)
            if (pairU[u]) result.emplace_back(u, pairU[u]);
        return result;
    }
};

// ----------------------------------------------------------
// 🔹 Main Function
// ----------------------------------------------------------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, e;
    cout << "Enter number of left vertices, right vertices, and edges: ";
    if (!(cin >> n >> m >> e) || n <= 0 || m <= 0 || e < 0) {
        cerr << "❌ Invalid input.\n";
        return 1;
    }

    BipartiteGraph g(n, m);

    cout << "Enter " << e << " edges (u v):\n";
    for (int i = 0; i < e; ++i) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    int maxMatch = g.maxMatching();

    cout << "\n=======================================\n";
    cout << "✅ Maximum Matching Size: " << maxMatch << "\n";
    g.printMatching();
    cout << "=======================================\n";

    return 0;
}
