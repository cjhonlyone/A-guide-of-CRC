`timescale 1 ns / 1 ps

module testbench;
	reg clk = 0;
	reg reset_async = 0;
	wire reset;

	always #5 clk = (clk === 1'b0);

	initial begin
		$dumpfile("testbench.vcd");
		$dumpvars(0, testbench);

		repeat (2) @(posedge clk);
		reset_async = 1;

		repeat (25) @(posedge clk);

		$finish;
	end

	reg [7:0] x = 8'hff;

	always @(posedge clk) begin
		if (!reset_async)
			x <= 8'hff;
		else
			x <= {x[6:0], 1'b0};
	end

	assign reset = x[7];

	reg enable;
	reg enable_p;
	reg init;
	reg m;
	reg [8:1]xm;
	wire [15:0] crc_out_0000;
	wire [15:0] crc_out_ffff;
	wire [15:0] crc_out_0000_p;
	wire [15:0] crc_out_ffff_p;

	reg [31:0] message = 32'h41000000;

	serial_crc_ccitt #(
		.init_value(16'h0000))
	serial_crc_ccitt_init_0000 (
		.clk(clk),
		.reset(reset),
		.enable(enable),
		.init(init), 
		.m(m), 
		.crc_out(crc_out_0000)
		);

	serial_crc_ccitt #(
		.init_value(16'hffff))
	serial_crc_ccitt_init_ffff(
		.clk(clk),
		.reset(reset),
		.enable(enable),
		.init(init), 
		.m(m), 
		.crc_out(crc_out_ffff)
		);

	parallel_crc_ccitt #(
		.init_value(16'h0000)) 
	inst_parallel_crc_ccitt_init_0000 (
			.clk     (clk),
			.reset   (reset),
			.enable  (enable_p),
			.init    (init),
			.x       (xm),
			.crc_out (crc_out_0000_p)
		);

	parallel_crc_ccitt #(
		.init_value(16'hffff)) 
	inst_parallel_crc_ccitt_init_ffff (
			.clk     (clk),
			.reset   (reset),
			.enable  (enable_p),
			.init    (init),
			.x       (xm),
			.crc_out (crc_out_ffff_p)
		);

	reg [15:0] crc_0000;
	reg [15:0] crc_ffff;
	task crc_ccitt;
	integer I;
		begin
		enable <= 0;
		init <= 0;
		m <= 0;
		enable_p <= 0;
		xm <= 0;

		wait (reset == 1'b0);
			
		@(posedge clk);

		for (I = 0; I < 8; I = I + 1) begin
			enable <= 1;
			m <= message[31];
			message <= {message[30:0],1'b0};
			@(posedge clk);	
			$display("m = %2d, i = %2d, crc_out_0000 = %x, crc_out_ffff = %x",
				m,I,crc_out_0000,crc_out_ffff);
		end 
		enable <= 0;
		@(posedge clk);	
		crc_0000 <= crc_out_0000;
		crc_ffff <= crc_out_ffff;
		$display("crc_0000 = %x",crc_out_0000);
		$display("crc_ffff = %x",crc_out_ffff);
		enable_p <= 1;
		xm <= 8'h41;
		@(posedge clk);	
		enable_p <= 0;
		xm <= 8'h41;
		@(posedge clk);	
		$display("crc_0000_p = %x",crc_out_0000_p);
		$display("crc_ffff_p = %x",crc_out_ffff_p);
		end
	endtask

	initial begin
		crc_ccitt();
	end

endmodule
