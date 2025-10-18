#pragma once

#include <thread>
#include <chrono>
#include <algorithm>
#include <map>
#include <deque>

#include "Order.h"

template <typename PriceType, typename OrderIDType>
class OrderBook {
public:
    OrderBook() = default;
    Order<PriceType, OrderIDType> &getBestBid() {
        return bids.rbegin()->second.front();
    };
    Order<PriceType, OrderIDType> &getBestAsk() {
        return asks.begin()->second.front();
    }
    bool hasBids() const {
        return bids.size() > 0;
    }
    bool hasAsks() const {
        return asks.size() > 0;
    }

    void executeIncoming(Order<PriceType, OrderIDType> &o) {
        if (o.is_buy) {
            while (matchIncomingBuy(o)) {}
        }
        else {
            while (matchIncomingSell(o)) {}
        }
    }
    bool matchIncomingBuy(Order<PriceType, OrderIDType> &o) {
        if (asks.size() > 0) {
            auto bestAsk = getBestAsk();
            if (o.price >= bestAsk.price) {
                int minQ = std::min(o.quantity, bestAsk.quantity);
                o.quantity -= minQ;
                bestAsk.quantity -= minQ;
                bestAsk.status = (bestAsk.quantity == 0 ? OrderStatus::Filled : OrderStatus::PartiallyFilled);
                if (bestAsk.status == OrderStatus::Filled) {
                    auto it = asks.find(bestAsk.price);
                    it->second.pop_front();
                    if (it->second.empty()) asks.erase(it);
                }

                o.status = (o.quantity == 0 ? OrderStatus::Filled : OrderStatus::PartiallyFilled);
                std::this_thread::sleep_for(std::chrono::nanoseconds(500)); // simulate sending out trade
                return (o.status != OrderStatus::Filled); // only return true if order hasn't been filled
            }
        }

        // create a bid for this order
        auto &q = bids[o.price];
        q.push_back(o);
        return false;
    }
    bool matchIncomingSell(Order<PriceType, OrderIDType> &o) {
        if (bids.size() > 0) {
            auto bestBid = getBestBid();
            if (o.price <= bestBid.price) {
                int minQ = std::min(o.quantity, bestBid.quantity);
                o.quantity -= minQ;
                bestBid.quantity -= minQ;
                bestBid.status = (bestBid.quantity == 0 ? OrderStatus::Filled : OrderStatus::PartiallyFilled);
                if (bestBid.status == OrderStatus::Filled) {
                    auto it = bids.find(bestBid.price);
                    it->second.pop_front();
                    if (it->second.empty()) bids.erase(it);
                }

                o.status = (o.quantity == 0 ? OrderStatus::Filled : OrderStatus::PartiallyFilled);
                std::this_thread::sleep_for(std::chrono::nanoseconds(500)); // simulate sending out trade
                return (o.status != OrderStatus::Filled); // only return true if order hasn't been filled
            }
        }

        // create a bid for this order
        auto &q = asks[o.price];
        q.push_back(o);
        return false;
    }


private:
    std::map<PriceType, std::deque<Order<PriceType, OrderIDType> > > bids;
    std::map<PriceType, std::deque<Order<PriceType, OrderIDType> > > asks;
};


