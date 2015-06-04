

local function _ReadOnly(o)
	setmetatable(o, {
		__index = function(t, k)
			local msg = "Cannot access to :"..tostring(k)
			print(msg)
			error(msg)
		end
	})
    local ro = {}
    ro.iter = function()
        return next, o, nil
    end
    setmetatable(ro, {
        __index = o,
        __newindex = function(t, k, v)
            error("Cannot write to read-only table with "..tostring(k)..":"..tostring(v))
        end
    })
    return ro
end

return _ReadOnly