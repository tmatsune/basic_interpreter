
#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <queue>

using namespace std;

string INT = "INT";
string STR = "STR";
string BOOL = "BOOl";
string FLT = "FLT";
string PLUS = "PLUS";
string MINUS = "MINUS";
string MUL = "MUL";
string DIV = "DIV";
string L_PAR = "L_PAR";
string R_PAR = "R_PAR";
string KEY = "KEY";
string IDENTIFY = "IDENTIFY";
string EQ = "EQ";
string EE = "EE";
string NE = "NE";
string LT = "LT";
string LTE = "LTE";
string GT = "GT";
string GTE = "GTE";
string L_SQ = "L_SQ";
string R_SQ = "R_SQ";
string COMMA = "COMMA";
string ARROW = "ARROW";


set<string> keys = {"var", "&&", "||", ":", "if", 
                    "elif", "else", "for", "to", 
                    "~", "while", "function", "->", "call"};

set<char> lets = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                  'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u',
                  'v', 'w', 'x', 'y', 'z', '&', '|', ':', '~', '#'};

set<char> digits = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
set<string> arithmitic = {PLUS, MINUS};
set<string> arithmitic2 = {DIV, MUL};
set<string> andor = {"||", "&&"};
set<string> unary = {LT, LTE, GT, GTE, NE, EE};

class Token{
    public:
    string type;
    string strVal;
    string keyWrd;
    int intVal;
    float floatVal;
    char letVal;

    Token(string type){
        this->type = type;
    }
};
class Lexer{
    public:
    string text;
    int pos;

    Lexer(string text){
        this->text = text;
        this->pos = 0;
    }
    void advance(){
        this->pos++;
    }
    Token* genNumber(){
        string fullNum = "";
        int dotCount;
        while(this->pos < this->text.size() && (digits.find(this->text[this->pos]) != digits.end() || this->text[this->pos] == '.')){
            char curr = this->text[this->pos];
            if(curr == '.'){
                dotCount++;
                fullNum += curr;
                if(dotCount > 1) cout << "error too many decimals" << endl;
            }else{
                fullNum += curr;
            }
            this->advance();
        }
        Token* num = new Token(INT);
        num->intVal = stoi(fullNum);
        return num;
    }
    Token* getString(){
        string fullStr = "";
        while(this->pos < this->text.size() && lets.find(this->text[this->pos]) != lets.end()){
            fullStr += this->text[this->pos];
            this->advance();
        }
        string tokType = (keys.find(fullStr) != keys.end()) ? KEY : IDENTIFY;
        Token *tok = new Token(tokType);
        if(tokType == KEY){
            tok->keyWrd = fullStr;
            return tok;
        }
        tok->keyWrd = fullStr;
        return tok;
    }
    Token* genStr(){
        string str = "";
        this->advance();
        while(this->pos < this->text.size() && this->text[this->pos] != '"'){
            str += this->text[this->pos];
            this->advance();
        }
        this->advance();
        Token* tok = new Token(STR);
        tok->strVal = str;
        return tok;
    }

