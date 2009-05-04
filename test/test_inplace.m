function test_inplace

%%
msgid = 'matlab_bgl:test_inplace';

try
  ipd = ipdouble(ones(5));
  ipd2 = ipdouble(ipd);
catch
  error(msgid, 'inplace failed copy constructor');
end

ipd(1,1) = 5;
if ipd2(2,2) == 5,
  error(msgid, 'inplace failed to deep copy');
end  
  
try
  ipd = ipdouble(ones(5));
  y = pi*ones(5,5);
  assign(ipd,y);
catch
  error(msgid, 'inplace failed on assign');
end

if ipd(1,1) ~= pi
  error(msgid, 'inplace failed to assign correctly');
end  
