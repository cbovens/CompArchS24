module stimulus ();

   logic  clk;
   integer handle3;
   integer desc3;
   
   
	 logic 	    we3; //write enable, will depend on clk
	 logic [4:0]   ra1, ra2, wa3; //2 5-bit inputs
	 logic [31:0]  wd3; //1 32-bit input
	 logic [31:0] rd1, rd2;

   // Instantiate DUT
   regfile dut (clk, we3, ra1, ra2, wa3, wd3, rd1, rd2);

   // Setup the clock to toggle every 1 time units 
   initial 
     begin	
	clk = 1'b1;
	forever #5 clk = ~clk;
     end

   initial
     begin
	// Gives output file name
	handle3 = $fopen("regtest.out");
	// Tells when to finish simulation
	#500 $finish;		
     end

   always 
     begin
	desc3 = handle3;
	#5 $fdisplay(desc3, "%b %h %h %h || %h %h", we3, ra1, ra2, wd3, rd1, rd2);
     end   
   
   initial 
     begin
  #5 we3 = 1'b1;
  #0 wd3 = 32'hAAAAEEEE; //arbitrary data
  #5 wd3 = 32'hEEEEAAAA;
  //synchronous write test
  #5 ra1 = 5'h1A;
  #0 wd3 = 32'hAAAAEEEE;
  #5 ra2 = 5'h1B;
  #5 wa3 = 5'h1A;
  #5 wa3 = 5'h1B;

  //attempt to write without we3 enabled rd's should reflect no changes!
  #5 we3 = 1'b0;
  #5 wd3 = 32'h03030309;
     end

endmodule // reg_tb