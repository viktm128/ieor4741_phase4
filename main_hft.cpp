#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include "Order.h"
#include "OrderBook.h"
#include "OrderManager.h"
#include "Timer.h"

using OrderType = Order<double, int>;
using OrderBookType = OrderBook<double, int>;
using OrderManagerType = OrderManager<double, int>;

int main() {
    OrderBookType ob;
    OrderManagerType om;
    const int num_ticks = 100'000;
    std::vector<long long> latencies;
    latencies.reserve(num_ticks);

    for (int i = 0; i < num_ticks; ++i) {
        Timer timer;
        timer.start();

        // Simulated tick + order match (replace with real logic)
        OrderType order(i, "AAPL", 150.0 + (i % 5), 100, i % 2 == 0, false);
        ob.executeIncoming(order);

        //Look at the book and decide if you'd like to send out some orders
        if (ob.hasAsks()) {
            auto bestAsk =  ob.getBestAsk();
            if (bestAsk.price < 152.0) om.place_order(true,"AAPL", bestAsk.price, bestAsk.quantity);
        }

        if (ob.hasBids()) {
            auto bestBid = ob.getBestBid();
            if (bestBid.price > 148.0) om.place_order(true,"AAPL", bestBid.price, bestBid.quantity);
        }


        latencies.push_back(timer.stop());
    }

    // Analyze latency
    const auto min = *std::min_element(latencies.begin(), latencies.end());
    const auto max = *std::max_element(latencies.begin(), latencies.end());
    const double mean = std::accumulate(latencies.begin(), latencies.end(), 0.0) / latencies.size();

    std::cout << "Tick-to-Trade Latency (nanoseconds):\n";
    std::cout << "Min: " << min << " | Max: " << max << " | Mean: " << mean << '\n';
}

