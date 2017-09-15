
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]){
  if (argc != 2){
    cout << "Invalid. Please input one file.\n";
    exit(1);
  }
  //cout << "opening file " << argv[1] << endl;
  ifstream infile;
  infile.open(argv[1]);
  if (!infile) {
        cout << "Unable to open file\n";
        exit(2); // terminate with error
  }

  //malloc data
  while (infile >> data) {

  }
  return 0;
}
