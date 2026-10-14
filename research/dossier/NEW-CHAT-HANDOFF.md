# SVR 2011 PSP investigation — handoff for a new Codex chat

Continue the existing investigation; do not restart or duplicate completed research. This handoff was prepared October 14, 2026. Local paths below belong to the old cloud environment and may not exist in a new one. Check what was restored before assuming files/processes are present.

## User's objective and preferences

Reach **150 additional playable wrestlers**, preserving the complete stock roster. Build and maintain a **native Windows desktop application** that selects a user-owned ULUS10543 ISO, activates eligible unused IDs as independent John Cena clones, and makes them selectable through the retained DLC category. It must handle records, mappings, flags and assets automatically, not just produce PowerShell commands. Do not modify the PS2-to-PSP converter or Texture Viewer. Preserve existing ID323 research and reserved sentinel322. Original inputs stay separate from experimental builds; retain rollback and reports.

The user authorized autonomous investigation, implementation and publishing project deliverables to RyosSplitter/Adding-Slots-to-SVR-2011. They are frustrated by repeated approvals, token/settings troubleshooting, broken file links, and Linux evidence being presented as Windows success. Never request a token in chat, claim an upload succeeded without verifying it, or imply the Windows bug is fixed.

## Most recent task and blocker

Windows PPSSPP1.20.4, One on One: the character-selection cursor **skips the dark WWE-logo tile immediately right of Divas**. It never focuses it or displays Downloaded Superstars. Screenshots and the user's ~10.11-second AVI confirm the failure, including horizontal movement at Divas. Several stock positions appear Locked; causality has not been established.

Official **Linux SDL PPSSPP1.20.4** successfully focuses the tile, displays Downloaded Superstars, enters the DLC list and previews CLONE51 through ordinary simulated controller input. Both **right-hand role1 and left-hand role0** have now succeeded. Player side alone does not explain Windows failure. Input was sent through debugger API `input.buttons.send`; debugger paused/read state, but successful navigation tests did **not** write memory, set registers, force coordinates/categories/IDs, or load savestates. Left-hand follow-up is navigation/list/preview/return evidence, not a new full match.

Windows cause remains unknown. No evidence establishes an emulator bug, wrong ISO, stale savestate, patch failure, locking-related failure, or alternate menu implementation. The old cloud environment cannot access the user's Windows process. The immediate next experiment is a diagnostic capture on their failing Windows session, then comparison of actual loaded instructions and cursor branch/state. Do not propose a corrective patch until the rejection point is identified.

## Repository and published deliverables

Repository: https://github.com/RyosSplitter/Adding-Slots-to-SVR-2011
Branch main. Latest published commit at handoff: **06f8a847fd48ab997f559bc16c450bade5b4ef32**.
Actual clean published checkout: `/tmp/svr-dlc18-delivery`.
Original working source checkout: `/workspace/Adding-Slots-to-SVR-2011`; it is an unborn local Git checkout with untracked project work. Do not mistake it for the published remote history.

Current Windows diagnostic app v2.2:
https://raw.githubusercontent.com/RyosSplitter/Adding-Slots-to-SVR-2011/main/downloads/SVR-DLC-Tab-Manager-v2.2-Windows.zip
Verified HTTP200, ZIP integrity and SHA256:
`66cd0a5e50a24f002555897cf46c03f01bd40290dfbec3dfc7e7bb2e2fa2f4d1`
Size2,784,587 bytes. Contains native EXE, data folder, instructions, validation and third-party license.

Earlier v2.1 model-edit-compatible application remains available:
https://raw.githubusercontent.com/RyosSplitter/Adding-Slots-to-SVR-2011/main/downloads/SVR-DLC-Tab-Manager-v2.1-Windows.zip
Earlier modified-files-only ZIP:
https://raw.githubusercontent.com/RyosSplitter/Adding-Slots-to-SVR-2011/main/downloads/SVR2011_DLC18_Modified_Files.zip
These links are genuine repository downloads, unlike unverified cloud workspace file links.

Published diagnostic instructions: `app/DIAGNOSTICS.md`.
Published dossier: `research/dossier/windows-dlc-navigation.md`.
Published validation: `app/validation-v2.json`, `validation-v2.1.json`, `validation-v2.2.json`.

## Windows application implementation

