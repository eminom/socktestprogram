
local Proto = require "app.Proto"
local Model = require "app.model.Model"
local ModelEvent = require "app.model.NetEvent.Events"

local NetworkCmd = class("Network")
NetworkCmd.Print = print

-- local function send_rand_info()
-- 	local randStr = {
-- 		'Stan', 'Kyle', 'Diablo', 'Zeratul', 'Diana', 'Randy Marsh'
-- 	}
-- 	local pack = {
-- 		number = 908070605040302010,
-- 		name = randStr[math.random(#randStr)]
-- 	}
-- 	local buffer = protobuf.encode("WorldListCommand", pack)
-- 	print('Sending Buffer['..tostring(buffer:len())..']..')
-- 	mm.SendBuffer(buffer)
-- end

function NetworkCmd.Send(pn, obj)
	assert( type(pn)=="string", "Protocol must be string")
	local id = Proto.toID(pn)
	assert( type(id)=="number" and id > 0, "Must be greater than zero")
	assert( type(obj)=="table", "Obj must be table")
	local buffer = Proto.packCmd(pn, obj)
	mm.SendBuffer(buffer, id)
	NetworkCmd.Print(pn .. " IS POSTED" .. " : ID = "..Proto.toID(pn).."")
end

-- Protocols processing for Directory
function NetworkCmd.RequestWorldList()
	NetworkCmd.Send("RequestWorldList", {})
end

function NetworkCmd.RegisterUser(account, passwd)
	NetworkCmd.Send("RequestUserRegister", {
			account = account,
			password = passwd 
	})
	-- NetworkCmd.Print("Registering user on directory server $$$$$$$$$$$$$$$$$$")
end

function NetworkCmd.RequestLogin()
	NetworkCmd.Send("RequestLogin", {
		device_id = Model.DeviceID(),
		-- map_id    = 13
		account  = Model.UserName,
		password = Model.Password,
		is_anonymous = Model.IsAnonymous(),
	})
end

function NetworkCmd.CreatePlayer(is_anonymous, device_id, account)
	NetworkCmd.Send("RequestCreatePlayer", {
		is_anonymous = is_anonymous,
		device_id    = device_id,
		account      = account
	})
end

function NetworkCmd.ConnectToDirectory()
	mm.ConnectServer(Model.DirectoryServer.Host, Model.DirectoryServer.Port, ModelEvent.DirectoryConnected, "directory")
	NetworkCmd.Print("<Connecting to directory server>")
end

function NetworkCmd.ConnectToWorld(host, port)
	mm.ConnectServer(host, port, ModelEvent.WorldConnected, "world")
	NetworkCmd.Print("<Connecting to world server>")
end

return NetworkCmd

