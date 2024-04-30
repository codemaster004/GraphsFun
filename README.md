# Graphs and funny stuff
class **Graph**
- `set<Edge> t_edges`
- `array<list<Vertex>> t_vertices`

## Degree sequence
can be obtained from `t_vertices` length of lists with neighbours

## Number of components
```python
component_count = 0
visits = VisitingStruct()
for vertex in graph:
    if not visits.is_visited(vertex):
        component_count += 1
        dfs(graph, vertex, visits)
```

## Bipartite
```python
is_bipartite = True
# Remember to run BFS for every Component
bfs_queue = Queue()
while not bfs_queue.is_empty():
    vertex = bfs_queue.pop()
    neighbours = vertex.get_neighbours()
    bfs_queue.extend(neighbours)
    for neighbour in neighbours:
        if not neighbour.is_colored():
             neighbour.color = other_color(vertex.color)
        elif neighbour.color == vertex.color:
            is_bipartite = False
```
