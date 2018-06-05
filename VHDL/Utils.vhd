library ieee;
use ieee.std_logic_1164.all;
use work.TypesPackages.all;
use work.ConstantsPackages.all;

package Utils is
  
  procedure padding(signal inp : in std_logic_vector;
                    signal outp : out std_logic_vector);
  procedure parsing(signal inp : in std_logic_vector;
                    signal outp : out blocks);
  procedure expansion(signal inp : in std_logic_vector);
  procedure permutation(signal inp : in std_logic_vector;
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
  
  procedure multiply(signal inp : in std_logic_vector;
                      signal int : in INTEGER;
                    signal outp : out std_logic_vector) is
    variable res : std_logic_vector;
    begin
      res := inp;
      multi : for i in 0 to (int - 1) loop
        res : inp + res;
      end loop;         
      outp <= res;           
  end procedure;
  
  procedure permutation(signal inp : in std_logic_vector(31 downto 0);
                    signal outp : out std_logic_vector(31 downto 0)) is 
    begin
      outp(31 downto 24) <= inp (0 to 7);
      outp(23 downto 16) <= inp (15 downto 8);
      outp(15 downto 0) <= inp (15 to 31);
  end procedure;
  
  procedure parsing(signal inp : in std_logic_vector;
                    signal outp : out blocks) is
    variable numberOfBlocks : INTEGER;
    variable index : INTEGER;
    variable temp : std_logic_vector(511 downto 0);
      begin
       numberOfBlocks := inp'length / 512;
       index := 0;
       padd : for i in 1 to (numberOfBlocks + 1) loop
         temp := inp( (i * 512) - 1 downto (i - 1) * 512);
         outp(index) := temp;
         temp := temp + 1;
       end loop;
  end procedure;
  
end package body;