module regfile (input logic         clk, 
		input logic 	    we3, //write enable, will depend on clk
		input logic [4:0]   ra1, ra2, wa3, //2 5-bit read addresses, 1 write address
		input logic [31:0]  wd3, //1 32-bit write data 
		output logic [31:0] rd1, rd2); //2 32-bit read data
   
   logic [31:0] 		    rf[31:1]; //32 (right) 32-bit (left) registers
   
   always_ff @ (posedge clk)
      begin
         if(we3 == 1 & wa3 != 0) rf[wa3] <= wd3; //write wd3 to register @ address wa3
      end
   always_comb   
      begin    
        
         rd1 = (ra1 != 0) ? rf[ra1] : 0; //Mux to ignore port 0 & read correctly
         rd2 = (ra2 != 0) ? rf[ra2] : 0;

      end
   
      
   // three ported register file
   // read two ports combinationally
   // write third port on rising edge of clock
   // register 0 hardwired to 0
   
   
endmodule // regfile