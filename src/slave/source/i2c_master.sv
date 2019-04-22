module i2c_master(
SDA,
SCL,
RST
);
input logic RST;
inout logic SCL;
inout logic SDA;
parameter i2c_delay=5;

reg scl_en;
reg sda_out;
wire sda_in;
assign SDA=sda_out?1'bz:1'b0;
assign sda_in=SDA;

assign SCL=scl_en?1'bz:1'b0;

pullup(SDA);
pullup(SCL);

always@(posedge RST)
begin
	if(RST)
	begin
		sda_out<=1'b1;
		scl_en<=1'b1;
	end
	else
	begin
		sda_out<=sda_out;
		scl_en<=scl_en;
	end
end


//---------------------------------------------------
//-----------------------base function---------------
//---------------------------------------------------
task start_bit();
	sda_out=1'b0;
	#i2c_delay;
	scl_en=1'b0;
	#i2c_delay;
	$display("I2C START\n");
endtask



task stop_bit();
	sda_out=1'b0;
	#i2c_delay;
	scl_en='b1;
	#i2c_delay;
	sda_out=1'b1;
	#i2c_delay;
	$display("I2C STOP\n");
endtask


task send_byte(input logic [7:0] data);
	integer i;
	#i2c_delay;
	for(i=0;i<=7;i=i+1)
	begin
		sda_out=data[7-i];
		#i2c_delay;
		scl_en=1'b1;
		#i2c_delay;
		scl_en=1'b0;
		#i2c_delay;
	end
	$display("SEND BYTE %h\n",data);
endtask

task detect_ack();
	#i2c_delay;
	scl_en=1'b1;
	if(sda_in==1'b0)
		$display("DETECT ACK\n");
	else
		$display($time,"ERROR ACK\n");
	#i2c_delay;
	scl_en=1'b0;
endtask


task restart();
	scl_en=1'b0;
	#i2c_delay;
	sda_out=1'b1;
	#i2c_delay;
	scl_en=1'b1;
	#i2c_delay;
	start_bit();
	$display("I2C RESTART\n");
endtask


task receive_byte(output logic [7:0] data);
	integer i;
	#i2c_delay;
	for(i=0;i<=7;i=i+1)
	begin
		#i2c_delay;
		scl_en=1'b1;
		data[7-i]=sda_in;
		#i2c_delay;
		scl_en=1'b0;
	end
	//gen ack or gen stop;
	$display("RECEIVE BYTE %h\n",data);
endtask

task gen_scl(input int cycles);
	integer i;
	for(i=0;i<cycles;i=i+1)
	begin
		#i2c_delay;
		scl_en=1'b1;
		#i2c_delay;
		scl_en=1'b0;
	end
	#i2c_delay;
	scl_en=1'b1;
	$display("MASTER GEN %d SCL CYCLES\n",cycles);
endtask


task gen_ack();
	#i2c_delay;
	scl_en=1'b0;
	#i2c_delay;
	sda_out=1'b0;
	#i2c_delay;
	scl_en=1'b1;
	#i2c_delay;
	scl_en=1'b0;
	#i2c_delay;
	sda_out=1'b1;
	$display("MASTER GEN ACK\n");
endtask





//-----------------------------------------------------
//-----------------------functions---------------------
//-----------------------------------------------------
task i2c_write(input logic [6:0] addr,input logic [7:0] index,input int N,input logic [7:0] data[$]);
	integer i;
	start_bit();
	send_byte({addr,1'b0});//write
	detect_ack();
	send_byte(index);
	detect_ack();
	for(i=0;i<N;i=i+1)
	begin
		send_byte(data[i]);
		detect_ack();
	end
	stop_bit();
	gen_scl(3);//for detect the stop to idle
	$display("write Bytes=%d to Addr=%h\n",N,addr);
	foreach	(data[j])
		$display("index=%d,data=%h\n",index+j,data[j]);
endtask



task i2c_read(input logic [6:0] addr,input logic [7:0] index,input int N,output logic [7:0] data[$]);
	integer i;
	start_bit();
	send_byte({addr,1'b0});//write
	detect_ack();
	send_byte(index);
	detect_ack();
	
	restart();
	send_byte({addr,1'b1});//read
	detect_ack();
	for(i=0;i<N;i=i+1)
	begin
		receive_byte(data[i]);
		if(i==N-1)//last data
		begin
			gen_scl(3);//no ack to jump to idle
			stop_bit();
		end
		else
			gen_ack();
	end
	$display("read Bytes=%d to Addr=%h\n",N,addr);
	foreach (data[j])
		$display("index=%d,data=%h\n",index+j,data[j]);
endtask




endmodule
