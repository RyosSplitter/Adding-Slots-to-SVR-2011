#include "patcher.hpp"
#include <iostream>
int main(int argc,char** argv){try{
 dlc::require(argc==3,"Usage: tests STOCK_ISO APPLICATION_DATA_FOLDER");dlc::Iso source(argv[1]);dlc::fs::path data(argv[2]);
 dlc::require(dlc::sha({})=="e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855","SHA256 empty vector");
 dlc::require(dlc::sha({'a','b','c'})=="ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad","SHA256 abc vector");
 dlc::Options all;all.install18=true;for(int id=51;id<=321;id++)if(dlc::vacant(id))all.ids.insert(id);dlc::require(all.ids.size()==151,"Candidate count");
 for(auto opt:std::vector<dlc::Options>{{{316},true},{{108,140,316},true},all}){
  auto result=dlc::prepare(source,data,opt);auto elf=result.replacements.at("/PSP_GAME/SYSDIR/EBOOT.BIN");auto offset=dlc::u32(elf,52+15*32+4);dlc::require(dlc::u32(elf,52+15*32+16)==0x30000,"Bank size");
  for(int id=0;id<322;id++)dlc::require(elf.at(offset+0x20000+id)==opt.ids.count(id),"DLC bitmap");size_t row=0;for(int id:opt.ids){auto name=dlc::slice(elf,offset+16+row++*840,148);dlc::require(dlc::u16(name,32)==id&&dlc::u16(name,0x74)==id&&dlc::u16(name,0x72)==1000+id,"Independent identity/sort fields");}
  dlc::require(dlc::u32(elf,0x089fe68c-0x088d8000+0x234)==0x3c020002&&dlc::u32(elf,0x089fe690-0x088d8000+0x234)==0x34440000,"ARC startup buffer not expanded");
 }
 for(int id:{-1,0,50,70,99,100,139,161,224,256,321,322,323}){bool rejected=false;try{dlc::prepare(source,data,{{id},true});}catch(...){rejected=true;}dlc::require(rejected,"Protected ID accepted");}
 bool rejected=false;try{dlc::patch(argv[1],argv[1],data,{{316},true});}catch(...){rejected=true;}dlc::require(rejected,"Original overwrite accepted");
 std::cout<<"PASS: SHA256, 151 candidates, independent records, selected-only DLC bitmap, ARC allocation, protected IDs, original overwrite guard\n";return 0;
 }catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