`app/main.cpp`: native Win32 GUI. ISO selection, Save As, checked candidate IDs, Select150 (excludes69), SelectAll151, async build, reports. v2.2 adds Capture PPSSPP diagnostic and a debugger-port field.
`app/patcher.hpp`: portable ISO9660/Joliet parser, SHA256, append replacement files into a new ISO, update descriptors/directories, verify all other embedded files unchanged, prevent original overwrite, remove failed partial outputs. Verification stream closes before Windows rename.
`app/activation.hpp`: clone records, resources, mappings, executable helpers and three menu patches.
`app/catalog.hpp`: names, approved candidates and pinned baseline hashes.
v2.2 does **not** change activation.hpp/patcher.hpp. It adds `diagnostic.hpp`, `diagnostic_windows.hpp`, portable `diagnostic-runner.cpp`, guard tests and vendored MIT nlohmann/json3.11.3.

v2.1 solved rejection of model/texture-modded source ch.pac: supported model archives are structurally validated rather than required to match whole-file stock hashes. Executable/core menu/character files still have strict verification. Existing original resource groups/mappings/payloads are preserved. Custom resources at a selected target ID are rejected. ID192 has original dormant resources; known baseline payloads are handled deliberately, custom replacements rejected. Donor139 must have five expected resource keys; actual donor assets from the selected source ISO are copied. The user's specific modded ISO was not provided for full runtime testing; only its report was available.

The requested **Enable DLC tab checkbox is not implemented**. Keep it separate from diagnosis/correction: it must eventually control a verified functioning patch, not label the current Windows failure as resolved.

## Slots and identity interpretation

The forum list100–321 covers222 identities, not320 entries. Its132 NoModel positions are eligible dormant identities within the existing ID space. IDs50–69 are another range in the game's existing identity system;50 is Chris Masters,51–69 dormant DLC candidates. These nineteen are distinct from the132 NoModel IDs. DLC is a **selection category**, not a separate ID namespace, and can display eligible NoModels too.

151 offered candidates =132 NoModel +19 dormant DLC identities. Select150 uses132+51–68; SelectAll151 includes69. ID69 is another dormant candidate, not proven to be a special bonus capacity or fully gameplay-certified individually. NoModel128 is ID316. Preserve named/NPC/CAW special identities, including IDs not on the normal main grid. Sentinel322 must never be repurposed. Experimental323 resource lookup/metadata/isolated DLC registration research remains intact, but final delivered approach prioritizes existing IDs.

Original NoModel records typically canonicalize to100 and have sort0, so toggling flags alone is insufficient. Clones get unique self/canonical IDs, CLONE<ID> names, sort1000+ID, donor139 metadata/models, own DAT/PRO/name/state, mappings and enable/unlock/classification dispatch. NoModels retain ordinalID−100; chosen51–69 use new ordinals252+ after30 CAW ordinals222–251.

## Exact150 ISO matching the user's report

Local generated ISO:
`/workspace/svr-research/experiments/dlc-app-tests/Exact150.iso`
Size **1,608,448,000 bytes**. Selected150 IDs as above.
User report attachment was `test.iso.patch-report.txt`. Matching core hashes:
- EBOOT: `c954696ea788360526ff52724bbc5321ddaf2f3b6401f06a28332f63ce13d241`
- MENU: `fdf573c6839f6df89cdf465a101d81da6ae8481a9f890ce0f2de0ad1f80183c7`
- bin.pac: `bc223de458147bdd9214e39930f6ae972d983d141118221c349b60ac700c3f5d`

Both independently extracted MENU copies are byte-identical and patched: raw `PSP_GAME/USRDIR/bin/menuPSP.bin` and nested MENU-0000 in bin.pac/OVRM. ARC OVRM directory matches bin.pac. This is static file evidence, not proof of the user's loaded Windows overlay or actual launched path.
Local audits: `tile-independent-audit.json`, `exact150-core-comparison.json` under dlc-app-tests.

## Menu patches and runtime structures

Expected MENU overlay base **08F17E80**. It is application-managed and may not be separately named in `hle.module.list`; do not invent a module identity from the base.
- Availability **08F58AB4**, MENU file offset40C34: `00001021`→`24020001`.
- Cursor candidate **08F583B8**, offset40538: `14200052`→`1420FFDF`, accepts row2/column20 by branching to08F58338. **Column19 remains skipped**.
- Filter **08F58BB0**, offset40D30: jr-ra/zero return replaced by private helper jump; Exact150 word `0A3FCE40` (helper08FF3900), delay word at08F58BB4 `00A02021`.
- Cursor entry **08F58110** fingerprint `27BDFF70 AFBF002C AFBE0028 AFB70024`.
- Accepted path08F58338; post-check08F58504; return08F586AC.
- Category classifier08F586D8: a1=x, a2=y; row2/cols19–20 map to category3.
- Availability entry08F58AB0.

