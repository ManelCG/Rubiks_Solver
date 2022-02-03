#ifndef CUBE_H
#define CUBE_H

struct Cube *init_cube(int);
void print_cube(struct Cube *);
void turn_face(struct Cube *, int, int);
void perform_algorithm(struct Cube *, char *);
void print_algorithm(char *);
void free_cube_mem(struct Cube*);
char *generate_scramble(int);
struct Cube *copy_cube(struct Cube *);
int is_solved(struct Cube *);

#endif
