#include "test.hpp"
#include "Cool/File/File.h"
#include "Cool/Log/ToUser.h"
#include "Cool/OSC/OSCManager.h"
#include "sol/types.hpp"
#define SOL_ALL_SAFETIES_ON 1
#include "sol/sol.hpp"

namespace Cool {

void test_lua()
{
    sol::state lua;
    auto const coollab = (lua["Coollab"] = lua.create_table());

    coollab["log"] = [](std::string const& message) {
        Cool::Log::ToUser::info("Script", message);
    };
    coollab["log_warning"] = [](std::string const& message) {
        Cool::Log::ToUser::warning("Script", message);
    };
    coollab["log_error"] = [](std::string const& message) {
        Cool::Log::ToUser::error("Script", message);
    };
    coollab["set_value"] = [](std::string const& name, float value) {
        Cool::osc_manager().set_value({name}, value);
    };

    // lua.load(sol::lib::base);
    // lua.open_libraries(sol::lib::base, sol::lib::package);

    lua.script(*Cool::File::to_string("C:/Dev/Cool/Lab/bob.lua"));
}

// auto log(lua_State* L) -> int
// {
//     const char* message = lua_tostring(L, -1);
//     Cool::Log::ToUser::info("Script", message);
//     return 0;
// }
// auto log_warning(lua_State* L) -> int
// {
//     const char* message = lua_tostring(L, -1);
//     Cool::Log::ToUser::warning("Script", message);
//     return 0;
// }
// auto log_error(lua_State* L) -> int
// {
//     const char* message = lua_tostring(L, -1);
//     Cool::Log::ToUser::error("Script", message);
//     return 0;
// }
// auto set_value(lua_State* L) -> int
// {
//     const char* name  = lua_tostring(L, -2);
//     lua_Number  value = lua_tonumber(L, -1);
//     Cool::osc_manager().set_value({name}, value);
//     return 0;
// }

// void test_lua()
// {
//     lua_State* L = luaL_newstate();
//     lua_newtable(L);

//     lua_pushstring(L, "log");
//     lua_pushcfunction(L, log);
//     lua_settable(L, -3);

//     lua_pushstring(L, "log_warning");
//     lua_pushcfunction(L, log_warning);
//     lua_settable(L, -3);

//     lua_pushstring(L, "log_error");
//     lua_pushcfunction(L, log_error);
//     lua_settable(L, -3);

//     lua_pushstring(L, "set_value");
//     lua_pushcfunction(L, set_value);
//     lua_settable(L, -3);

//     lua_setglobal(L, "Coollab");
//     luaL_dostring(L, Cool::File::to_string("C:/Dev/Cool/Lab/bob.lua")->c_str());
//     // lua_getglobal(L, "x");
//     // auto const x = lua_tonumber(L, -1);
//     // Cool::Log::ToUser::info("Script", fmt::format("{}", x));
// }

} // namespace Cool