
local ModPath = ModPath

-- Replace the log function to accurately log things.
local enter_here = ModPath.."/EnterHere"
if not SystemFS:exists(enter_here) then
    SystemFS:make_dir(enter_here)
end

SystemFS:delete_file(enter_here.."/short-log.txt")
local data_file = io.open(enter_here.."/data.txt", "w")

local function data_entry(str, ...)
    data_file:write(string.format(str.."\n", ...))
end

data_entry(os.date("%d %b %Y %X"))

local function list_mod(mod_name, mod_path, disabled, suffix)
    mod_name = mod_name or "Missing Name"
    suffix = (suffix or "")..(disabled and " <DISABLED>" or "")
    local space = string.rep(" ", 100-string.len(mod_name)-string.len(suffix or ""))
    data_entry("Name: %s"..suffix..space..'Path: %s', tostring(mod_name), tostring(mod_path))
end

function troubleshooterlog(...)
    local s = table.concat({...}, " ")
    local log_file = io.open(ModPath.."/EnterHere/short-log.txt", "a")
    if log_file then
        log_file:write(os.date("%d %b %Y %X")..": "..s.."\n")
        log_file:close()
    end
end

data_entry("::INSTALLED BLT MODS::\n")
local has_beardlib = false
for _, mod in pairs(BLT.Mods:Mods()) do
    if mod.id == "BeardLib" then
        has_beardlib = true
    end
    list_mod(mod.name, "mods/"..mod.id.."/", not mod:IsEnabled())
end

local disabled_blib_mods = {}
if has_beardlib then
    local beardlib_options = io.load_as_json("mods/saves/BeardLib_Options.txt")
    if beardlib_options and beardlib_options.DisabledMods then
        disabled_blib_mods = beardlib_options.DisabledMods
    end
end

data_entry("\n\n::INSTALLED BEARDLIB MODS::\n")
-- At the moment I am not reading the XML files as I fear it may crash the game entirely.
local mod_overrides_path = "assets/mod_overrides"
local blmods = SystemFS:list(mod_overrides_path, true)
local mod_overrides = {}
if #blmods == 0 then
    data_entry("NONE")
else
    local one = false
    for _, folder in pairs(blmods) do
        local path = mod_overrides_path.."/"..folder.."/"
        local disabled = disabled_blib_mods[path] ~= nil
        if SystemFS:exists(path.."main.xml") then --Make sure it's a BeardLib mod.
            one = true
            list_mod(folder, path, disabled)
        elseif SystemFS:exists(path.."add.xml") then
            one = true
            list_mod(folder, path, disabled, " <ADD>")
        else
            table.insert(mod_overrides, folder)
        end
    end
    if not one then
        data_entry("NONE")
    end
end

data_entry("\n\n::INSTALLED MAPS::\n")
local maps = SystemFS:list("Maps", true)
if #maps == 0 then
    data_entry("NONE")
else
    local one = false
    for _, folder in pairs(maps) do
        local path = "Maps/"..folder.."/"
        if folder ~= "backups" then
            one = true
            list_mod(folder, path, disabled_blib_mods[path] ~= nil)
        end
    end
    if not one then
        data_entry("NONE")
    end
end

data_entry("\n\n::INSTALLED OVERRIDES::\n")

if #mod_overrides == 0 then
    data_entry("NONE")
else
    for _, folder in pairs(mod_overrides) do
        list_mod(folder, mod_overrides_path.."/"..folder.."/")
    end
end

data_file:close()

orig_log = log
function log(...)
    troubleshooterlog(...)
    orig_log(...)
end