#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <deque>
#include <queue>
#include <map>
#include <set>
#include <cmath>
#include <ctime>
#include <random>

using namespace std;

struct Road {
    int to;
    int L;
    int street;
};

struct RoadWithString {
    int to;
    int L;
    string street;
};

int D, I, S, V, F;
vector < string > streets;
vector < vector < Road > > g;
vector < vector < Road > > gt;
vector < vector < RoadWithString > > gtt;
vector < vector < int > > roots;
map <string, int> MapRoadToInt;
map <int, string> MapIntToRoad;
map <int, map < int, int> > MapIntIntToL;
map <int, int > MapEndOfRoad;
map <int, int > MapRoadToL;
map <int, int > IthCross;

struct cross {
    vector < pair < int, int > > schedule;
};

vector < cross > ans;

void ReadInput(const string& in) {
    ifstream FileIn("C:\\Users\\panen\\CLionProjects\\HashCode2021\\" + in);
    FileIn>>D>>I>>S>>V>>F;
    g.clear();
    g.resize(I);
    gtt.clear();
    gtt.resize(I);
    map<string, pair<int,int> > Map;
    for (int i = 0; i < S; i++) {
        int u, v, l;
        string roadname;
        FileIn>>u>>v>>roadname>>l;
        g[u].push_back({v, l, -1});
        gtt[v].push_back({u, l, roadname});
        Map[roadname] = {u, g[u].size() - 1};
    }

    int cnt = -1;
    for (auto &e : Map) {
        cnt++;
        streets.push_back(e.first);
        g[e.second.first][e.second.second].street = cnt;
        MapIntIntToL[e.second.first][g[e.second.first][e.second.second].to] = g[e.second.first][e.second.second].L;
        //g[e.second.first][e.second.second].street = cnt;
        MapEndOfRoad[cnt] = g[e.second.first][e.second.second].to;
        MapIntToRoad[cnt] = e.first;
        MapRoadToL[cnt] = g[e.second.first][e.second.second].L;
        MapRoadToInt[e.first] = cnt;
    }

    for (int i = 0; i <= cnt; i++) {
        if (MapEndOfRoad[i] < 0 || MapEndOfRoad[i] >= I) {
            cout<<"I can't read input\n";
            exit(0);
        }
    }

    /*for (int i = 0; i < g.size(); i++) {
        for (auto &j : g[i]) {
            MapIntIntToL[j.street] = j.L;
        }
    }*/

    gt.clear();
    gt.resize(I);
    for (int i = 0; i < gtt.size(); i++) {
        //gt[i].resize(gtt[i].size());
        for (int j = 0; j < gtt[i].size(); j++) {
            gt[i].push_back({gtt[i][j].to, gtt[i][j].L, MapRoadToInt[gtt[i][j].street]});
        }
    }

    roots.clear();
    roots.resize(V);
    for (int i = 0; i < V; i++) {
        int nroads;
        FileIn >> nroads;
        for (int j = 0; j < nroads; j++) {
            string r;
            FileIn>>r;
            roots[i].push_back(MapRoadToInt[r]);
        }
    }

    FileIn.close();
}

struct node {
    int f, s, t;
    string st;
};

