	


-- from apps.
require "app.libs.functions"
require "app.libs.json"

require "app.Config"
require "app.EnvInit"
require "app.Pumper"

-------
local Model = require "app.model.Model"
local EventDispatcher = require "app.Dispatcher"
local GlobalMsgH = require "app.GlobalMsgHolder"
GlobalMsgH.init()

local clientState = Model.ClientState or error("Not initialized for Model.ClientState ?")
local connectorHandler = require "app.ConnectHandler":create():init()

local prePass = 0
-- dt: in second(s)
local function frameUpdate(dt)
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

local function startUp()
	print("<START>Disconnected from server")
	EventDispatcher.dispatch(ModelEvent.DisconnectedFromServer)
end

dd.exports.frameUpdate = frameUpdate
dd.exports.startUp = startUp
