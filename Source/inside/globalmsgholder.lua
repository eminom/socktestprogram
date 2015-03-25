

require "inside.model.netevent.events"

GlobalMessageHolder = {}
function GlobalMessageHolder.handleWorldListNotify(event, msg)
	-- print("#####")
	-- print('number is ', msg.number)
	-- print('name is ', msg.name)
	-- print("$$")
	print("")
	print("Global watch:")
	print("world list count is " .. tostring(#msg.world_list))
	for i=1, #msg.world_list do
		local one = msg.world_list[i]
		print("["..tostring(i).."]  >> ")
		print("host:" .. tostring(one.host))
		print("port:" .. tostring(one.port))
		print("id:"   .. tostring(one.id))
		print("name:" .. tostring(one.name))
	end
	print("")
end

function GlobalMessageHolder.init()
	-- print("WorldListNotify: <<< ", ModelEvent.WorldListNotify)
	EventDispatcher.addHandler(ModelEvent.WorldListNotify, GlobalMessageHolder.handleWorldListNotify)
end
