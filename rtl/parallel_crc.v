//-----------------------------------------------------
// Design Name : serial_crc_ccitt
// File Name   : serial_crc.v
// Function    : CCITT Serial CRC
// Coder       : Deepak Kumar Tala
//-----------------------------------------------------
module parallel_crc_ccitt #(
    parameter [15:0] init_value = 16'h0000)
    (
    //-----------Input Ports---------------
    input clk     ,
    input reset   ,
    input enable  ,
    input init    ,
    input [8:1] x ,
    //-----------Output Ports---------------
    output [15:0] crc_out
    );

//------------Internal Variables--------
reg   [16:1] c;
//-------------Code Start-----------------
assign crc_out = c[16:1];
// Logic to CRC Calculation
always @ (posedge clk) begin
    if (reset) begin
        c <= init_value;
    end else if (enable) begin
        if (init) begin
            c <= init_value;
        end else begin
            c[16] <= c[8] ^ c[12] ^ c[16] ^ x[4] ^ x[8];
            c[15] <= c[7] ^ c[11] ^ c[15] ^ x[3] ^ x[7];
            c[14] <= c[6] ^ c[10] ^ c[14] ^ x[2] ^ x[6];
            c[13] <= c[5] ^ c[9] ^ c[13] ^ c[16] ^ x[1] ^ x[5] ^ x[8];
            c[12] <= c[4] ^ c[15] ^ x[7];
            c[11] <= c[3] ^ c[14] ^ x[6];
            c[10] <= c[2] ^ c[13] ^ x[5];
            c[9] <= c[1] ^ c[12] ^ c[16] ^ x[4] ^ x[8];
            c[8] <= c[11] ^ c[15] ^ c[16] ^ x[3] ^ x[7] ^ x[8];
            c[7] <= c[10] ^ c[14] ^ c[15] ^ x[2] ^ x[6] ^ x[7];
            c[6] <= c[9] ^ c[13] ^ c[14] ^ x[1] ^ x[5] ^ x[6];
            c[5] <= c[13] ^ x[5];
            c[4] <= c[12] ^ c[16] ^ x[4] ^ x[8];
            c[3] <= c[11] ^ c[15] ^ x[3] ^ x[7];
            c[2] <= c[10] ^ c[14] ^ x[2] ^ x[6];
            c[1] <= c[9] ^ c[13] ^ x[1] ^ x[5];
        end
    end
end

endmodule
