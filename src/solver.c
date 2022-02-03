#include <stdlib.h>
#include <string.h>
#include <cube.h>
#include <solver.h>

struct CubePS{
  struct Cube *cube;
  char *decisions;
  int move_n;
};

struct CubePS *solve(struct CubePS *ps){

}

void free_ps_mem(struct CubePS *ps){
  free_cube_mem(ps->cube);
  free(ps->decisions);

  free(ps);
}

struct CubePS *initCubePS(struct Cube *cube){
  struct CubePS *cube_ps = malloc(sizeof(struct CubePS));

  cube_ps->cube = copy_cube(cube);
  cube_ps->decisions = NULL;
  cube_ps->move_n = 0;

  return cube_ps;
}

void set_decisions(struct CubePS *cube_ps, char *decisions){
  int ptr = 0;
  while (decisions[ptr] != '\0'){
    if (decisions[ptr] != '\'' && decisions[ptr] != '2'){
      cube_ps->move_n++;
    }
    ptr++;
  }
  cube_ps->decisions = malloc(++ptr);
  strcpy(cube_ps->decisions, decisions);
}

void add_decision(struct CubePS *cube_ps, char *decision){
  int new_d_size;
  int decisions_size = 0;
  int i, j;

  //Get size of decisions
  while (cube_ps->decisions[decisions_size] != '\0'){
    decisions_size++;
  }
  //Get size of new decision
  if (decision[1] != '\0'){
    new_d_size = 2;
  } else {
    new_d_size =1;
  }

  char *new_decisions = malloc(new_d_size + decisions_size + 1);

  for (i = 0; i < decisions_size; i++){
    new_decisions[i] = cube_ps->decisions[i];
  }

  for (j = 0; j < new_d_size; j++){
    new_decisions[i+j] = decision[j];
  }
  new_decisions[i+j+1] = '\0';

  free(cube_ps->decisions);
  cube_ps->decisions = new_decisions;
  cube_ps->move_n++;
}

int get_move_n(struct CubePS *ps){
  return ps->move_n;
}

char *get_decisions(struct CubePS *ps){
  return ps->decisions;
}

int is_solution(struct CubePS *cube_ps){
  struct Cube *solved = copy_cube(cube_ps->cube);

  perform_algorithm(solved, cube_ps->decisions);

  return is_solved(solved);
}
