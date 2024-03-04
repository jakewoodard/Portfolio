#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <vector>

class dset {
  struct node {
    node() { rank=0, parent=-1; }
    int rank;
    int parent;
  };

  public:
    dset(int Nsets);
    int size() { return Nsets; }
    int add_set();
    int merge(int, int);
    int find(int);

  private:
    int Nsets;
    std::vector<node> S;
};

dset::dset(int X) {
  S.assign(X, node());
  Nsets = X;
}

int dset::add_set() {
  S.insert(S.end(), 1, node());
  Nsets += 1;
  return S.size()-1;
}

int dset::merge(int i, int j) {
  i = find(i);
  j = find(j);
  if (i != j) {
    node &Si = S[i];
    node &Sj = S[j];

    // merge (union) by rank
    if (Si.rank > Sj.rank)      Sj.parent = i;
    else if (Si.rank < Sj.rank) Si.parent = j;
    else { Sj.parent = i; Si.rank += 1; }

    Nsets -= 1;
  }
return find(i);
}

int dset::find(int i) {
  if (S[i].parent == -1)
    return i;

  // recursive path compression
  S[i].parent = find(S[i].parent);
  return S[i].parent;
}

struct cell {
  cell() { x=0; y=0; }
  cell(int, int);
  int x; // column
  int y; // row
};

cell::cell(int i, int j) {
  x = i; y = j;
}


void swap(cell &c1, cell &c2) {
  int tempx, tempy;
  tempx = c1.x;
  tempy = c1.y;
  c1.x = c2.x;
  c1.y = c2.y;
  c2.x = tempx;
  c2.y = tempy;
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    // print usage error message to stderr
    printf("%s \n", "Usage: Mazemake Nrows Ncols maze.txt");
    return 0;
  }

  srand(time(NULL));
  int Nrow = atoi(argv[1]);
  int Ncol = atoi(argv[2]);

  // Grid wall:     (i0,j0) | (i1,j1)
  // Horizontal wall: (i,j) | (i+1,j)
  // Vertical wall:   (i,j) | (i,j+1)

  // number of interior grid walls
  int N = ((Nrow - 1) * Ncol) +((Ncol - 1) * Nrow);

  cell wall[N][2];


  // populate wall[][] with interior grid walls
  int k = 0;
  
  for (int i = 0; i < Ncol; i++) {
    for (int j = 0; j < Nrow; j++) {
      // vertical walls
      if (i != Ncol - 1) {
        wall[k][0] = cell(i, j); 
        wall[k][1] = cell(i + 1, j);
        k++;
      }
      // horizontal walls
      if (j != Nrow - 1) {
        wall[k][0] = cell(i, j);
        wall[k][1] = cell(i, j+1);
        k++;
      }
    }
  }

  int Rnum = 0;
  // randomly perturb list order: swap based
  for (int p=N-1; p>0; --p) {
    Rnum = rand();
    swap(wall[p][0], wall[Rnum % (p+1)][0]);
    swap(wall[p][1], wall[Rnum % (p+1)][1]);
  }

  // open output file for writing (argv[3])
  char * fname = (char *) malloc(64);
  int len = strlen(argv[3]);
  strncpy(fname, argv[3], len);
  FILE * pFile;
  pFile = fopen(fname, "w");

  // error check file open
  if (pFile == NULL) {
    printf("%s \n", "Error opening file");
    return 0;
  }

  // write MAZE, Nrow, Ncol header
  fprintf(pFile, "%s %d %d \n", "MAZE", Nrow, Ncol);
  
  dset ds(Nrow*Ncol);
  int ii;
  int jj;
  for (k=0; k<N; k++) {
    ii = wall[k][0].x + (wall[k][0].y * Ncol);
    jj = wall[k][1].x + (wall[k][1].y * Ncol);

    // if: pair of cells given by wall[k][] are not
    //	connected (belong to different disjoint sets),
    //	merge them
    if (ds.find(ii) != ds.find(jj)) { 
      ds.merge(ii, jj);
    }

    // else: write wall location to file as two pairs 
    //	of cell indices: i0 j0 i1 j1
    else {
      fprintf(pFile, "%d %d %d %d \n", wall[k][0].y, wall[k][0].x, wall[k][1].y, wall[k][1].x);
    }
    // if: all cells belong to the same set, break
    if (ds.size() == 1) break;
  }
  k++;

  // write any remaining walls to file
  for (int r = k; r < N; r++) {
    fprintf(pFile, "%d %d %d %d \n", wall[r][0].y, wall[r][0].x, wall[r][1].y, wall[r][1].x);
  }
  // close output file
  fclose(pFile);
  return 0;
}