void DeleteUselessRoads(const string& in) {
    ifstream FileIn("C:\\Users\\panen\\CLionProjects\\HashCode2021\\" + in);
    FileIn>>D>>I>>S>>V>>F;
    vector < node > vec(S);
    set < string > roads;
    vector < set < string > > income(I);
    vector < set < string > > outcome(I);

    for (int i = 0; i < S; i++) {
        FileIn>>vec[i].f>>vec[i].s>>vec[i].st>>vec[i].t;
        income[vec[i].s].insert(vec[i].st);
        if (vec[i].f < 0 || vec[i].f >= outcome.size()) {
            cout<<"Here "<<vec[i].f;
            exit(0);
        }
        outcome[vec[i].f].insert(vec[i].st);
    }

    vector < vector < string > > r(V);
    for (int i = 0; i < V; i++) {
        int cnt;
        FileIn>>cnt;
        for (int j = 0; j < cnt; j++) {
            string s;
            FileIn>>s;
            r[i].push_back(s);
            roads.insert(s);
        }
    }

    int numberstreets = S;
    for (int i = 0; i < S; i++) {
        if (!roads.count(vec[i].st)) {
            numberstreets--;
            income[vec[i].s].erase(vec[i].st);
            outcome[vec[i].f].erase(vec[i].st);
        }
    }
    //int numberinter = I;

    FileIn.close();
    ofstream FileOut("C:\\Users\\panen\\CLionProjects\\HashCode2021\\" + in);
    FileOut << D << " " << I << " " << numberstreets << " " << V << " "<<F<<"\n";
    for (int i = 0; i < S; i++) {
        if (roads.count(vec[i].st)) {
            FileOut<<vec[i].f<<" "<<vec[i].s<<" "<<vec[i].st<<" "<<vec[i].t<<"\n";
        }
    }

    for (int i = 0; i < V; i++) {
        FileOut<<r[i].size()<<" ";
        for (auto &j : r[i]) {
            FileOut<<j<<" ";
        }
        FileOut<<"\n";
    }

    FileOut.close();
}

void PrintInput(const string& out) {
    ofstream FileOut("C:\\Users\\panen\\CLionProjects\\HashCode2021\\" + out);
    FileOut<<D<<" "<<I<<" "<<S<<" "<<V<<" "<<F<<"\n";
    for (int i = 0; i < I; i++) {
        FileOut<<i<<": ";
        for (auto j : g[i]) {
            FileOut<<"["<<j.to<<", "<<j.street<<", "<<j.L<<"]; ";
        }
        FileOut<<"\n";
    }
    for (auto &s : streets) {
        FileOut<<s<<" ";
    }
    FileOut<<"\n";
    for (int i = 0; i < roots.size(); i++) {
        FileOut<<"car "<<i<<": ";
        for (auto j : roots[i]) {
            FileOut<<j<<" ";
        }
        FileOut<<"\n";
    }
    FileOut.close();
}

void solve() {
    ans.clear();
    ans.resize(I);
    for (int i = 0; i < I; i++) {
        //cout<<i<<"\n";
        //cout<<gt[i].size()<<"\n";
        //ans[i].schedule.resize(gt[i].size());
        if (gt[i].empty()) {
            continue;
        }
        IthCross[i] = IthCross.size();
        for (int j = 0; j < gt[i].size(); j++) {
            //ans[i].schedule.push_back({gt[i][j], 1});
            ans[i].schedule.emplace_back(gt[i][gt[i].size() - 1 - j].street, 1);
        }
    }

    //return;
    vector < int > through(S, 0);
    for (int i = 0; i < roots.size(); i++) {
        for (auto j : roots[i]) {
            through[j]++;
        }
    }

    vector < pair<int,int> > temp;
    for (int i = 0; i < through.size(); i++) {
        temp.push_back({through[i], i});
    }

    sort(temp.rbegin(), temp.rend());
    vector < int > order(temp.size());
    for (int i = 0; i < order.size(); i++) {
        order[temp[i].second] = temp[i].first;
    }
    ans.clear();
    ans.resize(I);
    srand(time(0));
    for (int i = 0; i < I; i++) {
        //cout<<i<<"\n";
        //cout<<gt[i].size()<<"\n";
        //ans[i].schedule.resize(gt[i].size());
        int period = 0;
        for (int j = 0; j < gt[i].size(); j++) {
            //ans[i].schedule.push_back({gt[i][j], 1});
            //ans[i].schedule.emplace_back(gt[i][j].street, 1);
            ans[i].schedule.emplace_back(gt[i][j].street, order[gt[i][j].to]);
            period += ans[i].schedule.back().second;
        }
        sort(ans[i].schedule.begin(), ans[i].schedule.end(), [](pair<int,int> a, pair<int,int> b) {
            return (a.second < b.second);
        });
        for (int j = 0; j < gt[i].size(); j++) {
            //ans[i].schedule.push_back({gt[i][j], 1});
            //ans[i].schedule.emplace_back(gt[i][j].street, 1);
            //ans[i].schedule[j].second = ans[i].schedule[j].second * 20 / period + 1;
            ans[i].schedule[j].second = 5;
            //ans[i].schedule[j].second = 1 + max(1, int(0.05 * (rand() % 1000)));
        }
        //reverse(ans[i].schedule.begin(), ans[i].schedule.end());
    }
}

