#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <Windows.h>

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;

#pragma pack(push,1)
struct FSInfo
{
	u_int Lead_Signature;
	u_char Reserved1[480];
	u_int Struct_Signature;
	u_int Free_Cluster_count;
	u_int Next_Free_Cluster;
	u_char Reserved2[12];
	u_int Trail_Signature;
};
struct FAT32 
{
	u_int FAT_Size32;
	u_short Ext_Flags;
	u_short File_System_Version;
	u_int Root_Dir_Cluster;
	u_short File_System_Info;
	u_short Boot_Record_Backup_Sec;
	u_char Reserved[12];
	u_char Drive_Number;
	u_char Reserved1;
	u_char Boot_Signature;
	u_int Volume_ID;
	u_char Volume_Label[11];
	u_char File_System_Type[8];
};

struct FAT 
{
	u_char jmp_Boot_code[3];
	u_char OEM_name[8];
	u_short Byte_per_sector;
	u_char Sector_per_Cluster;
	u_short Reserved_Sector_count;
	u_char Number_of_FAT;
	u_short Root_Dir_Entry_Count;
	u_short Total_Sector16;
	u_char Media;
	u_short FAT_Size16;
	u_short Sector_Per_track;
	u_short Number_of_Heads;
	u_int Hidden_Sector;
	u_int Total_Sector32;
	struct FAT32 FAT_32;
	u_char Boot_Code[420];
	u_short Signaute;
};
struct Directory_Entry
{
	u_char Name[8];
	u_char Ext[3];
	u_char Attribute;
	u_char NT_Resource;
	u_char Create_Time_Tenth;
	u_short Create_Time;
	u_short Create_Date;
	u_short Last_Access_data;
	u_short First_Cluster;
	u_short Write_Time;
	u_short Write_Date;
	u_short First_Cluster_low;
	u_int Filesize;
};
struct Long_File_Name_Entry
{
	u_char Order;
	u_char Name1[10];
	u_char Attribute;
	u_char Type;
	u_char Check_Sum;
	u_char Name2[12];
	u_short First_Cluster_low;
	u_char Name3[4];
};
#pragma pack(pop)

