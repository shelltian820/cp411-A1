
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <regex>

using namespace std;

//function prototypes
void read_lines(ifstream file);

//globals
string obj_name;
double *temp;
int v_count, f_count;
double v_array[] = malloc(sizeof(double));
double f_array[] = malloc(sizeof(double));

int main(int argc, char *argv[]){
  if (argc != 2){
    cout << "Invalid. Please input one file.\n";
    exit(1); //terminate with error
  }
  //cout << "opening file " << argv[1] << endl;
  ifstream infile;
  //open file
  infile.open(argv[1]);
  if (!infile) {
        cout << "Unable to open file\n";
        exit(2); //terminate with error
  }else{
    //read data into arrays & variables
    read_lines(infile);
  }

  //ask for user command repeatedly
  string command;
  while (command != "q"){
    cin >> command;
    cout << "command: " << command << endl;

  }
  //end program
  infile.close();
  cout << "Exiting...\n";
  return 0;
}


void read_lines(ifstream file){
  string line;
  regex re("(-)?\\d+((\\.)?\\d+)?");
  while ( getline (file,line) )
  {
    if (line[0]=='v'){
      //add to vertex array
      cout << line;
    }
    else if(line[0]=='f'){
      //add to face array
      cout << line;
    }
    else if(line[0]=='o'){
      //store object name
      cout << line;
    }

  }
}
void write_to_file(){
  ofstream outfile;
  outfile.open("out.obj");
}
