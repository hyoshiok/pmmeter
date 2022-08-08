#!/bin/bash
#SBATCH -J streams
#SBATCH --nodes=1
#SBATCH --tasks-per-node=32



NODES=1
CORES_PER_NODE=32
TOTAL_CORES=$((NODES*CORES_PER_NODE))
THREADS=1
PROCESSES=0
PPN=0

date
srun -n $NODES -N $NODES find /mnt/pmem_fsdax0 /mnt/pmem_fsdax1 -name pstream_test_file* -exec rm -f {} +
export KMP_AFFINITY=granularity=core,compact,1


for THREADS in 1 2 4 8 16 32;
do
  PROCESSES=$((TOTAL_CORES/THREADS))
  PPN=$((PROCESSES/NODES))
  echo $NODES "Nodes"
  echo $PROCESSES "MPI processes, each with " $THREADS "OpenMPI threads"
  echo $PPN " MPI processes per node"
  export OMP_NUM_THREADS=$THREADS
  date
  mpirun -n $PROCESSES --ppn $PPN ./distributed_streams 1
  srun -n $NODES -N $NODES find /mnt/pmem_fsdax0 /mnt/pmem_fsdax1 -name pstream_test_file* -exec rm -f {} +

done


