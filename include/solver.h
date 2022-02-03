#ifndef SOLVER_H
#define SOLVER_H

struct CubePS *initCubePS(struct Cube *cube);
void set_decisions(struct CubePS *, char *);
void add_decision(struct CubePS *, char *);
int is_solution(struct CubePS *);
int get_move_n(struct CubePS *);
char *get_decisions(struct CubePS *);

#endif
