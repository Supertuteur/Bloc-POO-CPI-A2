#include <cmath> 
#include <iostream>
#include <stdexcept> 
#include <vector>

using namespace std;

typedef float term_t;
typedef unsigned int exp_t;

template <int N> class Expression {
public:
  virtual term_t solve(term_t val[N]) = 0;
  virtual void differentiate(Expression<N> &exp, exp_t mask[N]) = 0;
  virtual string toString() = 0;
};

term_t power(term_t base, exp_t exp) {
  if (exp == 0)
    return 1.0f;

  for (exp_t i = 0; i < exp; ++i) {
    base *= base;
  }

  return base;
}

template <int N> class Monomial : public Expression<N> {
protected:
  term_t coeff;
  exp_t exps[N];

  constexpr static exp_t null_exps[N] = {0};

public:
  Monomial() { nullify(); }
  Monomial(term_t coeff) : coeff(coeff) { setExponents(null_exps); }
  Monomial(term_t coeff, const exp_t exps[N]) : coeff(coeff) {
    setExponents(exps);
  }
  term_t getCoeff() { return coeff; }
  void getExponents(exp_t exps[N]) {
    for (int i = 0; i < N; ++i) {
      exps[i] = this->exps[i];
    }
  }
  void nullify() {
    coeff = 0.0f;
    for (int i = 0; i < N; ++i) {
      exps[i] = 0;
    }
  }
  void setExponents(const exp_t exps[N]) {
    for (int i = 0; i < N; ++i) {
      this->exps[i] = exps[i];
    }
  }
  void differentiate(Expression<N> &exp, exp_t mask[N]) override {
    differentiate(dynamic_cast<Monomial<N> &>(exp), mask);
  }
  void differentiate(Monomial<N> &deriv, exp_t mask[N]) {
    bool isConstant = 1;

    deriv = *this;
    for (int i = 0; i < N; ++i) {
      if (mask[i] * exps[i] != 0) {
        deriv.coeff *= exps[i];
        deriv.exps[i] -= 1;

        isConstant &= 0;
      }
    }
    if (isConstant)
      deriv.nullify();
  }
  term_t solve(term_t vals[N]) override {
    term_t result = coeff;

    for (int i = 0; i < N; ++i) {
      result *= power(vals[i], exps[i]);
    }
    return result;
  };
  virtual string toString() override {
    string result = to_string(coeff);
    for (int i = 0; i < N; ++i) {
      if (exps[i] != 0)
        result += "x_" + to_string(i + 1) + "^" + to_string(exps[i]);
    }
    return result;
  }
};

const int N = 1;
class Monomial1D : public Monomial<N> {
public:
  Monomial1D() : Monomial() {}
  // this could be a constant
  Monomial1D(term_t coeff) : Monomial(coeff) {}
  Monomial1D(term_t coeff, const exp_t exps[N]) : Monomial(coeff, exps) {}
};

template <int N, int M> class Polynomial : public Expression<N> {
protected:
  Monomial<N> *terms[M];

public:
  Polynomial(Monomial<N> *terms[M]) {
    for (int i = 0; i < M; ++i) {
      this->terms[i] = terms[i];
    }
  }
  term_t solve(term_t vals[N]) override {
    term_t result = 0.0f;

    for (int i = 0; i < M; i++) {
      result += terms[i]->solve(vals);
    }
    return result;
  }
  void differentiate(Expression<N> &exp, exp_t mask[N]) override {
    differentiate(dynamic_cast<Polynomial<N, M> &>(exp), mask);
  }
  void differentiate(Polynomial<N, M> &deriv, exp_t mask[N]) {
    for (int i = 0; i < M; ++i) {
      terms[i]->differentiate(*deriv.terms[i], mask);
    }
  }
  string toString() override {
    string result = "";

    if (M > 0) {
      for (int i = 0; i < (M - 1); ++i) {
        result += terms[i]->toString();
        if (terms[i]->getCoeff() >= 0) {
          result += "+";
        }
      }
      result += terms[M - 1]->toString();
    }
    return result;
  }
};

template <int M> class Polynomial1D : public Polynomial<1, M> {
public:
  Polynomial1D(Monomial1D *mons[M]) : Polynomial<1, M>((Monomial<1> **)mons) {}
};

int main() {
  term_t vals[1] = {3.0f};
  const exp_t exps[1] = {2u};
  exp_t mask[1] = {1u};

  Monomial1D m1(12.4f, exps), m2(18.f, exps), dm3, *ms[2] = {&m1, &m2}, m4, m5;
  Monomial1D *ms1[2] = {&m4, &m5};
  
  m1.differentiate(dm3, mask);
  cout << dm3.toString() << endl;

  Polynomial1D<2> p1(ms), p1p(ms1), p2p(ms1);
  p1.differentiate(p1p, mask);
  cout << p1.toString() << endl;
  cout << p1p.toString() << endl;

  Expression<1> *exp = &p1;
  exp->differentiate(p2p, mask);
  cout << p2p.toString() << endl;

  return 0;
}