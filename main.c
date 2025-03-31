

/* COP 3502C Assignment 5
   This program is written by: Jalil Fountain */
   
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   
  
   typedef struct Node {
       char name[31]; 
       int tickets;
       struct Node *left, *right;
   } Node;
   
   
   Node* createNode(char* name, int tickets) {
    // Allocate space for the node, set the fields.
       Node* newNode = (Node*)malloc(sizeof(Node));
       strcpy(newNode->name, name);
       newNode->tickets = tickets;
       newNode->left =NULL; 
       newNode->right = NULL;
      
       return newNode;// Return a pointer to the created node.
   }
   
   Node* findNode(Node* current_ptr, char* name, int* depth) {
    // Check if there are nodes in the tree.
    if (current_ptr != NULL) {
        int comparison = strcmp(name, current_ptr->name);
        // Found the value at the root.
        if (comparison == 0)
            return current_ptr;

        if (depth != NULL)
            (*depth)++;
        // Search to the left.
        if (comparison < 0)
            return find(current_ptr->left, name, depth);
        // Or...search to the right.
        else
            return find(current_ptr->right, name, depth);
    }
    return NULL; // No node found.
}
   
   
   Node* maxVal(Node* root) {
    // Root stores the maximal value.
    if (root->right == NULL)
        return root;
    // The right subtree of the root stores the maximal value.
    else
        return maxVal(root->right);
}
   
   Node* insert(Node* root, char* name, int tickets, int* depth) {
    // inserting into an empty tree
    if (root == NULL) {
        return createNode(name, tickets);
    } else {
        int comparison = strcmp(name, root->name);

        // if the fan already exists, update ticket count
        if (comparison == 0) {
            root->tickets += tickets;
        }
        // insert into the right subtree
        else if (comparison > 0) {
            (*depth)++;
            if (root->right != NULL)
                root->right = insert(root->right, name, tickets, depth);
            else
                root->right = createNode(name, tickets);
        }
        // insert into the left subtree
        else {
            (*depth)++;
            if (root->left != NULL)
                root->left = insert(root->left, name, tickets, depth);
            else
                root->left = createNode(name, tickets);
        }

        // return the updated root
        return root;
    }
}

