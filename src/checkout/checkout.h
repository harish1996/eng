#ifndef _CHECKOUT_H
#define _CHECKOUT_H

#include <iostream>
#include "../tree/tree.h"

int _restore_tree( std::string hash, std::string prefix );

int _update_blob( TREE* tree, std::string filename, struct entry& node );

int _update_tree( TREE *tree, std::string name, struct entry& node );

int _try_addnew_blob( TREE *old_tree, TREE *new_tree, std::string filename, struct entry& node );

int _try_addnew_tree( TREE *old_tree, TREE *new_tree, std::string filename, struct entry& node );

int _restore_tree( TREE *new_tree, TREE *old_tree, std::string prefix );

#endif