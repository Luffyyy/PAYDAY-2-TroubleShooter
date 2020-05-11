
local ModPath = ModPath

-- Replace the log function to accurately log things.
SystemFS:delete_file(ModPath.."/EnterHere/log.txt")
local data_file = io.open(ModPath.."/EnterHere/data.txt", "w")

local function data_entry(str, ...)
    data_file:write(string.format(str.."\n", ...))
end

function troubleshooterlog(...)
    local s = table.concat({...}, " ")
    local log_file = io.open(ModPath.."/EnterHere/log.txt", "a")
    log_file:write(os.date("%c")..": "..s.."\n")
    log_file:close()
end

data_entry("::INSTALLED BLT MODS::\n")
for _, mod in pairs(BLT.Mods:Mods()) do
    local disabled = not mod:IsEnabled()
    local space = string.rep(" ", 100-string.len(mod.id)-(disabled and 4 or 0))
    data_entry('Name: %s'..(disabled and ' [x]' or '')..space..'Path: "mods/%s"', tostring(mod.id), tostring(mod.name))
end

data_entry("\n\n::INSTALLED BEARDLIB MODS::\n")
local mod_overrides_path = "assets/mod_overrides"
local blmods = SystemFS:list(mod_overrides_path, true)
local mod_overrides = {}
if #blmods == 0 then
    data_entry("NONE")
else
    local one = false
    for _, folder in pairs(blmods) do
        local path = mod_overrides_path.."/"..folder
        local space = string.rep(" ", 100-string.len(folder))
        if SystemFS:exists(path.."/main.xml") then --Make sure it's a BeardLib mod.
            one = true
            data_entry('Name: %s'..space..'Path: "%s"', tostring(folder), tostring(path))
        elseif SystemFS:exists(path.."/add.xml") then
            one = true
            data_entry('[ADD] Name: %s'..space..'Path: "%s"', tostring(folder), tostring(path))
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
        if folder ~= "backups" then
            local space = string.rep(" ", 100-string.len(folder))
            one = true
            data_entry('Name: %s'..space..'Path: "Maps/%s"', tostring(folder), tostring(folder))
        end
    end
    if not one then
        data_entry("NONE")
    end
end

data_entry("\n\n::INSTALLED MOD OVERRIDES::\n")

if #mod_overrides == 0 then
    data_entry("NONE")
else
    for _, folder in pairs(mod_overrides) do
        local space = string.rep(" ", 100-string.len(folder))
        data_entry('Name: %s'..space..'Path: "%s"', tostring(folder), tostring(mod_overrides_path.."/"..folder))
    end
end

data_file:close()

orig_log = log
function log(...)
    troubleshooterlog(...)
    orig_log(...)
end