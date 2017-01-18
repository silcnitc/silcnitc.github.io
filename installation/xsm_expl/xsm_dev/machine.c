#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "machine.h"
#include "tokenize.h"
#include "exception.h"


static
xsm_cpu _thecpu;

static
xsm_options _theoptions;

const char *instructions[]=
{
   "MOV",
   "ADD",
   "SUB",
   "MUL", 
   "DIV",
   "MOD",
   "INR",
   "DCR",
   "LT",
   "GT", 
   "EQ",
   "NE", 
   "GE", 
   "LE",
   "JZ",
   "JNZ",
   "JMP",
   "PUSH",
   "POP",
   "CALL",
   "RET",
   "BRKP",
   "INT",
   
   "LOADI",
   "LOAD",
   "STORE",
   "ENCRYPT",
   "BACKUP",
   "RESTORE",
   "PORT",
   "IN",
   "INI",
   "OUT",
   "IRET",
   "HALT"
};

int
machine_init (xsm_options *options)
{
   xsm_word *ipreg;

   _theoptions = *options;

   /* Set up the registers. */
   if (!registers_init ())
      return XSM_FAILURE;

   if (!memory_init())
      return XSM_FAILURE;

   if (!debug_init())
      return FALSE;

   /* Initialize machine. */
   word_store_string(memory_get_word(0), "LOADI 1, 0");
   word_store_string(memory_get_word(2), "LOADI 2, 1");
   word_store_string(memory_get_word(4), "JMP 512");

   //disk_read_block(memory_get_page(1), 0);

   /* Set up IP.. */
   ipreg = machine_get_ipreg ();
   word_store_integer(ipreg, 0);

   machine_set_mode(PRIVILEGE_KERNEL);

   /* The disk and console is idle.*/
   _thecpu.console_state = XSM_CONSOLE_IDLE;
   _thecpu.disk_state = XSM_DISK_IDLE;

   /* Initialise timer clock*/
   
   _thecpu.timer = _theoptions.timer;
   
   return XSM_SUCCESS;
}

int
machine_get_opcode (const char* instr)
{
   int i;
   
   for (i = 0; i < XSM_INSTRUCTION_COUNT; ++i)
   {
      if (!strcasecmp(instr, instructions[i]))
         return i;
   }
   
   return XSM_ILLINSTR;
}

/* When the lexer calls, serve him with the instruction to execute.
 * Here we have a problem. The read_bytes need to be of type yy_size_t.
 * If this argument raises a warning, then investigate what yy_size_t means,
 * and change it here accordingly.
 */
 
int
machine_serve_instruction (char _output_ *buffer, unsigned long _output_ *read_bytes, int _input_ max)
{
	
   int ip_val, i, j;
   xsm_word *ip_reg;
   int bytes_to_read;
   xsm_word *instr_mem;

   bytes_to_read = XSM_INSTRUCTION_SIZE * XSM_WORD_SIZE;
   
   ip_reg = machine_get_ipreg();
   ip_val = word_get_integer(ip_reg);

   ip_val = machine_translate_address(ip_val, FALSE);
   instr_mem = machine_memory_get_word(ip_val);
   
   memcpy (buffer, instr_mem->val, bytes_to_read);

   /* Trim. */
   for (i = 0; i < bytes_to_read; ++i)
   {
      if (buffer[i] == '\0'){
         for(j=i;j<bytes_to_read/2;j++)
			buffer[j] = ' ';
	 }
   }

   buffer[bytes_to_read - 1] = '\0';
    
   *read_bytes = bytes_to_read;
   return TRUE;
}

/* Raises an exception. This function never returns. */
void
machine_register_exception (char* message, int code)
{
   int mode;

   mode = machine_get_mode();
   exception_set(message, code, mode);
   /* Abandon ship! Abandon ship! */
   longjmp (_thecpu.h_exp_point, XSM_EXCEPTION_OCCURED);
}

