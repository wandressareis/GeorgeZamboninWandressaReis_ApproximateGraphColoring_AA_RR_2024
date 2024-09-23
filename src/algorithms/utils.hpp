#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <set>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

using namespace std;

void print_arr(int* v, int n);
int* read_numbers_file(char file_name[], int n);
int count_file_lines(char file_name[]);
void get_path_filename(char* buffer, char* path);
int get_filename_n(char* filename);
char get_filename_order(char* filename);
void write_csv(char* algorithm_name, char* graph_filename, int color_count, unsigned long long duration);
void readGraph(const string& filename, vector<vector<int>>& graph, int& numVertices, int& numEdges);