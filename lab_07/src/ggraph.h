#ifndef _GGRAPH_H_
#define _GGRAPH_H_

void free_graph(int **graph, int node);
int added_new_route(int **graph, int size, int route_len, int node, int max_len);
int find_len(int **graph, int size, int num, int len);
int input_graph(int **graph, int node, int route);
void print_graph(int **graph, int size, int fl, int *a);

#endif // _GGRAPH_H_ 