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

#include <gtest/gtest.h>

#include "bst.h"

struct TestBST : public ::testing::Test
{
    ds::bst<int> bst;
};

TEST_F(TestBST, ctor_empty)
{
    EXPECT_TRUE(bst.empty());
}

TEST_F(TestBST, find_empty_tree_throws)
{
    EXPECT_THROW(bst.find(0), std::runtime_error);
}

TEST_F(TestBST, insert_first)
{
    ds::bst<int> bst;

    bst.insert(3);
    EXPECT_EQ(bst.size(), 1u);
}

TEST_F(TestBST, insert_simple_tree)
{
    ds::bst<int> bst;

    bst.insert(2);
    bst.insert(1);
    bst.insert(3);

    // Find the root and test its correctly constructed
    auto node = bst.find(2);

    ASSERT_NE(node->left, nullptr);
    ASSERT_NE(node->right, nullptr);

    EXPECT_EQ(node->left->elem,  1);
    EXPECT_EQ(node->right->elem, 3);
    EXPECT_EQ(bst.size(), 3u);
}

TEST_F(TestBST, remove_leaf_left)
{
    ds::bst<int> bst;

    bst.insert(2);
    bst.insert(1);

    EXPECT_EQ(bst.size(), 2u);

    bst.remove(1);
    EXPECT_EQ(bst.size(), 1u);
}

TEST_F(TestBST, remove_leaf_right)
{
    ds::bst<int> bst;

    bst.insert(1);
    bst.insert(2);

    EXPECT_EQ(bst.size(), 2u);

    bst.remove(1);
    EXPECT_EQ(bst.size(), 1u);
}

TEST_F(TestBST, remove_leaf_root)
{
    ds::bst<int> bst;

    bst.insert(3);
    EXPECT_EQ(bst.size(), 1u);

    bst.remove(3);
    EXPECT_EQ(bst.size(), 0u);
}

TEST_F(TestBST, remove_with_one_child_left)
{
    ds::bst<int> bst;

    bst.insert(3);
    bst.insert(2);
    bst.insert(1);

    EXPECT_EQ(bst.size(), 3u);

    bst.remove(2);
    EXPECT_EQ(bst.size(), 2u);
}

TEST_F(TestBST, remove_with_one_child_right)
{
    ds::bst<int> bst;

    bst.insert(1);
    bst.insert(2);
    bst.insert(3);

    EXPECT_EQ(bst.size(), 3u);

    bst.remove(2);
    EXPECT_EQ(bst.size(), 2u);
}

TEST_F(TestBST, remove_with_one_child_root)
{
    ds::bst<int> bst;

    bst.insert(3);
    bst.insert(2);
    bst.insert(1);

    EXPECT_EQ(bst.size(), 3u);

    bst.remove(3);
    EXPECT_EQ(bst.size(), 2u);
}

TEST_F(TestBST, remove_with_two_children)
{
    ds::bst<int> bst;

    /*     8
          /
         5
        / \
       3   6
    */
    bst.insert(8);
    bst.insert(5);
    bst.insert(6);
    bst.insert(3);

    EXPECT_EQ(bst.size(), 4u);

    bst.remove(5);
    EXPECT_EQ(bst.size(), 3u);
}

TEST_F(TestBST, remove_with_two_children_root)
{
    ds::bst<int> bst;

    bst.insert(2);
    bst.insert(1);
    bst.insert(3);

    EXPECT_EQ(bst.size(), 3u);

    bst.remove(2);
    EXPECT_EQ(bst.size(), 2u);
}

TEST_F(TestBST, find_returns_insert_parent_if_not_found)
{
    ds::bst<int> bst;

    bst.insert(2);

    auto node = bst.find(1);

    EXPECT_EQ(node->elem, 2);
}

TEST_F(TestBST, find_returns_element)
{
    ds::bst<int> bst;

    bst.insert(5);
    bst.insert(7);
    bst.insert(4);
    bst.insert(2);

    auto node = bst.find(4);

    EXPECT_EQ(node->elem, 4);
}

TEST_F(TestBST, test)
{
    ds::bst<int> bst;
    bst.insert(2);
    bst.insert(3);
    bst.insert(1);
    bst.print_in_order();
    bst.print_pre_order();
    bst.print_post_order();
}
