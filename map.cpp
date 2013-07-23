#include "map.h"
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

	cout << mapa.numero_de_linhas << " ";
	cout << mapa.numero_de_colunas << endl;

	for (int i = 0; i < mapa.numero_de_linhas; ++i)
	{
		for (int j = 0; j < mapa.numero_de_colunas; ++j)
		{
			cout << mapa.position[i][j] << " ";
		}
		cout << endl;
	}

	return 0;
}
