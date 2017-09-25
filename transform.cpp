using namespace std;

/*calculate average of all points, find vector of translation from average
point to origin, translate all vertices with the vector of translation*/
void center(vector<vector<float>> &v){ //if not &v, works with copy of v
  int num_vertices = v.size();

  //calculate average x
  float total_x = 0.0;
  for(int i = 0; i < num_vertices; i++){
    float x_coord = v[i][0];
    total_x += x_coord;
  }
  float avg_x = total_x/num_vertices;

  //calculate average y
  float total_y = 0.0;
  for(int i = 0; i < num_vertices; i++){
    float y_coord = v[i][1];
    total_y += y_coord;
  }
  float avg_y = total_y/num_vertices;

  //calculate average z
  float total_z = 0.0;
  for(int i = 0; i < num_vertices; i++){
    float z_coord = v[i][2];
    total_z += z_coord;
  }
  float avg_z = total_z/num_vertices;

  //translate vertices to origin
  float change_x = 0.0 - avg_x;
  float change_y = 0.0 - avg_y;
  float change_z = 0.0 - avg_z;
  for(int i = 0; i < num_vertices; i++){
    v[i][0] = change_x + v[i][0]; //translate x
    v[i][1] = change_y + v[i][1]; //y
    v[i][2] = change_z + v[i][2]; //z
  }
  //cout << avg_x << " " << avg_y << " " << avg_z << endl;
}


void scale(vector<vector<float>> &v){
  int num_vertices = v.size();

 float min_x = 9999.0, min_y = 9999.0, min_z = 9999.0,
  max_x = -9999.0, max_y = -9999.0, max_z = -9999.0;

 for(int i=0; i < num_vertices; i++){
   if (v[i][0] < min_x) min_x = v[i][0];
   if (v[i][0] > max_x) max_x = v[i][0];
   if (v[i][1] < min_y) min_y = v[i][1];
   if (v[i][1] > max_y) max_y = v[i][1];
   if (v[i][2] < min_z) min_z = v[i][2];
   if (v[i][2] > max_z) max_z = v[i][2];
 }

  //calculate scale
  float scale = max_x - min_x;
  if ((max_y-min_y) > scale) scale = max_y-min_y;
  if ((max_z-min_z) > scale) scale = max_z-min_z;

  //cout << scale << endl;

  //apply to scale
  for(int i = 0; i < num_vertices; i++){
    v[i][0] = (v[i][0] / scale) * 1.25;
    v[i][1] = (v[i][1] / scale) * 1.25;
    v[i][2] = ((v[i][2] / scale) * 1.25) ; 

  }


}
