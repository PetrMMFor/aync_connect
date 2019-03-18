#include <vector>
#include "Functions.h"
#include <fstream>
#include "CHttpConnect.h"
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

namespace po = boost::program_options;

int main(int argc, char** argv) {
    // Описание инструкциий и ключей
    po::options_description desc("General option");
    std::string config_name;
    desc.add_options()
            ("help,h","Show help")
            ("Config,c", po::value<std::string>(&config_name)->required(),"Input .cfg file's");

    po::variables_map vm;
    po::parsed_options parsed = po::command_line_parser(argc,argv).options(desc).allow_unregistered().run();
    po::store(parsed,vm);

    if(vm.count("help")){
        std::cout << desc << std::endl;
        return 1;
    }
    try {
        po::notify(vm);
    } catch (std::exception &e) {
        std::cout << "exception: " << e.what() <<std::endl;
        return -1;
    }
    std::cout << config_name << std::endl;


    //Считывание конфига из файла *.json
    //и запускается демон

    while(1) {
        daemon_work(get_config(config_name));
        sleep(10);
    }
    return 0;
}

