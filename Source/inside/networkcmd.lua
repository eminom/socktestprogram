
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


function NetworkCmd.RequestWorldList()
	local buffer = protobuf.encode("WorldListCommand", {})
	mm.SendBuffer(buffer, 1)  -- With Type-code = 1
	NetworkCmd.Print("Request world list $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$")
end

function NetworkCmd.RequestLogin()
	local buffer = protobuf.encode("LoginCommand_anonymous", {
		device_id = "iPhone 5s",
		map_id    = 13
	})
	mm.SendBuffer(buffer, 20001)
	NetworkCmd.Print("Request login  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$...")
end

function NetworkCmd.ConnectToDirectory()
	mm.ConnectServer("192.168.2.2", "11000", ModelEvent.DirectoryConnected)
	NetworkCmd.Print("Connect to directory server $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$")
end

function NetworkCmd.ConnectToWorld(host, port)
	mm.ConnectServer(host, port, ModelEvent.WorldConnected)
	NetworkCmd.Print("Connect to world server $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$")
end


function NetworkCmd.RegisterUser(account, passwd)
	local buffer = protobuf.encode("RegisterUserCommand", {
			account = account,
			password = passwd 
	})
	mm.SendBuffer(buffer, 3)
	NetworkCmd.Print("Registering user on directory server $$$$$$$$$$$$$$$$$$")
end