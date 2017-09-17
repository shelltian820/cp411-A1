
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <regex>
#include <vector>
#include <iomanip>

using namespace std;

//function prototypes
void read(string filename, vector<double> &v, vector<int> &f);
vector<double> split_v(string str);
vector<int> split_f(string str);
void write(vector<double> &v, vector<int> &f);

//globals
int face_sides = 0;


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
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
    if (command.compare(w)==0){
      vector<double> v_vector;
      vector<int> f_vector;
      read(argv[1], v_vector, f_vector);
      // vector <double> :: iterator i;
      // vector <int> :: iterator j;
      //print result
      // cout << "v_vector: \n";
      // for (i = v_vector.begin(); i != v_vector.end(); ++i)
      //   cout << *i << " ";
      // cout << endl;
      // cout << "f_vector: \n";
      // for (j = f_vector.begin(); j != f_vector.end(); ++j)
      //   cout << *j << " ";
      // cout << endl;
      write(v_vector, f_vector);
    }

    cout << "---------------------------------------------------------------\n";

  }
  //end program
  cout << "Goodbye!\n";
  return 0;
}
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
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
        //cout << line << endl;
        vector<double> vertices;
        vertices = split_v(line);
        vector <double> :: iterator i;
        for (i = vertices.begin(); i != vertices.end(); ++i)
          v.push_back(*i);
      }
      if(line[0]=='f'){
        //add to face array
        //cout << line << endl;
        vector<int> faces;
        faces = split_f(line);
        vector <int> :: iterator j;
        if (!face_sides) face_sides = faces.size();
        for (j = faces.begin(); j != faces.end(); ++j)
          f.push_back(*j);
      }

    }
  }
  infile.close();
}

void write(vector<double> &v, vector<int> &f){
  ofstream outfile;
  outfile.open ("out.obj");
  //outfile << "Writing this to a file.\n";
  int m = 0, n = 0;
  //write v lines
  outfile << std::fixed;
  outfile << std::setprecision(6);
  while (m < v.size()/3){
    outfile << "v ";
    for(int i=0; i<3; i++){
      double d = v[3*m+i];
      outfile << d << " ";
    }
    m++;
    outfile << endl;
  }

  //write f lines
  outfile << std::setprecision(0);
  vector <int> :: iterator f_it;
  while (n < f.size()/3){
    outfile << "f ";
    for(int i=0; i<face_sides; i++){
      int in = f[face_sides*n+i];
      outfile << in << " ";
    }
    n++;
    outfile << endl;
  }

  //close file
  outfile.close();
  cout << "\n Data written to: out.obj \n";
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
