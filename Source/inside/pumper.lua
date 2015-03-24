

-------   Message Pump

local function MakeRule(typecode, proto_name, event_name)
	assert(event_name, "Event name must not be nil")
	return {
		TypeCode = typecode,
		Proto    = proto_name,
		Event    = event_name
	}
end

local distributeRules = {
	MakeRule(2, "WorldListNotify", ModelEvent.WorldListNotify),
	MakeRule(4, "RegisterUserNotify", ModelEvent.DirectoryRegisterUserNotify)
}


function redirectNetBuffer(typecode, buffer)
	for _, v in ipairs(distributeRules) do
		if v.TypeCode == typecode then
			local decode = protobuf.decode(v.Proto, buffer)
			if decode then
				EventDispatcher.dispatch(v.Event, decode)
			else
				error("Decoding error")
			end
		end
	end
end

function serverConnectOn(serverEvent)
	EventDispatcher.dispatch(serverEvent) 
end