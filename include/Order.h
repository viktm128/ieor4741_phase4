#pragma once
#include <string>
#include <memory>


enum class OrderStatus { New, Filled, PartiallyFilled, Cancelled };

template <typename PriceType, typename OrderIdType>
struct Order {
    OrderIdType id;
    std::string symbol;
    PriceType price;
    int quantity;
    bool is_buy;
    bool is_mine;
    OrderStatus status;

    Order(OrderIdType id, std::string sym, PriceType pr, int qty, bool buy, bool mine)
        : id(id), symbol(std::move(sym)), price(pr), quantity(qty), is_buy(buy),
          is_mine(mine), status(OrderStatus::New) {}
};
