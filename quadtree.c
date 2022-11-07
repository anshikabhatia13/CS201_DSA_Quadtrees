#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Point // Structure for the Point lying on the plane
{
    float x;
    float y;
};

struct Node // Structure for the SubRegion
{
    struct Point *origin;
    float dimension;
};

struct quadtree // Quadtree Structure
{
    struct Node *region;
    struct Point **points; // This is pointer to the Pointer for forming an array

    struct quadtree *NW; // all four defining a region
    struct quadtree *NE;
    struct quadtree *SW;
    struct quadtree *SE;
};

void Print(struct Node *);
struct Point *new_point(float, float);
struct Node *new_Node(struct Point *, float);
bool points_contained(struct Node *, struct Point *);
void Print_point(struct Point *);
int Intersection(struct Node *, struct Node *);
struct quadtree *quadtree_new(struct Node *);
long long Size(struct Point *points[]);
struct quadtree *subdivision(struct quadtree *);
bool Insert(struct quadtree *node, struct Point *);
struct Point **Range(struct quadtree *root, struct Node *);
void Print(struct Node *);

long long main()
{
    printf("\033[1m\033[4mQuad Tree\033[0m\n\n");

    printf("Specify Inital Axis Aligned Bounding Box \n");
    printf("Center: \n");
    float x, y;
    printf("X: ");
    scanf("%e", &x); // scanning the x of xy plane
    printf("Y: ");
    scanf("%e", &y); // scanning the y of xy plane
    float X = x, Y = y;
    float dimension_xy_plane;
    printf("Side Length of the Square region: ");
    scanf("%e", &dimension_xy_plane); // scanning for the Dimensions
    dimension_xy_plane = dimension_xy_plane / 2;

    struct Point *center = new_point(x, y);                     // assigning a new point
    struct Node *Region = new_Node(center, dimension_xy_plane); // assigning a new Node
    printf("Quad Tree Region\n");

    Print(Region); // This will print the region in which we are going to insert points

    struct quadtree *QUADTREE = quadtree_new(Region); // assigning a new QuadTree

    long long count;
    printf("Enter Number of Points you want to insert: ");
    scanf("%d", &count); // Scans the number of points user wsnts to enter
    int counter = count;
    long long i = 0;

    for (int j = 0; j < counter; j++) // loop for inserting an element in the quadtree
    {
        i++;
        printf("Point %d\n", j + 1);
        float x, y;
        printf("X: ");
        scanf("%e", &x);
        printf("Y: ");
        scanf("%e", &y);

        if (X + dimension_xy_plane < x || X - dimension_xy_plane > x || Y + dimension_xy_plane < y || Y - dimension_xy_plane > y) // The point will not insert if the point is not in the user defined region
        {
            printf("Point is Outside Region, So not Inserted\n");
            continue;
        }
        struct Point *p = new_point(x, y);
        printf("Point: ");
        Print_point(p);
        if (Insert(QUADTREE, p) == 0) //  This will insert the element in the quadtree
        {
            printf("Point is Outside Region, So not Inserted\n");
        }
        else
        {
            printf("Point inserted successfully\n");
        }
    }

    printf("\n\033[1m\033[4mPoints near the car\033[0m\n\n");
    printf("Specify the centre of the car\n");
    printf("Center: \n");
    float x_range, y_range;
    printf("X: ");
    scanf("%e", &x_range);
    printf("Y: ");
    scanf("%e", &y_range);

    float dimension_xy_plane_range;
    printf("Dimension of the region you want to see the objects if it is present or not: ");
    scanf("%e", &dimension_xy_plane_range);
    dimension_xy_plane_range /= 2;

    struct Point *center_range = new_point(x_range, y_range);
    struct Node *Region_range = new_Node(center_range, dimension_xy_plane_range);
    printf("Range Search Region\n");
    Print(Region_range);
    struct Point **res = Range(QUADTREE, Region_range); // forms the array present in the given region
    printf("Points inside that Region are as follows\n");
    long long j = 0;
    while (res[j] != NULL && j < 1024) // loop for printing the element in the given region
    {
        Print_point(res[j]);
        j++;
    }
    return 0;
}

struct Point *new_point(float x, float y) //  This will make the new point from the structure and returns it
{
    struct Point *point = (struct Point *)malloc(sizeof(struct Point));
    point->x = x;
    point->y = y;
    return point;
}

void Print_point(struct Point *point) // This prints the point and returns nothing
{
    printf("(%3.3f, %3.3f)\n", point->x, point->y);
}

struct Node *new_Node(struct Point *origin, float dimension) //  This makes the new node and then returns it
{
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    node->origin = origin;
    node->dimension = dimension;
    return node;
}

bool points_contained(struct Node *region, struct Point *point) // This function checks if we insert the point in the region or not
{
    if (point->y > region->origin->y + region->dimension)
    {
        return 0;
    }
    else if (point->x > region->origin->x + region->dimension)
    {
        return 0;
    }
    else if (point->x < region->origin->x - region->dimension)
    {
        return 0;
    }
    else if (point->y < region->origin->y - region->dimension)
    {
        return 0;
    }
    else
        return 1;
}

int Intersection(struct Node *first, struct Node *second) // This function
{
    if (first->origin->x - first->dimension < second->origin->x + second->dimension)
    {
        return 1;
    }
    else if (first->origin->y - first->dimension < second->origin->y + second->dimension)
    {
        return 1;
    }
    else if (first->origin->x + first->dimension > second->origin->x - second->dimension)
    {
        return 1;
    }
    else if (first->origin->y + first->dimension > second->origin->y - second->dimension)
    {
        return 1;
    }

    else
        return 0;
}

