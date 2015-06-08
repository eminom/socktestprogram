

local Proto = require "app.proto"
local EventDispatcher = require "app.Dispatcher"
local ModelEvent = require "app.model.NetEvent.Events"

-------   Message Pump
local function MakeRule(proto_name, event_name)
	assert(event_name, "Event name must not be nil")
	return {
		TypeCode = Proto.toID(proto_name) or error("No no no"),
		Proto    = proto_name,
		Event    = event_name
	}
end

local distributeRules = {
	MakeRule("ResponseWorldList", ModelEvent.WorldListNotify),
	MakeRule("ResponseUserRegister", ModelEvent.DirectoryRegisterUserNotify),
	MakeRule("ResponseLogin", ModelEvent.LoginNotify),
	MakeRule("ResponseCreatePlayer", ModelEvent.CreatePlayerNotify)
}

dd.exports.redirectNetBuffer = function(typecode, buffer)
	for _, v in ipairs(distributeRules) do
		if v.TypeCode == typecode then
			local decode = protobuf.decode(v.Proto, buffer)
			if decode then
				print("Distributing event(driven by network):"..tostring(v.Event))
				EventDispatcher.dispatch(v.Event, decode)
			else
				error("Decoding error with typecode:"..tostring(typecode).. ":"..v.Proto)
			end
		end
	end
end

dd.exports.serverConnectOn = function(serverEvent)
	EventDispatcher.dispatch(serverEvent) 
end

