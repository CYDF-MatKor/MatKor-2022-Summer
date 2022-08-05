#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef long double ld;

pair<vector<string>, vector<vector<ld>>> input(){
    ifstream input_file("../example data/02_multiple_linear_regression.csv");
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
template<typename T>class Matrix
{
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
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    setlocale(LC_ALL, "ko_KR.UTF-8");

    // cin.imbue(locale("ko_KR.UTF-8"));
    auto [row_split, dt] = input();
    ll n=dt.size();
    Matrix<long double>A(5,5),Y(5,1);
    ld ans=0;
    for(auto k : dt){
        ld y;
        vector<ld>x(5);
        tie(x[0],x[1],x[2],x[3],y)={k[0],k[1],k[2],k[3],k[4]};
        x[4]=10;
        y=log10(y);
        for(int i=0;i<5;i++){
            x[i]=log10(x[i]);
        }
        for(int i=0;i<5;i++){
            for(int j=0;j<5;j++){
                A.elements[i][j]+=x[i]*x[j];
            }
            Y.elements[i][0]+=x[i]*y;
        }
    }
    auto [success,ret]=A.inverse();
    if(!success){
        cout<<"No Solution"<<endl;
        return 0;
    }
    Matrix<long double>X=ret*Y;
    cout<<"X : ";
    ll H=round(X.elements[0][0]*2),
        O=round(X.elements[1][0]*2),
        C=round(X.elements[2][0]),
        N=round(X.elements[3][0]*2);
    ld k = pow((ld)10,X.elements[4][0]);
    if(C){
        cout<<"C"<<C<<' ';
    }
    if(H){
        cout<<"H"<<H<<' ';
    }
    if(O){
        cout<<"O"<<O<<' ';
    }
    if(N){
        cout<<"N"<<N<<' ';
    }
    cout<<", ";
    cout<<scientific<<setprecision(3)<<"k: "<<k<<'\n';
}