struct quadtree *quadtree_new(struct Node *origin) // This function forms the new quadtree
{
    struct quadtree *new = (struct quadtree *)malloc(sizeof(struct quadtree));
    new->SE = NULL;
    new->NE = NULL;
    new->SW = NULL;
    new->NW = NULL;

    new->region = origin;
    new->points = (struct Point **)malloc(sizeof(struct Point *) * 4);

    long long counter = 0;
    while (counter < 4) // Assinging all the points to the NULL pointer
    {
        new->points[counter] = NULL;
        counter++;
    }
    return new;
}

long long Size(struct Point *points[]) //  Returns the quadTree point size
{
    long long count = 0;
    while (count < 4)
    {
        if (points[count] == NULL)
            return count;
        count++;
    }
    return 4;
}

struct quadtree *subdivision(struct quadtree *root) // add the new node of Quad tree to the parent root
{
    float dim = root->region->dimension / 2;

    struct Point *bottomleftpoint = new_point(root->region->origin->x - dim, root->region->origin->y - dim);
    root->SW = quadtree_new(new_Node(bottomleftpoint, dim));

    struct Point *topleftpoint = new_point(root->region->origin->x - dim, root->region->origin->y + dim);
    root->NW = quadtree_new(new_Node(topleftpoint, dim));

    struct Point *bottomrighttpoint = new_point(root->region->origin->x + dim, root->region->origin->y - dim);
    root->SE = quadtree_new(new_Node(bottomrighttpoint, dim));

    struct Point *toprightpoint = new_point(root->region->origin->x + dim, root->region->origin->y + dim);
    root->NE = quadtree_new(new_Node(toprightpoint, dim));

    return root;
}

bool Insert(struct quadtree *root_node, struct Point *point) // Function to insert the point to the quadtree
{
    long long Point_Size;

    if (points_contained(root_node->region, point) == 0) // returns 0 if we cann't insert the point in the region
    {
        return 0;
    }
    Point_Size = Size(root_node->points); // checks the size

    if (root_node->NW == NULL && Point_Size < 4)
    {
        root_node->points[Point_Size] = point;
        return 1;
    }

    if (root_node->NW == NULL)
    {
        subdivision(root_node);
    }

    if (Insert(root_node->SW, point)) // INserts the point in the region
    {
        return 1;
    }
    else if (Insert(root_node->NW, point)) // INserts the point in the region
    {
        return 1;
    }
    else if (Insert(root_node->SE, point)) // INserts the point in the region
    {
        return 1;
    }
    else if (Insert(root_node->NE, point)) // INserts the point in the region
    {
        return 1;
    }
    else
        return 0;
}

struct Point **Range(struct quadtree *root, struct Node *range)   // Gives the points that are inserted in the region
{
    struct Point **OUT;
    OUT = (struct Point **)malloc(sizeof(struct Point *) * 1024);

    long long index = 0;

    int k = 0;
    while (k < 1024)  // assigning all the points to the NULL
    {
        OUT[k] = NULL;
        k++;
    }

    if (Intersection(root->region, range) == 0)  // If two regions are not intersecting then it will return the OUT  
    {
        return OUT;
    }

    long long points_size = Size(root->points);

    k = 0;
    while (points_size > k)  // inserting the point in the array if it present in the region
    {
        if (points_contained(range, root->points[k]))
        {
            OUT[index++] = root->points[k];
        }
        k++;
    }

    if (root->NW == NULL)
    {
        return OUT;
    }

    struct Point **NW_range = Range(root->NE, range);   // inserting the point in the array if it present in the region
    for (int i = 0; i < 1024 && NW_range[i] != NULL; i++)
    {
        OUT[index++] = NW_range[i];
    }

    struct Point **SW_range = Range(root->SW, range);   // inserting the point in the array if it present in the region
    for (int i = 0; i < 1024 && SW_range[i] != NULL; i++)
    {
        OUT[index++] = SW_range[i];
    }

    struct Point **NE_range = Range(root->NE, range);   // inserting the point in the array if it present in the region
    for (int i = 0; i < 1024 && NE_range[i] != NULL; i++)
    {
        OUT[index++] = NE_range[i];
    }

    struct Point **SE_range = Range(root->SE, range);   // inserting the point in the array if it present in the region
    for (int i = 0; i < 1024 && SE_range[i] != NULL; i++)
    {
        OUT[index++] = SE_range[i];
    }

    return OUT;
}

void Print(struct Node *point) // Prints the region in the terminal in which we can enter the numbers
{
    printf("\n");
    printf("(%3.3f, %3.3f)(NW)----------------(%3.3f, %3.3f)(NE)\n", point->origin->x - point->dimension, point->origin->y + point->dimension, point->origin->x + point->dimension, point->origin->y + point->dimension);
    printf("|                                                   |\n");
    printf("|                                                   |\n");
    printf("|                                                   |\n");
    printf("|                                                   |\n");
    printf("|                                                   |\n");
    printf("|                                                   |\n");
    printf("|                                                   |\n");
    printf("|                                                   |\n");
    printf("(%3.3f, %3.3f)(SW)----------------(%3.3f, %3.3f)(SE)\n", point->origin->x - point->dimension, point->origin->y - point->dimension, point->origin->x + point->dimension, point->origin->y - point->dimension);
    printf("\n");
}
