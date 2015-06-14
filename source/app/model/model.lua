
local ReadOnly = require "app.libs.ReadOnly"
local ClientState = require "app.model.ClientState"

local Model = ReadOnly{
	UserName = "baihai",
	Password = "baipasswd",
	DirectoryServer = ReadOnly{
		Host = "192.168.1.105",
		Port = "11000"
	},
	ClientState = ClientState:create(),
	DeviceID = function() return "X51" end,
	IsAnonymous = function() return true end
}

return Model



