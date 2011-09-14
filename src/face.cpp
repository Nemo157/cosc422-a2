#include <GL/gl.h>

#include "face.hpp"

Face::Face() {
    edge   = NULL;
    mem_normal = Vector();
}

Vector Face::normal() {
    if (mem_normal != Vector()) {
        return mem_normal;
    }

    Point point1 = edge->vert->point,
          point2 = edge->next->vert->point,
          point3 = edge->prev->vert->point;

    mem_normal = (point2 - point1).cross(point3 - point1).unit();
    return mem_normal;
}

void Face::display() {
    HE_edge *e = edge;
    do {
        Point  point  = edge->vert->point;
        Vector normal = edge->vert->normal;

        glNormal3d(normal.dx, normal.dy, normal.dz);
        glVertex3d(point.x, point.y, point.z);
        e = e->next;
    } while (e != edge);
}