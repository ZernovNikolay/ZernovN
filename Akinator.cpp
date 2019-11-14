#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <cmath>
#include <cassert>
#include <cstddef>
#include <exception>
#include <stdexcept>
#include <sstream>
using namespace std;
// написать воссоздание дерева

int gh = 0;
typedef string type;

class Tree{
public:
  type data;
  Tree* left;
  Tree* right;

};

void tree_print (Tree * tree) {
   if (tree == NULL)
      return;

   tree_print(tree->left);      // печатаем меньшие числа - левое поддерево
   //printf("%d ", tree->data);   // печатаем само число в узле
   cout << tree->data << " ";
   tree_print(tree->right);   // печатаем большие числа - правое поддерево
}

Tree* CreateBranch(Tree* ash, type new_data){
  if(ash == NULL){
    Tree* new_branch = (Tree *)calloc(1, sizeof(Tree));
    new_branch->data = new_data;
    new_branch->left = new_branch->right = NULL;
    return new_branch;
  }else if(new_data < ash->data){
    ash->left = CreateBranch(ash->left, new_data);
  }else if (new_data > ash->data){
    ash->right = CreateBranch(ash->right, new_data);
  }
  //cout << endl << "1 " << gh << endl;
  //gh++;
    //tree_print(ash);
    return ash;
}

void CreateAki(Tree* ash){
  //cout << "HAHA" << endl;
  //tree_print(ash);
  type new_data, old_data;
  new_data = "gh";
  old_data = ash->data;
  char branch;
  Tree* new_branch = (Tree *)calloc(1, sizeof(Tree));
  cout << "Give new question" << endl;
  cin.ignore(1);
  getline(cin, new_data);
  //cout << new_data << endl;
  ash->data = new_data;
  //new_branch->left = new_branch->right = NULL;
  cout << "Our answer is yes or no?" << endl;
  cin >> branch;
  //cout << "GGGG " << branch << " GGG" << endl;
  //new_data = ash->data;
  //cout << new_data << " DROCHY" << endl;
  if(branch == 'y'){
    ash->right = CreateBranch(ash->right, old_data);
    cout << "GIVE NO ANSWER" << endl;
    cin >> new_data;
    ash->left = CreateBranch(ash->left, new_data);
  }else if(branch == 'n'){
    ash->left = CreateBranch(ash->left, old_data);
    cout << "GIVE YES ANSWER" << endl;
    cin >> new_data;
    ash->right = CreateBranch(ash->right, new_data);
  }
  //tree_print(ash);
  return;
}

void tree_free (Tree * tree) {
   if (tree == NULL)
      return;

   tree_free(tree->left);      // печатаем меньшие числа - левое поддерево
   free(tree);
   tree_free(tree->right);   // печатаем большие числа - правое поддерево
}

void VISIT (Tree * tree) {
   /*if (tree == NULL){
     return;
   }*/
  if(tree->right == NULL){
    cout << "My answer is " << tree->data << endl;
    cout << "RIGHT?" << endl;
    char answer = 'a';
    cin >> answer;
    if(answer == 'n'){
      /*cout << "DRT " << tree->data << " " << tree->left << endl;
      cout << "JOKER1111" << endl;
      tree_print(tree);*/
      CreateAki(tree);
      return;
      /*cout << "JOKER2222" << endl;
      tree_print(tree);*/
    }else if(answer == 'y'){
      cout << "YOOOOOOHUUUUU" << endl;
      return;
    }
    //cout << tree->left << endl;
    //VISIT(tree->left);
  }
  cout << tree->data << endl;
  char answer = 'a';
  cin >> answer;
  if(answer == 'y'){
    if(tree->right != NULL){
      VISIT(tree->right);
    }else{
      cout << tree->data << " is answer1" << endl;
    }
  }else if(answer == 'n'){
    if(tree->left != NULL){
      VISIT(tree->left);
    }else{
      cout << tree->data << " is answer2" << endl;
    }
  }
}

int main() {
  Tree * tree = NULL;
  tree = CreateBranch(tree, "HAVE A SEX?");
  tree->right = CreateBranch(tree->right, "VADIM");
  tree->left = CreateBranch(tree->left, "KIRYA");
  tree_print(tree);
  VISIT(tree);
  tree_print(tree);
  cout << endl;
  //cin.ignore(1);
  VISIT(tree);
  tree_print(tree);
  cout << endl;
  VISIT(tree);
  //cout << tree->data << endl;
  //cout << tree->left << endl;
  tree_free(tree);
  return 0;
}
