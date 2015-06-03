	
require "inside.proto"
require "inside.dispatcher"
require "inside.model.model"
require "inside.test"
require "inside.globalmsgholder"
require "inside.networkcmd"
require "inside.pumper"


GlobalMessageHolder.init()
local clientState = Model.ClientState or error("Not initialized for Model.ClientState ?")
local connectorHandler = require "inside.connectorhandler":new():init()

local prePass = 0
-- dt: in second(s)
function frameUpdate(dt)
	EventDispatcher.check()
	prePass = prePass + dt
	if prePass >= 3 then
		prePass = 0
		local state = clientState:GetState()
		-- if ClientState.Disconnected == state then
		-- 	-- local buffer = protobuf.encode("WorldListCommand", {})
		-- 	-- mm.SendBuffer(buffer, 1)  -- With Type-code = 1
		-- elseif ClientState.Connecting == state then
		-- elseif ClientState.Connected == state then
		-- end
		-- local handle = {
		-- 	NetworkCmd.RequestWorldList,
		-- 	-- NetworkCmd.RequestLogin,
		-- 	function()
		-- 		NetworkCmd.RegisterUser(Model.UserName, Model.Password)
		-- 	end
		-- }
		-- local func = handle[math.random(#handle)]
		-- func()
	end
end

function startUp()
	print("disconnected from server")
	EventDispatcher.dispatch(ModelEvent.DisconnectedFromServer)
end

Proto.init()


print("INIT FINISHED\n\n\n")

