#define _WIN32_WINNT 0x0A00
#define CPPHTTPLIB_NO_LIBMMAP
#include <winsock2.h>
#include "httplib.h"
#include "json.hpp"
#include "BST.h"
#include "Calculator.h"
#include "HistoryManager.h"
#include <iostream>
#include <fstream>

using namespace httplib;
using json = nlohmann::json;

// Helper to save/load the BST numbers
void saveDataset(const std::vector<double>& data) {
    std::ofstream file("dataset.json");
    file << json(data).dump(4);
}

std::vector<double> loadDataset() {
    std::ifstream file("dataset.json");
    if (file.is_open()) {
        json j;
        file >> j;
        return j.get<std::vector<double>>();
    }
    return {};
}

int main() {
   Server svr;
    BST dataset;
    HistoryManager history;
    history.loadFromFile();

    // CORS logic (Same as before)
    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });
    svr.Options(R"(.*)", [](const Request&, Response& res) { res.status = 200; });

    // 1. Dataset History Persistence
    svr.Get("/dataset", [&](const Request&, Response& res) {
        res.set_content(json(dataset.getSorted()).dump(), "application/json");
    });

    // 1. Add number and SAVE to file
    svr.Post("/add-data", [&](const Request& req, Response& res) {
        try {
            auto j = json::parse(req.body);
            double val = j["value"];
            dataset.add(val);
            saveDataset(dataset.getSorted()); // Save to file
            res.set_content(json({{"status", "success"}}).dump(), "application/json");
        } catch (...) {
            res.status = 400;
            res.set_content(json({{"error", "Invalid data"}}).dump(), "application/json");
        }
    });

    svr.Get("/dataset", [&](const Request&, Response& res) {
        res.set_content(json(dataset.getSorted()).dump(), "application/json");
    });

    svr.Post("/clear", [&](const Request&, Response& res) {
        dataset.clear();
        saveDataset({}); // Clear file
        res.set_content(json({{"status", "cleared"}}).dump(), "application/json");
    });

    // 2. Binomial Fix: Ensure keys 'n', 'k', 'p' match React
    svr.Post("/calculate/binomial", [&](const Request& req, Response& res) {
        try {
            auto j = json::parse(req.body);
            double val = Calculator::binomialProb(j["n"], j["k"], j["p"]);
            history.addRecord("Binomial", val);
            res.set_content(json({{"result", val}}).dump(), "application/json");
        } catch (...) {
            res.status = 400;
            res.set_content(json({{"result", 0}, {"error", "Check inputs"}}).dump(), "application/json");
        }
    });

    // --- OTHER ENDPOINTS REMAIN THE SAME ---
    svr.Get("/calculate/mean", [&](const Request&, Response& res) {
        auto data = dataset.getSorted();
        double val = Calculator::getMean(data);
        history.addRecord("Mean", val);
        res.set_content(json({{"result", val}}).dump(), "application/json");
    });
    svr.Get("/calculate/median", [&](const Request&, Response& res) {
        auto data = dataset.getSorted();
        double val = Calculator::getMedian(data);
        history.addRecord("Median", val);
        res.set_content(json({{"result", val}}).dump(), "application/json");
    });
    svr.Get("/calculate/mode", [&](const Request&, Response& res) {
        auto data = dataset.getSorted();
        auto modes = Calculator::getMode(data);
        double result = modes.empty() ? 0 : modes[0];
        history.addRecord("Mode", result);
        res.set_content(json({{"result", result}}).dump(), "application/json");
    });
    svr.Get("/calculate/sd", [&](const Request&, Response& res) {
        auto data = dataset.getSorted();
        double val = Calculator::getStandardDeviation(data);
        history.addRecord("Std Dev", val);
        res.set_content(json({{"result", val}}).dump(), "application/json");
    });
    svr.Post("/calculate/ncr", [&](const Request& req, Response& res) {
        auto j = json::parse(req.body);
        long long val = Calculator::nCr(j["n"], j["r"]);
        history.addRecord("nCr", (double)val);
        res.set_content(json({{"result", val}}).dump(), "application/json");
    });
    svr.Post("/calculate/npr", [&](const Request& req, Response& res) {
        auto j = json::parse(req.body);
        long long val = Calculator::nPr(j["n"], j["r"]);
        history.addRecord("nPr", (double)val);
        res.set_content(json({{"result", val}}).dump(), "application/json");
    });
    svr.Get("/history", [&](const Request&, Response& res) {
        res.set_content(history.getHistoryAsJson().dump(), "application/json");
    });
    svr.Post("/undo", [&](const Request&, Response& res) {
        history.undo();
        res.set_content(json({{"status", "ok"}}).dump(), "application/json");
    });
    svr.Post("/redo", [&](const Request&, Response& res) {
        history.redo();
        res.set_content(json({{"status", "ok"}}).dump(), "application/json");
    });

    std::cout << "SERVER UPDATED: Persistence Active!" << std::endl;
    svr.listen("0.0.0.0", 8080);
    return 0;
}