    vector<Token*> genTokens(){
        vector<Token*> tokens;
        while(this->pos < this->text.size()){
            if(this->text[this->pos] == ' '){
                this->advance();
            }else if(digits.find(this->text[this->pos]) != digits.end() ){
                Token* tok = this->genNumber();
                tokens.push_back(tok);
            }else if(lets.find(this->text[this->pos]) != lets.end()){
                Token* tok = this->getString();
                tokens.push_back(tok);
            } else if(this->text[this->pos] == '"'){
                Token* tok = this->genStr();
                tokens.push_back(tok);
            } else if(this->text[this->pos] == '+') {
                Token* tok = new Token(PLUS);
                tokens.push_back(tok);
                this->advance();
            }else if(this->text[this->pos] == '-') {
                if(this->pos < this->text.size() && this->text[this->pos + 1] == '>'){
                    Token *tok = new Token(ARROW);
                    tokens.push_back(tok);
                    this->advance();
                }else{
                    Token *tok = new Token(MINUS);
                    tokens.push_back(tok);
                }
                this->advance();
            }else if(this->text[this->pos] == '/') {
                Token* tok = new Token(DIV);
                tokens.push_back(tok);
                this->advance();
            }else if(this->text[this->pos] == '*') {
                Token* tok = new Token(MUL);
                tokens.push_back(tok);
                this->advance();
            }else if(this->text[this->pos] == '['){
                Token* tok = new Token(L_SQ);
                tokens.push_back(tok);
                this->advance();
            } else if(this->text[this->pos] == ']') {
                Token* tok = new Token(R_SQ);
                tokens.push_back(tok);
                this->advance();
            } else if(this->text[this->pos] == '(') {
                Token* tok = new Token(L_PAR);
                tokens.push_back(tok);
                this->advance();
            }else if(this->text[this->pos] == ')') {
                Token* tok = new Token(R_PAR);
                tokens.push_back(tok);
                this->advance();
            }else if(this->text[this->pos] == '='){
                if(this->pos < this->text.size()){
                    if(this->text[this->pos + 1] == '='){
                        Token* tok = new Token(EE);
                        tokens.push_back(tok);
                        this->advance();
                    }else{
                        Token* tok = new Token(EQ);
                        tokens.push_back(tok);
                    }
                }
                this->advance();
            } else if (this->text[this->pos] == '!') {
                if(this->text[this->pos + 1] == '='){
                    Token* tok = new Token(NE);
                    tokens.push_back(tok);
                    this->advance();
                }else {
                    Token* tok = new Token(KEY);
                    tok->keyWrd = "!";
                    tokens.push_back(tok);
                }
                this->advance();
            } else if (this->text[this->pos] == '<') {
                if(this->text[this->pos + 1] == '='){
                    Token* tok = new Token(GTE);
                    tokens.push_back(tok);
                    this->advance();
                }else {
                    Token* tok = new Token(GT);
                    tokens.push_back(tok);
                }
                this->advance();
            }
            else if (this->text[this->pos] == '>') {
                if(this->text[this->pos + 1] == '='){
                    Token* tok = new Token(LTE);
                    tokens.push_back(tok);
                    this->advance();
                }else {
                    Token* tok = new Token(LT);
                    tokens.push_back(tok);
                }
                this->advance();
            } else{
                cout << "error invali char" << endl;
            }
        }
        return tokens;
    }

};


class AstNode{
    public:
    string type;
    int value;
    string keyWrd;
    AstNode* left;
    AstNode* right;
    vector<vector<AstNode*>> cases;
    AstNode* elsCas;
    vector<Token* > args;
    vector<AstNode*> funcArgs;

    string name;
    AstNode* startNode;
    AstNode* endNode;
    AstNode* step;
    AstNode* body;

    AstNode(string type){
        this->type = type;
    }
    void createIfNode(vector<vector<AstNode*>> cases, AstNode* elsCas){
        this->cases = cases;
        this->elsCas = (elsCas != NULL) ? elsCas : NULL;
    }
    void createForNode(string name, AstNode* startNode, AstNode* endNode, AstNode* step, AstNode* body){
        this->name = name;
        this->startNode = startNode;
        this->endNode = endNode;
        this->step = step;
        this->body = body;
    }
    void createWhileNode(AstNode* condition, AstNode* body){
        this->startNode = condition;
        this->body = body;
    }

    void createFuncNode(string name, vector<Token*> args, AstNode* body){
        this->name = name;
        this->args = args;
        this->body = body;
    }

    void createCallNode(AstNode* atom, vector<AstNode*> args){
        this->body = atom;
        this->funcArgs = args;
    }

    string repre(){
        if(this->type == INT){
            string rep = to_string(this->value);
            return rep;
        }else{
            string rep = "";
            rep += "(";
            rep += to_string(left->value);
            rep += " ";
            rep += this->type;
            rep += " ";
            rep += to_string(right->value);
            rep += ")";
            return rep;
        }
    }


};

class SymbolTable{
    public:
    unordered_map<string, AstNode*> table;

    SymbolTable(){
    }
    void set(string key, AstNode* node){
        if(this->table.find(key) != this->table.end()){
            this->table[key] = node;
        }else{  
            this->table.insert({key, node});
        }
    }
    AstNode* get(string key){
        if(this->table.find(key) != this->table.end()){
            return this->table[key];
        }else{
            return NULL;
        }
    }

};

