// C++ Program to Implement AVL Tree
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <strstream>
#include <sys/wait.h>
#include <zmq.hpp>
using namespace std;

struct NodeInfo {
    int id;             // Идентификатор узла
    int pid;            // PID процесса
    std::string endpoint; // Адрес ZeroMQ
};


template <typename T> class AVLNode {
public:
    T key;
    AVLNode* left;
    AVLNode* right;
    int height;


    AVLNode(T k)
        : key(k)
        , left(nullptr)
        , right(nullptr)
        , height(1)
    {
    }
};


template <typename T> class AVLTree {
private:

    AVLNode<T>* root;


    int height(AVLNode<T>* node)
    {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    int balanceFactor(AVLNode<T>* node)
    {
        if (node == nullptr)
            return 0;
        return height(node->left) - height(node->right);
    }


    AVLNode<T>* rightRotate(AVLNode<T>* y)
    {
        AVLNode<T>* x = y->left;
        AVLNode<T>* T2 = x->right;


        x->right = y;
        y->left = T2;


        y->height
            = max(height(y->left), height(y->right)) + 1;
        x->height
            = max(height(x->left), height(x->right)) + 1;


        return x;
    }


    AVLNode<T>* leftRotate(AVLNode<T>* x)
    {
        AVLNode<T>* y = x->right;
        AVLNode<T>* T2 = y->left;

        y->left = x;
        x->right = T2;


        x->height
            = max(height(x->left), height(x->right)) + 1;
        y->height
            = max(height(y->left), height(y->right)) + 1;


        return y;
    }


    AVLNode<T>* insert(AVLNode<T>* node, T key)
    {

        if (node == nullptr)
            return new AVLNode<T>(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        else
            return node;


        node->height = 1
                       + max(height(node->left),
                             height(node->right));


        int balance = balanceFactor(node);


        if (balance > 1 && key < node->left->key)
            return rightRotate(node);


        if (balance < -1 && key > node->right->key)
            return leftRotate(node);


        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }


        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }


    AVLNode<T>* minValueNode(AVLNode<T>* node)
    {
        AVLNode<T>* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }


    AVLNode<T>* deleteNode(AVLNode<T>* root, T key)
    {

        if (root == nullptr)
            return root;


        if (key < root->key)
            root->left = deleteNode(root->left, key);
        else if (key > root->key)
            root->right = deleteNode(root->right, key);
        else {

            if ((root->left == nullptr)
                || (root->right == nullptr)) {
                AVLNode<T>* temp
                    = root->left ? root->left : root->right;
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                }
                else
                    *root = *temp;
                delete temp;
            }
            else {

                AVLNode<T>* temp
                    = minValueNode(root->right);
                root->key = temp->key;
                root->right
                    = deleteNode(root->right, temp->key);
            }
        }

        if (root == nullptr)
            return root;


        root->height = 1
                       + max(height(root->left),
                             height(root->right));


        int balance = balanceFactor(root);


        if (balance > 1 && balanceFactor(root->left) >= 0)
            return rightRotate(root);


        if (balance > 1 && balanceFactor(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }


        if (balance < -1 && balanceFactor(root->right) <= 0)
            return leftRotate(root);


        if (balance < -1
            && balanceFactor(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }


    void inorder(AVLNode<T>* root)
    {
        if (root != nullptr) {
            inorder(root->left);
            cout << root->key << " ";
            inorder(root->right);
        }
    }


    bool search(AVLNode<T>* root, T key)
    {
        if (root == nullptr)
            return false;
        if (root->key == key)
            return true;
        if (key < root->key)
            return search(root->left, key);
        return search(root->right, key);
    }

public:

    AVLTree()
        : root(nullptr)
    {
    }


    void insert(T key) { root = insert(root, key); }


    void remove(T key) { root = deleteNode(root, key); }


    bool search(T key) { return search(root, key); }


    void printInorder()
    {
        inorder(root);
        cout << endl;
    }
};

std::map<int, NodeInfo> node_map;
AVLTree<int> tree;


std::string createNode(int id) {
    if (node_map.find(id) != node_map.end()) {
        return "Error: Node already exists";
    }

    int pid = fork();
    if (pid == -1) {
        return "Error: Fork failed";
    }

    if (pid == 0) {
        execl("./node", "./node", std::to_string(id).c_str(), NULL);
        exit(0);
    } else {
        std::ostringstream endpoint;
        endpoint << "tcp://127.0.0.1:" << 5555 + id;

        node_map[id] = {id, pid, endpoint.str()};
        tree.insert(id);
        return "Ok: Created node " + std::to_string(id) + " with PID " + std::to_string(pid);
    }
}


std::string execCommand(int id, const std::string& command) {
    if (node_map.find(id) == node_map.end()) {
        return "Error: Node not found";
    }

    try {
        zmq::context_t context(1);
        zmq::socket_t socket(context, ZMQ_REQ);
        socket.connect(node_map[id].endpoint);

        zmq::message_t request(command.size());
        memcpy(request.data(), command.data(), command.size());
        socket.send(request, zmq::send_flags::none);

        zmq::message_t reply;
        socket.recv(reply, zmq::recv_flags::none);

        return std::string(static_cast<char*>(reply.data()), reply.size());
    } catch (const zmq::error_t& e) {
        return "Error: Node is unavailable (" + std::string(e.what()) + ")";
    } catch (...) {
        return "Error: Unexpected failure";
    }
}

std::string pingNode(int id) {
    if (node_map.find(id) == node_map.end()) {
        return "Error: Not found";
    }

    try {
        zmq::context_t context(1);
        zmq::socket_t socket(context, ZMQ_REQ);

        // Установить тайм-ауты на подключение, отправку и получение
        socket.set(zmq::sockopt::sndtimeo, 1000); // 1 секунда
        socket.set(zmq::sockopt::rcvtimeo, 1000); // 1 секунда

        // Попытаться подключиться к узлу
        socket.connect(node_map[id].endpoint);

        // Отправить сообщение "ping"
        zmq::message_t ping("ping", 4);
        auto send_result = socket.send(ping, zmq::send_flags::none);
        if (!send_result) {
            // Если отправка не удалась
            return "Ok: 0 (send failed)";
        }

        // Попытаться получить ответ
        zmq::message_t reply;
        auto recv_result = socket.recv(reply, zmq::recv_flags::none);

        if (recv_result.value()) {
            return "Ok: 1"; // Узел отвечает
        } else {
            return "Ok: 0 (no reply)";
        }
    } catch (const zmq::error_t& e) {
        // Если сокет бросил исключение
        return "Ok: 0 (exception: " + std::string(e.what()) + ")";
    } catch (...) {
        return "Ok: 0 (unknown error)";
    }

    return "Ok: 0 (fallback)";
}



int main() {
    cout << "Manager started. Commands: create id, exec id [cmd], ping id\n";

    std::string input;
    while (getline(cin, input)) {
        std::istringstream iss(input);
        std::string cmd;
        int id;
        iss >> cmd >> id;

        if (cmd == "create") {
            cout << createNode(id) << "\n";
            tree.printInorder();
        } else if (cmd == "exec") {
            std::string subcmd;
            iss >> subcmd;
            cout << execCommand(id, subcmd) << "\n";
        } else if (cmd == "ping") {
            cout << pingNode(id) << "\n";
        } else {
            cout << "Error: Invalid command\n";
        }
    }

    return 0;
}