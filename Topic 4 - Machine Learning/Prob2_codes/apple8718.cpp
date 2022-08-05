#include <bits/stdc++.h>

#define ll long long int
#define ull unsigned long long int
#define lll __int128_t
#define INF (int)1e9
#define INFLL (long long)1e18
#define pii pair<int, int>
#define pll pair<long long int, long long int>
#define MOD (INF + 7)

using namespace std;

void init();

typedef long double ld;

pair<vector<string>, vector<vector<ld>>> input(){
    ifstream input_file("02_multiple_linear_regression.csv");
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

template<typename T>class Matrix;
template<typename T>Matrix<T>eyes(size_t);
template<typename T>class Matrix {
public:
    Matrix(size_t n, size_t m):n(n),m(m){
        elements.resize(n,vector<T>(m));
    }
    Matrix(){
        Matrix(0,0);
    }
    ~Matrix(){
        elements.clear();
    }
    template<typename T1>void operator=(const Matrix<T1>&r){
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                this->elements[i][j]=r.elements[i][j];
            }
        }
    }
    Matrix<T> operator+(const Matrix<T>&r)const{
        Matrix<T> ret(n,m);
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                ret.elements[i][j]=elements[i][j]+r.elements[i][j];
            }
        }
        return ret;
    }

    Matrix<T> operator*(const Matrix<T>&r)const{
        Matrix<T> ret(n,r.m);
        for(int i=0;i<n;i++){
            for(int j=0;j<r.m;j++){
                for(int k=0;k<m;k++){
                    ret.elements[i][j]+=elements[i][k]*r.elements[k][j];
                }
            }
        }
        return ret;
    }

    template<typename T1>Matrix<T> operator%(const T1&r)const{
        Matrix<T> ret(n,m);
        for(int i=0;i<n;i++){
            for(int j=0;j<m;j++){
                ret.elements[i][j]=elements[i][j]%r;
            }
        }
        return ret;
    }

    Matrix<T> operator^(const ll&r)const{
        if(!r){
            return eyes<T>(n);
        }
        else{
            Matrix<T> half=(*this)^(r>>1);
            if(r&1){
                return half*half*(*this);
            }
            else return half*half;
        }
    }
    pair<bool,Matrix<long double>>inverse()const{
        if(n!=m)return{false,Matrix<long double>()};
        Matrix<long double>org(n,n),ret(n,n);
        org=*this;
        ret.init();
        for(int i=0;i<n;i++){
            int nonz=n;
            for(int j=i;j<n;j++){
                if(org.elements[j][i]){
                    nonz=j;
                    break;
                }
            }
            if(nonz==n)return {false,Matrix<long double>()};
            swap(org.elements[i],org.elements[nonz]);
            swap(ret.elements[i],ret.elements[nonz]);
            long double div=org.elements[i][i];
            for(int j=0;j<n;j++){
                org.elements[i][j]/=div;
                ret.elements[i][j]/=div;
            }
            for(int j=i+1;j<n;j++){
                long double mul=org.elements[j][i];
                for(int k=0;k<n;k++){
                    ret.elements[j][k]-=mul*ret.elements[i][k];
                    org.elements[j][k]-=mul*org.elements[i][k];
                }
            }
        }
        for(int i=n-1;i>=0;i--){
            for(int j=i-1;j>=0;j--){
                long double mul=org.elements[j][i];
                for(int k=0;k<n;k++){
                    ret.elements[j][k]-=mul*ret.elements[i][k];
                    org.elements[j][k]-=mul*org.elements[i][k];
                }
            }
        }
        return {true,ret};
    }

    Matrix<T> POW(const ll&r,const ll&mod)const{
        if(!r){
            return eyes<T>(n);
        }
        else{
            Matrix<T> half=this->POW(r>>1,mod);
            if(r&1){
                return half*half*(*this)%mod;
            }
            else return half*half%mod;
        }
    }

    Matrix<T> operator+=(const Matrix<T>&r)const{
        return (*this)+r;
    }

    Matrix<T> operator*=(const Matrix<T>&r)const{
        return (*this)*r;
    }
    template<typename T1>Matrix<T> operator%=(const T1&r)const{
        return (*this)%r;
    }
    Matrix<T> operator^=(const ll&r)const{
        return (*this)^r;
    }
    void resize(size_t n, size_t m){
        this->n=n,this->m=m;
        elements.resize(n,vector<T>(m,0));
    }
    void init(){
        for(int i=0;i<min(n,m);i++){
            elements[i][i]=1;
        }
    }

    istream &input(istream&is){
        for(auto&k:elements)for(auto&kk:k)is>>kk;
        return is;
    }

    ostream &print(ostream&os)const{
        for(const auto k:elements){
            for(const auto kk:k){
                os<<kk<<' ';
            }
            os<<'\n';
        }
        return os;
    }
    size_t n,m;
    vector<vector<T>>elements;

protected:
private:
};

template<typename T>Matrix<T>eyes(size_t n){
    Matrix<T> ret(n,n);
    ret.init();
    return ret;
}

template<typename T>istream& operator>>(istream&is, Matrix<T>&M){
    return M.input(is);
}
template<typename T>ostream& operator<<(ostream&os, const Matrix<T>&M){
    return M.print(os);
}

int main(){
    init();

    auto [row_split, data] = input();
    Matrix<ld> A(5, 5), Y(5, 1), X;
    int i, j;

    for(auto a : data) {
        long double y;
        vector<long double> x(5);

        x[0] = log10(a[0]);     // H2
        x[1] = log10(a[1]);     // O2
        x[2] = log10(a[2]);     // C
        x[3] = log10(a[3]);     // N2
        x[4] = 1;                  // k
        y = log10(a[4]);        // Y

        for(i = 0; i < 5; i++) {
            for(j = 0; j < 5; j++) A.elements[i][j] += x[i] * x[j];

            Y.elements[i][0] += x[i] * y;
        }
    }

    auto [isPossible, inv] = A.inverse();
    if(!isPossible) {
        cout << "No Solution\n";
        return 0;
    }

    X = inv * Y;
    ll H, O, C, N;
    ld k;

    H = round(X.elements[0][0] * 2);
    O = round(X.elements[1][0] * 2);
    C = round(X.elements[2][0]);
    N = round(X.elements[3][0] * 2);
    k = pow((ld)10, X.elements[4][0]);

    cout << "C : " << C << '\n';
    cout << "H : " << H << '\n';
    cout << "O : " << O << '\n';
    cout << "N : " << N << '\n';
    cout << "k : " << scientific << setprecision(3) << k << '\n';

    return 0;
}

void init(void) {


#ifndef ONLINE_JUDGE
    freopen("input.txt", "rt", stdin);
    freopen("output.txt", "wt", stdout);
    #define debug if constexpr (debug) cout << "[DEBUG] "
    #define debugv(x) if constexpr (debug) cout << "[DEBUG] " << #x << " == " << (x) << '\n';
    #define debugc(c) if constexpr (!ndebug) { cout << "[DEBUG] "<< #c << ": "; for (const auto& elem : (c)) cout << elem << ", "; cout << '\n'; }
#endif
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
}