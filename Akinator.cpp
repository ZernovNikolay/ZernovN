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

void tree_print (Tree * tree, ofstream& output) {
  if (tree == NULL)
    return;
  output << tree->data << endl;
  output << "(" << endl;
  cout << tree->data << endl;
  tree_print(tree->left, output);      // печатаем меньшие числа - левое поддерево
  output << ")" << endl;
  //printf("%d ", tree->data);   // печатаем само число в узле
  output << "(" << endl;
  tree_print(tree->right, output);   // печатаем большие числа - правое поддерево
  output << ")" << endl;

}

void tree_print1 (Tree * tree) {
   if (tree == NULL)
      return;

   tree_print1(tree->left);      // печатаем меньшие числа - левое поддерево
   //printf("%d ", tree->data);   // печатаем само число в узле
   //cout << tree->data << endl;
   tree_print1(tree->right);   // печатаем большие числа - правое поддерево
}

Tree* CreateBranch(Tree* ash, type new_data){
  if(ash == NULL){
    Tree* new_branch = (Tree *)calloc(1, sizeof(Tree));
    new_branch->data = new_data;
    new_branch->left = new_branch->right = NULL;
    return new_branch;
  }
  //cout << endl << "1 " << gh << endl;
  //gh++;
    //tree_print(ash);
    return ash;
}

Tree* CreateNewBranch(Tree* ash, string* new_data, int& count){
  if(ash == NULL){
    //cout << "156" << endl;
    Tree* new_branch = (Tree *)calloc(1, sizeof(Tree));
    new_branch->data = new_data[count];
    count++;
    new_branch->left = new_branch->right = NULL;
    return new_branch;
  }
  //cout << "156" << endl;
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

Tree* CreateTreeFromFile(Tree* tree, string* data, int& count, int size){
  if(count < size){
    //cout << count << " " << size << endl;
    tree = CreateNewBranch(tree, data, count);
    if(data[count] == "("){
      count++;
      if(data[count] != ")"){
        tree->left = CreateTreeFromFile(tree->left, data, count, size);
      }
      count++;
      if(data[count] == "("){
        count++;
      }
      if(data[count] != ")"){
        tree->right = CreateTreeFromFile(tree->right, data, count, size);
      }
      count++;//
    }
    return tree;
  }
  return tree;
}

int main() {
  Tree* tree = NULL;
  /*tree = CreateBranch(tree, "HAVE A SEX?", 0);
  tree = CreateBranch(tree, "Vadim", 1);
  tree = CreateBranch(tree, "Kirya", -1);*/
  /*tree->right = CreateBranch(tree->right, "VADIM");
  tree->left = CreateBranch(tree->left, "KIRYA");*/
  string* data = (string*)calloc(100, sizeof(string));
  string bufer;
  cout << "HERE" << endl;
  int count = 0;
  ifstream input("Akinator.txt");
  cout << "HERE" << endl;
  int size = 0;
  if(!input.is_open()){
    cout << "HUESOS" << endl;
    exit(1);
  }
  while(getline(input, bufer)){
    //cout << bufer << endl;
    data[size] = bufer;
    //cout << data[size] << endl;
    size++;
  }
  cout << "HERE" << endl;
  input.close();
  //cout << size << endl;
  tree = CreateTreeFromFile(tree, data, count, size);
  //cout << "HERE" << endl;
  ofstream output("Akinator.txt");
  if(!output.is_open()){
    cout << "HUESOS" << endl;
    exit(1);
  }
  tree_print(tree, output);
  output.close();
  //cout << "LEFT LEFT " << tree->left->left->data << endl;
  //cout << "LEFT LEFT LEFT" << tree->left->left->left->data << endl;
  //cout << "RIGHT " << tree->right->data << endl;
  VISIT(tree);
  output.open("Akinator.txt");
  if(!output.is_open()){
    cout << "HUESOS" << endl;
    exit(1);
  }
  tree_print(tree, output);
  output.close();

  /*cout << endl;
  cin.ignore(1);
  VISIT(tree);
  tree_print(tree);
  cout << endl;
  VISIT(tree);*/
  //cout << tree->data << endl;
  //cout << tree->left << endl;
  tree_free(tree);
  return 0;
}
