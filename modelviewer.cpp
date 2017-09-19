
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
int num_vertices = 0;
int num_faces = 0;


////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
//vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int main(int argc, char *argv[]){
  if (argc != 2){
    cout << "Invalid. Please input one file.\n";
    exit(1); //terminate with error
  }
  string input = argv[1];
  string obj = ".obj";
  string input_end = input.substr(input.size()-4);
  if ( input_end.compare(obj) ){
    cout << "Invalid. Please input \".obj\" file.\n";
    exit(1);
  }

  //read file
  vector<double> v_vector;
  vector<int> f_vector;
  read(input, v_vector, f_vector);

  //ask for user commands repeatedly until they quit
  cout << "\nWelcome to Model Viewer. Please type in one of the following commands.";
  string command, q="q",w="w";
  while (command.compare(q)!= 0){
    //cout << "---------------------------------------------------------------";
    cout << "\nCommands:\n-  Quit(q)\n-  Write to file(w)\nEnter command:";
    cin >> command;
    //w = write to file
    if (command.compare(w)==0){
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

/*calculate average of all points, find vector of translation from average
point to origin, translate all vertices with the vector of translation*/
void center(){
  //calculate average x
  //calculate average y
  //calculate average z
  //calculate translation vector from (x,y,z) to (0,0,0)
  //apply to all points
}

/**/
void scale(){
  //find min x, max x, min y, max y
}


/*read obj file and adds data to a v list and f list (vector lists)*/
void read(string filename, vector<double> &v, vector<int> &f){
  ifstream infile;
  infile.open(filename);
  if (!infile) {
        cout << "Unable to open file\n";
        exit(2); //terminate with error
  }else{
    string line;
    //regex re("(-)?\\d+((\\.)?\\d+)?");
    while ( getline (infile,line) ){
      if (line[0]=='v'){
        //add to vertex array
        //cout << line << endl;
        num_vertices++;
        vector<double> vertices;
        vertices = split_v(line);
        vector <double> :: iterator i;
        for (i = vertices.begin(); i != vertices.end(); ++i)
          v.push_back(*i);
      }
      if(line[0]=='f'){
        //add to face array
        //cout << line << endl;
        num_faces++;
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

/*write v list and f list info to new file called "out.obj "*/
void write(vector<double> &v, vector<int> &f){
  ofstream outfile;
  outfile.open ("out.obj");
  //outfile << "Writing this to a file.\n";
  //write v lines
  outfile << std::fixed;
  outfile << std::setprecision(6);
  for(int m=0; m<num_vertices; m++){
    outfile << "v ";
    for(int i=0; i<3; i++){
      double d = v[3*m+i];
      outfile << d << " ";
    }
    outfile << endl;
  }

  //write f lines
  outfile << std::setprecision(0);
  vector <int> :: iterator f_it;
  for(int n=0; n<num_faces; n++){
    outfile << "f ";
    for(int i=0; i<face_sides; i++){
      int in = f[face_sides*n+i];
      outfile << in << " ";
    }
    outfile << endl;
  }

  //close file
  outfile.close();
  cout << "\n Data written to: out.obj \n";
}

/*split string for v lines and returns a vector*/
cvector<double> split_v(string str){
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

/*split string for f lines and returns a vector.
Helper function for read*/
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
