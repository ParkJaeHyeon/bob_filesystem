#include <stdio.h>

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long long int u_llint;

#pragma warning(disable:4996)

#pragma pack(push,1)

typedef struct GPT_Header
{
	u_llint Signature;
	u_int Revision;
	u_int Header_Size;
	u_int CRC32_of_Header;
	u_int Reserved;
	u_llint C_LBA;
	u_llint B_LBA;
	u_llint First_U_LBA;
	u_llint Last_U_LBA;
	u_char Disk_GUID[16];
	u_llint Partition_Entries_Start_LBA;
	u_int Number_of_Partition_entries;
	u_int Size_of_Partition_entry;
	u_int CRC32_of_partition_array;
};
typedef struct GPT_Partition_Entry
{
	u_char Partition_Type[16];
	u_char Unique_Partition[16];
	u_llint First_LBA;
	u_llint Last_LBA;
	u_llint Att_flags;
	u_char Partition_Name[72];
};
#pragma pack(pop)

int main(int argc, char* argv[])
{
	FILE *fp;

	struct GPT_Header * GPT_H;
	struct GPT_Partition_Entry * GPT_P;

	GPT_H = (struct GTP_Header *)malloc(88);
	
	if (argc != 2)
	{
		printf("Usasge : GPT.exe [filename]\n");
		return 0;
	}

	if ((fp = fopen(argv[1], "rb")) == NULL)
	{
		printf("error : not exist filename\n");
	}

	fseek(fp, 512, SEEK_SET);

	fread(GPT_H, 88, 1, fp);

	GPT_P = (struct GTP_Partition *) malloc((int)GPT_H->Size_of_Partition_entry);

	fseek(fp,GPT_H->Partition_Entries_Start_LBA * 512, SEEK_SET);

	int Size = GPT_H->Size_of_Partition_entry;
	u_llint offset = 0;

	printf("\n[ GPT Partition ]\n");
	printf("¦®-----------¦³--------------------¦³--------------¦¯\n");
	printf("¦¢ Partition ¦¢ First LBA          ¦¢ Start offset ¦¢\n");
	printf("¦²-----------¦¶--------------------¦¶--------------¦´\n");
	
	for (int i = 1; i <= 128; i++)
	{

		fread(GPT_P, Size, 1, fp);
		if (GPT_P->Partition_Type[0] == 0x00)
		{
			break;
		}
		offset = (GPT_P->First_LBA) * 512LL;
		printf("¦¢ #%d\t    ¦¢ 0x%016x ¦¢ ",i,GPT_P->First_LBA);
		printf("%lld\t¦¢ \n", offset);
		//printf("¦¢ \n");
	}
	printf("¦±-----------¦µ--------------------¦µ--------------¦°\n\n");
	fclose(fp);
	
	return 0;
}