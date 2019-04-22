NOTE:
This is the project that realize a I2C-Slave in Verilog using FPGA and realize I2C-master
using GPIO to pull up and down(software) in MCU-STM32, to verify the I2C-slave function
reference:https://dlbeer.co.nz/articles/i2c.html
jcyuan:

something to Change:
the FSM of the i2c-slave,if it jump into WRITE-STATE,it can't jump back to IELE when MASTER send I2C-STOP-BIT,
only RESET can back to IDLE,while the Read can jump back if NO-ACK from master.
So,I changed codes that When in WRITE-STATE, if the master send the I2C-STOP or RESET,FSM can jump back to IDLE.
However, it still has some bugs,such as when I2C-STOP-BIT comes, 
the master has to several send extra cycles SCL to make sure the slave come back to IDLE

Also,I add some comment for the code to read it more easily


doc:
Robust...:follow the doc to realize the I2C-Slave,change code for the FSM that "From Write to Idle"
STM...: the demo of STM32-MCU for the master of I2C,STM32F103ZET6

FPGA_i2c_slave:
the project of i2c_slave about quartus
the board is ALTERA-DE2 cycloneII EP2C35F672C6

misc:
some verification reference about I2C, mainly reference the doc of sim_core

STM32_i2c_master:
demo: some demo to reference about STM32-MCU
i2c_stm32: the keil project of STM32F103ZET6 about I2C master 
NOTE:
	J-Link setting
	STM32F103ZE: Xtal=8MHz
	the JLink Flash Download is STM32F10x High-density Flash

src:
master:
	the source code of STM32F103ZET6 in Keil project to realize the I2C-master
	using the Peripheral of GPIO(for I2C),UART(for information), LED(for show), KEY(for change mode),EEPROM(for verify)
	using I2C-EEPROM-slave to test the function-right of I2C-master

slave:
	source:
		i2c.v: the I2C slave verilog source code
		i2c_master: the simulation of I2C master beheavior
		tb_i2c: the testbench of I2C test
	FPGA_debug:
		de2_assign_pin.tcl: the constrain of the FPGA
		i2c: the fpga version of i2c-slave,just add some LED and SW to test and verify easily







