module bin2BCD(
  input logic [13:0] bin,
  output logic [3:0] bcd3,
  output logic [3:0] bcd2,
  output logic [3:0] bcd1,
  output logic [3:0] bcd0 
  ); 
  
  logic [29:0] shifter; 
  integer i;
 
  always_comb
  begin 
    shifter[13:0] = bin;
    shifter[29:14] = 0; 
    
    for (i = 0; i< 14; i = i+1) 
    begin 
      if (shifter[17:14] >= 5) 
        shifter[17:14] = shifter[17:14] + 3; 

      if (shifter[21:18] >= 5)             
        shifter[21:18] = shifter[21:18] + 3;

      if (shifter[25:22] >= 5)             
        shifter[25:22] = shifter[25:22] + 3; 

      if (shifter[29:26] >= 5)              
        shifter[29:26] = shifter[29:26] + 3; 

      shifter = shifter  << 1;    
    end  
  end

  assign  {bcd3, bcd2, bcd1, bcd0} = shifter[29:14];

endmodule
