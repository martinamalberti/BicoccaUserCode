#include <boost/program_options.hpp>
#include <iostream>
#include <stdexcept>


#include <iostream>
#include <iterator>

#include <stdlib.h>


int main( int argc, char** argv ){
 
 boost::program_options::options_description desc("options");
 desc.add_options()
    ("help", "produce help message")
    ("test", boost::program_options::value<int>(), "test")
    ("include", "return include directory")
    ("libs", "return libs directory")
    ;
 
 boost::program_options::variables_map vm;        
 boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
 boost::program_options::notify(vm);    
 
 if (vm.count("help")) {
  std::cout << desc << "\n";
  return 1;
}

if (vm.count("include")) {
 system("echo -I$NTUPLEPKGINCLUDE");
 return 1;
}

if (vm.count("libs")) {
 system("echo -L$NTUPLEPKGLIB -lEvent");
 return 1;
}

if (vm.count("test")) {
 std::cout << "Test = " << vm["test"].as<int>() << ".\n";
 }
 else {
  std::cout << "Test was not set.\n";
 }
 
}