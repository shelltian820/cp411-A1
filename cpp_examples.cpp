
// c++ examples of common problems
#include <iostream>
#include <string>
#include <regex>
#include <stdio.h>
#include <vector>
using namespace std;

void compare_strings(){
  string a = "q"; //must define a and b as strings first
  string b = "p";
  int val = a.compare(b);
  cout << val << endl;
}

// how to use regex
void regex_num(){
  string str = "v 99874525 -53 0.111 913.0 -4.123456";
  cout << str << endl;
  regex re("(-)?\\d+((\\.)?\\d+)?");
  sregex_iterator begin(str.begin(), str.end(), re);
  sregex_iterator end;
  //auto begin = std::sregex_iterator(str.begin(), str.end(), re);
  //auto end = std::sregex_iterator();
  int n =0;
  for (std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        cout << match_str << "  ";
  }
}

//stod
void strtod ()
{
  string orbits ("365.24 29.53 5");
  std::string::size_type sz;     // alias of size_t

  double earth = std::stod (orbits,&sz);
  double moon = std::stod (orbits.substr(sz)); //????
  cout << earth << endl;
  cout << moon << endl;

  string bob("-12.000300");
  std::string::size_type sz2;
  double bobby = std::stod (bob,&sz2);
  cout << bobby << endl;
}

//how to use vectors
void vectors(){
  vector <int> g1;
  vector <int> :: iterator i;

  //populating vector
  for (int i = 1; i <= 5; i++)
      g1.push_back(i);

  //iterate through vector
  cout << "Output of begin and end\t:\t";
  for (i = g1.begin(); i != g1.end(); ++i)
      cout << *i << '\t';
  cout << endl;

  //accessing new info
  cout << "Size : " << g1.size();
  cout << "\nCapacity : " << g1.capacity();
  cout << "Reference operator [g] : g1[2] = " << g1[2];
  cout << endl;
  cout << "at : g1.at(4) = " << g1.at(4);
  cout << endl;
  cout << "front() : g1.front() = " << g1.front();
  cout << endl;
  cout << "back() : g1.back() = " << g1.back();
  cout << endl;
}

void pass_vector_args(){
  std::vector<int> v;
    std::vector<int> u;
    u.assign ({1,1,1,1,1,1,1,1,1,1});
    f(v);
    g(u);

    for ( int x : v ) std::cout << x << ' ';
    std::cout << std::endl;

    for ( int x : u ) std::cout << x << ' ';
    std::cout << std::endl;
}
void f( std::vector<int> &v )
{
    v.assign( { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 } );
    for ( int x : v ) std::cout << x << ' ';
    std::cout << std::endl;
}
void g( std::vector<int> v )
{
    v.assign( { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 } );
    for ( int x : v ) std::cout << x << ' ';
    std::cout << std::endl;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

int main()
{
  //regex_num();
  //strtod();
  //vectors();
  //pass_vector_args();
}
