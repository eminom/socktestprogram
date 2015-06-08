
local ReadOnly = require "app.libs.ReadOnly"
local ModelEvent = {}

ModelEvent.DisconnectedFromServer = "Model.Net.DisconnectedFromServer"
ModelEvent.DirectoryConnected = "Model.Net.DirectoryConnected"
ModelEvent.WorldConnected = "Model.Net.WorldConnected"

-- The Directory
ModelEvent.WorldListNotify = "Model.Net.WorldListNotify"
ModelEvent.DirectoryRegisterUserNotify = "Model.Net.DirectoryRegisterUserNotify"

-- World
ModelEvent.LoginNotify = "Model.Net.WorldLoginNotify"
ModelEvent.CreatePlayerNotify = "Model.Net.CreatePlayerNotify"

-------- Wrapp it again.>>>
ModelEvent = ReadOnly(ModelEvent)

return ModelEvent
