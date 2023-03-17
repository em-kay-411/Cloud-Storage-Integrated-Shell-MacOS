#include <cstdio>
#include <iostream>
#include <string>
#include<vector>
#include <cstdlib>
// #include <filesystem>

using namespace std;

bool lexicValid(vector<vector<char> > arr, int col, int &curr, int n, int m){
    if(curr == -1){
        for(int i=0; i<n-1; i++){
            if(arr[i][col] > arr[i+1][col]){
                return false;
            }
        }
    }
    else{
        int i = 0;
        while(i < n-1){
            int init = i;
            while(arr[i][curr] == arr[i+1][curr]){
                i++;
            }
            for(int j = init; j<i; j++){
                if(arr[j][col] > arr[j+1][col]){
                    return false;
                }
            }
            i++;
        }
    }
    

    return true;
}

int main() {
    int n, m;
    cin >> n >> m ;
    vector<vector<char> > arr;

    for(int i=0; i<n; i++){
        vector<char>temp;
        for(int j=0; j<m; j++){
            char a;
            cin>>a;
            temp.push_back(a);
        }
        arr.push_back(temp);
    }
    int count = 0;
    int curr = -1;
    for(int i=0; i<m; i++){
        if(!lexicValid(arr, i, curr, n, m)){
            
            count++;
        }
        else{
            curr = i;
        }
    }

    if(count == m){
        cout<<"-1"<<endl;
    }
    else{
        cout<<count<<endl;
    }
    
    return 0;
}