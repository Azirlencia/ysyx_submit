module alu(
    input [31:0] A, B,
    input [3:0] F,
    input [4:0] shamt, // shamt for SLL
    output [31:0] Y,
    output Zero,
    output ltez // ltez for BLEZ
);

    // Internal signals
    wire [31:0] S, Bout;
    
    // Assign Bout based on F[3]
    assign Bout = F[3] ? ~B : B;
    
    // Calculate S for operations that need it
    assign S = A + Bout + F[3];
    
    // Combinational logic for Y based on F[2:0]
    always @(*) begin
        case (F[2:0])
            3'b000: Y = A & Bout; // AND
            3'b001: Y = A | Bout; // OR
            3'b010: Y = S;        // ADD or SUB
            3'b011: Y = S[31];    // SETHI (assuming S[31] is SETHI result)
            3'b100: Y = (B << shamt); // SLL
            default: Y = 32'bx;   // Default case, x for unknown
        endcase
    end
    
    // Assign Zero and ltez based on Y
    assign Zero = (Y == 32'b0);
    assign ltez = Zero | (Y[31] & ~Zero); // BLEZ (Less Than or Equal to Zero)

endmodule