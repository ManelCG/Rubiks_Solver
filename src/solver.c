#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <cube.h>
#include <solver.h>

#define MAX_SOLVE_DEPTH 6

struct CubePS{
  struct Cube *cube;
  char *decisions;
  int move_n;
  int solved;
};

struct Successors{
  struct CubePS **v;
  int n;
};

struct Successors *successors(struct CubePS *ps){
  struct Successors *succ = malloc(sizeof(struct Successors));
  char lastmove = get_last_move(ps);
  succ->n = 0;

  succ->v = malloc(sizeof(struct Successors) * 18);

  int ptr = 0;
  for (int i = 0; i < 6; i++){
    //Face
    char face = int_to_face(i);

    if (face != lastmove){
      char *move = malloc(3);
      move[0] = face;
      move[1] = '\0';
      move[2] = '\0';
      for (int j = 0; j < 3; j++){
        succ->v[ptr] = copyCubePS(ps);
        //cw, a-cw, 2
        switch(j){
          case 0: //Add cw
            add_decision(succ->v[ptr], move);
            succ->n++;
            break;
          case 1:
            move[1] = '\'';
            add_decision(succ->v[ptr], move);
            succ->n++;
            break;
          case 2:
            move[1] = '2';
            add_decision(succ->v[ptr], move);
            succ->n++;
            break;
        }
        ptr++;
      }
    }

  }
  return succ;
}

char get_last_move(struct CubePS *ps){
  if (get_decisions(ps) != NULL){
    int ptr = 0;
    char move = '\0';
    while (ps->decisions[ptr] != '\0'){
      if (ps->decisions[ptr] != '\0' &&
          ps->decisions[ptr] != '\'' &&
          ps->decisions[ptr] != '2'){
        move = ps->decisions[ptr];
      }
      ptr ++;
    }

    return move;
  }
  return '\0';
}

struct CubePS *solve(struct CubePS *ps){
  struct CubePS *solution;

  if (is_solution(ps)){
    return ps;
  }

  if (ps->move_n < MAX_SOLVE_DEPTH){
    struct Successors *succ = successors(ps);

    // printf("PS has %d\n", ps->move_n);
    for (int i = 0; i < succ->n; i++){
      // printf("Sending new for solve\n");
      // printf("With decisions %s\n", succ->v[i]->decisions);
      solution = solve(succ->v[i]);
      if (solution != NULL){
        if(is_solution(solution)){
          printf("Found solution\n");
          return solution;
        }
      }
    }
  }
  return NULL;
}

void free_ps_mem(struct CubePS *ps){
  free_cube_mem(ps->cube);
  free(ps->decisions);

  free(ps);
}

struct CubePS *initCubePS(struct Cube *cube){
  struct CubePS *cube_ps = malloc(sizeof(struct CubePS));

  cube_ps->cube = copy_cube(cube);
  cube_ps->decisions = malloc(1);
  strcpy(cube_ps->decisions, "");
  cube_ps->move_n = 0;
  cube_ps->solved = -1;

  return cube_ps;
}

struct CubePS *copyCubePS(struct CubePS *ps){
  struct CubePS *new = malloc(sizeof(struct CubePS));

  new->cube = copy_cube(ps->cube);
  new->solved = -1;
  int ptr = 0;
  while (ps->decisions[ptr] != '\0'){
    ptr++;
  }
  new->decisions = malloc(++ptr);
  strcpy(new->decisions, ps->decisions);
  new->move_n = ps->move_n;
  return new;
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
  perform_algorithm(cube_ps->cube, decision);
}

int get_move_n(struct CubePS *ps){
  return ps->move_n;
}

char *get_decisions(struct CubePS *ps){
  if (ps->decisions != NULL){
    return ps->decisions;
  } else {
    return "";
  }
}

int is_solution(struct CubePS *cube_ps){
  if (cube_ps->solved == -1){
    struct Cube *solved = copy_cube(cube_ps->cube);

    // perform_algorithm(solved, get_decisions(cube_ps));
    cube_ps->solved = is_solved(solved);
    return cube_ps->solved;
  } else {
    return cube_ps->solved;
  }


}
