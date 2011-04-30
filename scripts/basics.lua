function script_print(...)
   io.write("[script] ")
   for i=1,arg.n do
      io.write(arg[i])
   end
   io.write("\n")
end

