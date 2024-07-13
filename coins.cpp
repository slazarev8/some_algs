#include <cmath>
#include <cstring>
#include <iostream>
#include <queue>
#include <set>
#include <sstream>
#include <unordered_set>
#include <vector>

using namespace std;

const int INF = 1e9;

int n, m;

inline bool isValid(int i, int j) {
    return i >= 0 && i < n && j >= 0 && j < m;
}

void flipCoin(char& c) {
    if (c == '0') {
        c = '1';
    } else {
        c = '0';
    }
}

unsigned long long hash_board(const vector<string>& board) {
    unsigned long long hash_value = 0;
    int n = board.size(), m = board[0].size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            hash_value += (board[i][j] - '0') * std::pow(2, (i * m) + j);
        }
    }

    return hash_value;
}

typedef pair<int, int> pii;

string getFlipID(const pii& p1, const pii& p2) {
    ostringstream oss;
    oss << p1.first << "," << p1.second << "_" << p2.first << "," << p2.second;

    // If you want to make sure that the order of p1, p2 does not matter
    string id1 = oss.str();
    return id1;
    oss.str(""); // Reset the ostringstream
    oss << p2.first << "," << p2.second << "_" << p1.first << "," << p1.second;
    string id2 = oss.str();

    // Return the lexicographically smaller ID to ensure consistency
    if (id1 < id2)
        return id1;
    else
        return id2;
}

int bfs(const vector<string>& start, const vector<string>& end) {
    const pii moves[][2] = {{{0, 0}, {0, 1}}, {{0, 0}, {1, 0}}};

    queue<vector<string>> q;
    q.push(start);

    int dist[n][m];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            dist[i][j] = INF;
        }
    }

    dist[0][0] = 0;
    std::unordered_set<string> visited;

    int currLvl = 0;
    queue<vector<string>> newQ;
    while (!q.empty()) {
        auto now = q.front();
        q.pop();
        if (now == end)
            return currLvl;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                for (const pii* m : moves) {
                    pii p1 = {i + m[0].first, j + m[0].second};
                    pii p2 = {i + m[1].first, j + m[1].second};

                    if (isValid(p1.first, p1.second) && isValid(p2.first, p2.second)) {
                        vector<string> next = now;
                        flipCoin(next[p1.first][p1.second]);
                        flipCoin(next[p2.first][p2.second]);

                        string flipID = getFlipID(
                            p1, p2); // You need to implement getFlipID(p1, p2) function that
                                     // returns a unique ID string for each pair of (p1, p2)
                        if (!visited.count(flipID)) {
                            newQ.push(next);
                            visited.insert(flipID);
                        }
                    }
                }
            }
        }
        if (q.empty()) {
            q = newQ;
            newQ = queue<vector<string>>();
            currLvl++;
        }
    }

    return -1;
}

int main() {
    vector<string> field;
    cin >> n >> m;
    field.resize(n);

    for (int i = 0; i < n; i++) {
        cin >> field[i];
    }

    vector<string> ans1(n, string(m, '0')), ans2(n, string(m, '0'));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if ((i + j) % 2) {
                ans1[i][j] = '1';
            } else {
                ans2[i][j] = '1';
            }
        }
    }

    int answer = min(bfs(field, ans1), bfs(field, ans2));

    cout << answer << endl;
    return 0;
}
