//
//  main.cpp
//  DSDWrite
//
//  Created by leico on 2014/04/05.
//  Copyright (c) 2014年 leico. All rights reserved.
//
//
//
//usage:
//$DSDWrite input output
//
//ex)
//$DSDWrite data.dat sound.dsf

#include <cstdio>
#include <iostream>

long GetFileSize(FILE *fp)
{
	long fsize = 0;
	
	if (fp == NULL) {
		printf("File not Found!\n");
		exit(1);
	}
	
	/* ファイルサイズを調査 */ 
	fseek(fp, 0, SEEK_END); 
	fsize = ftell(fp);

  fseek(fp, 0, SEEK_SET);
	
	return fsize;
}







int main(int argc, const char * argv[])
{
  using namespace std;
  FILE *in, *out;
	
	if(argc != 3){
    cout << "over/less arguments" << endl;
    return 1;
  }

  char dsd_chunk_ID[4]  = {'D', 'S', 'D', ' '};
  long dsd_chunk_size   = 28;
  long dsf_total_size;
  long metadata_pointer = 0;

  char fmt_chunk_ID[4]    = {'f' ,'m', 't', ' '};
  long fmt_chunk_size     = 52;
  int  fmt_version        = 1;
  int  fmt_ID             = 0;
  int  fmt_channel_type   = 1;
  int  fmt_channels       = 1;
  int  fmt_sampling_rate  = 2822400;
  int  fmt_sampling_depth = 1;
  long fmt_total_sample;
  int  fmt_block_size     = 4096;
  int  fmt_reserved       = 0;

  char    data_chunk_ID[4] = {'d', 'a', 't', 'a'};
  long    data_chunk_size;
  u_char *data;

  in = fopen(argv[1], "rb");
  if(in == NULL){
    printf("File Not Found\n");
    return 1;
  }


  data_chunk_size  = GetFileSize(in);
  fmt_total_sample = data_chunk_size / fmt_channels;
  dsf_total_size   = dsd_chunk_size + fmt_chunk_size + data_chunk_size;

  data = (u_char *)calloc(data_chunk_size, sizeof(u_char));
  fread(data, 1, data_chunk_size, in);

  fclose(in);


  out = fopen(argv[2], "wb");
  if(out == NULL){
    printf("File Not Found\n");
    return 1;
  }

  fwrite( dsd_chunk_ID,     1, 4, out);
  fwrite(&dsd_chunk_size,   8, 1, out);
  fwrite(&dsf_total_size,   8, 1, out);
  fwrite(&metadata_pointer, 8, 1, out);
  
  fwrite( fmt_chunk_ID,       1, 4, out);
  fwrite(&fmt_chunk_size,     8, 1, out);
  fwrite(&fmt_version,        4, 1, out);
  fwrite(&fmt_ID,             4, 1, out);
  fwrite(&fmt_channel_type,   4, 1, out);
  fwrite(&fmt_channels,       4, 1, out);
  fwrite(&fmt_sampling_rate,  4, 1, out);
  fwrite(&fmt_sampling_depth, 4, 1, out);
  fwrite(&fmt_total_sample,   8, 1, out);
  fwrite(&fmt_block_size,     4, 1, out);
  fwrite(&fmt_reserved,       4, 1, out);

  fwrite( data_chunk_ID,   1, 4, out);
  fwrite(&data_chunk_size, 8, 1, out);
  fwrite(data, 1, data_chunk_size, out);

  fclose(out);


  return 0;
}

