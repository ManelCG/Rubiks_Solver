#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <cube.h>
#include <solver.h>

int main(int argc, char *argv[]){
  srandom(69);
  struct Cube *cube = init_cube(3);
  print_cube(cube);

  printf("\n\n");

  // char *alg = generate_scramble(20);
  // print_algorithm(alg);
  // perform_algorithm(cube, alg);

  char *alg = malloc(2);
  strcpy(alg, "R");
  perform_algorithm(cube, alg);

  printf("\n\n");
  print_cube(cube);

  printf("\n\n");


  struct CubePS *cube_ps = initCubePS(cube);
  set_decisions(cube_ps, "L'");
  add_decision(cube_ps, "L'");



  printf("Solved? %d\nBy using %d moves: %s\n", is_solution(cube_ps), get_move_n(cube_ps), get_decisions(cube_ps));

  free(cube_ps);
  free_cube_mem(cube);
}
