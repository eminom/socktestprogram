

-------   Message Pump

function redirectNetBuffer(typecode, buffer)
	if 2 == typecode then
		local decode = protobuf.decode("WorldListNotify", buffer)
		if decode then
			EventDispatcher.dispatch(ModelEvent.WorldListNotify, decode)
		end
	else
		-- TODO
	end
end

function serverConnectOn(serverEvent)
	EventDispatcher.dispatch(serverEvent) 
end