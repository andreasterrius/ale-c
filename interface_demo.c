//
// Created by alether on 10/7/23.
//
#include <stdio.h>

//  Define a structure for the base class
typedef struct {
    // A pointer to the vtable
    struct ShapeVTable* vtable;
} Shape;

// Define a vtable structure for the base class
typedef struct ShapeVTable {
    double (*area)(const Shape*);       // Function pointer for area calculation
    double (*perimeter)(const Shape*);  // Function pointer for perimeter calculation
} ShapeVTable;

typedef struct {
    Shape base;     // Base class as the first member
    double radius;
} Circle;

typedef struct {
    Shape base;     // Base class as the first member
    double width;
    double height;
} Rectangle;

double circleArea(const Shape* shape) {
    const Circle* circle = (const Circle*)shape;
    return 3.14159265359 * circle->radius * circle->radius;
}

double circlePerimeter(const Shape* shape) {
    const Circle* circle = (const Circle*)shape;
    return 2 * 3.14159265359 * circle->radius;
}

double rectangleArea(const Shape* shape) {
    const Rectangle* rectangle = (const Rectangle*)shape;
    return rectangle->width * rectangle->height;
}

double rectanglePerimeter(const Shape* shape) {
    const Rectangle* rectangle = (const Rectangle*)shape;
    return 2 * (rectangle->width + rectangle->height);
}

// Vtable for Circle
static ShapeVTable circleVTable = {
        .area = circleArea,
        .perimeter = circlePerimeter,
};

// Vtable for Rectangle
static ShapeVTable rectangleVTable = {
        .area = rectangleArea,
        .perimeter = rectanglePerimeter,
};

int main() {
    Circle myCircle = {.base.vtable = &circleVTable, .radius = 5.0};
    Rectangle myRectangle = {.base.vtable = &rectangleVTable, .width = 4.0, .height = 6.0};

    // Call functions through the base class pointer
    double circleAreaResult = myCircle.base.vtable->area(&myCircle);
    double circlePerimeterResult = myCircle.base.vtable->perimeter(&myCircle);

    double rectangleAreaResult = myRectangle.base.vtable->area(&myRectangle);
    double rectanglePerimeterResult = myRectangle.base.vtable->perimeter(&myRectangle);

    printf("Circle Area: %.2f\n", circleAreaResult);
    printf("Circle Perimeter: %.2f\n", circlePerimeterResult);
    printf("Rectangle Area: %.2f\n", rectangleAreaResult);
    printf("Rectangle Perimeter: %.2f\n", rectanglePerimeterResult);

    return 0;
}
