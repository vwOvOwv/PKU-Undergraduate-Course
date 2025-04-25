`timescale 1ns / 1ps

module stopwatch_ctrl(
    input logic clk_10Hz,  	//10Hz
    input logic btnC, 		//CLR Button
    input logic btnD, 		//PAUSE Button
    input logic [3:0] min1_cnt, sec10_cnt, sec1_cnt, ms100_cnt,
    output logic min1_clr, sec10_clr, sec1_clr, ms100_clr,
    output logic min1_load, sec10_load, sec1_load, ms100_load,
    output logic min1_en, sec10_en, sec1_en, ms100_en
    );
    
    assign {min1_load, sec10_load, sec1_load, ms100_load} = 4'b0000;
    logic clr;
    logic en;
    
    logic sync_clr0, sync_clr1;
    always_ff @(posedge clk_10Hz)
    begin
      sync_clr0 <= btnC;
      sync_clr1 <= sync_clr0;
    end
    assign clr = btnC | sync_clr1;
    
    logic sync_en0, sync_en1, sync_en2;
    always_ff @(posedge clk_10Hz)
    begin
      sync_en0 <= btnD;
      sync_en1 <= sync_en0;
      sync_en2 <= sync_en1;
    end
    
    always_ff @(posedge clk_10Hz)
    begin
      if (clr)
        en <= 0;
      else if (sync_en2 & ~sync_en1)
        en <= ~en;
    end
    
    //stopwatch control
    //
    // 写出下面的组合逻辑，下面所有信号名改成*_<姓名缩写>
    //
    assign ms100_en = en;
    assign sec1_en  = (ms100_cnt==9) & ms100_en;
    assign sec10_en = 
    assign min1_en  = 
    assign min10_en  = 
    
    assign ms100_clr = 
    assign sec1_clr  = 
    assign sec10_clr =
    assign min1_clr  = 

endmodule
