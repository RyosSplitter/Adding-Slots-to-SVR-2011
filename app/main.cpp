#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0602
#endif
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <thread>
#endif
#include "patcher.hpp"
#ifdef _WIN32
#include "diagnostic_windows.hpp"
#pragma comment(lib,"comctl32.lib")
HWND window,list,inputPath,outputPath,installBox,statusText,progressBar,patchButton,diagnoseButton,debugPort;
std::set<int> selected={316};bool busy=false;std::vector<int> visible;
std::wstring widen(const std::string& s){return std::wstring(s.begin(),s.end());}
std::wstring getText(HWND w){int n=GetWindowTextLengthW(w);std::wstring s(n+1,L'\0');GetWindowTextW(w,s.data(),n+1);s.resize(n);return s;}
void label(const wchar_t* text,int x,int y,int w,int h){auto a=CreateWindowW(L"STATIC",text,WS_CHILD|WS_VISIBLE,x,y,w,h,window,0,0,0);SendMessage(a,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),TRUE);}
HWND control(const wchar_t* cls,const wchar_t* text,DWORD style,int x,int y,int w,int h,int id){auto a=CreateWindowW(cls,text,WS_CHILD|WS_VISIBLE|style,x,y,w,h,window,(HMENU)(INT_PTR)id,0,0);SendMessage(a,WM_SETFONT,(WPARAM)GetStockObject(DEFAULT_GUI_FONT),TRUE);return a;}
void refresh(){ListView_DeleteAllItems(list);visible.clear();for(int id=50;id<=321;id++){if(!dlc::vacant(id))continue;int row=visible.size();visible.push_back(id);LVITEMW item{};item.mask=LVIF_TEXT;item.iItem=row;auto number=std::to_wstring(id);item.pszText=number.data();ListView_InsertItem(list,&item);auto name=widen(catalog[id]);auto suffix=name.find(L" (requires wrestler setup)");if(suffix!=std::wstring::npos)name.resize(suffix);name+=L" — will become CLONE "+std::to_wstring(id);ListView_SetItemText(list,row,1,name.data());ListView_SetCheckState(list,row,selected.count(id)>0);}}
void collect(){selected.clear();for(size_t row=0;row<visible.size();row++)if(ListView_GetCheckState(list,row))selected.insert(visible[row]);}
std::wstring choose(bool save){wchar_t file[32768]={};OPENFILENAMEW o{};o.lStructSize=sizeof(o);o.hwndOwner=window;o.lpstrFilter=L"PSP ISO image (*.iso)\0*.iso\0\0";o.lpstrFile=file;o.nMaxFile=32768;o.lpstrDefExt=L"iso";o.Flags=OFN_EXPLORER|OFN_PATHMUSTEXIST|(save?OFN_OVERWRITEPROMPT:OFN_FILEMUSTEXIST);if(save?GetSaveFileNameW(&o):GetOpenFileNameW(&o))return file;return L"";}
LRESULT CALLBACK proc(HWND w,UINT m,WPARAM a,LPARAM b){
 switch(m){case WM_CREATE:{window=w;label(L"SVR 2011 PSP — DLC Tab Manager",18,14,760,28);label(L"US ULUS10543 v1.00 ISO (model/texture edits supported):",18,47,760,20);inputPath=control(L"EDIT",L"",WS_BORDER|ES_AUTOHSCROLL,18,71,652,25,10);control(L"BUTTON",L"Browse…",BS_PUSHBUTTON,682,71,100,25,11);
 label(L"Save a NEW patched ISO — your original will stay unchanged:",18,105,760,20);outputPath=control(L"EDIT",L"",WS_BORDER|ES_AUTOHSCROLL,18,130,652,25,12);control(L"BUTTON",L"Save as…",BS_PUSHBUTTON,682,130,100,25,13);
 label(L"Check unused IDs to activate as independent Cena clones in the DLC tab:",18,170,760,20);
 list=control(WC_LISTVIEWW,L"",WS_BORDER|LVS_REPORT|LVS_SHOWSELALWAYS,18,196,764,262,20);ListView_SetExtendedListViewStyle(list,LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT|LVS_EX_DOUBLEBUFFER);LVCOLUMNW c{};c.mask=LVCF_TEXT|LVCF_WIDTH;c.cx=80;c.pszText=(LPWSTR)L"ID";ListView_InsertColumn(list,0,&c);c.cx=655;c.pszText=(LPWSTR)L"Original identity / setup status";ListView_InsertColumn(list,1,&c);refresh();
 control(L"BUTTON",L"Select 150 slots",BS_PUSHBUTTON,18,467,145,27,21);control(L"BUTTON",L"Select all 151",BS_PUSHBUTTON,172,467,145,27,22);control(L"BUTTON",L"Clear selection",BS_PUSHBUTTON,326,467,145,27,23);
 installBox=control(L"STATIC",L"Selected unused IDs receive working Cena models, records, mappings and DLC registration",0,18,504,764,27,24);
 label(L"151 candidates: 132 NoModel IDs + 19 dormant DLC IDs. Stock and special identities are protected.",18,538,764,20);
 label(L"The selected IDs become separate CLONE <ID> identities, using Cena assets.\nKeep your original ISO and back up savedata. ID 322 remains reserved.",18,564,764,38);
 patchButton=control(L"BUTTON",L"Activate selected slots & save new ISO",BS_PUSHBUTTON,18,617,335,33,30);progressBar=control(PROGRESS_CLASSW,L"",0,367,619,415,27,31);diagnoseButton=control(L"BUTTON",L"Capture PPSSPP diagnostic…",BS_PUSHBUTTON,18,660,270,30,40);label(L"Debugger port:",306,667,100,20);debugPort=control(L"EDIT",L"",WS_BORDER|ES_NUMBER,410,663,85,25,41);label(L"Local connection only; no game-memory changes",507,667,275,20);statusText=control(L"STATIC",L"Ready. No cheat codes or PowerShell required.",0,18,703,764,42,32);break;}
 case WM_COMMAND:{if(busy)break;int id=LOWORD(a);if(id==11||id==13){auto path=choose(id==13);if(!path.empty())SetWindowTextW(id==11?inputPath:outputPath,path.c_str());}
 if(id==21){selected.clear();for(int i=51;i<=321;i++)if(dlc::vacant(i)&&i!=69)selected.insert(i);refresh();}if(id==22){selected.clear();for(int i=51;i<=321;i++)if(dlc::vacant(i))selected.insert(i);refresh();}if(id==23){selected.clear();refresh();}
 if(id==40){auto portText=getText(debugPort);int port=0;try{port=std::stoi(portText);}catch(...){}if(port<1||port>65535){MessageBoxW(w,L"Enable PPSSPP remote debugging on localhost. Enter the port shown by its web debugger URL or ppsspp.ini RemoteISOPort. Keep PPSSPP at character selection.",L"Debugger port required",MB_OK);break;}auto src=choose(false);if(src.empty())break;auto report=dlc::fs::path(src);report+=L".runtime-diagnostic.json";auto textReport=report; textReport.replace_extension(L"txt"); auto protocolReport=report; protocolReport.replace_extension(L"protocol.jsonl");if(dlc::fs::exists(report)||dlc::fs::exists(textReport)||dlc::fs::exists(protocolReport)){MessageBoxW(w,L"A diagnostic already exists beside this ISO. Rename or move that report before capturing again.",L"Keep previous evidence",MB_OK);break;}MessageBoxW(w,L"Select the same ISO PPSSPP is running. For the next 45 seconds, move from Divas toward DLC, try Cross, and return using your normal controller. This briefly pauses at guarded breakpoints but never writes game memory or forces selection. Reports will be saved beside the ISO. See DIAGNOSTICS.md for setup.",L"Runtime diagnostic",MB_OK);busy=true;EnableWindow(patchButton,FALSE);EnableWindow(diagnoseButton,FALSE);SetWindowTextW(statusText,L"Connecting to local PPSSPP and checking the loaded menu…");std::thread([src,report,port,w]{bool ok=true;std::wstring result;try{auto protocol=report;protocol.replace_extension(L"protocol.jsonl");svrdiag::WindowsSocket socket(port,protocol);svrdiag::capture(src,report,[&](const std::string& e,svrdiag::Json p){return socket.call(e,p);},[w](const std::wstring& s){PostMessageW(w,WM_APP+3,0,(LPARAM)new std::wstring(s));});result=L"Diagnostic reports saved beside the selected ISO.\nSend the .runtime-diagnostic.json and .txt files. Capture completion does not certify a working DLC tab.";}catch(const std::exception& e){ok=false;result=widen(e.what());if(!dlc::fs::exists(report)){std::ofstream f(report);if(f)f<<svrdiag::Json{{"result","Connection/capture incomplete; no runtime success established"},{"error",e.what()}}.dump(2)<<"\n";}}PostMessageW(w,WM_APP+2,ok,(LPARAM)new std::wstring(result));}).detach();}
 if(id==30){collect();if(selected.empty()){MessageBoxW(w,L"Select at least one ID.",L"No selection",MB_OK);break;}auto src=getText(inputPath),dst=getText(outputPath);if(src.empty()||dst.empty()){MessageBoxW(w,L"Choose the original ISO and a new output filename.",L"Choose files",MB_OK);break;}
 wchar_t module[32768];GetModuleFileNameW(0,module,32768);auto data=dlc::fs::path(module).parent_path()/L"data";dlc::Options opt{selected,true};busy=true;EnableWindow(patchButton,FALSE);EnableWindow(diagnoseButton,FALSE);SetWindowTextW(statusText,L"Building and verifying the new ISO. This can take a few minutes…");
 std::thread([src,dst,data,opt,w]{std::wstring result;bool ok=true;try{dlc::patch(src,dst,data,opt,[w](int p){PostMessageW(w,WM_APP+1,p,0);});result=L"Saved and verified. Open the new ISO in PPSSPP; boot normally.\nThe .patch-report.txt beside it lists selected IDs, changes and rollback.";}catch(const std::exception& e){ok=false;result=widen(e.what());}auto message=new std::wstring(result);PostMessageW(w,WM_APP+2,ok,(LPARAM)message);}).detach();}break;}
 case WM_APP+1:SendMessage(progressBar,PBM_SETPOS,a,0);break;
 case WM_APP+3:{auto text=(std::wstring*)b;SetWindowTextW(statusText,text->c_str());delete text;break;}
 case WM_APP+2:{auto text=(std::wstring*)b;busy=false;EnableWindow(patchButton,TRUE);EnableWindow(diagnoseButton,TRUE);SetWindowTextW(statusText,text->c_str());MessageBoxW(w,text->c_str(),a?L"Operation finished":L"Operation failed",MB_OK|(a?MB_ICONINFORMATION:MB_ICONERROR));delete text;break;}
 case WM_CLOSE:if(busy){MessageBoxW(w,L"Please wait for the build to finish before closing.",L"Build in progress",MB_OK);return 0;}DestroyWindow(w);break;
 case WM_DESTROY:PostQuitMessage(0);break;default:return DefWindowProcW(w,m,a,b);
 }return 0;
}
int WINAPI wWinMain(HINSTANCE instance,HINSTANCE,LPWSTR,int show){INITCOMMONCONTROLSEX cc{sizeof(cc),ICC_LISTVIEW_CLASSES|ICC_PROGRESS_CLASS};InitCommonControlsEx(&cc);SetProcessDPIAware();WNDCLASSW cls{};cls.lpfnWndProc=proc;cls.hInstance=instance;cls.lpszClassName=L"SVRDlcManager";cls.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);cls.hCursor=LoadCursor(0,IDC_ARROW);RegisterClassW(&cls);auto w=CreateWindowW(cls.lpszClassName,L"SVR DLC Tab Manager v2.2 — Runtime diagnostics",WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,CW_USEDEFAULT,CW_USEDEFAULT,820,805,0,0,instance,0);ShowWindow(w,show);MSG msg;while(GetMessageW(&msg,0,0,0)>0){if(!IsDialogMessageW(w,&msg)){TranslateMessage(&msg);DispatchMessageW(&msg);}}return 0;}
#else
#include <iostream>
int main(int argc,char** argv){try{dlc::require(argc>=5,"Test runner: INPUT OUTPUT DATA IDS [--install18]");dlc::Options opt;std::istringstream s(argv[4]);std::string id;while(std::getline(s,id,','))opt.ids.insert(std::stoi(id));opt.install18=argc>5&&std::string(argv[5])=="--install18";dlc::patch(argv[1],argv[2],argv[3],opt,[](int p){static int previous=-1;if(p/10!=previous){previous=p/10;std::cout<<p<<"%\n";}});return 0;}catch(const std::exception& e){std::cerr<<e.what()<<"\n";return 1;}}
#endif
