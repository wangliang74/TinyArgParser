# TinyArgParser

C++11 单头文件命令行参数解析器，无第三方依赖。

## 示例代码

在项目根目录执行 `make` 编译示例代码 `examples/*.cpp`, 二进制文件目录 `build/`。

帮助：
```bash
$ ./build/simple -h
Usage: ./build/simple [options]

Options:
  -f                文件路径
  -t                超时时间
  --version         版本号
```

参数解析：
```bash
$ ./build/simple -t 10 -f config.txt --version
filename   : config.txt
timeout    : 10
is_verbose : 1
```

## 基本用法

将 `TinyArgParser.hpp` 放入头文件搜索路径后即可使用：

```cpp
#include "TinyArgParser.hpp"

int main(int argc, char** argv) {
    TinyArgParser::TinyArgParser arg_parser;
    std::string filename = "default.txt";
    std::string timeout;
    bool verbose = false;

    // add_option 添加携带值的参数
    arg_parser.add_option("-f", filename, "文件路径");

    // 设置必选参数
    arg_parser.add_option("-t", timeout, "超时时间")->required();

    // add_flag 添加不携带值的参数
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
```

## 许可证

采用 [MIT License](LICENSE)，许可文件包含英文原文和中文参考译文。
