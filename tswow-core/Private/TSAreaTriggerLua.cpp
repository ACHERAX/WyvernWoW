#include "TSLua.h"
#include "TSAreaTrigger.h"
#include "TSEvents.h"

#include <vector>

void TSLuaState::load_areatrigger_methods(uint32_t modid)
{
#if TRINITY
    auto ts_areatriggerentry = new_usertype<TSAreaTriggerEntry>("TSAreaTriggerEntry");
    LUA_FIELD(ts_areatriggerentry, TSAreaTriggerEntry, GetEntry);
    LUA_FIELD(ts_areatriggerentry, TSAreaTriggerEntry, GetContinentID);
    LUA_FIELD(ts_areatriggerentry, TSAreaTriggerEntry, GetX);
    LUA_FIELD(ts_areatriggerentry, TSAreaTriggerEntry, GetY);
    LUA_FIELD(ts_areatriggerentry, TSAreaTriggerEntry, GetZ);
    LUA_FIELD(ts_areatriggerentry, TSAreaTriggerEntry, GetRadius);
    LUA_FIELD(ts_areatriggerentry, TSAreaTriggerEntry, GetBoxLength);
    LUA_FIELD(ts_areatriggerentry, TSAreaTriggerEntry, GetBoxWidth);
    LUA_FIELD(ts_areatriggerentry, TSAreaTriggerEntry, GetBoxHeight);
    LUA_FIELD(ts_areatriggerentry, TSAreaTriggerEntry, GetBoxYaw);
#endif
}