xsm_word*
machine_memory_get_word (int address)
{
   xsm_word *result;

   result = memory_get_word(address);

   if (NULL == result)
   {
      /* Doomsday! */
      exception_set_ma(address);
      machine_register_exception("Illegal memory access", EXP_ILLMEM);
   }

   return result;
}

xsm_word *
machine_get_ipreg ()
{
   return registers_get_register("IP");
}

xsm_word *
machine_get_spreg ()
{
   return registers_get_register("SP");
}

void
machine_pre_execute(int ip_val)
{
   /* Clear the potential watchpoint trigger. */
   _thecpu.mem_low = -1;

   /* If debugging was requested and the mode is user, activate the debug command line. */
   if (_theoptions.debug)
   {
	   if( machine_get_mode() == PRIVILEGE_USER)
			debug_next_step (ip_val);
   }
}

int
machine_instr_req_privilege (int opcode)
{
   if (opcode >= TOKEN_KERN_LOW && TOKEN_KERN_HIGH >= opcode)
      return PRIVILEGE_KERNEL;
   return PRIVILEGE_USER;
}

int
machine_run ()
{
   int token, opcode;
   YYSTYPE token_info;
   xsm_word *ipreg;
   int ipval;
   int exp_occured;

   ipreg = machine_get_ipreg ();

   while (TRUE){
      /* Set the exception point. */
      exp_occured = setjmp (_thecpu.h_exp_point);

      if (exp_occured == XSM_EXCEPTION_OCCURED)
      {
         if (XSM_SUCCESS != machine_handle_exception())
            break;
      }

      /* Flush the instruction stream. */
      tokenize_clear_stream ();
      tokenize_reset ();

      ipval = word_get_integer(ipreg);
      machine_pre_execute (ipval);

      token = tokenize_next_token (&token_info);

      /* IP = IP + instruction length. */
      ipval = ipval + XSM_INSTRUCTION_SIZE;
      word_store_integer (ipreg, ipval);

      if (token != TOKEN_INSTRUCTION)
      {
         machine_register_exception("The simulator has encountered an illegal instruction", EXP_ILLINSTR);
      }

      opcode = machine_get_opcode(token_info.str);

      if (XSM_ILLINSTR == opcode)
      {
         machine_register_exception("The instruction is not available in this architecture", EXP_ILLINSTR);
      }

      if (machine_instr_req_privilege(opcode) == PRIVILEGE_KERNEL &&
         machine_get_mode() == PRIVILEGE_USER)
      {
         machine_register_exception("This instruction requires more privilege.", EXP_ILLINSTR);
      }

      if (XSM_HALT == machine_execute_instruction (opcode))
         break;

      /* Post executing instruction. 
			Enabled Only in User mode
       */
       
       if( machine_get_mode() == PRIVILEGE_USER)
			machine_post_execute ();
   
   }

   return TRUE;
}

/* Return the memory range accessed by the CPU for the last operation. */
void
machine_get_mem_access (int _output_ *mem_low, int _output_ *mem_high)
{
   *mem_low = _thecpu.mem_low;
   *mem_high = _thecpu.mem_high;
}

int
machine_handle_exception()
{
   char *message;
   int code, mode;
   int curr_ip;
  
   xsm_word *reg_eip, *reg_epn, *reg_ec, *reg_ema;

   /* Get the details about the exception. */
   mode = machine_get_mode ();
   code = exception_code ();
   message = exception_message();

   /* Get the exception registers. */
   reg_eip = registers_get_register("EIP");
   reg_epn = registers_get_register("EPN");
   reg_ec = registers_get_register("EC");
   reg_ema = registers_get_register("EMA");

   // fetch ip store in eip
   curr_ip = word_get_integer(registers_get_register("IP"));
   word_store_integer(reg_eip, curr_ip);
 
   switch(mode)
   {
      case EXP_ILLMEM:
         word_store_integer (reg_ema, exception_get_ma());
         break;

      case EXP_PAGEFAULT:
         word_store_integer (reg_epn, exception_get_epn());
         break;
   }

   if (PRIVILEGE_USER == mode)
   {
      machine_execute_interrupt_do(XSM_INTERRUPT_EXHANDLER);
      return XSM_SUCCESS;
   }

   fprintf (stderr, "%s: System halted.\n", message);
   /* TODO May print the machine status if required. */
   fprintf (stderr, "Trace:\nEIP %s\tEPN %s\t EC %s\t EMA %s\n",
      word_get_string(reg_eip), word_get_string(reg_epn), word_get_string(reg_ec), word_get_string(reg_ema));
   return XSM_FAILURE;
}