int main(int argc, char* argv[])
{
	FILE *fp;
	errno_t err;
	if (argc != 2)
	{
		printf("Uasge: FAT32.exe [filename]\n");
		return -1;
	}
	if (err = fopen_s(&fp, argv[1], "rb") != 0)
	{
		printf("not exist %s\n", argv[1]);
		return -1;
	}
	int cluster_size = 0;
	int sector_size = 0;

	struct FAT *fat32 = (struct FAT *)malloc(sizeof(struct FAT));
	struct FSInfo *FS_Info = (struct FSInfo *)malloc(sizeof(struct FSInfo));
	struct Directory_Entry *D_entry = (struct Directory_Entry *)malloc(sizeof(struct Directory_Entry));
	struct Long_File_Name_Entry *LD_entry = (struct Long_File_Name_Entry *)malloc(sizeof(struct Long_File_Name_Entry));
	u_char *FAT_Table;
	char Delete_File;

	wchar_t Wide_Name1[10] = { 0, };
	wchar_t Wide_Name2[12] = { 0, };
	wchar_t Wide_Name3[4] = { 0, };
	fread(fat32, 512, 1, fp);
	fread(FS_Info, 512, 1, fp);
	
	cluster_size = fat32->Byte_per_sector * fat32->Sector_per_Cluster;
	sector_size = fat32->Byte_per_sector;

	FAT_Table = malloc(sizeof(char) * fat32->FAT_32.FAT_Size32 * sector_size);
	fseek(fp, fat32->Reserved_Sector_count * sector_size, SEEK_SET);
	fread(FAT_Table, fat32->FAT_32.FAT_Size32 * sector_size, 1, fp);

	int Data_offset = fat32->Reserved_Sector_count * sector_size + fat32->FAT_32.FAT_Size32 * sector_size * 2;

	fseek(fp,Data_offset,SEEK_SET);

	int offset = 0;
	int count = 0;
	printf(" [FAT32]\n");
	while (offset <= cluster_size)
	{
		fread(D_entry, sizeof(struct Directory_Entry), 1, fp);
		offset += sizeof(struct Directory_Entry);
		if (D_entry->Name[0] == 0x00)
			break;
		//삭제된 파일
		if (D_entry->Name[0] == 0xE5 || D_entry->Name[0] == 0x05)
		{
			Delete_File = 'x';
			//LFN
			if ((D_entry->Attribute & 0x0F) == 0x0F)
			{
				while (1)
				{
					fread(LD_entry, sizeof(struct Long_File_Name_Entry),1, fp);
					offset += sizeof(struct Directory_Entry);
					count++;
					if (LD_entry->Attribute != 0x0F)
					{
						offset -= sizeof(struct Directory_Entry);
						fseek(fp, -(count+1) * sizeof(struct Long_File_Name_Entry), 1, fp);
						break;
					}
				}
				printf("[%c] ", Delete_File);
				while (1)
				{
					fread(LD_entry, sizeof(struct Long_File_Name_Entry), 1, fp);
					u_char *name1;
					u_char *name2;
					u_char *name3;
					fread(LD_entry, sizeof(struct Long_File_Name_Entry), 1, fp);
					memcpy(Wide_Name1, LD_entry->Name1, 10);
					int len = WideCharToMultiByte(CP_ACP, 0, Wide_Name1, -1, NULL, 0, NULL, NULL);
					name1 = (u_char *)malloc(len);
					WideCharToMultiByte(CP_ACP, 0, Wide_Name1, -1, name1, len, NULL, NULL);
					printf("%s", name1);
					memcpy(Wide_Name2, LD_entry->Name2, 12);
					len = WideCharToMultiByte(CP_ACP, 0, Wide_Name2, -1, NULL, 0, NULL, NULL);
					name2 = (u_char *)malloc(len);
					WideCharToMultiByte(CP_ACP, 0, Wide_Name2, -1, name2, len, NULL, NULL);
					printf("%s", name2);
					memcpy(Wide_Name3, LD_entry->Name3, 4);
					len = WideCharToMultiByte(CP_ACP, 0, Wide_Name3, -1, NULL, 0, NULL, NULL);
					name3 = (u_char *)malloc(len);
					WideCharToMultiByte(CP_ACP, 0, Wide_Name3, -1, name3, len, NULL, NULL);
					printf("%s", name3);
					free(name1);
					free(name2);
					free(name3);
					count--;
					if (count == 0)
					{
						break;
					}
					fseek(fp, -64, SEEK_CUR);
				}	
				fseek(fp, Data_offset + offset, SEEK_SET);
				fread(D_entry, sizeof(struct Long_File_Name_Entry), 1, fp);
				offset += sizeof(struct Long_File_Name_Entry);
				printf("\t File Size : %d\n", D_entry->Filesize);
				
				continue;
			}
			//SFN
			else
			{
				printf("[%c] %s", Delete_File, D_entry->Name);
				printf("\t File Size : %d\n", D_entry->Filesize);
			}
			continue;
		}
		else
			Delete_File = ' ';
		//삭제되지 않은 파일 LFN
		if ((D_entry->Attribute & 0x0F) == 0x0F)
		{
			count = D_entry->Name[0] ^ 0x40;
			fseek(fp, (count - 2) * 32, SEEK_CUR);
			offset += (count-1) * sizeof(struct Long_File_Name_Entry);
			printf("[%c] ", Delete_File);
			while (1)
			{
				u_char *name1;
				u_char *name2;
				u_char *name3;

				fread(LD_entry, sizeof(struct Long_File_Name_Entry), 1, fp);
				memcpy(Wide_Name1, LD_entry->Name1, 10);
				int len = WideCharToMultiByte(CP_ACP, 0, Wide_Name1, -1, NULL, 0, NULL, NULL);
				name1 = (u_char *)malloc(len);
				WideCharToMultiByte(CP_ACP, 0, Wide_Name1, -1, name1, len, NULL, NULL);
				printf("%s", name1);

				memcpy(Wide_Name2, LD_entry->Name2, 12);
				len = WideCharToMultiByte(CP_ACP, 0, Wide_Name2, -1, NULL, 0, NULL, NULL);
				name2 = (u_char *)malloc(len);
				WideCharToMultiByte(CP_ACP, 0, Wide_Name2, -1, name2, len, NULL, NULL);
				printf("%s", name2);
				memcpy(Wide_Name3, LD_entry->Name3, 4);
				len = WideCharToMultiByte(CP_ACP, 0, Wide_Name3, -1, NULL, 0, NULL, NULL);
				name3 = (u_char *)malloc(len);
				WideCharToMultiByte(CP_ACP, 0, Wide_Name3, -1, name3, len, NULL, NULL);
				printf("%s", name3);
				fseek(fp, -64, SEEK_CUR);
				free(name1);
				free(name2);
				free(name3);
				count--;
				if (count == 0)
				{					
					break;
				}
			}
			fseek(fp, Data_offset + offset, SEEK_SET);
			fread(D_entry, sizeof(struct Directory_Entry), 1, fp);
			offset += sizeof(struct Directory_Entry);
			printf("\t  File Size : %d\n", D_entry->Filesize);
			continue;
		}
		// 삭제되지 않는 SFN
		else
		{
			printf("[%c] %8s", Delete_File, D_entry->Name);
			printf("\t File Size : %d\n", D_entry->Filesize);
			continue;
		}
	}
	free(LD_entry);
	free(D_entry);
	free(FAT_Table);
	free(FS_Info);
	free(fat32);
	fclose(fp);
	return 0;
}