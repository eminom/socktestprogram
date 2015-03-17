


require "embed.embed"
require "embed.proto"

-- xpcall(function _func()
-- 		print "Site 100"
-- 		print "Site 200"
-- 		print "Init loaded"
-- 	end
-- 	, 
-- 	debug.traceback
-- )

-- print "after xpcall"
Proto.init()

-- function decoderXEminem()
-- 	print("This is pure test decoderX")
-- end

function decoderXEminem(buffer)
	print(buffer)
	if not buffer then
		print("No buffer")
		return
	end
	local decode = protobuf.decode("WorldListCommand", buffer)
	print('num is ', decode.num)
	print('name is ', decode.name)
	print(decode.name:len())

	-- for k,v in pairs(decode) do
	-- 	print k, v
	-- end
	print("Decode finished")
end

function encoderX()
	local pack = {
		num = 10,
		name = 'command name field'
	}
	local buffer = protobuf.encode("WorldListCommand", pack)
	print('length of buffer is [', buffer:len(), ']')

	mm.sendBuffer(buffer)
	print("sending done")
end

print("INIT FINISHED")