void
machine_post_execute ()
{
   /* Tick the timers. */
   _thecpu.timer--;
   
   if (_thecpu.timer == 0)
   {
      machine_execute_interrupt_do(XSM_INTERRUPT_TIMER);
      _thecpu.timer = _theoptions.timer;
   }
   /* Handle the disk interrupt. */
   if (_thecpu.disk_state == XSM_DISK_BUSY)
   {
      _thecpu.disk_wait--;

      if (_thecpu.disk_wait == 0)
      {
         if (XSM_DISKOP_LOAD == _thecpu.disk_op.operation)
         {
            machine_execute_load_do (_thecpu.disk_op.dest_page, _thecpu.disk_op.src_block);
            machine_execute_interrupt_do(XSM_INTERRUPT_DISK);
         }
         else if (XSM_DISKOP_STORE == _thecpu.disk_op.operation)
         {
            machine_execute_store_do (_thecpu.disk_op.dest_page, _thecpu.disk_op.src_block);
            machine_execute_interrupt_do(XSM_INTERRUPT_DISK);
         }

         _thecpu.disk_state = XSM_DISK_IDLE;
      }
   }

   if (XSM_CONSOLE_BUSY == _thecpu.console_state)
   {
      _thecpu.console_wait--;
      if (_thecpu.console_wait == 0)
      {
         if (XSM_CONSOLE_PRINT == _thecpu.console_op.operation)
         {
            machine_execute_print_do(&_thecpu.console_op.word);
            machine_execute_interrupt_do(XSM_INTERRUPT_CONSOLE);
         }
         else if (XSM_CONSOLE_READ == _thecpu.console_op.operation)
         {
            xsm_word *dest_port;
            machine_execute_in_do(&_thecpu.console_op.word);

            dest_port = registers_get_register ("P0");
            word_copy (dest_port, &_thecpu.console_op.word);
            machine_execute_interrupt_do(XSM_INTERRUPT_CONSOLE);
         }

         _thecpu.console_state = XSM_CONSOLE_IDLE;
      }
   }

   return;
}

int 
machine_execute_instruction (int opcode)
{
   switch (opcode)
   {
      case MOV:
      case PORT:
         machine_execute_mov ();
         break;

      case ADD:
      case SUB:
      case MUL:
      case DIV:
      case MOD:
         machine_execute_arith(opcode);
         break;

      case INR:
      case DCR:
         machine_execute_unary(opcode);
         break;

      case LT:
      case GT:
      case EQ:
      case NE:
      case GE:
      case LE:
         machine_execute_logical(opcode);
         break;

      case JZ:
      case JNZ:
      case JMP:
         machine_execute_jump (opcode);
         break;

      case PUSH:
      case POP:
         machine_execute_stack (opcode);
         break;

      case CALL:
         machine_execute_call ();
         break;

      case RET:
         machine_execute_ret ();
         break;

      case BRKP:
         machine_execute_brkp ();
         break;

      case INT:
         machine_execute_interrupt ();
         break;

      case LOAD:
         machine_execute_disk (XSM_DISKOP_LOAD, FALSE);
         break;

      case LOADI:
         machine_execute_disk (XSM_DISKOP_LOAD, TRUE);
         break;

      case STORE:
         machine_execute_disk (XSM_DISKOP_STORE, FALSE);
         break;

      case ENCRYPT:
         machine_execute_encrypt();
         break;

      case BACKUP:
         machine_execute_backup ();
         break;

      case RESTORE:
         machine_execute_restore ();
         break;

      case IN:
         machine_execute_ini ();
         break;

      case INI:
         machine_execute_ini ();
         break;

      case OUT:
         machine_execute_print ();
         break;

      case IRET:
         machine_execute_iret ();
         break;

      case HALT:	
         return XSM_HALT;
   }

   return TRUE;
}

