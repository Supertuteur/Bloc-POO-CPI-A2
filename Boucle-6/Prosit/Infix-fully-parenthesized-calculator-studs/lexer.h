#pragma once

#include <iostream>
#include <vector>

using namespace std;

enum TokenType {
  INTEGER,
  PLUSOPERATOR,
  MINUSOPERATOR,
  STAROPERATOR,
  SLASHOPERATOR,
  LPARENTHESIS,
  RPARENTHESIS,
};

class Token {
private:
  TokenType type;
  string value;

public:
  Token(TokenType type, string value) {
    this->type = type;
    this->value = value;
  }
  string getValue() { return value; }
  TokenType getType() { return type; }
  bool isType(TokenType type) { return this->type == type; }
  bool isOperator() {
    switch (type) {
    case TokenType::PLUSOPERATOR:
    case TokenType::MINUSOPERATOR:
    case TokenType::STAROPERATOR:
    case TokenType::SLASHOPERATOR:
      return true;
    default:
      return false;
    }
  }
  string toString() { return "(" + to_string(this->type) + ", " + value + ")"; }
};

class Lexer {
private:
  int readInteger(string expr, string &num) {
    int i;

    for (i = 0; i < expr.length() && isdigit(expr[i]); ++i) {
      num += expr[i];
    }

    return i - 1;
  }

public:
  vector<Token *> tokenize(string expr) {
    Token *curr;
    vector<Token *> tokens;

    for (int i = 0; i < expr.length(); ++i) {
      if (isdigit(expr[i])) {
        string num;

        i += readInteger(&expr[i], num);
        curr = new Token(TokenType::INTEGER, num);
      } else {
        switch (expr[i]) {
        case '(':
          curr = new Token(TokenType::LPARENTHESIS, "(");
          break;
        case ')':
          curr = new Token(TokenType::RPARENTHESIS, ")");
          break;
        case '+':
          curr = new Token(TokenType::PLUSOPERATOR, "+");
          break;
        case '-':
          curr = new Token(TokenType::MINUSOPERATOR, "-");
          break;
        case '*':
          curr = new Token(TokenType::STAROPERATOR, "*");
          break;
        case '/':
          curr = new Token(TokenType::SLASHOPERATOR, "/");
          break;
        case ' ':
        case '\t':
          continue;
        default:
          throw new exception();
        }
      }
      tokens.push_back(curr);
    }

    return tokens;
  }
};