
-- Global

local ReadOnly = require "app.libs.ReadOnly"
local EventDispatcher = {}

local handlers_ = {}
local eventCache = {}

function EventDispatcher.dispatch(event, ...)
	local chain = handlers_[event]
	if type(chain) == 'table' then
		local t = {}
		local more = {...}
		for i=1, #chain do
			table.insert(t, chain[i])
		end
		local ev = function()
			for _, f in ipairs(t) do
				f(event, unpack(more))
			end
		end
		table.insert(eventCache, ev)
	end
end

function EventDispatcher.check()
	while true do 
		local now = eventCache[1]
		if not now then
			break
		end
		now()  -- Execute it !
		table.remove(eventCache, 1)
	end
end

function EventDispatcher.addHandler(event, handler)
	-- DebugLog("Add handler for ", event)
	assert( type(handler) == 'function', "Must be function for handlers")
	assert( type(event) == 'string', 'Event must be of type string')
	handlers_[event] = handlers_[event] or {}
	local h = handlers_[event]
	h[#h+1] = handler
end

function EventDispatcher.removeHandler(event, handler)
	assert(type(handler)=="function", "Handler must be function")
	local index = 0
	local chain = handlers_[event]
	if type(chain) == 'table' then
		for i=1, #chain do
			if chain[i] == handler then
				index = i
				break
			end
		end
	end
	assert(index > 0, "Must be removed")
	table.remove(chain, index)
end

return EventDispatcher