int
machine_get_mode ()
{
   return _thecpu.mode;
}

xsm_word*
machine_get_register (const char *name)
{
   int mode;
   xsm_word *reg;

   mode = machine_get_mode();

   if (PRIVILEGE_USER == mode)
   {
      if (!registers_umode(name))
      {
         /* This register in not available in user mode. */
         machine_register_exception("Operand not available", EXP_ILLINSTR);
      }
   }

   if (!strcasecmp(name, "IP"))
   {
      machine_register_exception("IP register can not be directly manipulated", EXP_ILLINSTR);
   }

   reg = registers_get_register(name);

   if (!reg)
   {
      machine_register_exception("No such register", EXP_ILLINSTR);
   }

   return reg;
}

int
machine_execute_logical (int opcode)
{
   xsm_word *src_left_reg, *src_right_reg;
   int token;
   YYSTYPE token_info;
   
   int result, val_left, val_right;

   token = tokenize_next_token(&token_info);
   src_left_reg = machine_get_register(token_info.str);

   /* Comma */
   token = tokenize_next_token(&token_info);

   if (TOKEN_COMMA != token)
   { 
      machine_register_exception("Incorrect logical instruction.", EXP_ILLINSTR);
   }

   token = tokenize_next_token(&token_info);
   src_right_reg = machine_get_register(token_info.str);

  /* String operation */
  
  if(word_get_unix_type(src_left_reg) == XSM_TYPE_STRING || 
                  word_get_unix_type(src_right_reg) == XSM_TYPE_STRING){
					  
	  char* wor_left = word_get_string(src_left_reg);
	  char* wor_right = word_get_string(src_right_reg);
	  
	  switch(opcode)
	   {
		  case LT:
			 result = strcmp(wor_left,wor_right) < 0 ? 1 : 0;
			 break;

		  case GT:
			  result = strcmp(wor_left,wor_right) > 0 ? 1 : 0;
			 break;

		  case EQ:
			  result = strcmp(wor_left,wor_right) == 0 ? 1 : 0;
			 break;

		  case NE:
			 result = strcmp(wor_left,wor_right) != 0 ? 1 : 0;
			 break;

		  case GE:
			  result = strcmp(wor_left,wor_right) >= 0 ? 1 : 0;
			 break;

		  case LE:
			  result = strcmp(wor_left,wor_right) <= 0 ? 1 : 0;
			 break;
	   }
	   	
	  }
  
  /* Integer operation */
  
  else {
	   val_left = word_get_integer(src_left_reg);
	   val_right = word_get_integer(src_right_reg);

	   switch(opcode)
	   {
		  case LT:
			 result = val_left < val_right ? 1 : 0;
			 break;

		  case GT:
			 result = val_left > val_right ? 1 : 0;
			 break;

		  case EQ:
			 result = val_left == val_right ? 1 : 0;
			 break;

		  case NE:
			 result = val_left != val_right ? 1 : 0;
			 break;

		  case GE:
			 result = val_left >= val_right ? 1 : 0;
			 break;

		  case LE:
			 result = val_left <= val_right ? 1 : 0;
			 break;
	   }
    }

  word_store_integer(src_left_reg, result);
  return XSM_SUCCESS;

}

int
machine_execute_brkp ()
{
   /* TODO: Initiate debugger. */

   /* If debug mode is not enabled, neglect this instruction. */
   if (!_theoptions.debug)
      return XSM_SUCCESS;

	/* activate the debug mode 
		deactivated on pressing e or s in debug interface.
	*/
	debug_activate();
	
   return XSM_SUCCESS;
}

