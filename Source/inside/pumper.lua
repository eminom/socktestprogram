

-------   Message Pump

function redirectNetBuffer(typecode, buffer)
	if 2 == typecode then
		local decode = protobuf.decode("WorldListNotify", buffer)
		if decode then
			EventDispatcher.dispatch(ModelEvent.WorldListNotify, decode)
		end
	elseif 4 == typecode then
		local decode = protobuf.decode("RegisterUserNotify", buffer)
		if decode then
			EventDispatcher.dispatch(ModelEvent.DirectoryRegisterUserNotify, decode)
		end
		-- TODO
	end
end

function serverConnectOn(serverEvent)
	EventDispatcher.dispatch(serverEvent) 
end