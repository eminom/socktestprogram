
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
	return "8000"
end

local clientState = Model.ClientState or error("Not initialized for Model.ClientState ?")

local prePass = 0
-- dt: in second(s)
function frameUpdate(dt)
	prePass = prePass + dt
	if prePass >= 3 then
		prePass = 0
		-- send_rand_info()

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

