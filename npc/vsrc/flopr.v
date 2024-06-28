module flopr #(parameter WIDTH = 8)(
   input clk, reset,
   input [WIDTH-1:0] d,
   output reg [WIDTH-1:0] q
);



   always @(posedge clk or posedge reset) begin
      if (reset) begin
         q <= #1 0;  
      end else begin
         q <= #1 d;  
      end
   end

endmodule