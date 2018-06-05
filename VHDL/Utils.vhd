library ieee;
use ieee.std_logic_1164.all;
use work.TypesPackages.all;
use work.ConstantsPackages.all;

package Utils is
  
  procedure padding(signal inp : in std_logic_vector;
                    signal outp : out std_logic_vector);
  procedure parsing(signal inp : in std_logic_vector;
                    signal outp : out std_logic_vector);
  procedure expansion(signal inp : in std_logic_vector);
  procedure reverse(signal inp : in std_logic_vector;
                    signal outp : out std_logic_vector);
  procedure multiply(signal inp : in std_logic_vector;
                      signal int : in INTEGER;
                    signal outp : out std_logic_vector);
  procedure sigma0(signal x : in std_logic_vector;
                    signal outp : out std_logic_vector);
  procedure sigma1(signal x : in std_logic_vector;
                    signal outp : out std_logic_vector);
  procedure bigSigma0(signal x : in std_logic_vector;
                    signal outp : out std_logic_vector);
  procedure bigSigma1(signal x : in std_logic_vector;
                    signal outp : out std_logic_vector);
  procedure bigSigma2(signal x : in std_logic_vector;
                    signal outp : out std_logic_vector);
  procedure ch(signal x, y, z : in std_logic_vector;
               signal outp : out std_logic_vector);
  procedure maj(signal x, y, z : in std_logic_vector;
                signal outp : out std_logic_vector);
                
end package;

package body Utils is
  
end package body;