#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <vector>
namespace dlc {
namespace fs=std::filesystem;
using Bytes=std::vector<uint8_t>;
inline void require(bool b,const std::string& s){if(!b)throw std::runtime_error(s);}
inline uint32_t u32(const Bytes& b,size_t p){require(p+4<=b.size(),"Truncated structure");return b[p]|uint32_t(b[p+1])<<8|uint32_t(b[p+2])<<16|uint32_t(b[p+3])<<24;}
inline uint16_t u16(const Bytes& b,size_t p){require(p+2<=b.size(),"Truncated structure");return b[p]|uint16_t(b[p+1])<<8;}
inline void put32(Bytes& b,size_t p,uint32_t v){require(p+4<=b.size(),"Write outside structure");for(int i=0;i<4;i++)b[p+i]=v>>(8*i);}
inline void put16(Bytes& b,size_t p,uint16_t v){require(p+2<=b.size(),"Write outside structure");b[p]=v;b[p+1]=v>>8;}
inline void both32(Bytes& b,size_t p,uint32_t v){put32(b,p,v);for(int i=0;i<4;i++)b[p+4+i]=v>>(24-8*i);}
inline Bytes load(const fs::path& p){std::ifstream f(p,std::ios::binary);require(bool(f),"Cannot read "+p.string());auto size=fs::file_size(p);require(size<128*1024*1024,"Unexpectedly large patch input");Bytes b(size);f.read((char*)b.data(),b.size());require(bool(f)||b.empty(),"Read failed");return b;}
inline Bytes slice(const Bytes& b,size_t start,size_t count){require(start<=b.size()&&count<=b.size()-start,"Invalid slice");return Bytes(b.begin()+start,b.begin()+start+count);}
inline void save(const fs::path& p,const Bytes& b){std::ofstream f(p,std::ios::binary);require(bool(f),"Cannot write output");f.write((const char*)b.data(),b.size());require(bool(f),"Output write failed");}
// Small portable SHA-256, used to reject unsupported inputs and verify outputs.
inline std::string sha(const Bytes& input){
 static const uint32_t k[]={0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2};
 Bytes b=input;uint64_t bits=uint64_t(b.size())*8;b.push_back(128);while(b.size()%64!=56)b.push_back(0);for(int i=7;i>=0;i--)b.push_back(bits>>(8*i));
 uint32_t h[]={0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};auto r=[](uint32_t x,int n){return(x>>n)|(x<<(32-n));};
 for(size_t p=0;p<b.size();p+=64){uint32_t w[64];for(int i=0;i<16;i++)w[i]=uint32_t(b[p+4*i])<<24|uint32_t(b[p+4*i+1])<<16|uint32_t(b[p+4*i+2])<<8|b[p+4*i+3];for(int i=16;i<64;i++){auto a=w[i-15],z=w[i-2];w[i]=w[i-16]+(r(a,7)^r(a,18)^(a>>3))+w[i-7]+(r(z,17)^r(z,19)^(z>>10));}uint32_t a=h[0],c=h[2],d=h[3],e=h[4],f=h[5],g=h[6],z=h[7],bb=h[1];for(int i=0;i<64;i++){uint32_t t=z+(r(e,6)^r(e,11)^r(e,25))+((e&f)^(~e&g))+k[i]+w[i],s=(r(a,2)^r(a,13)^r(a,22))+((a&bb)^(a&c)^(bb&c));z=g;g=f;f=e;e=d+t;d=c;c=bb;bb=a;a=t+s;}uint32_t v[]={a,bb,c,d,e,f,g,z};for(int i=0;i<8;i++)h[i]+=v[i];}
 const char* hex="0123456789abcdef";std::string out;for(auto v:h)for(int i=7;i>=0;i--)out+=hex[(v>>(i*4))&15];return out;
}
inline std::string upper(std::string s){for(char& c:s)if(c>='a'&&c<='z')c-=32;return s;}
struct Entry{std::string path;uint32_t sector,size;uint64_t record;};
class Iso {
 std::ifstream stream;uint64_t length;std::set<uint64_t> visited;
 void directory(uint32_t sec,uint32_t size,std::string prefix,bool joliet,int depth){
  require(depth<32&&size<16*1024*1024,"Invalid directory size/depth");uint64_t tag=(uint64_t(joliet)<<63)|uint64_t(sec);require(visited.insert(tag).second,"Directory cycle/alias unsupported");auto data=read(uint64_t(sec)*2048,size);
  for(size_t p=0;p<data.size();){auto len=data[p];if(!len){p=((p/2048)+1)*2048;continue;}require(len>=34&&p+len<=data.size(),"Bad directory record");auto n=data[p+32];require(33+n<=len,"Bad directory name");if(n==1&&(data[p+33]==0||data[p+33]==1)){p+=len;continue;}std::string name;
   if(joliet){require(n%2==0,"Bad Joliet name");for(int i=0;i<n;i+=2){require(data[p+33+i]==0,"Non-ASCII Joliet name unsupported");name+=char(data[p+34+i]);}}
   else name.assign((char*)&data[p+33],n);
   if(auto pos=name.find(';');pos!=std::string::npos){name.resize(pos);}
   auto path=prefix+"/"+name;uint32_t s=u32(data,p+2),sz=u32(data,p+10);require(!(data[p+25]&128),"Multi-extent ISO files unsupported");
   if(data[p+25]&2)directory(s,sz,path,joliet,depth+1);else{Entry e{path,s,sz,uint64_t(sec)*2048+p};records[upper(path)].push_back(e);if(!joliet)require(files.emplace(upper(path),e).second,"Duplicate ISO path");}p+=len;
  }
 }
public:
 std::map<std::string,Entry> files;std::map<std::string,std::vector<Entry>> records;std::vector<uint64_t> descriptors;
 explicit Iso(const fs::path& p):stream(p,std::ios::binary),length(fs::file_size(p)){
  require(bool(stream)&&length>65536&&length%2048==0,"Select an uncompressed ISO, not CSO/ZIP");bool primary=false;
  for(int s=16;s<80;s++){auto b=read(uint64_t(s)*2048,2048);require(std::string((char*)&b[1],5)=="CD001","Not an ISO9660 image");if(b[0]==255)break;if(b[0]==1||b[0]==2){require(u16(b,128)==2048,"Unsupported sector size");descriptors.push_back(uint64_t(s)*2048);if(b[0]==1){require(!primary,"Multiple primary descriptors");primary=true;directory(u32(b,158),u32(b,166),"",false,0);}else if(b[88]==37&&b[89]==47)directory(u32(b,158),u32(b,166),"",true,0);}}
  require(primary,"No primary ISO directory");
 }
 Bytes read(uint64_t p,uint32_t size){require(p<=length&&size<=length-p,"ISO extent out of range");Bytes b(size);stream.clear();stream.seekg(p);stream.read((char*)b.data(),size);require(bool(stream)||!size,"ISO read failed");return b;}
 void close(){stream.close();}
 Bytes file(std::string p){auto i=files.find(upper(p));require(i!=files.end(),"ISO missing "+p);return read(uint64_t(i->second.sector)*2048,i->second.size);}
};
inline uint32_t j(uint32_t va){return 0x08000000|((va>>2)&0x03ffffff);}
inline Bytes words(std::initializer_list<uint32_t> w){Bytes b(w.size()*4);size_t p=0;for(auto v:w){put32(b,p,v);p+=4;}return b;}
inline void replace(Bytes& b,size_t off,const Bytes& old,const Bytes& next){require(slice(b,off,old.size())==old,"Unexpected instruction; unsupported build");require(old.size()==next.size(),"Instruction width differs");std::copy(next.begin(),next.end(),b.begin()+off);}
inline Bytes encode(const Bytes& b){Bytes payload;for(size_t off=0;off<b.size();off+=60000){auto n=std::min<size_t>(60000,b.size()-off);payload.insert(payload.end(),{255,128,254,uint8_t(n),uint8_t(n>>8)});payload.insert(payload.end(),b.begin()+off,b.begin()+off+n);}Bytes out={'B','P','E',' '};auto h=words({256,uint32_t(payload.size()),uint32_t(b.size())});out.insert(out.end(),h.begin(),h.end());out.insert(out.end(),payload.begin(),payload.end());return out;}
} // dlc
#include "catalog.hpp"
namespace dlc {
struct Options{std::set<int> ids;bool install18=false;};
struct Prepared{std::map<std::string,Bytes> replacements;bool base18;};
#include "activation.hpp"
inline Prepared prepare(Iso& iso,const fs::path& data,const Options& opt){
 require(iso.files.count("/PSP_GAME/USRDIR/PAC/EDIT/EDIT.PAC"),"Incomplete ISO: required edit/edit.pac is missing");
 if(opt.install18)return activate(iso,data,opt);
 require(!opt.ids.empty(),"Select at least one ID");for(int id:opt.ids)require((id>=50&&id<=69)||(id>=100&&id<=321),"Allowed IDs: 50-69 and 100-321. ID 322 is reserved.");
 for(const auto& a:assets)require(sha(load(data/a.name))==a.patchedHash,"Application data changed: "+std::string(a.name));
 auto input=iso.file(assets[0].path);auto hash=sha(input);bool stock=hash==assets[0].stockHash, base18=hash==assets[0].patchedHash;
 require(stock||base18,"Unsupported executable. Use the original US ULUS10543 v1.00 ISO or the tested DLC18 ISO. Repatching custom tool outputs is not supported: return to your original ISO.");
 Prepared result;result.base18=base18||opt.install18;
 if(opt.install18&&!base18){for(const auto& a:assets)require(sha(iso.file(a.path))==a.stockHash,"Installing the test slots requires unchanged source files; this ISO has custom changes to "+std::string(a.name));for(const auto& a:assets)result.replacements[upper(a.path)]=load(data/a.name);}
 Bytes elf=result.base18?load(data/"EBOOT.BIN"):load(data/"stock.elf");require(result.base18||sha(elf)=="d59fbf71beff5c18f8c8c378826d99af12ba6a0520e10b598a10db9aa0abdf22","Stock ELF data changed");
 const uint32_t base=0x08fc9600;const size_t ph=52+15*32;uint32_t segment;
 if(!result.base18){require(u32(elf,ph)==0&&u32(elf,ph+8)==base,"Unexpected program header");segment=(elf.size()+15)&~15u;elf.resize(segment+0x6000,0xa5);put32(elf,ph,1);put32(elf,ph+4,segment);put32(elf,ph+8,base);put32(elf,ph+12,0);put32(elf,ph+16,0x6000);put32(elf,ph+20,0x6000);put32(elf,ph+24,7);put32(elf,ph+28,16);
  replace(elf,0x08cc4a4c-0x088d8000+0x234,words({0x24639600}),words({0x2463f600}));replace(elf,0x089ff37c-0x088d8000+0x234,words({0x24429600}),words({0x2442f600}));
 }else{segment=u32(elf,ph+4);require(u32(elf,ph)==1&&u32(elf,ph+16)==0x6000,"Unexpected DLC18 segment");}
 for(size_t i=0x5500;i<0x6000;i++)require(elf.at(segment+i)==0xa5,"Filter storage is already occupied");
 std::fill(elf.begin()+segment+0x5500,elf.begin()+segment+0x5500+322,0);for(int id:opt.ids)elf[segment+0x5500+id]=1;
 uint32_t table=base+0x5500, code=base+0x5800;
 auto stub=words({0x2c820142,0x10400007,0,0x3c020000|(table>>16),0x34420000|(table&65535),0x00441021,0x90420000,0x03e00008,0,0x03e00008,0x00001021});
 std::copy(stub.begin(),stub.end(),elf.begin()+segment+0x5800);result.replacements[upper(assets[0].path)]=elf;
 Bytes menu;
 if(result.base18){menu=iso.file(assets[5].path);if(opt.install18&&!base18)menu=load(data/"menuPSP.bin");require(sha(menu)==assets[5].patchedHash,"MENU differs from the tested DLC18 build");replace(menu,0x40d30,words({j(0x08994c68),0x00a02021}),words({j(code),0x00a02021}));}
 else{menu=iso.file(assets[5].path);require(menu==load(data/"stock-menu.bin"),"Unsupported MENU build");replace(menu,0x40c34,words({0x00001021}),words({0x24020001}));replace(menu,0x40538,words({0x14200052}),words({0x1420ffdf}));replace(menu,0x40d30,words({0x03e00008,0x00001021}),words({j(code),0x00a02021}));}
 result.replacements[upper(assets[5].path)]=menu;
 Bytes bin=(opt.install18&&!base18)?load(data/"bin.pac"):iso.file(assets[4].path);
 Bytes arc=(opt.install18&&!base18)?load(data/"plistpsp.arc"):iso.file(assets[3].path);
 require(slice(bin,0,4)==Bytes({'D','P','A','C'}),"Invalid bin.pac");auto dir=slice(bin,0x800,u32(bin,4));require(slice(dir,0,4)==Bytes({'O','V','R','M'}),"Invalid overlay directory");size_t pos=0;int count=0;for(size_t i=8;i+8<=dir.size();i+=8)if(slice(dir,i,4)==Bytes({'M','E','N','U'})){pos=i;count++;}require(count==1,"Nonunique MENU overlay");
 auto packed=encode(menu);Bytes pac={'P','A','C',' '};auto header=words({1,0, uint32_t(packed.size())<<8});pac.insert(pac.end(),header.begin(),header.end());pac.insert(pac.end(),packed.begin(),packed.end());pac.resize((pac.size()+255)&~size_t(255),0);require(pac.size()/256<=65535,"MENU archive too large");bin.resize((bin.size()+2047)&~size_t(2047),0);uint64_t sec=(bin.size()-0x4000)/2048;require(sec<=65535,"Overlay archive too large");auto next=dir;put16(next,pos+4,sec);put16(next,pos+6,pac.size()/256);bin.insert(bin.end(),pac.begin(),pac.end());bin.resize((bin.size()+2047)&~size_t(2047),0);std::copy(next.begin(),next.end(),bin.begin()+0x800);put32(bin,8,bin.size()-0x4000);
 auto found=std::search(arc.begin(),arc.end(),dir.begin(),dir.end());require(found!=arc.end(),"ARC overlay directory does not match bin.pac");require(std::search(found+1,arc.end(),dir.begin(),dir.end())==arc.end(),"Ambiguous ARC directory");std::copy(next.begin(),next.end(),found);
 result.replacements[upper(assets[4].path)]=bin;result.replacements[upper(assets[3].path)]=arc;
 return result;
}
inline void patch(const fs::path& source,const fs::path& destination,const fs::path& data,const Options& opt,std::function<void(int)> progress={}){
 require(fs::exists(source),"Input ISO not found");require(!fs::exists(destination),"Output already exists; choose a new filename");require(fs::weakly_canonical(source)!=fs::weakly_canonical(destination),"Original ISO must remain separate");auto partial=destination;partial+=L".partial";auto report=destination;report+=L".patch-report.txt";require(!fs::exists(partial)&&!fs::exists(report),"Output or report already exists");
 Iso iso(source);auto prepared=prepare(iso,data,opt);uint64_t needed=fs::file_size(source);for(auto& row:prepared.replacements)needed+=((row.second.size()+2047)/2048)*2048;auto parent=destination.parent_path();if(parent.empty())parent=".";require(fs::space(parent).available>needed+16*1024*1024,"Not enough free disk space for the new ISO");
 try{
  std::ifstream in(source,std::ios::binary);std::fstream out(partial,std::ios::in|std::ios::out|std::ios::binary|std::ios::trunc);require(bool(out),"Cannot create output ISO");std::array<char,1024*1024> buffer;uint64_t done=0,total=fs::file_size(source);
  while(in){in.read(buffer.data(),buffer.size());auto n=in.gcount();if(n){out.write(buffer.data(),n);done+=n;if(progress)progress(int(70*done/total));}}require(in.eof()&&bool(out),"Copy failed");uint64_t end=done;
  for(auto& row:prepared.replacements){auto& bytes=row.second;require(end%2048==0,"Unaligned ISO");require(end/2048<=UINT32_MAX,"ISO too large");uint32_t sector=end/2048;out.seekp(end);out.write((char*)bytes.data(),bytes.size());uint64_t next=(end+bytes.size()+2047)&~uint64_t(2047);std::array<char,2048> zeros{};out.write(zeros.data(),next-end-bytes.size());for(const auto& record:iso.records.at(row.first)){Bytes patchbytes(16);both32(patchbytes,0,sector);both32(patchbytes,8,bytes.size());out.seekp(record.record+2);out.write((char*)patchbytes.data(),patchbytes.size());}end=next;}
  for(auto p:iso.descriptors){auto descriptor=iso.read(p,2048);both32(descriptor,80,end/2048);out.seekp(p);out.write((char*)descriptor.data(),descriptor.size());}out.flush();require(bool(out),"Write/flush failed");out.close();if(progress)progress(80);
  Iso check(partial);require(check.files.size()==iso.files.size(),"ISO file count changed");size_t index=0;
  for(auto& row:iso.files){auto before=row.second;auto after=check.files.at(row.first);auto replacement=prepared.replacements.find(row.first);if(replacement!=prepared.replacements.end())require(check.file(row.first)==replacement->second,"Replacement extraction verification failed");else{require(before.sector==after.sector&&before.size==after.size,"Unchanged file was relocated");uint64_t offset=uint64_t(before.sector)*2048;for(uint32_t p=0;p<before.size;){auto n=std::min<uint32_t>(1024*1024,before.size-p);require(iso.read(offset+p,n)==check.read(offset+p,n),"Unchanged file content differs");p+=n;}}
   if(progress)progress(80+int(19*++index/iso.files.size()));}
  check.close();
  std::ostringstream text;text<<"SVR DLC Tab Manager experimental build\nOriginal: "<<source.string()<<"\nNew ISO: "<<destination.string()<<"\nVerified files: "<<iso.files.size()<<"\nDLC filter IDs:";for(int id:opt.ids)text<<" "<<id;text<<"\nIndependent clone activation: "<<(opt.install18?"yes":"no")<<"\n";
  if(opt.install18){
   text<<"Owned bank VA: 0x08FC9600; size: 0x30000; end: 0x08FF9600\nDLC bitmap: VA 0x08FE9600; 322 bytes\n";
   text<<"Mappings:";int extra=0;for(int id:opt.ids)text<<" ID "<<id<<" -> ordinal "<<(id>=100?id-100:252+extra++);text<<"\n";
   auto elf=prepared.replacements.at("/PSP_GAME/SYSDIR/EBOOT.BIN");
   for(uint32_t va:{0x08995158u,0x0899519cu,0x08995184u,0x08a01780u,0x08a01690u,0x08a01704u,0x08a017f0u,0x08994c68u,0x089915f8u,0x08995204u,0x08cc4a44u,0x089ff36cu,0x089fe68cu,0x089fe690u})text<<"ELF instruction VA 0x"<<std::hex<<va<<" file offset 0x"<<(va-0x088d8000+0x234)<<" word 0x"<<u32(elf,va-0x088d8000+0x234)<<std::dec<<"\n";
   text<<"Selected records preserve each ID independently. No unselected record, original donor or save is edited.\n";
  }else{text<<"Filter-only mode: only identities already present in the native roster list can appear.\n";}
  text<<"MENU filter hook: 0x08F58BB0 / offset 0x40D30\n";
  for(auto& row:prepared.replacements){text<<row.first<<" SHA256 "<<sha(row.second)<<"\n";}
  text<<"Rollback: open your original ISO; restore your savedata backup to undo test progress. No save or original ISO is modified.\n";
  auto s=text.str();save(report,Bytes(s.begin(),s.end()));fs::rename(partial,destination);if(progress)progress(100);
 }catch(...){std::error_code ec;fs::remove(partial,ec);fs::remove(report,ec);throw;}
}
} // dlc
