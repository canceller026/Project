#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"
class node {
private:
    string name;
    bool type;
    string value;
    bool bePrint;
    int level;
    node* next;
public:
    node(string name, bool type, int level);
    string getName();
    bool getType();
    void xetValue(string value);
    string getValue();
    bool getBePrint();
    void xetBePrint(bool bePrint);
    int getLevel();
    node *getNext();
    void xetNext(node *next);

};

class idenlixt {
private:
    node* head;
    int size;
    idenlixt* prevlixt;
public:
    idenlixt();
    void addTolixt(string name, bool type, int level, bool headOrTail);
    void printlixt();
    void deletelixt();
    void xetHead(node *head);
    node* getHead();
    int getSize();
    void xetPrevlixt(idenlixt *prevlixt);
    idenlixt* getPrevlixt();
};

class SymbolTable {
private:
    idenlixt* currentlixt;
    int currentLevel;
public:
    SymbolTable();
    void run(string filename);
    void insert(string s);
    void assign(string s);
    void begin(string s);
    void end(string s);
    void print(string s);
    void rPrint(string s);
    void lookUp(string s);
    node* checkExistence(string name, idenlixt* a);
    void cleanMemory();
};
#endif
