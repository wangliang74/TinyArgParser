#pragma once

#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace TinyArgParser {

class Argument {
public:
    Argument(const std::string& name, std::string& value, const std::string& help)
        : name_(name), value_(&value), help_(help) {}

    Argument(const std::string& name, bool& value, const std::string& help)
        : name_(name), flag_value_(&value), help_(help) {}

    const std::string& get_name() const {
        return name_;
    }

    const std::string& get_help() const {
        return help_;
    }

    bool is_flag() const {
        return (flag_value_ != nullptr);
    }

    Argument* required(bool enabled = true) {
        required_ = enabled;
        return this;
    }

    bool is_required() const {
        return required_;
    }

    void assign(const std::string& value) {
        if (value_ == nullptr) {
            throw std::invalid_argument("cannot assign a string to flag: " + name_);
        }
        *value_ = value;
        is_assigned_ = true;
    }

    void assign(const char* value) {
        if (value == nullptr) {
            throw std::invalid_argument("null value for option: " + name_);
        }
        assign(std::string(value));
    }

    void assign(bool flag_value) {
        if (flag_value_ == nullptr) {
            throw std::invalid_argument("cannot assign a bool to option: " + name_);
        }
        *flag_value_ = flag_value;
        is_assigned_ = true;
    }

    bool is_assigned() const {
        return is_assigned_;
    }

private:
    std::string name_;
    std::string* value_ = nullptr;
    bool* flag_value_ = nullptr;
    std::string help_;
    bool required_ = false;
    bool is_assigned_ = false;
};

class TinyArgParser {
public:
    TinyArgParser() = default;
    TinyArgParser(const TinyArgParser&) = delete;
    TinyArgParser& operator=(const TinyArgParser&) = delete;

    Argument* add_option(const std::string& name, std::string& value, const std::string& help = "") {
        check_argument(name);
        args_.push_back(std::unique_ptr<Argument>(new Argument(name, value, help)));
        return args_.back().get();
    }

    Argument* add_flag(const std::string& name, bool& value, const std::string& help = "") {
        check_argument(name);
        args_.push_back(std::unique_ptr<Argument>(new Argument(name, value, help)));
        return args_.back().get();
    }

    bool parse(int argc, char** argv) {
        if (argc < 1 || argv == nullptr || argv[0] == nullptr) {
            return fail("invalid argc/argv");
        }

        for (int i = 1; i < argc; ++i) {
            if (argv[i] == nullptr) {
                return fail("null argument");
            }
            const std::string name(argv[i]);

            Argument* arg = find(name);
            if (arg == nullptr) {
                return fail("unknown argument: " + name);
            }
            if (arg->is_assigned()) {
                return fail("repeat argument: " + name);
            }

            if (arg->is_flag()) {
                arg->assign(true);
            } else {
                if (i + 1 >= argc || argv[i + 1] == nullptr) {
                    return fail("missing value for: " + name);
                }
                arg->assign(std::string(argv[++i]));
            }
        }

        for (std::size_t i = 0; i < args_.size(); ++i) {
            if (args_[i]->is_required() && !args_[i]->is_assigned()) {
                return fail("require argument: " + args_[i]->get_name());
            }
        }
        return true;
    }

    void print_usage(std::string program) const {
        auto program_name = program.empty() ? "program" : program;
        std::cout << "Usage: " << program_name << " [options]\n\n";
        std::cout << "Options:\n";
        const auto original_flags = std::cout.flags();
        for (const auto& arg : args_) {
            std::cout << "  " << std::left << std::setw(16)
                      << arg->get_name()
                      << "  "
                      << arg->get_help()
                      << "\n";
        }
        std::cout.flags(original_flags);
    }

private:
    bool fail(const std::string& message) {
        std::cerr << message << std::endl;
        return false;
    }

    Argument* find(const std::string& name) const {
        for (const auto& arg : args_) {
            if (arg->get_name() == name) {
                return arg.get();
            }
        }
        return nullptr;
    }

    void check_argument(const std::string& name) const {
        if (name.empty()) {
            throw std::invalid_argument("empty option name");
        }
        Argument* arg = find(name);
        if (arg != nullptr) {
           throw std::invalid_argument("duplicate option name: " + name);
        }
    }

    std::vector<std::unique_ptr<Argument>> args_;
};

} // namespace TinyArgParser
