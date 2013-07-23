#include "map.h"
#include "game.h"
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

Map* new_map()
{
	Map *map_pointer;
	map_pointer = (Map *) malloc (sizeof (Map));

	if (map_pointer == NULL)
		return NULL;

	map_pointer->numero_de_linhas = 0;
	map_pointer->numero_de_colunas = 0;
	map_pointer->position = NULL;

	return map_pointer;
}

void del_map (Map* mapa)
{
	if (mapa != NULL)
	{
		for (int i = 0; i < mapa->numero_de_linhas; ++i)
		{
			free (mapa->position[i]);
		}
		free (mapa->position);
	}

	return ;
}

int load_map(Map& mapa, const string& mapa_file_name)
{
	ifstream mapa_txt (mapa_file_name.c_str());	

	if (mapa_txt.is_open())
	{
		mapa_txt >> mapa.numero_de_linhas;
		mapa_txt >> mapa.numero_de_colunas;

		mapa.position = (int **) malloc (sizeof (int *) * mapa.numero_de_linhas);
		for (int i = 0; i < mapa.numero_de_linhas; ++i)
		{
			mapa.position[i] = (int *) malloc (sizeof (int) * mapa.numero_de_colunas);
		}

		for (int i = 0; i < mapa.numero_de_linhas; ++i)
		{
			for (int j = 0; j < mapa.numero_de_colunas; ++j)
			{
				mapa_txt >> mapa.position[i][j];
			}
		}
		mapa_txt.close();
	}
	else
	{
		return -1;
	}

	return 0;
}

void render_map (const Map& mapa)
{
	glClear (GL_COLOR_BUFFER_BIT);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glTranslatef (854 / 2.0, 480 / 2.0, 0.0);

	glBegin (GL_QUADS);
	
	glColor3f (0.0, 1.0, 1.0);
	
	glVertex2f (-5.0, -5.0);
	glVertex2f (5.0, -5.0);
	glVertex2f (5.0, 5.0);
	glVertex2f (-5.0, 5.0);
	
	glEnd();
}
