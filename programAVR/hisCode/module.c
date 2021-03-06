// Standard payload module functionality

#include <avr/io.h>
#include <avr/eeprom.h>
#include "base.h"
#include "comms.h"
#include "uart.h"

#include "module.h"

void module_setup()
{
	payload_tx_enable(FALSE); // disable payload TX (if not already done by local io_pins_setup())

  module_id = eeprom_read_byte(MODULE_EEPROM_ADDR_MODULE_ID);
	if (module_id > 127)
		module_id = MODULE_INITIAL_ID;

	comms_setup();
	uart0_setup();
}

void module_packet_scan(uint8_t *data, uint8_t length)
{
	// perform initial packet scan and handle standard module packet classes
	
	if (length > 0)
	{
		switch (data[0])
		{
			case MODULE_PACKET_CLASS_CHANGE_MODULE_ID:
				if ((length > 1)&&(data[1] < 128))
				{
					eeprom_write_byte(MODULE_EEPROM_ADDR_MODULE_ID, data[1]);
					send_text("Module ID changed %d -> %d", module_id, data[1]);
					module_id = data[1];
				}
				return;
		}
	}
	
	// call external packet_scan() for module-specific packet handling
	packet_scan(data, length);
}