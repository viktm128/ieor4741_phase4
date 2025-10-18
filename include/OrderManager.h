#pragma once

#include <map>
#include <memory>
#include <string>
#include <iostream>

#include "Order.h"

template <typename PriceType, typename OrderIDType>
class OrderManager {
public:
    OrderIDType place_order(bool buy, std::string sym, PriceType px, int qty) {
        auto ptr = std::make_unique<Order<PriceType, OrderIDType> >(next_id, sym, px, qty, buy, true);
        orders[next_id] = std::move(ptr);
        return next_id++;
    }
    void cancel(OrderIDType id) {
        if (orders[id]->is_mine) orders[id]->status = OrderStatus::Cancelled;
    }
    void handle_fill(OrderIDType id, int filled_qty) {
        if (orders.contains(id) && filled_qty > 0 && filled_qty <= orders[id]->quantity) {
            orders[id]->quantity -= filled_qty;
            if (orders[id]->quantity == 0) orders[id]->status = OrderStatus::Filled;
            else orders[id]->status = OrderStatus::PartiallyFilled;
        }
    }
    void print_active_orders() const {
        for (const auto &pair : orders) {
            const Order<PriceType, OrderIDType>* o = pair.second.get();
            if (o->is_mine && (o->status == OrderStatus::New || o->status == OrderStatus::PartiallyFilled)) {
                std::cout << "Order " << pair.second-> id << ": " << (o->is_buy ? "Buy" : "Sell") << " " << o->quantity << " @ " << o->price << std::endl;
            }
        }
    }
private:
    std::map<int, std::unique_ptr< Order<PriceType, OrderIDType> >> orders;
    OrderIDType next_id = 0;
};