int
machine_execute_unary (int opcode)
{
   int token;
   YYSTYPE token_info;
   xsm_word *arg_reg;
   int val;

   token = tokenize_next_token(&token_info);
   arg_reg = machine_get_register(token_info.str);

   val = word_get_integer(arg_reg);

   switch (opcode)
   {
      case INR:
         val = val + 1;
         break;

      case DCR:
         val = val - 1;
         break;
   }

   word_store_integer (arg_reg, val);
   return XSM_SUCCESS;
}

int
machine_execute_mov ()
{
   int token, mem_write_addr;
   xsm_word *l_address, *r_address;
   YYSTYPE token_info;

   token = tokenize_peek (&token_info);

   switch (token)
   {
      case TOKEN_DREF_L:
         _thecpu.mem_low = machine_get_address_int (TRUE);
         _thecpu.mem_high = _thecpu.mem_high;
         l_address = machine_memory_get_word(_thecpu.mem_low);
         break;

      case TOKEN_REGISTER:
         l_address = machine_get_register (token_info.str);
         token = tokenize_next_token (&token_info);
         break;
   }

   token = tokenize_next_token(&token_info);

   if (token != TOKEN_COMMA)
   {
      machine_register_exception("Malformed instruction.", EXP_ILLINSTR);
   }

   token = tokenize_peek (&token_info);

   switch (token)
   {
      case TOKEN_DREF_L:
         r_address = machine_get_address (FALSE);
         word_copy (l_address, r_address);
         break;

      case TOKEN_REGISTER:
         r_address = machine_get_register(token_info.str);
         word_copy (l_address, r_address);
         tokenize_next_token(&token_info);
         break;

      case TOKEN_NUMBER:
         word_store_integer (l_address, token_info.val);
         tokenize_next_token(&token_info);
         break;

      case TOKEN_STRING:
         word_store_string (l_address, token_info.str);
         tokenize_next_token(&token_info);
         break;

      default:
         ; /* Nothing to do. */
   }

   /* We have a successful write, log the operation. */


   return XSM_SUCCESS;
}

xsm_word*
machine_get_address (int write)
{
   int address = machine_get_address_int (write);
   return machine_memory_get_word(address);
}

/* Retrieve the memory address from the instruction stream as an *integer*. */
int
machine_get_address_int (int write)
{
   int token, address;
   YYSTYPE token_info;

   /* Skip the opening square bracket. */
   tokenize_next_token(&token_info);
   token = tokenize_next_token(&token_info);
	
   switch (token)
   {
      case TOKEN_REGISTER:
      {
         address = registers_get_integer (token_info.str);
      }
      break;

      case TOKEN_NUMBER:
         address = token_info.val;
         break;

      default:
         /* Mark him. */
         machine_register_exception ("Invalid memory derefence.", EXP_ILLINSTR);
   }

   /* Next one is a bracket, neglect. */
   tokenize_next_token(&token_info);

   /* Ask the MMU to translate the address for us. */
    
   address = machine_translate_address (address, write);

   if (XSM_MEM_NOWRITE == address)
   {
      exception_set_ma (address);
      machine_register_exception("Access violation.", EXP_ILLMEM);
   }
   
   else if (XSM_MEM_PAGEFAULT == address)
   {
      exception_set_epn (memory_addr_page(address));
      machine_register_exception("Page fault.", EXP_PAGEFAULT);
   }

   return address;
}

int
machine_translate_address (int address, int write)
{
   int ptbr;

   if (_thecpu.mode == PRIVILEGE_KERNEL)
      return address;

   /* User mode, ask the MMU to translate. */
   ptbr = word_get_integer (registers_get_register("PTBR"));
   return memory_translate_address (ptbr, address, write);
}

