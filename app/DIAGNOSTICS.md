# Windows runtime capture

This diagnostic observes SVR 2011 PSP ULUS10543 revision 1.00. It does not fix the skipped DLC tile or certify Windows gameplay. The existing patcher and its executable verification are unchanged. There is no new DLC on/off checkbox in this diagnostic release.

## Use the application

1. Enable PPSSPP's WebSocket/remote debugger **locally**. If its UI does not expose the setting, close PPSSPP, back up `PSP/SYSTEM/ppsspp.ini`, and edit the existing `[General]` section to contain:

   ```ini
   RemoteDebuggerOnStartup = True
   RemoteDebuggerLocal = True
   RemoteISOPort = 8793
   ```

   Reopen PPSSPP. Use another unused port if necessary; enter that same port in the application. This is a temporary debugger setting, not a cheat. Keep the server restricted to localhost. Do not add a second `[General]` section. Undo these settings after investigating. The exact config location depends on portable versus installed PPSSPP.
2. Boot the intended patched ISO and reach One on One character selection. Reproduce your existing setup first; do not change save data, renderer, CPU core, cheats, or input settings for the initial capture.
3. Open `SVR-DLC-Tab-Manager.exe`, enter the debugger port, then click **Capture PPSSPP diagnostic…**. Choose the same ISO that is currently running. The tool hashes the selected ISO; PPSSPP's `game.status` API does **not** expose the running file path, so it cannot automatically prove those two paths are identical.
4. During the 45-second capture, use your normal controller to approach Divas, move right toward DLC, try Cross, then return. Brief pauses at breakpoints are expected. The diagnostic itself sends no controller inputs.
5. Send the files beside the ISO: `.runtime-diagnostic.json`, `.runtime-diagnostic.txt`, and optionally `.runtime-diagnostic.protocol.jsonl`. The latter contains the full debugger protocol, including registers, memory reads and module names. Review before sharing. No upload is automatic. Reports are not overwritten; move previous reports before a new capture.

If the application cannot connect, check that PPSSPP is running its local debugger on the entered port. A connection failure is not evidence of a game patch failure. If capture is blocked, the JSON may still contain useful partial evidence. Check `error`, `cleanup_errors`, and `unresolved` before treating a capture as complete.

## Guarded capture

- Checks game ID/revision and a four-instruction MENU routine fingerprint before installing breakpoints.
- Compares disassembled instructions with the selected ISO at `08F583B8`, `08F58AB4`, and `08F58BB0`. Retains raw RAM bytes separately: JIT EmuHack markers can replace RAM instruction words, while `memory.disasm` exposes the original encoding.
- Verifies each trace-site instruction against the selected ISO. Refuses to overwrite an existing breakpoint at any trace address.
- Uses temporary execution breakpoints at cursor entry, candidate check, acceptance, post-check, cursor return, category classification, and availability. Candidate breakpoints require row 2 and columns 19 or higher. Hot entry/availability/return breakpoints are removed after one capture; classification is bounded to four hits. Total capture is bounded to 45 seconds/120 stops.
- Resolves the current cursor owner from live arguments, rather than reusing a previous allocation address. If cursor entry is not observed, an owner inferred from availability caller a0 is labeled as an inference and its main/grid pointers are checked. Absence of a breakpoint hit alone does not prove an alternate path; validate coverage. Reads both player structures, coordinates, categories, list counts/IDs/flags, and selected ID where the category/list interpretation is valid.
- Adds no memory writes, register writes, forced IDs/categories, savestate loads, or gameplay patches. Removes only its own breakpoints and restores the initial CPU pause/running state on a functioning connection. An unrelated pause ends the capture and is left paused. If PPSSPP crashes/disconnects, cleanup cannot be guaranteed; inspect `cleanup_errors`, reconnect and inspect breakpoints before proceeding.
- Reports modules from PPSSPP where supported. MENU is an application-managed overlay and may not appear as a separately named HLE module. Expected base `08F17E80` is an inferred overlay address supported by instruction fingerprints, not a claim that the module list identifies MENU by name.

## Controlled follow-up

Preserve the first failing report and settings/save backups. Verify its loaded instructions and breakpoint branch conditions first. Only then vary one factor per run: left/right player, mode, savedata copy, clean boot history, CPU core, renderer, or input mapping. Do not infer an emulator bug from an operating-system difference alone.

The success checklist remains: Divas → DLC focus → “Downloaded Superstars” → DLC list → normal clone selection/model load → return to original roster. A diagnostic report alone does not demonstrate all six.

## Developer validation

The diagnostic engine is portable C++17; Windows uses native WinHTTP WebSockets restricted to `127.0.0.1`. No Python or PowerShell is needed by Windows users. `vendor/json.hpp` is nlohmann/json v3.11.3 with its MIT license.

Cross-compile `main.cpp` with existing MinGW flags plus `-lwinhttp`. Define `_WIN32_WINNT >= 0x0602` (Windows 8+) to use WinHTTP WebSockets. Windows GUI/transport execution still needs a Windows machine; cross-compilation is not a Windows runtime test.

`diagnostic-tests.py` drives a portable test adapter around `capture()`, injecting wrong-game, fingerprint, breakpoint-conflict and partial-installation failures. The adapter prints each request as JSON to stdout, reads its response from stdin, throws on `event:error`, and calls `capture(ISO, REPORT, request, notifier, seconds)`. These tests verify guards/cleanup, not actual gameplay. Live Linux PPSSPP tests exercise the same capture engine through a WebSocket bridge; they do not test WinHTTP.