void SolveByProcessing() {
    vector < int > periodonint(I, 0);
    //vector < vector < int > > prefsum(I);
    //vector < set < int > > prefsumset(I);
    //map < int, int > MapRoadToBeginOfBurning;
    for (int i = 0; i < I; i++) {
        periodonint[i] = gt[i].size();
    }

    vector < vector < int > > Schedule(I);
    vector < set < int > > IsSet(I);
    for (int i = 0; i < Schedule.size(); i++) {
        Schedule[i].resize(gt[i].size(), -1);
    }

    vector < queue < int > > ForS(S);
    priority_queue < pair<int,int>, vector < pair<int,int> >, greater<pair<int,int>> > events;
    for (int i = 0; i < V; i++) {
        ForS[roots[i][0]].push(i);
    }
    for (int i = 0; i < S; i++) {
        if (!ForS[i].empty()) {
            events.push({0, i});
        }
    }
    vector < int > whereiscar(V); //on which road
    for (auto &i : whereiscar) {
        i = 0;
    }


    int res = 0;
    int additional = 0;
    int count = 0;
    vector < set < int > > fromout(D); //what crosses we have used at moment D
    vector < set < int > > LookAtRoad(D);
    vector < queue < pair<int,int> > > rest(D);
    for (int t = 0; t < D; t++) {
        while (!rest[t].empty()) {
            ForS[rest[t].front().first].push(rest[t].front().second);
            rest[t].pop();
        }
        while (!events.empty() && events.top().first == t) {
            auto curr = events.top(); //curr.second - index of current road
            events.pop();
            if (ForS[curr.second].empty() || LookAtRoad[t].count(curr.second)) {
                continue;
            }
            LookAtRoad[t].insert(curr.second);
            int car = ForS[curr.second].front();

            int cross = MapEndOfRoad[curr.second];
            int now = t % periodonint[cross]; //in which block light are burning
            int to = roots[car][whereiscar[car] + 1]; //to - next road
            int time = t;
            int tt;
            if (!IsSet[cross].count(curr.second)) {
                for (tt = now; Schedule[cross][tt] != -1;
                     time++, tt = (tt + 1) % periodonint[cross]) {}

                Schedule[cross][tt] = curr.second;
                IsSet[cross].insert(curr.second);
            } else {
                for (tt = now; Schedule[cross][tt] != curr.second;
                     time++, tt = (tt + 1) % periodonint[cross]) {}
            }

            if (time != t) { // if we can't now moving the car then try it later
                events.push({time, curr.second});
                continue;
            }
            events.push({t + 1, curr.second});

            if (fromout[t].count(cross)) {
                continue;
            }
            fromout[t].insert(cross);
            ForS[curr.second].pop();

            time += MapRoadToL[to];
            whereiscar[car]++;
            if (to == roots[car].back()) { //on the last road
                if (D - time >= 0) {
                    res += (D - time) + F;
                    count++;
                    additional += D - time;
                }
                continue; //remove this car
            }

            events.push({time, to});
            //ForS[to].push(car);
            if (time < D) {
                rest[time].push({to, car});
            }
        }
        fromout[t].clear();
        LookAtRoad[t].clear();
    }
    //cout<<"("<<count<<"; "<<additional<<");\n";

    ans.clear();
    ans.resize(I);
    for (int i = 0; i < I; i++) {
        if (!IsSet[i].empty()) {
            set < pair<int,int> > PosStr;
            for (int j = 0; j < Schedule[i].size(); j++) {
                if (Schedule[i][j] == -1) {
                    continue;
                }
                PosStr.insert({j, Schedule[i][j]});
                //ans.back().schedule.push_back({Schedule[i][j], 1});
            }
            auto fir = *PosStr.begin();
            fir.first = 0;
            PosStr.erase(PosStr.begin());
            PosStr.insert(fir);
            PosStr.insert({Schedule[i].size(), -1});
            while (!PosStr.empty()) {
                auto fir = *PosStr.begin();
                PosStr.erase(PosStr.begin());
                if (PosStr.empty()) {
                    break;
                }
                auto sec = *PosStr.begin();
                ans[i].schedule.push_back({fir.second, sec.first - fir.first});
            }
        }
    }
}

