/* ANALISADOR LÉXICO da Linguagem Ruby
* Integrantes:
* Bárbara
* Camilo
* Rafaela
* Reine
*/

#include <iostream>

/*DEFINIÇÕES DE VALORES GLOBAIS:
 * INPUT_LIN_TAM: quantidade máxima de caracteres por linha (limite de leitura)
 * MAX_TABLE_LEN: tamanho máximo em caracteres das palavras reservadas
 */
#define INPUT_LIN_TAM 1000
#define MAX_TABLE_LEN 20

/*ARRAYS - PALAVRAS RESERVADAS:
 * Os arrays especificam a que categoria pertence os lexemas contidos nele
 */
char ruby_operators[][MAX_TABLE_LEN] = {"..", "|", "^", "&&", "&", "<=>", "==", 
"===", "=-", ">", ">=", "<", "<=", "<<", ">>", "+@", "-@", "[]", 
"[]=", "\0"};
char ruby_sum_op[][MAX_TABLE_LEN] = {"+", "\0"};
char ruby_sub_op[][MAX_TABLE_LEN] = {"-", "\0"};
char ruby_mul_op[][MAX_TABLE_LEN] = {"*", "\0"};
char ruby_div_op[][MAX_TABLE_LEN] = {"/", "\0"};
char ruby_mod_op[][MAX_TABLE_LEN] = {"%", "\0"};
char ruby_exp_op[][MAX_TABLE_LEN] = {"**", "\0"};
char ruby_op_assign[][MAX_TABLE_LEN] = {"+=", "-=", "*=", "/=", "%=", "**=", 
"&=", "|=", "^=", "<<=", ">>=", "&&=", "||=", "=", "\0"};
char ruby_stmt_op[][MAX_TABLE_LEN] = {"alias", "{", "}", "defined?", "do", "then", "\0"};
char ruby_block_op[][MAX_TABLE_LEN] = {"begin", "end", "\0"};
char ruby_cond_op[][MAX_TABLE_LEN] = {"if", "elsif", "else", "unless", "case", 
"when", "until", "ensure",  "\0"};
char ruby_comm_op[][MAX_TABLE_LEN] = {"return", "yield","\0"};
char ruby_method_op[][MAX_TABLE_LEN] = {".", "::", "super", "\0"};
char ruby_func_op[][MAX_TABLE_LEN] = {"def", "undef", "(", ")", "\0", };
char ruby_term_stmt_op[][MAX_TABLE_LEN] = {";", "\n", "\0"};
char ruby_global_id[][MAX_TABLE_LEN] = {"$",  "\0"};
char ruby_op_id[][MAX_TABLE_LEN] = {"!", "?", "\0"};
char ruby_var_id[][MAX_TABLE_LEN] = {"@@", "@", "\0"};
char ruby_logic_op[][MAX_TABLE_LEN] = {"and", "not", "or" "\0"};
char ruby_bool_lit[][MAX_TABLE_LEN] = {"true", "false", "nil", "\0"};
char ruby_loop_op[][MAX_TABLE_LEN] = {"for" "while", "\0"};
char ruby_sep_op[][MAX_TABLE_LEN] = {",", "\0"};
char ruby_comment_begin[][MAX_TABLE_LEN] = {"=begin", "\0"};
char ruby_comment_end[][MAX_TABLE_LEN] = {"=end", "\0"};
char ruby_print_op[][MAX_TABLE_LEN] = {"puts", "print", "\0"};
char ruby_class_op[][MAX_TABLE_LEN] = {"class", "\0"};
char ruby_list_op[][MAX_TABLE_LEN] = {"[", "]", "\0"};

/*VARIÁVEIS GLOBAIS:
* As variáveis globais identificam se strings ou comentários foram fechados
* corretamente
*/
bool global_comment_open = false;
bool global_string_open = false;

/*NOMES DOS TOKENS:
 * -> Array que contém os nomes dos tokens
 * -> Cada nome de token corresponde a uma das categorias elencadas nos arrays
 * declarados acima
 */
