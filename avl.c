#include "avl.h"
#include "date.h"

// ----------------------------------functions to create a tree and nodes-----------------------------------------
//creates a tree
avltree * create_tree(entry* root)
{
    avltree * tree = malloc(sizeof(avltree));

    avltreenode* node = create_avl_node(root);

    if (tree != NULL) {
        tree->root = node;
    }
    return tree;
}

//creates an avltree node
avltreenode * create_avl_node(entry* item)
{   

    avltreenode * node = malloc(sizeof(avltreenode));
    if (node) {
        node->left = NULL;
        node->right = NULL;
        node->height = 1;
        node->data = item;
    }
    return node;
}


//----------------------------------functions to insert to tree and rotate if necessary---------------------------

avltreenode* insert_to_tree(avltreenode* node, entry* item) 

{
    int balance;

    if (node == NULL) {
        return(create_avl_node(item)); 
    }
  
    if (to_seconds(item->recordID) < to_seconds(node->data->recordID)) {
        node->left = insert_to_tree(node->left, item); 
    }
    else {
        node->right = insert_to_tree(node->right, item); 
    }
    
    
    node->height = 1 + max(get_tree_height(node->left),get_tree_height(node->right)); 
  
    balance = get_tree_balance(node); 
    
    //we check if the tree is still balanced or not


// T1, T2, T3 and T4 are subtrees.

    // left left rotate
  //        z                                      y 
  //       / \                                   /   \
  //      y   T4      Right Rotate (z)          x      z
  //     / \          - - - - - - - - ->      /  \    /  \ 
  //    x   T3                               T1  T2  T3  T4
  //   / \
  // T1   T2

    if (balance > 1 && to_seconds(item->recordID) < to_seconds(node->left->data->recordID)) 
        return right_tree_rotate(node); 
  


     // Right Right rotate 
    //xriazomai mono ena rotate
    //  z                                y
    //  /  \                            /   \ 
    // T1   y     Left Rotate(z)       z      x
    //     /  \   - - - - - - - ->    / \    / \
    //    T2   x                     T1  T2 T3  T4
    //        / \
    //      T3  T4


    if (balance < -1 && to_seconds(item->recordID) > to_seconds(node->right->data->recordID)) 
        return left_tree_rotate(node); 
  
    // Left Right rotate 
    // kanoume 2 fores rotate edw
    //       z                               z                           x
    //     / \                            /   \                        /  \ 
    //    y   T4  Left Rotate (y)        x    T4  Right Rotate(z)    y      z
    //   / \      - - - - - - - - ->    /  \      - - - - - - - ->  / \    / \
    // T1   x                          y    T3                    T1  T2 T3  T4
    //     / \                        / \
    //   T2   T3                    T1   T2

    if (balance > 1 && to_seconds(item->recordID) > to_seconds(node->left->data->recordID)) 
    { 
        node->left =  left_tree_rotate(node->left); 
        return right_tree_rotate(node); 
    } 
  
    // Right Left rotate
    //    z                            z                            x
    //   / \                          / \                          /  \ 
    // T1   y   Right Rotate (y)    T1   x      Left Rotate(z)   z      y
    //     / \  - - - - - - - - ->     /  \   - - - - - - - ->  / \    / \
    //    x   T4                      T2   y                  T1  T2  T3  T4
    //   / \                              /  \
    // T2   T3                           T3   T4

    if (balance < -1 && to_seconds(item->recordID) < to_seconds(node->right->data->recordID)) 
    { 
        node->right = right_tree_rotate(node->right); 
        return left_tree_rotate(node); 
    } 
    return node; 
} 


avltreenode *right_tree_rotate(avltreenode *y) 
{ 
    avltreenode *x = y->left; 
    avltreenode *subtree = x->right; 
  
    // Perform rotation 
    x->right = y; 
    y->left = subtree; 
  
    // Update heights 
    y->height = max(get_tree_height(y->left), get_tree_height(y->right))+1; 
    x->height = max(get_tree_height(x->left), get_tree_height(x->right))+1; 
  
    // Return new root 
    return x; 
} 
  
avltreenode *left_tree_rotate(avltreenode *x) 
{ 
    avltreenode *y = x->right; 
    avltreenode *subtree = y->left; 
  
    //rotation 
    y->left = x; 
    x->right = subtree; 
  
    //  Update heights 
    x->height = max(get_tree_height(x->left), get_tree_height(x->right))+1; 
    y->height = max(get_tree_height(y->left), get_tree_height(y->right))+1; 
  
    // Return new root 
    return y; 
} 

//-------------------------------------function to free the tree---------------------------------------------------

void avl_tree_free(avltreenode *root)
{
    // destroy all nodes 
     if(root!= NULL){
    
         avl_tree_free(root->left);
         avl_tree_free(root->right);
         free(root);

     }
}


// ---------------------------------------------------voithitikes-----------------------------------------
// get the height of the tree 
int get_tree_height(avltreenode* node) 
{ 
    if (node == NULL) 
        return 0; 

    return node->height; 
} 
  
int max(unsigned int  a, unsigned int b) 
{ 
    return( (a > b)? a : b ); 
} 

// balance of node 
int get_tree_balance(avltreenode* node) 
{ 
    if (node == NULL) 
        return 0; 
    return get_tree_height(node->left) - get_tree_height(node->right); 
}  

void Inorder(avltreenode* root) 
{ 
     if (root == NULL) 
          return; 
  
     /* first recur on left child */
    Inorder(root->left); 
   /* now recur on right child */
     printf("%s --", root->data->recordID);
     Inorder(root->right);
     /* then print the data of node */
       
  
     
} 