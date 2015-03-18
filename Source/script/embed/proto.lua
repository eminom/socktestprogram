
require "embed.protobuf"

-- 处理协议相关的逻辑
Proto = {}

-- 得到枚举
function Proto.toID(name)
	-- ID 反查表
	if Proto.nameToIDTable == nil then
		Proto.nameToIDTable = {}
	end
	local id = Proto.nameToIDTable[name];
	if id ~= nil then
		return id;
	end
	
	id = protobuf.get_enum_value(name..".Type", "ID");
	Proto.nameToIDTable[name] = id;
	
	return id;
end

-- 初始化
function Proto.init()
	-- 注册pb文件
	if not Proto.isRegister then
		Proto.isRegister = true

		local pbs = {
			"../proto/msg.pb",
			"../proto/worldcmd.pb",
		}
		print "Loading protos  ..."

		for i=1, #pbs do
			print("Loading of ", pbs[i])
			protobuf.register(mm.loadBinaryFile(pbs[i]))
			print("Loading finished")
		end
		print "Loading done"
	end
end

-- 打包命令
function Proto.packCmd(name, cmd)
	return protobuf.encode(name,cmd);
end

-- 针对pbc的optional 值做的特殊命令，忽略optional的默认值
function Proto.optionalNoDefault(msg,field)
	local originalMt = getmetatable(msg);
	setmetatable(msg,nil);
	local result = msg[field];
	setmetatable(msg,originalMt);
	return result;
end

print "proto inited"