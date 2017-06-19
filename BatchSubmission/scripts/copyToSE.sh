#! /bin/bash

OUTPUT="ineuteli/analysis/SFrameAnalysis/AnalysisOutputEM"
PNFS_OUTPUT="root://t3dcachedb.psi.ch:1094//pnfs/psi.ch/cms/trivcat/store/user/$OUTPUT"

cd "/scratch/$OUTPUT"
FILES=`ls */*.root`
N=`echo $FILES | wc -w`
i=1

for f in $FILES; do
  echo
  echo ">>> ${i}/${N}: ${f}"
  xrdcp -f $f ${PNFS_OUTPUT}/$f
  let i+=1
done
echo

