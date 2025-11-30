#pragma once

#include <iostream>
#include <string>

template <typename T>
class BinaryTree
{
private:
    BinaryTree* left = nullptr;
    BinaryTree* right = nullptr;
    int sizetree;
    void addtree(const BinaryTree* tree2)
    {
        add(*tree2->info);
        if (tree2->right != nullptr)
        {
            addtree(tree2->right);
        }
        if (tree2->left != nullptr)
        {
            addtree(tree2->left);
        }
    }
    bool cmp(const BinaryTree* tree1, const BinaryTree* tree2)
    {
        if (tree1->sizetree == tree2->sizetree)
        {
            if (*tree1->info != *tree2->info)
            {
                return false;
            }
            if ((tree1->left != nullptr) && (tree2->left != nullptr))
            {
                bool res = cmp(tree1->left, tree2->left);
                if (res == false)
                {
                    return false;
                }
            }
            if ((tree1->right != nullptr) && (tree2->right != nullptr))
            {
                bool res = cmp(tree1->right, tree2->right);
                if (res == false)
                {
                    return false;
                }
            }
            return true;
        }
        else
            return false;
    }
public:
    T* info;
    BinaryTree()
    {
        info = nullptr;
        sizetree = 0;
    }
    BinaryTree(T data)
    {
        info = new T(data);
        sizetree = 1;
    }

    void addElement(T data, bool side)
    {
        if (info == nullptr)
        {
            sizetree++;
            info = new T(data);
        }
        else
        {
            if (side == false)
            {
                if (left == nullptr)
                {
                    sizetree++;
                    left = new BinaryTree(data);
                }
                else
                {
                    sizetree++;
                    left->addElement(data, false);
                }
            }
            if (side == true)
            {
                if (right == nullptr)
                {
                    sizetree++;
                    left->addElement(data, true);
                }
            }
        }
    }

    //BinaryTree(const BinaryTree* tree2)
    //{
    //    addtree(tree2);
    //}
    //bool add(T data)
    //{
    //    bool noexistflag = true;
    //    if (info == nullptr)
    //    {
    //        sizetree++;
    //        info = new T(data);
    //    }
    //    else
    //    {
    //        if (data < *info)
    //        {
    //            if (left == nullptr)
    //            {
    //                sizetree++;
    //                left = new BinaryTree(data);
    //            }
    //            else
    //                if (left->add(data))
    //                    sizetree++;
    //                else
    //                    noexistflag = false;
    //        }
    //        else if (data > *info)
    //        {
    //            if (right == nullptr)
    //            {
    //                sizetree++;
    //                right = new BinaryTree(data);
    //            }
    //            else
    //                if (right->add(data))
    //                    sizetree++;
    //                else
    //                    noexistflag = false;
    //        }
    //        else
    //        {
    //            noexistflag = false;
    //        }
    //    }
    //    return noexistflag;
    //}
/*    T max()
    {
        if (info != nullptr)
        {
            if (right == nullptr)
                return *info;
            else
                right->max();
        }
        else
            std::cerr << "Дерево пусто";
    }
    T min()
    {
        if (info != nullptr)
        {
            if (left == nullptr)
                return *info;
            else
                left->min();
        }
        else
            std::cerr << "Дерево пусто";
    }
    */
    int size()
    {
        return sizetree;
    }
    int empty()
    {
        return (sizetree == 0);
    }
    std::string printsubtree(const BinaryTree* tree)
    {
        std::string str1;
        str1.append(std::to_string(*tree->info));
        str1.append("(");
        if (tree->left != nullptr)
            str1.append(printsubtree(tree->left));
        str1.append(")(");
        if (tree->right != nullptr)
            str1.append(printsubtree(tree->right));
        str1.append(")");
        return str1;
    }
    bool operator ==(BinaryTree<T>& tree2)
    {
        return cmp(this, &tree2);
    }
};

template <typename T>
std::ostream& operator <<(std::ostream& out, BinaryTree<T>& tree)
{
    out << tree.printsubtree(&tree);
    return out;
}
