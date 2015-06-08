
require "app.libs.protobuf"

local DebugLog = require "app.libs.DebugLog"
local ReadOnly = require "app.libs.ReadOnly"
local Proto = {
	nameToIDTable = {},
	isRegister = false,  -- Flag for "not init yet"
}

local _pbs = {
	"data.pb",
	"exceptiontype.pb",
	"cs_dir.pb",
	"cs_world.pb",
}

function Proto.toID(name)
	-- print(".toID with ", name)
	local id = Proto.nameToIDTable[name]
	if not id then
		id = protobuf.get_enum_value(name..".Type", "ID")
		Proto.nameToIDTable[name] = id
	end
	return id
end

function Proto.__init()
	if not Proto.isRegister then
		Proto.isRegister = true
		for i=1, #_pbs do
			DebugLog("Loading ".._pbs[i])
			protobuf.register(mm.LoadBinaryFile("../proto/".._pbs[i]))
		end
		Proto.test()
		print("Proto.init done")
	else
		print("Proto.init already done")
	end
end

--------------
--------------
--- The test are performed by loading the known ones.
function Proto.test()
	local namesForTest = {
		--Dir server
		"RequestWorldList",
		"RequestUserRegister",
		"ResponseWorldList",
		"ResponseUserRegister",

		-- World server
		"RequestLogin",
		"RequestCreatePlayer",
		"ResponseLogin",
		"ResponseCreatePlayer"
	}
	for _, v in ipairs(namesForTest) do
		local id = Proto.toID(v)
		assert(id > 0, "Message type id must be greater than zero")
		DebugLog("Protocol<"..v .. ">.Type(ID) is " .. tostring(id))
	end
end

Proto.packCmd = protobuf.encode

-- 针对pbc的optional 值做的特殊命令，忽略optional的默认值
function Proto.optionalNoDefault(msg, field)
	local originalMt = getmetatable(msg)
	setmetatable(msg, nil)
	local result = msg[field]
	setmetatable(msg, originalMt)
	return result
end

Proto.__init()

--------------
--- No global defined. Ever since. 
return Proto