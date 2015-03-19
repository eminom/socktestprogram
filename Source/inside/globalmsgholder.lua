
GlobalMessageHolder = {}
function GlobalMessageHolder.handleWorldCmd(event, msg)
	print("#####")
	print('number is ', msg.number)
	print('name is ', msg.name)
	print("$$")
end

function GlobalMessageHolder.init()
	EventDispatcher.addHandler(ModelEvent.WorldCmd, GlobalMessageHolder.handleWorldCmd)
end
