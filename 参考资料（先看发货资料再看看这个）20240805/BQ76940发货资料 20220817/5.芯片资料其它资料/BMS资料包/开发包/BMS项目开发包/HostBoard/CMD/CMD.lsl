// @(#)default.lsl	1.17
//
//  Linker script file for the VX-toolset for C166
//

// Define the near page addresses. Each DPP will point to a near page.
// It is recommended to keep __DPP3_ADDR at 0x00C000
#define __DPP0_ADDR	0x008000
#define __DPP1_ADDR	0xC00000
#define __DPP2_ADDR	0xC10000
#define __DPP3_ADDR	0x00C000

// Include the CPU specific .lsl file
// The CPU is specified by the __CPU__ macro
#ifndef __CPU__
#error No CPU defined, please link with -D__CPU__=<cpu>
#endif
#define __QUOTE_ME2(x) #x
#define __QUOTE_ME(x) __QUOTE_ME2(x)
#define CPULSL_FILE __QUOTE_ME(__CPU__.lsl)
#include CPULSL_FILE


// Define interrupt vector table
section_setup ::code
{
	vector_table "vector_table" ( vector_size = 4, size = 128, run_addr = 0xC00000, 
  				template="__vector_template", template_symbol="__lc_vector_target", 
				vector_prefix=".vector.", fill = loop)
	{
		vector (id=0, fill="__cstart");
	}
}

// Define the system stack
section_layout ::shuge (direction = high_to_low)
{
	group ( run_addr = [0xF200..0xFC00], ordered ) stack "system_stack" ( size = 512 );
}





