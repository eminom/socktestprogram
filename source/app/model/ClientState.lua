

local ClientState = class("ClientState")
local ValueMaster = require "app.libs.ValueMaster"
local ReadOnly = require "app.libs.ReadOnly"

local CltState = ReadOnly{
	Disconnected = 1,
	Connecting   = 2,
	Connected    = 3
}

function ClientState:ctor()
	self._state = ValueMaster(CltState, {_value = ClientState.Disconnected})
end

function ClientState:GetState()
	return self._state or error("No state ?? Client-State")
end


return ClientState