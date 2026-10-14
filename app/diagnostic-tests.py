"""Exercise diagnostic guards/cleanup via the portable stdin/stdout test adapter.
Usage: PYTHONPATH=... python diagnostic-tests.py RUNNER STOCK_OR_PATCHED_ISO OUTPUT_DIR
The runner includes diagnostic.hpp unchanged; the adapter is in DIAGNOSTICS.md.
"""
import io,json,struct,subprocess,sys
from pathlib import Path
import pycdlib
runner,source,out=sys.argv[1:];out=Path(out);out.mkdir(parents=True,exist_ok=True)
iso=pycdlib.PyCdlib();iso.open(source);fp=io.BytesIO();iso.get_file_from_iso_fp(fp,iso_path='/PSP_GAME/USRDIR/bin/menuPSP.bin;1');menu=fp.getvalue();iso.close()
summary={}
for mode in ['healthy','wrong-game','wrong-fingerprint','existing-breakpoint','add-failure']:
 report=out/(mode+'.json');p=subprocess.Popen([runner,source,str(report),'0'],stdin=subprocess.PIPE,stdout=subprocess.PIPE,text=True,stderr=subprocess.PIPE);calls=[];adds=0
 for line in p.stdout:
  q=json.loads(line);e=q['event'];calls.append(q);r={'event':e}
  if e=='version':r['version']='mock 1.20.4'
  elif e=='game.status':r['game']={'id':'OTHER' if mode=='wrong-game' else 'ULUS10543','version':'1.00'}
  elif e=='cpu.status':r.update(stepping=True,paused=False,pc=0x08f58110,ticks=0)
  elif e=='hle.module.list':r['modules']=[]
  elif e=='cpu.breakpoint.list':r['breakpoints']=[{'address':0x08f583b8}] if mode=='existing-breakpoint' else []
  elif e=='memory.disasm':
   r['lines']=[{'address':q['address']+4*i,'encoding':struct.unpack_from('<I',menu,q['address']+4*i-0x08f17e80)[0]} for i in range(q['count'])]
   if mode=='wrong-fingerprint' and q['address']==0x08f58110:r['lines'][0]['encoding']=0
  elif e=='memory.read':
   import base64
   r['base64']=base64.b64encode(menu[q['address']-0x08f17e80:q['address']-0x08f17e80+q['size']]).decode()
  elif e=='cpu.breakpoint.add':
   adds+=1
   if mode=='add-failure' and adds==2:r={'event':'error','message':'Injected failure before second breakpoint installation'}
  p.stdin.write(json.dumps(r)+'\n');p.stdin.flush()
 assert p.wait()==0,p.stderr.read();result=json.loads(report.read_text());installed=[x['address'] for x in calls if x['event']=='cpu.breakpoint.add'];removed=[x['address'] for x in calls if x['event']=='cpu.breakpoint.remove']
 assert not any(x['event'] in ['memory.write','cpu.setReg','input.buttons.send','game.state.load'] for x in calls)
 if mode=='healthy':assert len(installed)==7 and sorted(installed)==sorted(removed) and 'error' not in result
 elif mode=='add-failure':assert len(installed)==2 and removed==installed[:1] and 'error' in result
 else:assert not installed and 'error' in result
 summary[mode]={'passed':True,'requests':len(calls),'attempted_adds':len(installed),'removed':len(removed),'error':result.get('error')}
(out/'summary.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps(summary,indent=2))
