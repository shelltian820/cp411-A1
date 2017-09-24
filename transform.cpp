using namespace std;

/*calculate average of all points, find vector of translation from average
point to origin, translate all vertices with the vector of translation*/
void center(vector<vector<float>> v, vector<vector<int>> f){ //if not &v, works with copy of v
  int num_vertices = v.size();
  float n = float(num_vertices);

  //calculate average x
  float total_x = 0.0;
  for(int i = 0; i < num_vertices; i++){
    float x_coord = v[i][0];
    total_x += x_coord;
  }
  float avg_x = total_x/n;

  //calculate average y
  float total_y = 0.0;
  for(int i = 0; i < num_vertices; i++){
    float y_coord = v[i][1];
    total_y += y_coord;
  }
  float avg_y = total_y/n;

  //calculate average z
  float total_z = 0.0;
  for(int i = 0; i < num_vertices; i++){
    float z_coord = v[i][2];
    total_z += z_coord;
  }
  float avg_z = total_z/n;

  //print
  cout << fixed;
  cout << setprecision(6);
  cout << avg_x << " " << avg_y << " " << avg_z << endl;
}

// void translate(double change_x, double change_y, double change_z, vector<double> v){
//   for(int i=0; i<num_vertices, i++){
//     v[i*3] += change_x;
//     v[i*3+1] += change_y;
//     v[i*3+2] += change_z;
//   }
//   //return v ???
// }
/**/
// void scale(){
//   //find min x, max x, min y, max y
// }
