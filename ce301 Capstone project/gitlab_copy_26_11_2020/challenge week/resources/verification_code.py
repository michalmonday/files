# this code was used to verify how effective MST would be when
# used as an optimal path when starting from any vertex in:
# https://www.dwu.ac.pg/en/images/All_Attachements/Research%20Journals/vol_12/2010-V12-1_Fitina_et_al_spanning_trees_for_travel_planning.pdf

# It's described further in the Project intro document, it was a part
# of background reading.

from itertools import permutations

def floyd_warshall(graph):
    ''' This function was copied from:
        https://www.geeksforgeeks.org/floyd-warshall-algorithm-dp-16/  '''
    V = len(graph)
    dist = [*map(lambda i : [*map(lambda j : j , i)], graph)]
    for k in range(V): 
        for i in range(V): 
            for j in range(V): 
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]) 
    return dist

def get_path_length(path, optimal_distances):
    ''' optimal_distances is the result of Floyd Warshall algorithm. '''
    length = 0
    current_node = path[0]
    for next_node in path[1:]:
        length += optimal_distances[current_node][next_node]
        current_node = next_node
    return length

def get_n_best_paths_starting_from(n, starting_node, nodes, optimal_distances):
    ''' It checks all possible permutations. '''
    nodes_to_visit = [*range(len(nodes))]
    lengths = [999999]
    paths = [ [] ]
    for path in permutations(nodes_to_visit, len(nodes_to_visit)):
        if nodes[path[0]] != starting_node:
            continue
        length = get_path_length(path, optimal_distances)  
        if length < lengths[-1]:
            for i, L in enumerate(lengths):
                if length < L:
                    lengths.insert(i, length)
                    paths.insert(i, path)
                    if len(paths) > n:
                        del paths[-1]
                        del lengths[-1]
                    break
    return lengths, paths

INF  = 99999
nodes = ['KAV', 'MAS', 'HKN', 'MAD', 'POM', 'RAB', 'LAE', 'BUA']

# data from Figure 7 of: https://www.dwu.ac.pg/en/images/All_Attachements/Research%20Journals/vol_12/2010-V12-1_Fitina_et_al_spanning_trees_for_travel_planning.pdf
graph = [
    # KAV, MAS, HKN, MAD, POM, RAB, LAE, BUA
    [   0, 494, 600, 738, INF, 420, INF, INF], # KAV
    [ 494,   0, INF, INF, 766, INF, INF, INF], # MAS
    [ 600, INF,   0,1054, 603, 415, 521, INF], # HKN
    [ 738, INF,1054,   0, 567, 901, 910, INF], # MAD
    [ INF, 766, 603, 567,   0, 758, 459, 922], # POM
    [ 420, INF, 415, 901, 758,   0, 677, 474], # RAB 
    [ INF, INF, 521, 910, 459, 677,   0, INF], # LAE
    [ INF, INF, INF, INF, 922, 474, INF,   0]  # BUA
    ]


mst = [
    # KAV, MAS, HKN, MAD, POM, RAB, LAE, BUA
    [   0, 494, INF, INF, INF, 420, INF, INF], # KAV
    [ 494,   0, INF, INF, INF, INF, INF, INF], # MAS
    [ INF, INF,   0, INF, INF, 415, 521, INF], # HKN
    [ INF, INF, INF,   0, 567, INF, INF, INF], # MAD
    [ INF, INF, INF, 567,   0, INF, 459, INF], # POM
    [ 420, INF, 415, INF, INF,   0, INF, 474], # RAB 
    [ INF, INF, 521, INF, 459, INF,   0, INF], # LAE
    [ INF, INF, INF, INF, INF, 474, INF,   0]  # BUA
    ]

optimal_distances = floyd_warshall(graph)
mst_distances = floyd_warshall(mst)

for starting_node in nodes:
    # get total distance of mst (including travel back)
    mst_lengths, mst_paths = get_n_best_paths_starting_from(1, starting_node, nodes, mst_distances)
    mst_length = mst_lengths[0]
    mst_path = mst_paths[0]

    print(f'MST starting from {starting_node} results in the following distances:')
    print(mst_length, ' - '.join(nodes[i] for i in mst_path))
    print()

    lengths, paths = get_n_best_paths_starting_from(10, starting_node, nodes, optimal_distances)

    print(f'MST is {int(mst_length/lengths[0]*100)}% of optimal path.')
    print()

    print('Top 10 distances:')
    
    for length, path in zip(lengths, paths):
        print(length, ' - '.join(nodes[i] for i in path))        
    print()
    print()
    print()

#for node, distances in zip(nodes, optimal_distances):
#    print (node, distances)
