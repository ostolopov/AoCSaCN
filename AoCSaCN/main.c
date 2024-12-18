#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

#define BUFFER 81

typedef enum {
    RED, ORANGE, YELLOW, GREEN, BLUE, INDIGO, VIOLET, BLACK, WHITE
} Color;

typedef struct Object2D{
    char type[BUFFER];
    Color color;
    union {
        struct { int x, y, rad; } circle;      // круг
        struct { int x1, y1, x2, y2; } rectangle; // прямоугольник
        struct { int x1, y1, x2, y2, x3, y3; } triangle; // треугольник
    } data;
} Object2D;

typedef struct Object3D{
    char type[BUFFER];
    double density;
    union {
        struct { int rad; } sphere;              // шар
        struct { int length, width, height; } cuboid; // параллелепипед
        struct { int length; } tetrahedron;            // правильный тетраэдр
    } data;
} Object3D;







int main(void) {
    
    return 0;
}
