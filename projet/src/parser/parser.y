%{
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "tree.h"
#include "xml_builder.h"
int yylex(void);
void yyerror(char *);
int yyparse(void);

char* strdup(char*); //TODO : Inutile a discuter avec Paul/Frantisek

void add_head(tree t);
tree create_tree_text(char * text, tree right_tree);
tree evaluate(tree t); //substitution valeurs
tree head_tree = NULL;
int numberTree = 0;


struct variable{
    tree value;
    char *name;
    struct variable *next;
};

struct variable *first, *last;

struct variable* new_var(char* name);
void  assign(char * name, tree value);
struct variable* lookup(char* name);
void create_file(char *name,tree t);
%}

%union{
  char* value;
  struct tree_t* tree_parser;
  struct attributes_t*  attributes_parser;
 };

%type   <attributes_parser>  assign
%type   <tree_parser>        container content attributes begin variable keywords
%token  <value> LABEL TEXT NAME LET EMIT IN WHERE AFFECT END_LET

%%

start:          start begin '\n'
                {
                    add_head($2);
                    draw(head_tree);
                    depth_search(head_tree,0);
                    destroy_tree(head_tree);
                    head_tree=NULL;
                    printf("\n");
                }
        |       start EMIT TEXT begin '\n'
                {
                  create_file($3,$4);
                }
        |	    start variable END_LET '\n'
                {
                  printf("var save\n");
                  assign(get_label(get_daughters($variable)), get_right(get_daughters($variable)));
                }
        |       start keywords ';' '\n'
        |       start '\n'
        |       start error '\n'
                {
                  yyerrok;
                }
        |       /* empty */
                ;

keywords:       variable IN begin
                {
                  printf("coucou je suis ici j'ai bien trouvé ton keyword\n");
                  set_right($variable, $begin);
                  $$ = create_tree("in", false, false, _in, NULL, $variable, NULL);
                  draw($$);
                }
        |       begin WHERE variable
                {
                  set_right($variable, $begin);
                  tree where = create_tree("where", false, false, _where, NULL, $variable, NULL);
                  evaluate($begin);
                }
        ;

variable:	LET LABEL AFFECT begin
		{
          printf("coucou je suis ici j'ai bien trouvé ta variable\n");
          tree name = create_tree($LABEL, false, false, _word, NULL, NULL, $begin);
          $$ = create_tree("=", false, false, _affect, NULL, name, NULL);
          //printf("sauvegarde variable\n");
		}
	;



begin:          LABEL '/'
                {
                  $$ = create_tree($1, true, false, _tree, NULL, NULL, NULL);

                  //printf("state: begin | format: LABEL /\n");
                }
        |       LABEL container
                {
                  $$ = create_tree($1, false, false, _tree, NULL, $2, NULL);
                  
                  //printf("state: begin | format: LABEL container\n");
                }
        |       attributes
                {
                  $$ = $1;
                  //printf("state: begin | format: attribute\n");
                }
        |       container
                {
                  $$ = $1;
                  //printf("state: begin | format: container\n");
                }
        ;


container:     '{' content '}'
                {
                  $$ = $2;
                }
                ;

content:        TEXT content
                {
                    $$= create_tree($1, true, true, _word, NULL,NULL, $2);
                    // printf("state: content | format: TEXT + content\n");
                }
        |       attributes content
                {
                  set_right($1,$2);
                  $$ = $1;
                  //printf("state: content | format: attribute + content\n");
                }
        |       LABEL '/' content
                {
                  $$ = create_tree($1, true, false, _tree, NULL, NULL, NULL);
                  //printf("state: content | format: LABEL /\n");
                }
        |       LABEL container content
                {
                  $$ = create_tree($1, false, false, _tree, NULL, $2, $3);
                  // printf("state: content | format: LABEL container + content\n");
                }
        |       container content
                {
                  if (!$1)
                    $$=$2;
                  else {
                    add_right($1,$2);
                    $$ = $1;
                  }
                  // printf("state: content | format: container\n");
                }
        |       /* empty */
                {
                  $$ = NULL;
                }
                ;


attributes:      LABEL '[' assign ']' '/'
                {
                  $$ = create_tree($1, false, false, _tree, $3, NULL, NULL);
                  // printf("state: attribute | format: LABEL[assign]/\n");
                }
        |       LABEL '[' assign ']' separator container
                {
                  $$ = create_tree($1, false, false, _tree, $3, $6, NULL);
                  // printf("state: attribute | format: LABEL[assign] + separator + container\n");
                }
                ;

separator:
                separator ' '
        |       separator '\n'
        |       separator '\t'
        |       /* empty */
                ;



assign:         LABEL '=' TEXT assign
                {
                    $$ = create_attributes($1, $3, $4);
                }
        |       LABEL '=' TEXT
                {
                    $$ =  create_attributes($1, $3, NULL);
                }
        ;


%%
                /*

tree create_tree_text(char* text, tree right_tree){
  char *tmp;
  tmp=strtok(text," ");
  tree first = create_empty_tree();
  if (tmp){
    set_tp(first,_word);
    set_label(first,strdup(tmp));
    set_nullary(first,true);
    set_space(first,true);
    //first = create_tree(strdup(tmp),true,true,_word,NULL,NULL,NULL);
  }
  tree seconde = first;
  while (tmp){
    tree tmp_tree = create_tree(strdup(tmp),true,true,_word,NULL,NULL,NULL);
    set_right(seconde, tmp_tree);
    seconde = tmp_tree;
    tmp=strtok(NULL," ");
  }
  set_right(seconde, right_tree);
  return first;
}
                */

void add_head(tree t){
  if (head_tree == NULL){
    head_tree  = t;
    return;
  }
  add_right(head_tree, t);
}

tree evaluate(tree t){
  tree son = get_daughters(t);
  while(son != NULL){
    if(get_tp(son) == _var){
      //On récup l'arbre de la variable
      struct variable* var = lookup(get_label(son));
      //On remplace le fils actuel de notre arbre, par l'arbre de variable
      set_daughters(var->value, get_daughters(son));
      set_right(var->value, get_right(son));
      destroy_tree(son);
      set_daughters(t, var->value);
      son = var->value;
    }
    else {
      son = get_right(son);
    }
  }
  evaluate(get_right(t));
  evaluate(get_daughters(t));
  return t;
}


struct variable* new_var(char* name){
    struct variable* tmp =  malloc(sizeof(struct variable));
    tmp->name = strdup(name);
    tmp->value = 0;
    tmp->next = NULL;
    if (first == NULL)
	first = last = tmp;
    else
       {
	   last->next = tmp;
	   last = tmp;
       }
    return tmp;
}

struct variable* lookup(char* name){
    for (struct variable* p = first; p != NULL ;p = p->next){
	if(strcmp(p->name, name) == 0)
	    return p;
    }
    return new_var(name);
}

void assign(char* name, tree value){
  struct variable* tmp = lookup(name);
  tmp->value = value;
}

void create_file(char *name,tree t){
  printf("\n");
  int fd = open(name,O_CREAT|O_TRUNC|O_RDWR,0700);
  int save = dup(1);
  dup2(fd,1);
  depth_search(t,0);
  dup2(save,1);
  close(fd);
  close(save);
}