char ruby_table[][MAX_TABLE_LEN] = {"UNKNOWN", "OPERATOR", "SUM_OP", "SUB_OP", "MULT_OP", "DIV_OP", "MOD_OP", "EXP_OP", "ASSIGN_OP", "STATEMENT_OP", "BLOCK_OP", "COND_OP", "COMMAND_OP", "METHOD_OP", "FUNCTION_OP", "END_STATEMENT_OP", "GLOBAL_ID", "OPERATION_ID", "VAR_ID", "LOGIC_OP", "BOOL_OP", "LOOP_OP", "SEP_OP", "STRING_LIT", "INT_LIT", "FLOAT_LIT", "PRINT_OP", "CLASS_OP", "LIST_ASSOC_OP", "IDENTIFIER"};

// DECLARAÇÃO ANTECIPADA DE MÉTODOS:
void flush_input_lin(char* input_lin, int tam);
int check_if_res(char* lex, char list[][MAX_TABLE_LEN]);
int check_if_id(char* lex);
void check_tok(char* lex, int* tok, int* len);
void print_lex(char* lex, int tam);
void print_tok(int tok);

// MAIN:
int main() {
   int lin_num = 1; // contador de linhas
   int char_itr = 0; // var. iteradora utilizada pra percorrer os caracteres
   int tmp_itr = 0; // var. temporária com a mesma função da char_itr
   int t_tok, t_len; // identificadores de token e do tamanho do lexema
   char* input_lin = (char*) malloc(INPUT_LIN_TAM * sizeof(char)); // variável buffer que
   // mantém a linha atualmente sendo analisada
   while (std::cin.getline(input_lin, INPUT_LIN_TAM)) { // lê o input linha por linha
      // caso a linha seja vazia, pule-a:
      if (input_lin[0] == '\n' || input_lin[0] == '\0') {
         lin_num++;
         continue;
      }
      // não sendo vazia, continua:
      char_itr = 0; // zera a variável iteradora
      std::cout << ">> lin[" << lin_num++ << "]: " << input_lin << std::endl; // imprime a linha atual
      // enquanto não for o final da linha:
      while (input_lin[char_itr] != '\n' && input_lin[char_itr] != '\0') {
         t_tok = 0; // tokens a princípio desconhecidos (0 = UNKNOWN)
         t_len = 0; // tamano do lexema = 0; 
         if (input_lin[char_itr] == ' ') { // ignora espaços em branco
            char_itr++;
            continue;
         }
         check_tok(&input_lin[char_itr], &t_tok, &t_len); // checa o token do lex atual
         /* se for identificado que um comentário ou string foi aberto, sai do loop
         *  não realizando a análise dos lexemas:
         */
         if (global_comment_open || global_string_open) {
            char_itr++;
            break;
         }
         /* t_tok < 0 foi convencionado como um identificador universal de que aquela linha deve ser ignorada*/
         if (t_tok < 0) {
            break;
         }
         // senão, imprime a relação lex -> token:
         else {
            print_lex(&input_lin[char_itr], t_len);
            std::cout << " -> ";
            print_tok(t_tok);
            std::cout << std::endl;  
            /* a função check_tok modifica em seus parâmetros o valor de t_tok e de
            *  t_len. Isso porque o analisador léxico analisa char por char. No caso
            * do token de um lexema já ter sido identificado, o valor retornado de t_len
            * já informa o valor que deve incrementar char_itr, caso contrário o algoritmo
            * iria analisar várias vezes o mesmo lexema, uma vez para cada letra:
            */     
            char_itr += t_len;
         }
      }
      // criei essa função para "limpar" o buffer de input_lin
      flush_input_lin(&input_lin[0], char_itr);
      std::cout << std::endl;
   }
   /* aqui, ao chegar no final do programa, checa se a var. global_comment_open
   *  foi fechada, senão, isso indica que um comentário não foi corretamente finalizado:
   */
   if (global_comment_open) {
      std::cout << ">>> ERRO: COMENTÁRIO NÃO FECHADO!!" << std::endl;
   }
   // aqui, ao cehgar no final do programa, checa se alguma string não foi fechada:
   if (global_string_open) {
      std::cout << ">>> ERRO: STRING NÃO FECHADA!!" << std::endl;
   }
}

// função que "limpa" o buffer da var. input_lin:
void flush_input_lin(char* input_lin, int tam) {
   for (int i = 0; i < tam; i++) {
      input_lin[i] = '\n';
   }
}