int
machine_execute_arith (int opcode)
{
	// todo - Check for string operands - generate error.
   int result, token;
   xsm_reg *l_operand, *r_operand;
   YYSTYPE token_info;
   int l_value, r_value;

   token = tokenize_next_token(&token_info);

   if (token != TOKEN_REGISTER)
      machine_register_exception("Wrong operand.", EXP_ILLINSTR);

   l_operand = machine_get_register(token_info.str);
   l_value = word_get_integer(l_operand);

   /* Next one is a comma, neglect. */
   tokenize_next_token(&token_info);

   token = tokenize_next_token(&token_info);

   if (token == TOKEN_NUMBER)
   {
      r_value = token_info.val;
   }
   else
   {
      r_operand = machine_get_register(token_info.str);
      r_value = word_get_integer (r_operand);
   }

   switch (opcode)
   {
      case ADD:
         result = r_value + l_value;
         break;

      case SUB:
         result = l_value - r_value;
         break;

      case MUL:
         result = l_value * r_value;
         break;

      case DIV:
         /* Integer division by zero !*/
         if (0 == r_value)
         {
            machine_register_exception("Integer divide by zero.", EXP_ARITH);
         }
         result = l_value / r_value;
         break;

      case MOD:

         if (0 == r_value)
         {
            machine_register_exception("Integer modulus 0.", EXP_ARITH);
         }
         result = l_value % r_value;
         break;
   }

   word_store_integer (l_operand, result);
   return XSM_SUCCESS;
}

int
machine_execute_jump (int opcode)
{
   
   int test, target, token;
   YYSTYPE token_info;

   token = tokenize_next_token(&token_info);

   if (token == TOKEN_NUMBER)
   {
      test = TRUE; /* Take the branch, the jump is unconditional. */

      target = token_info.val;
   }
   else 
   {
	  
	  // If string contents set as 1 -> Non zero
	  if(word_get_unix_type (machine_get_register(token_info.str)) == XSM_TYPE_STRING)
		test = 1;
	  else
        test = word_get_integer(machine_get_register(token_info.str));
      
      /* Skip the comma. */
      tokenize_next_token(&token_info);
      token = tokenize_next_token(&token_info);
      target = token_info.val;
   }

   if (JZ == opcode)
      test = !test;
	
   if (test)
   {
      /* Take the branch. */
      word_store_integer (registers_get_register("IP"), target);
   }
   else
   {
      /* Nothing to do. */
   }

   return XSM_SUCCESS;
}

int
machine_execute_stack (int opcode)
{
   YYSTYPE token_info;
   int token;
   xsm_word *reg;

   token = tokenize_next_token(&token_info);

   if (token == TOKEN_REGISTER)
   {
      reg = machine_get_register(token_info.str);
   }
   else
   {
      /* TODO Invalid argument, raise exception. */
      machine_register_exception("Stack instructions require a register as argument.", EXP_ILLINSTR);

   }

   switch (opcode)
   {
      case PUSH:
         return machine_push_do(reg);

      case POP:
         return machine_pop_do (reg);
   }

   return XSM_SUCCESS;
}

int
machine_push_do (xsm_word *reg)
{
   xsm_word *xw_stack_top;
   xsm_word *sp_reg;
   int stack_top;

   /* Update SP */
   sp_reg = registers_get_register("SP");
   stack_top = word_get_integer(sp_reg);
   word_store_integer(sp_reg, stack_top + 1);

   /* Get the new stack pointer. */
   xw_stack_top = machine_stack_pointer (FALSE);

   /* Put the word on the top of stack. */
   word_copy (xw_stack_top, reg);
   return XSM_SUCCESS;
}

int
machine_pop_do (xsm_word *dest)
{
   xsm_word *xw_stack_top;
   xsm_word *sp_reg;
   int stack_top;

   xw_stack_top = machine_stack_pointer(TRUE);
   sp_reg = registers_get_register("SP");
   stack_top = word_get_integer(sp_reg);

   word_copy (dest, xw_stack_top);
   word_store_integer(sp_reg, stack_top - 1);
   return XSM_SUCCESS;
}

xsm_word*
machine_stack_pointer (int write)
{
   xsm_word *sp_reg;
   int stack_top;

   sp_reg = registers_get_register ("SP");
   stack_top = word_get_integer(sp_reg);

   stack_top = machine_translate_address (stack_top, write);

   return machine_memory_get_word(stack_top);
}

