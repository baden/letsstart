--
function obj(v)
	if v.name == nil then
		error ("No object name in constructor.");
	end
	if v.hit == nil then
		v.hit = function(self)
			print("--- Object `" .. v.name .. "` is hit `" .. self.name .. "`");
			return 0;
		end
	end
	print("Created object `" .. v.name .. "`");
	return v
end
