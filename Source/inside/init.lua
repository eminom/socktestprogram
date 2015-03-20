
require "inside.proto"
require "inside.dispatcher"
require "inside.model.model"
require "inside.test"
require "inside.globalmsgholder"
require "inside.pumper"

local function send_rand_info()
	local randStr = {
		'Stan', 'Kyle', 'Diablo', 'Zeratul', 'Diana', 'Randy Marsh'
	}
	local pack = {
		number = 908070605040302010,
		name = randStr[math.random(#randStr)]
	}
	local buffer = protobuf.encode("WorldListCommand", pack)
	print('Sending Buffer['..tostring(buffer:len())..']..')
	mm.sendBuffer(buffer)
end

local prePass = 0
-- dt: in second(s)
function frameUpdate(dt)
	prePass = prePass + dt
	if prePass >= 3 then
		prePass = 0
		send_rand_info()
	end
end

Proto.init()
GlobalMessageHolder.init()

print("INIT FINISHED")

