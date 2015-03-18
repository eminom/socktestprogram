


require "embed.embed"
require "embed.proto"

Proto.init()

function decoderX(buffer)
	-- print(buffer)
	-- if not buffer then
	-- 	print("No buffer")
	-- 	return
	-- end
	local decode = protobuf.decode("WorldListCommand", buffer)
	print('num is ', decode.num)
	print('name is ', decode.name)
	print(decode.name:len())
end

function encoderX()
	local randStr = {
		'Stan', 'Kyle', 'Diablo', 'Zeratul', 'Diana', 'Randy Marsh'
	}
	local pack = {
		num = 10,
		name = randStr[math.random(#randStr)]
	}
	local buffer = protobuf.encode("WorldListCommand", pack)
	print('length of buffer is [', buffer:len(), ']')
	mm.sendBuffer(buffer)
	-- print("sending done")
end

print("INIT FINISHED")