int
machine_execute_call_do (int target)
{
   int curr_ip, curr_sp;
   xsm_word *ipreg;
   xsm_word *stack_pointer;
   xsm_word *spreg;
	
   /* Increment SP. */
   spreg = registers_get_register("SP");
   curr_sp = word_get_integer(spreg);
   curr_sp = curr_sp + 1;
   word_store_integer(spreg, curr_sp);


   /* Save IP onto the stack. */
   ipreg = registers_get_register("IP");
   curr_ip = word_get_integer(ipreg);
   stack_pointer = machine_stack_pointer (TRUE);
   word_store_integer(stack_pointer, curr_ip);
	
   /* Update IP to the new code location. */
   word_store_integer (ipreg, target);
   
   return XSM_SUCCESS;
}

int
machine_execute_backup()
{	
   xsm_word *reg;
   int ireg;
   char str_reg[5];

   reg = registers_get_register("BP");
   machine_push_do (reg);

   reg = registers_get_register("PTBR");
   machine_push_do (reg);

   reg = registers_get_register("PTLR");
   machine_push_do (reg);

   for (ireg = 0; ireg < 19; ++ireg)
   {
      sprintf (str_reg, "R%d", ireg);
      reg = registers_get_register(str_reg);
      machine_push_do(reg);
   }

   return XSM_SUCCESS;
}

int
machine_execute_restore ()
{
   xsm_word *reg;
   int ireg;
   char str_reg[5];

   for (ireg = 18; ireg >= 0; ireg--)
   {
      sprintf (str_reg, "R%d", ireg);
      reg = registers_get_register(str_reg);
      machine_pop_do (reg);
   }

   reg = registers_get_register ("PTLR");
   machine_pop_do (reg);

   reg = registers_get_register ("PTBR");
   machine_pop_do (reg);

   reg = registers_get_register ("BP");
   machine_pop_do (reg);

   return XSM_SUCCESS;
}

int
machine_execute_call ()
{
   int token, target;
   YYSTYPE token_info;

   token = tokenize_next_token(&token_info);
   target = token_info.val;

   return machine_execute_call_do (target);
}

int
machine_execute_ret ()
{
   int target;
   xsm_word *spreg, *ipreg;
   xsm_word *stack_pointer;
   int curr_sp;

   spreg = registers_get_register ("SP");
   stack_pointer = machine_stack_pointer (TRUE);
   target = word_get_integer(stack_pointer);

   curr_sp = word_get_integer (spreg);
   curr_sp = curr_sp - 1;
   word_store_integer (spreg, curr_sp);

   ipreg = registers_get_register("IP");
   word_store_integer (ipreg, target);

   return XSM_SUCCESS;
}

int
machine_execute_interrupt()
{
   int token;
   YYSTYPE token_info;
   int interrupt_num;

   token = tokenize_next_token(&token_info);

   interrupt_num = token_info.val;
   
   return machine_execute_interrupt_do(interrupt_num);
}

int 
machine_execute_interrupt_do (int interrupt)
{
   int target;

   target = machine_interrupt_address (interrupt);
   
   machine_execute_call_do (target);

   /* Change the mode now, that will do. */
   machine_set_mode (PRIVILEGE_KERNEL);
   return XSM_SUCCESS;
}

int
machine_interrupt_address (int int_num)
{
   if (int_num < 0 || int_num > 18)
      return -1; /* Not supposed to happen. */

   return (int_num * 2 + 2) * XSM_PAGE_SIZE;
}

void
machine_set_mode (int mode)
{
   _thecpu.mode = mode;
}

int
machine_read_disk_arg()
{
   YYSTYPE token_info;
   int token;

   token = tokenize_next_token(&token_info);

   if (token == TOKEN_NUMBER)
   {
      return token_info.val;
   }
   else if (token == TOKEN_REGISTER){
      xsm_word *reg;

      reg = machine_get_register(token_info.str);
      return word_get_integer(reg);
   }
   else{
      machine_register_exception("Wrong arguments for disk instruction", EXP_ILLINSTR);
   }

   /* This instruction is never going to be executed. */
   return 0;
}

