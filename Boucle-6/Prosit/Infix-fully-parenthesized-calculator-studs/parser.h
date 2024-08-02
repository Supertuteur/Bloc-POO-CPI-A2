#pragma once

#include <iostream>
#include <stack>

#include "lexer.h"

using namespace std;

class Expression;

stack<Expression *> operators;
stack<int> operands;

class Expression {
public:
  virtual void interpret() = 0;
  virtual ~Expression() {}
};

class OpenExpression : public Expression {
  void interpret() { ; }
};

class UnaryOp : public Expression {
public:
  virtual int compute(int right) = 0;

  void interpret() override {
    /* TO BE COMPLETED */
  }
};

class MinusUnaryOp : public UnaryOp {
public:
      /* TO BE COMPLETED */

};

class PlusUnaryOp : public UnaryOp {
public:
      /* TO BE COMPLETED */

};

class BinaryOp : public Expression {
public:
  virtual int compute(int left, int right) = 0;

  void interpret() override {
        /* TO BE COMPLETED */

  }
};

class MinusBinaryOp : public BinaryOp {
public:
    /* TO BE COMPLETED */
};

class PlusBinaryOp : public BinaryOp {
public:
    /* TO BE COMPLETED */
};

class StarBinaryOp : public BinaryOp {
public:
    /* TO BE COMPLETED */
};

class DivBinaryOp : public BinaryOp {
public:
    /* TO BE COMPLETED */
};


enum OperatorType { UNARY, BINARY, TERNARY };

class OperatorFactory {
public:
  static Expression *build(TokenType tokenType, OperatorType operatorType) {
    Expression *expr;
    switch (operatorType) {
    case OperatorType::UNARY:
      switch (tokenType) {
      case TokenType::PLUSOPERATOR:
        expr = new PlusUnaryOp();
        break;
      case TokenType::MINUSOPERATOR:
        expr = new MinusUnaryOp();
        break;
      default:
        throw new runtime_error("Unhandled unary operator.");
      }
      break;
    case OperatorType::BINARY:
      switch (tokenType) {
      case TokenType::PLUSOPERATOR:
        expr = new PlusBinaryOp();
        break;
      case TokenType::MINUSOPERATOR:
        expr = new MinusBinaryOp();
        break;
      case TokenType::STAROPERATOR:
        expr = new StarBinaryOp();
        break;
      case TokenType::SLASHOPERATOR:
        expr = new DivBinaryOp();
        break;
      default:
        throw new runtime_error("Unhandled binary operator.");
      }
      break;
    default:;
    }

    /* TO BE COMPLETED */

  }
};

/*
  La grammaire LL(1):

  BASE_EXP -> ( EXP ) |  LITERAL
  EXP -> BASE_EXP BINOP BASE_EXP | UNOP BASE_EXP
  BINOP -> - | + | * | /
  UNOP -> - | +
  LITERAL -> [0-9]+
*/

/*
  For the motivated one's, the following grammar handles the more complex cases
  of fully parenthesized infix arithmetic expressions. This grammar has
  specifically been designed to be "code tractable", meaning that solving
  special cases (such as ((1)) ) SHOULD be eased. E' -> E E -> ( H H -> G O ) H
  -> L I ) G -> U K G -> E I -> B F I -> '' O -> B F O -> '' K -> E K -> L F ->
  L F -> E U -> + U -> - B -> U B -> * B -> / L -> 1

  Can be tested on https://jsmachines.sourceforge.net/machines/ll1.html
*/

class ExtendedParser {
  vector<Token *> tokens;
  int idx;

public:
  ExtendedParser(vector<Token *> tokens) {
    this->tokens = tokens;

    reset();
  }

