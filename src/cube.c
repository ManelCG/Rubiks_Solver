#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <cube.h>

struct Cube{
  int n;
  int dim;
  int ***faces;
};

int is_solved(struct Cube *cube){
  for (int face = 0; face < 6; face++){
    for (int i = 0; i < cube->n; i++){
      for (int j = 0; j < cube->n; j++){
        if (cube->faces[face][i][j] != face){
          return 0; //Not solved
        }
      }
    }
  }
  return 1; //Solved
}

struct Cube *copy_cube(struct Cube *cube){
  struct Cube *new = init_cube(cube->n);
  new->dim = cube->dim;

  for (int face = 0; face < 6; face++){
    for (int i = 0; i < cube->n; i++){
      for (int j = 0; j < cube->n; j++){
        new->faces[face][i][j] = cube->faces[face][i][j];
      }
    }
  }

  return new;
}

void free_cube_mem(struct Cube *cube){
  for (int face = 0; face < 6; face++){
    for (int i = 0; i < cube->n; i++){
      free(cube->faces[face][i]);
    }

    free(cube->faces[face]);
  }
}

char *generate_scramble(int len){
  char move, lastmove='\0', *alg, *aux = malloc((len*2) + 1);
  int ptr = 0;

  for (int i = 0; i < len; i++){
    switch ((int) random() % 6){
      case 0:
        move = 'U';
        break;
      case 1:
        move = 'L';
        break;
      case 2:
        move = 'F';
        break;
      case 3:
        move = 'R';
        break;
      case 4:
        move = 'B';
        break;
      case 5:
        move = 'F';
        break;
    }

    if (lastmove == '\0' || move != lastmove){
      aux[ptr] = move;
      lastmove = move;
      ptr ++;
      switch ((int) random() % 3){
        case 0:
          break;
        case 1:
          aux[ptr] = '\'';
          ptr++;
          break;
        case 2:
          aux[ptr] = '2';
          ptr++;
          break;
      }
    } else {
      i--;
    }
  }

  alg = malloc(++ptr);
  for (int i = 0; i < ptr; i++){
    alg[i] = aux[i];
  }

  free(aux);

  return alg;
}

void perform_algorithm(struct Cube *cube, char *alg){
  int face, clockwise, pointer = 0;
  while (alg[pointer] != '\0'){
    switch (alg[pointer]){
      case 'r':
      case 'R':
        face = 3;
        break;
      case 'l':
      case 'L':
        face = 1;
        break;
      case 'u':
      case 'U':
        face = 0;
        break;
      case 'd':
      case 'D':
        face = 5;
        break;
      case 'f':
      case 'F':
        face = 2;
        break;
      case 'b':
      case 'B':
        face = 4;
        break;
      default:
        break;
    }

    if (alg[pointer+1] == '\''){
      pointer++;
      clockwise = 0;
    } else if (alg[pointer+1] == '2') {
      pointer++;
      clockwise = 1;
      turn_face(cube, face, clockwise);
    } else {
      clockwise = 1;
    }

    turn_face(cube, face, clockwise);
    pointer++;
  }
}

