#include <iostream>
#include <fstream>
#include <cmath>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include "mesh.h"

Mesh* new_mesh ()
{
	Mesh *mesh_pointer;
	mesh_pointer = (Mesh *) malloc (sizeof (Mesh));

	if (mesh_pointer == NULL)
		return NULL;

	mesh_pointer->x_max = 0;
	mesh_pointer->x_min = 0;
	mesh_pointer->y_max = 0;
	mesh_pointer->y_min = 0;
	mesh_pointer->z_max = 0;
	mesh_pointer->z_min = 0;

	mesh_pointer->numero_face = 0;
	mesh_pointer->numero_color = 0;
	mesh_pointer->numero_vertex = 0;

	return mesh_pointer;
}

void del_mesh (Mesh* mesh)
{
	if (mesh != NULL)
	{
		if (mesh->list_faces != NULL)
			free (mesh->list_faces);
		if (mesh->list_color != NULL)
			free (mesh->list_color);
		if (mesh->list_vertex != NULL)
			free (mesh->list_vertex);

		free (mesh);
	}

	return ;
}

int load_mesh (Mesh& mesh, const string& file_name)
{
	int dummy, int_type_file;
	ifstream file_stream (file_name.c_str());
	char *off, *coff;
	string type_file;
	
	off = strdup ("OFF");
	coff = strdup ("COFF");

	if (file_stream.is_open())
	{
		file_stream >> type_file;

		if (strcmp (off, type_file.c_str()) == 0)
		{
			int_type_file = FILE_OFF;
//			cout << "Type file: " << int_type_file << endl;
		}
		else if (strcmp (coff, type_file.c_str()) == 0)
		{
			int_type_file = FILE_COFF;
//			cout << "Type file: " << int_type_file << endl;
		} else
		{
			cout << "Something wrong with type of file" << endl;
		}

		file_stream >> mesh.numero_vertex;
		file_stream >> mesh.numero_face;
		file_stream >> dummy;

		mesh.numero_color = mesh.numero_vertex;
/*
		cout << "Número de vertex: " << mesh.numero_vertex << "\t";
		cout << "Número de faces: " << mesh.numero_face << "\t";
		cout << "Dummy: " << dummy << endl;
*/
		mesh.list_vertex = (Vertex *) malloc (sizeof (Vertex) * mesh.numero_vertex);
		mesh.list_faces = (Face *) malloc (sizeof (Face) * mesh.numero_face);

		if (int_type_file == FILE_COFF)
		{
			mesh.list_color = (Color *) malloc (sizeof (Color) * mesh.numero_color);
			
			for (int i = 0; i < mesh.numero_vertex; ++i)
			{
				file_stream >> mesh.list_vertex[i].x;
				file_stream >> mesh.list_vertex[i].y;
				file_stream >> mesh.list_vertex[i].z;

				file_stream >> mesh.list_color[i].red;
				file_stream >> mesh.list_color[i].green;
				file_stream >> mesh.list_color[i].blue;

				file_stream >> dummy;
			}
			
			for (int i = 0; i < mesh.numero_face; ++i)
			{
				file_stream >> dummy;
				
				file_stream >> mesh.list_faces[i].vertex_1;
				file_stream >> mesh.list_faces[i].vertex_2;
				file_stream >> mesh.list_faces[i].vertex_3;
			}
		}
		else
		{
			for (int i = 0; i < mesh.numero_vertex; ++i)
			{
				file_stream >> mesh.list_vertex[i].x;
				file_stream >> mesh.list_vertex[i].y;
				file_stream >> mesh.list_vertex[i].z;
			}

			for (int i = 0; i < mesh.numero_face; ++i)
			{
				file_stream >> dummy;
				
				file_stream >> mesh.list_faces[i].vertex_1;
				file_stream >> mesh.list_faces[i].vertex_2;
				file_stream >> mesh.list_faces[i].vertex_3;
			}
		}
		file_stream.close();
	}

	resize_mesh (mesh);

	return 0;
}

void bounding_box (Mesh& mesh)
{
	for (int i = 0; i < mesh.numero_vertex; ++i)
	{
		if (mesh.x_max < mesh.list_vertex[i].x)
			mesh.x_max = mesh.list_vertex[i].x;

		if (mesh.y_max < mesh.list_vertex[i].y)
			mesh.y_max = mesh.list_vertex[i].y;

		if (mesh.z_max < mesh.list_vertex[i].z)
			mesh.z_max = mesh.list_vertex[i].z;

		if (mesh.x_min > mesh.list_vertex[i].x)
			mesh.x_min = mesh.list_vertex[i].x;

		if (mesh.y_min > mesh.list_vertex[i].y)
			mesh.y_min = mesh.list_vertex[i].y;

		if (mesh.z_min > mesh.list_vertex[i].z)
			mesh.z_min = mesh.list_vertex[i].z;
	}
/*
	cout << "X Max: " << mesh.x_max << "\t";
	cout << "X Min: " << mesh.x_min << endl;

	cout << "Y Max: " << mesh.y_max << "\t";
	cout << "Y Min: " << mesh.y_min << endl;

	cout << "Z Max: " << mesh.z_max << "\t";
	cout << "Z Min: " << mesh.z_min << endl;
*/
}

