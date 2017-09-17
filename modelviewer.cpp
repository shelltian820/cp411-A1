
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <regex>
#include <vector>

using namespace std;

//function prototypes
void read(string filename, vector<double> &v, vector<int> &f);
vector<double> split_v(string str);
vector<int> split_f(string str);

//globals
string obj_name;


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]){
  if (argc != 2){
    cout << "Invalid. Please input one file.\n";
    exit(1); //terminate with error
  }


  //ask for user command repeatedly
  string command, q="q",w="w";

  while (command.compare(q)!= 0){
    cout << "\nCommands: Quit(q)\nEnter command:";
    cin >> command;
    cout << "you entered: " << command << endl;
    if (command.compare(w)==0){
      vector<double> v_vector;
      vector<int> f_vector;
      read(argv[1], v_vector, f_vector);
      vector <double> :: iterator i;
      vector <int> :: iterator j;
      //print result
      cout << "v_vector: \n";
      for (i = v_vector.begin(); i != v_vector.end(); ++i)
        cout << *i << " ";
      cout << endl;
      cout << "f_vector: \n";
      for (j = f_vector.begin(); j != f_vector.end(); ++j)
        cout << *j << " ";
      cout << endl;
    }
  }
  //end program
  cout << "Goodbye!\n";
  return 0;
}
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void read(string filename, vector<double> &v, vector<int> &f){
  ifstream infile;
  infile.open(filename);
  if (!infile) {
        cout << "Unable to open file\n";
        exit(2); //terminate with error
  }else{
    string line;
    regex re("(-)?\\d+((\\.)?\\d+)?");
    while ( getline (infile,line) ){
      if (line[0]=='v'){
        //add to vertex array
        cout << line << endl;
        vector<double> vertices;
        vertices = split_v(line);
        vector <double> :: iterator i;
        for (i = vertices.begin(); i != vertices.end(); ++i)
          v.push_back(*i);
      }
      if(line[0]=='f'){
        //add to face array
        cout << line << endl;
        vector<int> faces;
        faces = split_f(line);
        vector <int> :: iterator j;
        for (j = faces.begin(); j != faces.end(); ++j)
          f.push_back(*j);

      }
    }
  }
  infile.close();
}
void write_to_file(){
  ofstream outfile;
  outfile.open("out.obj");
}

vector<double> split_v(string str){
  regex re("(-)?\\d+((\\.)?\\d+)?");
  sregex_iterator begin(str.begin(), str.end(), re);
  sregex_iterator end;
  vector<double> output;
  int n =0;
  for (std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        //convert string to double
        std::string::size_type sz;
        double d = std::stod (match_str,&sz);
        //add to output vector
        output.push_back(d);
        n++;
  }
  return output;
}

vector<int> split_f(string str){
  regex re("\\d+"); //look for integers
  sregex_iterator begin(str.begin(), str.end(), re);
  sregex_iterator end;
  vector<int> output;
  int n =0;
  for (std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        //convert string to int
        std::string::size_type sz;
        double k = std::stoi (match_str,&sz);
        //add to output vector
        output.push_back(k);
        n++;
  }
  return output;
}