int
machine_execute_disk (int operation, int immediate)
{
   int page_num;
   int block_num;
   xsm_word *page_base;

   page_num = machine_read_disk_arg();

   /* Comma, neglect */
   tokenize_skip_token();

   block_num = machine_read_disk_arg();

   if (immediate)
   {
      if (operation == XSM_DISKOP_LOAD)
         return machine_execute_load_do(page_num, block_num);
      else if (XSM_DISKOP_STORE == operation)
         return machine_execute_store_do(page_num, block_num);
   }
   else
      return machine_schedule_disk (page_num, block_num, _theoptions.disk, operation);

   return XSM_SUCCESS;
}

int
machine_execute_store_do (int page_num, int block_num)
{
   xsm_word *page_base;

   page_base = memory_get_page(page_num);
   return disk_write_page (page_base, block_num);
}

int
machine_schedule_disk (int page_num, int block_num, int firetime, int operation)
{
	
   /* If the disk is busy, just ignore the request. */
   if (_thecpu.disk_state == XSM_DISK_BUSY)
      return XSM_SUCCESS;

   _thecpu.disk_state = XSM_DISK_BUSY;
   _thecpu.disk_wait = firetime;
   _thecpu.disk_op.src_block = block_num;
   _thecpu.disk_op.dest_page = page_num;
   _thecpu.disk_op.operation = operation;

   return XSM_SUCCESS;
}

int
machine_execute_load_do (int page_num, int block_num)
{
   xsm_word *page_base;
   
   page_base = memory_get_page (page_num);

   return disk_read_block (page_base, block_num);
}

int
machine_execute_encrypt ()
{
   int token;
   YYSTYPE token_info;
   xsm_word *reg;

   token = tokenize_next_token(&token_info);
   reg = machine_get_register(token_info.str);

   /* Some very easy encryption. */
   word_encrypt (reg);

   return XSM_SUCCESS;
}

int
machine_execute_print_do (xsm_word *word)
{
   int type;
   char *str;
   int val;

   type = word_get_unix_type (word);

   if (type == XSM_TYPE_STRING)
   {
      str = word_get_string(word);
      fprintf (stdout, "%s\n", str);
   }
   else
   {
      val = word_get_integer(word);
      fprintf(stdout, "%d\n", val);
   }

   return XSM_SUCCESS;
}

int
machine_execute_print ()
{
   int val;
   xsm_word *reg;

   reg = registers_get_register ("P1");

   return machine_execute_print_do(reg);
}

/* Schedule a read operation. */
int
machine_schedule_in(int firetime)
{
   if (_thecpu.console_state == XSM_CONSOLE_BUSY)
      return XSM_FAILURE;

   _thecpu.console_op.operation = XSM_CONSOLE_READ;

   _thecpu.console_state = XSM_CONSOLE_BUSY;
   _thecpu.console_wait = firetime;

   return XSM_SUCCESS;
}

int
machine_execute_ini ()
{	
   xsm_word *reg;

   reg = registers_get_register ("P0");
   return machine_execute_in_do(reg);
}

int
machine_execute_in_do (xsm_word *word)
{
   char input[XSM_WORD_SIZE];
   int i;

   /*TODO: Be a bit careful here. */
   fgets (input, XSM_WORD_SIZE, stdin);

   /* Kill the extra newline. */
   for (i = 0; i < XSM_WORD_SIZE; ++i)
      if (input[i] == '\n')
         input[i] = '\0';
   
   //printf("READ: %s\n",input);   
   
   return word_store_string(word, input);
}

int
machine_execute_iret ()
{
   xsm_word target;
   xsm_word *ipreg;

   machine_set_mode (PRIVILEGE_USER);
   machine_pop_do (&target);
   
   ipreg = registers_get_register("IP");
   word_copy(ipreg, &target);
   return XSM_SUCCESS;
}

void
machine_destroy ()
{
   /* Demolish. */
   memory_destroy ();
   registers_destroy();
}
