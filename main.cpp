#include <iostream>
#include <stdlib.h>

#include "linda_communicator.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main( int argc, char* argv[] ) {
	LindaCommunicator lc("./text");
	int timeout = 10000;
	po::variables_map vm;
	po::options_description desc("Allowed options");
	try{
		desc.add_options()
			("help,h",					"produce help message")
			("read,r",	po::value<std::string>(), "show dependepcies on a screen")
			("output,o",	po::value<std::string>(), "parse only files with *cfg suffix")
			("input,i", po::value<std::string>(), "folder to parse")
			("console,c", 					"enter interactive mode")
			("show,s", 						"show all lines in a file")
			("file,f", po::value<std::string>(), "set file name")
			("timeout,t", po::value<int>(), "set timeout value")
		;

		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm);
	}
	catch( po::error& e )
	{
		std::cout<<e.what()<<std::endl;
		std::cout<<desc<<std::endl;
		return 0;
	}

	if( vm.count("help" )  ){
			std::cout<<desc<<std::endl;
			return 0;
	}

	if( vm.count("file") ) {
		auto file = vm["file"].as<std::string>();
		lc.setFile( file );
	}

	if( vm.count("show") ) {
		auto allLines = lc.showAll();
		for( const auto& line : allLines )
			std::cout<<"Line: "<<line<<std::endl;
	}

	if( vm.count("timeout") ) {
		timeout = vm["timeout"].as<int>();
	}

	if( vm.count("read") ) {
		auto linda = vm["read"].as<std::string>();
		std::cout<<lc.read( linda, timeout ).toString()<<std::endl;
	}
	else if( vm.count("output") ) {
		auto linda = vm["output"].as<std::string>();
		lc.output( linda );
		std::cout<<"success"<<std::endl;
	}
	else if( vm.count("input") ) {
		auto linda = vm["input"].as<std::string>();
		std::cout<<lc.input( linda, timeout ).toString()<<std::endl;
	}
	else if( vm.count("console") ) {
		while(true) {
			std::cout<<"Choose function"<<std::endl;
			std::cout<<"1. Read"<<std::endl;
			std::cout<<"2. Output"<<std::endl;
			std::cout<<"3. Input"<<std::endl;
			std::cout<<"0. Exit"<<std::endl;
			int choice;
			std::cin>>choice;
			if( choice == 0 ) {
				return 0;
			}
			if( choice > 3 ) {
				std::cout<<"Wrong number. Exiting."<<std::endl;
				return 0;
			}
			std::cout<<"Write in your linda"<<std::endl;
			std::string s;
			std::getline( std::cin, s );
			if( choice == 1 ) {
				auto lt = lc.read( s, timeout );
				std::cout<<lt.toString()<<std::endl;
			}
			else if( choice == 2 ) {
				lc.output( s );
			}
			else if( choice == 3 ) {
				auto lt = lc.input( s, timeout );
				std::cout<<lt.toString()<<std::endl;
			}
		}
	}

	return 0;
}
