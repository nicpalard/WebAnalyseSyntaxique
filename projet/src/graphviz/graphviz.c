#include <graphviz.h>
#include <string.h>


int cpt(void){
  static int cpt = -1;
  cpt++;
  return cpt;
}

static void str_parent(FILE * fd, char *name_parent){
  if (name_parent != NULL){
    fprintf(fd, "%s -> ", name_parent);
  }
}


static void str_null(FILE * fd){
  char name[1000];
  sprintf(name, "\"NULL (%d)\"", cpt());
  fprintf(fd, "%s [color=\"red:orange:yellow:green:cyan:blue:purple;0.14\"];\n%s [label=\"NULL\" color=\"red\"];\n", name, name);  
}


void draw_ast (FILE * fd, struct ast* a, char* name_parent){
  if (a == NULL){
    str_parent(fd, name_parent);
    str_null(fd);
    return;
  }
  
  char name[1000];
  
  switch(a->type){
    case INTEGER:
      printf ("\"INTEGER : %d\n", a->node->num);

      str_parent(fd, name_parent);
      sprintf(name, "\"INTEGER (%d)\"", cpt());
      fprintf(fd, "%s;\n%s [label=\"INTEGER\" color=\"gray\"];\n", name, name);
      
      str_parent(fd, name);
      memset(name, 0, sizeof(*name));
      sprintf(name, "\"%d (%d)\"", a->node->num, cpt());
      fprintf(fd, "%s;\n%s [label=\"%d\"];\n", name, name, a->node->num);

      break;
    case BINOP:
      printf ("BINOP : %s\n", name_binop(a->node->binop));

      str_parent(fd, name_parent);
      sprintf(name, "\"BINOP (%d)\"", cpt());
      fprintf(fd, "%s;\n%s [label=\"BINOP\" color=\"gray\"];\n", name, name);
      
      str_parent(fd, name);
      memset(name, 0, sizeof(*name));
      sprintf(name, "\"%s (%d)\"", name_binop(a->node->binop), cpt());
      fprintf(fd, "%s;\n%s [label=\"%s\"];\n", name, name, name_binop(a->node->binop));
      
      break;
    case UNARYOP:
      printf ("UNARYOP: NOT\n");

      str_parent(fd, name_parent);
      sprintf(name, "\"UNARYOP (%d)\"", cpt());
      fprintf(fd, "%s;\n%s [label=\"UNARYOP\" color=\"gray\"];\n", name, name);
      str_parent(fd, name);
      memset(name, 0, sizeof(*name));
      
      sprintf(name, "\"NOT (%d)\"", cpt());
      fprintf(fd, "%s;\n%s [label=\"NOT\"];\n", name, name);
      
      break;
    case VAR:
      printf ("VAR : %s\n", a->node->str);

      str_parent(fd, name_parent);
      sprintf(name, "\"VAR (%d)\"", cpt());
      fprintf(fd, "%s;\n%s [label=\"VAR\" color=\"gray\"];\n", name, name);
      
      str_parent(fd, name);
      memset(name, 0, sizeof(*name));
      sprintf(name, "\"%s (%d)\"", a->node->str, cpt());
      fprintf(fd, "%s;\n%s [label=\"%s\"];\n", name, name, a->node->str);
      
      break;
    case WORD:
      printf ("WORD\n");

      str_parent(fd, name_parent);
      sprintf(name, "\"WORD (%d)\"", cpt());
      fprintf(fd, "%s;\n%s [label=\"WORD\" color=\"gray\"];\n", name, name);
      
      str_parent(fd, name);
      memset(name, 0, sizeof(*name));
      sprintf(name, "\"%s (%d)\"", a->node->str, cpt());
      fprintf(fd, "%s;\n%s [label=\"%s\"];\n", name, name, a->node->str);
            
      break;
    case IMPORT:
      printf ("\"IMPORT (%d)\"\n", cpt());

      str_parent(fd, name_parent);
      sprintf(name, "\"IMPORT (%d)\"", cpt());
      fprintf(fd, "%s;\n%s [label=\"IMPORT\"];\n", name, name);
      
      draw_import(fd, a, name);
      
      break;
    case APP:
      printf ("APP\n");
      
      str_parent(fd, name_parent);
      sprintf(name, "\"APP (%d)\"", cpt());
      fprintf(fd, "%s;\n%s [label=\"APP\"];\n", name, name);
            
      draw_app(fd, a->node->app, name);
      break;

    case TREE:
      printf ("TREE\n");

      str_parent(fd, name_parent);
      sprintf(name, "\"TREE (%d)\"", cpt());
      fprintf(fd, "%s;\n%s [label=\"TREE\" color=\"#C37501\"];\n", name, name);
      
      draw_tree(fd, a->node->tree, name);
      break;
    case FOREST:
      printf ("FOREST\n");

      str_parent(fd, name_parent);
      sprintf(name, "\"FOREST (%d)\"", cpt());
      fprintf(fd, "%s;\n%s [label=\"FOREST\" color=\"#00CC00\"];\n", name, name);
      
      draw_forest(fd, a->node->forest, name);
      break;
    case FUN:
      printf ("FUN\n");
      
      str_parent(fd, name_parent);
      sprintf(name, "\"FUN (%d)\"", cpt());
      fprintf(fd, "%s;\n%s [label=\"FUN\" color=\"blue\"];\n", name, name);

      draw_fun(fd, a->node->fun, name);
      break;
    case MATCH:
      printf ("MATCH\n");
      
      str_parent(fd, name_parent);
      sprintf(name, "\"MATCH (%d)\"", cpt());
      fprintf(fd, "%s;\n%s [label=\"MATCH\" color=\"purple\"];\n", name, name);
      
      draw_match(fd, a->node->match, name);
      break;
    case COND:
      printf ("COND\n");

      str_parent(fd, name_parent);
      sprintf(name, "\"COND (%d)\"", cpt());
      fprintf(fd, "%s;\n%s [label=\"COND\" color=\"orange\"];\n", name, name);
      
      draw_cond(fd, a->node->cond, name);
      break;
    case DECLREC:
      printf ("DECLREC\n");

      str_parent(fd, name_parent);
      sprintf(name, "\"DECLREC (%d)\"", cpt());
      fprintf(fd, "%s;\n%s [label=\"DECLREC\"];\n", name, name);
      
      draw_fun(fd, a->node->fun, name);
      break;
    default:
      break;
  }
}


