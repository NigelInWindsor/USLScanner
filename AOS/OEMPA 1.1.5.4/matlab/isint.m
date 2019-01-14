% ISINT(A) %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%  Returns 1 for entries in matrix which are integers, and 0 for entries
%  which are non-integer.
% Alex Nelson 12/18/03
function a = isint(A)
  a = A==fix(A);