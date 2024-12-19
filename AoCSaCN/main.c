#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <sys/stat.h>

typedef enum {
    RED, ORANGE, YELLOW, GREEN, BLUE, INDIGO, VIOLET
} Color;

const char *COLOR_NAMES[] = {"red", "orange", "yellow", "green", "blue", "indigo", "violet"};
const char *input_file = "input.txt";
const char *output_file = "output.txt";
const char *stat_file = "stat.txt";
typedef union
{
    struct Сircle
    {
        int x, y, radius;
    } circle;
    struct Кectangle
    {
        int x1, y1, x2, y2;
    } rectangle;
    struct Triangle
    {
        int x1, y1, x2, y2, x3, y3;
    } triangle;
} Object_params;

typedef struct Object
{
    char type[20];
    Object_params params;
    Color color;
    double area;
} Object;

int get_int(int *number, int min, int max);
void calculate_area(Object *object);
void bubble_sort(Object *objects, int count, int direction);
void read_objects(const char *filename, Object **objects, int count);
void write_objects(const char *filename, Object *objects, int count, int flag);
void generate_file(const char *filename, int count);
void print_statistics(const char *filename, clock_t start, clock_t end, const char *program_filename);

int main(void)
{
    clock_t start_time = clock();

    int check = 0;
    int is_sorted = 0;
    printf("Enter the number of shapes to generate: ");
    int count = 0;
    check = get_int(&count, 0, INT_MAX);
    if (check == EOF)
    {
        return -1;
    }
    generate_file(input_file, count);
    Object *objects = NULL;
    read_objects(input_file, &objects, count);
    printf("Choose sorting order (1 for ascending, 0 for descending): ");
    int direction = 0;
    check = get_int(&direction, 0, 1);
    if (check == EOF)
    {
        free(objects);
        return -1;
    }
    write_objects(output_file, objects, count, is_sorted);
    bubble_sort(objects, count, direction);
    is_sorted = 1;
    write_objects(output_file, objects, count, is_sorted);
    free(objects);

    clock_t end_time = clock();
    print_statistics(stat_file, start_time, end_time, __FILE__);

    return 0;
}

int get_int(int *number, int min, int max)
{
        int check = 0, number_check, flag = 0;
        while(flag == 0)
        {
                check = scanf("%d" , &number_check);
                switch(check)
                {
                        case EOF:
                                printf("Program terminated.\n");
                                flag = 1;
                                break;
                        case 1 :
                                if (number_check < min || number_check > max)
                                {
                                    printf("Number out of range. Try again!\n");
                                    scanf("%*[^\n]");
                                    flag = 0;
                                    break;
                                }
                                *number = number_check;
                                scanf("%*[^\n]");
                                flag = 1;
                                break;
                        case 0:
                                printf("Invalid number. Try again!\n");
                                scanf("%*[^\n]");
                                flag = 0;
                                break;
                        
                }
        }
        return check;
}

void calculate_area(Object *object)
{
    if (strcmp(object->type, "Circle") == 0)
    {
        int radius = object->params.circle.radius;
        object->area = M_PI * radius * radius;
    }
    else if (strcmp(object->type, "Rectangle") == 0)
    {
        int width = abs(object->params.rectangle.x2 - object->params.rectangle.x1);
        int height = abs(object->params.rectangle.y2 - object->params.rectangle.y1);
        object->area = width * height;
    }
    else if (strcmp(object->type, "Triangle") == 0)
    {
        int x1 = object->params.triangle.x1;
        int y1 = object->params.triangle.y1;
        int x2 = object->params.triangle.x2;
        int y2 = object->params.triangle.y2;
        int x3 = object->params.triangle.x3;
        int y3 = object->params.triangle.y3;
        object->area = fabs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
    }
    else
    {
        object->area = 0.0;
    }
}

void bubble_sort(Object *objects, int count, int direction)
{
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            int condition = direction ? (objects[j].area > objects[j + 1].area) : (objects[j].area < objects[j + 1].area);
            if (condition)
            {
                Object temp = objects[j];
                objects[j] = objects[j + 1];
                objects[j + 1] = temp;
            }
        }
    }
}

void read_objects(const char *filename, Object **objects, int count)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Failed to open input file");
        return;
    }

    *objects = malloc(count * sizeof(Object));
    for (int i = 0; i < count; i++)
    {
        fscanf(file, "%s", (*objects)[i].type);
        if (strcmp((*objects)[i].type, "Circle") == 0)
        {
            fscanf(file, "%d %d %d", &(*objects)[i].params.circle.x, &(*objects)[i].params.circle.y, &(*objects)[i].params.circle.radius);
        }
        else if (strcmp((*objects)[i].type, "Rectangle") == 0)
        {
            fscanf(file, "%d %d %d %d",
                   &(*objects)[i].params.rectangle.x1,
                   &(*objects)[i].params.rectangle.y1,
                   &(*objects)[i].params.rectangle.x2,
                   &(*objects)[i].params.rectangle.y2);
        }
        else if (strcmp((*objects)[i].type, "Triangle") == 0)
        {
            fscanf(file, "%d %d %d %d %d %d",
                   &(*objects)[i].params.triangle.x1,
                   &(*objects)[i].params.triangle.y1,
                   &(*objects)[i].params.triangle.x2,
                   &(*objects)[i].params.triangle.y2,
                   &(*objects)[i].params.triangle.x3,
                   &(*objects)[i].params.triangle.y3);
        }
        char color_name[10];
        fscanf(file, "%s", color_name);
        for (int c = 0; c < 7; c++)
        {
            if (strcmp(color_name, COLOR_NAMES[c]) == 0)
            {
                (*objects)[i].color = c;
                break;
            }
        }
        calculate_area(&(*objects)[i]);
    }

    fclose(file);
}

