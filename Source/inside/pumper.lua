

require "inside.proto"

-- Ensure the initial sequence.
Proto.init()

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
	MakeRule("WorldListNotify", ModelEvent.WorldListNotify),
	MakeRule("RegisterUserNotify", ModelEvent.DirectoryRegisterUserNotify),
	MakeRule("LoginNotify", ModelEvent.LoginNotify),
	MakeRule("CreatePlayerNotify", ModelEvent.CreatePlayerNotify)
}

function redirectNetBuffer(typecode, buffer)
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

function serverConnectOn(serverEvent)
	EventDispatcher.dispatch(serverEvent) 
end