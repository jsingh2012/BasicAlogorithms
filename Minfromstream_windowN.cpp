#include<iostream>
#include<stdio.h>
#include<limits.h>
#include<string.h>
using namespace std;
/*
   DATA STRUCTURE and ALORITHM
   1. BST is used to store element of the window.
   2. The required memmory for the BST is preallocated.
   3. local free and alloc are used to manage memory to avoid system calls.
   4. Deletion from the BST is performed based on OLDEST OUT. With minmum index.
   5. Addition like normal BST.
   6. Min like normal BST.
USEAGE:
g++ stream.cpp -o stream
g++ stream.cpp
run
./a.out filename -i 
-i to display input
 */
typedef struct node{
	int data;  // it will store the incoming data
	int count; // it will store in index in the stream
	struct node* left;
	struct node* right;
	struct node* parent;
}node_t;
class window_tree{
	static node_t *root;
	int win_size;
	int count;
	public: 
	node_t *mem_buff;
	window_tree(int n){
		win_size = n;		
		mem_buff = new node_t[n];
		node_t *sp = mem_buff;
		count = 0;
		for(int i = 0; i < n; i++, sp++){
			sp->count = 0;
		}
	}
	int win_add(int indata, int outdata);
	int win_min();
	void show();
	private:
	node_t* find(node_t *root,int data);
	int win_del(int data);
	node_t* alloc_node();
	int free_node(node_t*);
};
node_t* window_tree:: root;
void window_tree:: show(){
	for(int i = 0; i < win_size; i++){
		if(mem_buff[i].count){
			cout<<" <"<<mem_buff[i].data<<",count :="<<mem_buff[i].count<<">;";
		}
	}
	cout<<endl;
}
node_t* window_tree:: find(node_t *ptr, int count){
	static node_t *rptr = NULL;
	//cout<<"find "<<count<<"ptr->count "<<ptr->count<<endl;
	if(ptr == root)
		rptr = NULL;
	if(!ptr)
		return rptr;
	if (ptr->count == count) 
		return rptr = ptr;
	if(!rptr)
		find(ptr->left, count);
	if(!rptr)
		find(ptr->right, count);
	return rptr;

}
node_t* window_tree:: alloc_node(){
	node_t *sp = mem_buff;
	for(int i = 0; i < win_size; i++, sp++){
		if(sp->count)
			continue;
		sp->count = 1;
		return sp;
	}
		return NULL;
	}
	int window_tree:: free_node(node_t* ptr){
	if(ptr){
		ptr->count = 0;
		return 0;
	}
	return 1;
	}
int window_tree:: win_del(int data){
	node_t *ptr = find(root,data);
	//cout<<"ptr count "<<ptr->count<<" del count "<< data<<endl;
	node_t *inords = NULL; 
	if(ptr->left && ptr->right){// both childern 
		//find inorder successor 
		inords = ptr->right; 
		while(inords->left){ 
			inords = inords->left; 
		}
		//Update the parent of inorder successor and the only child of inorder succesor
		if(inords == inords->parent->left){
			inords->parent->left = inords->right;
			if(inords->right)
				inords->right->parent = inords->parent;
		}
		else {
			inords->parent->right = inords->right;
			if(inords->left)
				inords->left->parent = inords->parent;
		}
		//move the inords to the ptr
		inords->left = ptr->left;
		inords->right = ptr->right;
		inords->parent = ptr->parent;
		//Update the parent of ptr
		if(ptr->parent){
			if(ptr == ptr->parent->left)
				ptr->parent->left = inords;
			else
				ptr->parent->right = inords;
		}
		if(inords->right) // Update the new right child of ptr
			inords->right->parent = inords;
		if(inords->left)// update the new left child of ptr
			inords->left->parent = inords;
		if(ptr == root)// if needed chage the root
			root = inords;
	} 
	else if(ptr->left){// only left child
		if(ptr->parent){ 
			if(ptr == ptr->parent->left){//if ptr is left child 
				ptr->parent->left = ptr->left; // update parent 
				ptr->left->parent = ptr->parent;//update the new child of parent 
			} 
			else{ 
				ptr->parent->right = ptr->left; // updat the parent 
				ptr->left->parent = ptr->parent;//update the new child of parent 
			} 
		} 
		else{//  ptr is root
			root = ptr->left;
			root->parent = NULL;		
		}
	}
	else if(ptr->right){// only right child
		if(ptr->parent){ 
			if(ptr == ptr->parent->left){ 
				ptr->parent->left = ptr->right; 
				ptr->right->parent = ptr->parent; 
			} 
			else{ 
				ptr->parent->right = ptr->right; 
				ptr->right->parent = ptr->parent; 
			}
		}
		else{// ptr is root
			root = ptr->right;
			root->parent = NULL;
		}
	}
	else{// left node
		if(ptr->parent){  
			if(ptr == ptr->parent->left)
				ptr->parent->left = NULL;
			else
				ptr->parent->right = NULL;
		}
		else
			root = NULL;
	}
	free_node(ptr);
	return 0;
}
int window_tree:: win_add(int indata, int count){
	if(win_size <= 0)
		return 1;
	node_t *new_node;
	if(count > win_size){
//cout<<"rem "<<count - win_size<<endl;
		win_del(count - win_size);
	}
	node_t *_ptr;
	new_node = alloc_node();
	if(!new_node)
		return 1;
	new_node->data = indata;
	new_node->count = count;
	new_node->left = NULL;
	new_node->right = NULL;
	if(NULL == root){
		new_node->parent = NULL;
		root = new_node;
		return 1;
	}
	_ptr = root;
	while(1){
		if(_ptr->data > indata){
			if(NULL == _ptr->left){
				new_node->parent = _ptr;
				_ptr->left = new_node;
				break;
			}
			_ptr = _ptr->left;
		}
		else{
			if(NULL == _ptr->right){
				new_node->parent = _ptr;
				_ptr->right = new_node;
				break;
			}
			_ptr = _ptr->right;
		}
	}
return 0;
}
int window_tree:: win_min(){
	if(win_size <= 0)
		return -INT_MAX;
	node_t *ptr = root;
	while(ptr->left){
		ptr = ptr->left;
	}
	return ptr->data;
}
int main(int argc, char **argv){
	if(argc < 2){
		cout<<"use:= ./a.out <FILENAME> [-i]\n";
		return 0;
	}
	char ch;
	FILE *fp;
	int num = 0;
	int window = 0;
	int count = 0;
	fp = fopen(argv[1],"r"); // read mode
	if( fp == NULL )
	{
		cout<<"Error while opening the file.\n";
		return 1;	
	}
	if(argc == 3 && !strcmp(argv[2],"-i")){
		cout<<"Input"<<endl;
		while((ch = fgetc(fp))!= EOF){
			cout<<ch;
		}
		fseek(fp,0,SEEK_SET);
	}
	cout<<"Output"<<endl;
	while((ch = fgetc(fp))!= '\n'){
		window = 10*window + (int)(ch - '0');
	}
	//cout<<window<<endl;
	window_tree *obj = new window_tree(window); 
	while((ch = fgetc(fp))!= EOF){
		if(ch == ' '){
			count++;
			obj->win_add(num,count);
			//obj->show();
			num = 0;
			if(count >= window)
				cout<<obj->win_min()<<" ";
			continue;
		}
		if(ch >= '0' && ch <= '9' )
			num = 10*num + (int)(ch - '0');
	}
	count++;
	obj->win_add(num,count);
	//	obj->show();
	cout<<obj->win_min()<<" \n";
	fclose(fp);
	return 0;
}
