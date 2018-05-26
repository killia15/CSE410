#ifndef PROJECT_1
#define PROJECT_1

#include <deque>
#include<iostream>
#include<string>
#include<vector>
#include<ctime>
#include<sys/wait.h>
#include<unistd.h>
using std::ctime;
using std::deque;
using std::string;
using std::cout; using std::cin; using std::endl;
using std::vector;

bool starts_with(string, string);
bool ends_with(string, string);
void strip(string&);
void write_dlist(string);
void cd(string);
void curr();
void date();
void dlist();
void env(int, char**);
void hlist();
string get_hlist(string);

#endif