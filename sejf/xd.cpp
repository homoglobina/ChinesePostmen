

    vector<pair<int,int>> eulerCycle3;

    cout << "\nEuler3" << ": ";
    for (const auto& edge : eulerCycle) {
        cout << "test";
        if (edge.second == -1){
            cout << "test2";
            auto parent = dijkstra3(edge.second, localAdjMatrix, vertices);
            auto shortestPath = reconstructPath(edge.second, edge.first, parent);
            for (const auto& edge2 : shortestPath) {
                eulerCycle3.emplace_back(edge2);


            }
        }
        else{
            eulerCycle3.emplace_back(edge);
        }
    }

    for (const auto& edge : eulerCycle3) {
        cout << "(" << edge.second << ", " << edge.first << ") ";
    }
