
require "inside.networkcmd"

local ConnectorHandler = {}

function ConnectorHandler:new()
	local o = {}
	setmetatable(o, self)
	self.__index = self
	return o
end

function ConnectorHandler:initWorldNotify()
	self.onWorldListNotified = function(event, decoded)
		print("world list count:" .. tostring(#decoded.world_list))
		local chosen 
		for i=1,#decoded.world_list do
			local one = decoded.world_list[i]
			if not chosen then
				chosen = one
				break
			end
		end
		if chosen then
			print("Now switching to world ["..tostring(chosen.id).."]")
			NetworkCmd.ConnectToWorld(chosen.host, chosen.port)
		end
	end
	EventDispatcher.addHandler(ModelEvent.WorldListNotify, self.onWorldListNotified)
end

function ConnectorHandler:initDisconnected()
	self.onDisconnected = function(event)
		print("reconnecting to directory server ...............")
		NetworkCmd.ConnectToDirectory()
	end
	EventDispatcher.addHandler(ModelEvent.DisconnectedFromServer, self.onDisconnected)
end

function ConnectorHandler:initDirectoryConnected()
	self.onDirectoryConnected = function()
		print("Directory connected ! ##############  ########")
		NetworkCmd.RequestWorldList()
	end
	EventDispatcher.addHandler(ModelEvent.DirectoryConnected, self.onDirectoryConnected)
end

function ConnectorHandler:initWorldConnected()
	self.onWorldConnected = function()
		print("World connected !")
		NetworkCmd.RequestLogin()
	end
	EventDispatcher.addHandler(ModelEvent.WorldConnected, self.onWorldConnected)
end

function ConnectorHandler:init()
	self:initWorldNotify()
	self:initDisconnected()
	self:initDirectoryConnected()
	self:initWorldConnected()
	return self
end

function ConnectorHandler:deinit()
	EventDispatcher.removeHandler(ModelEvent.WorldListNotify, sellf.onWorldListNotified)
	EventDispatcher.removeHandler(ModelEvent.DisconnectedFromServer, self.onDisconnected)
	EventDispatcher.removeHandler(ModelEvent.DirectoryConnected, self.onDirectoryConnected)
	EventDispatcher.removeHandler(ModelEvent.WorldConnected, self.onWorldConnected)
end

return ConnectorHandler