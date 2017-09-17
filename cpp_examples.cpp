
// c++ examples program
#include <iostream>
#include <string>
#include <regex>

using namespace std;

void regex_num(string str);

int main()
{
  string str = "v 0 53 0.11 913.0 -4.234234";
  regex_num(str);
  string a = "q";
  string b = "p";
  int val = a.compare(b);
  cout << val << endl;
}


void regex_num(string str){
  regex re("(-)?\\d+((\\.)?\\d+)?");
  sregex_iterator begin(str.begin(), str.end(), re);
  sregex_iterator end;
  string *output;
  int size = 1;
  output = new string[size];
  //auto begin = std::sregex_iterator(str.begin(), str.end(), re);
  //auto end = std::sregex_iterator();
  int n = 0;
  for (std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        output[n] = match_str;
        cout << output[n] << endl;
        n++;
        size++;
  }
}
