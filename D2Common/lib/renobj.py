"""renobj.py MAPFILE OUTDIR OBJ... : copy each object to OUTDIR with its symbol names renamed per MAPFILE
(old<TAB>new lines); the new names are appended to the string table."""
import sys, struct, os
mp=dict(l.rstrip('\n').split('\t') for l in open(sys.argv[1]) if l.strip())
od=sys.argv[2]; os.makedirs(od,exist_ok=True)
for p in sys.argv[3:]:
    data=bytearray(open(p,'rb').read())
    _,nsec,_,symptr,nsym,_,_=struct.unpack_from('<HHIIIHH',data,0)
    st=symptr+18*nsym; strsize=struct.unpack_from('<I',data,st)[0]
    tab=bytearray(data[st:st+strsize]); ch=0
    i=0
    while i<nsym:
        o=symptr+18*i; nm=data[o:o+8]; naux=data[o+17]
        if nm[:4]==b'\0\0\0\0':
            off=struct.unpack_from('<I',nm,4)[0]; name=bytes(tab[off:tab.index(0,off)]).decode('latin1')
        else: name=nm.rstrip(b'\0').decode('latin1')
        if name in mp:
            new=mp[name].encode('latin1'); off=len(tab); tab+=new+b'\0'; data[o:o+8]=struct.pack('<II',0,off); ch+=1
        i+=1+naux
    struct.pack_into('<I',tab,0,len(tab))
    out=data[:st]+tab+data[st+strsize:]
    open(os.path.join(od,os.path.basename(p)),'wb').write(out)
