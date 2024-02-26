module stimulus ();

   logic  clk;
   integer handle3;
   integer desc3;
   
   
	 logic 	    we3; //write enable, will depend on clk
	 logic [4:0]   ra1, ra2, wa3; //2 5-bit read addresses, 1 write address
	 logic [31:0]  wd3; //1 32-bit write data 
	 logic [31:0] rd1, rd2; //2 32-bit read data

  //test vector variables
   logic [31:0] vectornum, errors;
   logic [123:0] testvectors [10000:0];
   logic [31:0] rd1Exp, rd2Exp;
   logic[2:0] ignoreVect [3:0]; //will will in space for hex data in .tv


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
  $readmemh("regfile.tv", testvectors);
  vectornum = 0; errors = 0;
end


always @(posedge clk)
begin
  //begin bitswizzle
  #1; {ignoreVect[0], we3, wd3, ignoreVect[1], wa3, ignoreVect[2], ra1, ignoreVect[3], ra2, rd1Exp, rd2Exp} = testvectors[vectornum];
end


always @(negedge clk)
begin
    if (rd1 !== rd1Exp | rd2 !== rd2Exp) 
      begin 
      $display("Error Occured!!!");
      errors = errors + 1;
      end
      vectornum = vectornum + 1;
    if (testvectors[vectornum] === 'bx) 
      begin
      $display("%d tests completed with %d errors",
      vectornum, errors);
      $stop;
      end
end


endmodule //rf_tb