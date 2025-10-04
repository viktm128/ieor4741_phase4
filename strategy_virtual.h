#include "market_data.h"

class IStrategy {
public:
    IStrategy(double a1, double a2) : alpha1(a1), alpha2(a2) {}
    virtual ~IStrategy() {}
    virtual double on_tick(const Quote &q) = 0;
protected:
    double alpha1;
    double alpha2;
};

class SignalStrategyVirtual : public IStrategy {
public:
    SignalStrategyVirtual(double a1, double a2) : IStrategy(a1, a2) {}
    double on_tick(const Quote& q) override {
        const double mp = microprice(q);
        const double m  = mid(q);
        const double imb = imbalance(q);
        return alpha1 * (mp - m) + alpha2 * imb;
    }
};
