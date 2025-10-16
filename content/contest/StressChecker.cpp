#include "bits/stdc++.h"
using namespace std;

vector<string> readAns(ifstream &fin) {
  vector<string> v;
  string r;
  while(getline(fin, r)) v.push_back(r);
  return v;
}

int32_t main(int32_t argc, char* argv[]) {
  ifstream inf("input.txt", ifstream::in);
  ifstream ouf("output.txt", ifstream::in);
  ifstream ans("answer.txt", ifstream::in);
  
  vector<string> myAnswer = readAns(ouf);
  vector<string> correctAnswer = readAns(ans);
  if (myAnswer != correctAnswer) return -1;
  return 0;
}