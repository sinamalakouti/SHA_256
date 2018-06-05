library ieee;
use ieee.std_logic_1164.all;

package types is
    
    type row is std_logic_vector(31 downto 0);
    type blo is std_logic_vector(511 downto 0);
    
    type W is array(0 to 63) of row;
    type K is array(0 to 63) of row; 
    type blocks is array of blo; 
  end package;