/* FUNÇÃO QUE CHECA SE O LEXEMA É RESERVADO:
* A seguinte função é 'genérica' e procura se o lexema 'lex' se encontra num
* array 'list[][MAX_TABLE_LEN]'. Essa função é utilizada para fazer a checagem
* das palavras que se encontram nos arrays de palavras reservadas no início do
* código do programa
*/
int check_if_res(char* lex, char list[][MAX_TABLE_LEN]) {
   int list_i = 0;
   bool found;
   int i;
   while (list[list_i][0] != '\0') {
      i = 0;
      found = true;
      while (list[list_i][i] != '\0') {
         if (list[list_i][i] != lex[i]) {
            found = false;
            break;
         }
         i++;
      }
      if (list[list_i][i] == '\0' && found) {
         return i;
      }
      list_i++;
   }
   return 0;
}

// A função seguinte checa se o lexema 'lex' representa um comentário:
int check_if_comment(char* lex) {
	if (lex[0] == '#') return 1; // se possui '#' é comentário de uma linha
   // caso não seja comentário de uma linha, checa se é um '=begin':
	else if (check_if_res(lex, ruby_comment_begin) > 0) {
      global_comment_open = true; // modifica a var. global pra indicar que foi aberto
      return 1; // retorna 1 indicando que é um comentário
   }
   // senão, pode ser o caso de fechar o comentário, checando se é '=end':
   else if (check_if_res(lex, ruby_comment_end) > 0) {
      global_comment_open = false;
      return 1;
   }
   return 0;
}

/* A função abaixo checa se o lexema representa um identificador conforme as regras
* estabelecidas na gramática da linguagem Ruby utilizando a tabela ASCII. Isto é:
* IDENTIFIER : sequence in /[a-zA-Z_]{a-zA-Z0-9_}/.
*/
int check_if_id(char* lex) {
   int i = 0;
   if (lex[i] < 'A' || (lex[i] > 'Z' && lex[i] != '_' && lex[i] < 'a') || lex[i] > 'z')
      return -1;
   i++;
   while ((lex[i] >= '0' && lex[i] <= '9') || (lex[i] >= 'A' && lex[i] <= 'Z') || lex[i] == '_' || (lex[i] >= 'a' && lex[i] <= 'z'))
      i++;
   return i;   
}

// Checa se o lexema é um inteiro:
int check_if_int(char* lex) {
   int i = 0;
   if (lex[0] == '-') {
		if (lex[1] < '0' || lex[1] > '9') return 0;
		else i++;	   
	}   
  	while (lex[i] >= '0' && lex[i] <= '9') {
		i++;
   }
   return i;
}

// Checa se o lexema é um float:
int check_if_float(char* lex) {
   int i = 0;
   if (lex[i] == '-') {
      i++;
		if (lex[i++] < '0' || lex[1] > '9') return 0;
	}
   while (lex[i] >= '0' && lex[i] <= '9') i++;
   if (lex[i] == '.' && i > 0) {
      i++;
      while (lex[i] >= '0' && lex[i] <= '9') i++;
   }
   else return 0;
   return i;
}

// Checa se o lexema é uma string:
int check_if_string(char* lex) {
   int i = 0;
   if (lex[i] == '"') {
      global_string_open = true;
      i++;
      while (lex[i] != '"' && lex[i] != '\n') i++;
      if (lex[i] == '"') global_string_open = false;
   }
   else if (lex[i] == '\'') {
      i++;
      while (lex[i] != '\'' && lex[i] != '\n') i++;
      if (lex[i] == '\'') global_string_open = false;
   }
   else if (lex[i] == '`') {
      i++;
      while (lex[i] != '`' && lex[i] != '\n') i++;
      if (lex[i] == '`') global_string_open = false;
   }
   if (i > 0) i++;
   return i;
}

