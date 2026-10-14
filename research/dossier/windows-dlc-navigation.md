# Windows DLC navigation investigation — 2026-10-14

## Status

Windows PPSSPP 1.20.4 navigation failure remains confirmed by the user's recording and unexplained. No corrective executable/menu patch is justified yet. The Windows machine is not accessible from the research environment. The next required evidence is the guarded runtime capture from that machine.

The new v2.2 diagnostic is an addition to the existing Windows application. The patch engine, original roster, NoModel activation implementation, and working Linux patch set remain unchanged. The proposed DLC on/off checkbox is separate and has not been added as a claimed fix.

## Existing evidence

User report describes activation of 150 IDs (132 NoModel plus 51–68). An independent rebuild has these hashes, matching the supplied report:

- EBOOT: `c954696ea788360526ff52724bbc5321ddaf2f3b6401f06a28332f63ce13d241`
- MENU: `fdf573c6839f6df89cdf465a101d81da6ae8481a9f890ce0f2de0ad1f80183c7`
- bin.pac: `bc223de458147bdd9214e39930f6ae972d983d141118221c349b60ac700c3f5d`

Both independently extracted MENU copies match. ARC OVRM directory agrees with bin.pac. These are file checks, not proof of the loaded Windows overlay.

User video `ULUS10543_2026-10-14_13-48-56.avi` is approximately 10.11 seconds. It shows ordinary selection navigation and the DLC cell being skipped, including horizontal movement at Divas. Several stock positions display Locked. This does not establish whether locking causes the category rejection. It provides no loaded instruction or branch-state evidence.

## Runtime addresses

Expected MENU base `08F17E80`; application-managed overlay, not necessarily an HLE module entry.

| Address | File offset | Purpose / expected current patch |
| --- | --- | --- |
| `08F58110` | `40290` | cursor entry; fingerprint `27BDFF70 AFBF002C AFBE0028 AFB70024` |
| `08F583B8` | `40538` | candidate branch; `14200052` → `1420FFDF` |
| `08F58338` | `404B8` | accepted candidate path sets s5=1 |
| `08F58504` | `40684` | post-check state; inspect s2/s3/s5 |
| `08F586AC` | `4082C` | cursor return; capture result and owner |
| `08F586D8` | `40858` | category classification (a1=x, a2=y) |
| `08F58AB0` | `40C30` | availability function entry |
| `08F58AB4` | `40C34` | availability result; `00001021` → `24020001` |
| `08F58BB0` | `40D30` | character filter; private jump, exact150 `0A3FCE40` |
| `08F58BB4` | `40D34` | filter delay slot `00A02021` |

At candidate branch: s2=row, s3=column; at!=0 is the patched branch condition; accepted target `08F58338`. Row2/column20 is the exposed DLC cell; column19 remains skipped. Classifier recognizes DLC as category3. JIT raw RAM markers must not be interpreted as patch failure; use debugger disassembly `encoding` and preserve raw reads separately.

Cursor owner is obtained from a0 at cursor entry or s7 inside the cursor routine. main=*(owner+0x10). Players=*(main+0x2c+4*role). Coordinates at owner+0xb44c/b450+28*role. Grid pointer main+0xb058. Player category +0x6984; cache index +0xd60; cache=player+0x18a8+0xa14*index (indices0–7). Count +0, IDs +4, flags +0x50c. In category3, +0x48 is the selected list index; in the normal grid it is a row, so do not decode it as a wrestler list index.

## Linux controlled follow-up

Official Linux SDL PPSSPP v1.20.4; release SHA256 `661c098e6b7f7610171a57b7c533ce8bba6f2312b71e76d61e850461973eba21`. Same exact150 ISO; software renderer, savedata and other settings retained from the previous working test.

The prior successful test used role1/right-hand player. A new role0/left-hand test also reached native coordinates [20,2], entered category3, and selected ID51 from a 218-entry list by ordinary simulated controller inputs. Stable screenshots show “Downloaded Superstars” and “CLONE51”. Circle returned to the original roster/category tile. No memory/register writes or forced cursor/category/ID assignments were used in this follow-up. This is navigation/list/preview evidence, not a new full-match test.

An early attempt reused a cursor-owner address after leaving/re-entering selection; it was stale. Early short-input/breakpoint attempts failed to capture entry. The validated diagnostic resolved a fresh owner from live arguments. A tentative different-routine hypothesis was not confirmed and must not be treated as the cause. Player side alone cannot explain the Windows failure.

Local evidence: `/workspace/svr-research/experiments/dlc-app-tests/official1204/left-test-result.json`, `left-test-dlc-stable.png`, `left-test-tile-stable.png`, and diagnostic engine reports/protocols. Full unredacted protocol and user attachments stay local.

## Unresolved / next experiment

Run v2.2 capture on the failing Windows session without changing its current setup. Verify actual disassembled patch words, exact candidate/acceptance/post-check hits, at/s2/s3/s5, category classification and final coordinates/list. Compare against the selected ISO hashes and Linux trace. The API cannot prove the running ISO file path or retrieve full settings/save initialization history; confirm the opened path and retain settings/save copies separately.

If Windows executes the original cursor branch, investigate its loaded overlay/ISO identity before changing code. If it executes the patched branch but rejects earlier/later, use the recorded branch/state to choose a narrowly scoped additional breakpoint. If no guarded routine is hit despite normal movement, trace the active caller/menu implementation rather than assuming a Windows emulator defect. Propose the smallest correction only after that distinction is established.

## Diagnostic validation and limitations

Five portable guard/cleanup scenarios passed: healthy capture, wrong game refusal, wrong fingerprint refusal, existing-breakpoint refusal, and a simulated failure before a second breakpoint is installed. Healthy cleanup removes all seven owned breakpoints. Partial-install cleanup removes the first successfully installed breakpoint. No memory/register writes, controller injection, or savestate loads are emitted by the diagnostic engine.

A live official Linux run verified instruction comparisons, availability breakpoint capture, validated owner/main/grid pointers, both player snapshots, lists/flags, and cleanup. The last live capture observed the availability breakpoint but did not observe cursor candidate/acceptance breakpoints despite controller movement during the test. Full navigation breakpoint coverage is therefore **not validated**. The report records site hit counts and warns against interpreting missing hits as proof of an alternate path. A prior live capture did observe cursor entry and resolved the owner directly. This limitation is distinct from the independently successful ordinary-controller DLC entry test.

The native Windows application cross-compiles successfully and imports only standard Windows DLLs including WinHTTP. Windows GUI execution and WinHTTP connection/capture remain **unrun**, pending the user's Windows machine. No Windows correction is claimed.
