#ifndef CUBE_H
#define CUBE_H

struct Cube *init_cube(int);                      //Allocates memory for cube and sets each face to one number
void print_cube(struct Cube *);                   //Prints unfolded cube
void turn_face(struct Cube *, int f, int cw);     //Turns face f clockwise if cw=1 or anti-clockwise if cw=0
void perform_algorithm(struct Cube *, char *alg); //Performs alg by calling turn_face multiple times
void print_algorithm(char *);                     //Prints algorithm in pretty manner
void free_cube_mem(struct Cube*);                 //Frees all cube memory and then cube itself
char *generate_scramble(int);                     //Generates random algorithm without repetitions
struct Cube *copy_cube(struct Cube *);            //Properly copies all cube data. Returns pointer to new cube
int is_solved(struct Cube *);                     //Checks if all faces are solved

#endif
