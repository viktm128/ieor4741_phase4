#pragma once
#include "market_data.h"


// CRTP base: static dispatch
template <typename derived>
struct StrategyBase {
    // Non-virtual: will inline to Derived::on_tick_impl if small enough
    double on_tick(const Quote& q) const {
        return static_cast<const derived*>(this)->on_tick_impl(q);
    }
};


// Same behavior as SignalStrategyVirtual but via CRTP
struct SignalStrategyCRTP : public StrategyBase<SignalStrategyCRTP> {
    double alpha1;
    double alpha2;
    explicit SignalStrategyCRTP(const double a1, const double a2) : alpha1(a1), alpha2(a2) {}

    // Derived "impl"
    double on_tick_impl(const Quote& q) const {
        const double mp = microprice(q);
        const double m  = mid(q);
        const double imb = imbalance(q);
        return alpha1 * (mp - m) + alpha2 * imb;
    }
};