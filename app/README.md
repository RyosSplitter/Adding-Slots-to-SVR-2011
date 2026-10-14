# SVR DLC Tab Manager v2.1 — Windows application

A native Windows x64 desktop application for SVR 2011 PSP **US ULUS10543 v1.00**. It turns selected unused identities into independent John Cena clones and enables their normal selection through **Downloaded Superstars**. It handles character records, ID/ordinal mappings, enable/unlock decisions, model resources, companion ARC directories and the menu route.

No PowerShell, Python, account token or manual file replacement is needed.

## Use

1. Extract the entire application ZIP. Keep the `data` folder beside `SVR-DLC-Tab-Manager.exe`.
2. Double-click the `.exe`.
3. Browse to your **uncompressed US v1.00 ISO with supported original core files**. The previous experimental ISO and previously patched outputs are not supported inputs for activation.
4. Choose a **new** output filename.
5. Check the unused IDs you want activated. **316 is NoModel128**, for example.
6. **Select 150 slots** chooses all 132 NoModel identities plus DLC IDs 51–68, leaving 69 spare. **Select all 151** includes ID 69 too. Individual selection is also supported.
7. Click **Activate selected slots & save new ISO**. Wait for independent file-content verification to finish.
8. Boot the new ISO normally in PPSSPP. Do not load a savestate made with another build.
9. In normal wrestler selection, move along the bottom row past Divas to the rightmost WWE icon labelled **Downloaded Superstars**. Cross enters; Up/Down chooses a clone; Cross confirms; Circle exits.

Each activated identity is named **CLONE <ID>**, with Cena's model, attributes, profile and entrance defaults. Portrait previews remain silhouettes. These are functioning test-clone setups, not a collection of finished custom wrestlers. Replace their assets separately when creating your own wrestlers.

The executable is unsigned; Windows may show its usual unknown-publisher prompt. Source is available in this repository. The GUI executable was cross-compiled; its Windows GUI has not been run on a Windows machine in this environment.

## Which IDs can be activated?

There are **151 confirmed-unused candidates**: the 132 NoModel entries within 100–321 and the nineteen dormant DLC identities 51–69. The table includes all of them. Existing stock identities, Divas, NPCs, hidden named characters and CAW definitions are protected, even when a particular save does not currently show them in a menu. Chris Masters/50 is preserved. ID 322 is a menu sentinel and cannot be selected; separate ID 323 research remains unchanged.

This is intentional: an ID being absent from a menu does not prove it is unused. IDs 70–99 and other reserved/special domains are not certified activation candidates in this version.

Unselected placeholders remain unchanged. Selecting a NoModel slot now performs its setup; it is **not merely an eligibility checkbox**, as in v1. Model archives are checked structurally, and unselected resources are preserved. Model and texture edits are supported, including consistently updated resource directories and companion ARC entries. A selected slot that already contains custom resources is rejected so its wrestler is not silently overwritten. The original donor Cena/139 retains his records and resources, and a clone can coexist with him.

## Preservation and rollback

The original ISO and savedata are never written. The patcher copies the ISO, appends modified files, updates primary/Joliet directory records and volume sizes, preserves file paths/capitalization, then reads back every embedded file. Unchanged game-file payloads must match exactly. Unselected compressed character records and model mappings are preserved; the originals of any dormant selected resource payloads also remain in the source ISO.

Activation rejects unsupported executable/region/revision, changed executable/character metadata/MENU files, unsafe IDs, occupied output filenames, malformed archives, oversized directories and insufficient space. Failed builds remove incomplete outputs. Keep roughly 2 GB free for a full build and back up savedata before gameplay testing.

A `.patch-report.txt` beside the new ISO records active IDs, mappings, executable addresses/file offsets, output hashes and rollback. To choose a different set, return to your retained source ISO and save another new output. To undo the patch, open your original ISO. Restore the savedata backup if you also want to undo test progress.

The converter and Texture Viewer are not changed or required.

## Implementation notes

NoModel identities keep their existing regular ordinals `ID - 100`. Dormant DLC identities use new ordinals beginning at 252, after the original 222 regular and 30 CAW/special entries. Only the two verified native MENU constructors receive extra iterations for the chosen DLC identities. The native selection buffers retain their 322-entry capacity; the full test contains 219 rows: 68 baseline selectable entries plus 151 clones.

A private ELF LOAD bank at `0x08FC9600` reserves `0x30000` bytes for independent name/profile/state records and guarded dispatchers. Both heap-address high halves are adjusted so allocation starts after `0x08FF9600`. Selected DAT and runtime name records receive their own self ID, duplicate-identity field and unique sort key. Their model resources are copied into EMD/ch.pac, EMD2/ch2.pac or EMD3/ch3.pac according to the verified packed-key boundaries 13000 and 16000. Both MENU copies and companion ARC directories stay consistent.

The full directory exceeds the original startup ARC allocation of `0x18400` bytes. The allocation is increased to `0x20000` through instructions at `0x089FE68C` and `0x089FE690`; the builder rejects a directory that would exceed that reserved buffer. The first broad experimental build exposed this overflow and is retained as failed evidence, not a successful capacity result.

## Validation boundaries

The shared C++ engine builds and independently verifies all 246 game-file payloads. Static audits confirm 151 independent runtime identities, self/canonical fields, unique sort keys, preserved unselected records and 755 Cena resource payloads. Normal PPSSPP controller navigation visits all 151 IDs, wraps 320 → 51 and back, and preserves the exact stock 68-ID list and main-grid cell table.

NoModel128/316 was normally selected, launched against the unchanged original Cena/139, rendered its own packed resource 31604, completed a match and displayed Save Successful. Broader gameplay and fresh-save-load evidence is recorded in `validation-v2.json`. Preview traversal alone does **not** certify gameplay, every attire or persistence for every identity. Physical PSP, CSO, other regions, wireless/editor modes and older savestates remain unvalidated.

## Build from source

With a MinGW-w64 C++17 toolchain:

```
x86_64-w64-mingw32-g++ -std=c++17 -O2 -static -mwindows -municode main.cpp -o SVR-DLC-Tab-Manager.exe -lcomctl32 -lcomdlg32
```

Keep the verified `data` folder from the application ZIP next to the executable. The Linux build is a research test runner; end users use the Windows GUI.

## v2.1 compatibility change

The broad whole-file hash rejection for ch.pac/ch2.pac/ch3.pac has been replaced with archive structure, key, extent, stock-resource-presence and companion mapping checks. Unselected model payloads and resource mappings are preserved. Executable, character-record and MENU compatibility checks remain; this does not bypass unsupported core changes. Modified Cena assets are used as the model donor from the input ISO. A modified model-archive fixture passed preservation tests; your particular modded ISO has not been tested here.

## v2.2 runtime diagnostic addition

See [DIAGNOSTICS.md](DIAGNOSTICS.md). This adds local PPSSPP evidence capture without changing the activation engine. The reported Windows navigation failure is unresolved; Linux tests do not certify Windows behavior. The DLC on/off checkbox remains separate from this diagnostic work.
