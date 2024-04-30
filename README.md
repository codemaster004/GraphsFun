class **Graph**
- `set<Edge> t_edges`
- `array<list<Vertex>> t_vertices`

## Degree sequence
can be obtained from `t_vertices` length of lists with neighbours

## Number of components
Checking for count of unconnected sub-graphs in graph is as simple as running DFS in order to mark vertices as **visited** for a vertex, then checking all other vertices if a unvisited vertex is found update count, run DFS again and continue checking.
```python
component_count = 0
visits = SomeStruct()
for vertex in graph:
	if not is_visited(visits, vertex):
		component_count += 1
		dfs(graph, vertex, visits)
print(component_count)
```

## Bipartite
This just is a standard BFS implementation with added check for colour of a vertex. If an uncoloured vertex is found, it is assigned a colour opposite to the colour of current vertex. If a coloured vertex is found, we need to make sure it is opposite colour of current vertex.

```python  
is_bipartite = True  
  
# Remember to run BFS for every Component  
bfs_queue = Queue()  
while not bfs_queue.is_empty():  
    vertex = bfs_queue.pop()
    for neighbour in vertex.get_neighbours():
	    if not neighbour.is_colored(): # treat is_colored as visited 
		    bfs_queue.push(neighbour)
		    neighbour.color = other_color(vertex.color)
		elif neighbour.color == vertex.color:
			is_bipartite = False
print(is_bipartite)  
```  

## Eccentricity of vertices
Run BFS for a given vertex with every new vertex found assign it a distance of one larger than a distance of current vertex. Return a maximal distance.
```python  
distance = { start_vertex: 0 } # Using a hashMap here since I'm lazy  

# This is just for single vertex.  
# If needed can be run for all vertices  
bfs_queue = Queue()
bfs_queue.push(start_vertex)
while not bfs_queue.is_empty():
    vertex = bfs_queue.pop()
    vertex_distance = distance[vertex]
    for neighbour in vertex.get_neighbours():
        if neighbour not in distance: # treat distance as visited
            distance[neighbour] = vertex_distance+1
print(max(disctance.values()))
```