void draw_import(FILE * fd, struct ast * a, char * name_parent){
  if (a == NULL){
    str_parent(fd, name_parent);
    str_null(fd);
    return;
  }

  char name[1000];

  str_parent(fd, name_parent);

  for (int i = 0; i < a->node->chemin->n; i++){
    sprintf(name, "\"../ (%d)\"", cpt());
    fprintf(fd, "%s;\n%s [label=\"../\"];\n", name, name);
    if (i < a->node->chemin->n - 1){
      str_parent(fd, name);
    }
  }
  
  draw_dir(fd, a->node->chemin->dir, name);
}


void draw_dir(FILE * fd, struct dir * d, char * name_parent){
  if (d == NULL){
    str_parent(fd, name_parent);
    str_null(fd);
    return;
  }
  
  char name[1000];
  
  switch(d->descr){
    case DIR:
      sprintf(name, "\"dir: %s (%d)\"", d->str, cpt());
      break;
    case FILENAME:
      sprintf(name, "\"file: %s (%d)\"", d->str, cpt());
      break;
    case DECLNAME:
      sprintf(name, "\"func: %s (%d)\"", d->str, cpt());
      break;
    default:
      break;
  }
  
  str_parent(fd, name_parent);
  fprintf(fd, "%s;\n%s [label=\"%s\"];\n", name, name, d->str);
  draw_dir(fd, d->dir, name);
}


void draw_app(FILE * fd, struct app * a, char* name_parent){
  if (a == NULL){
    str_parent(fd, name_parent);
    str_null(fd);
    return;
  }

  draw_ast(fd, a->fun, name_parent);
  
  draw_ast(fd, a->arg, name_parent);
}

void draw_attributes(FILE * fd, struct attributes * a, char* name_parent){  
  if (a == NULL){
    str_parent(fd, name_parent);
    str_null(fd);
    return;
  }
  
  char name[1000];

  str_parent(fd, name_parent);
  sprintf(name, "\"ATTRIBUTES (%d)\"", cpt());
  fprintf(fd, "%s;\n%s [label=\"ATTRIBUTES\"];\n", name, name);
      
  draw_ast(fd, a->key, name);

  draw_ast(fd, a->value, name);
  
  draw_attributes(fd, a->next, name_parent);
}

