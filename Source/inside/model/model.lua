
Model = {}
ModelEvent = ModelEvent or {}

require "inside.model.netevent.events"

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

Model.UserName = "eminem1"
Model.Password = "123123"

--------------- Made to be global -----------------------
Model.ClientState = ClientState:new()



