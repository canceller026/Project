//Newest
#include"SymbolTable.h"
node::node(string name, bool type, int level) {
    this->name = name;
    this->type = type;
    this->value= "";
    this->level = level;
    this->bePrint = 1;
    this->next = NULL;
}
string node::getName(){
    return this->name;
    }
bool node::getType(){
    return this->type;
    }
void node::xetValue(string value){
    this->value=value;
    }
string node::getValue(){
    return this->value;
    }
bool node::getBePrint(){
    return this->bePrint;
    }
void node::xetBePrint(bool bePrint){
    this->bePrint = bePrint;
    }
int node::getLevel(){
    return this->level;
    }
node*node::getNext(){
    return this->next;
    }
void node::xetNext(node *next){
    this->next= next;
    }
idenlixt::idenlixt() {
    head = NULL;
    prevlixt = NULL;
    size = 0;
}
void idenlixt::xetHead(node *head){
    this->head=head;
}
node* idenlixt::getHead(){
    return this->head;
    }
int idenlixt::getSize(){
    return this->size;
    }
void idenlixt::xetPrevlixt(idenlixt *prevlixt){
    this->prevlixt=prevlixt;
    }
idenlixt* idenlixt::getPrevlixt(){
    return this->prevlixt;
    }
void idenlixt::addTolixt(string name, bool type, int level, bool headOrTail)
{
    if (head == NULL)
    {
        head= new node(name,type,level);
        size++;
        return;
    }
    node* newNode = new node(name,type,level);
    if (headOrTail) {
        node* tail = head;
        while (tail->getNext() != NULL)
        {
            tail = tail->getNext();
        }
        tail->xetNext(newNode);
    }
    else {
        newNode->xetNext(head);
        head = newNode;
    }
    size++;
}
void idenlixt::printlixt() {
    if (head == NULL)
        return;
    cout << head->getName() << "//" << head->getLevel();
    node* printNode = head->getNext();
    while (printNode != NULL) {
        cout << " " << printNode->getName() << "//" << printNode->getLevel();
        printNode = printNode->getNext();
    }
}
void idenlixt::deletelixt() {
    while (head != NULL) {
        node* toBeDeleted = head;
        head = head->getNext();
        delete(toBeDeleted);
    }
}
SymbolTable::SymbolTable() {
    currentlixt = new idenlixt();
    currentLevel = 0;
}
void SymbolTable::cleanMemory() {
    while (currentlixt != NULL) {   
        idenlixt* lixtForDel = currentlixt;
        currentlixt = currentlixt->getPrevlixt();
        lixtForDel->deletelixt();
        delete lixtForDel;
    }
    delete(currentlixt);
    currentlixt = NULL;
}
void SymbolTable::run(string filename) {
    fstream ifs;
    string s;
    ifs.open(filename, ios::in);
    while (!ifs.eof())
    {
        getline(ifs, s);
        switch (s[0])
        {
        case 'I':
            insert(s);
            cout << endl;
            break;
        case 'A':
            assign(s);
            cout << endl;
            break;
        case 'L':
            lookUp(s);
            cout << endl;
            break;
        case 'P':
            print(s);
            break;
        case 'R':
            rPrint(s);
            break;
        case 'B':
            begin(s);
            break;
        case 'E':
            end(s);
            break;
        default:
            cleanMemory();
            InvalidInstruction a(s);
            throw a;
            break;
        }
    }
    if (currentLevel > 0)
    {
        cleanMemory();
        UnclosedBlock a(currentLevel);
        throw a;
    }
    cleanMemory();
    ifs.close();
}
void SymbolTable::insert(string s)
{
    regex instruction("INSERT[ ][a-z][a-zA-Z0-9_]*[ ](number|string)");
    if (!regex_match(s, instruction)) {
        cleanMemory();
        InvalidInstruction a(s);
        throw a;
    }

    int place = s.find(" ");
    string name = s.substr(place + 1, s.find(" ", place + 1) - place - 1);
    bool type = s.substr(s.find(" ", place + 1) + 1) == "number";

    if (this->currentlixt == NULL)
    {
        currentlixt = new idenlixt();
        currentlixt->addTolixt(name, type, currentLevel, 1);
    }
    else if (this->currentlixt->getHead() == NULL)
    {
        currentlixt->addTolixt(name, type, currentLevel, 1);
    }
    else
    {
        node* checkNode = this->currentlixt->getHead();
        while (checkNode != NULL)
        {
            if (checkNode->getName() == name)
            {
                cleanMemory();
                Redeclared a(s);
                throw a;
            }
            checkNode = checkNode->getNext();
        }
        currentlixt->addTolixt(name, type, currentLevel, 0);
    }
    cout << "success";
}
void SymbolTable::assign(string s)
{
    if (this->currentlixt == NULL)
    {
        cleanMemory();
        Undeclared a(s);
        throw a;
    }

    regex instruction("ASSIGN[ ][a-z][a-zA-Z0-9_]*[ ]+(\'+[a-zA-Z0-9 .-]*+\'|([0-9]+)|([a-z][a-zA-Z0-9_]*))");

    if (!regex_match(s, instruction)) {
        cleanMemory();
        InvalidInstruction a(s);
        throw a;
    }

    int place = s.find(" ");
    string name = s.substr(place + 1, s.find(" ", place + 1) - place - 1), value = s.substr(s.find(" ", place + 1) + 1);
    node* targetNode = checkExistence(name, currentlixt);
    if (targetNode == NULL)
    {
        cleanMemory();
        Undeclared a(s);
        throw a;
    }

    if ('a' <= value[0] && value[0] <= 'z') {
        node* sourceNode = checkExistence(value, currentlixt);
        if (sourceNode == NULL) {
            cleanMemory();
            Undeclared a(s);
            throw a;
        }
        if (targetNode->getType() != sourceNode->getType()) {
            cleanMemory();
            TypeMismatch a(s);
            throw a;
        }
        targetNode->xetValue(sourceNode->getValue());
    }
    else if ((!(targetNode->getType()) && value[0] != 39) || ((targetNode->getType()) && value[0] == 39))
    {
        cleanMemory();
        TypeMismatch a(s);
        throw a;
    }
    else {
        targetNode->xetValue(value);
    }
    cout << "success";
}
void SymbolTable::begin(string s) {
    regex instruction("BEGIN");
    if (!regex_match(s, instruction)) {
        cleanMemory();
        InvalidInstruction a(s);
        throw a;
    }
    currentLevel++;
    idenlixt* newlixt = new idenlixt();
    newlixt->xetPrevlixt(currentlixt);
    this->currentlixt = newlixt;
}
void SymbolTable::end(string s) {
    regex instruction("END");
    if (!regex_match(s, instruction)) {
        cleanMemory();
        InvalidInstruction a(s);
        throw a;
    }
    currentLevel--;
    if (currentLevel < 0)
    {
        currentLevel = 0;
        cleanMemory();
        UnknownBlock a;
        throw a;
    }
    idenlixt* toBeDeleted = currentlixt;
    currentlixt = currentlixt->getPrevlixt();
    toBeDeleted->deletelixt();
    delete(toBeDeleted);
    /*idenlixt* check = current;
    while (check != NULL)
    {
        if (check->getSize() == 0)
        {
            check = check->getPrevlixt();
            continue;
        }
        node* temp = check->getHead();
        while (temp != NULL)
        {
            temp->xetBePrint(1);
            temp = temp->getNext();
        }
        check = check->getPrevlixt();
    }
    */
}
void SymbolTable::print(string s) {
    regex instruction("PRINT");
    if (!regex_match(s, instruction)) {
        cleanMemory();
        InvalidInstruction a(s);
        throw a;
    }
    int level = currentLevel;
    idenlixt* checklixt = currentlixt, *lixtForPrint = new idenlixt();
    while (checklixt->getSize() == 0)
    {
        level--;
        if (level < 0)
        {
            delete(lixtForPrint);
            return;
        }
        checklixt = checklixt->getPrevlixt();
    }

    if (checklixt->getPrevlixt() == NULL)
    {
        node* checkNode = checklixt->getHead();
        while (checkNode != NULL)
        {
            lixtForPrint->addTolixt(checkNode->getName(),checkNode->getType(), checkNode->getLevel(), 0);
            checkNode = checkNode->getNext();
        }
        lixtForPrint->printlixt();
        lixtForPrint->deletelixt();
        delete(lixtForPrint);
    }
    else {
        while (checklixt != NULL)                           //Traverse all levels, start from highest
        {
            node* needToCheck = checklixt->getHead();
            while (needToCheck != NULL)                   //Traverse all elements in a lixt, consider case BEGIN RPINT END
            {
                if (needToCheck->getBePrint())
                {
                    idenlixt* checklixt2 = checklixt->getPrevlixt();
                    while (checklixt2 != NULL)
                    {
                        node* checkOccurence = checklixt2->getHead();
                        while (checkOccurence != NULL)
                        {
                            if (checkOccurence->getName() == needToCheck->getName())
                            {
                                checkOccurence->xetBePrint(0);
                                break;
                            }
                            checkOccurence = checkOccurence->getNext();
                        }
                        checklixt2 = checklixt2->getPrevlixt();
                    }
                    lixtForPrint->addTolixt(needToCheck->getName(), needToCheck->getType(), needToCheck->getLevel(), 0);
                }
                needToCheck = needToCheck->getNext();
            }
            checklixt = checklixt->getPrevlixt();
            level--;
        }
        lixtForPrint->printlixt();
        lixtForPrint->deletelixt();
        delete(lixtForPrint);
    }
    cout << endl;
    idenlixt* checklixt3 = currentlixt;
    while (checklixt3 != NULL)
    {
        if (checklixt3->getSize() == 0)
        {
            checklixt3 = checklixt3->getPrevlixt();
            continue;
        }
        node* checkNode2 = checklixt3->getHead();
        while (checkNode2 != NULL)
        {
            checkNode2->xetBePrint(1);
            checkNode2 = checkNode2->getNext();
        }
        checklixt3 = checklixt3->getPrevlixt();
    }
}
void SymbolTable::rPrint(string s) {
    regex instruction("RPRINT");
    if (!regex_match(s, instruction)) {
        cleanMemory();
        InvalidInstruction a(s);
        throw a;
    }
    int level = currentLevel;
    idenlixt* checklixt = currentlixt;
    while (checklixt->getHead() == NULL)
    {
        level--;
        if (level < 0)
        {
            return;
        }
        checklixt=checklixt->getPrevlixt();
    }
    if (checklixt == NULL) {
        return;
    }
    if (checklixt->getPrevlixt() == NULL) {
        cout << checklixt->getHead()->getName() << "//" << level;
        node* checkNode = checklixt->getHead()->getNext();
        while (checkNode != NULL)
        {
            cout << " " << checkNode->getName()<< "//" << level;
            checkNode = checkNode->getNext();
        }
    }
    else {
        idenlixt* lixtForPrint = new idenlixt();
        while (checklixt != NULL)                           //Traverse all levels, start from highest
        {
            if (checklixt->getHead() == NULL)
            {
                checklixt = checklixt->getPrevlixt();
                continue;
            }
            node* needToCheck = checklixt->getHead();
            while (needToCheck != NULL)                   //Traverse all elements in a lixt, consider case BEGIN RPINT END
            {
                if (needToCheck->getBePrint())
                {
                    idenlixt* checkNode2 = checklixt->getPrevlixt();
                    while (checkNode2 != NULL)
                    {
                        node* checkOccurence = checkNode2->getHead();
                        while (checkOccurence != NULL)
                        {
                            if (checkOccurence->getName() == needToCheck->getName())
                            {
                                checkOccurence->xetBePrint(0);
                                break;
                            }
                            checkOccurence = checkOccurence->getNext();
                        }
                        checkNode2 = checkNode2->getPrevlixt();
                    }
                    lixtForPrint->addTolixt(needToCheck->getName(), needToCheck->getType(), needToCheck->getLevel(), 1);
                }
                needToCheck = needToCheck->getNext();
            }
            checklixt = checklixt->getPrevlixt();
            level--;
        }
        lixtForPrint->printlixt();
        lixtForPrint->deletelixt();
        delete (lixtForPrint);
    }
    cout << endl;
    idenlixt* checklixt2 = currentlixt;
    while (checklixt2 != NULL)
    {
        if (checklixt2->getSize() == 0)
        {
            checklixt2 = checklixt2->getPrevlixt();
            continue;
        }
        node* checkNode3 = checklixt2->getHead();
        while (checkNode3 != NULL)
        {
            checkNode3->xetBePrint(1);
            checkNode3 = checkNode3->getNext();
        }
        checklixt2 = checklixt2->getPrevlixt();
    }
}
void SymbolTable::lookUp(string s) {
    regex instruction("LOOKUP [a-z][a-zA-z0-9_]*");
    if (!regex_match(s, instruction)) {
        cleanMemory();
        InvalidInstruction a(s);
        throw a;
    }
    
    if (currentlixt == NULL)
    {
        cleanMemory();
        Undeclared a(s);
        throw a;
    }
    int place = s.find(" ");
    string name = s.substr(place + 1, s.find(" ", place + 1) - place - 1);
    idenlixt* checklixt = currentlixt;
    int level = currentLevel;
    while (checklixt != NULL)
    {
        node* checkNode = checklixt->getHead();
        {
            while (checkNode != NULL)
            {
                if (checkNode->getName() == name)
                {
                    cout << level;
                    return;
                }
                checkNode = checkNode->getNext();
            }
        }
        level--;
        checklixt = checklixt->getPrevlixt();
    }
    cleanMemory();
    Undeclared a(s);
    throw a;
}
node* SymbolTable::checkExistence(string name, idenlixt* a) {
    bool check = 0;
    idenlixt* checklixt = a;
    node* temp = checklixt->getHead();
    while (checklixt != NULL && (!check)) {
        if (checklixt->getHead() == NULL)
        {
            checklixt = checklixt->getPrevlixt();
            continue;
        }
        temp = checklixt->getHead();
        while (temp != NULL)
        {
            if (temp->getName() == name) {
                check = 1;
                break;
            }
            temp = temp->getNext();
        }
        checklixt = checklixt->getPrevlixt();
    }
    return temp;
}
/*
 void SymbolTable::assign(string s)
{
    if (this->current == NULL)
    {
        cleanMemory();
        Undeclared a(s);
        throw a;
    }

    regex instruction("ASSIGN[ ][a-z][a-zA-Z0-9_]*[ ]+(\'+[a-zA-Z0-9 .-]+\'|([0-9]+)|([a-z][a-zA-Z0-9_]*))");

    if (!regex_match(s, instruction)) {
        cleanMemory();
        InvalidInstruction a(s);
        throw a;
    }

    int place = s.find(" ");
    string name = s.substr(place + 1, s.find(" ", place + 1) - place - 1), value = s.substr(s.find(" ", place + 1) + 1);
    node* res = checkExistence(name, current);

    if ('a' <= value[0] && value[0] <= 'z') {
        node* res2 = checkExistence(value, current);
        if (res2 == NULL && res==NULL) {
            cleanMemory();
            Undeclared a(name+" "+value);
            throw a;
        }
        if (res2 == NULL && res!=NULL)
    {
        cleanMemory();
        Undeclared a(value);
        throw a;
    }
        if (res2 != NULL && res==NULL)
    {
        cleanMemory();
        Undeclared a(name);
        throw a;
    }
        if (res->getType() != res2->getType()) {
            cleanMemory();
            TypeMismatch a(s);
            throw a;
        }
        res->xetValue(res2->getValue());
    }
    else if ((!(res->getType()) && value[0] != 39) || ((res->getType()) && value[0] == 39))
    {
        cleanMemory();
        TypeMismatch a(s);
        throw a;
    }
    else {
        res->xetValue(value);
    }
    cout << "success";
}
void SymbolTable::begin(string s) {
    regex instruction("BEGIN");
    if (!regex_match(s, instruction)) {
        cleanMemory();
        InvalidInstruction a(s);
        throw a;
    }
    numBlock++;
    idenlixt* temp = new idenlixt();
    temp->xetPrevlixt(current);
    this->current = temp;
}
*/
/*void SymbolTable::lookUp(string s) {
    regex instruction("LOOKUP [a-z][a-zA-z0-9_]*");
    if (!regex_match(s, instruction)) {
        cleanMemory();
        InvalidInstruction a(s);
        throw a;
    }
    int place = s.find(" ");
    string name = s.substr(place + 1, s.find(" ", place + 1) - place - 1);
    if (current == NULL)
    {
        cleanMemory();
        Undeclared a(name);
        throw a;
    }
    idenlixt* temp1 = current;
    int block = numBlock;
    while (temp1 != NULL)
    {
        node* temp = temp1->getHead();
        {
            while (temp != NULL)
            {
                if (temp->getName() == name)
                {
                    cout << block;
                    return;
                }
                temp = temp->getNext();
            }
        }
        block--;
        temp1 = temp1->getPrevlixt();
    }
    cleanMemory();
    Undeclared a(name);
    throw a;
}*/