class Parser{
    public:
    vector<Token*> tokens;
    SymbolTable* lookup;

    int pos;
    Parser(vector<Token*> tokens){
        this->tokens = tokens;
        this->pos = 0;
    }
    AstNode* parse(){
        AstNode* res = this->expre();
        return res;
    }
    void advance(){
        this->pos++;
    }
    AstNode* call(){
        this->advance();
        AstNode* atom = this->factor();
        if(this->tokens[this->pos]->type != L_PAR) cout << "missing L_par" << endl;
        this->advance();
        vector<AstNode*> args;

        while(this->tokens[this->pos]->type != R_PAR){
            AstNode* arg = this->expre();
            args.push_back(arg);
            if(this->tokens[this->pos]->type == COMMA) this->advance();
        }
        this->advance();
        AstNode* funNode = new AstNode(KEY);
        funNode->keyWrd = "call";
        funNode->createCallNode(NULL, args);
        return funNode;
    }
    AstNode* factor(){
        if(this->pos < this->tokens.size()){
            Token* curr = this->tokens[this->pos];
            if(curr->type == L_PAR){
                this->advance();
                AstNode* node = this->arith();
                if (this->tokens[this->pos]->type != R_PAR) cout << "error no ending parenthesis" << endl;
                this->advance();
                return node;
            }else if(curr->type == KEY && curr->keyWrd == "call"){
                AstNode* callNode = this->call();
                return callNode;
            } else if(curr->type == L_SQ){
                this->advance();

            } else if(curr->type == INT){
                this->advance();
                AstNode* num = new AstNode(INT);
                num->value = curr->intVal;
                return num;
            }else if(curr->type == STR){
                this->advance();
                AstNode* str = new AstNode(STR);
                str->keyWrd = curr->strVal;
                return str;
            } else if(curr->type == MINUS){
                this->advance();
                AstNode* node = new AstNode(MINUS);
                AstNode* l = new AstNode(INT);
                l->value = 0;
                node->left = l;
                node->right = this->factor();
                return node;
            }else if (curr->type == IDENTIFY){
                AstNode* node = this->lookup->get(curr->keyWrd);
                this->advance();
                return node;
            } else if(curr->type == KEY && curr->keyWrd == "if"){
                AstNode* node = this->ifExpre();
                return node;
            } else if(curr->type == KEY && curr->keyWrd == "for"){
                AstNode* node = this->forExpre();
                return node;
            } else if(curr->type == KEY && curr->keyWrd == "while"){
                AstNode* node = this->whileExpre();
                return node;
            } else if (curr->type == KEY && curr->keyWrd == "function") {
                AstNode* node = this->funcExpre();
                return node;
            }
            return NULL;
            
        }
        return NULL;
    }
    AstNode* term(){
        AstNode* result = this->factor();
        while(this->pos < this->tokens.size() && arithmitic2.find(this->tokens[this->pos]->type) != arithmitic2.end()){
            AstNode* nwNode = new AstNode(this->tokens[this->pos]->type);
            this->advance();
            nwNode->left = result;
            nwNode->right = this->factor();
            result = nwNode;
        }
        return result;

    }
    AstNode* arith(){
        AstNode* result = this->term();
        while(this->pos < this->tokens.size() && arithmitic.find(this->tokens[this->pos]->type) != arithmitic.end()){
            AstNode* nwNode = new AstNode(this->tokens[this->pos]->type);
            this->advance();
            nwNode->left = result;
            nwNode->right = this->term();
            result = nwNode;
        }
        return result;

    }
    AstNode* compareExpre(){
        AstNode* result = this->arith();
        while(this->pos < this->tokens.size() && unary.find(this->tokens[this->pos]->type) != unary.end()) {
            AstNode* node = new AstNode(this->tokens[this->pos]->type);
            this->advance();
            node->left = result;
            node->right = this->arith();
            result = node;
        }
        return result;
    }

