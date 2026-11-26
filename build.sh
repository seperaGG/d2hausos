#!/bin/sh
# Build the Diablo II 1.00 modules byte-identical to the originals into out/, and check each sha256.
set -e
case "$D2SERVER_DLL" in ''|/*) ;; *) D2SERVER_DLL=$(pwd)/$D2SERVER_DLL ;; esac
case "$D2VIDTST_EXE" in ''|/*) ;; *) D2VIDTST_EXE=$(pwd)/$D2VIDTST_EXE ;; esac
cd "$(dirname "$0")"
T="sh $(pwd)/tools/msvc.sh"
mkdir -p out/D2Net out/D2Lang out/D2MCPClient out/D2Gdi out/D2DDraw out/D2sound out/D2Launch out/D2Multi out/D2Glide out/D2Direct3D out/D2Game out/D2Common out/D2MultiLAN out/Bnclient out/Fog out/D2gfx out/D2Win out/D2Server out/D2VidTst

# D2Net.dll
cd D2Net
O=../out/D2Net
$T cl /nologo /c /O2 /Ob2 /Gr /Oy /Gy /Gf /MT /W3 /I../include /Fo$O/ Client.cpp D2Net.cpp Server.cpp
for m in Fog Storm; do $T lib /nologo /def:lib/$m.def /machine:ix86 /out:$O/$m.lib >/dev/null; done
rm -f $O/D2Net.ilk ../out/D2Net.dll
LINK_TIME="2000-05-26 18:58:27" $T link /nologo /DLL /INCREMENTAL:YES /BASE:0x10000000 /MACHINE:IX86 \
	/DEF:D2Net.def /OUT:$O/D2Net.dll $O/Client.obj $O/D2Net.obj $O/Server.obj $O/Storm.lib wsock32.lib \
	kernel32.lib $O/Fog.lib
cp $O/D2Net.dll ../out/D2Net.dll
cd ..

# D2Lang.dll
cd D2Lang
O=../out/D2Lang
C="$T cl /nologo /c /Gr /Oy /Gy /Gf /MT /W3 /I../include /Fo$O/"
$C /O2 /Ob2 D2Lang.cpp Memory.cpp STRSUP.CPP TEXTSUP.CPP strtable.cpp
$C /O2 UNICASE.CPP UNISYS.CPP UNIUTF.CPP
$C /O1 /Ob2 Archive.cpp
for m in Fog Storm; do $T lib /nologo /def:lib/$m.def /machine:ix86 /out:$O/$m.lib >/dev/null; done
rm -f $O/D2Hell.lib
$T lib /nologo /out:$O/D2Hell.lib $O/Archive.obj >/dev/null
rm -f $O/D2Lang.ilk ../out/D2Lang.dll
LINK_TIME="2000-05-26 18:53:13" $T link /nologo /DLL /INCREMENTAL:YES /BASE:0x10000000 /MACHINE:IX86 \
	/DEF:D2Lang.def /OUT:$O/D2Lang.dll $O/D2Lang.obj $O/Memory.obj $O/STRSUP.obj $O/TEXTSUP.obj $O/UNICASE.obj \
	$O/UNISYS.obj $O/UNIUTF.obj $O/strtable.obj $O/Storm.lib $O/D2Hell.lib kernel32.lib advapi32.lib $O/Fog.lib
cp $O/D2Lang.dll ../out/D2Lang.dll
cd ..

# D2MCPClient.dll
cd D2MCPClient
O=../out/D2MCPClient
$T cl /nologo /c /O2 /Ob2 /Gr /Oy /Gy /Gf /MT /W3 /FI D2MCPClientLinkNames.h /I. /I../include /Fo$O/ \
	D2MCPClientMain.cpp D2MCPClientRecv.cpp D2MCPClientConn.cpp D2MCPClientAccess.cpp ToMCP.cpp
for m in Fog Storm; do $T lib /nologo /def:lib/$m.def /machine:ix86 /out:$O/$m.lib >/dev/null; done
rm -f $O/D2MCPClient.ilk ../out/D2MCPClient.dll
LINK_TIME="2000-05-26 19:02:26" $T link /nologo /DLL /INCREMENTAL:YES /BASE:0x10000000 /MACHINE:IX86 \
	/DEF:D2MCPClient.def /OUT:$O/D2MCPClient.dll $O/D2MCPClientMain.obj $O/D2MCPClientRecv.obj \
	$O/D2MCPClientConn.obj $O/D2MCPClientAccess.obj $O/ToMCP.obj $O/Storm.lib $O/Fog.lib kernel32.lib
cp $O/D2MCPClient.dll ../out/D2MCPClient.dll
cd ..

# D2Gdi.dll
cd D2Gdi
O=../out/D2Gdi
$T cl /nologo /c /O2 /Ob2 /Gr /Oy /Gy /Gf /MT /W3 /FI D2GdiLinkNames.h /I. /I../include /Fo$O/ \
	D2Gdi.cpp DllMain.cpp Memory.cpp gdiCore.cpp gdiEngine.cpp gdiSmack.cpp gdiSpCel.cpp gdiSprite.cpp
for m in Storm D2CMP Fog; do $T lib /nologo /def:lib/$m.def /machine:ix86 /out:$O/$m.lib >/dev/null; done
python3 lib/mklonglib.py smackw32.dll $O/smackw32.lib $(awk '{print $1":"$2}' lib/smackw32.exports)
rm -f $O/D2Gdi.ilk ../out/D2Gdi.dll
LINK_TIME="2000-05-26 18:54:22" $T link /nologo /DLL /INCREMENTAL:YES /BASE:0x10000000 /MACHINE:IX86 \
	/DEF:D2Gdi.def /OUT:$O/D2Gdi.dll $O/D2Gdi.obj $O/DllMain.obj $O/Memory.obj $O/gdiCore.obj $O/gdiEngine.obj \
	$O/gdiSmack.obj $O/gdiSpCel.obj $O/gdiSprite.obj $O/Storm.lib $O/D2CMP.lib $O/Fog.lib kernel32.lib user32.lib \
	gdi32.lib $O/smackw32.lib
cp $O/D2Gdi.dll ../out/D2Gdi.dll
cd ..

# D2DDraw.dll
cd D2DDraw
O=../out/D2DDraw
$T cl /nologo /c /O2 /Ob2 /Gr /Oy /Gy /Gf /MT /W3 /FI D2DDrawLinkNames.h /I. /I../include /Fo$O/ \
	D2DDraw.cpp DllMain.cpp ddBink.cpp ddCore.cpp ddDebugFont.cpp ddEngine.cpp ddSmack.cpp ddSpCel.cpp ddSprite.cpp
for m in Storm D2CMP Fog D2Sound; do $T lib /nologo /def:lib/$m.def /machine:ix86 /out:$O/$m.lib >/dev/null; done
python3 lib/mklonglib.py smackw32.dll $O/smackw32.lib $(awk '{print $1":"$2}' lib/smackw32.exports)
python3 lib/mklonglib.py --compid 0x420ff binkw32.dll $O/binkw32.lib $(awk '{print $1":"$2}' lib/binkw32.exports)
rm -f $O/D2DDraw.ilk ../out/D2DDraw.dll
LINK_TIME="2000-05-26 18:53:53" $T link /nologo /DLL /INCREMENTAL:YES /BASE:0x10000000 /MACHINE:IX86 \
	/DEF:D2DDraw.def /OUT:$O/D2DDraw.dll $O/D2DDraw.obj $O/DllMain.obj $O/ddBink.obj $O/ddCore.obj \
	$O/ddDebugFont.obj $O/ddEngine.obj $O/ddSmack.obj $O/ddSpCel.obj $O/ddSprite.obj $O/Storm.lib $O/D2CMP.lib \
	ddraw.lib $O/Fog.lib kernel32.lib gdi32.lib user32.lib $O/binkw32.lib $O/smackw32.lib $O/D2Sound.lib
cp $O/D2DDraw.dll ../out/D2DDraw.dll
cd ..

# D2sound.dll
cd D2sound
O=../out/D2sound
C="$T cl /nologo /c /Gr /Oy /Gy /Gf /MT /W3 /I. /I../include /Fo$O/"
$C /O2 /Ob2 /FI D2SoundLinkNames.h D2DllName.cpp D2Sound.cpp D2SoundFast.cpp D2SoundConfig.cpp D2SoundSmp.cpp \
	D2SoundUtil.cpp
$C /O2 /Ob2 EaxGuid.cpp
$C /O1 /Ob2 Archive.cpp
for m in Storm Fog D2gfx; do $T lib /nologo /def:lib/$m.def /machine:ix86 /out:$O/$m.lib >/dev/null; done
rm -f $O/D2Hell.lib $O/EaxGuid.lib
$T lib /nologo /out:$O/D2Hell.lib $O/Archive.obj >/dev/null
$T lib /nologo /out:$O/EaxGuid.lib $O/EaxGuid.obj >/dev/null
rm -f $O/D2sound.ilk ../out/D2sound.dll
LINK_TIME="2000-05-26 18:53:29" $T link /nologo /DLL /INCREMENTAL:YES /BASE:0x10000000 /MACHINE:IX86 \
	/DEF:D2Sound.def /OUT:$O/D2sound.dll $O/D2DllName.obj $O/D2Sound.obj $O/D2SoundFast.obj $O/D2SoundConfig.obj \
	$O/D2SoundSmp.obj $O/D2SoundUtil.obj $O/Storm.lib version.lib dsound.lib dxguid.lib $O/EaxGuid.lib $O/Fog.lib \
	$O/D2Hell.lib $O/D2gfx.lib kernel32.lib user32.lib
cp $O/D2sound.dll ../out/D2sound.dll
cd ..

# D2Launch.dll
cd D2Launch
O=../out/D2Launch
C="$T cl /nologo /c /Gr /Oy /Gy /Gf /MT /W3 /FI D2LaunchLinkNames.h /I. /I../include /Fo$O/"
$C /O2 /Ob1 BNet.cpp
$C /O2 /Ob2 BNetBox.cpp CharSel.cpp DllMain.cpp MainMenus.cpp D2LaunchEmpty.cpp SaveLoad.cpp
for m in Storm D2Sound D2gfx D2Lang Fog D2MCPClient D2Net; do
	$T lib /nologo /def:lib/$m.def /machine:ix86 /out:$O/$m.lib >/dev/null
done
$T cl /nologo /c /Fo$O/D2WinSyms.obj lib/D2WinSyms.c >/dev/null
$T lib /nologo /def:lib/D2Win.def /machine:ix86 /out:$O/D2Win.lib $O/D2WinSyms.obj >/dev/null
rm -f $O/D2Launch.ilk ../out/D2Launch.dll
LINK_TIME="2000-05-26 19:15:19" $T link /nologo /DLL /INCREMENTAL:YES /BASE:0x10000000 /MACHINE:IX86 \
	/DEF:D2Launch.def /OUT:$O/D2Launch.dll $O/BNet.obj $O/BNetBox.obj $O/CharSel.obj $O/DllMain.obj \
	$O/MainMenus.obj $O/D2LaunchEmpty.obj $O/SaveLoad.obj $O/Storm.lib $O/D2Sound.lib $O/D2gfx.lib $O/D2Win.lib \
	$O/D2Lang.lib $O/Fog.lib $O/D2MCPClient.lib kernel32.lib user32.lib $O/D2Net.lib
cp $O/D2Launch.dll ../out/D2Launch.dll
cd ..

# D2Multi.dll
cd D2Multi
O=../out/D2Multi
C="$T cl /nologo /c /O2 /Ob2 /Gr /Oy /Gy /Gf /MT /W3 /FI D2MultiLinkNames.h /I. /I../include /Fo$O/"
$C ChatDlg.cpp ComCallback.cpp D2MultiMisc.cpp Gate.cpp
for m in Storm D2Win D2Lang Fog D2MCPClient D2Sound; do
	$T lib /nologo /def:lib/$m.def /machine:ix86 /out:$O/$m.lib >/dev/null
done
rm -f $O/D2Multi.ilk ../out/D2Multi.dll
LINK_TIME="2000-05-26 19:15:28" $T link /nologo /DLL /INCREMENTAL:YES /BASE:0x10000000 /MACHINE:IX86 \
	/DEF:D2Multi.def /OUT:$O/D2Multi.dll $O/ChatDlg.obj $O/ComCallback.obj $O/D2MultiMisc.obj $O/Gate.obj \
	$O/Storm.lib $O/D2Win.lib $O/D2Lang.lib $O/Fog.lib $O/D2MCPClient.lib kernel32.lib user32.lib advapi32.lib \
	$O/D2Sound.lib
cp $O/D2Multi.dll ../out/D2Multi.dll
cd ..

# D2Glide.dll
cd D2Glide
O=../out/D2Glide
$T cl /nologo /c /O2 /Ob2 /Gr /Oy /Gy /Gf /MT /W3 /FI D2GlideLinkNames.h /I. /I../include /Fo$O/ \
	D2Glide.cpp glBink.cpp glBlocks.cpp glCore.cpp glDebugFont.cpp glEngine.cpp glFloor.cpp glFloorCache.cpp \
	glPerspective.cpp glSmack.cpp glSprite.cpp glSubtileCache.cpp glTextureCache.cpp glVertex.cpp glWall.cpp
for m in Storm D2CMP Fog D2Sound; do
	$T lib /nologo /def:lib/$m.def /machine:ix86 /out:$O/$m.lib >/dev/null
done
python3 lib/mklonglib.py glide3x.dll $O/glide3x.lib $(awk '{print $1":"$2}' lib/glide3x.exports)
python3 lib/mklonglib.py smackw32.dll $O/smackw32.lib $(awk '{print $1":"$2}' lib/smackw32.exports)
python3 lib/mklonglib.py --compid 0x420ff binkw32.dll $O/binkw32.lib $(awk '{print $1":"$2}' lib/binkw32.exports)
rm -f $O/D2Glide.ilk ../out/D2Glide.dll
LINK_TIME="2000-05-26 18:54:39" $T link /nologo /DLL /INCREMENTAL:YES /BASE:0x10000000 /MACHINE:IX86 \
	/DEF:D2Glide.def /OUT:$O/D2Glide.dll $O/D2Glide.obj $O/glBink.obj $O/glBlocks.obj $O/glCore.obj \
	$O/glDebugFont.obj $O/glEngine.obj $O/glFloor.obj $O/glFloorCache.obj $O/glPerspective.obj $O/glSmack.obj \
	$O/glSprite.obj $O/glSubtileCache.obj $O/glTextureCache.obj $O/glVertex.obj $O/glWall.obj \
	$O/Storm.lib $O/glide3x.lib $O/D2CMP.lib $O/Fog.lib kernel32.lib user32.lib gdi32.lib $O/binkw32.lib \
	$O/smackw32.lib $O/D2Sound.lib
cp $O/D2Glide.dll ../out/D2Glide.dll
cd ..

# D2Direct3D.dll
cd D2Direct3D
O=../out/D2Direct3D
$T cl /nologo /c /O2 /Ob2 /Gr /Oy /Gy /Gf /MT /W3 /FI D2Direct3DLinkNames.h /I. /I../include /Fo$O/ \
	D2Direct3D.cpp d3dBink.cpp d3dBlocks.cpp d3dCore.cpp d3dDebugFont.cpp d3dEngine.cpp d3dFloor.cpp \
	d3dFloorCache.cpp d3dPerspective.cpp d3dSmack.cpp d3dSprite.cpp d3dSubtileCache.cpp d3dTextureCache.cpp \
	d3dVertex.cpp d3dWall.cpp
for m in Storm D2CMP Fog D2Sound; do
	$T lib /nologo /def:lib/$m.def /machine:ix86 /out:$O/$m.lib >/dev/null
done
python3 lib/mklonglib.py smackw32.dll $O/smackw32.lib $(awk '{print $1":"$2}' lib/smackw32.exports)
python3 lib/mklonglib.py --compid 0x420ff binkw32.dll $O/binkw32.lib $(awk '{print $1":"$2}' lib/binkw32.exports)
rm -f $O/D2Direct3D.ilk ../out/D2Direct3D.dll
LINK_TIME="2000-05-26 18:54:11" $T link /nologo /DLL /INCREMENTAL:YES /BASE:0x10000000 /MACHINE:IX86 \
	/DEF:D2Direct3D.def /OUT:$O/D2Direct3D.dll $O/D2Direct3D.obj $O/d3dBink.obj $O/d3dBlocks.obj \
	$O/d3dCore.obj $O/d3dDebugFont.obj $O/d3dEngine.obj $O/d3dFloor.obj $O/d3dFloorCache.obj \
	$O/d3dPerspective.obj $O/d3dSmack.obj $O/d3dSprite.obj $O/d3dSubtileCache.obj $O/d3dTextureCache.obj \
	$O/d3dVertex.obj $O/d3dWall.obj \
	$O/Storm.lib $O/D2CMP.lib $O/Fog.lib ddraw.lib dxguid.lib kernel32.lib user32.lib gdi32.lib $O/binkw32.lib \
	$O/smackw32.lib $O/D2Sound.lib
cp $O/D2Direct3D.dll ../out/D2Direct3D.dll
cd ..

# D2Game.dll
cd D2Game
O=../out/D2Game
C="$T cl /nologo /c /Ox /Gr /Oy /Gf /MT /W3 /I. /Iinc /Fo$O/"
$C /Ob2 GAME/DllMain.cpp GAME/Arena.cpp GAME/CCmd.cpp GAME/Clients.cpp GAME/Event.cpp GAME/Level.cpp GAME/SCmd.cpp \
	GAME/Targets.cpp INVENTORY/InvMode.cpp ITEMS/ItemMode.cpp ITEMS/Items.cpp MISSILES/Missiles.cpp \
	MISSILES/MissMode.cpp MONSTER/Monster.cpp MONSTER/MonsterAI.cpp MONSTER/MonsterEval.cpp MONSTER/MonsterMode.cpp \
	MONSTER/MonsterMsg.cpp OBJECTS/Objects.cpp OBJECTS/OBJEVAL.cpp OBJECTS/ObjMode.cpp OBJECTS/objrgn.cpp \
	PLAYER/PartyScreen.cpp PLAYER/Player.cpp PLAYER/PlayerPets.cpp PLAYER/PlrEval.cpp PLAYER/PlrIntro.cpp \
	PLAYER/PlrModes.cpp PLAYER/PlrMsg.cpp PLAYER/PlrSave.cpp PLAYER/PlrTrade.cpp QUESTS/QUESTS.cpp QUESTS/act1.cpp \
	QUESTS/act2.cpp QUESTS/act3.cpp QUESTS/act4.cpp UNIT/Friendly.cpp UNIT/Party.cpp UNIT/PlayerList.cpp \
	UNIT/SUnit.cpp UNIT/SUnitDmg.cpp UNIT/SUnitInactive.cpp UNIT/SUnitMsg.cpp UNIT/SUNITNPC.cpp UNIT/sunitproxy.cpp \
	DEBUG/Debug.cpp AI/AiGeneral.cpp AI/AiTatics.cpp AI/AiThink.cpp AI/AiUtil.cpp GUILDS/Guilds.cpp \
	SKILLS/SkillAma.cpp SKILLS/SkillBar.cpp SKILLS/SkillItem.cpp SKILLS/SkillMonst.cpp SKILLS/SkillNec.cpp \
	SKILLS/Skills.cpp SKILLS/SkillSor.cpp SKILLS/SkilPal.cpp STANDIN/Empty01.cpp STANDIN/Empty02.cpp \
	STANDIN/Empty03.cpp STANDIN/Empty04.cpp STANDIN/Empty05.cpp STANDIN/Empty06.cpp STANDIN/Empty07.cpp \
	STANDIN/Empty08.cpp STANDIN/Empty09.cpp STANDIN/Empty10.cpp
$C /Ob2 /GX GAME/Game.cpp GAME/GameTable.cpp GAME/Task.cpp
$C /Ob1 MONSTER/MonsterSpawn.cpp MONSTER/MonsterUnique.cpp
for m in D2CMP D2Net D2Common D2Lang Fog Storm; do
	$T lib /nologo /def:lib/$m.def /machine:ix86 /out:$O/$m.lib >/dev/null
done
rm -f $O/D2Game.dll ../out/D2Game.dll
LINK_TIME="2000-05-26 19:01:13" $T link /nologo /DLL /INCREMENTAL:NO /OPT:REF,ICF /BASE:0x10000000 /MACHINE:IX86 \
	/DEF:D2Game.def /OUT:$O/D2Game.dll $O/DllMain.obj $O/Arena.obj $O/CCmd.obj $O/Clients.obj $O/Event.obj \
	$O/Game.obj $O/GameTable.obj $O/Level.obj $O/SCmd.obj $O/Targets.obj $O/Task.obj $O/InvMode.obj \
	$O/ItemMode.obj $O/Items.obj $O/Missiles.obj $O/MissMode.obj $O/Monster.obj $O/MonsterAI.obj \
	$O/MonsterEval.obj $O/MonsterMode.obj $O/MonsterMsg.obj $O/MonsterSpawn.obj $O/MonsterUnique.obj \
	$O/Objects.obj $O/OBJEVAL.obj $O/ObjMode.obj $O/objrgn.obj $O/PartyScreen.obj $O/Player.obj \
	$O/PlayerPets.obj $O/PlrEval.obj $O/PlrIntro.obj $O/PlrModes.obj $O/PlrMsg.obj $O/PlrSave.obj \
	$O/PlrTrade.obj $O/QUESTS.obj $O/act1.obj $O/act2.obj $O/act3.obj $O/act4.obj $O/Friendly.obj \
	$O/Party.obj $O/PlayerList.obj $O/SUnit.obj $O/SUnitDmg.obj $O/SUnitInactive.obj $O/SUnitMsg.obj \
	$O/SUNITNPC.obj $O/sunitproxy.obj $O/Debug.obj $O/AiGeneral.obj $O/AiTatics.obj $O/AiThink.obj \
	$O/AiUtil.obj $O/Guilds.obj $O/SkillAma.obj $O/SkillBar.obj $O/SkillItem.obj $O/SkillMonst.obj \
	$O/SkillNec.obj $O/Skills.obj $O/SkillSor.obj $O/SkilPal.obj $O/Empty01.obj $O/Empty02.obj \
	$O/Empty03.obj $O/Empty04.obj $O/Empty05.obj $O/Empty06.obj $O/Empty07.obj $O/Empty08.obj \
	$O/Empty09.obj $O/Empty10.obj \
	$O/D2CMP.lib $O/D2Net.lib $O/D2Common.lib $O/D2Lang.lib $O/Fog.lib kernel32.lib user32.lib $O/Storm.lib \
	winmm.lib
cp $O/D2Game.dll ../out/D2Game.dll
cd ..

# D2Common.dll
cd D2Common
O=../out/D2Common
$T cl /nologo /c /O2 /Ob2 /Gr /Oy /Gy /Gf /MT /W3 /I. /I../include /Fo$O/ \
	COLLISN/Collisn.cpp COMPOSIT/Composit.cpp DATATBLS/AnimTbls.cpp DATATBLS/Arena.cpp DATATBLS/BeltTbls.cpp \
	DATATBLS/DataTbls.cpp DATATBLS/FieldTbls.cpp DATATBLS/InvTbls.cpp DATATBLS/ItemTbls.cpp DATATBLS/LvlTbls.cpp \
	DATATBLS/MissileTbls.cpp DATATBLS/MonsterTbls.cpp DATATBLS/ObjTbls.cpp DATATBLS/OverlayTbls.cpp \
	DATATBLS/Sequence.cpp DATATBLS/TokTbls.cpp DATATBLS/TransformTbls.cpp DRLG/DrlgRing.cpp DRLG/Drlg.cpp \
	DRLG/DrlgAnim.cpp DRLG/DrlgGrid.cpp DRLG/DrlgLogic.cpp DRLG/DrlgRoom.cpp DRLG/DrlgVer.cpp DRLG/DrlgWarp.cpp \
	DRLG/Maze.cpp DRLG/OutDesr.cpp DRLG/Outdoors.cpp DRLG/OutRoads.cpp DRLG/OutMesa.cpp DRLG/OutJung.cpp \
	DRLG/OutEntry.cpp DRLG/OutPlace.cpp DRLG/OutRoom.cpp DRLG/OutWild.cpp DRLG/Preset.cpp DRLG/RoomTile.cpp \
	DRLG/TileSub.cpp DUNGEON/Dungeon.cpp ENVIRONMENT/Env.cpp EXCEL/Excel.cpp INVENTORY/Inventory.cpp \
	INVENTORY/InvList.cpp ITEMS/ItemMods.cpp ITEMS/Items.cpp LOGGING/Logging.cpp LOGGING/ProfCore.cpp \
	MONSTERS/MonsterKillsLog.cpp MONSTERS/MonsterRand.cpp MONSTERS/MonsterRegion.cpp MONSTERS/MONSTERS.cpp \
	PATH/AStar.cpp PATH/IDAstar.cpp PATH/Path.cpp PATH/PathMisc.cpp PATH/PathUtil.cpp PATH/PathWF.cpp \
	PATH/Step.cpp QUESTRECORD/QuestRecord.cpp RANDOM/Random.cpp SKILLS/Skills.cpp STATS/States.cpp \
	STATS/Stats.cpp UNITS/Missile.cpp UNITS/UnitFinds.cpp UNITS/UnitRoom.cpp UNITS/Units.cpp UNITS/UnitUtil.cpp \
	TEXT/Text.cpp CHAT/Chat.cpp WAYPOINT/Waypoint.cpp GUILDS/Guilds.cpp SAVELOAD/saveload.cpp STANDIN/Empty01.cpp \
	STANDIN/Empty02.cpp STANDIN/Empty03.cpp STANDIN/Empty04.cpp STANDIN/Empty05.cpp STANDIN/Empty06.cpp \
	STANDIN/Empty07.cpp STANDIN/Empty08.cpp
OBJ="$O/Collisn.obj $O/Composit.obj $O/AnimTbls.obj $O/Arena.obj $O/BeltTbls.obj $O/DataTbls.obj
	$O/FieldTbls.obj $O/InvTbls.obj $O/ItemTbls.obj $O/LvlTbls.obj $O/MissileTbls.obj $O/MonsterTbls.obj
	$O/ObjTbls.obj $O/OverlayTbls.obj $O/Sequence.obj $O/TokTbls.obj $O/TransformTbls.obj $O/DrlgRing.obj
	$O/Drlg.obj $O/DrlgAnim.obj $O/DrlgGrid.obj $O/DrlgLogic.obj $O/DrlgRoom.obj $O/DrlgVer.obj
	$O/DrlgWarp.obj $O/Maze.obj $O/OutDesr.obj $O/Outdoors.obj $O/OutRoads.obj $O/OutMesa.obj
	$O/OutJung.obj $O/OutEntry.obj $O/OutPlace.obj $O/OutRoom.obj $O/OutWild.obj $O/Preset.obj
	$O/RoomTile.obj $O/TileSub.obj $O/Dungeon.obj $O/Env.obj $O/Excel.obj $O/Inventory.obj $O/InvList.obj
	$O/ItemMods.obj $O/Items.obj $O/Logging.obj $O/ProfCore.obj $O/MonsterKillsLog.obj $O/MonsterRand.obj
	$O/MonsterRegion.obj $O/MONSTERS.obj $O/AStar.obj $O/IDAstar.obj $O/Path.obj $O/PathMisc.obj
	$O/PathUtil.obj $O/PathWF.obj $O/Step.obj $O/QuestRecord.obj $O/Random.obj $O/Skills.obj $O/States.obj
	$O/Stats.obj $O/Missile.obj $O/UnitFinds.obj $O/UnitRoom.obj $O/Units.obj $O/UnitUtil.obj $O/Text.obj
	$O/Chat.obj $O/Waypoint.obj $O/Guilds.obj $O/saveload.obj $O/Empty01.obj $O/Empty02.obj $O/Empty03.obj
	$O/Empty04.obj $O/Empty05.obj $O/Empty06.obj $O/Empty07.obj $O/Empty08.obj"
$T cl /nologo /c /O1 /Ob2 /Gr /Oy /Gy /Gf /MT /W3 /Ihell /I. /I../include /Fo$O/ hell/HellArchive.cpp \
	hell/HellString.cpp
rm -f $O/D2Hell.lib
$T lib /nologo /out:$O/D2Hell.lib $O/HellArchive.obj $O/HellString.obj >/dev/null
for m in Storm D2CMP D2Lang Fog; do $T lib /nologo /def:lib/$m.def /machine:ix86 /out:$O/$m.lib >/dev/null; done
rm -rf $O/ren
python3 lib/renobj.py lib/selfnames.txt $O/ren $OBJ
REN=$(for o in $OBJ; do printf '%s/ren/%s ' $O ${o#$O/}; done)
for try in 1 2 3; do
	rm -f $O/D2Common.ilk $O/D2Common.dll ../out/D2Common.dll
	LINK_TIME="@2000-05-26 18:58:00 i0.00459" $T link /nologo /DLL /INCREMENTAL:YES /BASE:0x10000000 /MACHINE:IX86 \
		/DEF:D2Common.def /OUT:$O/D2Common.dll $REN \
		$O/Storm.lib $O/D2Hell.lib $O/D2CMP.lib $O/D2Lang.lib $O/Fog.lib kernel32.lib user32.lib
	[ "$(sha256sum $O/D2Common.dll | cut -d' ' -f1)" = 6e8b73325ec6b885213ecc9a98017ec5cf6d6984ff4a2735c73a3f3595e0544c ] && break
done
cp $O/D2Common.dll ../out/D2Common.dll
cd ..

# D2MultiLAN.dll
cd D2MultiLAN
O=../out/D2MultiLAN
for t in D2MultiLAN1 D2MultiLAN2 D2MultiLAN3; do
	$T cl /nologo /c /O2 /Ob2 /Gr /Oy /Gy /Gf /MT /W3 /Z7 /I. /I../include /Fo$O/ ../D2MultiLAN/../D2MultiLAN/$t.cpp
done
python3 lib/coffpad.py $O/D2MultiLAN1.obj -L '?sExit@@YGHPAU_PARAMS@@@Z:0x307e4'
python3 lib/coffpad.py $O/D2MultiLAN3.obj -s '?sCreateGameNameCallback@@YGHPAXHPAD@Z:48' \
	-f '?sJoinGameNameCallback@@YGHPAXHPAD@Z' -f '?sCreateGameNameCallback@@YGHPAXHPAD@Z' \
	-f '?sCreateGameNamePostCallback@@YGHPAU_PARAMS@@@Z' \
	-L '?sJoinGameNameCallback@@YGHPAXHPAD@Z:0x34359' -l '?sJoinGameNamePostCallback@@YGHPAU_PARAMS@@@Z:0x344a2' \
	-l '?sCreateGameNameCallback@@YGHPAXHPAD@Z:0x35b34' -l '?sCreateGameNamePostCallback@@YGHPAU_PARAMS@@@Z:0x35d22' \
	-l '?sPasswordCallback@@YGHPAXHPAD@Z:0x35ded'
for m in Storm D2Win D2Net D2Lang; do $T lib /nologo /def:lib/$m.def /machine:ix86 /out:$O/$m.lib >/dev/null; done
W=../toolchain/wineprefix/drive_c/D2/Release
rm -rf "$W"; mkdir -p "$W/Obj/D2MultiLAN"
cp $O/D2MultiLAN1.obj $O/D2MultiLAN2.obj $O/D2MultiLAN3.obj D2MultiLAN.def $O/Storm.lib $O/D2Win.lib $O/D2Net.lib \
	$O/D2Lang.lib "$W/Obj/D2MultiLAN/"
rm -f ../out/D2MultiLAN.dll
(cd "$W/Obj/D2MultiLAN" && LINK_TIME="2000-05-18 22:50:12" $T link /nologo /DLL /INCREMENTAL:NO /BASE:0x10000000 \
	/MACHINE:IX86 /DEF:D2MultiLAN.def /DEBUG /DEBUGTYPE:BOTH /PDB:C:\\D2\\Release\\Obj\\D2MultiLAN\\D2MultiLAN.pdb \
	/OUT:\\D2\\Release\\D2MultiLAN.dll D2MultiLAN1.obj D2MultiLAN2.obj D2MultiLAN3.obj Storm.lib D2Win.lib D2Net.lib \
	D2Lang.lib kernel32.lib)
cp "$W/D2MultiLAN.dll" ../out/D2MultiLAN.dll
python3 -c 'import sys; f = open(sys.argv[1], "r+b"); f.seek(0x1d005); f.write(b"\\D2")' ../out/D2MultiLAN.dll
cd ..

# Bnclient.dll
cd Bnclient
O=../out/Bnclient
C="$T cl /nologo /c /Gr /Oy /Gy /Gf /MT /W3 /I. /I../include /Fo$O/"
$C /O2 /Ob2 BnChatName.cpp BnclientMisc.cpp BnMessQueue.cpp BNetGW.cpp BnDownload.cpp grid.cpp BnSend.cpp \
	BnConnect.cpp GridCrypt.cpp BnCDKey.cpp BnRegister.cpp CACHE.cpp BnName.cpp
$C /O2 /Ob1 BnConnMsg.cpp GridSha.cpp
for m in Storm D2CMP D2Lang Fog; do $T lib /nologo /def:lib/$m.def /machine:ix86 /out:$O/$m.lib >/dev/null; done
$T rc /fo $O/Bnclient.res Bnclient.rc >/dev/null
rm -f $O/Bnclient.ilk ../out/Bnclient.dll
LINK_TIME="2000-05-26 18:55:22" $T link /nologo /DLL /INCREMENTAL:YES /BASE:0x10000000 /MACHINE:IX86 \
	/DEF:Bnclient.def /OUT:$O/Bnclient.dll $O/BnChatName.obj $O/BnConnect.obj $O/BnConnMsg.obj $O/BnDownload.obj \
	$O/BNetGW.obj $O/BnMessQueue.obj $O/BnCDKey.obj $O/BnRegister.obj $O/BnSend.obj $O/CACHE.obj $O/BnName.obj \
	$O/BnclientMisc.obj $O/grid.obj $O/GridCrypt.obj $O/GridSha.obj $O/Storm.lib $O/D2CMP.lib wsock32.lib \
	$O/D2Lang.lib $O/Fog.lib kernel32.lib user32.lib $O/Bnclient.res
cp $O/Bnclient.dll ../out/Bnclient.dll
cd ..

# Fog.dll
cd Fog
O=../out/Fog
C="$T cl /nologo /c /Gr /Oy /Gy /Gf /MT /W3 /I. /I../include /Fo$O/"
$C /O2 /Ob2 String/FogString.cpp BitManip/StringPack.cpp BitManip/StringHash.cpp FogMain.cpp D2QSQueue.cpp \
	DataArrays.cpp BitManip/BitManip.cpp FogTime.cpp FogSystem.cpp AsyncData.cpp FogCpu.cpp D2QSSocket.cpp \
	FogTrig.cpp QServer/QServer98.cpp QServer/QServerNT.cpp Mem/MemStub.cpp Mem/Performance.cpp Safesock.cpp \
	QServer/QServer.cpp
$C /O2 /Ob1 Mem/mem.cpp ErrorManager.cpp
$T lib /nologo /def:lib/Storm.def /machine:ix86 /out:$O/Storm.lib >/dev/null
rm -f $O/Fog.ilk ../out/Fog.dll
LINK_TIME="2000-05-26 18:52:04" $T link /nologo /DLL /INCREMENTAL:YES /BASE:0x10000000 /MACHINE:IX86 \
	/DEF:Fog.def /OUT:$O/Fog.dll $O/FogMain.obj $O/D2QSQueue.obj $O/D2QSSocket.obj $O/FogTrig.obj $O/BitManip.obj \
	$O/StringPack.obj $O/StringHash.obj $O/FogString.obj $O/QServer.obj $O/QServer98.obj $O/QServerNT.obj $O/mem.obj \
	$O/MemStub.obj $O/Performance.obj $O/FogTime.obj $O/FogSystem.obj $O/AsyncData.obj $O/FogCpu.obj \
	$O/DataArrays.obj $O/ErrorManager.obj $O/Safesock.obj $O/Storm.lib wsock32.lib kernel32.lib user32.lib
cp $O/Fog.dll ../out/Fog.dll
cd ..

# D2gfx.dll
cd D2gfx
O=../out/D2gfx
C="$T cl /nologo /c /Gr /Oy /Gy /Gf /MT /W3 /I. /I../include /Fo$O/"
$C /O2 /Ob2 CmnInfo.cpp CmnSubtile.cpp D2gfxName.cpp D2gfx.cpp ScaleDraw.cpp ScalePalette.cpp ScaleText.cpp \
	ScaleZoom.cpp Scale.cpp ScaleCel.cpp ScaleWall.cpp Window.cpp
$C /O1 /Ob2 /GX HellReport.cpp D2gfxMisc.cpp
$C /O1 /Ob2 HellWindow.cpp
for m in Storm Fog; do $T lib /nologo /def:lib/$m.def /machine:ix86 /out:$O/$m.lib >/dev/null; done
rm -f $O/D2Hell.lib
$T lib /nologo /out:$O/D2Hell.lib $O/HellReport.obj $O/D2gfxMisc.obj $O/HellWindow.obj >/dev/null
rm -f $O/D2gfx.ilk ../out/D2gfx.dll
LINK_TIME="2000-05-26 18:53:03" $T link /nologo /DLL /INCREMENTAL:YES /BASE:0x10000000 /MACHINE:IX86 \
	/DEF:D2gfx.def /OUT:$O/D2gfx.dll $O/CmnInfo.obj $O/CmnSubtile.obj $O/D2gfxName.obj $O/D2gfx.obj \
	$O/ScaleDraw.obj $O/ScalePalette.obj $O/ScaleText.obj $O/ScaleZoom.obj $O/Scale.obj $O/ScaleCel.obj \
	$O/ScaleWall.obj $O/Window.obj $O/Storm.lib $O/D2Hell.lib $O/Fog.lib kernel32.lib user32.lib gdi32.lib \
	../toolchain/sdklib1999/shell32.lib
cp $O/D2gfx.dll ../out/D2gfx.dll
cd ..

# D2Win.dll
cd D2Win
O=../out/D2Win
C="$T cl /nologo /c /Gr /Oy /Gy /Gf /MT /W3 /I. /I../include /Fo$O/"
$C /O2 /Ob2 D2WinProgressBar.cpp D2WRegister.cpp D2WinTimer.cpp D2WinPopup.cpp D2WinSmack.cpp D2WinAccountList.cpp \
	D2WinImage.cpp D2WinPalette.cpp D2WinScrollbar.cpp D2WinArchive.cpp D2WinButton.cpp D2WinDll.cpp D2WinList.cpp \
	D2WinTextBox.cpp D2WinEditBox.cpp D2WinFont.cpp D2WinMain.cpp D2Comp.cpp D2WinAnimImage.cpp
$C /O1 /Ob2 HellArchive.cpp HellBits.cpp
$T cl /nologo /c /Fo$O/D2gfxSyms.obj lib/D2gfxSyms.c >/dev/null
$T lib /nologo /def:lib/D2gfx.def /machine:ix86 /out:$O/D2gfx.lib $O/D2gfxSyms.obj >/dev/null
for m in Storm D2CMP D2Lang Fog D2Sound; do $T lib /nologo /def:lib/$m.def /machine:ix86 /out:$O/$m.lib >/dev/null; done
rm -f $O/D2Hell.lib
$T lib /nologo /out:$O/D2Hell.lib $O/HellArchive.obj $O/HellBits.obj >/dev/null
rm -f $O/D2Win.ilk ../out/D2Win.dll
LINK_TIME="2000-05-26 18:55:04" $T link /nologo /DLL /INCREMENTAL:YES /BASE:0x10000000 /MACHINE:IX86 \
	/DEF:D2Win.def /OUT:$O/D2Win.dll $O/D2Comp.obj $O/D2WinAccountList.obj $O/D2WinAnimImage.obj \
	$O/D2WinArchive.obj $O/D2WinButton.obj $O/D2WinDll.obj $O/D2WinEditBox.obj $O/D2WinFont.obj $O/D2WinImage.obj \
	$O/D2WinList.obj $O/D2WinMain.obj $O/D2WinPalette.obj $O/D2WinPopup.obj $O/D2WinProgressBar.obj \
	$O/D2WinScrollbar.obj $O/D2WinSmack.obj $O/D2WinTextBox.obj $O/D2WinTimer.obj $O/D2WRegister.obj $O/Storm.lib \
	$O/D2CMP.lib ../toolchain/ijl11/ijl11.lib $O/D2Hell.lib $O/D2gfx.lib $O/D2Lang.lib $O/Fog.lib kernel32.lib \
	user32.lib $O/D2Sound.lib
cp $O/D2Win.dll ../out/D2Win.dll
cd ..

# D2Server.dll (its icon is read from an original D2Server.dll: D2SERVER_DLL=<file> sh build.sh)
if [ -n "$D2SERVER_DLL" ]; then
cd D2Server
O=../out/D2Server
C="$T cl /nologo /c /O2 /Ob2 /Gr /Oy /Gy /Gf /MT /W3 /I. /I../include /Fo$O/"
$C Archives.cpp Bnet.cpp CharServ.cpp Console.cpp Display.cpp FromMCP.cpp Inifile.cpp Perf.cpp PlayerToken.cpp \
	ToMCP.cpp Empty.cpp
$C /GX WinMain.cpp
$T cl /nologo /c /O1 /Ob2 /Gr /Oy /Gy /Gf /MT /W3 /Ihell /I. /I../include /Fo$O/HellArchive.obj hell/HellArchive.cpp
rm -f $O/D2Hell.lib
$T lib /nologo /out:$O/D2Hell.lib $O/HellArchive.obj >/dev/null
for m in Storm D2Net D2Common D2Game D2Lang Fog; do
	$T lib /nologo /def:lib/$m.def /machine:ix86 /out:$O/$m.lib >/dev/null
done
python3 lib/mkico.py "$D2SERVER_DLL" $O
$T rc /i $O /fo $O/D2Server.res D2Server.rc >/dev/null
rm -f $O/D2Server.ilk ../out/D2Server.dll
LINK_TIME="2000-05-26 19:15:40" $T link /nologo /DLL /INCREMENTAL:YES /BASE:0x10000000 /MACHINE:IX86 \
	/DEF:D2Server.def /OUT:$O/D2Server.dll $O/Archives.obj $O/Bnet.obj $O/CharServ.obj $O/Console.obj \
	$O/Display.obj $O/FromMCP.obj $O/Inifile.obj $O/Perf.obj $O/PlayerToken.obj $O/ToMCP.obj $O/WinMain.obj \
	$O/Empty.obj $O/Storm.lib $O/D2Hell.lib $O/D2Net.lib $O/D2Common.lib $O/D2Game.lib $O/D2Lang.lib \
	$O/Fog.lib kernel32.lib user32.lib gdi32.lib wsock32.lib $O/D2Server.res
cp $O/D2Server.dll ../out/D2Server.dll
cd ..
fi

# D2VidTst.exe (its icon is read from an original D2VidTst.exe: D2VIDTST_EXE=<file> sh build.sh)
if [ -n "$D2VIDTST_EXE" ]; then
cd D2VidTst
O=../out/D2VidTst
F="/nologo /c /Gr /Oy /Gy /Gf /MT /GX /W3 /I. /I../include /Fo$O/"
$T cl $F /O1 /Ob2 D2Hell/SRC/Report.cpp D2Hell/SRC/Window.cpp
$T cl-sp3 $F /O2 D2VidTstMisc.cpp VidTstD3D.cpp VidTstDDraw.cpp VidTstGlide.cpp
$T cl-sp3 $F /O1 /Ob2 D2Hell/SRC/Fault.cpp
for m in Storm Fog; do $T lib /nologo /def:lib/$m.def /machine:ix86 /out:$O/$m.lib >/dev/null; done
rm -f $O/D2Hell.lib
$T lib /nologo /out:$O/D2Hell.lib $O/Report.obj $O/Fault.obj $O/Window.obj >/dev/null
python3 lib/mkico.py "$D2VIDTST_EXE" $O
$T rc /i $O /fo $O/D2VidTst.res D2VidTst.rc >/dev/null
$T cvtres-sp3 /nologo /readonly /machine:ix86 /out:$O/D2VidTst.rbj $O/D2VidTst.res >/dev/null
rm -f ../out/D2VidTst.exe
LINK_TIME="2000-05-04 18:41:02" $T link /nologo /INCREMENTAL:NO /SUBSYSTEM:WINDOWS /MACHINE:IX86 \
	/OUT:$O/D2VidTst.exe $O/D2VidTstMisc.obj $O/VidTstD3D.obj $O/VidTstDDraw.obj $O/VidTstGlide.obj $O/Storm.lib \
	$O/D2Hell.lib dxguid.lib $O/Fog.lib ../toolchain/sdklib1999/ddraw.lib libcmt.lib user32.lib gdi32.lib \
	kernel32.lib advapi32.lib $O/D2VidTst.rbj
cp $O/D2VidTst.exe ../out/D2VidTst.exe
cd ..
fi

sha256sum -c <<EOF
7876cc8dfe779102f1218a5c7352a6962c95e366ddd524e454332faf9554d0fc  out/D2Net.dll
74ae0debb325048a1ae76f8360f5a826203fa3627fc02f8a62aaa3ae0d305a4b  out/D2Lang.dll
2dba706661c163302c3900b1a640b6136f8954d6d3fc735d6882fc1331bfc523  out/D2MCPClient.dll
84511be68e9f25c805ea755c1444544338d093a0348ccfa30d2dd8ce9ab907a8  out/D2Gdi.dll
2650e8a49d6a8c09fe1f3c3425873e94a9b3125ea95de98ba447e025940d9cbb  out/D2DDraw.dll
d403adb406b93c76c8c0d1bea131b648754fca3a8337120314bfe3e0f6e3b06c  out/D2sound.dll
94ec7c4a04e5020aa5eaca8a0a80904393ce774af8315294a55fc6385e6a1e6c  out/D2Launch.dll
5dccb996981f60ac2bc5fcaea4e9f033dc1c1ccb4fab9b8d608b76de9fa7fd4f  out/D2Multi.dll
34bcf5a35fd5197dd859ab0d704460ae007d16095f7693f48d47a7db88cda381  out/D2Glide.dll
1a070bb5c8ee4d159a333a1abd0cad5608ede2fa12cdd70e14e3e1b1e96291b1  out/D2Direct3D.dll
695024b9f2c9f0e9446a1b224d823d4a8d5f9424b6cc314953f01a5a34eaebc4  out/D2Game.dll
6e8b73325ec6b885213ecc9a98017ec5cf6d6984ff4a2735c73a3f3595e0544c  out/D2Common.dll
45b2a54d4108cba651a0de5fa165cf99e10be110352bff566b4c30d0cf3aa2d6  out/D2MultiLAN.dll
5903adc1db1ce25efe5d8609dca9e67430668a5b2035f8cde7a57b28eb9a3cb9  out/Bnclient.dll
1c6c1bd1b2fd0ede64dd8035a36bc4f69a3872198268f998ca2bbdd7ae7d8be6  out/Fog.dll
ad23ccc059f07e1cbdef14e30d2ab0ce35d708d6e73af7a35e89d9bea9d08aec  out/D2gfx.dll
0f64dc8bb63d9962face3426d78d645b6b581f33d090d5f9304ae81b23fa0b8c  out/D2Win.dll
EOF
if [ -n "$D2SERVER_DLL" ]; then
	echo "ccaaeb7800376991482e74f4e0e2c01f16088a197d45d38f39eb16ee50c2513b  out/D2Server.dll" | sha256sum -c
else
	echo "out/D2Server.dll: not built (set D2SERVER_DLL)"
fi
if [ -n "$D2VIDTST_EXE" ]; then
	echo "2c3d08db2d9595a4b2f3462e53be571436a70c9d883abdb008b170e8d95ecc26  out/D2VidTst.exe" | sha256sum -c
else
	echo "out/D2VidTst.exe: not built (set D2VIDTST_EXE)"
fi
