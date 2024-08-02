#include <iostream>
#include <stack>
#include <vector>

#include "lexer.h"
#include "parser.h"

using namespace std;

void free_tokens(vector<Token *> vec) {
  for (Token *i : vec) {
    delete i;
  }
}

void test(string expr, int expected) {
  Lexer lex;
  
  vector<Token *> tokens;
  tokens = lex.tokenize(expr);

  try {
    ExtendedParser parser = ExtendedParser(tokens);
    int res = parser.parse();

    cout << expr << "=" << to_string(res) << "=" << to_string(expected);
    
    if(res != expected) cout << " (Wrong ;-( ))";
    else cout << " (Correct :-) )";

    cout << endl;
  }
  catch(runtime_error err) {
    cout << "Syntax error: " << expr << endl;
  }

  free_tokens(tokens);
}

int main() {
  test("(((-1)*2)+3)", 1);
  test("((1+2)*(3+4))", 21);
  test("(5*3)", 15);
  test("(+1)", 1);
  test("(1 + (4 * 6))", 25);
  test("((4 * 6) + 1)", 25);
  test("(((-4) * 6) + 1)", -23);
  test("(((2+3)-10)*2)", -10);
  test("", 0); //syntax error
  test("( + )", 0); //syntax error
  


  return 0;
}