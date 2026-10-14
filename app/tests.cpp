#include "patcher.hpp"
#include <iostream>
int main(int argc,char** argv){try{
 dlc::require(argc==3,"Usage: tests STOCK_ISO APPLICATION_DATA_FOLDER");
 auto data=dlc::fs::path(argv[2]);
 dlc::require(dlc::sha({})=="e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855","Empty SHA256");
 dlc::require(dlc::sha({'a','b','c'})=="ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad","SHA256 abc");
 dlc::Iso stock(argv[1]);
 for(auto option:std::vector<dlc::Options>{{{139,161},false},{{51,68},true},{{316},false}}){auto p=dlc::prepare(stock,data,option);auto elf=p.replacements.at("/PSP_GAME/SYSDIR/EBOOT.BIN");auto seg=dlc::u32(elf,52+15*32+4);for(int i=0;i<322;i++)dlc::require(elf.at(seg+0x5500+i)==option.ids.count(i),"Bitmap mismatch");for(auto a:assets)if(p.replacements.count(dlc::upper(a.path)))std::cout<<a.name<<" prepared\n";}
 for(int id:{-1,0,70,99,322,323}){bool rejected=false;try{dlc::prepare(stock,data,{{id},false});}catch(...){rejected=true;}dlc::require(rejected,"Unsafe ID accepted");}
 bool rejected=false;try{dlc::patch(argv[1],argv[1],data,{{139},false});}catch(...){rejected=true;}dlc::require(rejected,"Original overwrite not rejected");
 std::cout<<"Stock and eighteen-slot preparation, bitmap, invalid-ID and overwrite guards passed\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
