
require "inside.proto"
require "inside.dispatcher"
require "inside.model.model"
require "inside.test"
require "inside.globalmsgholder"
require "inside.pumper"

-- local function send_rand_info()
-- 	local randStr = {
-- 		'Stan', 'Kyle', 'Diablo', 'Zeratul', 'Diana', 'Randy Marsh'
-- 	}
-- 	local pack = {
-- 		number = 908070605040302010,
-- 		name = randStr[math.random(#randStr)]
-- 	}
-- 	local buffer = protobuf.encode("WorldListCommand", pack)
-- 	print('Sending Buffer['..tostring(buffer:len())..']..')
-- 	mm.SendBuffer(buffer)
-- end

function GetHostName()
	return "192.168.2.2"
end

function GetHostPort()
	return "11000"
end

local clientState = Model.ClientState or error("Not initialized for Model.ClientState ?")

local ConnectorHandler = {}

function ConnectorHandler:new()
	local o = {}
	setmetatable(o, self)
	self.__index = self
	return o
end

function ConnectorHandler:init()
	self.onWorldListNotified = function(event, decoded)
		print("world list count:" .. tostring(#decoded.world_list))
		local chosen 
		for i=1,#decoded.world_list do
			local one = decoded_world_list[i]
			if not chosen then
				chosen = one
				break
			end
		end
		if chosen then
			-- print("Now switching to world ["..tostring(chosen.id).."]")
		end
	end
	EventDispatcher.addHandler(ModelEvent.WorldListNotify, self.onWorldListNotified)
end

function ConnectorHandler:deinit()
	EventDispatcher.removeHandler(ModelEvent.WorldListNotify, sellf.onWorldListNotified)
end

local connectorHandler = ConnectorHandler:new()

local prePass = 0
-- dt: in second(s)
function frameUpdate(dt)
	prePass = prePass + dt
	if prePass >= 3 then
		prePass = 0
		local state = clientState:GetState()
		if ClientState.Disconnected == state then
			local buffer = protobuf.encode("WorldListCommand", {})
			mm.SendBuffer(buffer, 1)  -- With Type-code = 1
		elseif ClientState.Connecting == state then

		elseif ClientState.Connected == state then

		end
	end
end

Proto.init()
GlobalMessageHolder.init()

print("INIT FINISHED")