    AstNode* expre(){
        if(this->pos < this->tokens.size() && this->tokens[this->pos]->type == KEY && this->tokens[this->pos]->keyWrd == "var"){
            this->advance();
            if(this->tokens[this->pos]->type != IDENTIFY) cout << "error did not provide name to assign variable" << endl;
            string name = this->tokens[this->pos]->keyWrd;
            this->advance();
            if(this->tokens[this->pos]->type != KEY && this->tokens[this->pos]->keyWrd == EQ) cout << "error no EQ" << endl;
            this->advance();
            AstNode* res = this->expre();
            AstNode* node = new AstNode(IDENTIFY);
            node->type = IDENTIFY;
            node->keyWrd = name;
            this->lookup->set(name, res);
            return node;
        }
        AstNode *result = this->compareExpre();
        while(this->pos < this->tokens.size() && this->tokens[this->pos]->type == KEY && andor.find(this->tokens[this->pos]->keyWrd) != andor.end() ){
            AstNode* node = new AstNode(KEY);
            node->keyWrd = this->tokens[this->pos]->keyWrd;
            this->advance();
            node->left = result;
            node->right = this->compareExpre();
            result = node;

        }
        return result;
    }

    AstNode* ifExpre(){
        vector<vector<AstNode*>> cases;
        AstNode* elscas = NULL;
        if(this->tokens[this->pos]->type == KEY && this->tokens[this->pos]->keyWrd == "if"){
            this->advance();
            AstNode* condition = this->expre();
            if (this->tokens[this->pos]->type != KEY && this->tokens[this->pos]->keyWrd != ":") cout << "missing then operator" << endl;
            this->advance();
            AstNode* then = this->expre();
            vector<AstNode*> currCas = {condition, then};
            cases.push_back(currCas);
        }
        while(this->pos < this->tokens.size() && this->tokens[this->pos]->type == KEY && this->tokens[this->pos]->keyWrd == "elif"){
            this->advance();
            AstNode *condition = this->expre();
            if (this->tokens[this->pos]->type == KEY && this->tokens[this->pos]->keyWrd != ":") cout << "missing then operator" << endl;
            this->advance();
            AstNode *then = this->expre();
            vector<AstNode *> currCas = {condition, then};
            cases.push_back(currCas);
        }
        if (this->pos < this->tokens.size() && this->tokens[this->pos]->type == KEY && this->tokens[this->pos]->keyWrd == "else"){
            this->advance();
            if (this->tokens[this->pos]->type == KEY && this->tokens[this->pos]->keyWrd != ":") cout << "missing ':'" << endl;
            AstNode *then = this->expre();
            elscas = then;
        }
        AstNode* node = new AstNode(KEY);
        node->keyWrd = "if";
        node->createIfNode(cases, elscas);
        return node;
    }

    AstNode* forExpre(){
        // key:for , identify:name, EQ, expre: number, 
        if(this->tokens[this->pos]->type == KEY && this->tokens[this->pos]->keyWrd != "for") cout << "error for loop" << endl;
        this->advance();
        
        if(this->tokens[this->pos]->type != IDENTIFY) cout << " error need identifier" << endl;
        string name = this->tokens[this->pos]->keyWrd;
        this->advance();

        if(this->tokens[this->pos]->type != EQ) cout << "error need '=' operator" << endl;
        this->advance();

        AstNode* startValue = this->expre();
        this->lookup->set(name, startValue);

        if(this->tokens[this->pos]->type == KEY && this->tokens[this->pos]->keyWrd != "to") cout << "error missing ~ key" << endl;
        this->advance();

        AstNode* endValue = this->expre();

        AstNode* step = NULL;
        if(this->tokens[this->pos]->type == KEY && this->tokens[this->pos]->keyWrd == "~"){
            this->advance();
            step = this->expre();
        }

        if (this->tokens[this->pos]->type == KEY && this->tokens[this->pos]->keyWrd != ":") cout << "missing ':' " << endl;
        this->advance();

        //cout << this->tokens[this->pos]->type << " " << this->tokens[this->pos]->keyWrd << endl;
        AstNode *body = this->expre();
        this->advance();

        AstNode* node = new AstNode(KEY);
        node->keyWrd = "for";
        node->createForNode(name, startValue, endValue, step, body);

        return node;
    }
    AstNode* whileExpre(){
        if (this->tokens[this->pos]->type == KEY && this->tokens[this->pos]->keyWrd != "while")
            cout << "error for loop" << endl;
        this->advance();

        AstNode* condition = this->expre();
        if(this->tokens[this->pos]->type == KEY && this->tokens[this->pos]->keyWrd != ":") cout << "error missing : "<< endl;
        this->advance();
        AstNode* body = this->expre();
        this->advance();
        AstNode* node = new AstNode(KEY);
        node->keyWrd = "while";
        node->createWhileNode(condition, body);
        return node;

    }
    AstNode* funcExpre(){
        if(this->tokens[this->pos]->type == KEY && this->tokens[this->pos]->keyWrd != "function"){
            cout << "error could not find function key word" << endl;
        }
        this->advance();

        if (this->tokens[this->pos]->type != IDENTIFY) cout << "error identify key word not found" << endl;
        string name = this->tokens[this->pos]->keyWrd;
        this->advance();

        if(this->tokens[this->pos]->type != L_PAR) cout << "error L_PAR not found" << endl;
        this->advance();

        vector<Token*> args;

        while(this->tokens[this->pos]->type == IDENTIFY){
            args.push_back(this->tokens[this->pos]);
            this->advance();
            if(this->tokens[this->pos]->type == COMMA) this->advance();
        }
        if (this->tokens[this->pos]->type != R_PAR) cout << "error could not find R_PAR" << endl;
        this->advance();

        if(this->tokens[this->pos]->type != ARROW) cout << "error missing arrow key word" << endl;
        this->advance();

        AstNode* expre = this->expre();
        AstNode* func = new AstNode(KEY);
        func->createFuncNode(name, args, expre);
        this->lookup->set(name, func);
        return func;

    }

};

