#define NOMINMAX
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
#include <vector>
#include <string>

using namespace httplib;
using json = nlohmann::json;

void saveCurrentBST(const std::vector<double>& data) {
    std::ofstream file("dataset.json");
    file << json(data).dump(4);
}

std::vector<double> loadCurrentBST() {
    std::ifstream file("dataset.json");
    if (file.is_open() && file.peek() != std::ifstream::traits_type::eof()) {
        json j; file >> j;
        return j.get<std::vector<double>>();
    }
    return {};
}

int main() {
    Server svr;
    BST dataset;
    HistoryManager history;

    std::vector<double> startData = loadCurrentBST();
    for(double d : startData) dataset.add(d);
    history.loadFromFile();

    svr.set_default_headers({
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });
    svr.Options(R"(.*)", [](const Request&, Response& res) { res.status = 200; });

    // --- DATASET ---
    svr.Post("/add-data", [&](const Request& req, Response& res) {
        auto j = json::parse(req.body);
        dataset.add(j["value"]);
        saveCurrentBST(dataset.getSorted());
        res.set_content("ok", "text/plain");
    });
    svr.Get("/dataset", [&](const Request&, Response& res) {
        res.set_content(json(dataset.getSorted()).dump(), "application/json");
    });
    svr.Post("/clear", [&](const Request&, Response& res) {
        dataset.clear(); saveCurrentBST({});
        res.set_content("ok", "text/plain");
    });

    // --- STANDARD STATS ---
    svr.Get("/calculate/mean", [&](const Request&, Response& res) {
        double v = Calculator::getMean(dataset.getSorted());
        history.addRecord("Mean", v);
        res.set_content(json({{"result", v}}).dump(), "application/json");
    });
    svr.Get("/calculate/median", [&](const Request&, Response& res) {
        double v = Calculator::getMedian(dataset.getSorted());
        history.addRecord("Median", v);
        res.set_content(json({{"result", v}}).dump(), "application/json");
    });
    svr.Get("/calculate/mode", [&](const Request&, Response& res) {
        auto m = Calculator::getMode(dataset.getSorted());
        double v = m.empty() ? 0 : m[0];
        history.addRecord("Mode", v);
        res.set_content(json({{"result", v}}).dump(), "application/json");
    });
    svr.Get("/calculate/sd", [&](const Request&, Response& res) {
        double v = Calculator::getStandardDeviation(dataset.getSorted());
        history.addRecord("Std Dev", v);
        res.set_content(json({{"result", v}}).dump(), "application/json");
    });

    // --- PROBABILITY (nCr, nPr, Binomial) ---
    svr.Post("/calculate/ncr", [&](const Request& req, Response& res) {
        auto j = json::parse(req.body);
        double v = (double)Calculator::nCr(j["n"], j["r"]);
        history.addRecord("nCr", v);
        res.set_content(json({{"result", v}}).dump(), "application/json");
    });
    svr.Post("/calculate/npr", [&](const Request& req, Response& res) {
        auto j = json::parse(req.body);
        double v = (double)Calculator::nPr(j["n"], j["r"]);
        history.addRecord("nPr", v);
        res.set_content(json({{"result", v}}).dump(), "application/json");
    });
    svr.Post("/calculate/binomial", [&](const Request& req, Response& res) {
        auto j = json::parse(req.body);
        double v = Calculator::binomialProb(j["n"], j["k"], j["p"]);
        history.addRecord("Binomial", v);
        res.set_content(json({{"result", v}}).dump(), "application/json");
    });

    // --- TWO EVENT SOLVER (MANUAL BUTTON LOGIC) ---
    svr.Post("/calculate/event-op", [&](const Request& req, Response& res) {
        try {
            auto j = json::parse(req.body);
            std::string op = j["op"];
            auto getV = [&](std::string k) { return j.contains(k) && j[k] != "" ? std::stod(j[k].get<std::string>()) : -1.0; };
            
            double pa = getV("pa"), pb = getV("pb"), pa_n = getV("pa_not"), pb_n = getV("pb_not"), inter = getV("inter");
            Calculator::normalize(pa, pb, pa_n, pb_n, inter);

            if (pa == -1 || pb == -1) { res.status = 400; return; }

            double result = 0; std::string name = "";
            if (op == "pa_not") { result = 1.0 - pa; name = "P(A')"; }
            else if (op == "pb_not") { result = 1.0 - pb; name = "P(B')"; }
            else if (op == "inter") { result = pa * pb; name = "P(AnB)"; }
            else if (op == "union") { result = pa + pb - (pa * pb); name = "P(AuB)"; }
            else if (op == "xor") { result = (pa + pb - (pa * pb)) - (pa * pb); name = "P(AxB)"; }
            else if (op == "neither") { result = 1.0 - (pa + pb - (pa * pb)); name = "P((AuB)')"; }

            history.addRecord(name, result);
            res.set_content(json({{"result", result}}).dump(), "application/json");
        } catch (...) { res.status = 400; }
    });

    svr.Get("/history", [&](const Request&, Response& res) {
        res.set_content(history.getHistoryAsJson().dump(), "application/json");
    });
    svr.Post("/undo", [&](const Request&, Response& res) { history.undo(); res.status = 200; });
    svr.Post("/redo", [&](const Request&, Response& res) { history.redo(); res.status = 200; });

    std::cout << "SERVER READY: Event Solver Active" << std::endl;
    svr.listen("0.0.0.0", 8080);
    return 0;
}