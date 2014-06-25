/*
*  This file is a part of the PARUS project.
*  Copyright (C) 2006  Alexey N. Salnikov
*  
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
* 
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
* 
* Files in  this catalogue has been written by group of authors.
* Some authors hesitate of the given source code and 
* refuse copyrights to it.
* 
* Alexey N. Salnikov salnikov@cmc.msu.ru
*
*/


#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include "ctree.h"
#include "inout.h"
#include "context.h"
#include "graph.h"

using namespace std;

void change(Context &context, struct external_declaration &arg, bool update = true);
string getNameByDeclarator(const struct declarator &arg);
void prepare(Context &context, struct translation_unit_list *tree, struct function_definition *main);

int yyparse();
extern struct translation_unit_list *parseResult;
TOptions options; // fixme - insert initiliazation code

struct InputSeq
{
    int total;
    int curr;
    char **names;
    FILE *input;
}
files;

void initFiles(int argc, char **argv)
{
    files. total = argc - 1;
    files. curr = 1;     // skip program name
    files. names = argv;
    files. input = fopen(argv[1], "r");
    // if file not exist - search first non-empty file
    while (files. input == NULL)
    {
        files. curr++;
        if (files.curr > files. total)
        {
            cout << "No correct input files" << endl;
            exit(0);
        }
        files. input = fopen(argv[files. curr], "r");
    }
    // FIXME -  check for non-empty file
    //cout<<"Initialization done"<<argv[files. curr]<<endl;
}

int getc_from_file()
{
    int c;
    c = fgetc(files. input);
    if (c != EOF)
        return c;
    //try open next file
    fclose(files. input);
    files. input = NULL;
    // if file not exist - search first non-empty file
    while (files. input == NULL)
    {
        //cout<<"Search"<<endl;
        files. curr++;
        if (files.curr > files. total)
        {
            return -1;
        }
        //cout<<"Try to open"<<files.names[files. curr]<<endl;
        files. input = fopen(files.names[files. curr], "r");
    }
    // FIXME -  check for non-empty file
    c = fgetc(files. input);

    return c;
}

string getCurrentFile()
{
    if (files.curr > files. total)
    {
        return "noname";
    }
    else
        return string(files.names[files. curr]);
}

int check(const struct external_declaration &arg, const string &name)
{
    /*
    struct external_declaration{
     // only one must be non-NULL
     struct function_definition  *_func_;
     struct declaration   *_decl_;
    };
    */
    if (arg._func_ == NULL)
        return 0;
    return (getNameByDeclarator(arg._func_->signature) == name);
}

void scanTree(struct translation_unit_list *arg)
{
    struct translation_unit_list *temp = arg;
    struct function_definition *_main_ = NULL;
    Context topContext; // create context
    topContext.setSuffix("");
    // print all declaration to "header" decls->head.print(header);
    ofstream header("header");
    // search 'main'
    while (temp != NULL)
    { // insert all global entity to context
        if (check(temp->head, string("main")))
        {
            //cout<<"Main found"<<endl;
            _main_ = temp->head._func_; // FIXME - check for duplicate main
        }
        else
        {
            temp->head.print(header);
        }
        change(topContext, temp->head);
        //temp->head.print(header);
        temp = temp->tail;
    }
    // change global Names to __ + suffix
    //cout<<"Before change :"<<topContext. getVariableList();
    topContext. changeNames();
    //cout<<"After change :"<<topContext. getVariableList()<<"end"<<endl;
    if (_main_ == NULL)
    {
        cout << "Function main not found. Linear graph can't be build..." << endl;
        return ;
    }
    int testvalue;
    //_main_->_body_.print(cout);
    cout << "Enter 1 milestone :";
    //cin>>testvalue;
    Context mainContext;
    mainContext. setParentContext(&topContext);
    //changeName(body._decls_, body._actions_, "_1");
    Graph linear;
    linear.setPath("graph.grf");
    cout << "Enter 2 milestone :";
    //cin>>testvalue;
    prepare(mainContext, parseResult, _main_); // change tree and main body
    struct declaration_list *decls = _main_->_body_._decls_;
    while (decls != NULL)
    {
        decls->head.print(header);
        decls = decls->tail;
    }
    cout << "Enter 3 milestone :";
    //cin>>testvalue;
    if (_main_->_body_. _actions_ != NULL)
    {
        struct statement_list *operators = _main_->_body_._actions_;
        //cout<<"Work begin "<<endl;
        while (operators != NULL)
        {
            // work with operators->head;
            struct in_out info = scanStatement(mainContext, operators->head, false);
            Node node(operators->head, info);
            linear. addNode(node);

            operators = operators->tail;
            //cout<<"Next"<<endl;
        }
        cout << "Enter 4 milestone :";
        //cin>>testvalue;
        linear. print();
    }
    else
    {
        cout << "Main body is empty. Work done..." << endl;
    }
}

int main(int argc, char **argv)
{
    parseResult = NULL;
    if (argc < 2)
    {
        cout << "Usage: cparser.exe {filename1}, ... {filenameN}" << endl;
        return 0;
    }
    initFiles(argc, argv);
    yyparse();
    if (parseResult == NULL)
    {
        printf("File recognition fault\n");
        return 0;
    }
    else
    {
        scanTree(parseResult);
        printf("Ok\n");
        return 0;
    }
}
