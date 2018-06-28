library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;
use work.types.all;
use work.constants.all;
use work.Utils.all;

entity SHA256 is
--signal x : std_logic_vector (31 downto 0) := "01100001011000100110001101100100";
    port(
    clk : in std_logic;
    reset : in std_logic;
    input : in std_logic_vector (31 downto 0) := "01100001011000100110001101100100";
    difficulty : in std_logic_vector(31 downto 0) := x"5350f119";
    timestamp : in std_logic_vector(31 downto 0) := x"358b0553";
    hashPrevBlock : in std_logic_vector(255 downto 0) := x"17975b97c18ed1f7e255adf297599b55330edab87803c8170100000000000000";
    version : in std_logic_vector(31 downto 0) := x"02000000";
    target : in std_logic_vector(255 downto 0) := x"1010101010101010101010101010101010101010101010101010101010101010";
    output : out std_logic_vector (255 downto 0));
end SHA256;

architecture Behavioral of SHA256 is

    signal currentState, nextState : state_type := START;
    signal result : std_logic_vector (255 downto 0);
    signal blockHeader : std_logic_vector(639 downto 0);
    signal merkelRoot : std_logic_vector(255 downto 0);
    signal hash,tempHash : std_logic_vector(255 downto 0);
    signal whileInput : std_logic_vector(671 downto 0);
    signal nonce : std_logic_vector(31 downto 0) := (others => '0');
    signal tempNonce : std_logic_vector(31 downto 0);
    signal one : std_logic_vector(31 downto 0) := "00000000000000000000000000000001";
    begin
        process(clk)
          begin
            if rising_edge (clk) then
              if reset = '1' then
                currentState <= START;
              else
                currentState <= nextState;
              end if;
            end if;
          end process;

          process(currentState, input, nonce, difficulty, timestamp, hashPrevBlock, version)
            begin
              case currentState is
                when START =>
                  hash<= x"ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff";
                  merkelRoot <= sha(input);
                  --nextState <= CREATE_BLOCK;
                when CREATE_BLOCK =>
                  blockHeader <= version & hashPrevBlock & merkelRoot & timestamp & difficulty & nonce;
                  nextState <= CHECKHER;
                when CHECKHER =>
                  if (hash > target) then
                    whileInput <= nonce & blockHeader;
                    nextState <= FIRST_CALC;
                  else
                    nextState <= FINAL;
                  end if;
                when FIRST_CALC =>
                   tempHash <= sha(whileInput);
                   tempNonce <= nonce;
                   nextState <= SECOND_CALC;
                when SECOND_CALC =>
                  hash <= sha(tempHash);
                  nonce <= tempNonce + one;
                  nextState <= CHECKHER;
                when FINAL =>
                  output <= hash;
                when others =>
                  nextState <= START;
                end case;
            end process;
end architecture;