int CalculateAnswer() {
    vector < int > periodonint(I, 0);
    vector < vector < int > > prefsum(I);
    vector < set < int > > prefsumset(I);
    map < int, int > MapRoadToBeginOfBurning;
    for (int i = 0; i < I; i++) {
        if (ans[i].schedule.empty()) {
            continue;
        }
        prefsum[i].push_back(periodonint[i]);
        prefsumset[i].insert(periodonint[i]);
        for (auto el : ans[i].schedule) {
            periodonint[i] += el.second;
            MapRoadToBeginOfBurning[el.first] = prefsum[i].back();
            prefsum[i].push_back(periodonint[i]);
            prefsumset[i].insert(periodonint[i]);
        }
    }
    for (auto &i : periodonint) {
        i = max(i, 1);
    }

    vector < queue < int > > ForS(S);
    priority_queue < pair<int,int>, vector < pair<int,int> >, greater<pair<int,int>> > events;
    for (int i = 0; i < V; i++) {
        ForS[roots[i][0]].push(i);
    }
    for (int i = 0; i < S; i++) {
        if (!ForS[i].empty()) {
            events.push({0, i});
        }
    }
    vector < int > whereiscar(V); //on which road
    for (auto &i : whereiscar) {
        i = 0;
    }


    int res = 0;
    int additional = 0;
    int count = 0;
    vector < set < int > > fromout(D); //what crosses we have used at moment D
    vector < set < int > > LookAtRoad(D);
    vector < queue < pair<int,int> > > rest(D);
    for (int t = 0; t < D; t++) {
        while (!rest[t].empty()) {
            ForS[rest[t].front().first].push(rest[t].front().second);
            rest[t].pop();
        }
        while (!events.empty() && events.top().first == t) {
            auto curr = events.top(); //curr.second - index of current cross
            events.pop();
            if (ForS[curr.second].empty() || LookAtRoad[t].count(curr.second)) {
                continue;
            }
            LookAtRoad[t].insert(curr.second);
            int car = ForS[curr.second].front();

            int cross = MapEndOfRoad[curr.second];
            int now = t % periodonint[cross]; //in which block light are burning
            int start = MapRoadToBeginOfBurning[curr.second]; //when this road is starting to burn
            auto it = prefsumset[cross].upper_bound(start);
            int end = (it == prefsumset[cross].end() ? periodonint[cross] : *it);
            int to = roots[car][whereiscar[car] + 1]; //to - next road
            int time = t + ((start <= now && now < end) ? (0) :
                    ((start + periodonint[cross] - now) % periodonint[cross]));

            if (time != t) { // if we can't now moving the car then try it later
                events.push({time, curr.second});
                continue;
            }
            events.push({t + 1, curr.second});

            if (fromout[t].count(cross)) {
                continue;
            }
            fromout[t].insert(cross);
            ForS[curr.second].pop();

            time += MapRoadToL[to];
            whereiscar[car]++;
            if (to == roots[car].back()) { //on the last road
                if (D - time >= 0) {
                    res += (D - time) + F;
                    count++;
                    additional += D - time;
                }
                continue; //remove this car
            }

            events.push({time, to});
            //ForS[to].push(car);
            if (time < D) {
                rest[time].push({to, car});
            }
        }
        fromout[t].clear();
        LookAtRoad[t].clear();
    }
    //cout<<"("<<count<<"; "<<additional<<");\n";
    return res;
}

