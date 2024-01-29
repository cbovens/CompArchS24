module regfile (input logic         clk, 
		input logic 	    we3, //write enable, will depend on clk
		input logic [4:0]   ra1, ra2, wa3, //2 5-bit addresses
		input logic [31:0]  wd3, //1 32-bit  
		output logic [31:0] rd1, rd2); //2 32-bit register 
   
   logic [31:0] 		    rf[31:0]; //32 (right) 32-bit (left) registers
   
   always_ff @ (posedge clk)
      begin
         if(we3 == 1) rf[wa3] <= wd3; //write wd3 to register @ address wa3
         rf[5'b00000] = 32'h00000000; //Resister 0 always set to 0, can't be written to
      end
   always_comb   
      begin
         rd1 <= rf[ra1];
         rd2 <= rf[ra2];
      end
   
      
   // three ported register file
   // read two ports combinationally
   // write third port on rising edge of clock
   // register 0 hardwired to 0
   
   
endmodule // regfile