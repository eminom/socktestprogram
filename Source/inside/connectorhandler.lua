
require "inside.networkcmd"

local ConnectorHandler = {}

function ConnectorHandler:new()
	local o = {}
	setmetatable(o, self)
	self.__index = self
	return o
end

function ConnectorHandler:initWorldNotify()
	self.onWorldListNotify = function(event, decoded)
		print("ConnectorHandler: world list count:" .. tostring(#decoded.world_list))
		local chosen 
		for i=1,#decoded.world_list do
			local one = decoded.world_list[i]
			if not chosen then
				chosen = one
				break
			end
		end
		-- if chosen then
		-- 	print("Now switching to world ["..tostring(chosen.id).."]")
		-- 	NetworkCmd.ConnectToWorld(chosen.host, chosen.port)
		-- end
		self.chosenServer = chosen
		NetworkCmd.RegisterUser(Model.UserName, Model.Password)
	end
	EventDispatcher.addHandler(ModelEvent.WorldListNotify, self.onWorldListNotify)
end

function ConnectorHandler:initDisconnected()
	self.onDisconnected = function(event)
		print("Reconnecting to directory server ########################")
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
		print("World connected ! ################################")
		NetworkCmd.RequestLogin()
	end
	EventDispatcher.addHandler(ModelEvent.WorldConnected, self.onWorldConnected)
end

function ConnectorHandler:initRegisterUser()
	self.onUserRegistered = function(event, decoded)
		print("User register result:"..tostring(decoded.exception))
		-- print("Dumping>>>>>>>>>>>>>>>>", type(decoded), decoded)
		-- if type(decoded) == 'table' then
		-- 	for k, v in pairs(decoded) do
		-- 		print("Key:"..tostring(k)..",  Value:"..tostring(v))
		-- 	end
		-- end
		if self.chosenServer then
			print("There is one selective server to connect:"..tostring(self.chosenServer.host)..":"..tostring(self.chosenServer.port))
			NetworkCmd.ConnectToWorld(self.chosenServer.host, self.chosenServer.port)
		end
	end
	EventDispatcher.addHandler(ModelEvent.DirectoryRegisterUserNotify, self.onUserRegistered)
end

function ConnectorHandler:initLoginNotify()
	self.onLoginNotify = function(event, decoded)
		-- print("\n")
		print("============LoginNotify========= ")
		print("Logged with token = " .. tostring(decoded.token))
		print("Exception = "..tostring(decoded.exception))
		print("")
		NetworkCmd.CreatePlayer(true, "X51", Model.UserName)
	end
	EventDispatcher.addHandler(ModelEvent.LoginNotify, self.onLoginNotify)
end

function ConnectorHandler:initCreatePlayerNotify()
	self.onCreatePlayerNotify = function(event, decoded)
		print("===========CreatePlayer ======")
		print(" Exception = ", decoded.exception)
	end
	EventDispatcher.addHandler(ModelEvent.CreatePlayerNotify, self.onCreatePlayerNotify)
end

function ConnectorHandler:init()
	self:initWorldNotify()
	self:initDisconnected()
	self:initDirectoryConnected()
	self:initWorldConnected()
	self:initRegisterUser()
	self:initLoginNotify()
	self:initCreatePlayerNotify()
	return self
end

function ConnectorHandler:deinit()
	EventDispatcher.removeHandler(ModelEvent.WorldListNotify, sellf.onWorldListNotify)
	EventDispatcher.removeHandler(ModelEvent.DisconnectedFromServer, self.onDisconnected)
	EventDispatcher.removeHandler(ModelEvent.DirectoryConnected, self.onDirectoryConnected)
	EventDispatcher.removeHandler(ModelEvent.WorldConnected, self.onWorldConnected)
	EventDispatcher.removeHandler(ModelEvent.DirectoryRegisterUserNotify, self.onUserRegistered)
	EventDispatcher.removeHandler(ModelEvent.LoginNotify, self.onLoginNotify)
end

return ConnectorHandler