#ifndef MAP_H
#define MAP_H

#include <string>

using namespace std;

typedef struct _Map
{
	int numero_de_linhas;
	int numero_de_colunas;
	int** position;
}Map;

Map* new_map ();
void del_map (Map* mapa);
int load_map (Map& mapa, const string& map_file_name);
void render_map (const Map& mapa);

#endif