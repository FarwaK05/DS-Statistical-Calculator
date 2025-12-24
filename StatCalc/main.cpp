#define _WIN32_WINNT 0x0A00
#define CPPHTTPLIB_NO_LIBMMAP
#include <winsock2.h>

#include "httplib.h"
#include "json.hpp"
#include "BST.h"
#include "Calculator.h"
#include "HistoryManager.h"
#include <iostream>

using namespace httplib;
using json = nlohmann::json;

int main() {
    Server svr;
    BST dataset;
    HistoryManager history;
    history.loadFromFile();

    std::cout << "Starting Statistical Calculator Backend..." << std::endl;

    // --- SIMPLE & UNIVERSAL CORS FIX ---
    // This tells the browser to allow React to talk to C++
    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });

    // Handle the 'pre-flight' request that browsers send automatically
    svr.Options(R"(.*)", [](const Request&, Response& res) {
        res.status = 200;
    });
    // ------------------------------------

    // 1. Add number to BST
    svr.Post("/add-data", [&](const Request& req, Response& res) {
        try {
            auto j = json::parse(req.body);
            dataset.add(j["value"]);
            std::cout << "Added to BST: " << j["value"] << std::endl;
            res.set_content("Success", "text/plain");
        } catch (...) {
            res.status = 400;
        }
    });

    // 2. Statistical: Mean
    svr.Get("/calculate/mean", [&](const Request&, Response& res) {
        auto data = dataset.getSorted();
        double val = Calculator::getMean(data);
        history.addRecord("Mean", val);
        res.set_content(json({{"result", val}}).dump(), "application/json");
    });

    // 3. Statistical: Median
    svr.Get("/calculate/median", [&](const Request&, Response& res) {
        auto data = dataset.getSorted();
        double val = Calculator::getMedian(data);
        history.addRecord("Median", val);
        res.set_content(json({{"result", val}}).dump(), "application/json");
    });

    // 4. Probability: nCr
    svr.Post("/calculate/ncr", [&](const Request& req, Response& res) {
        try {
            auto j = json::parse(req.body);
            long long val = Calculator::nCr(j["n"], j["r"]);
            history.addRecord("nCr", (double)val);
            res.set_content(json({{"result", val}}).dump(), "application/json");
        } catch (...) { res.status = 400; }
    });

    // 5. History
    svr.Get("/history", [&](const Request&, Response& res) {
        res.set_content(history.getHistoryAsJson().dump(), "application/json");
    });

    // 6. Undo (Stack usage)
    svr.Post("/undo", [&](const Request&, Response& res) {
        history.undo();
        res.set_content("Undone", "text/plain");
    });

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "SUCCESS: Backend is active at http://localhost:8080" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;

    svr.listen("0.0.0.0", 8080);
    return 0;
}