void turn_face(struct Cube *cube, int face, int clockwise){
  //Turn actual face
  int **new_face = malloc(cube->n * sizeof(char *));
  for (int i = 0; i < cube->n; i++){
    new_face[i] = malloc(cube->n);
  }

  if (clockwise == 1){
    for (int i = 0; i < cube->n; i++){
      for (int j = 0; j < cube->n; j++){
        new_face[j][cube->n - 1 - i] = cube->faces[face][i][j];
      }
    }
  } else {
    for (int i = 0; i < cube->n; i++){
      for (int j = 0; j < cube->n; j++){
        new_face[i][j] = cube->faces[face][j][cube->n - 1 - i];
      }
    }
  }
  free(cube->faces[face]);
  cube->faces[face] = new_face;

  //Turn adjacent faces
  int *aux  = malloc(cube->n);

  if (clockwise == 1){
    switch (face){
      case 0:
        //Save face 1
        //1<2<3<4
        for (int i = 0; i < cube->n; i++){
          aux[i] = cube->faces[1][0][i];
          cube->faces[1][0][i] = cube->faces[2][0][i];
          cube->faces[2][0][i] = cube->faces[3][0][i];
          cube->faces[3][0][i] = cube->faces[4][0][i];
          cube->faces[4][0][i] = aux[i];
        }
        break;

      case 1:
        //Save face 4 and 4<5<2<0 but 4 is inverted and oposite side
        //Aux > 0
        for (int i = 0; i < cube->n; i++){
          aux[i] = cube->faces[4][cube->n - i -1][cube->n-1];
          cube->faces[4][cube->n - i -1][cube->n-1] = cube->faces[5][i][0];
          cube->faces[5][i][0] = cube->faces[2][i][0];
          cube->faces[2][i][0] = cube->faces[0][i][0];
          cube->faces[0][i][0] = aux[i];
        }
        break;

      case 2:
        //Save right column of face 1
        //1<5<3<0
        for (int i = 0; i < cube->n; i++){
          aux[i] = cube->faces[1][i][cube->n -1];
          cube->faces[1][i][cube->n-1] = cube->faces[5][0][i];
          cube->faces[5][0][i] = cube->faces[3][cube->n - i - 1][0];
          cube->faces[3][cube->n-i-1][0] = cube->faces[0][cube->n-1][cube->n-i-1];
          cube->faces[0][cube->n-1][cube->n-i-1] = aux[i];
        }
        break;

      case 3:
        //Save face 0
        for (int i = 0; i < cube->n; i++){
          aux[i] = cube->faces[0][i][cube->n-1];
          cube->faces[0][i][cube->n-1] = cube->faces[2][i][cube->n-1];
          cube->faces[2][i][cube->n-1] = cube->faces[5][i][cube->n-1];
          cube->faces[5][i][cube->n-1] = cube->faces[4][cube->n-1-i][0];
          cube->faces[4][cube->n-1-i][0] = aux[i];
        }
        break;

      case 4:
        for (int i = 0; i < cube->n; i++){
          aux[i] = cube->faces[1][i][0];
          cube->faces[1][i][0] = cube->faces[0][0][cube->n - i - 1];
          cube->faces[0][0][cube->n - i - 1] = cube->faces[3][cube->n-i-1][cube->n-1];
          cube->faces[3][cube->n-i-1][cube->n-1] = cube->faces[5][cube->n-1][i];
          cube->faces[5][cube->n-1][i] = aux[i];
        }
        break;

      case 5:
        for (int i = 0; i < cube->n; i++){
          aux[i] = cube->faces[2][cube->n-1][i];
          cube->faces[2][cube->n-1][i] = cube->faces[1][cube->n-1][i];
          cube->faces[1][cube->n-1][i] = cube->faces[4][cube->n-1][i];
          cube->faces[4][cube->n-1][i] = cube->faces[3][cube->n-1][i];
          cube->faces[3][cube->n-1][i] = aux[i];
        }
        break;
      default:
        break;
    }
  } else {
    switch (face){
      case 0:
        for (int i = 0; i < cube->n; i++){
          aux[i] = cube->faces[1][0][i];
          cube->faces[1][0][i] = cube->faces[4][0][i];
          cube->faces[4][0][i] = cube->faces[3][0][i];
          cube->faces[3][0][i] = cube->faces[2][0][i];
          cube->faces[2][0][i] = aux[i];
        }
        break;

      case 1:
        for (int i = 0; i < cube->n; i++){
          aux[i] = cube->faces[4][cube->n - i -1][cube->n-1];
          cube->faces[4][cube->n - i -1][cube->n-1] = cube->faces[0][i][0];
          cube->faces[0][i][0] = cube->faces[2][i][0];
          cube->faces[2][i][0] = cube->faces[5][i][0];
          cube->faces[5][i][0] = aux[i];
        }
        break;

      case 2:
        for (int i = 0; i < cube->n; i++){
          aux[i] = cube->faces[1][i][cube->n -1];
          cube->faces[1][i][cube->n-1] = cube->faces[0][cube->n-1][cube->n-i-1];
          cube->faces[0][cube->n-1][cube->n-i-1] = cube->faces[3][cube->n-i-1][0];
          cube->faces[3][cube->n-i-1][0] = cube->faces[5][0][i];
          cube->faces[5][0][i] = aux[i];
        }
        break;

      case 3:
        for (int i = 0; i < cube->n; i++){
          aux[i] = cube->faces[0][i][cube->n-1];
          cube->faces[0][i][cube->n-1] = cube->faces[4][cube->n-1-i][0];
          cube->faces[4][cube->n-1-i][0] = cube->faces[5][i][cube->n-1];
          cube->faces[5][i][cube->n-1] = cube->faces[2][i][cube->n-1];
          cube->faces[2][i][cube->n-1] = aux[i];
        }
        break;

      case 4:
        for (int i = 0; i < cube->n; i++){
          aux[i] = cube->faces[1][i][0];
          cube->faces[1][i][0] = cube->faces[5][cube->n-1][i];
          cube->faces[5][cube->n-1][i] = cube->faces[3][cube->n-i-1][cube->n-1];
          cube->faces[3][cube->n-i-1][cube->n-1] = cube->faces[0][0][cube->n - i - 1];
          cube->faces[0][0][cube->n - i - 1] = aux[i];
        }
        break;

      case 5:
        for (int i = 0; i < cube->n; i++){
          aux[i] = cube->faces[2][cube->n-1][i];
          cube->faces[2][cube->n-1][i] = cube->faces[3][cube->n-1][i];
          cube->faces[3][cube->n-1][i] = cube->faces[4][cube->n-1][i];
          cube->faces[4][cube->n-1][i] = cube->faces[1][cube->n-1][i];
          cube->faces[1][cube->n-1][i] = aux[i];
        }
        break;
      default:
        break;
  }

  }

  free(aux);
  }



