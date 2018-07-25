#include <stdio.h>


typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;

#pragma warning(disable:4996)

#pragma pack(push,1)

typedef struct Partition	//Partition
{
	u_char Boot_Flag;		//Boot_Flag
	u_char S_CHS_A[3];		//Starting CHS Address
	u_char PT;				//Partition Type
	u_char E_CHS_A[3];		//Ending CHS Addr
	u_int S_LBA_A;			//Starting LBA Addr
	u_int Size_Sector;		//Size in Sector
};

typedef struct MBR		//Master Boot Record
{
	u_char Boot_Code[446];
	struct Partition P_table[4];
	u_short BRS;		//Boot Record Signature(0x55 0xAA)
};
#pragma pack(pop)

int main(int argc, char* argv[])
{
	FILE *fp;
	
	struct MBR *MBR;
	struct MBR *EBR;

	MBR = malloc(sizeof(512));
	EBR = malloc(sizeof(512));

	if (argc != 2)
	{
		printf("Usasge : MBR.exe [filename]\n");
		return 0;
	}
	//fp = fopen(argv[1], "rb");
	if ((fp = fopen(argv[1],"rb" ))== NULL)
	{
		printf("error : not exist filename\n");
	}


	fread(MBR, 512, 1, fp);

	
	printf("\n [Master Boot Record]\n");
	printf("¦®-----------¦³-----------¦³----------------¦³-----------¦³-----------------¦³----------------¦³----------------¦³---------------¦¯\n");
	printf("¦¢ Partition ¦¢ Boot Flag ¦¢ Start CHS addr ¦¢ Part type ¦¢ Ending CHS Addr ¦¢ Start LBA addr ¦¢ Size in Sector ¦¢ Start offset  ¦¢\n");
	printf("¦²-----------¦¶-----------¦¶----------------¦¶-----------¦¶-----------------¦¶----------------¦¶----------------¦¶---------------¦´\n");
	printf("¦¢ #%d\t    ¦¢ 0x%02x\t¦¢ 0x%02x%02x%02x\t ¦¢ 0x%02x\t     ¦¢ 0x%02x%02x%02x\t       ¦¢ 0x%08x\t¦¢ 0x%08x\t ¦¢ %-10d\t ¦¢\n",1,MBR->P_table[0].Boot_Flag,MBR->P_table[0].S_CHS_A[0],MBR->P_table[0].S_CHS_A[1],MBR->P_table[0].S_CHS_A[2], MBR->P_table[0].PT, MBR->P_table[0].E_CHS_A[0], MBR->P_table[0].E_CHS_A[1], MBR->P_table[0].E_CHS_A[2],MBR->P_table[0].S_LBA_A, MBR->P_table[0].Size_Sector,MBR->P_table[0].S_LBA_A * 512);
	printf("¦¢ #%d\t    ¦¢ 0x%02x\t¦¢ 0x%02x%02x%02x\t ¦¢ 0x%02x\t     ¦¢ 0x%02x%02x%02x\t       ¦¢ 0x%08x\t¦¢ 0x%08x\t ¦¢ %-10d\t ¦¢\n", 2, MBR->P_table[1].Boot_Flag, MBR->P_table[1].S_CHS_A[0], MBR->P_table[1].S_CHS_A[1], MBR->P_table[1].S_CHS_A[2], MBR->P_table[1].PT, MBR->P_table[1].E_CHS_A[0], MBR->P_table[1].E_CHS_A[1], MBR->P_table[1].E_CHS_A[2], MBR->P_table[1].S_LBA_A, MBR->P_table[1].Size_Sector, MBR->P_table[1].S_LBA_A * 512);
	printf("¦¢ #%d\t    ¦¢ 0x%02x\t¦¢ 0x%02x%02x%02x\t ¦¢ 0x%02x\t     ¦¢ 0x%02x%02x%02x\t       ¦¢ 0x%08x\t¦¢ 0x%08x\t ¦¢ %-10d\t ¦¢\n", 3, MBR->P_table[2].Boot_Flag, MBR->P_table[2].S_CHS_A[0], MBR->P_table[2].S_CHS_A[1], MBR->P_table[2].S_CHS_A[2], MBR->P_table[2].PT, MBR->P_table[2].E_CHS_A[0], MBR->P_table[2].E_CHS_A[1], MBR->P_table[2].E_CHS_A[2], MBR->P_table[2].S_LBA_A, MBR->P_table[2].Size_Sector, MBR->P_table[2].S_LBA_A * 512);
	printf("¦¢ #%d\t    ¦¢ 0x%02x\t¦¢ 0x%02x%02x%02x\t ¦¢ 0x%02x\t     ¦¢ 0x%02x%02x%02x\t       ¦¢ 0x%08x\t¦¢ 0x%08x\t ¦¢ %-10d\t ¦¢\n", 4, MBR->P_table[3].Boot_Flag, MBR->P_table[3].S_CHS_A[0], MBR->P_table[3].S_CHS_A[1], MBR->P_table[3].S_CHS_A[2], MBR->P_table[3].PT, MBR->P_table[3].E_CHS_A[0], MBR->P_table[3].E_CHS_A[1], MBR->P_table[3].E_CHS_A[2], MBR->P_table[3].S_LBA_A, MBR->P_table[3].Size_Sector, MBR->P_table[3].S_LBA_A * 512);
	printf("¦±-----------¦µ-----------¦µ----------------¦µ-----------¦µ-----------------¦µ----------------¦µ----------------¦µ---------------¦°\n\n");
	
	for (int i = 0; i < 4; i++)
	{
		if (MBR->P_table[i].PT == 0x5)			//0x5 = extended parttion
		{
			int EBR_location = (int)(MBR->P_table[i].S_LBA_A) * 512;
			int Extend_partition = i;
			u_int Extended_Partition_offset = EBR_location;

			fseek(fp, EBR_location, SEEK_SET);
			fread(EBR, 512, 1, fp);

			while (1)
			{
				
				printf("[Extended boot Record #%d ]\n", Extend_partition+1);
				printf("¦®-----------¦³-----------¦³----------------¦³-----------¦³-----------------¦³----------------¦³----------------¦³---------------¦¯\n");
				printf("¦¢ Partition ¦¢ Boot Flag ¦¢ Start CHS addr ¦¢ Part type ¦¢ Ending CHS Addr ¦¢ Start LBA addr ¦¢ Size in Sector ¦¢ Start offset  ¦¢\n");
				printf("¦²-----------¦¶-----------¦¶----------------¦¶-----------¦¶-----------------¦¶----------------¦¶----------------¦¶---------------¦´\n");
				printf("¦¢ #%d\t    ¦¢ 0x%02x\t¦¢ 0x%02x%02x%02x\t ¦¢ 0x%02x\t     ¦¢ 0x%02x%02x%02x\t       ¦¢ 0x%08x\t¦¢ 0x%08x\t ¦¢ %-10d\t ¦¢\n", 1, EBR->P_table[0].Boot_Flag, EBR->P_table[0].S_CHS_A[0], EBR->P_table[0].S_CHS_A[1], EBR->P_table[0].S_CHS_A[2], EBR->P_table[0].PT, EBR->P_table[0].E_CHS_A[0], EBR->P_table[0].E_CHS_A[1], EBR->P_table[0].E_CHS_A[2], EBR->P_table[0].S_LBA_A, EBR->P_table[0].Size_Sector,Extended_Partition_offset + EBR->P_table[0].S_LBA_A * 512);
				if (EBR->P_table[1].S_LBA_A == 0x00)
					EBR_location = 0;
				printf("¦¢ #%d\t    ¦¢ 0x%02x\t¦¢ 0x%02x%02x%02x\t ¦¢ 0x%02x\t     ¦¢ 0x%02x%02x%02x\t       ¦¢ 0x%08x\t¦¢ 0x%08x\t ¦¢ %-10d\t ¦¢\n", 2, EBR->P_table[1].Boot_Flag, EBR->P_table[1].S_CHS_A[0], EBR->P_table[1].S_CHS_A[1], EBR->P_table[1].S_CHS_A[2], EBR->P_table[1].PT, EBR->P_table[1].E_CHS_A[0], EBR->P_table[1].E_CHS_A[1], EBR->P_table[1].E_CHS_A[2], EBR->P_table[1].S_LBA_A, EBR->P_table[1].Size_Sector,EBR_location + EBR->P_table[1].S_LBA_A * 512);
				printf("¦±-----------¦µ-----------¦µ----------------¦µ-----------¦µ-----------------¦µ----------------¦µ----------------¦µ---------------¦°\n\n");
der
				Extended_Partition_offset += EBR->P_table[1].S_LBA_A * 512;

				if (EBR->P_table[1].PT == 0x00)
				{
					break;
				}
				fseek(fp, EBR_location + (int)(EBR->P_table[1].S_LBA_A)*512, SEEK_SET);
				fread(EBR, 512, 1, fp);
				Extend_partition++;
			}
			

		}
	}
	close(fp);
	
	return 0;
}