void AnnulateSomething() {
    srand(time(0));
    int cross = rand()  % I;
    if (ans[cross].schedule.size() < 2) {
        return;
    }
    int fir = rand() % ans[cross].schedule.size();
    int curr = CalculateAnswer();
    auto el = ans[cross].schedule[fir];
    //ans[cross].schedule.erase(ans[cross].schedule.begin() + fir);
    {
        for (int j = fir; j + 1 < ans[cross].schedule.size(); j++) {
            ans[cross].schedule[j] = ans[cross].schedule[j + 1];
        }
        ans[cross].schedule.pop_back();
    }
    //ans[cross].schedule[fir].second = 0;
    int next = CalculateAnswer();
    if (curr > next) {
        //ans[cross].schedule[fir].second = 0;
        //ans[cross].schedule.insert(ans[cross].schedule.begin() + fir, el);
        {
            ans[cross].schedule.push_back({-1, -1});
            for (int j = (int)ans[cross].schedule.size() - 1; j > fir; j--) {
                ans[cross].schedule[j] = ans[cross].schedule[j - 1];
            }
            ans[cross].schedule[fir] = el;
        }
    }
}

void SwapSomething() {
    srand(time(0));
    int cross = rand() % I;
    if (ans[cross].schedule.size() < 2) {
        return;
    }
    int fir = rand() % ans[cross].schedule.size();
    int sec;
    do {
        sec = rand() % ans[cross].schedule.size();
    } while (fir == sec);
    int curr = CalculateAnswer();
    swap(ans[cross].schedule[fir], ans[cross].schedule[sec]);
    int after = CalculateAnswer();
    if (curr > after) { //we don't improve answer
        swap(ans[cross].schedule[fir], ans[cross].schedule[sec]);
    } else {
        cout<<"upd: "<<after<<"\n";
    }
}

void TryToIncrease() {
    srand(time(0));
    int cross;
    do {
        cross = rand() % I;
    } while (ans[cross].schedule.size() < 2);
    int fir = rand() % ans[cross].schedule.size();
    int curr = CalculateAnswer();
    int before = curr;
    int val = 4;
    int step = 0;
    int after;
    do {
        ans[cross].schedule[fir].second += val + step;
        if (ans[cross].schedule[fir].second > D) {
            break;
        }
        after = CalculateAnswer();
        if (curr > after) { //before answer was better
            break;
        }
        curr = after;
        step += val;
    } while (true);
    cout<<"step = "<<step<<"\n";

    ans[cross].schedule[fir].second -= val + step;

    if (before <= curr) {
        cout<<"upd: "<<curr<<"\n";
    }
}

void TryToIncreaseAll() {
    srand(time(0));
    for (int cross = 0; cross < I; cross++) {
        cout<<"\t\tcross = "<<cross<<"\n";
        if (ans[cross].schedule.size() < 2) {
            continue;
        }
        int fir = rand() % ans[cross].schedule.size();
        int curr = CalculateAnswer();
        int before = curr;
        int val = 4;
        int step = 0;
        int after;
        do {
            ans[cross].schedule[fir].second += val + step;
            if (ans[cross].schedule[fir].second > D) {
                break;
            }
            after = CalculateAnswer();
            if (curr > after) { //before answer was better
                break;
            }
            curr = after;
            step += val;
        } while (true);
        cout<<"step = "<<step<<"\n";

        ans[cross].schedule[fir].second -= val + step;

        if (before <= curr) {
            cout<<"upd: "<<curr<<"\n";
        }
    }
}

void TryToDecrease() {
    srand(time(0));
    int cross;
    do {
        cross = rand() % I;
    } while (ans[cross].schedule.size() < 2);

    int fir = rand() % ans[cross].schedule.size();
    int curr = CalculateAnswer();
    if (ans[cross].schedule[fir].second == 1) {
        return;
    }
    ans[cross].schedule[fir].second--;
    int after = CalculateAnswer();
    if (curr > after) {
        ans[cross].schedule[fir].second++;
    }
}

