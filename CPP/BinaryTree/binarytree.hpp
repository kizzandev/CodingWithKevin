#include <bits/stdc++.h>

namespace binarytree
{
    /*
    struct for the Binary Tree Node
    */
    template <typename T>
    struct Node
    {
        T info;
        Node<T> *left = NULL;
        Node<T> *right = NULL;
    };

    /*
    create a new node
    */
    template <typename T>
    Node<T> *newNode()
    {
        Node<T> *p = new Node<T>;
        p->left = NULL;
        p->right = NULL;
        return p;
    }

    /*
    crear un nuevo nodo con un valor inicial
    */
    template <typename T>
    Node<T> *newNode(T value)
    {
        Node<T> *p = new Node<T>;
        p->info = value;
        p->left = NULL;
        p->right = NULL;
        return p;
    }

    /*
    returns left node
    */
    template <typename T>
    Node<T> *goLeft(Node<T> *node)
    {
        // nodo que se encuenta a la izquierda
        return node->left;
    }

    /*
    returns right node
    */
    template <typename T>
    Node<T> *goRight(Node<T> *node)
    {
        // nodo que se encuenta a la derecha
        return node->right;
    }

    /*
    checks if current node is NULL
    */
    template <typename T>
    bool isNull(Node<T> *&node)
    {
        // return node == NULL;
        return (!node);
    }

    /*
    checks if value at left or right is null
    side:
    0 -> izquierda
    1 -> derecha
    */
    template <typename T>
    bool isNull(Node<T> *&node, int side)
    {
        if (side == 1 && node->right ||
            side == 0 && node->left)
            return true;
        return false;
        // return side && node->right || !side && node->left;
    }

    /*
    checks if node is a leaf
    a leaf is a node with a value and no children
    */
    template <typename T>
    bool isLeaf(Node<T> *&node)
    {
        if (!node->left &&
            !node->right)
            return true;
        return false;
        // return !node->left && !node->right;
    }

    /*
    checks if value_1 > value_2
    */
    template <typename T>
    bool isGT(T value_1, T value_2)
    {
        return value_1 > value_2;
    }

    /*
    adds a node to the tree
    */
    template <typename T>
    void addNode(Node<T> *&node, T value)
    {
        if (isNull(node))
        {
            node = newNode(value);
            return;
        }

        Node<T> *p = node;

        while (!isLeaf(p))
        {
            while (p->left != NULL && isGT(p->info, value))
                p = goLeft(p);

            if (p->right != NULL && !isGT(p->info, value))
                p = goRight(p);
            else
                break;
        }

        Node<T> *q = newNode(value);

        if (isGT(p->info, value))
            p->left = q;
        else
            p->right = q;
    }

    /*
    print to console the values of the tree in ascending order
    */
    template <typename T>
    void print(Node<T> *node)
    {
        if (node == NULL)
            return;

        print(node->left);
        std::cout << node->info << " ";
        print(node->right);
    }

    /*
    print to console the values of the tree in descending order
    */
    template <typename T>
    void printReversed(Node<T> *node)
    {
        if (node == NULL)
            return;

        printReversed(node->right);
        std::cout << node->info << " ";
        printReversed(node->left);
    }

    /*
    print to console the values of the tree, first left part then right part
    */
    template <typename T>
    void printLeftToRight(Node<T> *node)
    {
        if (node == NULL)
            return;

        std::cout << node->info << " ";
        printLeftToRight(node->left);
        printLeftToRight(node->right);
    }

    /*
    print to console the values of the tree, idk, just playing with it
    */
    template <typename T>
    void printMostLeftFirstRight(Node<T> *node)
    {
        if (node == NULL)
            return;

        printMostLeftFirstRight(node->left);
        printMostLeftFirstRight(node->right);
        std::cout << node->info << " ";
    }

    /*
    get the depth (or levels) of the tree
    */
    template <typename T>
    int _getDepth(Node<T> *node)
    {
        if (isNull(node))
            return 0;

        return 1 + std::max(_getDepth(node->left), _getDepth(node->right));
    }

    /*
    print the x level of the tree, transversally, horizontally
    */
    template <typename T>
    void _printLevel(Node<T> *node, int level)
    {
        if (node == NULL)
            return;

        if (level == 1)
        {
            int depth = _getDepth(node);
            for (int i = depth - 1; i > 0; i--)
            {
                std::cout << "   ";
            }

            std::cout << node->info << "  ";
        }
        else
        {
            if (node->left == NULL)
                std::cout << "    ";
            _printLevel(node->left, level - 1);
            _printLevel(node->right, level - 1);
        }
    }

    /*
    print tree vertically
    */
    template <typename T>
    void printTreeV(Node<T> *&node)
    {
        if (isNull(node))
            return;

        int depth = _getDepth(node);
        for (int i = 1; i <= depth; i++)
        {
            _printLevel(node, i);
            std::cout << std::endl;
        }
    }

    /*
    print tree horizontally
    */
    template <typename T>
    void printTreeH(Node<T> *&node, int level)
    {
        if (isNull(node))
            return;

        printTreeH(node->right, level + 1);
        for (int i = 0; i < level; i++)
        {
            std::cout << "    ";
        }
        std::cout << node->info << std::endl;
        printTreeH(node->left, level + 1);
    };

    /*
    searches value, returns boolean
    */
    template <typename T>
    bool search(Node<T> *&node, T value)
    {
        if (isNull(node))
            return false;

        if (node->info == value ||
            search(node->left, value) ||
            search(node->right, value))
            return true;

        return false;
    }

} // namespace binarytree
