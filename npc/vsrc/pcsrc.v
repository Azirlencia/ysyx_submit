module pcsrc(
    input equalD, branchD,
    output pcsrcD
);

    always @(*) begin
        if (equalD && branchD) begin
            pcsrcD = 1;
        end else begin
            pcsrcD = 0;
        end
    end

endmodule