/* FUNÇÃO DE IDENTIFICAÇÃO DOS TOKENS:
* lex: lexema analisado
* tok: recebe o valor do token encontrado
* len: recebe o valor do 'tamanho' do lexema
*
* Os valores de 'tok'  correspondem aos índices do array 'ruby_table'.
* Além disso, existem dois valores convencionados:
* 0: desconhecimento (UNKNOWN)
* -1: valor genérico que indica que aquele lexema deve ser ignorado
*
* É a principal função do programa. Aqui o token do lexema 'lex' é identificado, em alguns
* casos utilizando o princípio da exclusão ("se não é um, deve ser o outro"). Por esta 
* razão, a ordem de análise importa para que o resultado final esteja correto.
*
* Importante notar que cada função recebe e modifica o parâmetro 'len'. Nesse caso, 'len'
* serve tanto pra indicar se o lexema corresponde àquele token (se o valor de retorno de
* len for maior que 0), quanto para indicar o tamanho do lexema. Caso o valor retornado
* seja 0, então não corresponde àquele token.
*/
void check_tok(char* lex, int* tok, int* len) {
	 *len = check_if_comment(lex);
   if (*len > 0) {
      *tok = -1;
      return;
   }
   *len = check_if_res(lex, ruby_operators);
   if (*len > 0) {
      *tok = 1;     
      return;
   }
   *len = check_if_res(lex, ruby_sum_op);
   if (*len > 0) {
      *tok = 2;
      return;
   }
   *len = check_if_res(lex, ruby_func_op);
   if (*len > 0) {
      *tok = 14;
      return;
   }
    *len = check_if_res(lex, ruby_stmt_op);
   if (*len > 0) {
      *tok = 9;
      return;
   }
   *len = check_if_float(lex);
   if (*len > 0) {
      *tok = 25;
      return;
   }
    *len = check_if_int(lex);
   if (*len > 0) {
      *tok = 24;
      return;
   }  
    *len = check_if_res(lex, ruby_sub_op);
   if (*len > 0) {
      *tok = 3;
      return;
   }
   *len = check_if_res(lex, ruby_mul_op);
   if (*len > 0) {
      *tok = 4;
      return;
   }
   *len = check_if_res(lex, ruby_div_op);
   if (*len > 0) {
      *tok = 5;
      return;
   }
   *len = check_if_res(lex, ruby_mod_op);
   if (*len > 0) {
      *tok = 6;
      return;
   }
   *len = check_if_res(lex, ruby_exp_op);
   if (*len > 0) {
      *tok = 7;
      return;
   }
   *len = check_if_res(lex, ruby_op_assign);
   if (*len > 0) {
      *tok = 8;
      return;
   }
   *len = check_if_res(lex, ruby_block_op);
   if (*len > 0) {
      *tok = 10;
      return;
   }
   *len = check_if_res(lex, ruby_cond_op);
   if (*len > 0) {
      *tok = 11;
      return;
   }
   *len = check_if_res(lex, ruby_comm_op);
   if (*len > 0) {
      *tok = 12;
      return;
   }
   *len = check_if_res(lex, ruby_method_op);
   if (*len > 0) {
      *tok = 13;
      return;
   }
   *len = check_if_res(lex, ruby_term_stmt_op);
   if (*len > 0) {
      *tok = 15;
      return;
   }
   *len = check_if_res(lex, ruby_global_id);
   if (*len > 0) {
      *tok = 16;
      return;
   }
   *len = check_if_res(lex, ruby_op_id);
   if (*len > 0) {
      *tok = 17;
      return;
   }
   *len = check_if_res(lex, ruby_var_id);
   if (*len > 0) {
      *tok = 18;
      return;
   }
   *len = check_if_res(lex, ruby_logic_op);
   if (*len > 0) {
      *tok = 19;
      return;
   }
   *len = check_if_res(lex, ruby_bool_lit);
   if (*len > 0) {
      *tok = 20;
      return;
   }
   *len = check_if_res(lex, ruby_loop_op);
   if (*len > 0) {
      *tok = 21;
      return;
   }
   *len = check_if_res(lex, ruby_sep_op);
   if (*len > 0) {
      *tok = 22;
      return;
   }
   *len = check_if_res(lex, ruby_print_op);
   if (*len > 0) {
      *tok = 26;
      return;
   }
   *len = check_if_res(lex, ruby_class_op);
   if (*len > 0) {
      *tok = 27;
      return;
   }
   *len = check_if_res(lex, ruby_list_op);
   if (*len > 0) {
      *tok = 28;
      return;
   }
   *len = check_if_string(lex);
   if (*len > 0) {
      *tok = 23;
      return;
   }
   *len = check_if_id(lex);
   if (*len > 0) {
      *tok = 29;
      return;
   }
   *tok = 0;
   *len = 1;
  return;
}

// função que recebe um lexema e seu tamanho e o imprime (sem quebra de linha):
void print_lex(char* lex, int tam) {
   for (int i = 0; i < tam; i++) {
      std::cout << lex[i];
   }
}

// Função que imprime o token:
void print_tok(int tok) {
   std::cout << ruby_table[tok];
}