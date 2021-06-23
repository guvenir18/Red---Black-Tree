
/*
    İlke Anıl Güvenir
    150180042
    Note: Please compile with c++11
*/


#include <iostream>
#include <string>
#include <fstream>

using namespace std;


// RED : true, BLACK : false
struct Node {
    string name;
    string team;
    string season;
    int rebound;
    int point;
    int assist;
    bool color;
    int key;
    Node* left;
    Node* right;
    Node* parent;
    Node();
    Node(string);
    Node(string, string, string, int, int, int);
};

Node::Node(string name, string team, string season, int rebound, int point, int assist) {
    this->name = name;
    this->team = team;
    this->season = season;
    this->rebound = rebound;
    this->point = point;
    this->assist = assist;
    left = nullptr;
    right = nullptr;
    parent = nullptr;
    color = true;
}

Node::Node(string name) {
    this->name = name;
    left = nullptr;
    right = nullptr;
    parent = nullptr;
    color = true;
}

struct RBTree {
    Node* root;
    Node* max_point;
    Node* max_rebound;
    Node* max_assist;
    Node* searchNode(string);
    void rotateRight(Node*);
    void rotateLeft(Node*);
    RBTree();
    ~RBTree();
    void insert(Node*);
    void deleteTree(Node*);
    void fixViolation(Node*);
    bool getColor(Node*);
    void printPreorder(Node*, int);
};

//At first, I was not going to implement this function to make fewer function calls but I need this to get color of a nullptr.
bool RBTree::getColor(Node* pt) {
    if (pt == nullptr) {
        return false;
    }
    return pt->color;
}

void RBTree::printPreorder(Node* pt, int depth) {
    if (pt == nullptr) {
        return;
    }
    string color = pt->color ? "RED" : "BLACK";
    for(int i = 0; i < depth; i++) {
        cout << "-";
    }
    cout << "(" << color << ") " << pt->name << endl;
    depth++;
    printPreorder(pt->left, depth);
    printPreorder(pt->right, depth);
}

// Delete tree in postorder.
void RBTree::deleteTree(Node* start) {
    if(start == nullptr) {
        return;
    }
        deleteTree(start->left);
        deleteTree(start->right);
        delete start;
}

RBTree::RBTree() {
    root = nullptr;
    max_point = root;
    max_assist = root;
    max_rebound = root;
}

// Destructor calls deleteTree.
RBTree::~RBTree() {
    deleteTree(root);
}

void RBTree::rotateLeft(Node* pt) {
    Node* temp = pt->right;
    pt->right = temp->left;
    if (pt->right != nullptr) {
        pt->right->parent = pt;
    }
    temp->parent = pt->parent;
    if (pt->parent == nullptr) {
        root = temp;
    } else if (pt->parent->left == pt) {
        pt->parent->left = temp;
    } else {
        pt->parent->right = temp;
    }
    temp->left = pt;
    pt->parent = temp;
}

void RBTree::rotateRight(Node* pt) {
    Node *temp = pt->left;  
    pt->left = temp->right;  
    if (pt->left != nullptr) {
        pt->left->parent = pt; 
    } 
    temp->parent = pt->parent; 
    if (pt->parent == nullptr) {
        root = temp; 
    } else if (pt == pt->parent->left) {
        pt->parent->left = temp; 
    } else {
        pt->parent->right = temp; 
    }
    temp->right = pt; 
    pt->parent = temp; 
}

void RBTree::insert(Node* newNode) {
    if (root == nullptr) {
        newNode->color = false;
        root = newNode;
        max_point = root;
        max_rebound = root;
        max_assist = root;
        return;
    }
    Node* temp = root;
    Node* temp2 = root;
    while(temp != nullptr) {
        if (newNode->name.compare(temp->name) >= 0)  {
            temp2 = temp;
            temp = temp->right;
        } else {
            temp2 = temp;
            temp = temp->left;
        }
    }
    if (newNode->name.compare(temp2->name) >= 0) {
        temp2->right = newNode;
        newNode->parent = temp2;
        fixViolation(newNode);
        return;
    } else {
        temp2->left = newNode;
        newNode->parent = temp2;
        fixViolation(newNode);
        return;
    }
}

