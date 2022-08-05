#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;

pair<vector<string>, vector<vector<ld>>> input(){
    ifstream input_file("../example data/01_single_linear_regression.csv");
    string rows;
    vector<string> row_split;
    vector<vector<ld>> dt;
    string row;
    getline(input_file, rows);
    stringstream ss(rows);
    while(getline(ss, row, ',')){
        row_split.push_back(row);
    }
    vector<vector<ld>> data;
    while(getline(input_file, rows)){
        ss.clear();
        ss.str(rows);
        vector<ld> row_data;
        long double x;
        while(getline(ss, row, ',')){
            row_data.push_back(stold(row));
        }
        data.push_back(row_data);
    }
    input_file.close();
    return {row_split, data};
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    setlocale(LC_ALL, "ko_KR.UTF-8");

    // cin.imbue(locale("ko_KR.UTF-8"));
    auto [row_split, dt] = input();
    ll n=dt.size();
    ld x_square = 0, x_sum = 0, y_sum = 0, xy_sum = 0;
    for(auto k : dt){
        ld x,y;
        tie(x,y) = {k[0], k[1]};
        x_square += x*x;
        x_sum += x;
        y_sum += y;
        xy_sum += x*y;
    }
    ld a = (n*xy_sum - x_sum*y_sum)/(n*x_square - x_sum*x_sum);
    ld b = (x_square*y_sum - x_sum*xy_sum)/(n*x_square - x_sum*x_sum);
    cout<<fixed<<setprecision(4)<<b/a<<'\n';
}