
require "inside.protobuf"
require "inside.model.netevent.events"

Proto = {
	nameToIDTable = {}
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

function Proto.init()
	if not Proto.isRegister then
		Proto.isRegister = true
		local pbs = {
			"data.pb",
			"exceptiontype.pb",
			"cs_dir.pb",
			"cs_world.pb",
		}
		for i=1, #pbs do
			-- print ("Loading "..pbs[i])
			protobuf.register(mm.LoadBinaryFile("../proto/"..pbs[i]))
		end
		Proto.test()
		print("Proto.init done")
	else
		print("Proto.init already done")
	end
end

function Proto.test()
	local namesForTest = {
		--Dir server
		"RequestWorldList",
		"RequestRegisterUser",

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
		print("Protocol<"..v .. ">.Type(ID) is " .. tostring(id))
	end
end

function Proto.packCmd(name, cmd)
	return protobuf.encode(name, cmd)
end

-- 针对pbc的optional 值做的特殊命令，忽略optional的默认值
function Proto.optionalNoDefault(msg, field)
	local originalMt = getmetatable(msg)
	setmetatable(msg,nil)
	local result = msg[field]
	setmetatable(msg,originalMt)
	return result
end