void resize_mesh (Mesh& mesh)
{
	float delta_x, delta_y, delta_z, delta_resize;

	bounding_box (mesh);

	delta_x = (mesh.x_max + mesh.x_min) / 2;
	delta_y = (mesh.y_max + mesh.y_min) / 2;
	delta_z = (mesh.z_max + mesh.z_min) / 2;


	if (delta_x < delta_y && delta_z < delta_y)
		delta_resize = delta_y;

	if (delta_y < delta_x && delta_z < delta_x)
		delta_resize = delta_x;

	if (delta_x < delta_z && delta_y < delta_z)
		delta_resize = delta_z;
/*
	cout << "Delta X: " << delta_x << endl;
	cout << "Delta Y: " << delta_y << endl;
	cout << "Delta Z: " << delta_z << endl;
	cout << "Delta Resize: " << delta_resize << endl;
*/
	delta_x /= delta_resize;
	delta_y /= delta_resize;
	delta_z /= delta_resize;
/*
	cout << "Delta X: " << delta_x << endl;
	cout << "Delta Y: " << delta_y << endl;
	cout << "Delta Z: " << delta_z << endl;
*/
	for (int i = 0; i < mesh.numero_vertex; ++i)
	{
		mesh.list_vertex[i].x /= delta_resize;
		mesh.list_vertex[i].x -= delta_x;

		mesh.list_vertex[i].y /= delta_resize;
		mesh.list_vertex[i].y -= delta_y;

		mesh.list_vertex[i].z /= delta_resize;
		mesh.list_vertex[i].z -= delta_z;
	}

	float x_1, x_2, x_n;
	float y_1, y_2, y_n;
	float z_1, z_2, z_n;
	float norma;

	for (int i = 0; i < mesh.numero_face; ++i)
	{
		x_1 = mesh.list_vertex[mesh.list_faces[i].vertex_1].x - mesh.list_vertex[mesh.list_faces[i].vertex_2].x;
		y_1 = mesh.list_vertex[mesh.list_faces[i].vertex_1].y - mesh.list_vertex[mesh.list_faces[i].vertex_2].y;
		z_1 = mesh.list_vertex[mesh.list_faces[i].vertex_1].z - mesh.list_vertex[mesh.list_faces[i].vertex_2].z;

		x_2 = mesh.list_vertex[mesh.list_faces[i].vertex_2].x - mesh.list_vertex[mesh.list_faces[i].vertex_3].x;
		y_2 = mesh.list_vertex[mesh.list_faces[i].vertex_2].y - mesh.list_vertex[mesh.list_faces[i].vertex_3].y;
		z_2 = mesh.list_vertex[mesh.list_faces[i].vertex_2].z - mesh.list_vertex[mesh.list_faces[i].vertex_3].z;

		x_n = ((y_1 * z_2) - (y_2 * z_1));
		y_n = ((z_1 * x_2) - (z_2 * x_1));
		z_n = ((x_1 * y_2) - (x_2 * y_1));

		norma = (float) sqrt(pow(x_n, 2) + pow(y_n, 2) + pow(z_n, 2));

		mesh.list_faces[i].normal_x = x_n / norma;
		mesh.list_faces[i].normal_y = y_n / norma;
		mesh.list_faces[i].normal_z = z_n / norma;
	}

	return ;
}

void render_mesh (const Mesh& mesh)
{
	glBegin (GL_TRIANGLES);

	for (int i = 0; i < mesh.numero_face; ++i)
	{
		glVertex3f(
			mesh.list_vertex[mesh.list_faces[i].vertex_1].x,
			mesh.list_vertex[mesh.list_faces[i].vertex_1].y,
			mesh.list_vertex[mesh.list_faces[i].vertex_1].z
			); //vertex_1

		glVertex3f(
			mesh.list_vertex[mesh.list_faces[i].vertex_2].x,
			mesh.list_vertex[mesh.list_faces[i].vertex_2].y,
			mesh.list_vertex[mesh.list_faces[i].vertex_2].z
			); //vertex_2
/*
		glVertex3f(
			mesh.list_vertex[mesh.list_faces[i].vertex_2].x,
			mesh.list_vertex[mesh.list_faces[i].vertex_2].y,
			mesh.list_vertex[mesh.list_faces[i].vertex_2].z
			); //vertex_2

		glVertex3f(
			mesh.list_vertex[mesh.list_faces[i].vertex_3].x,
			mesh.list_vertex[mesh.list_faces[i].vertex_3].y,
			mesh.list_vertex[mesh.list_faces[i].vertex_3].z
			); //vertex_3
*/
		glVertex3f(
			mesh.list_vertex[mesh.list_faces[i].vertex_3].x,
			mesh.list_vertex[mesh.list_faces[i].vertex_3].y,
			mesh.list_vertex[mesh.list_faces[i].vertex_3].z
			); //vertex_3
/*
		glVertex3f(
			mesh.list_vertex[mesh.list_faces[i].vertex_1].x,
			mesh.list_vertex[mesh.list_faces[i].vertex_1].y,
			mesh.list_vertex[mesh.list_faces[i].vertex_1].z
			); //vertex_1
*/
		glNormal3f(
			mesh.list_faces[i].normal_x,
			mesh.list_faces[i].normal_y,
			mesh.list_faces[i].normal_z
			); //normal
	}
	glEnd();

	return ;
}
