--Message Pump

function redirectNetBuffer(typecode, buffer)
	if 1 == typecode or 17 == typecode then
		local decode = protobuf.decode("WorldListCommand", buffer)
		if decode then
			EventDispatcher.dispatch(ModelEvent.WorldCmd, decode)
		end
	else
		-- TODO
	end
end

