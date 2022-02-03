#ifndef SOLVER_H
#define SOLVER_H

struct CubePS *initCubePS(struct Cube *cube);   //Creates Partial Solution from Cube structure
void set_decisions(struct CubePS *, char *);    //Sets PS decisions to string passed as arg
void add_decision(struct CubePS *, char *);     //Adds one move from string to PS
int is_solution(struct CubePS *);               //Checks if PS->decisions solve PS->cube
int get_move_n(struct CubePS *);                //Returns PS->move_n
char *get_decisions(struct CubePS *);           //Returns PS->decisions (as pointer to string)
void free_ps_mem(struct CubePS *);              //Frees all memory from PS, and then PS itself
struct CubePS *solve(struct CubePS *);          //Returns shortest solution for PS

#endif