At candidate branch s2=row, s3=column, at controls patched branch, s5 acceptance. Inspect actual branch conditions and destinations, not only bytes.
Owner=a0 at cursor entry or s7 within it. main=*(owner+0x10). Players=*(main+0x2c+4*role). Cursor coordinates owner+0xb44c/b450+28*role. Grid pointer main+0xb058. Player category+0x6984; list-cache index+0xd60; cache=player+0x18a8+0xa14*index, indices0–7. Count+0, IDs+4, flags+0x50c. In category3, player+0x48 is list index; in normal grid it is a row. Normal grid dimensions/table pointers depend on grid table index; do not assume DLC mode has normal-grid coordinates.

Owner allocations change when leaving/re-entering selection. Earlier attempts reused a stale owner and failed. Resolve live pointers each session. Validated left-hand owner09236750/main0922B660 are **examples, not constants**.

PPSSPP JIT can replace raw RAM instructions with EmuHack words (example filter `6820FBD2`). `memory.disasm` encoding exposes the original instruction. Preserve raw bytes but do not label JIT markers patch corruption.

Executable private bank: VA08FC9600, size30000, end08FF9600. Bitmap08FE9600; per-ID dispatch banks from+21000; filter+2A300 when dormant DLC selections present. Helpers cover map/enable/inverse/name/profile/state/ordinal-name/DLC/unlock getters. Heap high-half instructions at08CC4A44 and089FF36C changed to0900 to pair correctly with signed low9600. Expanded ARC exceeded original startup allocation18400; fixed allocation at089FE68C/089FE690 to20000 (128KiB), with size guard. Preserve these fixes; do not revert successful builds to the known overflow build.

## Diagnostic behavior and limitations

Windows v2.2 uses native WinHTTP WebSockets to **127.0.0.1 only**, no Python/PowerShell required. Enter the actual PPSSPP Local server port. To enable server, close PPSSPP, back up its `PSP/SYSTEM/ppsspp.ini`, and edit existing `[General]`:
```
RemoteDebuggerOnStartup = True
RemoteDebuggerLocal = True
RemoteISOPort = 8793
```
Reopen PPSSPP; `http://127.0.0.1:8793/debugger/` should load. App port8793 matches that example. Port alone does not enable remote debugging. Preserve existing sections/settings; do not add duplicate General sections.

Capture hashes the selected ISO, checks running ULUS10543 revision1.00, verifies cursor fingerprint and trace-site instructions, and compares disassembled patch words with selected ISO. It refuses existing conflicting breakpoints, uses guarded temporary execution breakpoints, reads both players/lists, records branch/register state, removes its own breakpoints and restores initial pause/running state while connection remains available. No memory/register writes, controller injection or savestate loading. Bounds45 seconds/120 hits. Hot entry/availability/return sites are removed after one observation; classifier after four.

If entry is not observed, availability caller a0 can supply an owner **inference**, guarded by main/grid pointers and labeled. Missing hits are explicitly unresolved; they do not establish another path or an emulator bug. The last live Linux diagnostic observed availability and player/list state but **full candidate/acceptance breakpoint coverage was not validated**. Earlier live diagnostic did hit cursor entry. Next investigator must retain this limitation.

Five portable guard tests passed: healthy capture, wrong-game refusal, wrong-fingerprint refusal, existing-breakpoint refusal, cleanup after a failure before second breakpoint installation. Windows EXE cross-compiles; **Windows GUI and WinHTTP execution are unrun**. Linux engine tests used the same portable capture code through a WebSocket bridge, not WinHTTP. Therefore do not claim the Windows diagnostic is runtime-validated before receiving a report.

Reports beside selected ISO: `.runtime-diagnostic.json`, `.runtime-diagnostic.txt`, `.runtime-diagnostic.protocol.jsonl`. Existing report files are protected from overwrite. The API identifies game/revision but does not expose actual running ISO path, full settings, savedata or initialization history. Confirm those separately, varying one variable per experiment. Connection/cleanup failures must remain explicit.

## Evidence and testing hierarchy

Separate static binary findings, isolated/native tests, PPSSPP runtime, normal selection, actual gameplay, and persistence.

- All151 static audit: unique self/canonical metadata, preserved unselected DAT/PRO;755 donor resource payloads audited.
- All151 normal controller traversal: all151 IDs, stock68 prefix/main grid preserved, cache219 (Exact150218).
- Gameplay samples: clone316 versus originalCena139; full151 build108 versus140, entrances/ring/match completion/save; fresh-save reload and151 traversal. These do **not** individually gameplay-certify all151 candidates or modes.
- Earlier separate DLC18 build51–68 has broader completed-match/save-reload testing, not automatically transferable to every v2 candidate.
- Current left-hand Exact150 follow-up: tile label, list entry51 preview, Circle return. No new full-match certification.

