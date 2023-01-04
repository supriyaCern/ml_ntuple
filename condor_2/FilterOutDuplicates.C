/**********************************************************************
 Created on : 27/05/2022
 Purpose    : Filter out the duplicate entries of an array of strings
 Author     : Indranil Das, Visiting Fellow
 Email      : indranil.das@cern.ch | indra.ehep@gmail.com
**********************************************************************/
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

int FilterOutDuplicates(const char *infile="fl.txt")
{  
  vector<string> flist;
  ifstream fin(infile);
  string s;
  while(getline(fin,s)){
    //cout << "\t" << s << endl;
    flist.push_back(s);
  }
  sort(flist.begin(), flist.end());
  auto res = unique(flist.begin(), flist.end());
  for(vector<string>::iterator it = flist.begin(); it < res; it++){
    cout << *it << endl;
  }
  return true;
}
