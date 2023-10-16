# Build mox worlds
if [ $# -ne 1 ]
then
    echo "Usage: $0 <number of mox worlds>"
    exit 1
fi
n=$1
i=0
while [ $i -lt $n ]
do
    ./mox_world_builder.bat -steps 50 -gridSize 10 10 -liveCellProbability .5 -save cells10x10_$i.txt -minBlueCells 1 -maxBlueCells 3 -randomSeed $i
    i=`expr $i + 1`
done
exit 0


