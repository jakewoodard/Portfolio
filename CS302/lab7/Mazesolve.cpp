#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

struct cell { 
  cell() { x=0; y=0; }
  cell(int i, int j) { x=i; y=j; }
  int x; // column
  int y; // row
};

struct stack {
  stack(int x) { top = -1; arr = (cell *)calloc(x, sizeof(cell)); }
  ~stack() { delete [] arr; }
  int top; // top element
  cell * arr; // container
};

// stack functions 
void push (stack &s, cell c) {
  s.arr[++s.top] = c;
}
 
cell pop (stack &s) {
  cell c;
  c = s.arr[s.top--];
  return c;
}

cell peek (stack &s) {
  cell c;
  c = s.arr[s.top];
  return c;
}

// get direction
int get_dir(int x, int y, int xx, int yy) {
  if (x == xx) {
    if (y < yy) return 0;
    if (y > yy) return 2;
  } 
  else {
    if (x < xx) return 1;
    if (x > xx) return 3;
  } 
  return -1;
}

// find path solution via recursion
bool solve(stack &p, int **v, int ***wall, cell s, cell t) {
  // current position is 's' & destination is 't'
  printf("source %d %d \n", s.x, s.y);
  printf("sink   %d %d \n", t.x, t.y);
  v[s.y][s.x] = 1;
  push(p, s);
   
  if (s.x == t.x && s.y == t.y) { return true; } // sink found
 
  // for each direction at source
  for (int d = 0; d < 4; d++) {  
    if (wall[s.y][s.x][d] == 1) continue;
    
    // find sink.x sink.y
    if (d == 0) { t.x = s.x; t.y = s.y + 1; }
    if (d == 1) { t.x = s.x + 1; t.y = s.y; }
    if (d == 2) { t.x = s.x; t.y = s.y - 1; }
    if (d == 3) { t.x = s.x - 1; t.y = s.y; }

    if (v[t.y][t.x] == 1) continue; // if: been visited before
    if (solve(p, v, wall, s, t) == true) return true;
  }
  v[s.y][s.x] = 0;
  pop(p);
  return false;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    // print usage error message to stderr
    printf("%s \n", "Usage: Mazesolve maze.txt path.txt");
    return 0;
  } 

  // open input file for reading
  char * fname1 = (char *) malloc(64);
  int len1 = strlen(argv[1]);
  strncpy(fname1, argv[1], len1);
  FILE * pFile1;
  pFile1 = fopen(fname1, "w");
  if (pFile1 == NULL) {
    printf("%s \n", "Error opening input file");
  }

  // open output file for writing
  char * fname2 = (char *) malloc(64);
  int len2 = strlen(argv[2]);
  strncpy(fname2, argv[2], len2);
  FILE * pFile2;
  pFile2 = fopen(fname2, "w");
  if (pFile2 == NULL) {
    printf("%s \n", "Error opening output file");
  }

  // LOL lets make these global
  int Nrow;
  int Ncol;
  
  // determine Nrow,Ncol from input file
  fscanf(pFile1, "MAZE %d %d", &Nrow, &Ncol);

  // total # cells
  int NN;
  NN = Nrow * Ncol;

  // create array of walls for each grid cell
  //int wall[Nrow][Ncol][4];
  int ***wall;
  wall = new int ** [Nrow];
  for (int w = 0; w < Nrow; w++) {
    wall[w] = new int * [Ncol];
    for (int y = 0; y < Ncol; y++) {
      wall[w][y]= new int [4];
    }
  }
  
  // No wall = 0
  // Wall present = 1
  
  // initialize to have interior walls unset
  for (int j = 0; j < Nrow; j++) {
    for (int i = 0; i < Ncol; i++) {
      wall[j][i][0] = 0;                  // North
      wall[j][i][1] = 0;                  // East
      wall[j][i][2] = 0;                  // South
      wall[j][i][3] = 0;                  // West
      // set boundary walls
      if (i == 0) wall[j][i][3] = 1;      // West
      if (j == 0) wall[j][i][0] = 1;      // North
      if (i == Ncol-1) wall[j][i][1] = 1; // East
      if (j == Nrow-1) wall[j][i][2] = 1; // South
    }
  }

  int x1, x2, y1, y2;
  
  // read input file, set interior walls
  while (fscanf(pFile2, "%d %d %d %d", &y1, &x1, &y2, &x2) == 4) {
    wall[x1][y1][get_dir(x1, y1, x2, y2)] = 1;
    wall[x2][y2][get_dir(x2, y2, x1, y1)] = 1;
  }

  // initalize DFS path computation
  // stack "P" for path
  int kk = NN*5;
  stack P(kk);

  // source = (0,0)
  cell S;
  S.y = 0; S.x = 0;

  // sink = (Nrow-1,Ncol-1)
  cell T;
  int ff = Ncol-1; int gg = Nrow-1;
  T.x = ff; T.y = gg;
  printf("hey its still segfaulting dumbass\n");
  // bool iswhiite[][] (aka visited cells)
  int **V;
  V = new int * [Nrow];
  for (int w = 0; w < Nrow; w++) {
    V[w] = new int [Ncol];
  }
  for (int u = 0; u < Ncol; u++) { 
    for (int h = 0; h < Nrow; h++) { 
      V[u][h] = 0;
    }
  }
  
  // cell link[][]

  printf("hey its still segfaulting dumbass\n");
  // carry out DFS source-to-sink computation
  bool A = solve(P, V, wall, S, T);
  // if no solution
  if (A == false) {
    printf("%s \n", "No solution for maze");
    fclose(pFile1);
    fclose(pFile2);
    return 0;
  } else {
  // write PATH, Nrow, Ncol header
    fprintf(pFile2, "%s %d %d \n", "PATH", Nrow, Ncol); 
  // write cells on path to file
    int b = P.top+1;
    for (int q = 0; q < b; q++) {
      fprintf(pFile2, "%d %d \n", P.arr[q].y, P.arr[q].x);
    }
  }

  // close open files
  // free memory
  for (int i=0; i<Nrow; i++) {
    for (int j=0; j<Ncol; j++) {
      delete [] wall[i][j];
    }
    delete [] wall[i];
    delete [] V[i];
  }
  delete [] wall;
  delete [] V;

  fclose(pFile1);
  fclose(pFile2);
  return 0;
}