  Token *next() {
    if (idx < tokens.size()) {
      return tokens[idx++];
    }

    throw runtime_error("Syntax error.");
  }
  void rewind() { idx--; }
  void reset() { idx = 0; }
  int parse() {
    reset();

    consumeBaseExpression();

    return operands.top();
  }
  void consumeLeftParenthesis() {
    Token *curr = next();

    if (!curr->isType(TokenType::LPARENTHESIS)) {
      throw runtime_error("Expected opening parenthesis.");
    }
  }
  void consumeRightParenthesis() {
    Token *curr = next();

    if (!curr->isType(TokenType::RPARENTHESIS)) {
      throw runtime_error("Expected closing parenthesis.");
    }
  }
  bool consumeBinop() {
    Token *curr = next();

    if (curr->isType(TokenType::MINUSOPERATOR) ||
        curr->isType(TokenType::PLUSOPERATOR) ||
        curr->isType(TokenType::STAROPERATOR) ||
        curr->isType(TokenType::SLASHOPERATOR)) {
      Expression *expr;

      /* TO BE COMPLETED */

      return true;
    }
    rewind();

    return false;
  }
  bool consumeUnop() {
    Token *curr = next();

    if (curr->isType(TokenType::MINUSOPERATOR) ||
        curr->isType(TokenType::PLUSOPERATOR)) {
      Expression *expr;

      /* TO BE COMPLETED */

      return true;
    }
    rewind();

    return false;
  }
  bool consumeLitteral() {
    Token *curr = next();

    if (curr->isType(TokenType::INTEGER)) {
      /* TO BE COMPLETED */

      return true;
    }
    rewind();

    return false;
  }
  void consumeBaseExpression() {
    if (!consumeLitteral()) {
      /* TO BE COMPLETED */

      solve();
    }
  }
  void consumeExpression() {
    if (!consumeUnop()) {
      consumeBaseExpression();
      consumeBinop();
    }

    consumeBaseExpression();
  }
  void solve() {
    Expression *expr;

    if (operators.size() >= 1) {
      expr = operators.top();
      operators.pop();

      expr->interpret();
      delete expr;
    } else
      throw runtime_error("Syntax error.");
  }
};

/* Poorly implemented parser : 

class Parser {
public:
  void parse(vector<Token *> tokens) {
    Expression *expr;

    for (int i = 0; i < tokens.size(); ++i) {
      Token *curr = tokens[i];

      if (curr->isType(TokenType::LPARENTHESIS)) {
        operators.push(new OpenExpression());
        if (i + 1 < tokens.size()) {
          if (tokens[i + 1]->isOperator()) {
            curr = tokens[++i];

            expr = OperatorFactory::build(curr->getType(), OperatorType::UNARY);
            operators.push(expr);
          }
        } else
          throw new runtime_error("Expected token after opening parenthesis.");
      } else if (tokens[i]->isType(TokenType::INTEGER)) {
        operands.push(stoi(tokens[i]->getValue()));
      } else if (tokens[i]->isOperator()) {
        if (!(i - 1 > 0 && i + 1 < tokens.size() &&
              (tokens[i - 1]->isType(TokenType::INTEGER) ||
               tokens[i - 1]->isType(TokenType::RPARENTHESIS)) &&
              (tokens[i + 1]->isType(TokenType::INTEGER) ||
               tokens[i + 1]->isType(TokenType::LPARENTHESIS)))) {
          throw new runtime_error("Expected two operands for binary operator.");
        }

        expr =
            OperatorFactory::build(tokens[i]->getType(), OperatorType::BINARY);

        operators.push(expr);
      } else if (tokens[i]->isType(TokenType::RPARENTHESIS)) {
        if (operators.size() >= 2) {
          expr = operators.top();
          operators.pop();

          expr->interpret();
          delete expr;

          // pop left parenthesis
          expr = operators.top();
          operators.pop();

          delete expr;
        } else
          throw new runtime_error("Syntax error.");
      }
    }
    if (operators.size() != 0) {
      throw new runtime_error("Syntax error.");

      while (operators.size() != 0) {
        expr = operators.top();
        operators.pop();

        delete expr;
      }
    }

    cout << operands.top() << endl;
  }
};*/
