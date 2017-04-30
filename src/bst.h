//-*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
/* The MIT License (MIT)
 *
 * Copyright (c) 2017 Brandon Schaefer
 *                    brandontschaefer@gmail.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef DS_BST_H_
#define DS_BST_H_

#include <memory>

namespace ds
{

template <typename T>
struct Node
{
    T elem{};
    Node* left  {nullptr};
    Node* right {nullptr};
    Node* parent{nullptr};
};

template <typename T>
class bst
{
public:
    bst() = default;

    ~bst();

    bool empty() const;
    size_t size() const;

    void insert(T const& elem);
    void remove(T const& elem);

    Node<T>* find(T const& elem) const;

    // Print functions
    void print_in_order() const;
    void print_pre_order() const;
    void print_post_order() const;

private:
    void delete_all_node(Node<T>* current);

    void swap_subtree(Node<T>* from, Node<T>* to);

    void create_node(Node<T>** node, T const& elem);

    Node<T>* find_lowest_leaf(Node<T>* current) const;

    Node<T>* recursive_find(Node<T>* current, T const& elem) const;
    size_t recursive_size(Node<T>* current) const;

    void recursive_print_in_order(Node<T>* current) const;
    void recursive_print_pre_order(Node<T>* current) const;
    void recursive_print_post_order(Node<T>* current) const;

    std::allocator<Node<T>> node_allocator;
    Node<T>* root{nullptr};
};

template <typename T>
bst<T>::~bst()
{
    delete_all_node(root);
}

template <typename T>
void bst<T>::delete_all_node(Node<T>* current)
{
    if (current)
    {
        delete_all_node(current->left);
        delete_all_node(current->right);
        node_allocator.deallocate(current, 1);
    }
}

template <typename T>
bool bst<T>::empty() const
{
    return size() == 0;
}

template <typename T>
size_t bst<T>::size() const
{
    return recursive_size(root);
}

template <typename T>
size_t bst<T>::recursive_size(Node<T>* current) const
{
    if (current == nullptr)
    {
        return 0;
    }

    return 1 + recursive_size(current->left) + recursive_size(current->right);
}

template <typename T>
Node<T>* bst<T>::find(T const& elem) const
{
    return recursive_find(root, elem);
}

template <typename T>
Node<T>* bst<T>::recursive_find(Node<T>* current, T const& elem) const
{
    if (current == nullptr)
    {
        throw std::runtime_error("Empty tree");
    }

    if (elem == current->elem)
    {
        return current;
    }
    else if (elem < current->elem)
    {
        if (current->left == nullptr)
        {
            return current;
        }

        return recursive_find(current->left, elem);
    }

    if (current->right == nullptr)
    {
        return current;
    }

    return recursive_find(current->right, elem);
}

template <typename T>
void bst<T>::create_node(Node<T>** node, T const& elem)
{
    *node = node_allocator.allocate(1);
    node_allocator.construct(*node);

    (*node)->elem   = elem;
}

template <typename T>
void bst<T>::insert(T const& elem)
{
    if (root == nullptr)
    {
        root = node_allocator.allocate(1);
        node_allocator.construct(root);

        root->elem = elem;
    }
    else
    {
        auto parent = find(elem);

        // We already exists in the tree, ignore dup
        if (parent->elem != elem)
        {
            if (elem < parent->elem)
            {
                create_node(&parent->left, elem);
                parent->left->parent = parent;
            }
            else
            {
                create_node(&parent->right, elem);
                parent->right->parent = parent;
            }
        }
    }
}

template <typename T>
void bst<T>::swap_subtree(Node<T>* from, Node<T>* to)
{
    auto parent = from->parent;

    if (parent == nullptr)
    {
        root = to;
    }
    else
    {
        if (parent->left && parent->left == from)
        {
            parent->left = to;
        }
        else
        {
            parent->right = to;
        }
    }

    if (to != nullptr)
    {
        to->parent = from->parent;

        // Dont wany cycles now do we
        if (from->left != to)
        {
            to->left = from->left;
        }

        if (from->right != to)
        {
            to->right = from->right;
        }
    }
}

template <typename T>
void bst<T>::remove(T const& elem)
{
    auto node_to_delete = find(elem);

    // Otherwise not found
    if (node_to_delete->elem == elem)
    {
        // Leaf case
        if (node_to_delete->left  == nullptr &&
            node_to_delete->right == nullptr)
        {
            swap_subtree(node_to_delete, nullptr);
        }
        // Case 1 child
        else if (node_to_delete->left  == nullptr ||
                 node_to_delete->right == nullptr)
        {
            if (node_to_delete->left == nullptr)
            {
                swap_subtree(node_to_delete, node_to_delete->right);
            }
            else
            {
                swap_subtree(node_to_delete, node_to_delete->left);
            }
        }
        // Case 2 children
        else
        {
            auto lowest_right = find_lowest_leaf(node_to_delete->right);
            swap_subtree(node_to_delete, lowest_right);
        }

        node_allocator.deallocate(node_to_delete, 1);
    }
}

template <typename T>
Node<T>* bst<T>::find_lowest_leaf(Node<T>* current) const
{
    if (current->left != nullptr)
    {
        return find_lowest_leaf(current->left);
    }
    else if (current->right != nullptr)
    {
        return find_lowest_leaf(current->right);
    }
    else
    {
        return current;
    }
}

template <typename T>
void bst<T>::print_in_order() const
{
    recursive_print_in_order(root);
    std::cout << '\n';
}

template <typename T>
void bst<T>::print_pre_order() const
{
    recursive_print_pre_order(root);
    std::cout << '\n';
}

template <typename T>
void bst<T>::print_post_order() const
{
    recursive_print_post_order(root);
    std::cout << '\n';
}

template <typename T>
void bst<T>::recursive_print_in_order(Node<T>* current) const
{
    if (current)
    {
        recursive_print_in_order(current->left);
        std::cout << current->elem << ' ';
        recursive_print_in_order(current->right);
    }
}

template <typename T>
void bst<T>::recursive_print_pre_order(Node<T>* current) const
{
    if (current)
    {
        std::cout << current->elem << ' ';
        recursive_print_pre_order(current->left);
        recursive_print_pre_order(current->right);
    }
}

template <typename T>
void bst<T>::recursive_print_post_order(Node<T>* current) const
{
    if (current)
    {
        recursive_print_post_order(current->left);
        recursive_print_post_order(current->right);
        std::cout << current->elem << ' ';
    }
}

} // namespace ds

#endif // DS_BST_H_
