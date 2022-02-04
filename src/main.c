#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <cube.h>
#include <solver.h>

#define MAX_SOLVE_DEPTH 5
#define MAX_SUCC_SIZE 500

int main(int argc, char *argv[]){
  srandom(420);
  struct Cube *cube = init_cube(3);
  char *scramble = generate_scramble(6);
  printf("Generated scramble:\n");
  print_algorithm(scramble);

  // char *tperm = "RUR'U'R'FR2U'R'U'RUR'F'";
  // printf("T-Perm:\n");
  // print_algorithm(tperm);

  perform_algorithm(cube, scramble);

  printf("\n\n");
  print_cube(cube);
  printf("\n\n");

  struct CubePS *cube_ps = initCubePS(cube);

  cube_ps = solve(cube_ps);

  if (cube_ps != NULL){

    printf("Solved? %d\nBy using %d moves:", is_solution(cube_ps), get_move_n(cube_ps));
    print_algorithm(get_decisions(cube_ps));
    perform_algorithm(cube, get_decisions(cube_ps));
    print_cube(cube);
    free_ps_mem(cube_ps);
  } else {
    printf("No solution found under %d moves\n", MAX_SOLVE_DEPTH);
  }


  free_cube_mem(cube);
}
