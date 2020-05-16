#!/bin/bash

##parameters
interfile="../data/S2Rplus_spec_chr2L_11M_12M.txt"
chrlensfile="../data/dm3_chrm_sizes"
chrom="chr2L"
start=11000000
end=12000000
outfolder="../output"
res=2000
job_prefix="dros"
threads=20
EXE_PATH="./../bin/sBIF"

##command
cmd="$EXE_PATH -i $interfile -c $chrom -l $chrlensfile -s $start -e $end -o $outfolder -r $res -j $job_prefix -p $threads "
echo $cmd
$cmd 
echo "Done."



