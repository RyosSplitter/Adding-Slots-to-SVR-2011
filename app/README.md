# SVR DLC Tab Manager — Windows application

A native Windows x64 desktop application for SVR 2011 PSP **US ULUS10543 v1.00**.
No PowerShell, Python, account token, or manual archive replacement is needed.

## Use

1. Extract the entire application ZIP into a folder. Keep the `data` folder beside `SVR-DLC-Tab-Manager.exe`.
2. Double-click `SVR-DLC-Tab-Manager.exe`.
3. Browse to your original uncompressed `.iso`, then choose a **new** output filename.
4. Check the IDs you want allowed in **Downloaded Superstars**. The list shows stock names and the 132 NoModel labels.
5. Optionally select **Install eighteen tested Cena test slots**. This installs IDs 51–68 only when the six source files are unchanged stock inputs. The application refuses to overwrite custom changes when installing these slots.
6. Click **Enable DLC tab & create patched ISO**. Wait for content verification to finish.
7. Open the new ISO in PPSSPP. Boot normally instead of loading an older savestate.
8. At wrestler selection, move along the bottom row past Divas to the rightmost WWE icon labelled **Downloaded Superstars**. Cross enters; Up/Down changes wrestler; Cross confirms; Circle exits.

The application is unsigned. Windows may show its usual unknown-publisher prompt. The source is included in the repository.

## What choosing an ID means

The checkboxes configure the native category filter. **They do not create missing wrestler models, independent metadata, entrances, or other assets, nor force locked/excluded entries into the shared roster list.** Only identities already registered in that list can appear. A NoModel slot such as **316 / NoModel128** still needs full wrestler setup. Merely supplying its model is not sufficient if its identity fields or enable state are still dormant. The application warns when NoModel entries are selected.

The optional eighteen-slot mode supplies the already-tested Cena clones and their independent records, so IDs 51–68 can appear without separate wrestler setup. Portraits for these clones remain silhouettes. Checking fewer of these IDs changes visibility in the DLC category; it does not delete their records or resources. The stock roster remains intact.

Supported selections: 50–69 and 100–321. ID 322 is a reserved menu sentinel and is rejected. ID 323 expansion research is unchanged and outside this application's supported range. Dormant ID 69 is not activated by the optional eighteen-slot installation.

Supported executable inputs: the original US v1.00 executable and the tested DLC18 executable. Custom asset edits can remain in place if the executable and MENU match a supported input and their companion overlay directory remains consistent. Unsupported executable/MENU modifications are rejected. To change the selection again, start from your retained original or the tested DLC18 ISO; do not feed this tool its own output in version 1.

## Preservation, verification and rollback

The original ISO and savedata are never written. The patcher copies the input ISO, appends changed files, updates matching primary/Joliet directory records and volume sizes, and independently reads back **every embedded file**. Unchanged game-file payloads must remain byte-for-byte identical. Paths and capitalization are preserved. Output/report collisions, insufficient space, invalid archives, unsupported builds and unsafe IDs are rejected. Failed builds remove their incomplete output.

A `.patch-report.txt` beside the new ISO records selected IDs, file hashes, filter locations and rollback. Keep enough free space for another full ISO (roughly 1.6 GB) and back up savedata before gameplay tests. Rollback means reopening your original ISO; restore the savedata backup if you also want to undo test progress.

## Validation

The shared patching engine was compiled and run on Linux using the same C++ source as the Windows application. Custom Cena/Orton filtering was tested in PPSSPP with normal controller navigation: 139 → 161 → 139 → 161; returning from DLC mode preserved the shared roster list. Both original-stock and tested-DLC18 inputs were independently rebuilt and all 246 file payloads verified. The Windows x64 GUI executable was cross-compiled and checked for standard Windows-only DLL dependencies; the GUI itself has not been run on a Windows machine here.

This is an experimental menu/configuration utility. The prior DLC18 build has completed-match and regular save/reload evidence for all eighteen clones; that evidence does not certify every selectable ID, mode or custom wrestler in outputs from this tool. Physical PSP, CSO, other regions/revisions and loading older savestates are not supported/validated.

## Build from source

Using a MinGW-w64 C++17 toolchain:

```
x86_64-w64-mingw32-g++ -std=c++17 -O2 -static -mwindows -municode main.cpp -o SVR-DLC-Tab-Manager.exe -lcomctl32 -lcomdlg32
```

Place the verified application `data` folder next to the executable. Its supported input/output hashes are in `catalog.hpp`; application data is checked before use. The Linux executable is a research test runner; end users use the Windows GUI.
