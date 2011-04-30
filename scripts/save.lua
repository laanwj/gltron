-- dump global environment

function savevar (n,v)
 if v == nil then return end
 if type(v)=="userdata" or type(v)=="function" then return end
 -- if type(v)=="userdata" or type(v)=="function" then io.write("\t-- ") end
 -- don't print lua constants
 if string.sub(n, 1, 1) == "_" then return end
 io.write("settings.", n," = ")
 if type(v) == "string" then io.write(string.format("%q",v))
 elseif type(v) == "table" then
   if v.__visited__ ~= nil then
     io.write(v.__visited__)
   else
    io.write("{ }\n")
    v.__visited__ = n
    for r,f in v do
      if r ~= "__visited__" then
        if type(r) == 'string' then
          savevar(n.."."..r,f)
	else
          savevar(n.."["..r.."]",f)
	end
      end
    end
   end
 else io.write(tostring(v)) end
 io.write("\n")
end

function save ()
  table.foreach(settings,savevar)
end
