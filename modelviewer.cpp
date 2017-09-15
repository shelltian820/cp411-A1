
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>



int main(int argc, char *argv[]){
  if (argc != 2){
    std::cout << "Invalid. Please input 1 file only.\n";
    return 1;
  }
  std::string file = argv[1];
  std::cout << "file name:" << file << std::endl;


  return 0;
}
