# SVR 2011 PSP DLC roster experiment

Experimental PPSSPP build for US PSP ULUS10543. The DLC tab exposes eighteen separate test identities, IDs 51–68, using John Cena assets. These are test clones, not a finished 150-wrestler roster.

See the release instructions before testing. Keep the original ISO and back up savedata. Boot normally rather than loading an older savestate.

## Windows runtime diagnostic build v2.2

[Download v2.2 Windows diagnostics](https://raw.githubusercontent.com/RyosSplitter/Adding-Slots-to-SVR-2011/main/downloads/SVR-DLC-Tab-Manager-v2.2-Windows.zip). Adds **Capture PPSSPP diagnostic…** and a local debugger-port field to the existing native application. It captures loaded menu instructions, guarded breakpoint hits, branch/register state and both player structures. It sends no controller inputs or memory/register writes; the patch engine is unchanged.

**The reported Windows DLC tile skipping remains unresolved.** Linux 1.20.4 navigation has now succeeded for both left-hand and right-hand players. Cross-compilation and diagnostic guard tests do not establish Windows runtime success. [Diagnostic setup/instructions](app/DIAGNOSTICS.md), [research dossier](research/dossier/windows-dlc-navigation.md). This release provides evidence collection, not a corrective patch or an on/off checkbox.

## Windows DLC Tab Manager v2.1

[Download the Windows desktop application](https://github.com/RyosSplitter/Adding-Slots-to-SVR-2011/raw/refs/heads/main/downloads/SVR-DLC-Tab-Manager-v2.1-Windows.zip). Extract it and double-click the `.exe`; keep its `data` folder beside it. Select a US ULUS10543 v1.00 ISO with supported original core files (model/texture edits supported), choose unused IDs, and save a new ISO. Selected IDs become independent Cena clones with working records, resource mappings and normal DLC-tab registration. Individual selection, **Select 150 slots**, and **Select all 151** presets are available.

All 151 candidates passed normal controller traversal, with the stock list/main grid preserved. Gameplay samples and fresh-save reload passed; this is not a completed-match test of every identity or mode. [Instructions, scope and build notes](app/README.md), [validation evidence](app/validation-v2.json). Existing named/NPC/CAW identities and sentinel 322 remain protected. Converter and Texture Viewer are unchanged. The original experimental DLC18 ZIP remains available separately.
