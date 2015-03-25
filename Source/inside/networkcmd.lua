
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
	local buffer = protobuf.encode(proto, obj)
	mm.SendBuffer(buffer, Proto.toID(proto))
	NetworkCmd.Print(proto .. " is posted")
end

function NetworkCmd.RequestWorldList()
	NetworkCmd.Send("WorldListCommand", {})
end

function NetworkCmd.RequestLogin()
	NetworkCmd.Send("LoginCommand", {
		device_id = "iPhone 5s",
		-- map_id    = 13
		account  = Model.UserName,
		password = Model.Password,
		is_anonymous = false,
	})
end

function NetworkCmd.RegisterUser(account, passwd)
	NetworkCmd.Send("RegisterUserCommand", {
			account = account,
			password = passwd 
	})
	NetworkCmd.Print("Registering user on directory server $$$$$$$$$$$$$$$$$$")
end

function NetworkCmd.ConnectToDirectory()
	mm.ConnectServer("192.168.2.23", "11000", ModelEvent.DirectoryConnected)
	NetworkCmd.Print("Connect to directory server $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$")
end

function NetworkCmd.ConnectToWorld(host, port)
	mm.ConnectServer(host, port, ModelEvent.WorldConnected)
	NetworkCmd.Print("Connect to world server $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$")
end

