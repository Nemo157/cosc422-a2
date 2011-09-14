#ifndef MODEL_HPP
#define MODEL_HPP
#include <string>
#include <vector>

#include "geom.hpp"

struct HE_vert;
struct HE_face;

struct HE_edge {
    unsigned int index;
    HE_vert *vert;
    HE_face *face;
    HE_edge *next;
    HE_edge *prev;
    HE_edge *pair;
};

struct HE_vert {
    unsigned int index;
    Point point;
    HE_edge *edge;
    Vector normal;
};

struct HE_face {
    unsigned int index;
    HE_edge *edge;
    Vector normal;
};

class Model {
    public:
        Model();
        Model(const Model&);
        Model(const std::string);

        ~Model();

        void operator=(const Model&);

        void display();
        void collapse_some_edge();
        void collapse_some_edges();
        void toggle_error_metric();
        unsigned int current_faces();
        void calculate_normals();
        void toggle_normal_map_display();

    private:
        HE_vert *verts;
        HE_face *faces;
        HE_edge *edges;

        unsigned int num_verts, num_faces, num_edges;
};
#endif
