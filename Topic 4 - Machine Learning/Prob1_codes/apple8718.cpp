#include <bits/stdc++.h>
#include "./Matrix cpp header/matrix.h"

using namespace std;

int main() {

    int i, j, cnt = 0;
    long double TSum = 0, VTSum = 0, TVTSum = 0, TTSum = 0;
    long double ET, EVT, ETVT, ETT, VT, a, b, Cov;

    freopen("./01_single_linear_regression.csv","r",stdin);
    string str;

    getline(cin, str);
    long double x,y;
    char c;
    while(cin>>x>>c>>y) {
        TSum += x;
        VTSum += y;
        TVTSum += x * y;
        TTSum += x * x;
        cnt++;
    }

    ET = (double)TSum / cnt;
    EVT = (double)VTSum / cnt;
    ETVT = (double)TVTSum / cnt;
    ETT = (double)TTSum / cnt;
    VT = ETT - ET * ET;
    Cov = ETVT - ET * EVT;

    a = Cov / VT;
    b = EVT - ET * Cov / VT;

    cout << fixed << setprecision(4) << b / a << endl;
}