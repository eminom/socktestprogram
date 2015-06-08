


local ModelEvent = require "app.model.NetEvent.Events"
local EventDispatcher = require "app.Dispatcher"

local function PrintSumMsg(title, cb)
	print("#######################===G===########################")
	print("##################   "..title.."  ##############")
	cb()
	print("#############################################")
	print("")
end

local GlobalMessageHolder = class("GlobalMessageHolder")
function GlobalMessageHolder.handleWorldListNotify(event, msg)
	PrintSumMsg( "WorldListNotify", function()
		print("world list count is " .. tostring(#msg.world_list))
		for i=1, #msg.world_list do
			local one = msg.world_list[i]
			print("["..tostring(i).."]  >> ")
			print("host:" .. tostring(one.host))
			print("port:" .. tostring(one.port))
			print("id:"   .. tostring(one.id))
			print("name:" .. tostring(one.name))
		end
	end)
end

function GlobalMessageHolder.handleLoginNotify(event, msg)
	PrintSumMsg("LoginNotify", function()
		print("exception :" .. tostring(msg.exception))
		print("token: ".. tostring(msg.token))
	end)
end

function GlobalMessageHolder.handleCreatePlayerNotify(event, msg)
	PrintSumMsg("CreatePlayer", function()
		print("exception = " .. tostring(msg.exception))
	end)
end

function GlobalMessageHolder.handleUserRegister(event, msg)
	PrintSumMsg("UserRegister", function()
		print("exception = " .. tostring(msg.exception))
	end)
end

function GlobalMessageHolder.init()
	-- print("WorldListNotify: <<< ", ModelEvent.WorldListNotify)
	EventDispatcher.addHandler(ModelEvent.WorldListNotify, GlobalMessageHolder.handleWorldListNotify)
	EventDispatcher.addHandler(ModelEvent.LoginNotify, GlobalMessageHolder.handleLoginNotify)
	EventDispatcher.addHandler(ModelEvent.CreatePlayerNotify, GlobalMessageHolder.handleCreatePlayerNotify)
	EventDispatcher.addHandler(ModelEvent.DirectoryRegisterUserNotify, GlobalMessageHolder.handleUserRegister)
end

return GlobalMessageHolder