void draw_tree(FILE * fd, struct tree * t, char* name_parent){
  if (t == NULL){
    str_parent(fd, name_parent);
    str_null(fd);
    return;
  }
  
  printf("label: %s\n", t->label);
  printf("is_value: %d\n", t->is_value);
  printf("nullary: %d\n", t->nullary);
  printf("space: %d\n", t->space);

  draw_attributes(fd, t->attributes, name_parent);

  draw_ast(fd, t->daughters, name_parent);
 
}

void draw_forest(FILE * fd, struct forest * f, char* name_parent){
  if (f == NULL){
    str_parent(fd, name_parent);
    str_null(fd);
    return;
  }
  
  printf("is_value: %d\n", f->is_value);
  
  draw_ast(fd, f->head, name_parent);
  
  draw_ast(fd, f->tail, name_parent);
      
}

void draw_fun(FILE * fd, struct fun * f, char* name_parent){
  if (f == NULL){
    str_parent(fd, name_parent);        
    str_null(fd);
    return;
  }
  
  printf("%s\n", f->id);
  
  draw_ast(fd, f->body, name_parent);
}

void draw_patterns(FILE * fd, struct patterns * p, char* name_parent){
  if (p == NULL){
    str_parent(fd, name_parent);
    str_null(fd);
    return;
  }
  
  draw_pattern(fd, p->pattern, name_parent);

  draw_ast(fd, p->res, name_parent);

  draw_patterns(fd, p->next, name_parent);
}


void draw_match(FILE * fd, struct match * m, char * name_parent){
  if (m == NULL){
    str_parent(fd, name_parent);
    str_null(fd);
    return;
  }
  
  draw_ast(fd, m->ast, name_parent);

  draw_patterns(fd, m->patterns, name_parent);
}


void draw_cond(FILE * fd, struct cond * c, char * name_parent){
  if (c == NULL){
    str_parent(fd, name_parent);
    str_null(fd);
    return;
  }

  draw_ast(fd, c->cond, name_parent);
  
  draw_ast(fd, c->then_br, name_parent);
  
  draw_ast(fd, c->else_br, name_parent);
}


void draw_pattern(FILE * fd, struct pattern * p, char* name_parent){
  if (p == NULL){
    str_parent(fd, name_parent);
    str_null(fd);
    return;
  }
  printf("pattern !");
  
  //printf("pattern_type %s\n", p->ptype);
  //printf("pnode %s\n", p->pnode);
  // === a voir === //
}


void draw(struct ast* a){
  FILE *fd = fopen("fichier.dot", "w+");
  
  if (fd == NULL) {
    fprintf(stderr, "\"fichier.dot\": erreur ouverture fichier.");
    exit(EXIT_FAILURE);
  }
  
  fprintf(fd, "digraph {\n");
  draw_ast(fd, a, NULL);
  fprintf(fd, "}\n");
  
  fclose(fd);
}











struct {
  char * name;
  int id;
} conv_ast_type[DECLREC+1] =
  {
    {"INTEGER", INTEGER},
    {"BINOP", BINOP},
    {"UNARYOP", UNARYOP},
    {"VAR", VAR},
    {"IMPORT", IMPORT},
    {"APP", APP},
    {"WORD", WORD},
    {"TREE", TREE},
    {"FOREST", FOREST},
    {"FUN", FUN},
    {"MATCH", MATCH},
    {"COND", COND},
    {"DECLREC", DECLREC}
  };


enum ast_type find_ast_type(char * name){
  for (int i = 0; i < DECLREC; i++)
    if (strcmp(name, conv_ast_type[i].name) == 0)
      return conv_ast_type[i].id;
  return DECLREC;
}


char *name_ast_type(enum ast_type id){
  if (id < DECLREC)
    return conv_ast_type[id].name;
  return conv_ast_type[DECLREC].name;
}


struct {
  char * name;
  int id;
} conv_binop[AND+1] =
  {
    {"PLUS", PLUS},
    {"MINUS", MINUS},
    {"MULT", MULT},
    {"DIV", DIV},
    {"LEQ", LEQ},
    {"LE", LE},
    {"GEQ", GEQ},
    {"GE", GE},
    {"EQ", EQ},
    {"OR", OR},
    {"AND", AND}
  };


enum binop find_binop(char * name){
  for (int i = 0; i < AND; i++)
    if (strcmp(name, conv_binop[i].name) == 0)
      return conv_binop[i].id;
  return AND;
}


char *name_binop(enum binop id){
  if (id < AND)
    return conv_ast_type[id].name;
  return conv_ast_type[AND].name;
}