struct Cube *init_cube(int n){
  struct Cube *cube = malloc(sizeof(struct Cube));

  cube->dim = 3;
  cube->n = n;
  cube->faces = malloc(6 * sizeof (char **));

  int l = 0;

  for (int i = 0; i < 6; i++){    //For each face
    cube->faces[i] = malloc(cube->n * sizeof(char*));

    for (int j = 0; j < cube->n; j++){
      cube->faces[i][j] = malloc(cube->n);
      for (int k = 0; k < cube->n; k++){
        cube->faces[i][j][k] = l;
        // l++;
      }
    }
    l++;

  }

  return cube;

}


void print_cube(struct Cube *cube){
  int face = 0;
  for (int i = 0; i < cube->n; i++){
    for (int j = 0; j < cube->n; j++){
      printf("    ");
    }
    for (int j = 0; j < cube->n; j++){
      printf(" %2d ", cube->faces[face][i][j]);
    }
    printf("\n");
  }
  for (int i = 0; i < cube->n; i++){
    for (face = 1; face < 5; face++){
      for (int j = 0; j < cube->n; j++){
        printf(" %2d ", cube->faces[face][i][j]);
      }
    }
    printf("\n");
  }
  for (int i = 0; i < cube->n; i++){
    for (int j = 0; j < cube->n; j++){
      printf("    ");
    }
    for (int j = 0; j < cube->n; j++){
      printf(" %2d ", cube->faces[face][i][j]);
    }
    printf("\n");
  }
}

void print_algorithm(char *alg){
  int i = 0;
  while (alg[i] != '\0'){
    printf("%c", alg[i]);
    if (alg[i+1] != '\'' && alg[i+1] != '2' && alg[i+1] != '\0'){
      printf(" ");
    }

    i++;
  }
}