Node* parent(Node* root, Node* node) {
    // Take care of NULL cases
    if (root == NULL || root == node)
        return NULL;

    // The root is the direct parent of node
    if (root->left == node || root->right == node)
        return root;

    // Look for node's parent in the left side of the tree
    if (strcmp(node->name, root->name) < 0)
        return parent(root->left, node);

    // Look for node's parent in the right side of the tree
    else if (strcmp(node->name, root->name) > 0)
        return parent(root->right, node);

    return NULL; // Catch any other extraneous cases
}


    int hasOnlyLeftChild(Node *node) {
        return (node->left != NULL && node->right == NULL);
    }

    int hasOnlyRightChild(Node *node) {
        return (node->left == NULL && node->right != NULL);
    }
    int isLeaf(Node* node) {
        return (node->left == NULL && node->right == NULL);
    }
   
   Node* delete(Node* root, char* name) {
    if (root == NULL) return NULL;

    Node *delnode, *new_del_node, *save_node;
    Node *par;
    char save_name[31]; 

    delnode = findNode(root, name,NULL); // Get a pointer to the node to delete
    par = parent(root, delnode); // Get the parent of this node

   // Take care of the case where the node to delete is a leaf node.
    if (isLeaf(delnode)) {//case 1
        // Deleting the only node in the tree.
        if (par == NULL) {
            free(root);// free the memory for the node.
            return NULL;
        }
        // Deletes the node if it's a left child.
        if (strcmp(name, par->name) < 0) {
            free(par->left);// Free the memory for the node.
            par->left = NULL;
        } 
        // Deletes the node if it's a right child.
        else {
            free(par->right);// Free the memory for the node.
            par->right = NULL;
        }
        return root; // Return the root of the new tree.
    }

    // Take care of the case where the node to be deleted only has a left
    // child.
    if (hasOnlyLeftChild(delnode)) {
        // Deleting the root node of the tree.
        if (par == NULL) {
            save_node = delnode->left;
            free(delnode); // Free the node to delete.
            return save_node; // Return the new root node of the resulting tree.
        }
        // Deletes the node if it's a left child.
        if (strcmp(name, par->name) < 0) {
            save_node = par->left; // Save the node to delete.
            par->left = par->left->left; // Readjust the parent pointer.
        } 
        // Deletes the node if it's a right child.
        else {
            save_node = par->right;// Save the node to delete.
            par->right = par->right->left;// Readjust the parent pointer.
        }
        free(save_node); //Free the memory for the deleted node.
        return root;// Return the root of the tree after the deletion.
    }

    // Takes care of the case where the deleted node only has a right child.
    if (hasOnlyRightChild(delnode)) {
        // Node to delete is the root node.
        if (par == NULL) {
            save_node = delnode->right;
            free(delnode);
            return save_node;
        }
        // Delete's the node if it is a left child.
        if (strcmp(name, par->name) < 0) {
            save_node = par->left;
            par->left = par->left->right;
        } 
        // Delete's the node if it is a right child.
        else {
            save_node = par->right;
            par->right = par->right->right;
        }
        free(save_node);
        return root;
    }

    //if your code reaches hear it means delnode has two children
    // Find the new physical node to delete.
    new_del_node = maxVal(delnode->left);
    strcpy(save_name, new_del_node->name);
    delnode->tickets = new_del_node->tickets;
    
    
    root = delete(root, save_name);// Now, delete the proper value.
    
    // Restore the data to the original node to be deleted.
    strcpy(delnode->name, save_name);

    return root;
}
   //function to find height of the tree
   int height(Node* root) {
    if (root == NULL) 
        return -1; 

    int leftHeight = height(root->left); //height of left subtree
    int rightHeight = height(root->right); //height of right subtree
    
    if (leftHeight > rightHeight) 
        return leftHeight + 1;
    else 
        return rightHeight + 1;
}
   //function to sum the tickets
    double sumTickets(Node* root) {
        if (root != NULL)
            return root->tickets + sumTickets(root->left) + sumTickets(root->right);
        else
            return 0;
    }
    //function to count all the nodes
    int countNodes(Node* root) {
        if (root == NULL)
            return 0;
        return 1 + countNodes(root->left) + countNodes(root->right);
}
   //function to count tickets before a given name
    int countTicketsBefore(Node* root, char* name) {
        if (root != NULL) {
            int comparison = strcmp(name, root->name);
        
        if (comparison >= 0)
            return root->tickets + countTicketsBefore(root->left, name) + countTicketsBefore(root->right, name);
        else
            return countTicketsBefore(root->left, name);
    }
    return 0;
}
   //function to free the tree
   void freeTree(Node* root) {
       if (root == NULL) return;
       freeTree(root->left);
       freeTree(root->right);
       free(root);
   }
   
   int main() {
    Node* root = NULL; // initialize the root to NULL
    int n; // number of commands
    char command[10], name[31]; 
    int tickets; // number of tickets 

    scanf("%d", &n); //scan number of commands

    for (int i = 0; i < n; i++) {
        scanf("%s", command); // 

        if (strcmp(command, "buy") == 0) {
            scanf("%s %d", name, &tickets); // read name and ticket count
            int depth = 0;
            Node* node = find(root, name, &depth); // try to find the fan 

            if (node) {
                // fan already exists, just update their ticket count
                node->tickets += tickets;
                printf("%s %d %d\n", name, node->tickets, depth);
            } else {
                // fan does not exist, insert a new node
                root = insert(root, name, tickets, &depth);
                printf("%s %d %d\n", name, tickets, depth);
            }
        }
        else if (strcmp(command, "use") == 0) {
            scanf("%s %d", name, &tickets); // read name and tickets to use
            int depth = 0;
            Node* node = find(root, name, &depth); // try to find the fan

            if (!node) {
                // fan not found
                printf("%s not found\n", name);
            } else if (node->tickets <= tickets) {
                // not enough tickets or exactly zero left, delete fan
                root = delete(root, name);
                printf("%s deleted\n", name);
            } else {
                // subtractt tickets and print update
                node->tickets -= tickets;
                printf("%s %d %d\n", name, node->tickets, depth);
            }
        }
        else if (strcmp(command, "find") == 0) {
            scanf("%s", name); // read name to search for
            int depth = 0;
            Node* node = find(root, name, &depth); // search for the fan

            if (node)
                // fan found, print their info
                printf("%s %d %d\n", name, node->tickets, depth);
            else
                // fan not found
                printf("%s not found\n", name);
        }
        else if (strcmp(command, "mean_tickets") == 0) {
            double totalTickets = sumTickets(root); // sum of all tickets
            int totalNodes = countNodes(root);   // total number of fans
        
            if (totalNodes != 0) {
                double mean = totalTickets / totalNodes; //calculate mean
                printf("%.2f\n", mean);
            }
        }
        else if (strcmp(command, "height_balance") == 0) {
            int leftHeight = -1;
            int rightHeight = -1;
        
            if (root != NULL) {
                // get heights of left and right subtrees
                leftHeight = height(root->left);
                rightHeight = height(root->right);
            }
        
            printf("left height = %d right height = %d ", leftHeight, rightHeight);
            if (leftHeight == rightHeight) {
                // tree is balanced
                printf("balanced\n");
            } else {
                printf("not balanced\n");
            }
        }
        else if (strcmp(command, "count_before") == 0) {
            scanf("%s", name); // read the target name
            printf("%d\n", countTicketsBefore(root, name)); 
        }
    }
    // free all memory 
    freeTree(root);
    return 0;
}