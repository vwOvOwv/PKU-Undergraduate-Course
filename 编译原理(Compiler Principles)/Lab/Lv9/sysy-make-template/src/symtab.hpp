#pragma once

#include <map>
#include <string>
#include <list>

struct SymbolInfo {
    bool is_const; 
    int const_val;
    std::string var_name;
    bool is_array = false;
    bool is_pointer = false;
    std::vector<int> dims;
};

class SymbolTable {
private:
    // 作用域栈
    std::list<std::map<std::string, SymbolInfo>> scopes;
    int var_counter = 0;

public:
    SymbolTable() {
        scopes.emplace_back();
    }

    void enter_scope() {
        scopes.emplace_back();
    }

    void exit_scope() {
        scopes.pop_back();
    }

    // 插入符号
    std::string push(const std::string& id, bool is_const, int const_val, bool is_array = false, bool is_pointer = false, std::vector<int> dims = {}) {
        // 全局变量保持原名
        // 局部变量使用 id + "_" + counter
        std::string unique_ir_name;
        if (scopes.size() == 1) { // 全局作用域
            unique_ir_name = id;
        } else {
            unique_ir_name = id + "_" + std::to_string(var_counter++);
        }
        
        // 构造 SymbolInfo
        SymbolInfo info;
        info.is_const = is_const;
        info.const_val = const_val;
        info.var_name = unique_ir_name;
        info.is_array = is_array;
        info.is_pointer = is_pointer;
        info.dims = dims;

        scopes.back()[id] = info;
        return unique_ir_name;
    }

    // 查找符号 (从内层向外层找)
    SymbolInfo* lookup(const std::string& id) {
        for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
            auto search = it->find(id);
            if (search != it->end()) {
                return &search->second;
            }
        }
        return nullptr;
    }
};