void write_objects(const char *filename, Object *objects, int count, int flag)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        perror("Failed to open output file");
        return;
    }
    fprintf(file, "Number of objects: %d\n\n", count);
    fprintf(file, "Initial data:\n");
    for (int i = 0; i < count; i++)
    {
        if (strcmp(objects[i].type, "Circle") == 0) {
            fprintf(file, "%s %d, %d, %d %s\n",
                    objects[i].type,
                    objects[i].params.circle.x,
                    objects[i].params.circle.y,
                    objects[i].params.circle.radius,
                    COLOR_NAMES[objects[i].color]);
        }
        else if (strcmp(objects[i].type, "Rectangle") == 0)
        {
            fprintf(file, "%s %d, %d, %d, %d %s\n",
                    objects[i].type,
                    objects[i].params.rectangle.x1,
                    objects[i].params.rectangle.y1,
                    objects[i].params.rectangle.x2,
                    objects[i].params.rectangle.y2,
                    COLOR_NAMES[objects[i].color]);
        }
        else if (strcmp(objects[i].type, "Triangle") == 0)
        {
            fprintf(file, "%s %d, %d, %d, %d, %d, %d %s\n",
                    objects[i].type,
                    objects[i].params.triangle.x1,
                    objects[i].params.triangle.y1,
                    objects[i].params.triangle.x2,
                    objects[i].params.triangle.y2,
                    objects[i].params.triangle.x3,
                    objects[i].params.triangle.y3,
                    COLOR_NAMES[objects[i].color]);
        }
    }
    if (flag == 1)
    {
        fprintf(file, "\nSorted data:\n");
        for (int i = 0; i < count; i++)
        {
            fprintf(file, "%s %s Area: %.2f\n", objects[i].type, COLOR_NAMES[objects[i].color], objects[i].area);
        }
    }
    fclose(file);
}

void generate_file(const char *filename, int count)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        perror("Failed to open file for generation");
        return;
    }

    fprintf(file, "Number of objects: %d\n\n", count);
    for (int i = 0; i < count; i++)
    {
        int shape_type = rand() % 3;
        const char *color = COLOR_NAMES[rand() % 7];

        if (shape_type == 0)
        {
            int x = rand() % 100;
            int y = rand() % 100;
            int radius = rand() % 50 + 1;
            fprintf(file, "Circle %d %d %d %s\n", x, y, radius, color);
        }
        else if (shape_type == 1)
        {
            int x1 = rand() % 100;
            int y1 = rand() % 100;
            int x2 = x1 + rand() % 50 + 1;
            int y2 = y1 + rand() % 50 + 1;
            fprintf(file, "Rectangle %d %d %d %d %s\n", x1, y1, x2, y2, color);
        }
        else if (shape_type == 2)
        {
            int x1 = rand() % 100;
            int y1 = rand() % 100;
            int x2 = rand() % 100;
            int y2 = rand() % 100;
            int x3 = rand() % 100;
            int y3 = rand() % 100;
            
            fprintf(file, "Triangle %d %d %d %d %d %d %s\n", x1, y1, x2, y2, x3, y3, color);
        }
    }

    fclose(file);
}

void print_statistics(const char *filename, clock_t start, clock_t end, const char *program_filename)
{
    FILE *stat_file = fopen(filename, "w");
    if (!stat_file)
    {
        perror("Failed to open input file");
        return;
    }
    struct stat st;
    stat(program_filename, &st);
    fprintf(stat_file, "Execution time: %ld ms\n", (end - start) * 1000 / CLOCKS_PER_SEC);
    fprintf(stat_file, "Program size: %lld bytes\n", st.st_size);
    FILE *file = fopen(program_filename, "r");
    if (file)
    {
        int lines = 0, characters = 0;
        char c;
        while ((c = fgetc(file)) != EOF)
        {
            characters++;
            if (c == '\n')
            {
                lines++;
            }
        }
        fclose(file);
        fprintf(stat_file, "Lines of code: %d\n", lines);
        fprintf(stat_file, "Characters in code: %d\n", characters);
        fprintf(stat_file, "The approximate time spent on writing and debugging the program is 3 hours, learning the syntax for 30 minutes.\n");
        
    }
    else
    {
        fprintf(stat_file, "Failed to open program file for statistics.\n");
    }
    fclose(file);
}
