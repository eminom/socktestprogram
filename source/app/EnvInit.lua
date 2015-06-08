

--[[    ]]--
-- YES. Project shall comply with current app structures

dd = dd or {}
dd.exports = dd.exports or {}

if DISABLE_GLOBAL then
	setmetatable(_G, {
		__newindex = function(t,k,v)
			print("You shall export to global with dd.exports")
			error("Cannot set global with:"..tostring(k)..":"..tostring(v))
		end,
		__index = dd.exports
	})
end