class Number{
    public:
    int val;
    string strVal;

    string name;
    AstNode* bodyNode;
    vector<Token*> args; 

    Number(){
        this->strVal = "";
    }
    void makeFunc(string name, AstNode* bodyNode, vector<Token*>){

    }

};

class Interpreter{
    public:
    SymbolTable* lookup;
    Interpreter(){

    }
    Number* visit(AstNode* node){
        Number* res = getType(node);
        return res;
    }
    Number* numVisit(AstNode* node){
        Number *res = new Number();
        res->val = node->value;
        return res;
    }
    Number* plusVisit(AstNode* node){
        Number* res = new Number();
        Number* l = this->visit(node->left);
        Number* r = this->visit(node->right);
        int val = l->val + r->val;
        res->val = val;
        return res;
        
    }
    Number* minusVisit(AstNode* node){
        Number* res = new Number();
        Number* l = this->visit(node->left);
        Number* r = this->visit(node->right);
        int val = l->val - r->val;
        res->val = val;
        return res;
    }
    Number* divVisit(AstNode* node){
        Number* res = new Number();
        Number* l = this->visit(node->left);
        Number* r = this->visit(node->right);
        int val = l->val / r->val;
        res->val = val;
        return res;
        
    }
    Number* mulVisit(AstNode* node){
        Number* res = new Number();
        Number* l = this->visit(node->left);
        Number* r = this->visit(node->right);
        int val = l->val * r->val;
        res->val = val;
        return res;
    }
    Number* unaryVisit(AstNode* node, string op){
        Number* res = new Number();
        Number *l = this->visit(node->left);
        Number *r = this->visit(node->right);
        if(op == EE){
            bool tf = l->val == r->val;
            res->val = 1 ? (tf == true) : 0;
        }else if(op == GT){
            bool tf = l->val < r->val;
            res->val = 1 ? (tf == true) : 0;
        }else if(op == GTE){
            bool tf = l->val <= r->val;
            res->val = 1 ? (tf == true) : 0;
        }else if(op == LT){
            bool tf = l->val > r->val;
            res->val = 1 ? (tf == true) : 0;
        }else if(op == LTE){
            bool tf = l->val <= r->val;
            res->val = 1 ? (tf == true) : 0;
        }else if(op == NE) {
            bool tf = l->val != r->val;
            res->val = 1 ? (tf == true) : 0;
        }
        return res;
    }
    Number* andorVisit(AstNode* node, string andor){
        Number* res = new Number();
        Number* l = this->visit(node->left);
        Number* r = this->visit(node->right);
        if(andor == "&&"){
            bool tf = l->val == 1 && r->val == 1;
            res->val = 1 ? (tf == true) : 0;
        }else{
            bool tf = l->val == 1 || r->val == 1;
            res->val = 1 ? (tf == true) : 0;
        }
        return res;
    }
    Number* ifVisit(AstNode* node){
        for(vector<AstNode*> cas : node->cases){
            AstNode* condition = cas[0];
            AstNode* then = cas[1];
            Number* cond = this->visit(condition);
            if(cond->val == 1){
                Number* res = this->visit(then);
                return res;
            }
        }
        if(node->elsCas != NULL){
            AstNode *then = node->elsCas;
            Number* res = this->visit(then);
            return res;
        }
        return NULL;
    }

