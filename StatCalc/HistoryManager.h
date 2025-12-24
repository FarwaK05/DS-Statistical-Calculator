#ifndef HISTORY_MANAGER_H
#define HISTORY_MANAGER_H
#include <stack>
#include <deque>
#include <string>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

struct CalcResult {
    std::string operation;
    double result;
};

class HistoryManager {
private:
    std::deque<CalcResult> history; // The bounded history (last 20)
    std::stack<CalcResult> undoStack; // Stack for undo feature
    const std::string filename = "history.json";

public:
    void addRecord(std::string op, double res) {
        CalcResult entry = {op, res};
        if (history.size() >= 20) history.pop_front();
        history.push_back(entry);
        undoStack.push(entry);
        saveToFile();
    }

    void undo() {
        if (!history.empty()) {
            history.pop_back();
            if(!undoStack.empty()) undoStack.pop();
            saveToFile();
        }
    }

    void saveToFile() {
        json j_list = json::array();
        for (auto& item : history) {
            j_list.push_back({{"op", item.operation}, {"res", item.result}});
        }
        std::ofstream file(filename);
        file << j_list.dump(4);
    }

    void loadFromFile() {
        std::ifstream file(filename);
        if (file.is_open() && file.peek() != std::ifstream::traits_type::eof()) {
            json j_list;
            file >> j_list;
            history.clear();
            for (auto& item : j_list) {
                history.push_back({item["op"], item["res"]});
            }
        }
    }

    json getHistoryAsJson() {
        json j_list = json::array();
        for (auto& item : history) {
            j_list.push_back({{"op", item.operation}, {"res", item.result}});
        }
        return j_list;
    }
};
#endif