Node* RBTree::searchNode(string search) {
    Node* temp = root;
    while(temp != nullptr && search != temp->name) {
        if ((search.compare(temp->name) >= 0))  {
            temp = temp->right;
        } else {
            temp = temp->left;
        }
    }
    return temp;
}

void RBTree::fixViolation(Node* pt) {
    Node *parent = nullptr;
    Node *grandparent = nullptr;
    while (pt != root && getColor(pt) == true && getColor(pt->parent) == true) {
        parent = pt->parent;
        grandparent = parent->parent;
        if (parent == grandparent->left) {
            Node *uncle = grandparent->right;
            if (getColor(uncle) == true) {
                uncle->color = false;
                parent->color = false;
                grandparent->color = true;
                pt = grandparent;
            } else {
                if (pt == parent->right) {
                    rotateLeft(parent);
                    pt = parent;
                    parent = pt->parent;
                }
                rotateRight(grandparent);
                swap(parent->color, grandparent->color);
                pt = parent;
            }
        } else {
            Node *uncle = grandparent->left;
            if (getColor(uncle) == true) {
                uncle->color = false;
                parent->color = false;
                grandparent->color = true;
                pt = grandparent;
            } else {
                if (pt == parent->left) {
                    rotateRight(parent);
                    pt = parent;
                    parent = pt->parent;
                }
                rotateLeft(grandparent);
                swap(parent->color, grandparent->color);
                pt = parent;
            }
        }
    }
    root->color = false;
}

int main(int argc, char* argv[]) {
    RBTree myTree;
    ifstream file;
    char* filename = argv[1]; // Read file from command line
    file.open(filename);
    string line;
    getline(file, line); // to read first line
    string name, team, season, s_rebound, s_point, s_assist;
    int rebound, point, assist;
    string current_season = "2000-2001";
    for(int i = 0; i < 6873; i++) {
        getline(file, season, ',');
        if (season.compare(current_season) != 0) {
            cout << "End of the " << current_season << " Season" << endl;
            cout << "Max Points: " << myTree.max_point->point << " - Player Name: " << myTree.max_point->name << endl;
            cout << "Max Assists: " << myTree.max_assist->assist << " - Player Name: " << myTree.max_assist->name << endl;
            cout << "Max Rebs: " << myTree.max_rebound->rebound << " - Player Name: " << myTree.max_rebound->name << endl;
            if (current_season == "2000-2001") {
                int depth = 0;
                myTree.printPreorder(myTree.root, depth);
            }
            current_season = season;
        }
        getline(file, name, ',');
        getline(file, team, ',');
        getline(file, s_rebound, ',');
        getline(file, s_assist, ',');
        getline(file, s_point);
        rebound = stoi(s_rebound);
        point = stoi(s_point);
        assist = stoi(s_assist);
        Node* check = myTree.searchNode(name);
        if (check == nullptr) {
            Node* newnode = new Node(name, team, season, rebound, point, assist);
            myTree.insert(newnode);
            if (newnode->rebound > myTree.max_rebound->rebound) {
                myTree.max_rebound = newnode;
            }
            if (newnode->assist > myTree.max_assist->assist) {
                myTree.max_assist = newnode;
            }
            if (newnode->point > myTree.max_point->point) {
                myTree.max_point= newnode;
            }
        } else {
            check->assist += assist;
            check->rebound += rebound;
            check->point += point;
            if (check->rebound > myTree.max_rebound->rebound) {
                myTree.max_rebound = check;
            }
            if (check->assist > myTree.max_assist->assist) {
                myTree.max_assist = check;
            }
            if (check->point > myTree.max_point->point) {
                myTree.max_point = check;
            }
        }
    }
        cout << "End of the " << current_season << " Season" << endl;
        cout << "Max Points: " << myTree.max_point->point << " - Player Name: " << myTree.max_point->name << endl;
        cout << "Max Assists: " << myTree.max_assist->assist << " - Player Name: " << myTree.max_assist->name << endl;
        cout << "Max Rebs: " << myTree.max_rebound->rebound << " - Player Name: " << myTree.max_rebound->name << endl;
    return 0;
}