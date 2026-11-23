#include "check.h"
#include "tracked.h"
#include "solution.cpp"

void run_tests() {
    Tracked::reset();
    {
        auto root = make_node(1);
        auto c = add_child(root, 2);
        auto gc = add_child(c, 3);
        CHECK_TRUE("make_node returns a node", root != nullptr);
        CHECK_EQ("root->tag.value", root ? root->tag.value : -1, 1);
        CHECK_EQ("gc->tag.value", gc ? gc->tag.value : -1, 3);
        CHECK_EQ("depth(root) == 0", depth(root), 0);
        CHECK_EQ("depth(c) == 1", depth(c), 1);
        CHECK_EQ("depth(gc) == 2", depth(gc), 2);
        CHECK_EQ("root->children.size()", root ? root->children.size() : 0u, 1u);
        CHECK_EQ("root.use_count() == 1 (children only watch it)", root.use_count(), 1L);
        CHECK_EQ("c.use_count() == 2 (caller + root->children)", c.use_count(), 2L);
        CHECK_EQ("three Tracked alive in scope", Tracked::alive, 3);
        CHECK_TRUE("parent_of(root) is nullptr", parent_of(root) == nullptr);
        CHECK_TRUE("parent_of(gc) is c", parent_of(gc) == c);
        {
            auto held = parent_of(gc);      // lock() hands out a real owner
            CHECK_EQ("holding parent_of(gc) bumps c.use_count() to 3", c.use_count(), 3L);
        }
        CHECK_EQ("c.use_count() back to 2 after the holder dies", c.use_count(), 2L);
    }
    CHECK_EQ("alive == 0 after the whole tree leaves scope", Tracked::alive, 0);

    Tracked::reset();
    {
        std::shared_ptr<Node> c;
        {
            auto root = make_node(1);
            c = add_child(root, 2);
            add_child(c, 3);
            CHECK_EQ("alive == 3 while root is held", Tracked::alive, 3);
        }
        // The caller's root is gone and nothing else owns it: it must have been destroyed.
        CHECK_TRUE("parent_of(c) is nullptr once root is gone", parent_of(c) == nullptr);
        CHECK_EQ("alive == 2 with root gone (c and its child remain)", Tracked::alive, 2);
        CHECK_EQ("depth(c) == 0 now that it is detached", depth(c), 0);
    }
    CHECK_EQ("alive == 0 after c leaves scope", Tracked::alive, 0);

    Tracked::reset();
    {
        auto root = make_node(10);
        auto a = add_child(root, 11);
        auto b = add_child(root, 12);
        add_child(a, 13);
        CHECK_EQ("root with two children: children.size()", root ? root->children.size() : 0u, 2u);
        CHECK_TRUE("siblings both report root as parent", parent_of(a) == root && parent_of(b) == root);
        CHECK_EQ("alive == 4 for a four-node tree", Tracked::alive, 4);
    }
    CHECK_EQ("alive == 0 after the four-node tree leaves scope", Tracked::alive, 0);
}
