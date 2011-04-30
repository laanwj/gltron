function color_interpolate(t, c1, c2)
	local color = { }
	color.r = (1 - t) * c1.r + t * c2.r
	color.g = (1 - t) * c1.g + t * c2.g
	color.b = (1 - t) * c1.b + t * c2.b
	return color
end

function drawCircleSegment(circle, range)
	c_pushMatrix()
	c_translate(circle.center.x, circle.center.y, 0)
	c_drawCircle(range.arc_start.angle, range.arc_end.angle, 
							 20, circle.inner_radius, circle.radius,
							 range.arc_start.color, range.arc_end.color, 
							 range.arc_start.color, range.arc_end.color)
	c_popMatrix()
end

function drawRectangle(rect, range)
	 c_pushMatrix()
	 -- convert range to pixels
	 local left = rect.right + 
			range.range_end.width * (rect.left - rect.right)
	 local right = rect.right + 
			range.range_start.width * (rect.left - rect.right)

	 c_translate(left, rect.top, 0)
	 c_drawRectangle(right - left, rect.bottom - rect.top,
							range.range_end.color, range.range_end.color,
							range.range_start.color, range.range_start.color)
	c_popMatrix()
end

-- flags for text

FontFormat = {
	AlignTop				=	0, -- 0x00000000,
	AlignVCenter			=	1, -- 0x00000001,
	AlignBottom				=	2, -- 0x00000002,
	AlignLeft				=	4, -- 0x00000004,
	AlignCenter				=   8, -- 0x00000008,
	AlignRight				=	16, -- 0x00000010,
	ScaleFitHorizontally	=	32, -- 0x00000020,
	ScaleFitVertically		=	64, -- 0x00000040,
	WordWrap				=	128  -- 0x00000080
}

function drawSpeedDigital(speed, rect, color) -- between 0 and N
	c_pushMatrix()
	c_translate(rect.x, rect.y, 0)
	if(speed > 0) then
		c_color(color.r, color.g, color.b, color.a)
 		c_drawTextFitIntoRect(
 			string.format("%.0f", speed * 10), -- TODO: restring.format speed into string
 			rect.w, rect.h,
 			FontFormat.ScaleFitHorizontally + 
 			FontFormat.ScaleFitVertically)
 	end
	c_popMatrix()
end

function drawSpeedAnalog(speed, circle, angles)
	-- speed: between 0 and 1
	-- circle center in pixel coordinates
	-- circle inner and outer radius in pixels
	-- io.write("speed: ", speed, "\n")
	-- for each area, draw a circle arc, with optional color
	-- interpolation through the arc
	for index, range in angles do
		if(speed > range.arc_start.speed) then
			if(speed > range.arc_end.speed) then
				drawCircleSegment(circle, range)
			else
				local t = (speed - range.arc_start.speed) / 
						(range.arc_end.speed - range.arc_start.speed)
				local r = {
					arc_start = {
						speed = range.arc_start.speed,
						angle = range.arc_start.angle,
						color = range.arc_start.color
					},
					arc_end = {
						speed = range.arc_end.speed
					}
				}
				r.arc_end.angle = 
						(1 - t) * range.arc_start.angle + 
					 t * range.arc_end.angle
				r.arc_end.color = 
						color_interpolate(t, range.arc_start.color, range.arc_end.color)
				drawCircleSegment(circle, r)
			end
		end
	end
end

function drawBar(charge, rect, ranges) -- between 0 and 1
	-- io.write("charge: ", charge, "\n")
	-- io.write(string.format("rect: %.2f, %.2f, %.2f, %.2f\n", rect.top, rect.left, rect.right, rect.bottom))
	-- Rectangle in pixel coordinates
	-- for each area, draw a rectangle, with optional color
	-- interpolation through the arc
	for index, range in ranges do
		-- io.write(string.format("range: %.2f, %.2f %.2f, %.2f\n", range.range_start.charge, range.range_end.charge, range.range_start.width, range.range_end.width))
		if(charge > range.range_start.charge) then
			if(charge > range.range_end.charge) then
				local r = range
				drawRectangle(rect, r)
			else
				local t = (charge - range.range_start.charge) / 
					(range.range_end.charge - range.range_start.charge)
				local r = {
					range_start = {
						charge = range.range_start.charge,
						color = range.range_start.color,
						width = range.range_start.width
					},
					range_end = {
						charge = range.range_end.charge
					}
				}
				r.range_end.width = 
					(1 - t) * range.range_start.width + 
					t * range.range_end.width
				r.range_end.color = 
					color_interpolate(t, range.range_start.color, range.range_end.color)
				drawRectangle(rect, r)
			end
		end
	end
end
