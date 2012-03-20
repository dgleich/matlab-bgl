function n=end(ipa,a,n)
% INPLACE/END Allow inline "end" indices to work.
%
% n=end(ipa,a,n) Returns the ending index along dimension a.
%
% Example:
%    ipa = inplace(cumsum(ones(5,1)));
%    ipa(3:end)

% History
% 2012-03-20: Fixed linear indexing bug from Laurens Bakker

if( 1 == n )
   n = numel(ipa.get_a());
else
   n = size(ipa.get_a(),a);
end