
Model = {}
ModelEvent = {}

require "inside.model.netevent.worldcmd"

ClientState = {
	Disconnected = 1,
	Connecting   = 2,
	Connected    = 3
}

function ClientState:new()
	local o = {}
	setmetatable(o, self)
	self.__index = self
	self.state = ClientState.Disconnected
	return o
end

function ClientState:GetState()
	return self.state
end


--------------- Made to be global -----------------------
Model.ClientState = ClientState:new()



