#include "TinyArgParser.hpp"

int main(int argc, char** argv) {
    TinyArgParser::TinyArgParser arg_parser;

    // add_option 添加携带值的参数
    std::string filename = "default.txt";
    arg_parser.add_option("-f", filename, "文件路径");

    // 设置必选参数
    std::string timeout;
    arg_parser.add_option("-t", timeout, "超时时间")->required();

    // add_flag 添加不携带值的参数
    bool is_verbose = false;
    arg_parser.add_flag("--version", is_verbose, "版本号");

    // 解析 -h 命令
    if (argc == 2 && argv[1] != nullptr && std::string(argv[1]) == "-h") {
        arg_parser.print_usage(argv[0]);
        return 1;
    }

    // 解析命令行参数
    if (!arg_parser.parse(argc, argv)) {
        return 1;
    }

    // 打印参数
    std::cout << "filename   : " << filename << std::endl;
    std::cout << "timeout    : " << timeout << std::endl;
    std::cout << "is_verbose : " << is_verbose << std::endl;
    return 0;
}
