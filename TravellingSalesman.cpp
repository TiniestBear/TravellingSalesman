// Made by Michał Wiercioch

#include "TravellingSalesman.h"

using namespace std;

struct Node {
	int id;
	vector<int> distances;
	Node(int id, vector<int> distances) : id(id), distances(distances) {}
	friend bool operator<(const Node& lhs, const Node& rhs) { return lhs.id < rhs.id; }
};

void getMinDistance(std::vector<Node>& nodes, int graphSize, Node& firstNode, int& minDist, string& minRoute)
{
	do {
		int dist = 0;
		int i = 0;
		string route = "1";
		for (; i < graphSize - 1; ++i) {
			if (!i) {
				dist += firstNode.distances[nodes[i].id - 2];
				route += to_string(nodes[i].id);
			}
			else
			{
				if (nodes[i - 1].id < nodes[i].id) {
					dist += nodes[i - 1].distances[nodes[i].id - 2];
					route += to_string(nodes[i].id);
				}
				else {
					dist += nodes[i - 1].distances[nodes[i].id - 1];
					route += to_string(nodes[i].id);
				}
			}
		}
		dist += nodes[i - 1].distances[0];
		route += "1";
		if (minDist == 0 || minDist > dist) {
			minDist = dist;
			minRoute = route;
		}
	} while (next_permutation(nodes.begin(), nodes.end()));
}

void addNode(Node& firstNode, vector<Node>& nodes, vector<int> distances) {
	firstNode.distances.push_back(distances[0]);
	for (auto& node : nodes) {
		node.distances.push_back(distances[node.id - 1]);
	}
	nodes.push_back(Node(distances.size() + 1, distances));
}

void getNode(Node& firstNode, vector<Node>& nodes) {
	cout << firstNode.id << " { ";
	for (auto dist : firstNode.distances) {
		cout << dist << " ";
	}
	cout << "} " << endl;
	for (auto& node : nodes) {
		cout << node.id << " { ";
		for (auto dist : node.distances) {
			cout << dist << " ";
		}
		cout << "}" << endl;
	}
}

void delNode(Node& firstNode, vector<Node>& nodes, int nodeId, int& minDist) {
	if (nodeId == 1) {
		firstNode = nodes[0];
		firstNode.id = 1;
		firstNode.distances.erase(firstNode.distances.begin());
		nodes.erase(nodes.begin());
		for (auto& node : nodes) {
			--node.id;
			node.distances.erase(node.distances.begin());
		}
	}
	else {
		int posToDel = (nodeId - 2);
		firstNode.distances.erase(firstNode.distances.begin() + posToDel);
		for (auto it = nodes.begin(); it != nodes.begin() + posToDel; ++it) {
			it->distances.erase(it->distances.begin() + posToDel);
		}
		if (nodes.begin() + posToDel != nodes.end()) {
			for (auto it = nodes.begin() + (nodeId - 1); it != nodes.end(); ++it) {
				--it->id;
				it->distances.erase(it->distances.begin() + (nodeId - 1));
			}
		}
		nodes.erase(nodes.begin() + posToDel);
	}
	minDist = 0;
	getNode(firstNode, nodes);
}

void printHelp() {
	cout
		<< "This program was developed strictly to solve walking salesman problem." << endl
		<< "By default every loop will perform recalculation of shortest route." << endl
		<< "Also by default You are in addition mode, so given that we start with 4 nodes" << endl
		<< "to add more nodes to the path we need to specify the distances to all previously existing nodes." << endl
		<< "Currenly system only accepts space separated strings with integers." << endl
		<< "For example to add 5th node type in:" << endl
		<< "10 10 10 10" << endl
		<< "this will create node equidistant from all previously existing ones." << endl
		<< "To remove existing nodes type in del" << endl
		<< "Data in del is presented in the following way:" << endl
		<< "ID {Array of distances to other nodes}" << endl
		<< "For example ID=2 and {2 7 6} means that node number 3 is connected to three other nodes:" << endl
		<< "node id=1 distant by 2, node id=3 distant by 7 and node id=4 distant by 6." << endl
		<< "Node self id is ommited. Deletion of nodes will cause remaining ones to renumerate to maintain continuousness." << endl
		<< "Afterwards You can choose the id of node You wish deleted." << endl
		<< "To exit type in esc or exit" << endl;
}

int main()
{
	Node firstNode = Node(1, { 10, 15, 20 });
	vector<Node> nodes = { Node(2, {10, 35, 25}), Node(3, {15, 35, 30}), Node(4, {20, 25, 30}) };
	int graphSize = nodes.size() + 1;
	int minDist = 0;
	string minRoute = "";
	string userInput;
	string num = "";
	vector<int> distances;

	cout << "Hello traveller." << endl;

	printHelp();

	while (true) {
		try {
			getMinDistance(nodes, graphSize, firstNode, minDist, minRoute);
			cout << "Current minimum distance " << minDist << endl;
			cout << "Current shortest route " << minRoute << endl;
			cout << "Awaiting user input." << endl;
			cout << "To add new node please provide " << graphSize << " space separated integers." << endl;
			cout << "To remove node please type in del." << endl;
			cout << "To see help type in help." << endl;
			cout << "To exit type in exit or esc." << endl << endl;
			userInput = "";
			distances = {};

			std::getline(std::cin, userInput);

			if (userInput == "esc" || userInput == "exit") {
				break;
			}

			if (userInput == "help" || userInput == "h" || userInput == "?") {
				printHelp();
				continue;
			}

			if (userInput == "del") {
				if (graphSize > 2) {
					getNode(firstNode, nodes);
					userInput = "";
					cout << "Select node to delete: ";
					std::getline(std::cin, userInput);

					delNode(firstNode, nodes, stoi(userInput), minDist);
					--graphSize;
					continue;
				}
				else {
					cout << "Graph to small" << endl;
					continue;
				}
			}

			num = "";
			for (auto c : userInput) {
				if (c == 32) {
					distances.push_back(stoi(num));
					num = "";
					continue;
				}
				else {
					num += c;
				}
			}

			if (num != "") {
				distances.push_back(stoi(num));
			}

			if (distances.size() == graphSize) {
				addNode(firstNode, nodes, distances);
				++graphSize;
			}
			else {
				if (distances.size() > graphSize) {
					cout << "Too many paths, nothing added." << endl;
				}
				else {
					cout << "Too few paths, nothing added." << endl;
				}
			}
			cout << "-------------------------" << endl;
		}
		catch (const std::exception& e) {
			cout << e.what() << " thrown" << endl;
		}
	}

	return 0;
}
