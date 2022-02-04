#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <cube.h>
#include <solver.h>

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

  printf("Solved? %d\nBy using %d moves:", is_solution(cube_ps), get_move_n(cube_ps));
  print_algorithm(get_decisions(cube_ps));
  perform_algorithm(cube, get_decisions(cube_ps));
  print_cube(cube);


  free_ps_mem(cube_ps);
  free_cube_mem(cube);
}
