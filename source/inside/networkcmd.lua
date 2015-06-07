
NetworkCmd = {}

NetworkCmd.Print = function(...)
	print(...)
end

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

function NetworkCmd.Send(proto, obj)
	local id = Proto.toID(proto)
	assert( type(id)=="number" and id > 0, "Must be greater than zero")
	local buffer = protobuf.encode(proto, obj)
	mm.SendBuffer(buffer, id)
	NetworkCmd.Print(proto .. " IS POSTED" .. " : ID = "..Proto.toID(proto).."")
end

function NetworkCmd.RequestWorldList()
	NetworkCmd.Send("RequestWorldList", {})
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

function NetworkCmd.RegisterUser(account, passwd)
	NetworkCmd.Send("RequestUserRegister", {
			account = account,
			password = passwd 
	})
	-- NetworkCmd.Print("Registering user on directory server $$$$$$$$$$$$$$$$$$")
end

function NetworkCmd.CreatePlayer(is_anonymous, device_id, account)
	NetworkCmd.Send("CreatePlayerCommand", {
		is_anonymous = is_anonymous,
		device_id    = device_id,
		account      = account
	})
end

function NetworkCmd.ConnectToDirectory()
	mm.ConnectServer(Model.DirectoryServer.Host, Model.DirectoryServer.Port, ModelEvent.DirectoryConnected, "directory")
	NetworkCmd.Print("Connect to directory server...")
end

function NetworkCmd.ConnectToWorld(host, port)
	mm.ConnectServer(host, port, ModelEvent.WorldConnected, "world")
	NetworkCmd.Print("Connect to world server $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$")
end

