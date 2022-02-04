#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <cube.h>
#include <solver.h>

#define MAX_SUCC_SIZE 50
#define MAX_SOLVE_DEPTH 5

struct CubePS{
  struct Cube *cube;
  char *decisions;
  int move_n;
  int solved;
};

struct Successors{
  struct CubePS **v;
  int size;
  int n;
  int next_add;
  int next_extend;
  int next_check;
  int depth;
};

int rec = 0;

struct Successors *successors(struct Successors *succ){
  char lastmove = get_last_move(succ->v[succ->next_extend]);

  for (int i = 0; i < 6; i++){
    //Face
    char face = int_to_face(i);

    if (face != lastmove){
      char *move = malloc(3);
      move[0] = face;
      move[1] = '\0';
      move[2] = '\0';
      for (int j = 0; j < 3; j++){
        succ->v[succ->next_add] = copyCubePS(succ->v[succ->next_extend]);
        //cw, a-cw, 2
        switch(j){
          case 0: //Add cw
            add_decision(succ->v[succ->next_add], move);
            succ->n++;
            break;
          case 1:
            move[1] = '\'';
            add_decision(succ->v[succ->next_add], move);
            succ->n++;
            break;
          case 2:
            move[1] = '2';
            add_decision(succ->v[succ->next_add], move);
            succ->n++;
            break;
        }
        // printf("Added succ \"%s\" n %d\n", get_decisions(succ->v[succ->next_add]), succ->next_add);
        succ->next_add++;
      }
    }

  }
  //return succ;
}

struct CubePS *solve(struct CubePS *ps){
  struct CubePS *solution = NULL;
  struct CubePS *best_solution = NULL;

  if (is_solution(ps)){
    return ps;
  }

  struct Successors *succ = malloc(sizeof(struct Successors));
  succ->size = MAX_SUCC_SIZE;
  succ->v = malloc(sizeof(struct CubePS) * succ->size);

  succ->v[0] = ps;
  succ->next_add = 1;
  succ->next_extend = 0;
  succ->next_check = 1;
  succ->n = 1;


  struct CubePS *solve_rec(struct Successors *succ){
    // for (int i = succ->next_extend; i < succ->n; i++){
    while (succ->next_extend < succ->n){
      // printf("Here\n");
      // printf("Successors for %d. Dec: '%s'\n", succ->next_extend, get_decisions(succ->v[succ->next_extend]));
      if (succ->n + 18 >= succ->size){
        succ->size *= 2;
        struct CubePS **newv = malloc(sizeof(struct CubePS) * succ->size);

        for (int i = 0; i < succ->n; i++){
          if (succ->v[i] != NULL){
            newv[i] = copyCubePS(succ->v[i]);
            free_ps_mem(succ->v[i]);
          }
        }
        free(succ->v);
        succ->v = newv;

        // int i;
        // for (i = 0; i < succ->n; i++){
        //   if (succ->v[i] != NULL){
        //     printf("We have %d free spots\n", i);
        //     break;
        //   }
        // }
        // exit(0);
      }

      successors(succ);

      // printf("Added all successors\n");
      free_ps_mem(succ->v[succ->next_extend]);
      succ->v[succ->next_extend] = NULL;
      succ->next_extend ++;

      // printf("next_check %d\nnext_add %d\nnext_extend %d\n",
      //         succ->next_check, succ->next_add, succ->next_extend);

      //Check if we found solution and return
      while (succ->next_check < succ->n){
        if (is_solution(succ->v[succ->next_check])){
            return succ->v[succ->next_check];
        }
        succ->next_check++;
      }
    }

    return NULL;
  }

  solution = solve_rec(succ);
  best_solution = solution;

  return best_solution;
  // if (ps->move_n < MAX_SOLVE_DEPTH){
  //   struct Successors *succ = successors(ps);
  //   // free_ps_mem(ps);
  //   free(ps);

  //   // printf("PS has %d\n", ps->move_n);
  //   for (int i = 0; i < succ->n; i++){
  //     // printf("Sending new for solve\n");
  //     // printf("With decisions %s\n", succ->v[i]->decisions);
  //     rec++;
  //     solution = solve(succ->v[i]);    //Recursion
  //     if (solution != NULL){
  //       if(is_solution(solution)){
  //         printf("Found solution\n");
  //         if (best_solution != NULL){
  //           if (solution->move_n < best_solution->move_n){
  //             free_ps_mem(best_solution);
  //             best_solution = copyCubePS(solution);
  //             printf("Found best solution\n");
  //           }
  //         } else {
  //           printf("Found best solution\n");
  //           best_solution = copyCubePS(solution);
  //         }
  //       }
  //     }
  //   }
  //   free(succ->v);
  //   free(succ);
  // } else {
  //   free(ps);
  //   // free_ps_mem(ps);
  // }
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

void free_ps_mem(struct CubePS *ps){
  // free_cube_mem(ps->cube);
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

  // new->cube = copy_cube(ps->cube);
  new->cube = ps->cube;
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
  new_decisions[i+j] = '\0';

  free(cube_ps->decisions);
  cube_ps->decisions = new_decisions;
  cube_ps->move_n++;
  // perform_algorithm(cube_ps->cube, decision);
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
    struct Cube *solved= copy_cube(cube_ps->cube);
    perform_algorithm(solved, get_decisions(cube_ps));
    cube_ps->solved = is_solved(solved);
    free_cube_mem(solved);
    return cube_ps->solved;
  } else {
    return cube_ps->solved;
  }


}
