
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

Model.UserName = "baihai"
Model.Password = "baipasswd"

Model.DirectoryServer = {
	Host = "192.168.2.12",
	Port = "11000"
}

function Model.DeviceID()
	return "X51"
end

function Model.IsAnonymous()
	return true
end

--------------- Made to be global -----------------------
Model.ClientState = ClientState:new()



