module stimulus ();

   logic  clock;
   logic  In;
   logic  reset_b;
   
   logic  Out;
   
   integer handle3;
   integer desc3;
   
   // Instantiate DUT
   FSM dut (Out, reset_b, clock, In);

   // Setup the clock to toggle every 1 time units 
   initial 
     begin	
	clock = 1'b1;
	forever #5 clock = ~clock;
     end

   initial
     begin
	// Gives output file name
	handle3 = $fopen("test.out");
	// Tells when to finish simulation
	#500 $finish;		
     end

   always 
     begin
	desc3 = handle3;
	#5 $fdisplay(desc3, "%b %b || %b", 
		     reset_b, In, Out);
     end   
   
   initial 
     begin      
	#0  reset_b = 1'b1; //Test Reset Switches
	//#10 reset_b = 1'b1;	
	#10  In = 1'b0; //S0 -> S0, Output: 1
     #10  In = 1'b1; //S0 -> S2, Output: 0
     #10  In = 1'b1; //S2 -> S2, Output: 1
     #10  In = 1'b0; //S2 -> S1, Output: 0
     #10  In = 1'b0; //S1 -> S0, Output: 1
     
     //Back to S0, loop around back to S2 to show input at S1 doesn't matter

     #10  In = 1'b1; //S0 -> S2, Output: 0 
     #10  In = 1'b0; //S2 -> S1, Output: 0
     #10  In = 1'b1; //S1 -> S0, Output: 1
     end

endmodule // FSM_tb

