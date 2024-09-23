#include "utils.hpp"

void print_arr(int* v, int n) {
  int i;
  for(i = 0; i < n; i++) printf("%d ", v[i]);
  printf("\n");
}

int* read_numbers_file(char file_name[], int n) {
  int* numbers = (int*)malloc(n * sizeof(int));

  FILE *arq;
  char Linha[100];
  char *result;
  int i;

  arq = fopen(file_name, "rt");

  if (arq == NULL) {
    printf("Problemas na abertura do arquivo\n");
    return NULL;
  }

  i = 1;
  while (!feof(arq)) {
	// Lê uma linha (inclusive com o '\n')
    result = fgets(Linha, 100, arq);  // o 'fgets' lê até 99 caracteres ou até o '\n'
    if (result)  // Se foi possível ler
        // printf("%d ", atoi(Linha));
	    numbers[i-1] = atoi(Linha);
    i++;
  }
  fclose(arq);
  return numbers;
}

void get_path_filename(char* buffer, char* path) {
  int i;
  int filename_i = 0;
  for(i = 0; path[i] != '\0'; i++) if (path[i] == '/') filename_i = i+1;

  for(i = 0; path[i]; i++) buffer[i] = path[filename_i + i];
  buffer[i] = '\0';
}

int get_filename_n(char* filename) {
  int i = 0;
  char str_n[100];
  while (1) {
    if (filename[i] == '\0'){
      printf("error: invalid numbers sequence filename.\n");
      exit(-1);
    }
    if (filename[i++] == '-') break;
  }
  int j = 0;
  while (1) {
    str_n[j++] = filename[i];
    if (filename[i] == '\0'){
      printf("error: invalid numbers sequence filename.\n");
      exit(-1);
    }
    if (filename[i++] == '.') break;
  }
  return atoi(str_n);
}

char get_filename_order(char* filename) {
  if (filename[1] != '-') {
      printf("error: invalid numbers sequence filename.\n");
      exit(-1);
  }
  return filename[0];
}

void write_csv(char* algorithm_name, char* graph_filename, int color_count, unsigned long long duration) {
  char filename[] = "log.csv";
  FILE* csv;
  if (!(access("log.csv", F_OK) != -1)) {
    csv = fopen(filename, "w");
    fprintf(csv, "algorithm_name,graph_filename,color_count,duration_ns\n");
  } else {
    csv = fopen(filename, "a");
  }
  fprintf(csv, "%s,%s,%d,%llu\n", algorithm_name, graph_filename, color_count, duration);
  fclose(csv);
}

// Function to read the graph from .col file
void readGraph(const string& filename, vector<vector<int>>& graph, int& numVertices, int& numEdges) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open the file!" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line[0] == 'p') {
            stringstream ss(line);
            string tmp;
            ss >> tmp >> tmp >> numVertices >> numEdges;
            graph.resize(numVertices);
        }
        else if (line[0] == 'e') {
            stringstream ss(line);
            char edge;
            int u, v;
            ss >> edge >> u >> v;
            u--; v--; // Convert to zero-indexed
            graph[u].push_back(v);
            graph[v].push_back(u);
        }
    }
}