    Number* forVisit(AstNode* node){
        Number* startNum = this->visit(node->startNode);
        Number* endNum = this->visit(node->endNode);
        Number* step = new Number();
        step->val = 1;
        if(node->step != NULL){
            step = this->visit(node->step);
        }
        int i = startNum->val;
        AstNode* astnode = new AstNode(INT);
        astnode->value = startNum->val;
        while(i < endNum->val){
            astnode->value += step->val;
            this->lookup->set(node->name, astnode);
            i += step->val;
            Number* body = this->visit(node->body);
            cout << " -> " << body->val << endl;
        }
        return NULL;
    }

    Number* whileVisit(AstNode* node){
        while (true){
            Number* condition = this->visit(node->startNode);
            if (condition->val == 0) break;
            Number* body = this->visit(node->body);
            cout << " -> " << body->val << endl;
        }
        return NULL;
    }

    Number* assignVisit(AstNode* node){
        AstNode* var = this->lookup->get(node->keyWrd);
        Number* num = this->visit(var);
        var->value = num->val;
        this->lookup->set(node->keyWrd, var);
        return num;
    }
    Number* stringVisit(AstNode* node){
        Number* num = new Number();
        num->strVal = node->keyWrd;
        return num;
    }

    Number* getType(AstNode* node){
        if(node->type == INT){
            Number* num = numVisit(node);
            return num;
        }else if(node->type == IDENTIFY){
            Number* num = assignVisit(node);
            return num;
        } else if(node->type == PLUS) {
            Number *num = plusVisit(node);
            return num;
        }else if(node->type == MINUS) {
            Number *num = minusVisit(node);
            return num;
        }else if(node->type == DIV) {
            Number *num = divVisit(node);
            return num;
        }else if(node->type == MUL) {
            Number *num = mulVisit(node);
            return num;
        }else if(node->type == STR) {
            Number* num = this->stringVisit(node);
            return num;
        }else if(node->type == KEY && (node->keyWrd == "&&" || node->keyWrd == "||")){
            Number* num = this->andorVisit(node, node->keyWrd);
            return num;
        } else if(unary.find(node->type) != unary.end()){
            Number* num = this->unaryVisit(node, node->type);
            return num;
        } else if(node->type == KEY && node->keyWrd == "if"){
            Number* num = this->ifVisit(node);
            return num;
        } else if(node->type == KEY && node->keyWrd == "for") {
            Number* num = this->forVisit(node);
            return num;
        } else if(node->type == KEY && node->keyWrd == "while") {
            Number* num  = this->whileVisit(node);
            return num;
        }
        return NULL;
    }


};

vector<Token*> run(string text){
    Lexer* lexer = new Lexer(text);
    vector<Token*> tokens = lexer->genTokens();
    return tokens;
}




int main(){
    SymbolTable* symbolTable = new SymbolTable();

    while(true){
        string text;
        cout << "input > ";
        getline(cin, text);
        vector<Token *> tokens = run(text);

        cout << "[ ";
        for (Token *tok : tokens){
            if (tok->type == INT){ cout << tok->type << ": " << tok->intVal << " , "; }
            else{ cout << tok->type << " , ";}
        }
        cout << " ]" << endl;
        Parser* parser = new Parser(tokens);
        parser->lookup = symbolTable;
        AstNode* root = parser->parse();

        Interpreter *interpreter = new Interpreter();
        interpreter->lookup = symbolTable;

        Number *number = interpreter->visit(root);
        if(number != NULL && number->strVal != "") cout << number->strVal << endl;  
        else if(number != NULL) cout << "result is: " << number->val << endl;   
        else cout << "no conditions were true" << endl;
    }

    return 0;
}
