// Portable diagnostic engine test adapter; Windows users use the GUI instead.
#include "patcher.hpp"
#include "diagnostic.hpp"
#include <iostream>
int main(int argc,char** argv){
 try{
  dlc::require(argc>=3,"Usage: diagnostic-runner ISO REPORT [SECONDS]");
  svrdiag::capture(argv[1],argv[2],[](const std::string& event,svrdiag::Json args){
   args["event"]=event;std::cout<<args.dump()<<std::endl;
   std::string line;dlc::require(bool(std::getline(std::cin,line)),"Bridge disconnected");
   auto response=svrdiag::Json::parse(line);
   dlc::require(response.value("event","")!="error",response.dump());return response;
  },[](const auto& message){std::wcerr<<message<<L'\n';},argc>3?std::stoi(argv[3]):45);
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}
}
