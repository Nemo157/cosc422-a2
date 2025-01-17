#include <fstream>
#include <stdlib.h>
#include <limits.h>

#include "model.hpp"

Model::Model(const std::string filename) {
    std::ifstream fp_in;
    float vx,vy,vz;

    fp_in.open(filename.c_str());
    if(fp_in.fail()) {
        std::cout << "Error opening mesh file" << std::endl;
        exit(1);
    }

    fp_in.ignore(INT_MAX, '\n');
    fp_in >> num_verts >> num_faces >> num_edges;

    verts = new Vert[num_verts];

    for (unsigned int i = 0; i < num_verts; i++) {
        fp_in >> vx >> vy >> vz;
        verts[i].point = Point(vx,vy,vz);
        verts[i].edge = NULL;
    }

    num_edges = num_faces * 3;
    
    faces = new Face[num_faces];
    // Need extra space for boundary edges.  Requirements unknown but at most the same as the number of edges.
    edges = new Edge[2*num_edges];

    unsigned int num, k[3], l;
    for (unsigned int i = 0; i < num_faces; i++) {
        fp_in >> num >> k[0] >> k[1] >> k[2];

        if (num != 3) {
            std::cout << "ERROR: Polygon with index " << i  << " is not a triangle." << std::endl;
            exit(1);
        }

        for (unsigned int j = 0; j < 3; j++) {
            edges[3*i+j].vert = &verts[k[j]];
            verts[k[j]].edge = &edges[3*i+j];
            edges[3*i+j].next = &edges[3*i+(j+1)%3];
            edges[3*i+j].prev = &edges[3*i+(j+2)%3];
            edges[3*i+j].face = &faces[i];
            edges[3*i+j].pair = NULL;
        }

        faces[i].edge = &edges[3*i];

        l = 3*(i+1);
    }
    
    fp_in.close();

    std::cout << " File successfully read." << std::endl;

    for (unsigned int i = 0; i < num_edges; i++) {
        if (edges[i].pair == NULL) {
            for (unsigned int j = i+1; j < num_edges; j++) {
                if (edges[j].vert == edges[i].prev->vert && edges[j].prev->vert == edges[i].vert) {
                    edges[i].pair = &edges[j];
                    edges[j].pair = &edges[i];
                    break;
                }
            }
            // No edge found so create a boundary edge.
            if (edges[i].pair == NULL) {
                edges[i].pair = &edges[l];

                edges[l].vert = edges[i].prev->vert;

                Edge *e1 = &edges[i];
                do {
                    e1 = e1->prev->pair;
                } while (e1 != NULL && e1->face != NULL);

                if (e1 != NULL) {
                    edges[l].next = e1;
                    e1->prev = &edges[l];
                }

                Edge *e2 = &edges[i];
                do {
                    e2 = e2->next->pair;
                } while (e2 != NULL && e2->face != NULL);

                if (e2 != NULL) {
                    edges[l].prev = e2;
                    e2->next = &edges[l];
                }

                edges[l].face = NULL;
                edges[l].pair = &edges[i];

                l++;
            }
        }
    }

    std::cout << "Found [" << l - num_edges << "] boundary edges." << std::endl;
    num_edges = l;

    std::cout << "Found all pairs." << std::endl;

    bool error = false;
    for (unsigned int i = 0; i < num_edges; i++) {
        if (edges[i].vert != NULL && edges[i].pair == NULL) {
            std::cout << "Found edge [" << i << "] without a pair." << std::endl;
            error = true;
        }
        if (edges[i].pair != NULL && edges[i].prev == NULL) {
            std::cout << "Found edge [" << i << "] without a prev." << std::endl;
            error = true;
        }
        if (edges[i].pair != NULL && edges[i].next == NULL) {
            std::cout << "Found edge [" << i << "] without a next." << std::endl;
            error = true;
        }
    }

    if (error) {
        std::cout << "Quitting cause model is broken." << std::endl;
        exit(2);
    }
}
