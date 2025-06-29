library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;

entity maxadc is

    port(clk,datain,reset  :in std_logic;

        dataout1,dataout2  :out signed(15 downto 4));
end maxadc;

architecture behavioral of maxadc is


 
begin
    sclk<=sck;
    csel<=selec;
    busy<=bs;

        process(clk,reset)    ---SCLK signal counter---

           begin
              if reset= '0' then
                 clkcount<=0;
           else
                 if rising_edge(clk) then
                    if clkcount<49 then
                        clkcount<= clkcount+1;
                    else
                        clkcount<=0;
                    end if;
                else
                      clkcount<=clkcount;
                end if;

            end if;
       end process;

       process(clk,clkcount)
     
         begin
              if rising_edge(clk) then
               if(clkcount>=0 and clkcount<=19) then
                   sck<='1';
             elsif(clkcount>20 and clkcount<=49) then
                   sck<='0';
            end if;
           end if;
      end process;

    
      
      process(sck,datacount)
          
           begin
               if falling_edge(sck) then
                    if  (datacount<=16 and datacount>15) then
                        bs<='0';
                        cs<='0';  
                        bs<='0';
                 elsif (datacount<=1 and sck='1') then
                       cs<='0';
                 end if;
              end if;
     end process;

      process(reset,sck,selec)
   
           begin

            if reset<='1' then
 
                    if falling_edge(sck) then
             
                      datacount<=datacount-1;
                        if datacount=1 then
                        datacount<=16;

                         dataout2<=datareg;
                      end if;                            
                   end if;
                     else 
                      datacount<=datacount;

                  end if;

         else
               datacount<=16;

 
       end if;            
     end process; 

     
   process(sck,reset,chs,datacount)
      
      begin
           if reset='1' then
               if rising_edge(sck) then
                  if chs="00" then
                       selec<='0';
                 elsif chs="01" then
                       selec<='1';
                    end if;
                 else
                       selec<='0';
               end if;
             end if;
           end if;
   end process;


end behavioral;
