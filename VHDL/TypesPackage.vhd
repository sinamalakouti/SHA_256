library ieee;
use ieee.std_logic_1164.all;

package types is
    --type row is array (31 downto 0) of std_logic;
    type W is array(0 to 63) of std_logic_vector (31 downto 0);
    type K is array(0 to 63) of std_logic_vector (31 downto 0);
    type state_type is (START, CREATE_BLOCK, CHECKHER, FIRST_CALC, SECOND_CALC, FINAL);
    type blocks is array(integer range <>) of std_logic_vector(511 downto 0);
 end package;
