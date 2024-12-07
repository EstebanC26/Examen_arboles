#include <iostream>

using namespace std;

template <typename T>
class AVLTree {
private:
    struct Node {
        T data;
        Node* left;
        Node* right;
        int height;

        Node(const T& data) : data(data), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    int height(Node* node) {
        return node ? node->height : 0;
    }

    int balanceFactor(Node* node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    void updateHeight(Node* node) {
        if (node) {
            node->height = 1 + max(height(node->left), height(node->right));
        }
    }

    int max(int a, int b) {
        return (a > b) ? a : b;
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        updateHeight(y);
        updateHeight(x);

        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        updateHeight(x);
        updateHeight(y);

        return y;
    }

    Node* balance(Node* node) {
        updateHeight(node);

        int bf = balanceFactor(node);

        if (bf > 1) {
            if (balanceFactor(node->left) < 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }

        if (bf < -1) {
            if (balanceFactor(node->right) > 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }

        return node;
    }

    Node* insert(Node* node, const T& data) {
        if (!node) return new Node(data);

        if (data < node->data) {
            node->left = insert(node->left, data);
        } else if (data > node->data) {
            node->right = insert(node->right, data);
        } else {
            return node;
        }

        return balance(node);
    }

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    Node* remove(Node* node, const T& data) {
        if (!node) return node;

        if (data < node->data) {
            node->left = remove(node->left, data);
        } else if (data > node->data) {
            node->right = remove(node->right, data);
        } else {
            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;
                if (!temp) {
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }
                delete temp;
            } else {
                Node* temp = minValueNode(node->right);
                node->data = temp->data;
                node->right = remove(node->right, temp->data);
            }
        }

        if (!node) return node;

        return balance(node);
    }

    void inOrder(Node* node) const {
        if (node) {
            inOrder(node->left);
            cout << node->data << " ";
            inOrder(node->right);
        }
    }

    void printTree(Node* node, int space) const {
        if (node == nullptr) return;

        space += 10;

        printTree(node->right, space);

        cout << endl;
        for (int i = 10; i < space; i++) {
            cout << " ";
        }
        cout << node->data << "\n";

        printTree(node->left, space);
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(const T& data) {
        root = insert(root, data);
    }

    void remove(const T& data) {
        root = remove(root, data);
    }

    void inOrder() const {
        inOrder(root);
        cout << endl;
    }

    void printTree() const {
        printTree(root, 0);
    }
};

void mostrarMenu() {
    cout << "Seleccione una opcion:" << endl;
    cout << "1. Insertar un nodo" << endl;
    cout << "2. Eliminar un nodo" << endl;
    cout << "3. Mostrar recorrido en orden" << endl;
    cout << "4. Visualizar el arbol" << endl;
    cout << "5. Salir" << endl;
}

int main() {
    AVLTree<int> tree;
    int opcion, valor;

    do {
        mostrarMenu();
        cout << "Ingrese su opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "Ingrese el valor a insertar: ";
                cin >> valor;
                tree.insert(valor);
                cout << "Valor insertado." << endl;
                break;
            case 2:
                cout << "Ingrese el valor a eliminar: ";
                cin >> valor;
                tree.remove(valor);
                cout << "Valor eliminado." << endl;
                break;
            case 3:
                cout << "Recorrido en orden del arbol AVL:" << endl;
                tree.inOrder();
                break;
            case 4:
                cout << "Visualizacion del arbol AVL:" << endl;
                tree.printTree();
                break;
            case 5:
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo." << endl;
                break;
        }
    } while (opcion != 5);

    return 0;
}