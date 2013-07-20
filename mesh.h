#ifndef MESH_H
#define MESH_H

#include <string>

using namespace std;

#define FILE_OFF 1
#define FILE_COFF 2

typedef struct _Vertex{
	float x;
	float y;
	float z;
}Vertex;

typedef struct _Face{
	int vertex_1, vertex_2, vertex_3;
	float normal_x, normal_y, normal_z;
}Face;

typedef struct _Color{
	int red;
	int green;
	int blue;
}Color;

typedef struct{
	int numero_vertex;
	Vertex *list_vertex;
	
	int numero_face;
	Face *list_faces;
	
	int numero_color;
	Color *list_color;

	float x_max, x_min;
	float y_max, y_min;
	float z_max, z_min;
}Mesh;

Mesh* new_mesh ();
void del_mesh (Mesh& mesh);
int load_mesh (Mesh& mesh, const string& file_name);
void render_mesh (const Mesh& mesh);
void resize_mesh (Mesh& mesh);

#endif