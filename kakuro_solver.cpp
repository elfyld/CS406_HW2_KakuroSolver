#include <iostream>
#include <string>

#include <fstream>
#include <sstream>
#include <vector>

#include <bits/stdc++.h>
#include <array>

using namespace std;

enum direction {d_down, d_right, none};

#define COORD std::pair<int, int>

//#define DEBUG

int iter = 0;

///Auxiliary functions

void display_arr(int* arr, int n){

  cout << "arr: ";

  for(int i = 0; i < n; i++){
    cout << arr[i] << " ";
  }

  cout << endl;
  
}

void print_coords(COORD start, COORD end){

  cout << "Start:" << start.first << "," << start.second << endl;
  cout << "End:" << end.first << "," << end.second << endl;
  
}

int find_length(COORD start, COORD end, direction dir){

  if(dir == d_down)
    return end.first - start.first;
  if(dir == d_right)
    return end.second - start.second;

  return -1;
}

void convert_sol(int** mat, int** &sol_mat, int m, int n){

  sol_mat = new int*[m]; //Rows
  for(int i = 0; i < m; i++){
    sol_mat[i] = new int[n]; //Cols
  }

  for(int i = 0; i < m; i++){
    for(int j = 0; j < m; j++){
      if(mat[i][j] == -2)
	sol_mat[i][j] = -2; //Empty value cell
      else
	sol_mat[i][j] = -1; //Hint or empty cell
    }
  }
}

void print_one_matrix(int** matrix, int m, int n){
  std::cout << "Matrix: " << std::endl;
  for(int i = 0; i < m; i++){ //rows
    for(int j = 0; j < n; j++){ //cols
      std::cout << matrix[i][j] << "\t";
    }
    std::cout << "\n";
  }
}

void sol_to_file(int** mat, int** sol_mat, int m, int n, string fname){

  string outfname = "visualize.kakuro";
  ofstream to_write(outfname);

  to_write << m << " " << n << "\n";

  for(int i = 0; i < m; i++){
    for(int j = 0; j < n; j++){
      if(mat[i][j] != -2)
	to_write << mat[i][j] << " ";
      else
	to_write << sol_mat[i][j] << " ";
    }
    to_write << "\n";
  }

  to_write.close();
}

void read_matrix(int** &matrix, std::ifstream &afile, int m, int n){

  matrix = new int*[m]; //rows

  for(int i = 0; i < m; i++){
    matrix[i] = new int[n]; //cols
  }

  int val;
  for(int i = 0; i < m; i++){
    for(int j = 0; j < n; j++){
      afile >> val;
      matrix[i][j] = val;
    }
  }
}

///Auxiliary functions

struct sum{
  COORD start;
  COORD end;

  int hint;
  int dir;
  int length;
  int* arr;

  void print_sum(){
    cout << "############################" << endl;
    cout << "Creating sum with: " << endl;
    print_coords(start, end);
    cout << "Hint: " << hint << endl;
    cout << "Direction: " << dir << endl;
    cout << "Length: " << length << endl;
    cout << "############################" << endl;
  }
  
  sum(COORD _start, COORD _end, int _hint, direction _dir):
    start(_start), end(_end), hint(_hint), dir(_dir)
  {
    length = find_length(_start, _end, _dir);
    arr = new int[length];
    #ifdef DEBUG
    cout << "############################" << endl;
    cout << "Creating sum with: " << endl;
    print_coords(start, end);
    cout << "Hint: " << hint << endl;
    cout << "Direction: " << dir << endl;
    cout << "Length: " << length << endl;
    cout << "############################" << endl;
    #endif
  }
  
  //~sum(){
  //delete arr;
  //}
};


COORD find_end(int** matrix, int m, int n, int i, int j, direction dir){ //0 down 1 right

  if(dir == d_right){
    for(int jj = j+1; jj < n; jj++){
      if(matrix[i][jj] != -2 || jj == n - 1){
	if(matrix[i][jj] == -2 && jj == n -1)
	  jj++;
	COORD END = COORD(i, jj);
	return END;
      }
    }
  }

  if(dir == d_down){
    for(int ii = i+1; ii < m; ii++){
      if(matrix[ii][j] != -2 || ii == m - 1){
	if(matrix[ii][j] == -2 && ii == m - 1)
	  ii++;
	COORD END = COORD(ii, j);
	return END;
      }
    }
  }
  
}


