#include <iostream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

const int VERTEX_COUNT = 6;
const int INF = 1000000000;

vector<string> placeNames;
int road[VERTEX_COUNT][VERTEX_COUNT];

struct DijkstraResult {
    vector<int> distance;
    vector<int> parent;
};

void initializeGraph() {
    placeNames.clear();
    placeNames.push_back("Hotel");
    placeNames.push_back("Museum");
    placeNames.push_back("Beach");
    placeNames.push_back("Fort");
    placeNames.push_back("Park");
    placeNames.push_back("Market");

    int i, j;
    for (i = 0; i < VERTEX_COUNT; ++i) {
        for (j = 0; j < VERTEX_COUNT; ++j) {
            road[i][j] = INF;
        }
        road[i][i] = 0;
    }

    // Road distances are in kilometres. Each road works in both directions.
    road[0][1] = road[1][0] = 4;  // Hotel - Museum
    road[0][2] = road[2][0] = 7;  // Hotel - Beach
    road[1][2] = road[2][1] = 2;  // Museum - Beach
    road[1][3] = road[3][1] = 5;  // Museum - Fort
    road[2][3] = road[3][2] = 3;  // Beach - Fort
    road[2][4] = road[4][2] = 6;  // Beach - Park
    road[3][4] = road[4][3] = 2;  // Fort - Park
    road[3][5] = road[5][3] = 7;  // Fort - Market
    road[4][5] = road[5][4] = 3;  // Park - Market
}

DijkstraResult dijkstra(int source) {
    DijkstraResult result;
    result.distance.assign(VERTEX_COUNT, INF);
    result.parent.assign(VERTEX_COUNT, -1);

    vector<bool> visited(VERTEX_COUNT, false);
    result.distance[source] = 0;

    int step;
    for (step = 0; step < VERTEX_COUNT; ++step) {
        int u = -1;
        int i;

        for (i = 0; i < VERTEX_COUNT; ++i) {
            if (!visited[i] &&
                (u == -1 || result.distance[i] < result.distance[u])) {
                u = i;
            }
        }

        if (u == -1 || result.distance[u] == INF) {
            break;
        }

        visited[u] = true;
        int v;
        for (v = 0; v < VERTEX_COUNT; ++v) {
            if (road[u][v] != INF &&
                result.distance[u] + road[u][v] < result.distance[v]) {
                result.distance[v] = result.distance[u] + road[u][v];
                result.parent[v] = u;
            }
        }
    }

    return result;
}

void displayResults(const DijkstraResult& result, int source) {
    cout << "\nShortest distances from " << placeNames[source] << ":\n";

    int i;
    for (i = 0; i < VERTEX_COUNT; ++i) {
        if (i == source) {
            continue;
        }

        cout << "  " << placeNames[i] << ": ";
        if (result.distance[i] == INF) {
            cout << "unreachable\n";
        } else {
            cout << result.distance[i] << " km\n";
        }
    }

    vector<int> reachable;
    for (i = 0; i < VERTEX_COUNT; ++i) {
        if (i != source && result.distance[i] != INF) {
            reachable.push_back(i);
        }
    }

    // Selection sort by distance; use the place name to break ties.
    int a;
    for (a = 0; a < static_cast<int>(reachable.size()); ++a) {
        int best = a;
        int b;
        for (b = a + 1; b < static_cast<int>(reachable.size()); ++b) {
            int candidate = reachable[b];
            int currentBest = reachable[best];
            if (result.distance[candidate] < result.distance[currentBest] ||
                (result.distance[candidate] == result.distance[currentBest] &&
                 placeNames[candidate] < placeNames[currentBest])) {
                best = b;
            }
        }
        int temporary = reachable[a];
        reachable[a] = reachable[best];
        reachable[best] = temporary;
    }

    cout << "\nReachable attractions, nearest first:\n";
    if (reachable.empty()) {
        cout << "  None\n";
    }
    for (i = 0; i < static_cast<int>(reachable.size()); ++i) {
        int place = reachable[i];
        cout << "  " << placeNames[place] << " - "
             << result.distance[place] << " km\n";
    }
}

int main() {
    initializeGraph();

    const int hotel = 0;
    DijkstraResult result = dijkstra(hotel);
    displayResults(result, hotel);

    return 0;
}

