
-- Global
EventDispatcher = {}

local handlers_ = {}

function EventDispatcher.dispatch(event, more)
	local chain = handlers_[event]
	if type(chain) == 'table' then
		for i=1, #chain do
			chain[i](event, more)
		end
	end
end

function EventDispatcher.addHandler(event, handler)
	-- print("Add handler for ", event)
	assert( type(event) == 'string', 'Event must be of type string')
	handlers_[event] = handlers_[event] or {}
	if type(handler) == 'function' then
		local h = handlers_[event]
		h[#h+1] = handler
	else
		error("handler must be funciton")
	end
end

function EventDispatcher.removeHandler(event, handler)
	local index = 0
	local chain = handlers_[event]
	if type(chain) == 'table' then
		for i=1,#chain do
			if chain[i] == handler then
				index = i
				break
			end
		end
		if index ~= 0 then
			table.remove(chain, index)
		end
	end
end


print "Dispatcher init"