vector<sum> get_sums(int** matrix, int m, int n){

  vector<sum> sums;
  
  for(int i = 0; i < m; i++){
    for(int j = 0; j < n; j++){
      int val = matrix[i][j];
      if(val != -1 && val != -2){
	int hint = val;
	hint = hint / 10;

	if((hint%100) == 0){
	  hint = (int)(hint/100); //Position right
	  COORD START = COORD(i, j+1); 
	  COORD END = find_end(matrix, m, n, i, j, d_right);
	  sum _sum = sum(START, END, hint, d_right);
      _sum.print_sum(); 
	  sums.push_back(_sum);
	}

	else{
	  int div = (int)(hint/100);  //Positon down
	  int rem = (int)(hint%100);
   
	  if(div == 0 && rem != 0){
	    COORD START = COORD(i+1,j);
	    COORD END = find_end(matrix, m, n, i, j, d_down);
	    sum _sum = sum(START, END, rem, d_down);
	    sums.push_back(_sum);
	  }

	  if(div != 0 && rem != 0){
	    COORD START1 = COORD(i+1,j);
	    COORD START2 = COORD(i,j+1);
	    COORD END1 = find_end(matrix, m, n, i, j, d_down);
	    COORD END2 = find_end(matrix, m, n, i, j, d_right);
	    sum _sum1 = sum(START1, END1, rem, d_down);
	    sum _sum2 = sum(START2, END2, div, d_right);
	    sums.push_back(_sum1);
	    sums.push_back(_sum2);
	  }
	}
      }

      
    }
  }
  return sums;
}

pair<int, int> find_unassigned_location(int** sol_mat, int m, int n) {
  for (int row = 0; row < m; row++) {
    for (int col = 0; col < n; col++) {
      if (sol_mat[row][col] == -2) {
        return make_pair(row, col);
      }
    }
  }
  return make_pair(-1, -1);
}

bool is_safe(int** sol_mat, int row, int col, int num, const vector<sum>& sums, int m ,int n) {
  
  
  // Check if the number is valid according to the sum hints
  for (int j = 0; j < m; j++) {
    if (sol_mat[row][j] == num) {
      return false;
    }
  }
   for (int j = 0; j < n; j++) {
    if (sol_mat[j][col] == num) {
      return false;
    }
  }

  for (const sum& s : sums) {
    if ((s.dir == d_down && col == s.start.second && row >= s.start.first && row < s.end.first) ||
        (s.dir == d_right && row == s.start.first && col >= s.start.second && col < s.end.second)) {
      
      int sum_value = num;
      bool last_cell = true;
      if (s.dir == d_down) {
        for (int i = s.start.first; i < s.end.first; i++) {
          if (i != row && sol_mat[i][col] != -2) {
            sum_value += sol_mat[i][col];
          }
          if (sol_mat[i][col] == -2 && i != row) {
            last_cell = false;
          }
        }
      } else {
        for (int j = s.start.second; j < s.end.second; j++) {
          if (j != col && sol_mat[row][j] != -2) {
            sum_value += sol_mat[row][j];
          }
          if (sol_mat[row][j] == -2 && j != col) { //if I am not at the end of the column and there exist -2 I am not in last cell
            last_cell = false;
          }
        }
      }

      if (sum_value > s.hint || (last_cell && sum_value != s.hint)) {//if sum value satisifes sum constraint then the numbers are correct 
        return false;
      }
    }
  }

  return true;
}



// Rename validNumberCheck to solve_kakuro_helper
bool validNumberCheck(int** sol_mat, int m, int n, const vector<sum>& sums) {
  pair<int, int> unassigned_location = find_unassigned_location(sol_mat, m, n);
  if (unassigned_location.first == -1) {
    return true;
  }
  
  int row = unassigned_location.first;
  int col = unassigned_location.second;

  for (int num = 1; num <= 9; num++) {
    if (is_safe(sol_mat, row, col, num, sums,m,n) ) {
      sol_mat[row][col] = num;
       cout << "Trying number " << num << " at row " << row << " and col " << col << endl;

      if (validNumberCheck(sol_mat, m, n, sums)) {
        return true;
      }

      sol_mat[row][col] = -2;
    }
  }

  return false;
}




  
  


 




bool solution(int** mat, int** &sol_mat, vector<sum> sums, int m, int n){

  //TO DO: Write the solution
  //You can use any algorithm and data type
  //Write your solution to file in main function using sol_to_mat() after solving it
   return validNumberCheck(sol_mat, m, n, sums);
  
}

int main(int argc, char** argv){
  
   std::string filename(argv[1]);
  std::ifstream file;
  file.open(filename.c_str());
  int m, n;
  file >> m;
  file >> n;

  int** mat;
  read_matrix(mat, file, m, n);
  print_one_matrix(mat, m, n);
  
  int** sol_mat;
  convert_sol(mat, sol_mat, m, n);
  print_one_matrix(sol_mat, m, n);
  
  vector<sum> sums = get_sums(mat, m, n);

  cout << "Solution: " <<solution(mat, sol_mat, sums, m, n) << endl;

  print_one_matrix(sol_mat, m, n);
  sol_to_file(mat, sol_mat, m, n, "solution.kakuro");
  
  for(int i = 0; i < n; i++){
    delete mat[i];
    delete sol_mat[i];
  }

  delete mat;
  delete sol_mat;
  
  return 0;
}
