a = {x = 100, y = 200, z = 300}

function Lua_Test(lpDoc)
   a, b = Lua_Test2();
   if (a.x == b.x) then
   	Doc_MessageBox(lpDoc, b.y.."���"..a.z)
   else
   	Doc_MessageBox(lpDoc, "�����")
   end
end

function Lua_Test2()
   b = a
   b.y = 9990
   b = a
   return a, b;
end
