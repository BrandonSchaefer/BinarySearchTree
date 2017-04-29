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

#ifndef BST_H
#define BST_H

#include <algorithm>
#include <memory>

namespace ds
{

namespace
{
}

template <typename T>
struct Node
{
    T elem{};
    Node* left  {nullptr};
    Node* right {nullptr};
    Node* parent{nullptr};
};

/*

Find:
    2
   / \
  1  3

Node* find(T const& elem)

Insert:
    

Delete:

*/

template <typename T>
class bst
{
public:
    bst() = default;

    ~bst()
    {
        node_allocator.deallocate(root, 1);
    }

    void insert(T const& elem)
    {
        if (root == nullptr)
        {
            root = node_allocator.allocate(1);
            node_allocator.construct(root);

            root->elem = elem;
        }
        else
        {
            Node<T>* parent = find(elem);

            // We already exists in the tree, ignore dup
            if (parent->elem != elem)
            {
                if (elem < parent->elem)
                {
                    parent->left = node_allocator.allocate(1);
                    node_allocator.construct(parent->left);

                    parent->left->elem   = elem;
                    parent->left->parent = parent;
                }
                else
                {
                    parent->right = node_allocator.allocate(1);
                    node_allocator.construct(parent->right);

                    parent->right->elem    = elem;
                    parent->right->parent = parent;
                }
            }
        }
    }

    bool empty() const
    {
        return size() == 0;
    }

    size_t size() const
    {
        return recursive_size(root);
    }

    Node<T>* find(T const& elem)
    {
        return recursive_find(root, elem);
    }

private:
    Node<T>* recursive_find(Node<T>* current, T const& elem)
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

    size_t recursive_size(Node<T>* current) const
    {
        if (current == nullptr)
        {
            return 0;
        }

        return 1 + recursive_size(current->left) + recursive_size(current->right);
    }

    std::allocator<Node<T>> node_allocator;
    Node<T>* root{nullptr};
};

} // namespace ds

#endif // QUEUE_H
