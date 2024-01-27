#include "gamemode.hpp"
#include "objectdir.hpp"
#include "system.hpp"
#include "data.hpp"
#include "symbols.hpp"
#include "messages.hpp"
#include "platformmgr.hpp"
#include "messages/modechangedmsg.hpp"
#include "hmx/object.hpp"
#include "datautil.hpp"

GameMode* TheGameMode;
extern PlatformMgr ThePlatformMgr;

void GameModeInit(){
    TheGameMode = new GameMode();
}

GameMode::GameMode(){
    SetName("gamemode", ObjectDir::sMainDir);
    SetMode("init");
}

GameMode::~GameMode(){

}

bool GameMode::InMode(Symbol target){
    if(curMode == target) return true;

    DataArray* modes = SystemConfig("modes");
    Symbol iter = curMode;
    
    while(modes->FindArray(iter, true)->FindArray(SymParentMode, false)){
        iter = modes->FindArray(iter, true)->FindArray(SymParentMode, true)->GetSymAtIndex(1);
        if(iter == target) return true;
    }
    
    return false;
}

DataNode GameMode::OnSetMode(const DataArray* da){
    da->GetNodeCount();
    SetMode(da->GetSymAtIndex(2));
    return DataNode(0);
}

void GameMode::SetMode(Symbol mode){
    if(curMode == mode) return;
    else {
        DataArray* cfg = SystemConfig("modes");
        Hmx::Object::HandleType(MsgExit.GetArray());
        curMode = mode;
        DataArray* cloned = cfg->FindArray(curMode, true)->Clone(true, false, 0);
        if(cloned->FindArray(SymParentOnly, false) != 0){
            cloned->FindArray(SymParentOnly, true)->GetIntAtIndex(1);
        }
        Symbol iter = curMode;
        while(cfg->FindArray(iter, true)->FindArray(SymParentMode, false) != 0){
            iter = cfg->FindArray(iter, true)->FindArray(SymParentMode, true)->GetSymAtIndex(1);
            DataMergeTags(cloned, cfg->FindArray(iter, true));
        }
        DataMergeTags(cloned, cfg->FindArray("defaults", true));
        SetTypeDef(cloned);
        cloned->DecRefCount();
        Hmx::Object::HandleType(MsgEnter.GetArray());
        ThePlatformMgr.SetUnkCE55(Property("online_play_required", true)->Int(0));
        unk20 = Property("enable_overdrive", true)->Int(0);
        unk2c = InMode("h2h");
        unk30 = InMode("practice");
        unk34 = Property("can_lose", true)->Int(0);
        unk3c = Property("crowd_reacts", true)->Int(0);
        unk40 = Property("is_solo", true)->Int(0);
        unk44 = Property("disable_guitar_fx", true)->Int(0);
        unk48 = InMode("drum_trainer");
        unk50 = Property("enable_streak", true)->Int(0);
        unk58 = InMode("tutorial");
        unk5c = Property("load_chars", true)->Int(0);
        unk60 = Property("show_stars", true)->Int(0);
        unk64 = Property("has_song_sections", true)->Int(0);
        unk68 = Property("enable_whammy", true)->Int(0);
        unk6c = Property("end_with_song", true)->Int(0);
        unk70 = InMode("story");
        static ModeChangedMsg msg;
        MsgSource::Handle(msg.GetArray(), false);
    }
}

ModeChangedMsg::ModeChangedMsg() : Message(Type()) {
    
}

Symbol ModeChangedMsg::Type(){
    static Symbol t("mode_changed");
    return t;
}

void PlatformMgr::SetUnkCE55(bool b){
    unkCE55 = b;
}

extern char* PathName(const Hmx::Object*);

DataNode GameMode::Handle(DataArray* da, bool b){
    Symbol sym = da->GetSymAtIndex(1);
    if(sym == SymInMode){
        return DataNode(InMode(da->GetSymAtIndex(2)));
    }
    if(sym == SymGetMode){
        return DataNode(curMode.m_string);
    }
    if(sym == SymSetMode){
        DataNode node = OnSetMode(da);
        if(node.GetType() != kDataUnhandled) return DataNode(node);
    }
    {
    DataNode hmxnode = Hmx::Object::Handle(da, false);
    if(hmxnode.GetType() != kDataUnhandled) return DataNode(hmxnode);
    }
    {
    DataNode msgnode = MsgSource::Handle(da, false);
    if(msgnode.GetType() != kDataUnhandled) return DataNode(msgnode);
    }
    if(b) PathName(this);
    return DataNode(kDataUnhandled, 0);
}