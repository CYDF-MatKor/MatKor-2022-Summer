#include <iostream>
#include <string>
#include <vector>
#include <iomanip>


using namespace std;
typedef long long ll;
typedef long long LL;
typedef vector<int> vi;
typedef vector<ll> vl;
#define pb(x) push_back(x)
#define all(x) (x).begin(), (x).end()
ll gcd(ll a, ll b){return b?gcd(b,a%b):a;}
ll lcm(ll a, ll b){if(a&&b)return a*(b/gcd(a,b)); return a+b;}
ll POW(ll a, ll b, ll rem){ll p=1;for(;b;b/=2,a=(a*a)% rem)if(b&1)p=(p*a)%rem;return p;}

long double E(vector<long double> x, ll n)
{
    long double sum = 0;
    for(ll i = 0; i < x.size(); i++) {
        sum += x[i];
    }

    return sum/n;
}

long double COV(vector<long double> x, vector<long double> y, ll n)
{
    // E(xy) - E(x) - E(y)

    long double sum = 0;
    for(ll i = 0; i < x.size(); i++) {
        sum += x[i] * y[i];
    }

    sum /= n;
    sum -= E(x, n) * E(y, n);
    return sum;
}  

long double V(vector<long double> x, ll n)
{
    // E(x^2) - {E(x)}^2
    long double sum = 0;
    for(ll i = 0 ; i < x.size(); i++) {
        sum += x[i] * x[i];
    }

    sum /= n;
    sum -= E(x, n) * E(x, n);
    return sum;
}

int main(void)
{
 
    vector<long double> t;
    vector<long double> v;
    long double x, y;
    char c;
    string trash_str;
    freopen("01_single_linear_regression.csv", "r", stdin);
    getline(cin, trash_str);

    while(cin >> x >> c >> y) {
        t.push_back(x);
        v.push_back(y);
    }

    ll n = t.size();

    long double res1 = COV(t, v, n) / V(t, n);
    long double res2 = E(v, n) - E(t, n) * COV(t, v, n) / V(t, n);
    cout<<fixed<<setprecision(4)<<res2 / res1<<'\n';
}