void SwapAll() {
    for (int i = 0; i < I; i++) {
        cout<<"i = "<<i<<"\n";
        if (ans[i].schedule.size() < 2) {
            continue;
        }
        /*vector < int > order(ans[i].schedule.size());
        for (int j = 0; j < order.size(); j++) {
            order[j] = j;
        }*/
        for (int q = 1; q; q--) {
            if (q % 10 == 0) {
                cout << "q = " << q << "\n";
            }
            int fir = rand() % ans[i].schedule.size();
            int sec;
            do {
                sec = rand() % ans[i].schedule.size();
            } while (fir == sec);
            int curr = CalculateAnswer();
            swap(ans[i].schedule[fir], ans[i].schedule[sec]);
            int after = CalculateAnswer();
            if (curr > after) { //we don't improve answer
                swap(ans[i].schedule[fir], ans[i].schedule[sec]);
            }
        }
    }
}

void PropagateAnswer() {
    cout<<"before: "<<CalculateAnswer()<<"\n";
    for (int t = 0; t < 1; t++) {
        if (t % 1 == 0) {
            cout << "\tTest " << t << ";\n";
        }
        //SwapSomething();
        SwapAll();
        //TryToIncrease();
        //TryToIncreaseAll();
        //TryToDecrease();
        //AnnulateSomething();
    }
    cout<<"after: "<<CalculateAnswer()<<"\n";
}

void PrintAns(const string& out) {
    cout<<"My answer is "<<CalculateAnswer()<<"\n";
    //return;
    ofstream FileOut("C:\\Users\\panen\\CLionProjects\\HashCode2021\\" + out);
    int cnt = 0;
    for (int i = 0; i < ans.size(); i++) {
        cnt += !ans[i].schedule.empty();
    }
    FileOut<<cnt<<"\n";
    for (int i = 0; i < ans.size(); i++) {
        if (ans[i].schedule.empty()) {
            continue;
        }
        int ccnt = 0;
        for (auto &el : ans[i].schedule) {
            ccnt += el.first != -1;
        }
        FileOut<<i<<"\n";
        FileOut<<ccnt<<"\n";
        //FileOut<<ans[i].schedule.size()<<"\n";
        for (auto &el : ans[i].schedule) {
            if (el.first == -1) {
                //cout<<"Oops\n";
                //exit(0);
                continue;
            }
            //FileOut<<el.first<<" "<<el.second<<"\n";
            FileOut<<MapIntToRoad[el.first]<<" "<<el.second<<"\n";
        }
    }
    FileOut.close();
}

void DefineAns() {
    ans.clear();
    ans.resize(4);
    ans[0].schedule.push_back({2, 2});
    ans[1].schedule.push_back({0, 2});
    ans[1].schedule.push_back({1, 2});
    ans[2].schedule.push_back({3, 2});
    ans[3].schedule.push_back({4, 2});
}

void ReadFromOutInAns(const string& out) {
    ifstream FileIn("C:\\Users\\panen\\CLionProjects\\HashCode2021\\" + out);
    ans.clear();
    int n;
    FileIn>>n;
    ans.resize(I);
    for (int i = 0; i < n; i++) {
        int inter;
        int cnt;
        FileIn>>inter>>cnt;
        for (int j = 0; j < cnt; j++) {
            string s;
            int l;
            FileIn>>s>>l;
            ans[inter].schedule.push_back({MapRoadToInt[s], l});
        }
    }
    FileIn.close();
}

int main() {
    for (char let = 'e'; let <= 'e'; let++) {
        string testin = string(1, let) + ".in";
        string testout = string(1, let) + ".out";
        //DeleteUselessRoads(testin);
        ReadInput(testin);
        //PrintInput(testout);
        //ReadFromOutInAns(testout);
        //PropagateAnswer();
        SolveByProcessing();
        //solve();
        //DefineAns();
        PrintAns(testout);

    }
}