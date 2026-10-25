#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct Edge {
    int to;
    int weight;       // station-to-station segments = 1 in this version
    string lineName;
};

class MetroNetwork {
private:
    vector<string> stationNames;
    unordered_map<string, int> stationIds;
    vector<vector<Edge>> graph;
    vector<vector<string>> lineStations;
    vector<string> lineNames;

    static string normalize(const string& input) {
        string out;
        for (unsigned char c : input) {
            if (!isspace(c)) out += static_cast<char>(tolower(c));
        }
        return out;
    }

    int getOrCreateStation(const string& name) {
        const string key = normalize(name);
        auto it = stationIds.find(key);
        if (it != stationIds.end()) return it->second;
        int id = static_cast<int>(stationNames.size());
        stationNames.push_back(name);
        stationIds[key] = id;
        graph.emplace_back();
        return id;
    }

    void addLine(const string& lineName, const vector<string>& stops) {
        if (stops.size() < 2) return;
        lineNames.push_back(lineName);
        lineStations.push_back(stops);
        for (const auto& stop : stops) getOrCreateStation(stop);

        for (size_t i = 0; i + 1 < stops.size(); ++i) {
            int u = stationIds.at(normalize(stops[i]));
            int v = stationIds.at(normalize(stops[i + 1]));
            graph[u].push_back({v, 1, lineName});
            graph[v].push_back({u, 1, lineName});
        }
    }

    bool loadData(const string& filename) {
        ifstream in(filename);
        if (!in) return false;

        string line;
        while (getline(in, line)) {
            if (line.empty() || line[0] == '#') continue;
            stringstream ss(line);
            string lineName, stopsField;
            if (!getline(ss, lineName, '|') || !getline(ss, stopsField)) continue;

            vector<string> stops;
            stringstream stopStream(stopsField);
            string stop;
            while (getline(stopStream, stop, '|')) {
                if (!stop.empty()) stops.push_back(stop);
            }
            addLine(lineName, stops);
        }
        return !lineStations.empty();
    }

public:
    bool load(const string& filename) { return loadData(filename); }

    void listStations() const {
        vector<string> names = stationNames;
        sort(names.begin(), names.end());
        cout << "\nStations loaded: " << names.size() << "\n";
        for (const auto& name : names) cout << "  - " << name << '\n';
    }

    void listLines() const {
        cout << "\nLines loaded: " << lineNames.size() << "\n";
        for (size_t i = 0; i < lineNames.size(); ++i)
            cout << "  " << lineNames[i] << " (" << lineStations[i].size() << " listed stops)\n";
    }

    void findRoute(const string& source, const string& destination) const {
        auto sIt = stationIds.find(normalize(source));
        auto dIt = stationIds.find(normalize(destination));
        if (sIt == stationIds.end()) {
            cout << "Source station not found. Use option 2 to view supported station names.\n";
            return;
        }
        if (dIt == stationIds.end()) {
            cout << "Destination station not found. Use option 2 to view supported station names.\n";
            return;
        }

        const int src = sIt->second, dst = dIt->second;
        const int n = static_cast<int>(stationNames.size());
        const int INF = numeric_limits<int>::max();
        vector<int> dist(n, INF), parent(n, -1);
        vector<string> parentLine(n);
        using State = pair<int, int>;
        priority_queue<State, vector<State>, greater<State>> pq;
        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty()) {
            State current = pq.top();
            pq.pop();
            int d = current.first, u = current.second;
            if (d != dist[u]) continue;
            if (u == dst) break;

            for (const Edge& e : graph[u]) {
                int nd = d + e.weight;
                if (nd < dist[e.to]) {
                    dist[e.to] = nd;
                    parent[e.to] = u;
                    parentLine[e.to] = e.lineName;
                    pq.push({nd, e.to});
                }
            }
        }

        if (dist[dst] == INF) {
            cout << "No route found in the loaded dataset.\n";
            return;
        }

        vector<int> path;
        for (int cur = dst; cur != -1; cur = parent[cur]) path.push_back(cur);
        reverse(path.begin(), path.end());

        cout << "\n========== SHORTEST ROUTE ==========\n";
        cout << "From: " << stationNames[src] << "\nTo:   " << stationNames[dst] << "\n\n";
        cout << "Route:\n";
        string previousLine;
        int changes = 0;
        for (size_t i = 0; i < path.size(); ++i) {
            if (i) {
                const string& line = parentLine[path[i]];
                if (!previousLine.empty() && line != previousLine) {
                    cout << "\n   [Change line: " << previousLine << " -> " << line << "]\n";
                    ++changes;
                }
                cout << " -> ";
            }
            cout << stationNames[path[i]];
            if (i + 1 < path.size()) previousLine = parentLine[path[i + 1]];
        }
        cout << "\n\nStation-to-station segments: " << dist[dst] << '\n';
        cout << "Stations in route: " << path.size() << '\n';
        cout << "Line changes (approx.): " << changes << '\n';
        cout << "Note: all adjacent-stop edges weigh 1, so this minimizes stops/segments,\n"
             << "not real kilometres, fare, or travel time. Transfer walking links are not modelled.\n";
        cout << "====================================\n";
    }
};

int main() {
    MetroNetwork metro;
    if (!metro.load("metro_lines.txt")) {
        cerr << "Could not load metro_lines.txt.\n"
             << "Run metro.exe from the project folder containing this data file.\n";
        return 1;
    }

    cout << "============================================\n";
    cout << "  DELHI-NCR METRO SHORTEST ROUTE FINDER\n";
    cout << "  Dijkstra's Algorithm | C++17\n";
    cout << "============================================\n";

    while (true) {
        cout << "\n1. Find shortest route\n2. List all stations\n3. List metro lines\n4. Exit\nChoice: ";
        int choice;
        if (!(cin >> choice)) break;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            string source, destination;
            cout << "Source station: ";
            getline(cin, source);
            cout << "Destination station: ";
            getline(cin, destination);
            metro.findRoute(source, destination);
        } else if (choice == 2) {
            metro.listStations();
        } else if (choice == 3) {
            metro.listLines();
        } else if (choice == 4) {
            cout << "Goodbye!\n";
            break;
        } else {
            cout << "Please enter 1, 2, 3, or 4.\n";
        }
    }
    return 0;
}
