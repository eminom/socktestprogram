
----------------------------
------- Coded by Eminem ----
------- Art by ANG Wang ----
----------------------------
------- FUNS STUDIO     ----
----------------------------
----------------------------
----------------------------

local function _CreateValueMaster(m, proto)
    assert( type(proto) == "table", "Must be table")
    assert( type(proto.setValue) == "nil", "Must be empty")
    proto.setValue = function(self, val)
        self._value = val
    end
    setmetatable(proto, {
        __index  = function(self, k)
            if k:sub(1,2) == "is" and k:len() > 2
                and m[k:sub(3)] ~= nil then
                local field = k:sub(3)
                local sf = function(me)
                    --print("Testing for "..field)
                    return rawget(me, "_value") == m[field]
                end
                rawset(self, k, sf)
                return sf
            elseif k:sub(1,3) == "set" and k:len() > 3 
                and m[k:sub(4)] ~= nil then
                local field = k:sub(4)
                local sf = function(me)
                    return rawset(me, "_value", m[field])
                end
                rawset(self, k, sf)
                return sf
            else
                error("Cannot access to "..k)
          end
        end,
    __newindex = function(t, k, v)
            error("Write only table:writing to "..k.." with value:"..tostring(v))
        end
    })
    return proto
end

return _CreateValueMaster