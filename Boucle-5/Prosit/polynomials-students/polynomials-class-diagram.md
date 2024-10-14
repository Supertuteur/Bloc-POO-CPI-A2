classDiagram
    class Client {
        +main() int$
    }

    class Expression~N~ {
        +solve(val[N] : term_t) term_t
        +differentiate(Expression~N~& exp, exp_t mask[N]) void
        +toString() string
    }

    class Monomial~N~ {
        -coeff : term_t
        -exps[N] : exp_t 
        +Monomial()
        +Monomial(coeff : term_t)
        +Monomial(coeff : term_t, exps[N] : const exp_t)
        +getCoeff() term_t
        +getExponents(exps[N] : exp_t ) void
        +nullify() void
        +setExponents(exps[N] : const exp_t ) void
        +differentiate(exp : Expression~N~&, mask[N] : exp_t) void
        +differentiate(deriv : Monomial~N~&, mask[N] : exp_t) void
        +solve(vals[N] : term_t) term_t
        +toString() string
    }

    class Monomial1D~1~ {
        +Monomial1D()
        +Monomial1D(coeff : term_t)
        +Monomial1D(coeff : term_t, exps[N] : const exp_t)
    }

    class Polynomial~N, M~ {
        -terms[M] : (Monomial~N~*)
        +Polynomial(terms[M] : Monomial~N~*)
        +solve(vals[N] : term_t) term_t
        +differentiate(exp : Expression~N~&, mask[N] : exp_t) void
        +differentiate(deriv : Polynomial~N,M~&, mask[N] : exp_t) void
        +toString() string
    }

    class Polynomial1D~1, M~ {
        +Polynomial1D(mons[M] : Monomial1D*)
    }

    <<Abstract>> Expression
    Client ..> Monomial1D
    Client ..> Polynomial1D
    Client ..> Expression
    Expression <|-- Monomial
    Monomial <|-- Monomial1D
    Expression <|-- Polynomial
    Polynomial <|-- Polynomial1D