
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]){
  if (argc != 2){
    cout << "Invalid. Please input one file.\n";
    exit(1); //terminate with error
  }
  //cout << "opening file " << argv[1] << endl;
  ifstream infile;
  string line;
  infile.open(argv[1]);
  if (!infile) {
        cout << "Unable to open file\n";
        exit(2); //terminate with error
  }
  else{
    //initialize arrays & variables
    string obj_name;
    v_array = malloc(sizeof(string));
    //read data into arrays & variables
    read_lines(infile, line);
  }

  //ask for user command repeatedly
  string command;
  while (command != 'q'){
    cin >> command;

  }
  //end program
  infile.close();
  cout << "Exiting...\n";
  return 0;
}


void read_lines(ifstream, string, *vertex_array, *face_array){
  while ( getline (infile,line) )
  {
    if (line[0]=='v'){
      //add to vertex array

    }
    elif (line[0]=='f'){
      //add to face array
    }
    elif (line[0]=='o'){
      //store object name
    }

  }
}
void write_to_file(){
  ofstream outfile;
  outfile.open("out.obj");
}
