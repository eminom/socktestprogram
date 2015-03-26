


require "inside.model.netevent.events"

local function PrintSumMsg(title, cb)
	print("#######################===G===########################")
	print("##################   "..title.."  ##############")
	cb()
	print("#############################################")
	print("")
end

GlobalMessageHolder = {}
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
		print("exception = " .. tostring(decoded.exception))
	end)
end

function GlobalMessageHolder.init()
	-- print("WorldListNotify: <<< ", ModelEvent.WorldListNotify)
	EventDispatcher.addHandler(ModelEvent.WorldListNotify, GlobalMessageHolder.handleWorldListNotify)
	EventDispatcher.addHandler(ModelEvent.LoginNotify, GlobalMessageHolder.handleLoginNotify)
	EventDispatcher.addHandler(ModelEvent.CreatePlayerNotify, GlobalMessageHolder.handleCreatePlayerNotify)
end
