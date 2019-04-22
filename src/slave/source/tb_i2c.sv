module tb_i2c();
wire SDA;
wire SCL;
reg RST;
reg [7:0] cmd;
reg [7:0] data_recv;
reg [7:0] data[$];
reg [7:0] recv[$];
parameter [6:0] slave_addr=7'h55;
parameter [7:0] index=8'h01;
parameter read=1'b1,write=1'b0;

i2c u_i2c(.SCL(SCL),.SDA(SDA),.RST(RST));
i2c_master u_i2c_master(.SCL(SCL),.SDA(SDA),.RST(RST));
initial
begin
	RST=1'b0;
	#2;
	RST=1'b1;
	#5;
	RST=1'b0;
end

initial
begin
	#12;
//one test
//	data={8'h5a};
//	u_i2c_master.i2c_write(slave_addr,index,1,data);
	u_i2c.reg_01<=8'h5a;//to force the data
	u_i2c_master.i2c_read(slave_addr,index,1,recv);


//two test
/*	data={8'h5a,8'ha5};
	u_i2c_master.i2c_write(slave_addr,index,2,data);

	u_i2c.reg_00<=8'ha5;//to force the data
	u_i2c.reg_01<=8'h5a;//to force the data
	u_i2c.reg_02<=8'haa;//to force the data
	u_i2c.reg_03<=8'h55;//to force the data
	u_i2c_master.i2c_read(slave_addr,index,2,recv);*/

end


//-----------------------initial test----------------------
//initial
//begin

//	#12;
/*	u_i2c_master.start_bit();
	cmd={slave_addr,write};
	u_i2c_master.send_byte(cmd);
	u_i2c_master.detect_ack();
	u_i2c_master.send_byte(index);
	u_i2c_master.detect_ack();
	u_i2c_master.send_byte(8'ha5);//data=8'ha5
	u_i2c_master.detect_ack();
	u_i2c_master.stop_bit();*/



/*	u_i2c.reg_03<=8'h5a;//to force the data
	u_i2c_master.start_bit();
	cmd={slave_addr,write};
	u_i2c_master.send_byte(cmd);
	u_i2c_master.detect_ack();
	u_i2c_master.send_byte(index);
	u_i2c_master.detect_ack();
	
	u_i2c_master.restart();
	cmd={slave_addr,read};
	u_i2c_master.send_byte(cmd);
	u_i2c_master.detect_ack();
	u_i2c_master.receive_byte(data_recv);

	u_i2c_master.gen_ack();//for continue to read

	//u_i2c_master.gen_scl(3);//for stop to read
	//u_i2c_master.stop_bit();
*/
//end
//-----------------------initial test----------------------


//-----------------------function test---------------------
//initial
//begin
//	#12;
//one test
/*	data={8'h5a};
	u_i2c_master.i2c_write(slave_addr,index,1,data);

	u_i2c.reg_03<=8'h5a;//to force the data
	u_i2c_master.i2c_read(slave_addr,index,1,recv);
*/

//two test
/*	data={8'h5a,8'ha5};
	u_i2c_master.i2c_write(slave_addr,index,2,data);

	u_i2c.reg_00<=8'ha5;//to force the data
	u_i2c.reg_01<=8'h5a;//to force the data
	u_i2c.reg_02<=8'haa;//to force the data
	u_i2c.reg_03<=8'h55;//to force the data
	u_i2c_master.i2c_read(slave_addr,index,2,recv);
*/
//end
//------------------------function test---------------------






endmodule



