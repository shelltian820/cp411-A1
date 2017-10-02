using namespace std;

void read(string filename, vector<vector<float>> &v, vector<vector<int>> &f);
vector<float> split_v(string str);
vector<int> split_f(string str);
void write(vector<vector<float>> v, vector<vector<int>> f);

/*read obj file and adds data to a v list and f list (vector lists)*/
void read(string filename, vector<vector<float>> &v, vector<vector<int>> &f){
  ifstream infile;
  infile.open(filename);
  if (!infile) {
        cout << "Unable to open file\n";
        exit(2); //terminate with error
  }else{
    string line;
    while ( getline (infile,line) ){
      if (line[0]=='v'){
        v.push_back(split_v(line));
      }
      if(line[0]=='f'){
        vector<int> output = split_f(line);
        int i = 1, j = 2;
        //split shape into triangles
        while(j < output.size()){
          vector<int> triangle;
          triangle.assign({output[0], output[i], output[j]});
          i++; j++;
          f.push_back(triangle);
        }
      }
    }
  }
  infile.close();
}


/*split string for v lines and returns a vector*/
vector<float> split_v(string str){
  regex re("(-)?\\d+((\\.)?\\d+)?");
  sregex_iterator begin(str.begin(), str.end(), re);
  sregex_iterator end;
  vector<float> output;
  int n =0;
  for (std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        std::string match_str = match.str();
        //convert string to float
        std::string::size_type sz;
        float d = stof(match_str, &sz);
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
        int k = stoi (match_str,&sz);
        //add to output vector
        output.push_back(k);
        n++;
  }
  return output;
}

/*write v list and f list info to new file called "out.obj "*/
void write(vector<vector<float>> v, vector<vector<int>> f){
  ofstream outfile;
  outfile.open ("out.obj");
  //outfile << "Writing this to a file.\n";
  //write v lines
  outfile << std::fixed;
  outfile << std::setprecision(6);
  for(int m=0; m<v.size(); m++){
    outfile << "v ";
    vector<float> vertex = v[m];
    outfile << vertex[0] << " "
        << vertex[1] << " "
        << vertex[2] << endl;
  }

  //write f lines
  vector <int> :: iterator it;
  for(int n=0; n<f.size(); n++){
    outfile << "f ";
    vector<int> face = f[n];
    for (it = face.begin(); it != face.end(); ++it)
      outfile << *it << " ";
    outfile << endl;
  }

  //close file
  outfile.close();
  cout << "\n Data written to: out.obj \n";
}
