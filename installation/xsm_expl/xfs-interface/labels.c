#include "labels.h"

static
label *_root;

const int ins_length = XSM_INSTRUCTION_SIZE * XSM_WORD_SIZE + 1; 

void
labels_reset ()
{
	label *ptr = _root, *next;
	
	while (ptr != NULL)
	{
		next = ptr->next;
		
		free(ptr->name);
		free(ptr);
		ptr = next;
	}
	
	_root = NULL;
}

int 
labels_phase_one(FILE *fp)
{
	char instruction[ins_length];
	char *label;
	int address = 0;
	
	fseek (fp, 0, SEEK_SET);
	while (fgets(instruction, ins_length, fp))
	{
		remove_newline_character(instruction,ins_length);
		if (labels_is_label(instruction))
		{
			label = labels_get_name (instruction);
			labels_insert (label, address);
		}
		else
		{
			address = address + XSM_INSTRUCTION_SIZE;
		}
	}
	
	return TRUE; 
}

int
labels_is_label (const char *str)
{
	if (str[strlen(str)-1]==':')
		return TRUE;
	return FALSE;
}

char*
labels_get_name (char *label)
{
	const char *delim = ":";
	
	return strdup(strtok(label, delim));
}

void
labels_insert (char *label_name, int address)
{
	label *ptr;
	
	ptr = (label *) malloc (sizeof(label));
	
	ptr->name = label_name;
	ptr->address = address;
	
	ptr->next = _root;	
	_root = ptr;
	
	return;		
}

int
labels_phase_two (FILE *fin, FILE *fout, int base_address)
{
	char line[ins_length],instruction[ins_length];
	char instr[XSM_WORD_SIZE];
	int address = 0,flag_isJumpOrCallIns;
	const char *s = " ,";
	char *opcode, *leftop, *rightop, *sep;
	
	fseek (fin, 0, SEEK_SET);
	while (fgets(line, ins_length, fin))
	{
		remove_newline_character(line,ins_length);

		if (labels_is_label(line)||strlen(line)<=0)
			continue;

		strncpy(instruction, line, ins_length);

		opcode = strtok (instruction, s);
		leftop = strtok (NULL, s);
		rightop = strtok (NULL, s);

		flag_isJumpOrCallIns = 0;
		sep="";

		if (!strcasecmp (opcode, "JMP") || !strcasecmp(opcode, "CALL"))
		{	
			flag_isJumpOrCallIns = 1;
			rightop = leftop;
			leftop = "";
		}
		else if (!strcasecmp(opcode, "JNZ") || !strcasecmp(opcode, "JZ"))
		{
			flag_isJumpOrCallIns = 1;
			sep=", ";
		}

		if (flag_isJumpOrCallIns == 1 && labels_is_charstring(rightop))
		{
			address = labels_get_target (rightop);
			
			if (address < 0)
			{
				fprintf (stderr, "Can not resolve label \"%s\".\n", rightop);
				return FALSE;
			}
			fprintf (fout, "%s %s%s%d\n", opcode, leftop, sep, address + base_address);
		}
		else
		{
			fprintf (fout, "%s\n", line);	
		}
	}
	
	return TRUE;			
}

int
labels_get_target (const char *name)
{
	label *ptr;

	ptr = _root;
	
	while (ptr)
	{
		if (!strcmp (ptr->name, name))
			return ptr->address;
		ptr = ptr->next;
	}
	
	return -1;
}

int
labels_is_charstring (char *str)
{
	char *p = str;

	if(!str)
		return FALSE;

	while (*p)
	{
		if (isalpha(*p))
			return TRUE;
			
		p++;
	}
	
	return FALSE;
}

int
labels_resolve (const char *filename, char *outfile, int base_address)
{
	FILE *fin, *ftemp;
		
	fin = fopen (filename, "r");
	
	if (!fin)
	{
		fprintf (stderr, "Can't open source file.\n");
		return FALSE;
	}
	
	labels_random_name(outfile);
	ftemp = fopen (outfile, "w");
	
	if (!ftemp)
	{
		fprintf (stderr, "Can't create temporary file.\n");
		return FALSE;
	}
	
	labels_phase_one (fin);

	labels_phase_two (fin, ftemp, base_address);	
	
	fclose (fin);
	fclose (ftemp);
}

void
labels_random_name (char *name)
{
	srand (time(NULL));
	sprintf (name, "tempfile.%d.xsm", rand());	
}

int
remove_newline_character (char *str,int length)
{
	char *p = str;
	int i=0;

	if(!str)
		return FALSE;

	while (*p&&i<length)
	{
		if (*p=='\n'||*p=='\0')
		{
			*p='\0';
			return TRUE;
		}
		i++;
		p++;
	}
	
	return FALSE;
}