## Old environment files/toolchain

Research root `/workspace/svr-research`.
Immutable complete baseline `/workspace/svr-research/experiments/E0-intake246.iso` (246 embedded files), `baseline-manifest.json`, originals/, derived/EBOOT.ELF, derived/overlays/MENU-0000.bin, derived/CHAR-DAT and CHAR-PRO, donor139 inputs under experiments/E7-donor139.
Generated final All151.iso and Exact150.iso under experiments/dlc-app-tests. Old18 ISO `/workspace/ppsspp-dlc18/SVR2011_ULUS10543_DLC18_TEST.iso`; modified-files ZIP and manifests beside it. Preserve original/donor/ID323 work. Some obsolete generated ISOs were removed previously after retaining reproducible inputs and evidence; do not assume they exist.

Official Linux1.20.4 release AppImage SHA256:
`661c098e6b7f7610171a57b7c533ce8bba6f2312b71e76d61e850461973eba21`.
Tools `/workspace/svr-research/tools/ppsspp-official-1.20.4`; unchanged official binary runs via bundled ld-linux/library path, local Xvfb display:93, dummy audio, software renderer. No emulator code patch. Test config/data/logs/screenshots in dlc-app-tests/official1204; debugger port8793. Processes may not survive a new environment.
Old API: no client.config.set; CPUstatus uses ticks; cpu.resume/stepping broadcasts may omit tickets. input.buttons.send true/false is ordinary input. Screenshots via X11 import because software GPU screenshot API failed. Research `ppsspp_probe.py` also contains an **old isolated getter test that writes registers**; do not run its main as normal-selection proof. Use only its Debugger transport as appropriate.
Python libs `/workspace/svr-research/tools/python-libs`. MinGW `/workspace/svr-research/tools/mingw/root`. Cross-compile main.cpp with C++17/static/mwindows/municode, local include/library paths, `-lcomctl32 -lcomdlg32 -lwinhttp`. Windows8+ WinHTTP WebSockets. Core patch engine portable Linux tests exist. Vendor JSON SHA256 `9bea4c8066ef4a1c206b2be5a36302f8926f7fdc6087af5d20b417d0cf103ea6`.

## ISO transfer problem / most recent user request

The user wants to move to another environment partly because a direct ISO chat link reports Unable to load file. **No usable chat file-export tool was exposed.** Workspace links have not been demonstrated downloadable. GitHub repository pushes and raw downloads work through platform authentication; release-asset uploads repeatedly returned401. A GitHub token was never successfully usable in this environment. Do not repeat requests for tokens/settings or assert a new environment fixes this automatically. Google Drive upload capability was never established despite plugin discussions; inspect actual tools before claiming it is available.

The full ISO has not been publicly uploaded. Patcher/modified-files app ZIPs were successfully published. Avoid claiming there is a full-ISO GitHub/Drive link. The old environment has only about451MiB free on32GiB, insufficient to duplicate a1.6GB ISO into parts.

Latest transfer experiment: created **only first16MiB** of Exact150 at `/workspace/iso-download-test/Exact150.iso.part001`. Size16,777,216, SHA256 `0ebfcf5db340f06c07f39b41eafc179b02fda167a04a480e4c0870ace6f8ddfc`. A complete16MiB split would require96 parts. Remaining parts were not created. User was shown a workspace link but has **not confirmed it downloads**. No reassembly application has been created. Part001 is not playable; it is only a delivery test. `download-test.json` records details. Do not describe the ISO as fully split or delivered.

## Recommended next actions

1. Inspect new environment restoration, repo checkout, files, free space, actual connectors and supported export/upload capabilities. Do not assume old processes/auth/files survived. Retrieve published v2.2/source/dossier; acquire original ISO/research files only if missing and needed, without repeating completed findings.
2. Help the user obtain Windows diagnostic JSON/TXT from the actual failing session. First assess transport/guard errors and cleanup. Establish the actual launched ISO and disassembled loaded code. Inspect candidate branch, accepted/post-check path and final native category/list, explicitly handling missing breakpoint coverage.
3. Locate the precise rejection condition before proposing the smallest reversible correction. Verify all six Windows success criteria through normal input: Divas→DLC focus→Downloaded Superstars→list→clone select/model load→return to original roster. Keep checkbox implementation separate.
4. For ISO delivery, test real export support or a user-authorized upload destination before creating many parts. More disk space alone does not create downloadable links. If actual delivery works, create all verified parts plus native Windows reassembly/whole-file verification, clearly stating build and unresolved Windows behavior.
